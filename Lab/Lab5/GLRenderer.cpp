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
	cameraX = 5.4;
	cameraY = 8.5;
	cameraZ = 0.0;
	cameraYaw = 0.0f;
	cameraDistanceFromCoordinateOrigin = sqrt(pow(cameraX, 2) + pow(cameraY, 2));
	cameraPitch = asin(cameraY / cameraDistanceFromCoordinateOrigin) * 180 / M_PI;
	cameraY *= 1.3;
	normalsOn = true;
	redLightOn = true;
	greenLightOn = true;
	blueLightOn = true;
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
	glClearColor(1.0, 1.0, 1.0, 1.0);
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
	glDisable(GL_LIGHTING);
	glLoadIdentity();

	gluLookAt(cameraX, cameraY, cameraZ,
		0.0, 4.0, 0.0,
		0.0, 1.0, 0.0);

	glPushMatrix();
	DrawLights();
	glPopMatrix();

	SetRoomLightning();
	DrawCoordinateLines();
	DrawRoom();
	DrawBase();
	glTranslatef(0.0f, 1.5f, 0.0f);
	DrawCylinder();
	DrawBox();
	DrawVase();

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
	cameraY = 1.3 * cameraDistanceFromCoordinateOrigin * yPos;
	cameraZ = cameraDistanceFromCoordinateOrigin * zPos;
}

void CGLRenderer::StopMovingCamera()
{
	firstMouse = true;
}

void CGLRenderer::SwitchNormals()
{
	normalsOn = !normalsOn;
}

void CGLRenderer::SwitchRedLight()
{
	redLightOn = !redLightOn;
}

void CGLRenderer::SwitchGreenLight()
{
	greenLightOn = !greenLightOn;
}

void CGLRenderer::SwitchBlueLight()
{
	blueLightOn = !blueLightOn;
}

int CGLRenderer::mod(int k, int n) { return ((k %= n) < 0) ? k + n : k; }

void CGLRenderer::DrawBase()
{
	GLMaterial sphereMat;
	sphereMat.SetAmbient(0.7f, 0.7f, 0.7f, 1.0f);
	sphereMat.SetDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
	sphereMat.SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);

	sphereMat.SelectFront();
	DrawHalfSphere(2.0f);
}

void CGLRenderer::DrawHalfSphere(float r)
{
	glBegin(GL_QUAD_STRIP);
	for (float alpha = 0.0f; alpha < 90.0f; alpha++)
	{
		double alphaRadians = alpha * M_PI / 180.0;
		double alphaPlusOneRadians = (alpha + 1.0) * M_PI / 180.0;

		for (float beta = 0.0f; beta <= 360.0f; beta++)
		{
			double betaRadians = beta * M_PI / 180.0;

			float x1 = r * cos(alphaRadians) * cos(betaRadians);
			float y1 = r * sin(alphaRadians);
			float z1 = r * cos(alphaRadians) * sin(betaRadians);

			float normal1X = cos(betaRadians);
			float normal1Y = sin(alphaRadians);
			float normal1Z = sin(betaRadians);

			float x2 = r * cos(alphaPlusOneRadians) * cos(betaRadians);
			float y2 = r * sin(alphaPlusOneRadians);
			float z2 = r * cos(alphaPlusOneRadians) * sin(betaRadians);

			float normal2X = cos(betaRadians);
			float normal2Y = sin(alphaPlusOneRadians);
			float normal2Z = sin(betaRadians);

			glNormal3f(normal1X, normal1Y, normal1Z);
			glVertex3f(x1, y1, z1);

			glNormal3f(normal2X, normal2Y, normal2Z);
			glVertex3f(x2, y2, z2);
		}
	}
	glEnd();
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
	float step = side / resolution;

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

void CGLRenderer::DrawCylinder()
{
	const int cylPoints = 360;
	const float cylAngle = 360.0f / cylPoints;
	const float cylHeight = 2.0f;

	float base[cylPoints * 3];
	float normals[cylPoints * 3];

	float currAngle = 0.0f;
	float r = 1.0f;
	for (int i = 0; i < cylPoints * 3; i += 3)
	{
		base[i] = r * cos(currAngle * M_PI / 180.0); // x
		base[i + 1] = 0.0f; // y
		base[i + 2] = r * sin(currAngle * M_PI / 180.0); // z

		normals[i] = cos(currAngle * M_PI / 180.0); // x
		normals[i + 1] = 0.0f; // y
		normals[i + 2] = sin(currAngle * M_PI / 180.0); // z

		currAngle += cylAngle;
	}

	float sides[2 * cylPoints * 3];
	for (int i = 0; i < cylPoints * 3; i += 3)
	{
		sides[i] = base[i];
		sides[i + 1] = base[i + 1];
		sides[i + 2] = base[i + 2];
		sides[3 * cylPoints + i] = base[i];
		sides[3 * cylPoints + i + 1] = base[i + 1] + cylHeight;
		sides[3 * cylPoints + i + 2] = base[i + 2];
	}

	u_short baseIndices[cylPoints];
	for (int i = 0; i < cylPoints; i++)
		baseIndices[i] = i;

	u_short sidesIndices[2 * cylPoints + 2];
	for (int i = 0; i < 2 * cylPoints; i += 2)
	{
		sidesIndices[i] = i / 2;
		sidesIndices[i + 1] = i / 2 + cylPoints;
	}
	sidesIndices[2 * cylPoints + 2 - 2] = sidesIndices[0];
	sidesIndices[2 * cylPoints + 2 - 1] = sidesIndices[1];

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	GLMaterial cylMat;
	cylMat.SetAmbient(0.7f, 0.7f, 0.7f, 1.0f);
	cylMat.SetDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
	cylMat.SelectFront();
	glNormalPointer(GL_FLOAT, 0, normals);
	glVertexPointer(3, GL_FLOAT, 0, sides);
	glDrawElements(GL_QUAD_STRIP, 2 * cylPoints + 2, GL_UNSIGNED_SHORT, sidesIndices);

	glTranslatef(0.0f, cylHeight, 0.0f);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void CGLRenderer::DrawBox()
{
	GLMaterial boxMat;
	boxMat.SetAmbient(0.7f, 0.7f, 0.7f, 1.0f);
	boxMat.SetDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
	boxMat.SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);
	boxMat.SetEmission(0.0f, 0.0f, 0.0f, 1.0f);

	boxMat.SelectFront();
	float side = 2.3f;
	glBegin(GL_QUADS);
	{
		// bottom side
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(side, 0.0f, side);
		glVertex3f(side, 0.0f, -side);
		glVertex3f(-side, 0.0f, -side);
		glVertex3f(-side, 0.0f, side);

		// left side
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(side, 0.0f, side);
		glVertex3f(side, 1.0f, side);
		glVertex3f(-side, 1.0f, side);
		glVertex3f(-side, 0.0f, side);

		// "front" side
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-side, 0.0f, -side);
		glVertex3f(-side, 0.0f, side);
		glVertex3f(-side, 1.0f, side);
		glVertex3f(-side, 1.0f, -side);

		// right side
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(side, 0.0f, -side);
		glVertex3f(-side, 0.0f, -side);
		glVertex3f(-side, 1.0f, -side);
		glVertex3f(side, 1.0f, -side);

		// back side
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f(side, 0.0f, side);
		glVertex3f(side, 0.0f, -side);
		glVertex3f(side, 1.0f, -side);
		glVertex3f(side, 1.0f, side);

	}
	glEnd();


	// top side
	glTranslatef(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(side, 0.0f, side);
	glVertex3f(side, 0.0f, -side);
	glVertex3f(-side, 0.0f, -side);
	glVertex3f(-side, 0.0f, side);
	glEnd();

}

void CGLRenderer::DrawVase()
{
	float partHeight = 0.4f;
	glTranslatef(0.0, -0.1f, 0.0f);
	float nextR = DrawVasePart1();
	glTranslatef(0.0f, partHeight, 0.0f);
	nextR = DrawVasePart2(nextR);
	glTranslatef(0.0f, partHeight, 0.0f);
	nextR = DrawVasePart3(nextR);
	glTranslatef(0.0f, partHeight, 0.0f);
	nextR = DrawVasePart4(nextR);
	glTranslatef(0.0f, partHeight, 0.0f);
	nextR = DrawVasePart5(nextR);
	glTranslatef(0.0f, partHeight, 0.0f);
	nextR = DrawVasePart6(nextR);
	glTranslatef(0.0f, partHeight, 0.0f);
	nextR = DrawVasePart7(nextR);
	glTranslatef(0.0f, partHeight, 0.0f);
	nextR = DrawVasePart8(nextR);
	glTranslatef(0.0f, partHeight, 0.0f);
	nextR = DrawVasePart9(nextR);
	glTranslatef(0.0f, partHeight, 0.0f);
	nextR = DrawVasePart10(nextR);
	glTranslatef(0.0f, partHeight, 0.0f);
	nextR = DrawVasePart11(nextR);
	glTranslatef(0.0f, partHeight, 0.0f);
	nextR = DrawVasePart12(nextR);
	glTranslatef(0.0f, partHeight, 0.0f);
	nextR = DrawVasePart13(nextR);
	glTranslatef(0.0f, partHeight, 0.0f);
	nextR = DrawVasePart14(nextR);
}

float CGLRenderer::DrawVasePart1()
{
	const u_short quads = 40;
	float fullHeight = 2.0f;
	float partHeight = 0.4f;
	float otherPartHeight = fullHeight - partHeight;
	float r = 1.5f;
	float r1 = otherPartHeight * r / fullHeight; // r1/r == h1/h
	float L = sqrt(pow(fullHeight, 2) + pow(r, 2));
	float L1 = sqrt(pow(partHeight, 2) + pow(r1, 2));
	float ny = r / L;
	float nr = fullHeight / L;
	float ny1 = r1 / L1;
	float nr1 = otherPartHeight / L1;
	GLMaterial partMaterial;
	partMaterial.SetDiffuse(97 / 255.0, 97 / 255.0, 146 / 255.0, 1.0f);
	partMaterial.SetAmbient(97 / 255.0, 97 / 255.0, 146 / 255.0, 1.0f);
	partMaterial.SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);

	GLMaterial lineMaterial;
	lineMaterial.SetDiffuse(0.0f, 1.0f, 0.0f, 1.0f);
	lineMaterial.SetAmbient(0.0f, 1.0f, 0.0f, 1.0f);
	lineMaterial.SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);

	float bottomNormalsStartX[quads];
	float bottomNormalsStartY[quads];
	float bottomNormalsStartZ[quads];
	float bottomNormalsEndX[quads];
	float bottomNormalsEndY[quads];
	float bottomNormalsEndZ[quads];

	float topNormalsStartX[quads];
	float topNormalsStartY[quads];
	float topNormalsStartZ[quads];
	float topNormalsEndX[quads];
	float topNormalsEndY[quads];
	float topNormalsEndZ[quads];

	int normalsIndex = 0;
	partMaterial.SelectFront();
	glBegin(GL_QUAD_STRIP);
	{
		for (float j = 0.0f; j <= 360.0f; j += 360.0f / quads)
		{
			float x1 = r * cos(j * M_PI / 180.0);
			float y1 = 0.0f;
			float z1 = r * sin(j * M_PI / 180.0);

			float normal1X = nr * cos(j * M_PI / 180.0);
			float normal1Y = ny;
			float normal1Z = nr * sin(j * M_PI / 180.0);

			float x2 = r1 * cos(j * M_PI / 180.0);
			float y2 = partHeight;
			float z2 = r1 * sin(j * M_PI / 180.0);

			float normal2X = nr1 * cos(j * M_PI / 180.0);
			float normal2Y = ny1;
			float normal2Z = nr1 * sin(j * M_PI / 180.0);

			glNormal3f(normal1X, normal1Y, normal1Z);
			glVertex3f(x1, y1, z1);

			glNormal3f(normal2X, normal2Y, normal2Z);
			glVertex3f(x2, y2, z2);

			if (normalsIndex < quads)
			{
				bottomNormalsStartX[normalsIndex] = x1;
				bottomNormalsStartY[normalsIndex] = y1;
				bottomNormalsStartZ[normalsIndex] = z1;
				bottomNormalsEndX[normalsIndex] = x1 + normal1X / 4;
				bottomNormalsEndY[normalsIndex] = y1 + normal1Y / 4;
				bottomNormalsEndZ[normalsIndex] = z1 + normal1Z / 4;

				topNormalsStartX[normalsIndex] = x2;
				topNormalsStartY[normalsIndex] = y2;
				topNormalsStartZ[normalsIndex] = z2;
				topNormalsEndX[normalsIndex] = x2 + normal2X / 4;
				topNormalsEndY[normalsIndex] = y2 + normal2Y / 4;
				topNormalsEndZ[normalsIndex] = z2 + normal2Z / 4;
			}

			normalsIndex++;
		}
	}
	glEnd();

	if (normalsOn)
	{
		glBegin(GL_LINES);
		lineMaterial.SelectFront();
		for (int i = 0; i < normalsIndex - 1; i++)
		{
			// bottom normal
			glVertex3f(bottomNormalsStartX[i], bottomNormalsStartY[i], bottomNormalsStartZ[i]);
			glVertex3f(bottomNormalsEndX[i], bottomNormalsEndY[i], bottomNormalsEndZ[i]);

			// top normal
			glVertex3f(topNormalsStartX[i], topNormalsStartY[i], topNormalsStartZ[i]);
			glVertex3f(topNormalsEndX[i], topNormalsEndY[i], topNormalsEndZ[i]);
		}
		glEnd();
	}

	return r1;
}

float CGLRenderer::DrawVasePart2(float nextR)
{
	const u_short quads = 40;
	float fullHeight = 2.0f;
	float partHeight = 0.4f;
	float otherPartHeight = fullHeight - partHeight;
	float r = nextR;
	float r1 = otherPartHeight * r / fullHeight; // r1/r == h1/h
	float L = sqrt(pow(fullHeight, 2) + pow(r, 2));
	float L1 = sqrt(pow(partHeight, 2) + pow(r1, 2));
	float ny = r / L;
	float nr = fullHeight / L;
	float ny1 = r1 / L1;
	float nr1 = otherPartHeight / L1;
	GLMaterial partMaterial;
	partMaterial.SetDiffuse(67 / 255.0, 67 / 255.0, 144 / 255.0, 1.0f);
	partMaterial.SetAmbient(67 / 255.0, 67 / 255.0, 144 / 255.0, 1.0f);
	partMaterial.SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);

	GLMaterial lineMaterial;
	lineMaterial.SetDiffuse(0.0f, 1.0f, 0.0f, 1.0f);
	lineMaterial.SetAmbient(0.0f, 1.0f, 0.0f, 1.0f);
	lineMaterial.SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);

	float bottomNormalsStartX[quads];
	float bottomNormalsStartY[quads];
	float bottomNormalsStartZ[quads];
	float bottomNormalsEndX[quads];
	float bottomNormalsEndY[quads];
	float bottomNormalsEndZ[quads];

	float topNormalsStartX[quads];
	float topNormalsStartY[quads];
	float topNormalsStartZ[quads];
	float topNormalsEndX[quads];
	float topNormalsEndY[quads];
	float topNormalsEndZ[quads];

	int normalsIndex = 0;
	partMaterial.SelectFront();
	glBegin(GL_QUAD_STRIP);
	{
		for (float j = 0.0f; j <= 360.0f; j += 360.0f / quads)
		{
			float x1 = r * cos(j * M_PI / 180.0);
			float y1 = 0.0f;
			float z1 = r * sin(j * M_PI / 180.0);

			float normal1X = nr * cos(j * M_PI / 180.0);
			float normal1Y = ny;
			float normal1Z = nr * sin(j * M_PI / 180.0);

			float x2 = r1 * cos(j * M_PI / 180.0);
			float y2 = partHeight;
			float z2 = r1 * sin(j * M_PI / 180.0);

			float normal2X = nr1 * cos(j * M_PI / 180.0);
			float normal2Y = ny1;
			float normal2Z = nr1 * sin(j * M_PI / 180.0);

			glNormal3f(normal1X, normal1Y, normal1Z);
			glVertex3f(x1, y1, z1);

			glNormal3f(normal2X, normal2Y, normal2Z);
			glVertex3f(x2, y2, z2);

			if (normalsIndex < quads)
			{
				bottomNormalsStartX[normalsIndex] = x1;
				bottomNormalsStartY[normalsIndex] = y1;
				bottomNormalsStartZ[normalsIndex] = z1;
				bottomNormalsEndX[normalsIndex] = x1 + normal1X / 4;
				bottomNormalsEndY[normalsIndex] = y1 + normal1Y / 4;
				bottomNormalsEndZ[normalsIndex] = z1 + normal1Z / 4;

				topNormalsStartX[normalsIndex] = x2;
				topNormalsStartY[normalsIndex] = y2;
				topNormalsStartZ[normalsIndex] = z2;
				topNormalsEndX[normalsIndex] = x2 + normal2X / 4;
				topNormalsEndY[normalsIndex] = y2 + normal2Y / 4;
				topNormalsEndZ[normalsIndex] = z2 + normal2Z / 4;
			}

			normalsIndex++;
		}
	}
	glEnd();

	if (normalsOn)
	{
		glBegin(GL_LINES);
		lineMaterial.SelectFront();
		for (int i = 0; i < normalsIndex - 1; i++)
		{
			// bottom normal
			glVertex3f(bottomNormalsStartX[i], bottomNormalsStartY[i], bottomNormalsStartZ[i]);
			glVertex3f(bottomNormalsEndX[i], bottomNormalsEndY[i], bottomNormalsEndZ[i]);

			// top normal
			glVertex3f(topNormalsStartX[i], topNormalsStartY[i], topNormalsStartZ[i]);
			glVertex3f(topNormalsEndX[i], topNormalsEndY[i], topNormalsEndZ[i]);
		}
		glEnd();
	}

	return r1;
}

float CGLRenderer::DrawVasePart3(float nextR) // cylinder
{
	const u_short quads = 40;
	float partHeight = 0.4f;
	float r = nextR;
	float r1 = nextR;

	GLMaterial partMaterial;
	partMaterial.SetDiffuse(97 / 255.0, 97 / 255.0, 146 / 255.0, 1.0f);
	partMaterial.SetAmbient(97 / 255.0, 97 / 255.0, 146 / 255.0, 1.0f);
	partMaterial.SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);

	GLMaterial lineMaterial;
	lineMaterial.SetDiffuse(0.0f, 1.0f, 0.0f, 1.0f);
	lineMaterial.SetAmbient(0.0f, 1.0f, 0.0f, 1.0f);
	lineMaterial.SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);

	float bottomNormalsStartX[quads];
	float bottomNormalsStartY[quads];
	float bottomNormalsStartZ[quads];
	float bottomNormalsEndX[quads];
	float bottomNormalsEndY[quads];
	float bottomNormalsEndZ[quads];

	float topNormalsStartX[quads];
	float topNormalsStartY[quads];
	float topNormalsStartZ[quads];
	float topNormalsEndX[quads];
	float topNormalsEndY[quads];
	float topNormalsEndZ[quads];

	int normalsIndex = 0;
	partMaterial.SelectFront();
	glBegin(GL_QUAD_STRIP);
	{
		for (float j = 0.0f; j <= 360.0f; j += 360.0f / quads)
		{
			float x1 = r * cos(j * M_PI / 180.0);
			float y1 = 0.0f;
			float z1 = r * sin(j * M_PI / 180.0);

			float normal1X = cos(j * M_PI / 180.0);
			float normal1Y = 0.0f;
			float normal1Z = sin(j * M_PI / 180.0);

			float x2 = r1 * cos(j * M_PI / 180.0);
			float y2 = partHeight;
			float z2 = r1 * sin(j * M_PI / 180.0);

			float normal2X = cos(j * M_PI / 180.0);
			float normal2Y = 0.0f;
			float normal2Z = sin(j * M_PI / 180.0);

			glNormal3f(normal1X, normal1Y, normal1Z);
			glVertex3f(x1, y1, z1);

			glNormal3f(normal2X, normal2Y, normal2Z);
			glVertex3f(x2, y2, z2);

			if (normalsIndex < quads)
			{
				bottomNormalsStartX[normalsIndex] = x1;
				bottomNormalsStartY[normalsIndex] = y1;
				bottomNormalsStartZ[normalsIndex] = z1;
				bottomNormalsEndX[normalsIndex] = x1 + normal1X / 4;
				bottomNormalsEndY[normalsIndex] = y1 + normal1Y / 4;
				bottomNormalsEndZ[normalsIndex] = z1 + normal1Z / 4;

				topNormalsStartX[normalsIndex] = x2;
				topNormalsStartY[normalsIndex] = y2;
				topNormalsStartZ[normalsIndex] = z2;
				topNormalsEndX[normalsIndex] = x2 + normal2X / 4;
				topNormalsEndY[normalsIndex] = y2 + normal2Y / 4;
				topNormalsEndZ[normalsIndex] = z2 + normal2Z / 4;
			}

			normalsIndex++;
		}
	}
	glEnd();

	if (normalsOn)
	{
		glBegin(GL_LINES);
		lineMaterial.SelectFront();
		for (int i = 0; i < normalsIndex - 1; i++)
		{
			// bottom normal
			glVertex3f(bottomNormalsStartX[i], bottomNormalsStartY[i], bottomNormalsStartZ[i]);
			glVertex3f(bottomNormalsEndX[i], bottomNormalsEndY[i], bottomNormalsEndZ[i]);

			// top normal
			glVertex3f(topNormalsStartX[i], topNormalsStartY[i], topNormalsStartZ[i]);
			glVertex3f(topNormalsEndX[i], topNormalsEndY[i], topNormalsEndZ[i]);
		}
		glEnd();
	}

	return r1;
}

float CGLRenderer::DrawVasePart4(float nextR)
{
	const u_short quads = 40;
	float partHeight = 0.4f;
	float r = nextR;
	float r1 = nextR;

	GLMaterial partMaterial;
	partMaterial.SetDiffuse(67 / 255.0, 67 / 255.0, 144 / 255.0, 1.0f);
	partMaterial.SetAmbient(67 / 255.0, 67 / 255.0, 144 / 255.0, 1.0f);
	partMaterial.SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);

	GLMaterial lineMaterial;
	lineMaterial.SetDiffuse(0.0f, 1.0f, 0.0f, 1.0f);
	lineMaterial.SetAmbient(0.0f, 1.0f, 0.0f, 1.0f);
	lineMaterial.SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);

	float bottomNormalsStartX[quads];
	float bottomNormalsStartY[quads];
	float bottomNormalsStartZ[quads];
	float bottomNormalsEndX[quads];
	float bottomNormalsEndY[quads];
	float bottomNormalsEndZ[quads];

	float topNormalsStartX[quads];
	float topNormalsStartY[quads];
	float topNormalsStartZ[quads];
	float topNormalsEndX[quads];
	float topNormalsEndY[quads];
	float topNormalsEndZ[quads];

	int normalsIndex = 0;
	partMaterial.SelectFront();
	glBegin(GL_QUAD_STRIP);
	{
		for (float j = 0.0f; j <= 360.0f; j += 360.0f / quads)
		{
			float x1 = r * cos(j * M_PI / 180.0);
			float y1 = 0.0f;
			float z1 = r * sin(j * M_PI / 180.0);

			float normal1X = cos(j * M_PI / 180.0);
			float normal1Y = 0.0f;
			float normal1Z = sin(j * M_PI / 180.0);

			float x2 = r1 * cos(j * M_PI / 180.0);
			float y2 = partHeight;
			float z2 = r1 * sin(j * M_PI / 180.0);

			float normal2X = cos(j * M_PI / 180.0);
			float normal2Y = 0.0f;
			float normal2Z = sin(j * M_PI / 180.0);

			glNormal3f(normal1X, normal1Y, normal1Z);
			glVertex3f(x1, y1, z1);

			glNormal3f(normal2X, normal2Y, normal2Z);
			glVertex3f(x2, y2, z2);

			if (normalsIndex < quads)
			{
				bottomNormalsStartX[normalsIndex] = x1;
				bottomNormalsStartY[normalsIndex] = y1;
				bottomNormalsStartZ[normalsIndex] = z1;
				bottomNormalsEndX[normalsIndex] = x1 + normal1X / 4;
				bottomNormalsEndY[normalsIndex] = y1 + normal1Y / 4;
				bottomNormalsEndZ[normalsIndex] = z1 + normal1Z / 4;

				topNormalsStartX[normalsIndex] = x2;
				topNormalsStartY[normalsIndex] = y2;
				topNormalsStartZ[normalsIndex] = z2;
				topNormalsEndX[normalsIndex] = x2 + normal2X / 4;
				topNormalsEndY[normalsIndex] = y2 + normal2Y / 4;
				topNormalsEndZ[normalsIndex] = z2 + normal2Z / 4;
			}

			normalsIndex++;
		}
	}
	glEnd();

	if (normalsOn)
	{
		glBegin(GL_LINES);
		lineMaterial.SelectFront();
		for (int i = 0; i < normalsIndex - 1; i++)
		{
			// bottom normal
			glVertex3f(bottomNormalsStartX[i], bottomNormalsStartY[i], bottomNormalsStartZ[i]);
			glVertex3f(bottomNormalsEndX[i], bottomNormalsEndY[i], bottomNormalsEndZ[i]);

			// top normal
			glVertex3f(topNormalsStartX[i], topNormalsStartY[i], topNormalsStartZ[i]);
			glVertex3f(topNormalsEndX[i], topNormalsEndY[i], topNormalsEndZ[i]);
		}
		glEnd();
	}

	return r1;
}

float CGLRenderer::DrawVasePart5(float nextR)
{
	return nextR;

}

float CGLRenderer::DrawVasePart6(float nextR)
{
	return nextR;

}

float CGLRenderer::DrawVasePart7(float nextR)
{
	return nextR;

}

float CGLRenderer::DrawVasePart8(float nextR)
{
	return nextR;

}

float CGLRenderer::DrawVasePart9(float nextR)
{
	return nextR;

}

float CGLRenderer::DrawVasePart10(float nextR)
{
	return nextR;

}

float CGLRenderer::DrawVasePart11(float nextR)
{
	return nextR;

}

float CGLRenderer::DrawVasePart12(float nextR)
{
	return nextR;

}

float CGLRenderer::DrawVasePart13(float nextR)
{
	return nextR;

}

float CGLRenderer::DrawVasePart14(float nextR)
{
	return nextR;

}

void CGLRenderer::DrawLights()
{
	glEnable(GL_LIGHTING);
	if (redLightOn)
	{
		GLMaterial redLight;
		redLight.SetEmission(1.0f, 0.0f, 0.0f, 1.0f);
		redLight.SetAmbient(0.0f, 0.0f, 0.0f, 1.0f);
		redLight.SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);
		redLight.SetDiffuse(0.0f, 0.0f, 0.0f, 1.0f);
		redLight.SelectFront();

		GLfloat spot_direction[] = { 0.0f, 0.0f, -1.0f };
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);

		glPushMatrix();
		glTranslatef(0.0f, 10.0f, 9.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		float light_ambient[] = { 0.1, 0.0, 0.0, 1.0 };
		DrawHalfSphere(.4f);
		float light_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
		float light_emission[] = { 1.0, 0.0, 0.0, 1.0 };
		float light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0 };
		float light_position[] = { 0.0f, 0.0f, 0.0f, 1.0 };
		glLightfv(GL_LIGHT1, GL_POSITION, light_position);
		glLightfv(GL_LIGHT1, GL_EMISSION, light_emission);
		glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25);

		glEnable(GL_LIGHT1);
		glPopMatrix();
	}
	else
		glDisable(GL_LIGHT1);
	if (greenLightOn)
	{
		GLMaterial greenLight;
		greenLight.SetEmission(0.0f, 1.0f, 0.0f, 1.0f);
		greenLight.SetAmbient(0.0f, 0.0f, 0.0f, 1.0f);
		greenLight.SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);
		greenLight.SetDiffuse(0.0f, 0.0f, 0.0f, 1.0f);
		greenLight.SelectFront();

		GLfloat spot_direction[] = { 0.0f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);

		glPushMatrix();
		glTranslatef(0.0f, 10.0f, -9.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		DrawHalfSphere(.4f);
		float light_ambient[] = { 0.0, 0.1, 0.0, 1.0 };
		float light_diffuse[] = { 0.0, 1.0, 0.0, 1.0 };
		float light_emission[] = { 0.0, 1.0, 0.0, 1.0 };
		float light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0 };
		float light_position[] = { 0.0f, 0.0f, 0.0f, 1.0 };
		glLightfv(GL_LIGHT2, GL_POSITION, light_position);
		glLightfv(GL_LIGHT2, GL_EMISSION, light_emission);
		glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 25);

		glEnable(GL_LIGHT2);
		glPopMatrix();
	}
	else
		glDisable(GL_LIGHT2);
	if (blueLightOn)
	{
		GLMaterial blueLight;
		blueLight.SetEmission(0.0f, 0.0f, 1.0f, 1.0f);
		blueLight.SetAmbient(0.0f, 0.0f, 0.0f, 1.0f);
		blueLight.SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);
		blueLight.SetDiffuse(0.0f, 0.0f, 0.0f, 1.0f);
		blueLight.SelectFront();

		GLfloat spot_direction[] = { 0.0f, -1.0f, 0.0f };
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);
		glPushMatrix();
		glTranslatef(0.0f, 20.0f, 0.0f);
		glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
		DrawHalfSphere(.4f);
		float light_ambient[] = { 0.0, 0.0, 0.1, 1.0 };
		float light_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
		float light_emission[] = { 0.0, 0.0, 1.0, 1.0 };
		float light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0 };
		float light_position[] = { 0.0f, 0.0f, 0.0f, 1.0 };
		glLightfv(GL_LIGHT3, GL_POSITION, light_position);
		glLightfv(GL_LIGHT3, GL_EMISSION, light_emission);
		glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);
		glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 25);

		glEnable(GL_LIGHT3);
		glPopMatrix();
	}
	else
		glDisable(GL_LIGHT3);
}

void CGLRenderer::SetRoomLightning()
{
	GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	float light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	float light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_emission[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0 };
	float light_position[] = { 6.0f, 10.0f, 8.0f, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_EMISSION, light_emission);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//GLfloat spot_direction[] = { -1.0f, -1.0f, -1.0f };
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45);

	glEnable(GL_LIGHT0);
}

void CGLRenderer::DrawCoordinateLines()
{
	glLineWidth(2.0);
	GLMaterial xMat, yMat, zMat;
	xMat.SetDiffuse(1.0f, 0.0f, 0.0f, 1.0f);
	xMat.SetAmbient(1.0f, 0.0f, 0.0f, 1.0f);
	xMat.SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);
	yMat.SetDiffuse(0.0f, 1.0f, 0.0f, 1.0f);
	yMat.SetAmbient(0.0f, 1.0f, 0.0f, 1.0f);
	yMat.SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);
	zMat.SetDiffuse(0.0f, 0.0f, 1.0f, 1.0f);
	zMat.SetAmbient(0.0f, 0.0f, 1.0f, 1.0f);
	zMat.SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);
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
