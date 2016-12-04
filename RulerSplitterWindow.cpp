#include "stdafx.h"
#include "RulerSplitterWindow.h"
#include "RulerCornerView.h"
#include "RulerView.h"

/////////////////////////////////////////////////////////////////////////////
// CRulerSplitterWnd

CRulerSplitterWnd::CRulerSplitterWnd()
{
	m_cxSplitter = 0;
	m_cySplitter = 0;
	m_cxBorderShare = 0;
	m_cyBorderShare = 0;
	m_cxSplitterGap = 1;
	m_cySplitterGap = 1;
	m_bRulersVisible = FALSE;
}

CRulerSplitterWnd::~CRulerSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CRulerSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CRulerSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CRulerSplitterWnd::CreateRulers(CWnd* pParent, CCreateContext* pContext, DWORD dwStyle, UINT nID)
{
	if (!CreateStatic(pParent, 2, 2, dwStyle, nID))
		return FALSE;

	if (!CreateView(0, 0, RUNTIME_CLASS(CRulerCornerView), CSize(0, 0), pContext))
		return FALSE;
	if (!CreateView(0, 1, RUNTIME_CLASS(CRulerView), CSize(0, 0), pContext))
		return FALSE;
	if (!CreateView(1, 0, RUNTIME_CLASS(CRulerView), CSize(0, 0), pContext))
		return FALSE;
	if (!CreateView(1, 1, pContext->m_pNewViewClass, CSize(0, 0), pContext))
		return FALSE;

	SetColumnInfo(0, 0, 0);
	SetRowInfo(0, 0, 0);

	((CRulerView*)GetPane(0, 1))->SetRulerType(RT_HORIZONTAL);
	((CRulerView*)GetPane(1, 0))->SetRulerType(RT_VERTICAL);

	SetActivePane(1, 1);

	return TRUE;
}

void CRulerSplitterWnd::ShowRulers(BOOL bShow, BOOL bSave)
{
	int nSize = (bShow) ? RULER_SIZE : 0;
	int nSizeBorder = (bShow) ? 3 : 1;

	SetRowInfo(0, nSize, 0);
	SetColumnInfo(0, nSize, 0);
	m_cxSplitterGap = nSizeBorder;
	m_cySplitterGap = nSizeBorder;
	m_bRulersVisible = (bSave) ? bShow : m_bRulersVisible;
	RecalcLayout();
}

void CRulerSplitterWnd::UpdateRulersInfo(stRULER_INFO stRulerInfo)
{
	int Msg = stRulerInfo.uMessage;
	CRulerView *pRV;

	pRV = (CRulerView*)GetPane(0, 1);
	if(pRV->NeedsUpdate(Msg) )
		pRV->UpdateRulersInfo(stRulerInfo);
	pRV = ((CRulerView*)GetPane(1, 0));
	if(pRV->NeedsUpdate(Msg))
		pRV->UpdateRulersInfo(stRulerInfo);

	//    if (((int)(stRulerInfo.DocSize.cx*stRulerInfo.fZoomFactor) < stRulerInfo.DocSize.cx) ||
	//        ((int)(stRulerInfo.DocSize.cy*stRulerInfo.fZoomFactor) < stRulerInfo.DocSize.cy))
	//        ShowRulers(FALSE, FALSE);
	//    else if (m_bRulersVisible)
	//        ShowRulers(TRUE, TRUE);
}

void CRulerSplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	//Lock Splitter
	CWnd::OnLButtonDown(nFlags, point);
}

void CRulerSplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	//Lock Splitter
	CWnd::OnMouseMove(nFlags, point);
}


BOOL CRulerSplitterWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	///lock splitter
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
