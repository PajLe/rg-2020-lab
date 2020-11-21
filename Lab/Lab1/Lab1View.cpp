
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
}

void CLab1View::DrawGreenTriangle(CDC* pDC)
{
}

void CLab1View::DrawHachedTriangle(CDC* pDC)
{
}

void CLab1View::DrawPurpleTriangle(CDC* pDC)
{
}

void CLab1View::DrawGridButton(CDC* pDC)
{
	CBrush gridButtonBrush(RGB(236, 236, 236));
	CBrush* oldBrush = pDC->SelectObject(&gridButtonBrush);
	pDC->Rectangle(gridButton);
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(CString("ISKLJUCI GRID (MREZU)"), gridButton, DT_CENTER);

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
}

void CLab1View::DrawGrid(CDC* pDC)
{
}

double CLab1View::DistanceBetweenTwoPoints(POINT p1, POINT p2)
{
	return sqrt(pow(abs(p1.x - p2.x), 2) + pow(abs(p1.y - p2.y), 2));
}

// CLab1View message handlers


void CLab1View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonUp(nFlags, point);
}