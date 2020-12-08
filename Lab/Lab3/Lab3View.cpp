
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

CLab3Doc* CLab3View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab3Doc)));
	return (CLab3Doc*)m_pDocument;
}
#endif //_DEBUG


// CLab3View message handlers
