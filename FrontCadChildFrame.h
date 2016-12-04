#pragma once
#include "RulerSplitterWindow.h"

// CFrontCadChildFrame frame

class CFrontCadChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFrontCadChildFrame)
	CRulerSplitterWnd m_RulerSplitter;
protected:
	CFrontCadChildFrame();           // protected constructor used by dynamic creation
	virtual ~CFrontCadChildFrame();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	void ShowRulers(int nShow);
	void UpdateRulersInfo(stRULER_INFO info);
};


