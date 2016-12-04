// FrontCadDoc.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "FrontCadDoc.h"


// CFrontCadDoc

IMPLEMENT_DYNCREATE(CFrontCadDoc, CDocument)

CFrontCadDoc::CFrontCadDoc()
{
}

BOOL CFrontCadDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CFrontCadDoc::~CFrontCadDoc()
{
}


BEGIN_MESSAGE_MAP(CFrontCadDoc, CDocument)
END_MESSAGE_MAP()


// CFrontCadDoc diagnostics

#ifdef _DEBUG
void CFrontCadDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CFrontCadDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CFrontCadDoc serialization

void CFrontCadDoc::Serialize(CArchive& ar)
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


// CFrontCadDoc commands
