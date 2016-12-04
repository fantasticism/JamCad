
// JamCadProjView.cpp : implementation of the CJamCadProjView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "JamCad.h"
#endif

#include "JamCadProjDoc.h"
#include "JamCadProjView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJamCadProjView

IMPLEMENT_DYNCREATE(CJamCadProjView, CTreeView)

BEGIN_MESSAGE_MAP(CJamCadProjView, CTreeView)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CONTEXTMENU()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

// CJamCadProjView construction/destruction

CJamCadProjView::CJamCadProjView()
{
	// TODO: add construction code here

}

CJamCadProjView::~CJamCadProjView()
{
}

BOOL CJamCadProjView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

void CJamCadProjView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	GetTreeCtrl().InsertItem("Junk", TVI_ROOT, TVI_SORT);
}


// CJamCadProjView diagnostics

#ifdef _DEBUG
void CJamCadProjView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CJamCadProjView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CJamCadProjDoc* CJamCadProjView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJamCadProjDoc)));
	return (CJamCadProjDoc*)m_pDocument;
}
#endif //_DEBUG


// CJamCadProjView message handlers


void CJamCadProjView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CTreeView::OnMouseMove(nFlags, point);
}


BOOL CJamCadProjView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	return CTreeView::OnMouseWheel(nFlags, zDelta, pt);
}


void CJamCadProjView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CTreeView::OnLButtonDown(nFlags, point);
}


void CJamCadProjView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CTreeView::OnLButtonUp(nFlags, point);
}


void CJamCadProjView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CTreeView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CJamCadProjView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CTreeView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CJamCadProjView::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
}


void CJamCadProjView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CTreeView::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CJamCadProjView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CTreeView::OnHScroll(nSBCode, nPos, pScrollBar);
}
