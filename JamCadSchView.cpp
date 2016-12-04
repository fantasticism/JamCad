// JamCadSchView.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "JamCadSchView.h"


// CJamCadSchView

IMPLEMENT_DYNCREATE(CJamCadSchView, CAutoScrollView)

CJamCadSchView::CJamCadSchView()
{

}

CJamCadSchView::~CJamCadSchView()
{
}

BEGIN_MESSAGE_MAP(CJamCadSchView, CAutoScrollView)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CONTEXTMENU()
//	ON_WM_VSCROLL()
//	ON_WM_HSCROLL()
ON_COMMAND(ID_EDIT_COPY, &CJamCadSchView::OnEditCopy)
ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CJamCadSchView::OnUpdateEditCopy)
ON_COMMAND(ID_EDIT_CUT, &CJamCadSchView::OnEditCut)
ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CJamCadSchView::OnUpdateEditCut)
ON_COMMAND(ID_EDIT_PASTE, &CJamCadSchView::OnEditPaste)
ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CJamCadSchView::OnUpdateEditPaste)
END_MESSAGE_MAP()


// CJamCadSchView drawing

void CJamCadSchView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CJamCadSchView diagnostics

#ifdef _DEBUG
void CJamCadSchView::AssertValid() const
{
	CAutoScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CJamCadSchView::Dump(CDumpContext& dc) const
{
	CAutoScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CJamCadSchView message handlers


void CJamCadSchView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	UpdateMousePositions(point);
	CAutoScrollView::OnMouseMove(nFlags, point);
	Invalidate();
}


BOOL CJamCadSchView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	return CAutoScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


void CJamCadSchView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CAutoScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CJamCadSchView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CAutoScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CJamCadSchView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	EnableAutoScroll(1);
	UpdateMousePositions(point);

	CAutoScrollView::OnLButtonDown(nFlags, point);
}


void CJamCadSchView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	UpdateMousePositions(point);
	CAutoScrollView::OnLButtonUp(nFlags, point);
}


void CJamCadSchView::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
}


void CJamCadSchView::OnAutoScroll(CPoint point, bool BeforeScroll)
{
}


void CJamCadSchView::OnInitialUpdate()
{
	SetDocSize(CSize(24000, 24000));	//document size is in mils

	CAutoScrollView::OnInitialUpdate();
}


void CJamCadSchView::OnEditCopy()
{
	MessageBox("SCH Copy");
}


void CJamCadSchView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CJamCadSchView::OnEditCut()
{
	// TODO: Add your command handler code here
}


void CJamCadSchView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CJamCadSchView::OnEditPaste()
{
	// TODO: Add your command handler code here
}


void CJamCadSchView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}
