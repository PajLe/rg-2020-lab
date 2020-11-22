
// Lab1View.cpp : implementation of the CLab1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab1.h"
#endif

#include "Lab1Doc.h"
#include "Lab1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES
#include <math.h>

// CLab1View

IMPLEMENT_DYNCREATE(CLab1View, CView)

BEGIN_MESSAGE_MAP(CLab1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CLab1View construction/destruction

CLab1View::CLab1View() noexcept
{
	// TODO: add construction code here
	drawGridButtonClicked = false;
	mainRectSideSize = 500;
	gridSquareSize = mainRectSideSize / 20;
	gridButton = CRect(0, 0, mainRectSideSize * 7 / 20, mainRectSideSize / 20);
	mainRect = CRect(gridButton.left, gridButton.bottom + 10, gridButton.left + mainRectSideSize, gridButton.bottom + 10 + mainRectSideSize);
}

CLab1View::~CLab1View()
{
}

BOOL CLab1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab1View drawing

void CLab1View::OnDraw(CDC* pDC)
{
	DrawGridButton(pDC);
	DrawBackgroundRect(pDC);
	DrawYellowTriangle(pDC);
	DrawPinkParallelogram(pDC);
	DrawRedTriangle(pDC);
	DrawLeftGrayTriangle(pDC);
	DrawOrangeSquare(pDC);
	DrawRightGrayTriangle(pDC);
	DrawGreenTriangle(pDC);
	DrawHachedTriangle(pDC);
	DrawPurpleTriangle(pDC);
	if (drawGridButtonClicked)
		DrawGrid(pDC);
}


// CLab1View printing

BOOL CLab1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab1View diagnostics

#ifdef _DEBUG
void CLab1View::AssertValid() const
{
	CView::AssertValid();
}

void CLab1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab1Doc* CLab1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab1Doc)));
	return (CLab1Doc*)m_pDocument;
}
#endif //_DEBUG

void CLab1View::DrawLeftGrayTriangle(CDC* pDC)
{
	CPen redPen(PS_SOLID, 5, RGB(255, 0, 0));
	CBrush grayBrush(RGB(221, 221, 221));

	POINT grayTrianglePoints[] =
	{
		{mainRect.left + 7 * gridSquareSize, mainRect.top + 7 * gridSquareSize},
		{mainRect.left + 7 * gridSquareSize, mainRect.bottom - 7 * gridSquareSize},
		{mainRect.left + gridSquareSize, mainRect.bottom - 7 * gridSquareSize}
	};

	CPen* oldPen = pDC->SelectObject(&redPen);
	CBrush* oldBrush = pDC->SelectObject(&grayBrush);
	pDC->Polygon(grayTrianglePoints, 3);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	grayBrush.DeleteObject();
	redPen.DeleteObject();
}

void CLab1View::DrawOrangeSquare(CDC* pDC)
{
	CPen redPen(PS_SOLID, 5, RGB(255, 0, 0));
	CBrush orangeBrush(RGB(255, 153, 51));

	POINT orangeSquarePoints[] =
	{
		{mainRect.left + 7 * gridSquareSize, mainRect.top + 7 * gridSquareSize},
		{mainRect.left + 7 * gridSquareSize, mainRect.bottom - 7 * gridSquareSize},
		{mainRect.right - 7 * gridSquareSize, mainRect.bottom - 7 * gridSquareSize},
		{mainRect.right - 7 * gridSquareSize, mainRect.top + 7 * gridSquareSize}
	};

	CPen* oldPen = pDC->SelectObject(&redPen);
	CBrush* oldBrush = pDC->SelectObject(&orangeBrush);
	pDC->Polygon(orangeSquarePoints, 4);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	orangeBrush.DeleteObject();
	redPen.DeleteObject();
}

void CLab1View::DrawGreenTriangle(CDC* pDC)
{
	CPen redPen(PS_SOLID, 5, RGB(255, 0, 0));
	CBrush greenBrush(RGB(50, 205, 50));

	POINT A = { mainRect.left + gridSquareSize, mainRect.bottom - gridSquareSize};
	POINT B = { mainRect.left + 7 * gridSquareSize, mainRect.bottom - gridSquareSize };
	POINT C = { mainRect.left + gridSquareSize, mainRect.top + 13 * gridSquareSize };
	POINT greenTrianglePoints[] = { A, B, C };
	double a = DistanceBetweenTwoPoints(B, C);
	double b = DistanceBetweenTwoPoints(A, C);
	double c = DistanceBetweenTwoPoints(A, B);
	POINT triangleCenter = { int((a * A.x + b * B.x + c * C.x) / (a + b + c)), int((a * A.y + b * B.y + c * C.y) / (a + b + c)) };

	CPen* oldPen = pDC->SelectObject(&redPen);
	CBrush* oldBrush = pDC->SelectObject(&greenBrush);
	pDC->Polygon(greenTrianglePoints, 3);

	CPen redPen2(PS_SOLID, 3, RGB(255, 0, 0));
	pDC->SelectObject(&redPen2);
	POINT octagonPoints[8];
	double alfa = 0;
	for (int i = 0; i < 8; i++)
	{
		octagonPoints[i] =
		{
			triangleCenter.x + int(gridSquareSize * 1 * cos(alfa * M_PI / 180) + 0.5),
			triangleCenter.y + int(gridSquareSize * 1 * sin(alfa * M_PI / 180) + 0.5)
		};
		alfa += 360 / 8;
	}
	pDC->Polygon(octagonPoints, 8);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	redPen2.DeleteObject();
	greenBrush.DeleteObject();
	redPen.DeleteObject();
}

void CLab1View::DrawHachedTriangle(CDC* pDC)
{
	CPen redPen(PS_SOLID, 5, RGB(255, 0, 0));
	CBrush blueVerticalHachedBrush(HS_VERTICAL, RGB(102, 153, 255));

	POINT A = { mainRect.right - 7 * gridSquareSize, mainRect.bottom - 7 * gridSquareSize };
	POINT B = { mainRect.left + 7 * gridSquareSize, mainRect.bottom - gridSquareSize };
	POINT C = { mainRect.left + gridSquareSize, mainRect.top + 13 * gridSquareSize };
	POINT hachedTrianglePoints[] = { A, B, C };
	double a = DistanceBetweenTwoPoints(B, C);
	double b = DistanceBetweenTwoPoints(A, C);
	double c = DistanceBetweenTwoPoints(A, B);
	POINT triangleCenter = { int((a * A.x + b * B.x + c * C.x) / (a + b + c)), int((a * A.y + b * B.y + c * C.y) / (a + b + c)) };

	CPen* oldPen = pDC->SelectObject(&redPen);
	CBrush* oldBrush = pDC->SelectObject(&blueVerticalHachedBrush);
	pDC->Polygon(hachedTrianglePoints, 3);

	CPen redPen2(PS_SOLID, 3, RGB(255, 0, 0));
	pDC->SelectObject(&redPen2);
	POINT squarePoints[4];
	double alfa = 0;
	for (int i = 0; i < 4; i++)
	{
		squarePoints[i] =
		{
			triangleCenter.x + int(gridSquareSize * 1.3 * cos(alfa * M_PI / 180) + 0.5),
			triangleCenter.y + int(gridSquareSize * 1.3 * sin(alfa * M_PI / 180) + 0.5)
		};
		alfa += 360 / 4;
	}
	pDC->Polygon(squarePoints, 4);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	redPen2.DeleteObject();
	blueVerticalHachedBrush.DeleteObject();
	redPen.DeleteObject();
}

void CLab1View::DrawPurpleTriangle(CDC* pDC)
{
	CPen redPen(PS_SOLID, 5, RGB(255, 0, 0));
	CBrush purpleBrush(RGB(153, 0, 204));

	POINT A = { mainRect.right - gridSquareSize, mainRect.top + 7 * gridSquareSize };
	POINT B = { mainRect.left + 7 * gridSquareSize, mainRect.bottom - gridSquareSize };
	POINT C = { mainRect.right - gridSquareSize, mainRect.bottom - gridSquareSize };
	POINT purpleTrianglePoints[] = { A, B, C };
	double a = DistanceBetweenTwoPoints(B, C);
	double b = DistanceBetweenTwoPoints(A, C);
	double c = DistanceBetweenTwoPoints(A, B);
	POINT triangleCenter = { int((a * A.x + b * B.x + c * C.x) / (a + b + c)), int((a * A.y + b * B.y + c * C.y) / (a + b + c)) };

	CPen* oldPen = pDC->SelectObject(&redPen);
	CBrush* oldBrush = pDC->SelectObject(&purpleBrush);
	pDC->Polygon(purpleTrianglePoints, 3);

	CPen redPen2(PS_SOLID, 3, RGB(255, 0, 0));
	pDC->SelectObject(&redPen2);
	POINT heptagonPoints[7];
	double alfa = 0;
	for (int i = 0; i < 7; i++)
	{
		heptagonPoints[i] =
		{
			triangleCenter.x + int(gridSquareSize * 1.85 * cos(alfa * M_PI / 180) + 0.5),
			triangleCenter.y + int(gridSquareSize * 1.85 * sin(alfa * M_PI / 180) + 0.5)
		};
		if (i == 4)
			heptagonPoints[i].x = heptagonPoints[i - 1].x;
		alfa += 360 / 7;
	}
	pDC->Polygon(heptagonPoints, 7);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	redPen2.DeleteObject();
	purpleBrush.DeleteObject();
	redPen.DeleteObject();
}

void CLab1View::DrawGridButton(CDC* pDC)
{
	CBrush gridButtonBrush(RGB(236, 236, 236));
	CBrush* oldBrush = pDC->SelectObject(&gridButtonBrush);
	pDC->Rectangle(gridButton);
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	if (drawGridButtonClicked)
		pDC->DrawText(CString("ISKLJUCI GRID (MREZU)"), gridButton, DT_CENTER);
	else
		pDC->DrawText(CString("UKLJUCI GRID (MREZU)"), gridButton, DT_CENTER);

	pDC->SelectObject(oldBrush);
	pDC->SetBkMode(oldBkMode);
	gridButtonBrush.DeleteObject();
}

void CLab1View::DrawBackgroundRect(CDC* pDC)
{
	CBrush grayBrush(RGB(221, 221, 221));
	CBrush* oldBrush = pDC->SelectObject(&grayBrush);
	CGdiObject* oldPen = pDC->SelectStockObject(NULL_PEN);
	pDC->Rectangle(mainRect);

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
	grayBrush.DeleteObject();
}

void CLab1View::DrawYellowTriangle(CDC* pDC)
{
	CPen redPen(PS_SOLID, 5, RGB(255, 0, 0));
	CBrush yellowBrush(RGB(255, 255, 0));

	POINT A = { mainRect.left + gridSquareSize, mainRect.top + gridSquareSize };
	POINT B = { mainRect.left + 13 * gridSquareSize, mainRect.top + gridSquareSize };
	POINT C = { mainRect.left + gridSquareSize, mainRect.top + 13 * gridSquareSize };
	POINT yellowTrianglePoints[] = { A, B, C };
	double a = DistanceBetweenTwoPoints(B, C);
	double b = DistanceBetweenTwoPoints(A, C);
	double c = DistanceBetweenTwoPoints(A, B);
	POINT triangleCenter = { int((a * A.x + b * B.x + c * C.x) / (a + b + c)), int((a * A.y + b * B.y + c * C.y) / (a + b + c)) };

	CPen* oldPen = pDC->SelectObject(&redPen);
	CBrush* oldBrush = pDC->SelectObject(&yellowBrush);
	pDC->Polygon(yellowTrianglePoints, 3);

	CPen redPen2(PS_SOLID, 3, RGB(255, 0, 0));
	pDC->SelectObject(&redPen2);
	POINT pentagonPoints[5];
	double alfa = 0;
	for (int i = 0; i < 5; i++)
	{
		pentagonPoints[i] = 
		{ 
			triangleCenter.x + int(gridSquareSize * 1.85 * cos(alfa * M_PI / 180) + 0.5),
			triangleCenter.y + int(gridSquareSize * 1.85 * sin(alfa * M_PI / 180) + 0.5) 
		};
		alfa += 360 / 5;
	}
	pDC->Polygon(pentagonPoints, 5);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	redPen2.DeleteObject();
	yellowBrush.DeleteObject();
	redPen.DeleteObject();
}

void CLab1View::DrawPinkParallelogram(CDC* pDC)
{
	CPen redPen(PS_SOLID, 5, RGB(255, 0, 0));
	CBrush pinkBrush(RGB(255, 153, 204));

	POINT parallelogramPoints[] =
	{
		{mainRect.left + 13 * gridSquareSize, mainRect.top + gridSquareSize},
		{mainRect.right - gridSquareSize, mainRect.top + gridSquareSize},
		{mainRect.right - 7 * gridSquareSize, mainRect.top + 7 * gridSquareSize},
		{mainRect.left + 7 * gridSquareSize, mainRect.top + 7 * gridSquareSize}
	};
	CPen* oldPen = pDC->SelectObject(&redPen);
	CBrush* oldBrush = pDC->SelectObject(&pinkBrush);
	pDC->Polygon(parallelogramPoints, 4);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	pinkBrush.DeleteObject();
	redPen.DeleteObject();
}

void CLab1View::DrawRedTriangle(CDC* pDC)
{
	CPen redPen(PS_SOLID, 5, RGB(255, 0, 0));
	CBrush redBrush(RGB(255, 0, 0));

	POINT redTrianglePoints[] =
	{
		{mainRect.right - gridSquareSize, mainRect.top + gridSquareSize},
		{mainRect.right - gridSquareSize, mainRect.top + 7 * gridSquareSize},
		{mainRect.right - 7 * gridSquareSize, mainRect.top + 7 * gridSquareSize}
	};

	CPen* oldPen = pDC->SelectObject(&redPen);
	CBrush* oldBrush = pDC->SelectObject(&redBrush);
	pDC->Polygon(redTrianglePoints, 3);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	redBrush.DeleteObject();
	redPen.DeleteObject();
}

void CLab1View::DrawRightGrayTriangle(CDC* pDC)
{
	CPen redPen(PS_SOLID, 5, RGB(255, 0, 0));
	CBrush grayBrush(RGB(221, 221, 221));

	POINT grayTrianglePoints[] =
	{
		{mainRect.right - gridSquareSize, mainRect.top + 7 * gridSquareSize},
		{mainRect.right - 7 * gridSquareSize, mainRect.top + 7 * gridSquareSize},
		{mainRect.right - 7 * gridSquareSize, mainRect.bottom - 7 * gridSquareSize}
	};

	CPen* oldPen = pDC->SelectObject(&redPen);
	CBrush* oldBrush = pDC->SelectObject(&grayBrush);
	pDC->Polygon(grayTrianglePoints, 3);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	grayBrush.DeleteObject();
	redPen.DeleteObject();
}

void CLab1View::DrawGrid(CDC* pDC)
{
	CPen gridWhitePen(PS_SOLID, 2, RGB(255, 255, 255));
	POINT verticalGridlinePoints[40];
	POINT horizontalGridlinePoints[40];
	DWORD pointCount[40];
	for (int i = 0; i < 40; i++)
		pointCount[i] = 2; // all polyLines have 2 points

	for (int i = 0; i < 40 - 1; i += 2)
	{
		verticalGridlinePoints[i] = { mainRect.left + i/2 * gridSquareSize, mainRect.top }; // top points
		verticalGridlinePoints[i + 1] = { mainRect.left + i/2 * gridSquareSize, mainRect.bottom }; // bottom points
		horizontalGridlinePoints[i] = { mainRect.left, mainRect.top + i/2 * gridSquareSize }; // left points
		horizontalGridlinePoints[i + 1] = { mainRect.right, mainRect.top + i/2 * gridSquareSize }; // right points
	}

	POINT allPoints[80];
	for (int i = 0; i < 40; i++)
	{
		allPoints[i] = verticalGridlinePoints[i];
		allPoints[40 + i] = horizontalGridlinePoints[i];
	}

	CPen* oldPen = pDC->SelectObject(&gridWhitePen);
	pDC->PolyPolyline(allPoints, pointCount, 40);

	pDC->SelectObject(oldPen);
	gridWhitePen.DeleteObject();
}

double CLab1View::DistanceBetweenTwoPoints(POINT p1, POINT p2)
{
	return sqrt(pow(abs(p1.x - p2.x), 2) + pow(abs(p1.y - p2.y), 2));
}

// CLab1View message handlers


void CLab1View::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRgn rgn; 
	rgn.CreateRectRgn(gridButton.left, gridButton.top, gridButton.right, gridButton.bottom);
	if (rgn.PtInRegion(point)) 
	{ 
		drawGridButtonClicked = !drawGridButtonClicked;
		Invalidate(); 
	}

	CView::OnLButtonUp(nFlags, point);
}