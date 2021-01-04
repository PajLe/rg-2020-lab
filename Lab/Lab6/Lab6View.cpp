
// Lab6View.cpp : implementation of the CLab6View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab6.h"
#endif

#include "Lab6Doc.h"
#include "Lab6View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab6View

IMPLEMENT_DYNCREATE(CLab6View, CView)

BEGIN_MESSAGE_MAP(CLab6View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CLab6View construction/destruction

CLab6View::CLab6View() noexcept
{
	// TODO: add construction code here

}

CLab6View::~CLab6View()
{
}

BOOL CLab6View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab6View drawing

void CLab6View::OnDraw(CDC* pDC)
{
	CLab6Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer.DrawScene(pDC);
}


// CLab6View printing

BOOL CLab6View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab6View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab6View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab6View diagnostics

#ifdef _DEBUG
void CLab6View::AssertValid() const
{
	CView::AssertValid();
}

void CLab6View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab6Doc* CLab6View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab6Doc)));
	return (CLab6Doc*)m_pDocument;
}
#endif //_DEBUG


// CLab6View message handlers


int CLab6View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


void CLab6View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}


BOOL CLab6View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	// return CView::OnEraseBkgnd(pDC);
}


void CLab6View::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_glRenderer.StopMovingCamera();

	CView::OnLButtonUp(nFlags, point);
}


void CLab6View::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{
		m_glRenderer.MoveCamera(point);
		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}


void CLab6View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CLab6View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}
