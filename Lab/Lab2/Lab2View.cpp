
// Lab2View.cpp : implementation of the CLab2View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab2.h"
#endif

#include "Lab2Doc.h"
#include "Lab2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES
#include <math.h>

// CLab2View

IMPLEMENT_DYNCREATE(CLab2View, CView)

BEGIN_MESSAGE_MAP(CLab2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CLab2View construction/destruction

CLab2View::CLab2View() noexcept
{
	mainRectSideSize = 500;
	gridSquareSize = mainRectSideSize / 20;
	mainRect = CRect(0, 0, mainRectSideSize, mainRectSideSize);
	leftCactusPartDeg = 135;
	rightCactusPartDeg = 0;
	gridToggled = false;
}

CLab2View::~CLab2View()
{
}

BOOL CLab2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab2View drawing

void CLab2View::OnDraw(CDC* pDC)
{
	DrawBackground(pDC);
	DrawStaticPartOfTheCactus(pDC);
	DrawLeftMovingPartOfTheCactus(pDC);
	DrawRightMovingPartOfTheCactus(pDC);
	DrawFlowerPot(pDC);
	DrawStudentInfo(pDC);
	if (gridToggled)
		DrawGrid(pDC);

	/*int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	XFORM xform, xformOld;
	GetWorldTransform(pDC->m_hDC, &xformOld);

	CRect rect(300, 300, 500, 500);

	double piSeventh = M_PI / 4.0;
	xform.eM11 = 1.0f;
	xform.eM12 = 0.0f;
	xform.eM21 = 0.0f;
	xform.eM22 = 1.0f;
	xform.eDx = -rect.CenterPoint().x;
	xform.eDy = -rect.CenterPoint().y;
	SetWorldTransform(pDC->m_hDC, &xform);

	xform.eM11 = cos(piSeventh);
	xform.eM12 = sin(piSeventh);
	xform.eM21 = -sin(piSeventh);
	xform.eM22 = cos(piSeventh);
	xform.eDx = rect.CenterPoint().x;
	xform.eDy = rect.CenterPoint().y;
	ModifyWorldTransform(pDC->m_hDC, &xform, MWT_RIGHTMULTIPLY);
	pDC->Rectangle(rect);


	SetWorldTransform(pDC->m_hDC, &xformOld);
	SetGraphicsMode(pDC->m_hDC, prevMode);*/
}


// CLab2View printing

BOOL CLab2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab2View diagnostics

#ifdef _DEBUG
void CLab2View::AssertValid() const
{
	CView::AssertValid();
}

void CLab2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

void CLab2View::DrawBackground(CDC* pDC)
{
	CBrush skyBrush(RGB(135, 206, 235));
	CBrush* oldBrush = pDC->SelectObject(&skyBrush);
	CGdiObject* oldPen = pDC->SelectStockObject(NULL_PEN);
	pDC->Rectangle(mainRect);

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
	skyBrush.DeleteObject();
}

void CLab2View::DrawStaticPartOfTheCactus(CDC* pDC)
{
	HENHMETAFILE cactusPart = GetEnhMetaFile(CString("res/cactus_part.emf"));

	// first from bottom not rotated cactus part
	CRect middleCactusPart(int(mainRect.left + 8.5 * gridSquareSize),
		mainRect.bottom - 6 * gridSquareSize,
		int(mainRect.right - 8.5 * gridSquareSize),
		mainRect.bottom - 3 * gridSquareSize);
	PlayEnhMetaFile(pDC->m_hDC, cactusPart, middleCactusPart);

	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	XFORM xform, xformOld;
	GetWorldTransform(pDC->m_hDC, &xformOld);
	double firstRotatedCactusSide = 2.0 * gridSquareSize;
	double firstRotatedCactusDiag = sqrt(2.0 * pow(firstRotatedCactusSide, 2));
	CRect firstRotatedCactusPart(int(mainRect.left + 10.2 * gridSquareSize), 
		int(mainRect.bottom - 5.8 * gridSquareSize - firstRotatedCactusDiag),
		mainRect.right - 8 * gridSquareSize,
		int(mainRect.bottom - 5.4 * gridSquareSize));
	xform.eM11 = 1.0f;
	xform.eM12 = 0.0f;
	xform.eM21 = 0.0f;
	xform.eM22 = 1.0f;
	xform.eDx = float(-firstRotatedCactusPart.CenterPoint().x);
	xform.eDy = float(-firstRotatedCactusPart.CenterPoint().y);
	SetWorldTransform(pDC->m_hDC, &xform);

	double piFourth = M_PI / 4.0;
	xform.eM11 = float(cos(piFourth));
	xform.eM12 = float(sin(piFourth));
	xform.eM21 = float(-sin(piFourth));
	xform.eM22 = float(cos(piFourth));
	xform.eDx = float(firstRotatedCactusPart.CenterPoint().x);
	xform.eDy = float(firstRotatedCactusPart.CenterPoint().y);
	ModifyWorldTransform(pDC->m_hDC, &xform, MWT_RIGHTMULTIPLY);

	PlayEnhMetaFile(pDC->m_hDC, cactusPart, firstRotatedCactusPart);
	CBrush darkGreenBrush(RGB(0, 204, 0));
	CBrush* oldBrush = pDC->SelectObject(&darkGreenBrush);
	CRect topRightCircle(int(firstRotatedCactusPart.left + 0.5 * gridSquareSize),
		firstRotatedCactusPart.top - 0.4 * gridSquareSize,
		int(firstRotatedCactusPart.right - 0.5 * gridSquareSize),
		firstRotatedCactusPart.top + 0.4 * gridSquareSize);
	PlayEnhMetaFile(pDC->m_hDC, cactusPart, CRect(
		topRightCircle.left,
		topRightCircle.top - firstRotatedCactusDiag,
		topRightCircle.right,
		topRightCircle.CenterPoint().y));

	// restore old transform 
	SetWorldTransform(pDC->m_hDC, &xformOld);

	PlayEnhMetaFile(pDC->m_hDC, cactusPart, CRect(
		mainRect.right - 8.2 * gridSquareSize,
		mainRect.bottom - 8 * gridSquareSize - firstRotatedCactusDiag,
		mainRect.right - 7.4 * gridSquareSize,
		mainRect.bottom - 8 * gridSquareSize));
	pDC->Ellipse(CRect(mainRect.right - (8 + 0.2) * gridSquareSize,
		mainRect.bottom - (9 - 0.4) * gridSquareSize,
		mainRect.right - (8 - 0.6) * gridSquareSize,
		mainRect.bottom - (8 - 0.2) * gridSquareSize));

	// static circles
	CRect bottomCircle(int(mainRect.left + 9.6 * gridSquareSize),
		int(mainRect.bottom - 3.4 * gridSquareSize),
		int(mainRect.left + 10.4 * gridSquareSize),
		int(mainRect.bottom - 2.6 * gridSquareSize));
	pDC->Ellipse(bottomCircle);

	CRect topCircle(bottomCircle.left,
		int(mainRect.bottom - 6.4 * gridSquareSize),
		bottomCircle.right,
		int(mainRect.bottom - 5.6 * gridSquareSize));
	pDC->Ellipse(topCircle);

	// restore old DC objects
	pDC->SelectObject(oldBrush);
	darkGreenBrush.DeleteObject();
	SetGraphicsMode(pDC->m_hDC, prevMode);
	DeleteEnhMetaFile(cactusPart);
}

void CLab2View::DrawLeftMovingPartOfTheCactus(CDC*)
{
}

void CLab2View::DrawRightMovingPartOfTheCactus(CDC*)
{
}

void CLab2View::DrawFlowerPot(CDC*)
{
}

void CLab2View::DrawStudentInfo(CDC*)
{
}

void CLab2View::DrawGrid(CDC* pDC)
{
	CPen gridWhitePen(PS_SOLID, 2, RGB(255, 255, 255));
	POINT verticalGridlinePoints[40];
	POINT horizontalGridlinePoints[40];
	DWORD pointCount[40];
	for (int i = 0; i < 40; i++)
		pointCount[i] = 2; // all polyLines have 2 points

	for (int i = 0; i < 40 - 1; i += 2)
	{
		verticalGridlinePoints[i] = { mainRect.left + i / 2 * gridSquareSize, mainRect.top }; // top points
		verticalGridlinePoints[i + 1] = { mainRect.left + i / 2 * gridSquareSize, mainRect.bottom }; // bottom points
		horizontalGridlinePoints[i] = { mainRect.left, mainRect.top + i / 2 * gridSquareSize }; // left points
		horizontalGridlinePoints[i + 1] = { mainRect.right, mainRect.top + i / 2 * gridSquareSize }; // right points
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

int CLab2View::mod(int k, int n) { return ((k %= n) < 0) ? k + n : k; }

CLab2Doc* CLab2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab2Doc)));
	return (CLab2Doc*)m_pDocument;
}
#endif //_DEBUG


// CLab2View message handlers


void CLab2View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
	case VK_RETURN:
		gridToggled = !gridToggled;
		Invalidate();
		break;
	case VK_LEFT:
		rightCactusPartDeg = mod((rightCactusPartDeg - 6), 360);
		Invalidate();
		break;
	case VK_RIGHT:
		rightCactusPartDeg = mod((rightCactusPartDeg + 6), 360);
		Invalidate();
		break;
	case 0x41: // A
		leftCactusPartDeg = mod((rightCactusPartDeg - 6), 360);
		Invalidate();
		break;
	case 0x44: // D
		leftCactusPartDeg = mod((rightCactusPartDeg + 6), 360);
		Invalidate();
		break;
	}

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
