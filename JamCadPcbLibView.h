#pragma once


// CJamCadPcbLibView view

class CJamCadPcbLibView : public CTreeView
{
	DECLARE_DYNCREATE(CJamCadPcbLibView)

protected:
	CJamCadPcbLibView();           // protected constructor used by dynamic creation
	virtual ~CJamCadPcbLibView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


