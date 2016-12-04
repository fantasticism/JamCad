#if !defined(AFX_STATICPREVIEW_H__5C700A14_0191_4CE9_B36F_D5F0B59B0E95__INCLUDED_)
#define AFX_STATICPREVIEW_H__5C700A14_0191_4CE9_B36F_D5F0B59B0E95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticPreview.h : header file
//

enum StaticPreivewMode {
	STATICPREVIEW_OFFSET_CENTER,
	STATICPREVIEW_OFFSET_X10,
	STATICPREVIEW_OFFSET_Y10
};
#include "CadObject.h"

/////////////////////////////////////////////////////////////////////////////
// CStaticPreview window

class CStaticPreview : public CStatic
{
// Construction
	CScale m_Scale;
	CPoint m_Offset;
	int m_X, m_Y;
	int m_OffsetMode;
public:
	CStaticPreview();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticPreview)
	//}}AFX_VIRTUAL

// Implementation
public:
	CCadObject *m_pObj;
	virtual ~CStaticPreview();
	inline void EnableAxis(int x_en, int y_en) { m_X = x_en; m_Y = y_en; }
	inline void SetScale(CScale scale) { m_Scale = scale; }
	inline void SetOffsetMode(int m) { m_OffsetMode = m; }
	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticPreview)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	void Init();
	void AutoScale(CRect &rect);
	void SetViewOffset(CPoint Point);
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICPREVIEW_H__5C700A14_0191_4CE9_B36F_D5F0B59B0E95__INCLUDED_)
