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

CGLRenderer::CGLRenderer()
{
	firstMouse = true;
	cameraX = 10.0;
	cameraY = 10.0;
	cameraZ = 0.0;
	cameraYaw = 0.0f;
	cameraPitch = asin(cameraY / sqrt(pow(cameraX, 2) + pow(cameraY, 2))) * 180 / M_PI;
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
	glClearColor(128.0 / 255.0, 192.0 / 255.0, 255.0 / 255.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();

	gluLookAt(cameraX, cameraY, cameraZ,
		0.0, 3.4, 0.0,
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
	DrawWholeFlower();

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
	cameraX = 14.1 * xPos; // 14.1 = 10*sqrt(2) == initial diag
	cameraY = 14.1 * yPos;
	cameraZ = 14.1 * zPos;
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

	u_char indices[pointCount + 4];
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
	glDrawElements(GL_LINES, pointCount + 4, GL_UNSIGNED_BYTE, indices);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void CGLRenderer::DrawWholeFlower()
{
	DrawFlowerpot();
	DrawBottomRectPrism();
	DrawBottomSphere();

	// left cactus side
	glPushMatrix();
	{
		glTranslatef(0.0f, -0.3f, 0.0f);
		glRotatef(45.0f, 1.0, 0.0, 0.0);
		glTranslatef(0.0f, 0.3f, 0.0f);
		DrawLeftCylinder();
		DrawFirstLeftSphere();
		DrawLeftRectPrism();
		DrawSecondLeftSphere();
	}
	glPopMatrix();

	// right cactus side
	glPushMatrix();
	{
		glTranslatef(0.0f, -0.3f, 0.0f);
		glRotatef(-45.0f, 1.0, 0.0, 0.0);
		glTranslatef(0.0f, 0.3f, 0.0f);
		DrawFirstRightCylinder();
		DrawFirstRightSphere();
		glPushMatrix(); // "turn right"
		{
			glTranslatef(0.0f, -0.3f, 0.0f);
			glRotatef(-45.0f, 1.0, 0.0, 0.0);
			glTranslatef(0.0f, 0.3f, 0.0f);
			DrawSecondRightOctaPrism();
			DrawSecondRightSphere();
		}
		glPopMatrix();

		// "turn left(top)"
		glPushMatrix();
		{
			glTranslatef(0.0f, -0.3f, 0.0f);
			glRotatef(45.0f, 1.0, 0.0, 0.0);
			glTranslatef(0.0f, 0.3f, 0.0f);
			DrawRightRectPrism();
			DrawRightTopmostSphere();
		}
		glPopMatrix();
	}
	glPopMatrix();

	// middle cactus part
	glTranslatef(0.0f, -0.3f, 0.0f);
	glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.3f, 0.0f);
	glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
	DrawRotatingRectPrism();
	DrawFirstRotatingSphere();
	glPushMatrix();
	{
		glTranslatef(0.0f, -0.3f, 0.0f);
		glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.3f, 0.0f);
		DrawFirstLeftCone();
		DrawFirstLeftConeSphere();
		DrawSecondLeftCone();
		DrawSecondLeftConeSphere();
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0.0f, -0.3f, 0.0f);
		glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.3f, 0.0f);
		DrawRightOctaPrism();
		DrawRightSphere();
		DrawRightCone();
		DrawTopRightSphere();
	}
	glPopMatrix();
}

void CGLRenderer::DrawFlowerpot()
{
	const int octagonPoints = 8;
	const float octagonAngle = 360.0f / octagonPoints;

	float bottomBase[octagonPoints * 3];
	float topBase[octagonPoints * 3];
	float topOctaPrismBases[octagonPoints * 3];

	float currAngle = 0.0f;
	float rBottom = 1.0f;
	float rTop = 1.2f;
	float rTopOctaPrismBases = 1.5f;
	for (int i = 0; i < octagonPoints * 3; i += 3)
	{
		bottomBase[i] = rBottom * cos(currAngle * M_PI / 180.0); // x
		bottomBase[i + 1] = 0.0f; // y
		bottomBase[i + 2] = -rBottom * sin(currAngle * M_PI / 180.0); // z

		topBase[i] = rTop * cos(currAngle * M_PI / 180.0); // x
		topBase[i + 1] = 0.0f; // y
		topBase[i + 2] = -rTop * sin(currAngle * M_PI / 180.0); // z

		topOctaPrismBases[i] = rTopOctaPrismBases * cos(currAngle * M_PI / 180.0); // x
		topOctaPrismBases[i + 1] = 0.0f; // y
		topOctaPrismBases[i + 2] = -rTopOctaPrismBases * sin(currAngle * M_PI / 180.0); // z
		currAngle += octagonAngle;
	}

	float sides[2 * octagonPoints * 3];
	float bottomPrismHeight = 1.4f;
	for (int i = 0; i < octagonPoints * 3; i += 3)
	{
		sides[i] = bottomBase[i];
		sides[i + 1] = bottomBase[i + 1];
		sides[i + 2] = bottomBase[i + 2];
		sides[3 * octagonPoints + i] = topBase[i];
		sides[3 * octagonPoints + i + 1] = topBase[i + 1] + bottomPrismHeight;
		sides[3 * octagonPoints + i + 2] = topBase[i + 2];
	}

	float topSides[2 * octagonPoints * 3];
	float topPrismHeight = 0.6f;
	for (int i = 0; i < octagonPoints * 3; i += 3)
	{
		topSides[i] = topOctaPrismBases[i];
		topSides[i + 1] = topOctaPrismBases[i + 1];
		topSides[i + 2] = topOctaPrismBases[i + 2];
		topSides[3 * octagonPoints + i] = topOctaPrismBases[i];
		topSides[3 * octagonPoints + i + 1] = topOctaPrismBases[i + 1] + topPrismHeight;
		topSides[3 * octagonPoints + i + 2] = topOctaPrismBases[i + 2];
	}

	u_char baseIndices[octagonPoints];
	for (int i = 0; i < octagonPoints; i++)
		baseIndices[i] = i;

	u_char sidesIndices[2 * octagonPoints + 2]
	{
		0, 8, 1, 9,
		2, 10,
		3, 11,
		4, 12,
		5, 13,
		6, 14,
		7, 15,
		0, 8
	};

	glColor4f(163 / 255.0, 93 / 255.0, 29 / 255.0, 0.0);

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, bottomBase);
	glDrawElements(GL_POLYGON, octagonPoints, GL_UNSIGNED_BYTE, baseIndices);

	glVertexPointer(3, GL_FLOAT, 0, sides);
	glDrawElements(GL_QUAD_STRIP, 2 * octagonPoints + 2, GL_UNSIGNED_BYTE, sidesIndices);

	glVertexPointer(3, GL_FLOAT, 0, topBase);
	glTranslatef(0.0f, bottomPrismHeight, 0.0f);
	glDrawElements(GL_POLYGON, octagonPoints, GL_UNSIGNED_BYTE, baseIndices);

	glVertexPointer(3, GL_FLOAT, 0, topOctaPrismBases);
	glDrawElements(GL_POLYGON, octagonPoints, GL_UNSIGNED_BYTE, baseIndices);

	glVertexPointer(3, GL_FLOAT, 0, topSides);
	glDrawElements(GL_QUAD_STRIP, 2 * octagonPoints + 2, GL_UNSIGNED_BYTE, sidesIndices);

	glVertexPointer(3, GL_FLOAT, 0, topOctaPrismBases);
	glTranslatef(0.0f, topPrismHeight, 0.0f);
	glDrawElements(GL_POLYGON, octagonPoints, GL_UNSIGNED_BYTE, baseIndices);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void CGLRenderer::DrawBottomRectPrism()
{
	const int rectPoints = 4;

	float base[rectPoints * 3]{
		0.5f, 0.0f, 0.5f,
		0.5f, 0.0f, -0.5f,
		-0.5f, 0.0f, -0.5f,
		-0.5f, 0.0f, 0.5f
	};

	float sides[2 * rectPoints * 3];
	float quadPrismHeight = 1.45f;
	for (int i = 0; i < rectPoints * 3; i += 3)
	{
		sides[i] = base[i];
		sides[i + 1] = base[i + 1];
		sides[i + 2] = base[i + 2];
		sides[3 * rectPoints + i] = base[i];
		sides[3 * rectPoints + i + 1] = base[i + 1] + quadPrismHeight;
		sides[3 * rectPoints + i + 2] = base[i + 2];
	}

	u_char baseIndices[rectPoints];
	for (int i = 0; i < rectPoints; i++)
		baseIndices[i] = i;

	u_char sidesIndices[2 * rectPoints + 2]
	{
		0, 4, 1, 5,
		2, 6,
		3, 7,
		0, 4
	};

	glColor4f(0 / 255.0, 188 / 255.0, 0 / 255.0, 0.0f);

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, base);
	glDrawElements(GL_POLYGON, rectPoints, GL_UNSIGNED_BYTE, baseIndices);

	glVertexPointer(3, GL_FLOAT, 0, sides);
	glDrawElements(GL_QUAD_STRIP, 2 * rectPoints + 2, GL_UNSIGNED_BYTE, sidesIndices);

	glVertexPointer(3, GL_FLOAT, 0, base);
	glTranslatef(0.0f, quadPrismHeight, 0.0f);
	glDrawElements(GL_POLYGON, rectPoints, GL_UNSIGNED_BYTE, baseIndices);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void CGLRenderer::DrawBottomSphere()
{
	glTranslatef(0.0f, 0.3f, 0.0f);
	DrawSphere(0.3);
	glTranslatef(0.0f, 0.3f, 0.0f);
}

void CGLRenderer::DrawLeftCylinder()
{
	const int cylPoints = 360;
	const float cylAngle = 360.0f / cylPoints;
	const float cylHeight = 1.45f;

	float base[cylPoints * 3];

	float currAngle = 0.0f;
	float r = 0.3f;
	for (int i = 0; i < cylPoints * 3; i += 3)
	{
		base[i] = r * cos(currAngle * M_PI / 180.0); // x
		base[i + 1] = 0.0f; // y
		base[i + 2] = -r * sin(currAngle * M_PI / 180.0); // z

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

	glVertexPointer(3, GL_FLOAT, 0, base);
	glDrawElements(GL_POLYGON, cylPoints, GL_UNSIGNED_SHORT, baseIndices);

	glVertexPointer(3, GL_FLOAT, 0, sides);
	glDrawElements(GL_QUAD_STRIP, 2 * cylPoints + 2, GL_UNSIGNED_SHORT, sidesIndices);

	glVertexPointer(3, GL_FLOAT, 0, base);
	glTranslatef(0.0f, cylHeight, 0.0f);
	glDrawElements(GL_POLYGON, cylPoints, GL_UNSIGNED_SHORT, baseIndices);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void CGLRenderer::DrawFirstLeftSphere()
{
	glTranslatef(0.0f, 0.3f, 0.0f);
	DrawSphere(0.3);
	glTranslatef(0.0f, 0.3f, 0.0f);
}

void CGLRenderer::DrawLeftRectPrism()
{
	const int rectPoints = 4;

	float base[rectPoints * 3]{
		0.6f, 0.0f, 0.6f,
		0.6f, 0.0f, -0.6f,
		-0.6f, 0.0f, -0.6f,
		-0.6f, 0.0f, 0.6f
	};

	float sides[2 * rectPoints * 3];
	float quadPrismHeight = 1.4f;
	for (int i = 0; i < rectPoints * 3; i += 3)
	{
		sides[i] = base[i];
		sides[i + 1] = base[i + 1];
		sides[i + 2] = base[i + 2];
		sides[3 * rectPoints + i] = base[i];
		sides[3 * rectPoints + i + 1] = base[i + 1] + quadPrismHeight;
		sides[3 * rectPoints + i + 2] = base[i + 2];
	}

	u_char baseIndices[rectPoints];
	for (int i = 0; i < rectPoints; i++)
		baseIndices[i] = i;

	u_char sidesIndices[2 * rectPoints + 2]
	{
		0, 4, 1, 5,
		2, 6,
		3, 7,
		0, 4
	};

	glColor4f(0 / 255.0, 188 / 255.0, 0 / 255.0, 0.0f);

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, base);
	glDrawElements(GL_POLYGON, rectPoints, GL_UNSIGNED_BYTE, baseIndices);

	glVertexPointer(3, GL_FLOAT, 0, sides);
	glDrawElements(GL_QUAD_STRIP, 2 * rectPoints + 2, GL_UNSIGNED_BYTE, sidesIndices);

	glVertexPointer(3, GL_FLOAT, 0, base);
	glTranslatef(0.0f, quadPrismHeight, 0.0f);
	glDrawElements(GL_POLYGON, rectPoints, GL_UNSIGNED_BYTE, baseIndices);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void CGLRenderer::DrawSecondLeftSphere()
{
	glTranslatef(0.0f, 0.3f, 0.0f);
	DrawSphere(0.3);
	glTranslatef(0.0f, 0.3f, 0.0f);
}

void CGLRenderer::DrawFirstRightCylinder()
{
	const int cylPoints = 360;
	const float cylAngle = 360.0f / cylPoints;
	const float cylHeight = 1.45f;

	float base[cylPoints * 3];

	float currAngle = 0.0f;
	float r = 0.3f;
	for (int i = 0; i < cylPoints * 3; i += 3)
	{
		base[i] = r * cos(currAngle * M_PI / 180.0); // x
		base[i + 1] = 0.0f; // y
		base[i + 2] = -r * sin(currAngle * M_PI / 180.0); // z

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

	glVertexPointer(3, GL_FLOAT, 0, base);
	glDrawElements(GL_POLYGON, cylPoints, GL_UNSIGNED_SHORT, baseIndices);

	glVertexPointer(3, GL_FLOAT, 0, sides);
	glDrawElements(GL_QUAD_STRIP, 2 * cylPoints + 2, GL_UNSIGNED_SHORT, sidesIndices);

	glVertexPointer(3, GL_FLOAT, 0, base);
	glTranslatef(0.0f, cylHeight, 0.0f);
	glDrawElements(GL_POLYGON, cylPoints, GL_UNSIGNED_SHORT, baseIndices);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void CGLRenderer::DrawFirstRightSphere()
{
	glTranslatef(0.0f, 0.3f, 0.0f);
	DrawSphere(0.3);
	glTranslatef(0.0f, 0.3f, 0.0f);
}

void CGLRenderer::DrawSecondRightOctaPrism()
{
	const int octagonPoints = 8;
	const float octagonAngle = 360.0f / octagonPoints;
	const float octaPrismHeight = 1.4f;

	float base[octagonPoints * 3];

	float currAngle = 0.0f;
	float r = .5f;
	for (int i = 0; i < octagonPoints * 3; i += 3)
	{
		base[i] = r * cos(currAngle * M_PI / 180.0); // x
		base[i + 1] = 0.0f; // y
		base[i + 2] = -r * sin(currAngle * M_PI / 180.0); // z

		currAngle += octagonAngle;
	}

	float sides[2 * octagonPoints * 3];
	for (int i = 0; i < octagonPoints * 3; i += 3)
	{
		sides[i] = base[i];
		sides[i + 1] = base[i + 1];
		sides[i + 2] = base[i + 2];
		sides[3 * octagonPoints + i] = base[i];
		sides[3 * octagonPoints + i + 1] = base[i + 1] + octaPrismHeight;
		sides[3 * octagonPoints + i + 2] = base[i + 2];
	}

	u_short baseIndices[octagonPoints];
	for (int i = 0; i < octagonPoints; i++)
		baseIndices[i] = i;

	u_short sidesIndices[2 * octagonPoints + 2];
	for (int i = 0; i < 2 * octagonPoints; i += 2)
	{
		sidesIndices[i] = i / 2;
		sidesIndices[i + 1] = i / 2 + octagonPoints;
	}
	sidesIndices[2 * octagonPoints + 2 - 2] = sidesIndices[0];
	sidesIndices[2 * octagonPoints + 2 - 1] = sidesIndices[1];

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, base);
	glDrawElements(GL_POLYGON, octagonPoints, GL_UNSIGNED_SHORT, baseIndices);

	glVertexPointer(3, GL_FLOAT, 0, sides);
	glDrawElements(GL_QUAD_STRIP, 2 * octagonPoints + 2, GL_UNSIGNED_SHORT, sidesIndices);

	glVertexPointer(3, GL_FLOAT, 0, base);
	glTranslatef(0.0f, octaPrismHeight, 0.0f);
	glDrawElements(GL_POLYGON, octagonPoints, GL_UNSIGNED_SHORT, baseIndices);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void CGLRenderer::DrawSecondRightSphere()
{
	glTranslatef(0.0f, 0.3f, 0.0f);
	DrawSphere(0.3);
	glTranslatef(0.0f, 0.3f, 0.0f);
}

void CGLRenderer::DrawRightRectPrism()
{
	DrawBottomRectPrism();
}

void CGLRenderer::DrawRotatingRectPrism()
{
	const int rectPoints = 4;
	glEnable(GL_DEPTH_TEST);

	float base[rectPoints * 3]{
		0.3f, 0.0f, 0.3f,
		0.3f, 0.0f, -0.3f,
		-0.3f, 0.0f, -0.3f,
		-0.3f, 0.0f, 0.3f
	};

	float sides[2 * rectPoints * 3];
	float quadPrismHeight = 1.45f;
	for (int i = 0; i < rectPoints * 3; i += 3)
	{
		sides[i] = base[i];
		sides[i + 1] = base[i + 1];
		sides[i + 2] = base[i + 2];
		sides[3 * rectPoints + i] = base[i];
		sides[3 * rectPoints + i + 1] = base[i + 1] + quadPrismHeight;
		sides[3 * rectPoints + i + 2] = base[i + 2];
	}

	u_char baseIndices[rectPoints];
	for (int i = 0; i < rectPoints; i++)
		baseIndices[i] = rectPoints - 1 - i;

	u_char sidesIndices[2 * rectPoints + 2]
	{
		0, 4, 1, 5,
		2, 6,
		3, 7,
		0, 4
	};

	glColor4f(188 / 255.0, 188 / 255.0, 0 / 255.0, 0.0f);

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, base);
	glDrawElements(GL_POLYGON, rectPoints, GL_UNSIGNED_BYTE, baseIndices);

	glVertexPointer(3, GL_FLOAT, 0, sides);
	glDrawElements(GL_QUAD_STRIP, 2 * rectPoints + 2, GL_UNSIGNED_BYTE, sidesIndices);

	glVertexPointer(3, GL_FLOAT, 0, base);
	glTranslatef(0.0f, quadPrismHeight, 0.0f);
	glDrawElements(GL_POLYGON, rectPoints, GL_UNSIGNED_BYTE, baseIndices);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void CGLRenderer::DrawFirstRotatingSphere()
{
	glTranslatef(0.0f, 0.3f, 0.0f);
	DrawSphere(0.3);
	glTranslatef(0.0f, 0.3f, 0.0f);
}

void CGLRenderer::DrawFirstLeftCone()
{
	const int conePoints = 360;
	const float coneAngle = 360.0f / conePoints;
	const float coneHeight = 1.4f;

	float base[(conePoints + 1) * 3];
	u_short indices[conePoints + 1];
	for (int i = 0; i < conePoints + 1; i++)
		indices[i] = i;

	float currAngle = 0.0f;
	float r = .5f;
	base[0] = 0.0f;
	base[1] = 0.0f;
	base[2] = 0.0f;
	for (int i = 3; i < conePoints * 3 + 3; i += 3)
	{
		base[i] = r * cos(currAngle * M_PI / 180.0); // x
		base[i + 1] = 0.0f; // y
		base[i + 2] = r * sin(currAngle * M_PI / 180.0); // z

		currAngle += coneAngle;
	}

	float side[(conePoints + 1) * 3];
	side[0] = 0.0f;
	side[1] = coneHeight;
	side[2] = 0.0f;
	for (int i = 3; i < conePoints * 3 + 3; i += 3)
	{
		side[i] = base[i];
		side[i + 1] = base[i + 1];
		side[i + 2] = base[i + 2];
	}

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, base);
	glDrawElements(GL_TRIANGLE_FAN, (conePoints + 1), GL_UNSIGNED_SHORT, indices);

	glVertexPointer(3, GL_FLOAT, 0, side);
	glDrawElements(GL_TRIANGLE_FAN, (conePoints + 1), GL_UNSIGNED_SHORT, indices);

	glTranslatef(0.0f, coneHeight, 0.0f);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void CGLRenderer::DrawFirstLeftConeSphere()
{
	glTranslatef(0.0f, 0.3f, 0.0f);
	DrawSphere(0.3);
	glTranslatef(0.0f, 0.3f, 0.0f);
}

void CGLRenderer::DrawSecondLeftCone()
{
	const int conePoints = 360;
	const float coneAngle = 360.0f / conePoints;
	const float coneHeight = 1.6f;

	float base[(conePoints + 1) * 3];
	u_short indices[conePoints + 1];
	for (int i = 0; i < conePoints + 1; i++)
		indices[i] = i;

	float currAngle = 0.0f;
	float r = .5f;
	base[0] = 0.0f;
	base[1] = 0.0f;
	base[2] = 0.0f;
	for (int i = 3; i < conePoints * 3 + 3; i += 3)
	{
		base[i] = r * cos(currAngle * M_PI / 180.0); // x
		base[i + 1] = 0.0f; // y
		base[i + 2] = r * sin(currAngle * M_PI / 180.0); // z

		currAngle += coneAngle;
	}

	float side[(conePoints + 1) * 3];
	side[0] = 0.0f;
	side[1] = coneHeight;
	side[2] = 0.0f;
	for (int i = 3; i < conePoints * 3 + 3; i += 3)
	{
		side[i] = base[i];
		side[i + 1] = base[i + 1];
		side[i + 2] = base[i + 2];
	}

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, base);
	glDrawElements(GL_TRIANGLE_FAN, (conePoints + 1), GL_UNSIGNED_SHORT, indices);

	glVertexPointer(3, GL_FLOAT, 0, side);
	glDrawElements(GL_TRIANGLE_FAN, (conePoints + 1), GL_UNSIGNED_SHORT, indices);

	glTranslatef(0.0f, coneHeight, 0.0f);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void CGLRenderer::DrawSecondLeftConeSphere()
{
	glTranslatef(0.0f, 0.3f, 0.0f);
	DrawSphere(0.3);
	glTranslatef(0.0f, 0.3f, 0.0f);
}

void CGLRenderer::DrawRightOctaPrism()
{
	const int octagonPoints = 8;
	const float octagonAngle = 360.0f / octagonPoints;
	const float octaPrismHeight = 1.4f;

	float base[octagonPoints * 3];

	float currAngle = 0.0f;
	float r = .5f;
	for (int i = 0; i < octagonPoints * 3; i += 3)
	{
		base[i] = r * cos(currAngle * M_PI / 180.0); // x
		base[i + 1] = 0.0f; // y
		base[i + 2] = -r * sin(currAngle * M_PI / 180.0); // z

		currAngle += octagonAngle;
	}

	float sides[2 * octagonPoints * 3];
	for (int i = 0; i < octagonPoints * 3; i += 3)
	{
		sides[i] = base[i];
		sides[i + 1] = base[i + 1];
		sides[i + 2] = base[i + 2];
		sides[3 * octagonPoints + i] = base[i];
		sides[3 * octagonPoints + i + 1] = base[i + 1] + octaPrismHeight;
		sides[3 * octagonPoints + i + 2] = base[i + 2];
	}

	u_short baseIndices[octagonPoints];
	for (int i = 0; i < octagonPoints; i++)
		baseIndices[i] = i;

	u_short sidesIndices[2 * octagonPoints + 2];
	for (int i = 0; i < 2 * octagonPoints; i += 2)
	{
		sidesIndices[i] = i / 2;
		sidesIndices[i + 1] = i / 2 + octagonPoints;
	}
	sidesIndices[2 * octagonPoints + 2 - 2] = sidesIndices[0];
	sidesIndices[2 * octagonPoints + 2 - 1] = sidesIndices[1];

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, base);
	glDrawElements(GL_POLYGON, octagonPoints, GL_UNSIGNED_SHORT, baseIndices);

	glVertexPointer(3, GL_FLOAT, 0, sides);
	glDrawElements(GL_QUAD_STRIP, 2 * octagonPoints + 2, GL_UNSIGNED_SHORT, sidesIndices);

	glVertexPointer(3, GL_FLOAT, 0, base);
	glTranslatef(0.0f, octaPrismHeight, 0.0f);
	glDrawElements(GL_POLYGON, octagonPoints, GL_UNSIGNED_SHORT, baseIndices);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void CGLRenderer::DrawRightSphere()
{
	glTranslatef(0.0f, 0.3f, 0.0f);
	DrawSphere(0.3);
	glTranslatef(0.0f, 0.3f, 0.0f);
}

void CGLRenderer::DrawRightCone()
{
	const int conePoints = 360;
	const float coneAngle = 360.0f / conePoints;
	const float coneHeight = 1.6f;

	float base[(conePoints + 1) * 3];
	u_short indices[conePoints + 1];
	for (int i = 0; i < conePoints + 1; i++)
		indices[i] = i;

	float currAngle = 0.0f;
	float r = .5f;
	base[0] = 0.0f;
	base[1] = 0.0f;
	base[2] = 0.0f;
	for (int i = 3; i < conePoints * 3 + 3; i += 3)
	{
		base[i] = r * cos(currAngle * M_PI / 180.0); // x
		base[i + 1] = 0.0f; // y
		base[i + 2] = r * sin(currAngle * M_PI / 180.0); // z

		currAngle += coneAngle;
	}

	float side[(conePoints + 1) * 3];
	side[0] = 0.0f;
	side[1] = coneHeight;
	side[2] = 0.0f;
	for (int i = 3; i < conePoints * 3 + 3; i += 3)
	{
		side[i] = base[i];
		side[i + 1] = base[i + 1];
		side[i + 2] = base[i + 2];
	}

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, base);
	glDrawElements(GL_TRIANGLE_FAN, (conePoints + 1), GL_UNSIGNED_SHORT, indices);

	glVertexPointer(3, GL_FLOAT, 0, side);
	glDrawElements(GL_TRIANGLE_FAN, (conePoints + 1), GL_UNSIGNED_SHORT, indices);

	glTranslatef(0.0f, coneHeight, 0.0f);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void CGLRenderer::DrawTopRightSphere()
{
	glTranslatef(0.0f, 0.3f, 0.0f);
	DrawSphere(0.3);
	glTranslatef(0.0f, 0.3f, 0.0f);
}

void CGLRenderer::DrawRightTopmostSphere()
{
	glTranslatef(0.0f, 0.3f, 0.0f);
	DrawSphere(0.3);
	glTranslatef(0.0f, 0.3f, 0.0f);
}
