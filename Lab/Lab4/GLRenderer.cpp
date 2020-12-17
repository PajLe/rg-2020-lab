#include "pch.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
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
	glClearColor(128.0 / 255.0, 192.0 / 255.0, 255.0 / 255.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	gluLookAt(10.0, 10.0, 0.0,
		0.0, 2.0, 0.0,
		0.0, 1.0, 0.0);


	glBegin(GL_LINES);
	{
		glColor3f(1.0, 0.0, 0.0); // x
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(10.0, 0.0, 0.0);
		glColor3f(0.0, 1.0, 0.0); // y
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 10.0, 0.0);
		glColor3f(0.0, 0.0, 1.0); // z
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 10.0);
	}
	glEnd();
	DrawGrid();
	glBegin(GL_TRIANGLES);
	{
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-1.0, -1.0, -3.0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(1.0, -1.0, -3.0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(1.0, 1.0, -3.0);
	}
	glEnd();

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

void CGLRenderer::DrawGrid()
{
	float topLeftX = -5.0f;
	float topLeftY = 0.0f;
	float topLeftZ = 5.0f;
	const int pointCount = 40;
	const int rows = 11;
	const int cols = 11;
	float pointCoords3f[pointCount * 3];

	// "top" and "bottom" points
	for (int i = 0; i < cols; i++)
	{
		pointCoords3f[i * 3] = topLeftX; // x
		pointCoords3f[i * 3 + 1] = topLeftY; // y
		pointCoords3f[i * 3 + 2] = topLeftZ - i * 1.0; // z

		pointCoords3f[rows * 3 + i * 3] = topLeftX + 10.0; // x
		pointCoords3f[rows * 3 + i * 3 + 1] = topLeftY; // y
		pointCoords3f[rows * 3 + i * 3 + 2] = topLeftZ - i * 1.0; // z
	}

	// "left" and "right" points
	int currPoint = cols * 2;
	for (int i = 0; i < rows - 2; i++)
	{
		pointCoords3f[3 * currPoint] = topLeftX + 1.0 + i * 1.0; // x
		pointCoords3f[3 * currPoint + 1] = topLeftY; // y
		pointCoords3f[3 * currPoint + 2] = topLeftZ; // z

		pointCoords3f[3 * (currPoint + rows - 2)] = topLeftX + 1.0 + i * 1.0; // x
		pointCoords3f[3 * (currPoint + rows - 2) + 1] = topLeftY; // y
		pointCoords3f[3 * (currPoint + rows - 2) + 2] = topLeftZ - 10.0; // z

		currPoint++;
	}

	u_short indices[pointCount + 4];
	int currPos = 0;
	for (int i = 0; i < cols; i++)
	{
		indices[currPos] = i;
		indices[currPos + 1] = i + rows;
		currPos += 2;
	}

	for (int i = 0; i < rows - 2; i++)
	{
		indices[currPos] = 22 + i;
		indices[currPos + 1] = 22 + i + rows - 2;
		currPos += 2;
	}
	indices[40] = 0;
	indices[41] = 10;
	indices[42] = 11;
	indices[43] = 21;

	glVertexPointer(3, GL_FLOAT, 0, pointCoords3f);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColor3f(1.0, 1.0, 1.0);
	glDrawElements(GL_LINES, pointCount + 4, GL_UNSIGNED_SHORT, indices);

	glDisableClientState(GL_VERTEX_ARRAY);
}
