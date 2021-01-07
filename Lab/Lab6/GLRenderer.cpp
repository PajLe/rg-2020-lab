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
	// camera info
	firstMouse = true;
	cameraX = 5.4;
	cameraY = 10.5;
	cameraZ = 0.0;
	cameraYaw = 0.0f;
	cameraDistanceFromCoordinateOrigin = sqrt(pow(cameraX, 2) + pow(cameraY, 2));
	cameraPitch = asin(cameraY / cameraDistanceFromCoordinateOrigin) * 180 / M_PI;
	cameraY *= 1.4;
	cameraX *= 0.9;

	// textures
	truck = new GLTexture();
	terrain = new GLTexture();

	// materials
	truckMat = new GLMaterial();
	truckMat->SetSpecular(0.0f, 0.0f, 0.0f, 1.0f);
	truckMat->SetAmbient(161 / 255.0, 161 / 255.0, 102 / 255.0, 1.0f);
	truckMat->SetDiffuse(161 / 255.0, 161 / 255.0, 102 / 255.0, 1.0f);
}

CGLRenderer::~CGLRenderer()
{
	// textures
	truck->Release();
	delete truck;
	terrain->Release();
	delete terrain;

	// materials
	delete truckMat;
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
	glClearColor(204 / 255.0, 204 / 255.0, 204 / 255.0, 1.0);

	// textures
	truck->LoadTexture(CString("res/canvas.png"));
	terrain->LoadTexture(CString("res/index.jpg"));

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::StopMovingCamera()
{
	firstMouse = true;
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(cameraX, cameraY, cameraZ,
		0.0, 6.0, 0.0,
		0.0, 1.0, 0.0);

	SetRoomLightning();
	DrawCoordinateLines();
	DrawTerrain();

	glPushMatrix();
	DrawWheels();
	glPopMatrix();

	DrawTruckBody();

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
	cameraX = 2.1 * cameraDistanceFromCoordinateOrigin * xPos; // 0.9 - random factor to keep eye inside of the cube
	//if (cameraX >= 9.9f) // definitely keep eye inside of the cube x-wise
	//	cameraX = 9.9f;
	//if (cameraX <= -9.9f)
	//	cameraX = -9.9f;
	cameraY = 1.4 * cameraDistanceFromCoordinateOrigin * yPos; // 1.4 - random factor so y can grow more
	cameraZ = 2.1 * cameraDistanceFromCoordinateOrigin * zPos;
	//if (cameraZ >= 9.9f) // definitely keep eye inside of the cube z-wise
	//	cameraZ = 9.9f;
	//if (cameraZ <= -9.9f)
	//	cameraZ = -9.9f;
}

void CGLRenderer::SetRoomLightning()
{
	GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	float light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	float light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_emission[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0 };
	float light_position[] = { 10.0f, 10.0f, 10.0f, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_EMISSION, light_emission);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void CGLRenderer::DrawCoordinateLines()
{
	glLineWidth(2.0);
	bool lightingEnabled = glIsEnabled(GL_LIGHTING);
	if (lightingEnabled);
		glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(10.0, 0.0, 0.0);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 10.0, 0.0);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 10.0);
	}
	glEnd();
	if (lightingEnabled)
		glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawTerrain()
{
	terrain->PrepareTexture(false);
	terrain->Select();

	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	{
		glNormal3f(0.0f, 1.0f, 0.0f);

		glTexCoord2f(0.0f, 4.0f);
		glVertex3f(-20.0f, -1.5f, 20.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(20.0f, -1.5f, 20.0f);

		glTexCoord2f(2.0f, 0.0f);
		glVertex3f(20.0f, -1.5f, -20.0f);

		glTexCoord2f(2.0f, 4.0f);
		glVertex3f(-20.0f, -1.5f, -20.0f);

	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawWheels()
{
	DrawBottomLeftWheel();
	DrawTopLeftWheel();
	DrawTopRightWheel();
	DrawBottomRightWheel();
}

void CGLRenderer::DrawTruckBody()
{
	DrawLeftTruckSide();
}

void CGLRenderer::DrawWheel()
{
	const u_char points = 16;
	const float pointAngle = 360.f / points;
	const float r = 1.5f;

	truck->PrepareTexture(false);
	truck->Select();
	glEnable(GL_TEXTURE_2D);

	// front base
	glBegin(GL_POLYGON);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);
		float angle = 0.0f;
		for (int i = 0; i < points; i++)
		{
			float x = r * cos(angle * M_PI / 180.0);
			float y = r * sin(angle * M_PI / 180.0);

			glTexCoord2f((10.0f + x) / 16.0f, (1.5f + y) / 16.0f);
			glVertex3f(x, y, 0.0f);
			angle += pointAngle;
		}
	}
	glEnd();

	// back base
	glBegin(GL_POLYGON);
	{
		glNormal3f(0.0f, 0.0f, -1.0f);
		float angle = 0.0f;
		for (int i = 0; i < points; i++)
		{
			float x = r * cos(angle * M_PI / 180.0);
			float y = r * sin(angle * M_PI / 180.0);

			glTexCoord2f((10.0f + x) / 16.0f, (1.5f + y) / 16.0f);
			glVertex3f(-x, y, -1.0f);
			angle += pointAngle;
		}
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);

	// side
	truckMat->Select();
	glBegin(GL_QUAD_STRIP);
	{
		float angle = 0.0f;
		for (int i = 0; i <= points; i++)
		{
			float x = r * cos(angle * M_PI / 180.0);
			float y = r * sin(angle * M_PI / 180.0);

			glNormal3f(x / r, y / r, 0.0f);
			glVertex3f(x, y, 0.0f);
			glNormal3f(x / r, y / r, 0.0f);
			glVertex3f(x, y, -1.0f);
			angle += pointAngle;
		}
	}
	glEnd();
}

void CGLRenderer::DrawBottomLeftWheel()
{
	glTranslatef(0.5f, 0.0f, 2.5f);
	DrawWheel();
}

void CGLRenderer::DrawTopLeftWheel()
{
	glTranslatef(-4.0f, 0.0f, 0.0f);
	DrawWheel();
}

void CGLRenderer::DrawBottomRightWheel()
{
	glTranslatef(4.0f, 0.0f, 0.0f);
	DrawWheel();
}

void CGLRenderer::DrawTopRightWheel()
{
	glTranslatef(0.0f, 0.0f, -4.0f);
	DrawWheel();
}

void CGLRenderer::DrawLeftTruckSide()
{
	truck->PrepareTexture(false);
	truck->Select();

	glEnable(GL_TEXTURE_2D);

	const float z = 3.0f;

	// split concave polygon into multiple convex (check sketches.png)
	// 1
	glBegin(GL_POLYGON);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);

		// 0
		glTexCoord2f(4.0f / 16.0f, 0.0f);
		glVertex3f(-2.0f, 7.0f, z);

		// 1
		glTexCoord2f(2.0f / 16.0f, 0.0f);
		glVertex3f(-4.0f, 7.0f, z);

		// 2
		glTexCoord2f(0.0f, 2.0f / 16.0f);
		glVertex3f(-6.0f, 5.0f, z);

		// 19
		glTexCoord2f(0.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(-6.0f, 2.0f, z);

		// 20
		glTexCoord2f(1.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(-5.0f, 2.0f, z);

		// 21
		glTexCoord2f(2.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(-4.0f, 2.0f, z);

		// 22
		glTexCoord2f(4.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(-2.0f, 2.0f, z);
	}
	glEnd();

	// 2
	glBegin(GL_POLYGON);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);

		// 3
		glTexCoord2f(0.0f, 7.0f / 16.0f);
		glVertex3f(-6.0f, 0.0f, z);

		// 4
		glTexCoord2f(1.0f / 16.0f, 7.0f / 16.0f);
		glVertex3f(-5.0f, 0.0f, z);

		// 20
		glTexCoord2f(1.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(-5.0f, 2.0f, z);

		// 19
		glTexCoord2f(0.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(-6.0f, 2.0f, z);
	}
	glEnd();

	// 6
	glBegin(GL_POLYGON);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);

		// 20
		glTexCoord2f(1.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(-5.0f, 2.0f, z);

		// 5
		glTexCoord2f(1.0f / 16.0f, 6.0f / 16.0f);
		glVertex3f(-5.0f, 1.0f, z);

		// 21
		glTexCoord2f(2.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(-4.0f, 2.0f, z);
	}
	glEnd();

	// 7
	glBegin(GL_POLYGON);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);

		// 7
		glTexCoord2f(3.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(-3.0f, 2.0f, z);

		// 8
		glTexCoord2f(4.0f / 16.0f, 6.0f / 16.0f);
		glVertex3f(-2.0f, 1.0f, z);

		// 22
		glTexCoord2f(4.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(-2.0f, 2.0f, z);
	}
	glEnd();

	// 3
	glBegin(GL_POLYGON);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);

		// 9
		glTexCoord2f(4.0f / 16.0f, 7.0f / 16.0f);
		glVertex3f(-2.0f, 0.0f, z);

		// 10
		glTexCoord2f(5.0f / 16.0f, 7.0f / 16.0f);
		glVertex3f(-1.0f, 0.0f, z);

		// 23
		glTexCoord2f(5.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(-1.0f, 2.0f, z);

		// 22
		glTexCoord2f(4.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(-2.0f, 2.0f, z);
	}
	glEnd();

	// 8
	glBegin(GL_POLYGON);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);

		// 11
		glTexCoord2f(5.0f / 16.0f, 6.0f / 16.0f);
		glVertex3f(-1.0f, 1.0f, z);

		// 12
		glTexCoord2f(6.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(0.0f, 2.0f, z);

		// 23
		glTexCoord2f(5.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(-1.0f, 2.0f, z);
	}
	glEnd();

	// 9
	glBegin(GL_POLYGON);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);

		// 13
		glTexCoord2f(7.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(1.0f, 2.0f, z);

		// 14
		glTexCoord2f(8.0f / 16.0f, 6.0f / 16.0f);
		glVertex3f(2.0f, 1.0f, z);

		// 24
		glTexCoord2f(8.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(2.0f, 2.0f, z);
	}
	glEnd();

	// 4
	glBegin(GL_POLYGON);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);

		// 15
		glTexCoord2f(8.0f / 16.0f, 7.0f / 16.0f);
		glVertex3f(2.0f, 0.0f, z);

		// 16
		glTexCoord2f(12.0f / 16.0f, 7.0f / 16.0f);
		glVertex3f(6.0f, 0.0f, z);

		// 25
		glTexCoord2f(12.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(6.0f, 2.0f, z);

		// 24
		glTexCoord2f(8.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(2.0f, 2.0f, z);
	}
	glEnd();

	// 5
	glBegin(GL_POLYGON);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);

		// 17
		glTexCoord2f(12.0f / 16.0f, 4.0f / 16.0f);
		glVertex3f(6.0f, 3.0f, z);

		// 18
		glTexCoord2f(4.0f / 16.0f, 4.0f / 16.0f);
		glVertex3f(-2.0f, 3.0f, z);

		// 22
		glTexCoord2f(4.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(-2.0f, 2.0f, z);

		// 25
		glTexCoord2f(12.0f / 16.0f, 5.0f / 16.0f);
		glVertex3f(6.0f, 2.0f, z);
	}
	glEnd();

	// points in sketches.png	
	//// 0
	//glTexCoord2f(4.0f / 16.0f, 0.0f);
	//glVertex3f(-2.0f, 7.0f, z); 

	//// 1
	//glTexCoord2f(2.0f / 16.0f, 0.0f);
	//glVertex3f(-4.0f, 7.0f, z);

	//// 2
	//glTexCoord2f(0.0f, 2.0f / 16.0f);
	//glVertex3f(-6.0f, 5.0f, z);

	//// 3
	//glTexCoord2f(0.0f, 7.0f / 16.0f);
	//glVertex3f(-6.0f, 0.0f, z);

	//// 4
	//glTexCoord2f(1.0f / 16.0f, 7.0f / 16.0f);
	//glVertex3f(-5.0f, 0.0f, z);

	//// 5
	//glTexCoord2f(1.0f / 16.0f, 6.0f / 16.0f);
	//glVertex3f(-5.0f, 1.0f, z);

	//// 6
	//glTexCoord2f(2.0f / 16.0f, 5.0f / 16.0f);
	//glVertex3f(-4.0f, 2.0f, z);

	//// 7
	//glTexCoord2f(3.0f / 16.0f, 5.0f / 16.0f);
	//glVertex3f(-3.0f, 2.0f, z);

	//// 8
	//glTexCoord2f(4.0f / 16.0f, 6.0f / 16.0f);
	//glVertex3f(-2.0f, 1.0f, z);

	//// 9
	//glTexCoord2f(4.0f / 16.0f, 7.0f / 16.0f);
	//glVertex3f(-2.0f, 0.0f, z);

	//// 10
	//glTexCoord2f(5.0f / 16.0f, 7.0f / 16.0f);
	//glVertex3f(-1.0f, 0.0f, z);

	//// 11
	//glTexCoord2f(5.0f / 16.0f, 6.0f / 16.0f);
	//glVertex3f(-1.0f, 1.0f, z);

	//// 12
	//glTexCoord2f(6.0f / 16.0f, 5.0f / 16.0f);
	//glVertex3f(0.0f, 2.0f, z);

	//// 13
	//glTexCoord2f(7.0f / 16.0f, 5.0f / 16.0f);
	//glVertex3f(1.0f, 2.0f, z);

	//// 14
	//glTexCoord2f(8.0f / 16.0f, 6.0f / 16.0f);
	//glVertex3f(2.0f, 1.0f, z);

	//// 15
	//glTexCoord2f(8.0f / 16.0f, 7.0f / 16.0f);
	//glVertex3f(2.0f, 0.0f, z);

	//// 16
	//glTexCoord2f(12.0f / 16.0f, 7.0f / 16.0f);
	//glVertex3f(6.0f, 0.0f, z);

	//// 17
	//glTexCoord2f(12.0f / 16.0f, 4.0f / 16.0f);
	//glVertex3f(6.0f, 3.0f, z);

	//// 18
	//glTexCoord2f(4.0f / 16.0f, 4.0f / 16.0f);
	//glVertex3f(-2.0f, 3.0f, z);

	//// 19
	//glTexCoord2f(0.0f / 16.0f, 5.0f / 16.0f);
	//glVertex3f(-6.0f, 2.0f, z);

	//// 20
	//glTexCoord2f(1.0f / 16.0f, 5.0f / 16.0f);
	//glVertex3f(-5.0f, 2.0f, z);

	//// 21
	//glTexCoord2f(2.0f / 16.0f, 5.0f / 16.0f);
	//glVertex3f(-4.0f, 2.0f, z);

	//// 22
	//glTexCoord2f(4.0f / 16.0f, 5.0f / 16.0f);
	//glVertex3f(-2.0f, 2.0f, z);

	//// 23
	//glTexCoord2f(5.0f / 16.0f, 5.0f / 16.0f);
	//glVertex3f(-1.0f, 2.0f, z);

	//// 24
	//glTexCoord2f(8.0f / 16.0f, 5.0f / 16.0f);
	//glVertex3f(2.0f, 2.0f, z);

	//// 25
	//glTexCoord2f(12.0f / 16.0f, 5.0f / 16.0f);
	//glVertex3f(6.0f, 2.0f, z);

	glDisable(GL_TEXTURE_2D);
}
