// PcbDoc.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "PcbDoc.h"


// CPcbDoc

IMPLEMENT_DYNCREATE(CPcbDoc, CDocument)

CPcbDoc::CPcbDoc()
{
}

BOOL CPcbDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CPcbDoc::~CPcbDoc()
{
}


BEGIN_MESSAGE_MAP(CPcbDoc, CDocument)
END_MESSAGE_MAP()


// CPcbDoc diagnostics

#ifdef _DEBUG
void CPcbDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CPcbDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CPcbDoc serialization

void CPcbDoc::Serialize(CArchive& ar)
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


// CPcbDoc commands
