// JamCadSchLibPartDoc.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "JamCadSchLibPartDoc.h"
#include "DlgSchPartProp.h"


// CJamCadSchLibPartDoc

IMPLEMENT_DYNCREATE(CJamCadSchLibPartDoc, CDocument)

CJamCadSchLibPartDoc::CJamCadSchLibPartDoc()
{
	m_pDrawing = 0;
	m_Unit = 0;
}

BOOL CJamCadSchLibPartDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	m_pDrawing = new CCadSchPart;
	m_pDrawing->Create();
	SetTitle(m_pDrawing->GetPartNameProp()->GetFieldValue());
	return TRUE;
}

CJamCadSchLibPartDoc::~CJamCadSchLibPartDoc()
{
	if (m_pDrawing) delete m_pDrawing;
}


BEGIN_MESSAGE_MAP(CJamCadSchLibPartDoc, CDocument)
END_MESSAGE_MAP()


// CJamCadSchLibPartDoc diagnostics

#ifdef _DEBUG
void CJamCadSchLibPartDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CJamCadSchLibPartDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CJamCadSchLibPartDoc serialization

void CJamCadSchLibPartDoc::Serialize(CArchive& ar)
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


// CJamCadSchLibPartDoc commands


void CJamCadSchLibPartDoc::InsertObject(CCadObject * pObj)
{
	m_pDrawing->InsertObject(pObj, GetCurrentUnit());
}


void CJamCadSchLibPartDoc::AddObject(CCadObject * pObj)
{
	m_pDrawing->AddObject(pObj, GetCurrentUnit());
}


void CJamCadSchLibPartDoc::RemoveObject(CCadObject * pObj)
{
	m_pDrawing->RemoveObject(pObj, GetCurrentUnit());
}

int CJamCadSchLibPartDoc::CheckSelected(CPoint p, CCadObject ** ppSelList, int n, int flag)
{
	///--------------------------------------
	/// CheckSelected
	///
	/// This function is used to see if
	/// there are any objects under the
	/// point.  This function returns the
	/// first 'n' objects it finds.  It does
	/// not actually select anything.
	///
	/// parameters:
	///		p......point of interest
	///		ppSelList...pointer to an array of selected objects
	///		n.....maximum number of objects to check for
	///		flag..what kind of objects are inlcuded in list
	///				0-All Objects
	///				1-Unselected Objects
	///				2-Selected Objects
	///	returns:
	///		number of objects that are under the point
	///--------------------------------------------
	int count = 0;
	CCadObject *pCO = m_pDrawing->GetHead();

	while (pCO && (count < n))
	{
		count = pCO->CheckSelected(p, CSize(0,0), ppSelList, count, n, flag);
		pCO = pCO->GetNext();
	}
	pCO = m_pDrawing->GetProperties().GetHead();
	while (pCO && (count < n))
	{
		count = pCO->CheckSelected(p, CSize(0,0), ppSelList, count, n, flag);
		pCO = pCO->GetNext();
	}
	return count;
}
