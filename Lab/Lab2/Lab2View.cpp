
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

void CLab2View::DrawBackground(CDC*)
{
}

void CLab2View::DrawStaticPartOfTheCactus(CDC*)
{
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

void CLab2View::DrawGrid(CDC*)
{
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
