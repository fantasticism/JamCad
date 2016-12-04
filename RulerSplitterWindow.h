#pragma once

#include "JamCadDefines.h"

/////////////////////////////////////////////////////////////////////////////
// CRulerSplitterWnd window

class CRulerSplitterWnd : public CSplitterWnd
{
	// Construction
public:
	CRulerSplitterWnd();
	BOOL CreateRulers(CWnd* pParent, CCreateContext* pContext, DWORD dwStyle = WS_CHILD | WS_VISIBLE, UINT nID = AFX_IDW_PANE_FIRST);
	void ShowRulers(BOOL bShow = TRUE, BOOL bSave = TRUE);
	void UpdateRulersInfo(stRULER_INFO stRulerInfo);

	// Attributes
private:
	BOOL m_bRulersVisible;

	// Implementation
public:
	virtual ~CRulerSplitterWnd();

	// Generated message map functions
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
};
