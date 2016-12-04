#pragma once

//-------------------------------------------
// This is the files for a Schematic Library
// view object
//--------------------------------------------

#include "CadSchLib.h"

// CJamCadLibDoc document

class CJamCadLibDoc : public CDocument
{
	DECLARE_DYNCREATE(CJamCadLibDoc)
	HTREEITEM m_TreeItem;
	CCadSchLib *m_pLibDwg;
public:
	CJamCadLibDoc();
	virtual ~CJamCadLibDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	inline HTREEITEM GetTreeItem() { return m_TreeItem; }
	inline void SetTreeItem(HTREEITEM hTI) { m_TreeItem = hTI; }
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};
