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
}

void CGLRenderer::DrawLeftWall()
{
	float side = 20.0f;
	u_short resolution = 100;
	float topLeftX = 10.0f;
	float topLeftY = 20.0f;
	float topLeftZ = 10.0f;
	GLMaterial wallMat;
	wallMat.SetAmbient(0.8f, 0.8f, 0.8f, 1.0f);
	wallMat.SetDiffuse(0.8f, 0.8f, 0.8f, 1.0f);

	float step = side / resolution;

	wallMat.SelectFront();
	glBegin(GL_QUADS);
	{
		for (u_short i = 0; i < resolution; i++)
		{
			for (u_short j = 0; j < resolution; j++)
			{
				glNormal3f(0.0f, 0.0f, -1.0f);
				glVertex3f(topLeftX - step * j, topLeftY - step * i, topLeftZ);
				glNormal3f(0.0f, 0.0f, -1.0f);
				glVertex3f(topLeftX - step * (j + 1), topLeftY - step * i, topLeftZ);
				glNormal3f(0.0f, 0.0f, -1.0f);
				glVertex3f(topLeftX - step * (j + 1), topLeftY - step * (i + 1), topLeftZ);
				glNormal3f(0.0f, 0.0f, -1.0f);
				glVertex3f(topLeftX - step * j, topLeftY - step * (i + 1), topLeftZ);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawFrontWall()
{
	float side = 20.0f;
	u_short resolution = 100;
	float topLeftX = -10.0f;
	float topLeftY = 20.0f;
	float topLeftZ = 10.0f;
	GLMaterial wallMat;
	wallMat.SetAmbient(0.8f, 0.8f, 0.8f, 1.0f);
	wallMat.SetDiffuse(0.8f, 0.8f, 0.8f, 1.0f);

	float step = side / resolution;

	wallMat.SelectFront();
	glBegin(GL_QUADS);
	{
		for (u_short i = 0; i < resolution; i++)
		{
			for (u_short j = 0; j < resolution; j++)
			{
				glNormal3f(1.0f, 0.0f, 0.0f);
				glVertex3f(topLeftX, topLeftY - step * i, topLeftZ - step * j);
				glNormal3f(1.0f, 0.0f, 0.0f);
				glVertex3f(topLeftX, topLeftY - step * i, topLeftZ - step * (j + 1));
				glNormal3f(1.0f, 0.0f, 0.0f);
				glVertex3f(topLeftX, topLeftY - step * (i + 1), topLeftZ - step * (j + 1));
				glNormal3f(1.0f, 0.0f, 0.0f);
				glVertex3f(topLeftX, topLeftY - step * (i + 1), topLeftZ - step * j);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawRightWall()
{
	float side = 20.0f;
	u_short resolution = 100;
	float topLeftX = -10.0f;
	float topLeftY = 20.0f;
	float topLeftZ = -10.0f;
	GLMaterial wallMat;
	wallMat.SetAmbient(0.8f, 0.8f, 0.8f, 1.0f);
	wallMat.SetDiffuse(0.8f, 0.8f, 0.8f, 1.0f);

	float step = side / resolution;

	wallMat.SelectFront();
	glBegin(GL_QUADS);
	{
		for (u_short i = 0; i < resolution; i++)
		{
			for (u_short j = 0; j < resolution; j++)
			{
				glNormal3f(0.0f, 0.0f, 1.0f);
				glVertex3f(topLeftX + step * j, topLeftY - step * i, topLeftZ);
				glNormal3f(0.0f, 0.0f, 1.0f);
				glVertex3f(topLeftX + step * (j + 1), topLeftY - step * i, topLeftZ);
				glNormal3f(0.0f, 0.0f, 1.0f);
				glVertex3f(topLeftX + step * (j + 1), topLeftY - step * (i + 1), topLeftZ);
				glNormal3f(0.0f, 0.0f, 1.0f);
				glVertex3f(topLeftX + step * j, topLeftY - step * (i + 1), topLeftZ);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawBackWall()
{
	float side = 20.0f;
	u_short resolution = 100;
	float topLeftX = 10.0f;
	float topLeftY = 20.0f;
	float topLeftZ = -10.0f;
	GLMaterial wallMat;
	wallMat.SetAmbient(0.8f, 0.8f, 0.8f, 1.0f);
	wallMat.SetDiffuse(0.8f, 0.8f, 0.8f, 1.0f);

	float step = side / resolution;

	wallMat.SelectFront();
	glBegin(GL_QUADS);
	{
		for (u_short i = 0; i < resolution; i++)
		{
			for (u_short j = 0; j < resolution; j++)
			{
				glNormal3f(-1.0f, 0.0f, 0.0f);
				glVertex3f(topLeftX, topLeftY - step * i, topLeftZ + step * j);
				glNormal3f(-1.0f, 0.0f, 0.0f);
				glVertex3f(topLeftX, topLeftY - step * i, topLeftZ + step * (j + 1));
				glNormal3f(-1.0f, 0.0f, 0.0f);
				glVertex3f(topLeftX, topLeftY - step * (i + 1), topLeftZ + step * (j + 1));
				glNormal3f(-1.0f, 0.0f, 0.0f);
				glVertex3f(topLeftX, topLeftY - step * (i + 1), topLeftZ + step * j);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawTopWall()
{
	float side = 20.0f;
	u_short resolution = 100;
	float topLeftX = 10.0f;
	float topLeftY = 20.0f;
	float topLeftZ = 10.0f;
	GLMaterial wallMat;
	wallMat.SetAmbient(0.3f, 0.3f, 0.3f, 1.0f);
	wallMat.SetDiffuse(0.3f, 0.3f, 0.3f, 1.0f);

	float step = side / resolution;

	wallMat.SelectFront();
	glBegin(GL_QUADS);
	{
		for (u_short i = 0; i < resolution; i++)
		{
			for (u_short j = 0; j < resolution; j++)
			{
				glNormal3f(0.0f, -1.0f, 0.0f);
				glVertex3f(topLeftX - step * i, topLeftY, topLeftZ - step * j);
				glNormal3f(0.0f, -1.0f, 0.0f);
				glVertex3f(topLeftX - step * i, topLeftY, topLeftZ - step * (j + 1));
				glNormal3f(0.0f, -1.0f, 0.0f);
				glVertex3f(topLeftX - step * (i + 1), topLeftY, topLeftZ - step * (j + 1));
				glNormal3f(0.0f, -1.0f, 0.0f);
				glVertex3f(topLeftX - step * (i + 1), topLeftY, topLeftZ - step * j);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawBottomWall()
{
	float side = 20.0f;
	u_short resolution = 100;
	float topLeftX = -10.0f;
	float topLeftY = 0.0f;
	float topLeftZ = 10.0f;
	GLMaterial wallMat;
	wallMat.SetAmbient(0.7f, 0.7f, 0.7f, 1.0f);
	wallMat.SetDiffuse(0.7f, 0.7f, 0.7f, 1.0f);

	GLMaterial transparentBack;
	transparentBack.SetDiffuse(0.0f, 0.0f, 0.0f, 0.0f);

	float step = side / resolution;

	transparentBack.SelectBack();
	wallMat.SelectFront();
	glBegin(GL_QUADS);
	{
		for (u_short i = 0; i < resolution; i++)
		{
			for (u_short j = 0; j < resolution; j++)
			{
				glNormal3f(0.0f, 1.0f, 0.0f);
				glVertex3f(topLeftX + step * i, topLeftY, topLeftZ - step * j);
				glNormal3f(0.0f, 1.0f, 0.0f);
				glVertex3f(topLeftX + step * i, topLeftY, topLeftZ - step * (j + 1));
				glNormal3f(0.0f, 1.0f, 0.0f);
				glVertex3f(topLeftX + step * (i + 1), topLeftY, topLeftZ - step * (j + 1));
				glNormal3f(0.0f, 1.0f, 0.0f);
				glVertex3f(topLeftX + step * (i + 1), topLeftY, topLeftZ - step * j);
			}
		}
	}
	glEnd();
}

void CGLRenderer::SetRoomLightning()
{
	GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	float light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	float light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_position[] = { 6.0f, 20.0f, 8.0f, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	GLfloat spot_direction[] = { -1.0f, -1.0f, -1.0f };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45);

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
		xMat.SelectFront();
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(10.0, 0.0, 0.0);

		yMat.SelectFront();
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 10.0, 0.0);

		zMat.SelectFront();
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 10.0);
	}
	glEnd();
}
