
// Lab3View.cpp : implementation of the CLab3View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab3.h"
#endif

#include "Lab3Doc.h"
#include "Lab3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES
#include <math.h>
#include "DImage.h"

// CLab3View

IMPLEMENT_DYNCREATE(CLab3View, CView)

BEGIN_MESSAGE_MAP(CLab3View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CLab3View construction/destruction

CLab3View::CLab3View() noexcept
{
	mainRectSideSize = 500;
	gridSquareSize = mainRectSideSize / 20;
	mainRect = CRect(0, 0, mainRectSideSize, mainRectSideSize);
}

CLab3View::~CLab3View()
{
}

BOOL CLab3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab3View drawing

void CLab3View::OnDraw(CDC* pDC)
{
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap a;
	a.CreateCompatibleBitmap(pDC, mainRect.Width(), mainRect.Height());
	memDC->SelectObject(&a);
	a.DeleteObject();
	memDC->BitBlt(0, 0, mainRect.Width(), mainRect.Height(), pDC, 0, 0, SRCCOPY);

	DrawInMemDC(memDC);

	pDC->BitBlt(0, 0, 500, 500, memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();
	delete memDC;
}


// CLab3View printing

BOOL CLab3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab3View diagnostics

#ifdef _DEBUG
void CLab3View::AssertValid() const
{
	CView::AssertValid();
}

void CLab3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

void CLab3View::DrawInMemDC(CDC* pDC)
{
	DrawGrid(pDC);
	DrawTopLeft(pDC);
	DrawTopMiddle(pDC);
	DrawTopRight(pDC);
	DrawCenterLeft(pDC);
	DrawCenterMiddle(pDC);
	DrawCenterRight(pDC);
	DrawBottomLeft(pDC);
	DrawBottomMiddle(pDC);
	DrawBottomRight(pDC);
}

CBitmap* CLab3View::MakeImageTransparentAndReturnMask(CDC* pDC, CBitmap* image, int width, int height)
{
	CBitmap* bmpMask = new CBitmap();
	bmpMask->CreateBitmap(width, height, 1, 1, NULL);
	CDC SrcDC;
	SrcDC.CreateCompatibleDC(pDC);
	CDC DstDC;
	DstDC.CreateCompatibleDC(pDC);
	CBitmap* pOldSrcBmp = SrcDC.SelectObject(image);
	CBitmap* pOldDstBmp = DstDC.SelectObject(bmpMask);

	COLORREF clrTopLeft = SrcDC.GetPixel(0, 0);
	SrcDC.SetBkColor(clrTopLeft);
	DstDC.BitBlt(0, 0, width, height, &SrcDC, 0, 0, SRCCOPY);

	SrcDC.SetTextColor(RGB(255, 255, 255));
	SrcDC.SetBkColor(RGB(0, 0, 0));
	SrcDC.BitBlt(0, 0, width, height, &DstDC, 0, 0, SRCAND);
	SrcDC.DeleteDC();
	DstDC.DeleteDC();

	return bmpMask;
}

void CLab3View::DrawGrid(CDC* pDC)
{
	CPen gridWhitePen(PS_SOLID, 2, RGB(221, 221, 221));
	POINT verticalGridlinePoints[42];
	POINT horizontalGridlinePoints[42];
	DWORD pointCount[42];
	for (int i = 0; i < 42; i++)
		pointCount[i] = 2; // all polyLines have 2 points

	for (int i = 0; i < 42 - 1; i += 2)
	{
		verticalGridlinePoints[i] = { mainRect.left + i / 2 * gridSquareSize, mainRect.top }; // top points
		verticalGridlinePoints[i + 1] = { mainRect.left + i / 2 * gridSquareSize, mainRect.bottom }; // bottom points
		horizontalGridlinePoints[i] = { mainRect.left, mainRect.top + i / 2 * gridSquareSize }; // left points
		horizontalGridlinePoints[i + 1] = { mainRect.right, mainRect.top + i / 2 * gridSquareSize }; // right points
	}

	POINT allPoints[84];
	for (int i = 0; i < 42; i++)
	{
		allPoints[i] = verticalGridlinePoints[i];
		allPoints[42 + i] = horizontalGridlinePoints[i];
	}

	CPen* oldPen = pDC->SelectObject(&gridWhitePen);
	pDC->PolyPolyline(allPoints, pointCount, 42);

	pDC->SelectObject(oldPen);
	gridWhitePen.DeleteObject();
}

void CLab3View::DrawTopLeft(CDC* pDC)
{
	DImage dimg;
	dimg.Load(CString("res/11.dib"));
	int w = dimg.Width();
	int h = dimg.Height();
	CBitmap* bmpImage = dimg.GetCBitmap();

	// transparency
	CBitmap* mask = MakeImageTransparentAndReturnMask(pDC, bmpImage, w, h);

	XFORM oldTransform;
	GetWorldTransform(pDC->m_hDC, &oldTransform);

	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	ModifyWorldTransformTranslate(pDC, -0.8 * gridSquareSize, -gridSquareSize, false);
	ModifyWorldTransformTranslate(pDC, w / 2, h / 2, false);
	ModifyWorldTransformMirror(pDC, false, true, false);
	ModifyWorldTransformRotate(pDC, 57, false);
	ModifyWorldTransformTranslate(pDC, -w / 2, -h / 2, false);
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	memDC->SelectObject(mask);
	pDC->BitBlt(0, 0, w, h, memDC, 0, 0, SRCAND);
	memDC->SelectObject(bmpImage);
	pDC->BitBlt(0, 0, w, h, memDC, 0, 0, SRCPAINT);

	memDC->DeleteDC();
	delete memDC;

	mask->DeleteObject();
	delete mask;
	SetWorldTransform(pDC->m_hDC, &oldTransform);
	SetGraphicsMode(pDC->m_hDC, prevMode);
}

void CLab3View::DrawTopMiddle(CDC* pDC)
{
	DImage dimg;
	dimg.Load(CString("res/12.dib"));
	int w = dimg.Width();
	int h = dimg.Height();
	CBitmap* bmpImage = dimg.GetCBitmap();

	// transparency
	CBitmap* mask = MakeImageTransparentAndReturnMask(pDC, bmpImage, w, h);

	XFORM oldTransform;
	GetWorldTransform(pDC->m_hDC, &oldTransform);

	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	ModifyWorldTransformTranslate(pDC, 5.2 * gridSquareSize, -1.25 * gridSquareSize, false);
	ModifyWorldTransformTranslate(pDC, w / 2, h / 2, false);
	ModifyWorldTransformMirror(pDC, true, false, false);
	ModifyWorldTransformRotate(pDC, -80, false);
	ModifyWorldTransformTranslate(pDC, -w / 2, -h / 2, false);
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	memDC->SelectObject(mask);
	pDC->BitBlt(0, 0, w, h, memDC, 0, 0, SRCAND);
	memDC->SelectObject(bmpImage);
	pDC->BitBlt(0, 0, w, h, memDC, 0, 0, SRCPAINT);

	memDC->DeleteDC();
	delete memDC;

	mask->DeleteObject();
	delete mask;
	SetWorldTransform(pDC->m_hDC, &oldTransform);
	SetGraphicsMode(pDC->m_hDC, prevMode);
}

void CLab3View::DrawTopRight(CDC* pDC)
{
	DImage dimg;
	dimg.Load(CString("res/13.dib"));
	int w = dimg.Width();
	int h = dimg.Height();
	CBitmap* bmpImage = dimg.GetCBitmap();

	// transparency
	CBitmap* mask = MakeImageTransparentAndReturnMask(pDC, bmpImage, w, h);

	XFORM oldTransform;
	GetWorldTransform(pDC->m_hDC, &oldTransform);

	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	ModifyWorldTransformTranslate(pDC, 11 * gridSquareSize, -0.8 * gridSquareSize, false);
	ModifyWorldTransformTranslate(pDC, w / 2, h / 2, false);
	ModifyWorldTransformMirror(pDC, false, true, false);
	ModifyWorldTransformRotate(pDC, 16, false);
	ModifyWorldTransformTranslate(pDC, -w / 2, -h / 2, false);
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	memDC->SelectObject(mask);
	pDC->BitBlt(0, 0, w, h, memDC, 0, 0, SRCAND);
	memDC->SelectObject(bmpImage);
	pDC->BitBlt(0, 0, w, h, memDC, 0, 0, SRCPAINT);

	memDC->DeleteDC();
	delete memDC;

	mask->DeleteObject();
	delete mask;
	SetWorldTransform(pDC->m_hDC, &oldTransform);
	SetGraphicsMode(pDC->m_hDC, prevMode);
}

void CLab3View::DrawCenterLeft(CDC* pDC)
{
	DImage dimg;
	dimg.Load(CString("res/21.dib"));
	int w = dimg.Width();
	int h = dimg.Height();
	CBitmap* bmpImage = dimg.GetCBitmap();

	// transparency
	CBitmap* mask = MakeImageTransparentAndReturnMask(pDC, bmpImage, w, h);

	XFORM oldTransform;
	GetWorldTransform(pDC->m_hDC, &oldTransform);

	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	ModifyWorldTransformTranslate(pDC, -1.2 * gridSquareSize, 4.6 * gridSquareSize, false);
	ModifyWorldTransformTranslate(pDC, w / 2, h / 2, false);
	ModifyWorldTransformMirror(pDC, false, true, false);
	ModifyWorldTransformRotate(pDC, 171, false);
	ModifyWorldTransformTranslate(pDC, -w / 2, -h / 2, false);
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	memDC->SelectObject(mask);
	pDC->BitBlt(0, 0, w, h, memDC, 0, 0, SRCAND);
	memDC->SelectObject(bmpImage);
	pDC->BitBlt(0, 0, w, h, memDC, 0, 0, SRCPAINT);

	memDC->DeleteDC();
	delete memDC;

	mask->DeleteObject();
	delete mask;
	SetWorldTransform(pDC->m_hDC, &oldTransform);
	SetGraphicsMode(pDC->m_hDC, prevMode);
}

void CLab3View::DrawCenterMiddle(CDC* pDC)
{
}

void CLab3View::DrawCenterRight(CDC* pDC)
{
}

void CLab3View::DrawBottomLeft(CDC* pDC)
{
}

void CLab3View::DrawBottomMiddle(CDC* pDC)
{
}

void CLab3View::DrawBottomRight(CDC* pDC)
{
}

void CLab3View::ModifyWorldTransformRotate(CDC* pDC, float angle, bool rightMultiply)
{
	double radians = angle * M_PI / 180.0;
	XFORM xform;
	xform.eM11 = float(cos(radians));
	xform.eM12 = float(sin(radians));
	xform.eM21 = float(-sin(radians));
	xform.eM22 = float(cos(radians));
	xform.eDx = 0.0f;
	xform.eDy = 0.0f;
	ModifyWorldTransform(pDC->m_hDC, &xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLab3View::ModifyWorldTransformTranslate(CDC* pDC, float eDx, float eDy, bool rightMultiply)
{
	XFORM xform;
	xform.eM11 = 1.0f;
	xform.eM12 = 0.0f;
	xform.eM21 = 0.0f;
	xform.eM22 = 1.0f;
	xform.eDx = eDx;
	xform.eDy = eDy;
	ModifyWorldTransform(pDC->m_hDC, &xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLab3View::ModifyWorldTransformScale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xform;
	xform.eM11 = sX;
	xform.eM12 = 0.0f;
	xform.eM21 = 0.0f;
	xform.eM22 = sY;
	xform.eDx = 0.f;
	xform.eDy = 0.f;
	ModifyWorldTransform(pDC->m_hDC, &xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLab3View::ModifyWorldTransformMirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM xform;
	xform.eM11 = mx ? -1.0f : 1.0f;
	xform.eM12 = 0.0f;
	xform.eM21 = 0.0f;
	xform.eM22 = my ? -1.0f : 1.0f;
	xform.eDx = 0.f;
	xform.eDy = 0.f;
	ModifyWorldTransform(pDC->m_hDC, &xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

CLab3Doc* CLab3View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab3Doc)));
	return (CLab3Doc*)m_pDocument;
}
#endif //_DEBUG


// CLab3View message handlers
