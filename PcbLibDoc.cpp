// PcbLibDoc.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "PcbLibDoc.h"


// CPcbLibDoc

IMPLEMENT_DYNCREATE(CPcbLibDoc, CDocument)

CPcbLibDoc::CPcbLibDoc()
{
}

BOOL CPcbLibDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CPcbLibDoc::~CPcbLibDoc()
{
}


BEGIN_MESSAGE_MAP(CPcbLibDoc, CDocument)
END_MESSAGE_MAP()


// CPcbLibDoc diagnostics

#ifdef _DEBUG
void CPcbLibDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CPcbLibDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CPcbLibDoc serialization

void CPcbLibDoc::Serialize(CArchive& ar)
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


// CPcbLibDoc commands
