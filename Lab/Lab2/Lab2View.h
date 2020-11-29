
// Lab2View.h : interface of the CLab2View class
//

#pragma once


class CLab2View : public CView
{
protected: // create from serialization only
	CLab2View() noexcept;
	DECLARE_DYNCREATE(CLab2View)

// Attributes
public:
	CLab2Doc* GetDocument() const;

private:
	CRect mainRect;
	int mainRectSideSize;
	int gridSquareSize;
	int leftCactusPartDeg;
	int rightCactusPartDeg;
	bool gridToggled;
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
	virtual ~CLab2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	void DrawBackground(CDC*);
	void DrawStaticPartOfTheCactus(CDC*);
	void DrawLeftMovingPartOfTheCactus(CDC*);
	void DrawRightMovingPartOfTheCactus(CDC*);
	void DrawFlowerPot(CDC*);
	void DrawStudentInfo(CDC*);
	void DrawGrid(CDC*);
	int mod(int k, int n);

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in Lab2View.cpp
inline CLab2Doc* CLab2View::GetDocument() const
   { return reinterpret_cast<CLab2Doc*>(m_pDocument); }
#endif

