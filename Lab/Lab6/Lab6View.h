
// Lab6View.h : interface of the CLab6View class
//

#pragma once
#include "GLRenderer.h"


class CLab6View : public CView
{
protected: // create from serialization only
	CLab6View() noexcept;
	DECLARE_DYNCREATE(CLab6View)

// Attributes
public:
	CLab6Doc* GetDocument() const;

private:
	CGLRenderer m_glRenderer;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CLab6View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // debug version in Lab6View.cpp
inline CLab6Doc* CLab6View::GetDocument() const
   { return reinterpret_cast<CLab6Doc*>(m_pDocument); }
#endif

