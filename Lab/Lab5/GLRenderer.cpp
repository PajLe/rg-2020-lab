#include "pch.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES
#include <math.h>
#include "GLMaterial.h"

CGLRenderer::CGLRenderer()
{
	firstMouse = true;
	cameraX = 5.0;
	cameraY = 8.5;
	cameraZ = 0.0;
	cameraYaw = 0.0f;
	cameraDistanceFromCoordinateOrigin = sqrt(pow(cameraX, 2) + pow(cameraY, 2));
	cameraPitch = asin(cameraY / cameraDistanceFromCoordinateOrigin) * 180 / M_PI;
	rotateX = 0.0f;
	rotateY = 0.0f;
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1.0, 1.0, 1.0, 0.0);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();

	gluLookAt(cameraX, cameraY, cameraZ,
		0.0, 4.0, 0.0,
		0.0, 1.0, 0.0);

	SetRoomLightning();
	DrawCoordinateLines();
	DrawRoom();

	glFlush();
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	double aspect = (double)w / (double)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::MoveCamera(CPoint cursorPoint) // https://learnopengl.com/Getting-started/Camera
{
	if (firstMouse)
	{
		lastPoint.x = cursorPoint.x;
		lastPoint.y = cursorPoint.y;
		firstMouse = false;
	}

	float xoffset = lastPoint.x - cursorPoint.x; // moving left moves camera to the right
	float yoffset = cursorPoint.y - lastPoint.y; // moving up moves camera down (mouse y grows going down)
	lastPoint.x = cursorPoint.x;
	lastPoint.y = cursorPoint.y;

	float sensitivity = 0.5f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	cameraYaw += xoffset;
	cameraPitch += yoffset;

	if (cameraPitch > 89.0f)
		cameraPitch = 89.0f;
	if (cameraPitch < -89.0f)
		cameraPitch = -89.0f;

	double xPos = cos(cameraYaw * M_PI / 180.0) * cos(cameraPitch * M_PI / 180.0);
	double yPos = sin(cameraPitch * M_PI / 180.0);
	double zPos = -sin(cameraYaw * M_PI / 180.0) * cos(cameraPitch * M_PI / 180.0);
	cameraX = cameraDistanceFromCoordinateOrigin * xPos;
	cameraY = cameraDistanceFromCoordinateOrigin * yPos;
	cameraZ = cameraDistanceFromCoordinateOrigin * zPos;
}

void CGLRenderer::StopMovingCamera()
{
	firstMouse = true;
}

void CGLRenderer::RotateX(float angle)
{
	rotateX += angle;
	rotateX = mod(rotateX, 360);
}

void CGLRenderer::RotateY(float angle)
{
	rotateY += angle;
	rotateY = mod(rotateY, 360);
}

int CGLRenderer::mod(int k, int n) { return ((k %= n) < 0) ? k + n : k; }

void CGLRenderer::DrawSphere(float r)
{
	const u_short pointsPerAlpha = 360;
	const u_char alphaRange = 180;

	u_int* sphereIndices = new u_int[(pointsPerAlpha * 2 + 2) * (alphaRange - 1)];
	u_int alphaIndicesArrayPos = 0;
	for (char alpha = -90; alphaIndicesArrayPos < (pointsPerAlpha * 2 + 2) * (alphaRange - 1); alpha++)
	{
		u_short alphaPoint = (alpha + 90) * pointsPerAlpha;
		u_short alphaPointNext = (alpha + 1 + 90) * pointsPerAlpha;

		u_short beta;
		for (beta = 0; beta < pointsPerAlpha * 2; beta += 2)
		{
			sphereIndices[alphaIndicesArrayPos + beta] = alphaPoint + beta / 2;
			sphereIndices[alphaIndicesArrayPos + beta + 1] = alphaPointNext + beta / 2;
		}
		sphereIndices[alphaIndicesArrayPos + beta] = sphereIndices[alphaIndicesArrayPos];
		sphereIndices[alphaIndicesArrayPos + beta + 1] = sphereIndices[alphaIndicesArrayPos + 1];

		alphaIndicesArrayPos += pointsPerAlpha * 2 + 2;
	}
	
	float* spherePoints = new float[3 * pointsPerAlpha * alphaRange];
	float* normalsPoints = new float[3 * pointsPerAlpha * alphaRange];

	for (char alpha = -90; alpha < 90; alpha++)
	{
		double alphaRadians = alpha * M_PI / 180.0;
		u_int alphaArrayPos = (alpha + 90) * pointsPerAlpha * 3;

		for (u_short beta = 0; beta < 3 * pointsPerAlpha; beta += 3)
		{
			double betaRadians = (beta / 3) * M_PI / 180.0;

			float x1 = r * cos(alphaRadians) * cos(betaRadians);
			float y1 = r * sin(alphaRadians);
			float z1 = r * cos(alphaRadians) * sin(betaRadians);

			float normal1X = cos(betaRadians);
			float normal1Y = sin(alphaRadians);
			float normal1Z = sin(betaRadians);

			spherePoints[alphaArrayPos + beta] = x1;
			spherePoints[alphaArrayPos + beta + 1] = y1;
			spherePoints[alphaArrayPos + beta + 2] = z1;
			
			normalsPoints[alphaArrayPos + beta] = normal1X;
			normalsPoints[alphaArrayPos + beta + 1] = normal1Y;
			normalsPoints[alphaArrayPos + beta + 2] = normal1Z;
		}
	}

	glColor4f(0 / 255.0, 188 / 255.0, 0 / 255.0, 0.0f);
	glVertexPointer(3, GL_FLOAT, 0, spherePoints);
	glNormalPointer(GL_FLOAT, 0, normalsPoints);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glDrawElements(GL_QUAD_STRIP, (pointsPerAlpha * 2 + 2) * (alphaRange - 1), GL_UNSIGNED_INT, sphereIndices);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	delete[] normalsPoints;
	delete[] spherePoints;
	delete[] sphereIndices;
}

void CGLRenderer::DrawRoom()
{
	DrawLeftWall();
	DrawFrontWall();
	DrawRightWall();
	DrawBackWall();
	DrawTopWall();
	DrawBottomWall();
	/*const u_char cubePoints = 8;
	float height = 20.f;
	
	float vertices[cubePoints * 3];
	u_char indices[cubePoints * 3];

	vertices[0] = -height / 2; vertices[1] = 0; vertices[2] = height / 2;
	vertices[3] = height / 2; vertices[4] = 0; vertices[5] = height / 2;
	vertices[6] = height / 2; vertices[7] = height; vertices[8] = height / 2;
	vertices[9] = -height / 2; vertices[10] = height; vertices[11] = height / 2;
	vertices[12] = -height / 2; vertices[13] = 0; vertices[14] = -height / 2;
	vertices[15] = height / 2; vertices[16] = 0; vertices[17] = -height / 2;
	vertices[18] = height / 2; vertices[19] = height; vertices[20] = -height / 2;
	vertices[21] = -height / 2; vertices[22] = height; vertices[23] = -height / 2;

	indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 3;
	indices[4] = 1; indices[5] = 5; indices[6] = 6; indices[7] = 2;
	indices[8] = 7; indices[9] = 6; indices[10] = 5; indices[11] = 4;
	indices[12] = 0; indices[13] = 3; indices[14] = 7; indices[15] = 4;
	indices[16] = 7; indices[17] = 3; indices[18] = 2; indices[19] = 6;
	indices[20] = 0; indices[21] = 4; indices[22] = 5; indices[23] = 1;

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices);

	glDisableClientState(GL_VERTEX_ARRAY);*/
}

void CGLRenderer::DrawLeftWall()
{
	u_short resolution = 100;
}

void CGLRenderer::DrawFrontWall()
{
}

void CGLRenderer::DrawRightWall()
{
}

void CGLRenderer::DrawBackWall()
{
}

void CGLRenderer::DrawTopWall()
{
}

void CGLRenderer::DrawBottomWall()
{
}

void CGLRenderer::SetRoomLightning()
{
	GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	float light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_specular[] = { 0.1, 0.1, 0.1, 1.0 };
	float light_position[] = { 10.0, 20.0, 9.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	GLfloat spot_direction[] = { 0.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void CGLRenderer::DrawCoordinateLines()
{
	glLineWidth(2.0);
	GLMaterial xMat, yMat, zMat;
	xMat.SetDiffuse(1.0f, 0.0f, 0.0f, 1.0f);
	xMat.SetAmbient(1.0f, 0.0f, 0.0f, 1.0f);
	yMat.SetDiffuse(0.0f, 1.0f, 0.0f, 1.0f);
	yMat.SetAmbient(0.0f, 1.0f, 0.0f, 1.0f);
	zMat.SetDiffuse(0.0f, 0.0f, 1.0f, 1.0f);
	zMat.SetAmbient(0.0f, 0.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	{
		xMat.Select();
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(10.0, 0.0, 0.0);

		yMat.Select();
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 10.0, 0.0);

		zMat.Select();
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 10.0);
	}
	glEnd();
}
