// PcbView.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "PcbView.h"


// CPcbView

IMPLEMENT_DYNCREATE(CPcbView, CAutoScrollView)

CPcbView::CPcbView()
{

}

CPcbView::~CPcbView()
{
}

BEGIN_MESSAGE_MAP(CPcbView, CAutoScrollView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
//	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()


// CPcbView drawing

void CPcbView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CPcbView diagnostics

#ifdef _DEBUG
void CPcbView::AssertValid() const
{
	CAutoScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CPcbView::Dump(CDumpContext& dc) const
{
	CAutoScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPcbView message handlers


void CPcbView::OnInitialUpdate()
{
	SetDocSize(CSize(24000, 24000));
	CAutoScrollView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}


void CPcbView::OnLButtonDown(UINT nFlags, CPoint point)
{
	EnableAutoScroll(1);
	UpdateMousePositions(point);
	CAutoScrollView::OnLButtonDown(nFlags, point);
}


void CPcbView::OnLButtonUp(UINT nFlags, CPoint point)
{
	UpdateMousePositions(point);
	CAutoScrollView::OnLButtonUp(nFlags, point);
}


void CPcbView::OnMouseMove(UINT nFlags, CPoint point)
{
	UpdateMousePositions(point);
	CAutoScrollView::OnMouseMove(nFlags, point);
	Invalidate();
}


//void CPcbView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	// This feature requires Windows Vista or greater.
//	// The symbol _WIN32_WINNT must be >= 0x0600.
//	// TODO: Add your message handler code here and/or call default
//
//	CAutoScrollView::OnMouseHWheel(nFlags, zDelta, pt);
//}


BOOL CPcbView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	return CAutoScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


void CPcbView::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
}


void CPcbView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CAutoScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CPcbView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CAutoScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}
