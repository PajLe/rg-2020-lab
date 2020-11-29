
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
END_MESSAGE_MAP()

// CLab2View construction/destruction

CLab2View::CLab2View() noexcept
{
	// TODO: add construction code here

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
	HENHMETAFILE a = GetEnhMetaFile(CString("res/cactus_part.emf"));
	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	XFORM xform, xformOld;
	GetWorldTransform(pDC->m_hDC, &xformOld);

	double piSeventh = M_PI / 7.0;
	xform.eM11 = cos(piSeventh);
	xform.eM12 = sin(piSeventh);
	xform.eM21 = -sin(piSeventh);
	xform.eM22 = cos(piSeventh);
	xform.eDx = 0.0; 
	xform.eDy = 0.0;
	SetWorldTransform(pDC->m_hDC, &xform);
	PlayEnhMetaFile(pDC->m_hDC, a, CRect(100, 100, 350, 450));

	SetWorldTransform(pDC->m_hDC, &xformOld);
	SetGraphicsMode(pDC->m_hDC, prevMode);
	DeleteEnhMetaFile(a);
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

CLab2Doc* CLab2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab2Doc)));
	return (CLab2Doc*)m_pDocument;
}
#endif //_DEBUG


// CLab2View message handlers
