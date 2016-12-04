// JamCadPcbLibView.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "JamCadPcbLibView.h"


// CJamCadPcbLibView

IMPLEMENT_DYNCREATE(CJamCadPcbLibView, CTreeView)

CJamCadPcbLibView::CJamCadPcbLibView()
{

}

CJamCadPcbLibView::~CJamCadPcbLibView()
{
}

BEGIN_MESSAGE_MAP(CJamCadPcbLibView, CTreeView)
END_MESSAGE_MAP()


// CJamCadPcbLibView diagnostics

#ifdef _DEBUG
void CJamCadPcbLibView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CJamCadPcbLibView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CJamCadPcbLibView message handlers
