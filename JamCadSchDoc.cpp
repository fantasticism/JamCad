// JamCadSchDoc.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "JamCadSchDoc.h"
#include "JamCadDefines.h"


// CJamCadSchDoc

IMPLEMENT_DYNCREATE(CJamCadSchDoc, CDocument)

CJamCadSchDoc::CJamCadSchDoc()
{
	m_pLex = 0;
	m_pPath = 0;
	m_pDrawing = 0;
}

BOOL CJamCadSchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	m_pDrawing = new CJamSch;
	return TRUE;
}

CJamCadSchDoc::~CJamCadSchDoc()
{
	if (m_pDrawing) delete m_pDrawing;
}


BEGIN_MESSAGE_MAP(CJamCadSchDoc, CDocument)
END_MESSAGE_MAP()


// CJamCadSchDoc diagnostics

#ifdef _DEBUG
void CJamCadSchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CJamCadSchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CJamCadSchDoc serialization

void CJamCadSchDoc::Serialize(CArchive& ar)
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


// CJamCadSchDoc commands


void CJamCadSchDoc::AddObject(CCadObject * pObj)
{
	if (m_pDrawing) m_pDrawing->AddObject(pObj);
}


void CJamCadSchDoc::InsertObject(CCadObject * pObj)
{
	if (m_pDrawing) m_pDrawing->InsertObject(pObj);
}


int CJamCadSchDoc::RemoveObject(CCadObject * pObj)
{
	if (m_pDrawing)m_pDrawing->RemoveObject(pObj);
	return 0;
}

int CJamCadSchDoc::Parser(void)
{
	m_pLex = new CLexer(m_pPath);
	int Token;
	while (1)
	{
		Token = m_pLex->Lex();	//get a token from the lexer
		if (TOKEN_SCHEMATIC == Token)
		{
			Token = m_pDrawing->Parse(Token ,m_pLex);
		}
		else
		{
			MessageBox(NULL, ("Not A Sch File"), ("ERROR"),MB_ICONHAND | MB_OK);
		}
	}
	return 0;
}

int CJamCadSchDoc::Save(FILE *pO)
{
	m_pDrawing->Save(pO,0);
	return 0;
}
