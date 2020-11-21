
// Lab1View.h : interface of the CLab1View class
//

#pragma once


class CLab1View : public CView
{
protected: // create from serialization only
	CLab1View() noexcept;
	DECLARE_DYNCREATE(CLab1View)

// Attributes
public:
	CLab1Doc* GetDocument() const;

private:
	bool drawGridButtonClicked;
	CRect gridButton;
	CRect mainRect;
	int mainRectSideSize;
	int gridSquareSize;

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
	virtual ~CLab1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

private:
	void DrawGridButton(CDC*);
	void DrawBackgroundRect(CDC*);
	void DrawYellowTriangle(CDC*);
	void DrawPinkParallelogram(CDC*);
	void DrawRedTriangle(CDC*);
	void DrawLeftGrayTriangle(CDC*);
	void DrawOrangeSquare(CDC*);
	void DrawRightGrayTriangle(CDC*);
	void DrawGreenTriangle(CDC*);
	void DrawHachedTriangle(CDC*);
	void DrawPurpleTriangle(CDC*);
	void DrawGrid(CDC*);
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in Lab1View.cpp
inline CLab1Doc* CLab1View::GetDocument() const
   { return reinterpret_cast<CLab1Doc*>(m_pDocument); }
#endif

