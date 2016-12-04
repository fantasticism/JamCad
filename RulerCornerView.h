#pragma once
/////////////////////////////////////////////////////////////////////////////
// CRulerCornerView view

class CRulerCornerView : public CView
{
protected:
	CRulerCornerView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CRulerCornerView)

	// Attributes
public:

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRulerCornerView)
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

	// Implementation
protected:
	virtual ~CRulerCornerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CRulerCornerView)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
