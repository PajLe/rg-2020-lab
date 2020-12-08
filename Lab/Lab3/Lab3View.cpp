
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

void CLab3View::DrawGrid(CDC* pDC)
{
}

void CLab3View::DrawTopLeft(CDC* pDC)
{
	DImage dimg;
	dimg.Load(CString("res/12.dib"));
	int w = dimg.Width();
	int h = dimg.Height();
	CBitmap* bmpImage = dimg.GetCBitmap();

	/////////////////////////////////////// modify
	CBitmap bmpMask;
	bmpMask.CreateBitmap(w, h, 1, 1, NULL);
	CDC SrcDC;
	SrcDC.CreateCompatibleDC(pDC);
	CDC DstDC;
	DstDC.CreateCompatibleDC(pDC);
	CBitmap* pOldSrcBmp = SrcDC.SelectObject(bmpImage);
	CBitmap* pOldDstBmp = DstDC.SelectObject(&bmpMask);

	COLORREF clrTopLeft = SrcDC.GetPixel(0, 0);
	SrcDC.SetBkColor(clrTopLeft);
	DstDC.BitBlt(0, 0, w, h, &SrcDC, 0, 0, SRCCOPY);

	SrcDC.SetTextColor(RGB(255, 255, 255));
	SrcDC.SetBkColor(RGB(0, 0, 0));
	SrcDC.BitBlt(0, 0, w, h, &DstDC, 0, 0, SRCAND);
	SrcDC.DeleteDC();
	DstDC.DeleteDC();
	/////////////
	CDC* MemDC = new CDC();
	MemDC->CreateCompatibleDC(pDC);
	CBitmap* bmpOldT = MemDC->SelectObject(&bmpMask);
	pDC->BitBlt(0, 0, w, h, MemDC, 0, 0, SRCAND);
	MemDC->SelectObject(bmpImage);
	pDC->BitBlt(0, 0, w, h, MemDC, 0, 0, SRCPAINT);
	MemDC->SelectObject(bmpOldT);
	delete MemDC;
	//pDC->BitBlt(0, 0, w, h, &SrcDC, 0, 0, SRCCOPY);

	bmpMask.DeleteObject();
}

void CLab3View::DrawTopMiddle(CDC* pDC)
{
}

void CLab3View::DrawTopRight(CDC* pDC)
{
}

void CLab3View::DrawCenterLeft(CDC* pDC)
{
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
