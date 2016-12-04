#pragma once
#include "JamCadDefines.h"

/////////////////////////////////////////////////////////////////////////////
// CRulerView view

class CRulerView : public CView
{
	// Attributes
	UINT   m_rulerType;
	CPoint m_scrollPos;
	CPoint m_lastPos;
	CSize  m_DocSize;
	float  m_fZoomFactor;
	CDC *m_pMemDC;
	CBitmap *m_pBitMap;
	CFont *m_pHFont;
	CFont *m_pVFont;
	char **m_ppTickLabels;	//pointer to an array of tick labels
	int m_nTickLables;		//max number of tick labels
protected:
	CRulerView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CRulerView)


	// Operations
public:
	inline void SetRulerType(UINT rulerType = RT_HORIZONTAL){m_rulerType = rulerType;}
	void UpdateRulersInfo(stRULER_INFO stRulerInfo);

private:
	void DrawTicker(CDC* pDC, CRect rulerRect, int nFactor, int nBegin = 0, BOOL bShowPos = TRUE);
	void DrawCursorPos(CPoint NewPos);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRulerView)
public:
	virtual void OnInitialUpdate();
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	// Implementation
protected:
	virtual ~CRulerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CRulerView)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void CreateMemDC();
	inline int GetType(void) { return m_rulerType; }
	inline int NeedsUpdate(int msg)
	{
		int rV = 0;
		if ((msg == RW_HSCROLL && GetType() == RT_HORIZONTAL) ||
			(msg == RW_VSCROLL && GetType() == RT_VERTICAL) ||
			(msg == RW_POSITION) ||
			(msg == RW_SETLABELS) ||
			(msg == RW_ZOOM)
			) rV = 1;
		return rV;
	}
};
