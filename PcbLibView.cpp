// PcbLibView.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "PcbLibView.h"


// CPcbLibView

IMPLEMENT_DYNCREATE(CPcbLibView, CView)

CPcbLibView::CPcbLibView()
{

}

CPcbLibView::~CPcbLibView()
{
}

BEGIN_MESSAGE_MAP(CPcbLibView, CView)
END_MESSAGE_MAP()


// CPcbLibView drawing

void CPcbLibView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CPcbLibView diagnostics

#ifdef _DEBUG
void CPcbLibView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CPcbLibView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPcbLibView message handlers
