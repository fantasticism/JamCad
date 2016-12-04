#pragma once
//-----------------------------------------
// This is the Object for a Schematic 
// Library object
//-----------------------------------------

// CJamCadLibView view

class CJamCadLibView : public CTreeView
{
	DECLARE_DYNCREATE(CJamCadLibView)

protected:
	CJamCadLibView();           // protected constructor used by dynamic creation
	virtual ~CJamCadLibView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	void DoSchLibContextMenu(CPoint pt, HTREEITEM, CCadObject *pObjItem);
};


