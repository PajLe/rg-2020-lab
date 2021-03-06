
// Lab4View.cpp : implementation of the CLab4View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab4.h"
#endif

#include "Lab4Doc.h"
#include "Lab4View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab4View

IMPLEMENT_DYNCREATE(CLab4View, CView)

BEGIN_MESSAGE_MAP(CLab4View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CLab4View construction/destruction

CLab4View::CLab4View() noexcept
{
	// TODO: add construction code here

}

CLab4View::~CLab4View()
{
}

BOOL CLab4View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab4View drawing

void CLab4View::OnDraw(CDC* pDC)
{
	CLab4Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer.DrawScene(pDC);
}


// CLab4View printing

BOOL CLab4View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab4View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab4View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab4View diagnostics

#ifdef _DEBUG
void CLab4View::AssertValid() const
{
	CView::AssertValid();
}

void CLab4View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab4Doc* CLab4View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab4Doc)));
	return (CLab4Doc*)m_pDocument;
}
#endif //_DEBUG


// CLab4View message handlers


int CLab4View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


void CLab4View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CLab4View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}


BOOL CLab4View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	// return CView::OnEraseBkgnd(pDC);
}


void CLab4View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


void CLab4View::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{
		m_glRenderer.MoveCamera(point);
		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}


void CLab4View::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_glRenderer.StopMovingCamera();

	CView::OnLButtonUp(nFlags, point);
}


void CLab4View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_LEFT:
		m_glRenderer.RotateX(-10.0f);
		Invalidate();
		break;
	case VK_RIGHT:
		m_glRenderer.RotateX(10.0f);
		Invalidate();
		break;
	case 0x41: // A
		m_glRenderer.RotateY(-10.0f);
		Invalidate();
		break;
	case 0x44: // D
		m_glRenderer.RotateY(10.0f);
		Invalidate();
		break;
	}

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
