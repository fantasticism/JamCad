#pragma once

#include "CadObject.h"
#include "Lexer.h"
#include "JamSch.h"

// CJamCadSchDoc document

class CJamCadSchDoc : public CDocument
{
	DECLARE_DYNCREATE(CJamCadSchDoc)
	CLexer *m_pLex;
public:
	CJamCadSchDoc();
	virtual ~CJamCadSchDoc();
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
	char *m_pPath;	//path to schematic file
public:
	CJamSch *m_pDrawing;
	CCadObject *m_pSelectedHead;
	void AddObject(CCadObject * pObj);
	void InsertObject(CCadObject * pObj);
	int RemoveObject(CCadObject * pObj);
	int Parser();
	int Save(FILE *pO);
	void SetPathName(char *s) { m_pPath = s; }
};
