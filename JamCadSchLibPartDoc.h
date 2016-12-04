#pragma once

#include "CadSchPart.h"
#include "CadSchPart.h"

// CJamCadSchLibPartDoc document

class CJamCadSchLibPartDoc : public CDocument
{
	DECLARE_DYNCREATE(CJamCadSchLibPartDoc)
	CCadSchPart *m_pDrawing;
	int m_Unit;
public:
	CJamCadSchLibPartDoc();
	virtual ~CJamCadSchLibPartDoc();
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

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnNewDocument();
	int CheckSelected(CPoint p, CCadObject **ppL, int n, int flag);
	void InsertObject(CCadObject * pObj);
	void AddObject(CCadObject * pObj);
	void RemoveObject(CCadObject * pObj);
	inline HTREEITEM GetTreeItem(void) { 
		HTREEITEM rv = 0;
		if (m_pDrawing)rv = m_pDrawing->GetTreeItem();
		return rv;
	}
	inline CCadSchPart *GetDrawing() { return m_pDrawing; }
	inline int GetCurrentUnit() { return m_Unit; }
	inline void SetCurrentUnit(int u) { m_Unit = u; }
};
