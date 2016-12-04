// JamCadLibDoc.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "JamCadLibDoc.h"
#include "DlgSchLibProp.h"
#include "Lexer.h"

// CJamCadLibDoc

IMPLEMENT_DYNCREATE(CJamCadLibDoc, CDocument)

CJamCadLibDoc::CJamCadLibDoc()
{
}

BOOL CJamCadLibDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	CDlgSchLibProp Dlg;
	CProp *pP = new CProp;
	MakeSchLibProperties(pP);
	Dlg.m_pProp = pP;
	if (IDOK == Dlg.DoModal())
	{
		m_pLibDwg = new CCadSchLib;
		m_pLibDwg->GetProperties().Move(*pP);
		m_pLibDwg->Create();
		delete pP;
	}
	return TRUE;
}

CJamCadLibDoc::~CJamCadLibDoc()
{
}


BEGIN_MESSAGE_MAP(CJamCadLibDoc, CDocument)
END_MESSAGE_MAP()


// CJamCadLibDoc diagnostics

#ifdef _DEBUG
void CJamCadLibDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CJamCadLibDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CJamCadLibDoc serialization

void CJamCadLibDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// CJamCadLibDoc commands


BOOL CJamCadLibDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: Add your specialized code here and/or call the base class
	FILE *pO;

	pO = fopen(lpszPathName, "w");
	if (pO)
	{
		m_pLibDwg->Save(pO,0);
		fclose(pO);
	}
	return true;
}


BOOL CJamCadLibDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	FILE *pIn = fopen(lpszPathName, "r");
	if (pIn == 0) return FALSE;
	m_pLibDwg = new CCadSchLib;
	CLexer *pLex = new CLexer(pIn);
	int Token = pLex->Lex();
	m_pLibDwg->Parse(Token, pLex);
	delete pLex;
	fclose(pIn);
	return TRUE;
}
