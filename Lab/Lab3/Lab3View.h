
// Lab3View.h : interface of the CLab3View class
//

#pragma once


class CLab3View : public CView
{
protected: // create from serialization only
	CLab3View() noexcept;
	DECLARE_DYNCREATE(CLab3View)

// Attributes
public:
	CLab3Doc* GetDocument() const;
private:
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
	virtual ~CLab3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
private:
	void DrawInMemDC(CDC* pDC);

	CBitmap* MakeImageTransparentAndReturnMask(CDC* pDC, CBitmap* image, int width, int height);
	void MakeImageGrayscale(CBitmap* bitmap);

	void DrawGrid(CDC* pDC);
	void DrawTopLeft(CDC* pDC);
	void DrawTopMiddle(CDC* pDC);
	void DrawTopRight(CDC* pDC);
	void DrawCenterLeft(CDC* pDC);
	void DrawCenterMiddle(CDC* pDC);
	void DrawCenterRight(CDC* pDC);
	void DrawBottomLeft(CDC* pDC);
	void DrawBottomMiddle(CDC* pDC);
	void DrawBottomRight(CDC* pDC);

	void ModifyWorldTransformRotate(CDC* pDC, float angle, bool rightMultiply);
	void ModifyWorldTransformTranslate(CDC* pDC, float eDx, float eDy, bool rightMultiply);
	void ModifyWorldTransformScale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void ModifyWorldTransformMirror(CDC* pDC, bool mx, bool my, bool rightMultiply);
protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Lab3View.cpp
inline CLab3Doc* CLab3View::GetDocument() const
   { return reinterpret_cast<CLab3Doc*>(m_pDocument); }
#endif

