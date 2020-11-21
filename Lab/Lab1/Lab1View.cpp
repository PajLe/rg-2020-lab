
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

void CLab1View::OnDraw(CDC* /*pDC*/)
{
	CLab1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
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


// CLab1View message handlers


void CLab1View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonUp(nFlags, point);
}
