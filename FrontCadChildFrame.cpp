// FrontCadChildFrame.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "FrontCadChildFrame.h"


// CFrontCadChildFrame

IMPLEMENT_DYNCREATE(CFrontCadChildFrame, CMDIChildWnd)

CFrontCadChildFrame::CFrontCadChildFrame()
{

}

CFrontCadChildFrame::~CFrontCadChildFrame()
{
}


BEGIN_MESSAGE_MAP(CFrontCadChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()


// CFrontCadChildFrame message handlers


BOOL CFrontCadChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	if (!CMDIChildWnd::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}


BOOL CFrontCadChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_RulerSplitter.CreateRulers(this, pContext)) {
		TRACE("Error creation of rulers\n");
		return CMDIChildWnd::OnCreateClient(lpcs, pContext);
	}

	return TRUE;
}


void CFrontCadChildFrame::ShowRulers(int nShow)
{
	m_RulerSplitter.ShowRulers(nShow);
}


void CFrontCadChildFrame::UpdateRulersInfo(stRULER_INFO info)
{
	m_RulerSplitter.UpdateRulersInfo(info);
}
