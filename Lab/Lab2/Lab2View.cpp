
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
	leftCactusPartDeg = 315;
	rightCactusPartDeg = 90;
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
	DrawLeftMovingPartOfTheCactus(pDC);
	DrawRightMovingPartOfTheCactus(pDC);
	DrawStaticPartOfTheCactus(pDC);
	DrawFlowerPot(pDC);
	DrawStudentInfo(pDC);
	if (gridToggled)
		DrawGrid(pDC);

	//int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	//XFORM xform, xformOld;
	//GetWorldTransform(pDC->m_hDC, &xformOld);

	//CRect rect(200, 200, 300, 350);
	//CBrush redBrush(RGB(255, 0, 0));
	//CRect rect2(300, 350, 450, 450);
	//CBrush greenBrush(RGB(0, 255, 0));

	//double piSeventh = M_PI / 2.0;

	//xform.eM11 = 1.0f;
	//xform.eM12 = 0.0f;
	//xform.eM21 = 0.0f;
	//xform.eM22 = 1.0f;
	//xform.eDx = rect.CenterPoint().x;
	//xform.eDy = rect2.CenterPoint().y;
	//SetWorldTransform(pDC->m_hDC, &xform);

	//xform.eM11 = cos(piSeventh);
	//xform.eM12 = sin(piSeventh);
	//xform.eM21 = -sin(piSeventh);
	//xform.eM22 = cos(piSeventh);
	//xform.eDx = 0.0;
	//xform.eDy = 0.0;
	//ModifyWorldTransform(pDC->m_hDC, &xform, MWT_LEFTMULTIPLY);

	//xform.eM11 = 1.0f;
	//xform.eM12 = 0.0f;
	//xform.eM21 = 0.0f;
	//xform.eM22 = 1.0f;
	//xform.eDx = -rect.CenterPoint().x;
	//xform.eDy = -rect2.CenterPoint().y;
	//ModifyWorldTransform(pDC->m_hDC, &xform, MWT_LEFTMULTIPLY);

	//CBrush* oldBrush = pDC->SelectObject(&redBrush);
	//pDC->Ellipse(rect);

	////ModifyWorldTransform(pDC->m_hDC, &xform, MWT_IDENTITY);

	//xform.eM11 = 1.0f;
	//xform.eM12 = 0.0f;
	//xform.eM21 = 0.0f;
	//xform.eM22 = 1.0f;
	//xform.eDx = rect2.CenterPoint().x;
	//xform.eDy = rect2.CenterPoint().y;
	//ModifyWorldTransform(pDC->m_hDC, &xform, MWT_LEFTMULTIPLY);

	//xform.eM11 = cos(piSeventh);
	//xform.eM12 = sin(piSeventh);
	//xform.eM21 = -sin(piSeventh);
	//xform.eM22 = cos(piSeventh);
	//xform.eDx = 0.0;
	//xform.eDy = 0.0;
	//ModifyWorldTransform(pDC->m_hDC, &xform, MWT_LEFTMULTIPLY);

	//xform.eM11 = 1.0f;
	//xform.eM12 = 0.0f;
	//xform.eM21 = 0.0f;
	//xform.eM22 = 1.0f;
	//xform.eDx = -rect2.CenterPoint().x;
	//xform.eDy = -rect2.CenterPoint().y;
	//ModifyWorldTransform(pDC->m_hDC, &xform, MWT_LEFTMULTIPLY);
	//
	//pDC->SelectObject(&greenBrush);
	//pDC->Ellipse(rect2);


	//SetGraphicsMode(pDC->m_hDC, prevMode);
	//SetWorldTransform(pDC->m_hDC, &xformOld);
	//pDC->SelectObject(oldBrush);
	//greenBrush.DeleteObject();
	//redBrush.DeleteObject();
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
	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	XFORM xformOld;
	GetWorldTransform(pDC->m_hDC, &xformOld);

	CRect bottomCircle(int(mainRect.left + 9.6 * gridSquareSize),
		int(mainRect.bottom - 3.4 * gridSquareSize),
		int(mainRect.left + 10.4 * gridSquareSize),
		int(mainRect.bottom - 2.6 * gridSquareSize));

	// first from bottom not rotated cactus part
	CRect middleCactusPart(mainRect.left + 8.8 * gridSquareSize,
		mainRect.bottom - 6 * gridSquareSize,
		mainRect.right - 8.8 * gridSquareSize,
		mainRect.bottom - 3 * gridSquareSize);
	PlayEnhMetaFile(pDC->m_hDC, cactusPart, middleCactusPart);

	CRect middleCircle(bottomCircle.left,
		int(mainRect.bottom - 6.4 * gridSquareSize),
		bottomCircle.right,
		int(mainRect.bottom - 5.6 * gridSquareSize));

	CRect firstRotatedCactusPart(
		mainRect.left + 9.2 * gridSquareSize,
		mainRect.bottom - 9 * gridSquareSize,
		mainRect.right - 9.2 * gridSquareSize,
		mainRect.bottom - 6 * gridSquareSize
	);
	SetWorldTransformTranslate(pDC, float(firstRotatedCactusPart.CenterPoint().x), float(firstRotatedCactusPart.bottom));
	ModifyWorldTransformRotate(pDC, 45 * M_PI / 180, MWT_LEFTMULTIPLY);
	ModifyWorldTransformTranslate(pDC, -float(firstRotatedCactusPart.CenterPoint().x), -float(firstRotatedCactusPart.bottom), MWT_LEFTMULTIPLY);
	PlayEnhMetaFile(pDC->m_hDC, cactusPart, firstRotatedCactusPart);
	
	CRect topCircle(
		mainRect.left + (10 - 0.4) * gridSquareSize,
		firstRotatedCactusPart.top - 0.4 * gridSquareSize,
		mainRect.left + (10 + 0.4) * gridSquareSize,
		firstRotatedCactusPart.top + 0.4 * gridSquareSize
	);

	CRect middleThinCactusPart(
		mainRect.left + 9.5 * gridSquareSize,
		mainRect.top + 8 * gridSquareSize,
		mainRect.right - 9.5 * gridSquareSize,
		mainRect.top + 11 * gridSquareSize
	);
	PlayEnhMetaFile(pDC->m_hDC, cactusPart, middleThinCactusPart);

	// rotate middle thin cactus part to the left by 45 deg = -pi/4
	ModifyWorldTransformTranslate(pDC, middleThinCactusPart.CenterPoint().x, middleThinCactusPart.bottom, MWT_LEFTMULTIPLY);
	ModifyWorldTransformRotate(pDC, -M_PI / 4.0, MWT_LEFTMULTIPLY);
	ModifyWorldTransformTranslate(pDC, -middleThinCactusPart.CenterPoint().x, -middleThinCactusPart.bottom, MWT_LEFTMULTIPLY);
	PlayEnhMetaFile(pDC->m_hDC, cactusPart, middleThinCactusPart);
	
	// reset world transforms and bring back only global part transform
	ModifyWorldTransformTranslate(pDC, 0.0f, 0.0f, MWT_IDENTITY);
	ModifyWorldTransformTranslate(pDC, float(firstRotatedCactusPart.CenterPoint().x), float(firstRotatedCactusPart.bottom), MWT_LEFTMULTIPLY);
	ModifyWorldTransformRotate(pDC, 45 * M_PI / 180, MWT_LEFTMULTIPLY);
	ModifyWorldTransformTranslate(pDC, -float(firstRotatedCactusPart.CenterPoint().x), -float(firstRotatedCactusPart.bottom), MWT_LEFTMULTIPLY);
	CBrush darkGreenBrush(RGB(0, 204, 0));
	CBrush* oldBrush = pDC->SelectObject(&darkGreenBrush);
	pDC->Ellipse(topCircle);

	// reset all world transforms to draw bottom circle
	ModifyWorldTransformTranslate(pDC, 0.0f, 0.0f, MWT_IDENTITY);
	pDC->Ellipse(bottomCircle);
	pDC->Ellipse(middleCircle);

	// restore old transform 
	SetWorldTransform(pDC->m_hDC, &xformOld);

	// restore old DC objects
	pDC->SelectObject(oldBrush);
	darkGreenBrush.DeleteObject();
	SetGraphicsMode(pDC->m_hDC, prevMode);
	DeleteEnhMetaFile(cactusPart);
}

void CLab2View::DrawLeftMovingPartOfTheCactus(CDC* pDC)
{
	HENHMETAFILE cactusPart = GetEnhMetaFile(CString("res/cactus_part.emf"));
	HENHMETAFILE rotatingCactusPart = GetEnhMetaFile(CString("res/cactus_part_light.emf"));
	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	XFORM xformOld;
	GetWorldTransform(pDC->m_hDC, &xformOld);

	CRect lightGreenCactusPart(
		mainRect.left + 9.2 * gridSquareSize,
		mainRect.bottom - 9 * gridSquareSize,
		mainRect.right - 9.2 * gridSquareSize,
		mainRect.bottom - 6 * gridSquareSize
	);
	SetWorldTransformTranslate(pDC, float(lightGreenCactusPart.CenterPoint().x), float(lightGreenCactusPart.bottom));
	ModifyWorldTransformRotate(pDC, leftCactusPartDeg * M_PI / 180, MWT_LEFTMULTIPLY);
	ModifyWorldTransformTranslate(pDC, -float(lightGreenCactusPart.CenterPoint().x), -float(lightGreenCactusPart.bottom), MWT_LEFTMULTIPLY);
	PlayEnhMetaFile(pDC->m_hDC, rotatingCactusPart, lightGreenCactusPart);

	CRect bottomCircle(
		mainRect.left + (10 - 0.4) * gridSquareSize,
		lightGreenCactusPart.top - 0.4 * gridSquareSize,
		mainRect.left + (10 + 0.4) * gridSquareSize,
		lightGreenCactusPart.top + 0.4 * gridSquareSize
	);

	CRect middleThinCactusPart(
		mainRect.left + 9.5 * gridSquareSize,
		mainRect.top + 8 * gridSquareSize,
		mainRect.right - 9.5 * gridSquareSize,
		mainRect.top + 11 * gridSquareSize
	);
	PlayEnhMetaFile(pDC->m_hDC, cactusPart, middleThinCactusPart);

	CRect topCircle(
		mainRect.left + (10 - 0.4) * gridSquareSize,
		middleThinCactusPart.top - 0.4 * gridSquareSize,
		mainRect.left + (10 + 0.4) * gridSquareSize,
		middleThinCactusPart.top + 0.4 * gridSquareSize
	);

	CRect topThickCactusPart(
		mainRect.left + 9 * gridSquareSize,
		mainRect.top + 5 * gridSquareSize,
		mainRect.right - 9 * gridSquareSize,
		mainRect.top + 8 * gridSquareSize
	);
	PlayEnhMetaFile(pDC->m_hDC, cactusPart, topThickCactusPart);

	// rotate thin cactus around its bottom to the right by pi/4
	ModifyWorldTransformTranslate(pDC, middleThinCactusPart.CenterPoint().x, middleThinCactusPart.bottom, MWT_LEFTMULTIPLY);
	ModifyWorldTransformRotate(pDC, M_PI / 4.0, MWT_LEFTMULTIPLY);
	ModifyWorldTransformTranslate(pDC, -middleThinCactusPart.CenterPoint().x, -middleThinCactusPart.bottom, MWT_LEFTMULTIPLY);
	PlayEnhMetaFile(pDC->m_hDC, cactusPart, middleThinCactusPart);

	// rotate thin cactus around its bottom to the left by pi/4 (reset to middle) + pi/4 (rotate to the left) = pi/2 to the left = -pi/2
	ModifyWorldTransformTranslate(pDC, middleThinCactusPart.CenterPoint().x, middleThinCactusPart.bottom, MWT_LEFTMULTIPLY);
	ModifyWorldTransformRotate(pDC, -M_PI / 2.0, MWT_LEFTMULTIPLY);
	ModifyWorldTransformTranslate(pDC, -middleThinCactusPart.CenterPoint().x, -middleThinCactusPart.bottom, MWT_LEFTMULTIPLY);
	PlayEnhMetaFile(pDC->m_hDC, cactusPart, middleThinCactusPart);

	// reset world transforms and bring back only global part transform
	ModifyWorldTransformTranslate(pDC, 0.0f, 0.0f, MWT_IDENTITY);
	ModifyWorldTransformTranslate(pDC, float(lightGreenCactusPart.CenterPoint().x), float(lightGreenCactusPart.bottom), MWT_LEFTMULTIPLY);
	ModifyWorldTransformRotate(pDC, leftCactusPartDeg * M_PI / 180, MWT_LEFTMULTIPLY);
	ModifyWorldTransformTranslate(pDC, -float(lightGreenCactusPart.CenterPoint().x), -float(lightGreenCactusPart.bottom), MWT_LEFTMULTIPLY);
	CBrush darkGreenBrush(RGB(0, 204, 0));
	CBrush* oldBrush = pDC->SelectObject(&darkGreenBrush);
	pDC->Ellipse(bottomCircle);
	pDC->Ellipse(topCircle);

	// restore old objects
	pDC->SelectObject(oldBrush);
	darkGreenBrush.DeleteObject();
	DeleteEnhMetaFile(cactusPart);
	DeleteEnhMetaFile(rotatingCactusPart);
	// restore old transform 
	SetWorldTransform(pDC->m_hDC, &xformOld);
}

void CLab2View::DrawRightMovingPartOfTheCactus(CDC* pDC)
{
	/*HENHMETAFILE cactusPart = GetEnhMetaFile(CString("res/cactus_part.emf"));
	HENHMETAFILE rotatingCactusPart = GetEnhMetaFile(CString("res/cactus_part_light.emf"));
	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	XFORM xformOld;
	GetWorldTransform(pDC->m_hDC, &xformOld);

	CRect lightGreenCactusPart(
		mainRect.right - 7.5 * gridSquareSize,
		mainRect.top + 8 * gridSquareSize,
		mainRect.right - 8.5 * gridSquareSize,
		mainRect.top + 11 * gridSquareSize
	);
	PlayEnhMetaFile(pDC->m_hDC, rotatingCactusPart, lightGreenCactusPart);

	SetGraphicsMode(pDC->m_hDC, prevMode);
	DeleteEnhMetaFile(cactusPart);
	DeleteEnhMetaFile(rotatingCactusPart);*/
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

void CLab2View::SetWorldTransformRotate(CDC* pDC, double radians)
{
	XFORM xform;
	xform.eM11 = float(cos(radians));
	xform.eM12 = float(sin(radians));
	xform.eM21 = float(-sin(radians));
	xform.eM22 = float(cos(radians));
	xform.eDx = 0.0f;
	xform.eDy = 0.0f;
	SetWorldTransform(pDC->m_hDC, &xform);
}

void CLab2View::SetWorldTransformTranslate(CDC* pDC, float eDx, float eDy)
{
	XFORM xform;
	xform.eM11 = 1.0f;
	xform.eM12 = 0.0f;
	xform.eM21 = 0.0f;
	xform.eM22 = 1.0f;
	xform.eDx = eDx;
	xform.eDy = eDy;
	SetWorldTransform(pDC->m_hDC, &xform);
}

void CLab2View::ModifyWorldTransformRotate(CDC* pDC, double radians, DWORD modifyMode)
{
	XFORM xform;
	xform.eM11 = float(cos(radians));
	xform.eM12 = float(sin(radians));
	xform.eM21 = float(-sin(radians));
	xform.eM22 = float(cos(radians));
	xform.eDx = 0.0f;
	xform.eDy = 0.0f;
	ModifyWorldTransform(pDC->m_hDC, &xform, modifyMode);
}

void CLab2View::ModifyWorldTransformTranslate(CDC* pDC, float eDx, float eDy, DWORD modifyMode)
{
	XFORM xform;
	xform.eM11 = 1.0f;
	xform.eM12 = 0.0f;
	xform.eM21 = 0.0f;
	xform.eM22 = 1.0f;
	xform.eDx = eDx;
	xform.eDy = eDy;
	ModifyWorldTransform(pDC->m_hDC, &xform, modifyMode);
}

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
		leftCactusPartDeg = mod((leftCactusPartDeg - 6), 360);
		Invalidate();
		break;
	case 0x44: // D
		leftCactusPartDeg = mod((leftCactusPartDeg + 6), 360);
		Invalidate();
		break;
	}

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
