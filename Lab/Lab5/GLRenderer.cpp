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
	glTranslatef(0.0, -0.1f, 0.0f);
	DrawVasePart1();
	glTranslatef(0.0f, 0.5f, 0.0f);
	DrawVasePart2();
	glTranslatef(0.0f, 0.5f, 0.0f);
	DrawVasePart3();
	glTranslatef(0.0f, 0.5f, 0.0f);
	DrawVasePart4();
	glTranslatef(0.0f, 0.5f, 0.0f);
	DrawVasePart5();
	glTranslatef(0.0f, 0.5f, 0.0f);
	DrawVasePart6();
	glTranslatef(0.0f, 0.5f, 0.0f);
	DrawVasePart7();
	glTranslatef(0.0f, 0.5f, 0.0f);
	DrawVasePart8();
	glTranslatef(0.0f, 0.5f, 0.0f);
	DrawVasePart9();
	glTranslatef(0.0f, 0.5f, 0.0f);
	DrawVasePart10();
	glTranslatef(0.0f, 0.5f, 0.0f);
	DrawVasePart11();
	glTranslatef(0.0f, 0.5f, 0.0f);
	DrawVasePart12();
	glTranslatef(0.0f, 0.5f, 0.0f);
	DrawVasePart13();
	glTranslatef(0.0f, 0.5f, 0.0f);
	DrawVasePart14();
}

void CGLRenderer::DrawVasePart1()
{
}

void CGLRenderer::DrawVasePart2()
{
}

void CGLRenderer::DrawVasePart3()
{
}

void CGLRenderer::DrawVasePart4()
{
}

void CGLRenderer::DrawVasePart5()
{
}

void CGLRenderer::DrawVasePart6()
{
}

void CGLRenderer::DrawVasePart7()
{
}

void CGLRenderer::DrawVasePart8()
{
}

void CGLRenderer::DrawVasePart9()
{
}

void CGLRenderer::DrawVasePart10()
{
}

void CGLRenderer::DrawVasePart11()
{
}

void CGLRenderer::DrawVasePart12()
{
}

void CGLRenderer::DrawVasePart13()
{
}

void CGLRenderer::DrawVasePart14()
{
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
	float light_position[] = { 6.0f, 20.0f, 8.0f, 0.0 };
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
