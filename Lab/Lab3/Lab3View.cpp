
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
	// TODO: add construction code here

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

void CLab3View::OnDraw(CDC* /*pDC*/)
{
	CLab3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
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

void CLab3View::ModifyWorldTransformRotate(CDC* pDC, double radians, bool rightMultiply)
{
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
