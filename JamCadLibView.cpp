// JamCadLibView.cpp : implementation file
//
//---------------------------------------------------------
// View Class for a Schematic Library Object 
//---------------------------------------------------------

#include "stdafx.h"
#include "JamCad.h"
#include "JamCadLibView.h"
#include "JamCadLibDoc.h"
#include "CadSchLib.h"
#include "JamCadSchLibPartDoc.h"
#include "MyMultiDocTemplate.h"

// CJamCadLibView

IMPLEMENT_DYNCREATE(CJamCadLibView, CTreeView)

CJamCadLibView::CJamCadLibView()
{

}

CJamCadLibView::~CJamCadLibView()
{
}

BEGIN_MESSAGE_MAP(CJamCadLibView, CTreeView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CJamCadLibView::OnNMDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CJamCadLibView::OnNMRClick)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, &CJamCadLibView::OnTvnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CJamCadLibView::OnTvnEndlabeledit)
END_MESSAGE_MAP()


// CJamCadLibView diagnostics

#ifdef _DEBUG
void CJamCadLibView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CJamCadLibView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CJamCadLibView message handlers


void CJamCadLibView::OnInitialUpdate()
{
	char *s;
	HTREEITEM Item;
	CTreeView::OnInitialUpdate();
	CJamCadLibDoc *pDoc = (CJamCadLibDoc *)GetDocument();
	CCadObject *pO = pDoc->m_pLibDwg->GetHead();
	s = pDoc->m_pLibDwg->GetNameProp()->GetFieldValue();
	Item = GetTreeCtrl().InsertItem(s, TVI_ROOT, TVI_SORT);
	pDoc->m_pLibDwg->SetTreeItem(Item);
	while (pO)
	{
		switch (pO->GetType())
		{
			case OBJECT_TYPE_SCHLIB:
				s = ((CCadSchLib *)pO)->GetNameProp()->GetFieldValue();
				GetTreeCtrl().InsertItem(s, TVI_ROOT, TVI_SORT);
				break;
		}
	}
}

BOOL CJamCadLibView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_EDITLABELS;
	return CTreeView::PreCreateWindow(cs);
}

void CJamCadLibView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	CJamCadLibDoc *pDoc = (CJamCadLibDoc *)GetDocument();
	CPoint p, ClientPos;
	GetCursorPos(&p);
	ClientPos = p;
	ScreenToClient(&ClientPos);
	HTREEITEM Item = GetTreeCtrl().HitTest(ClientPos);
	if (Item)
	{
		CCadObject *pObj = pDoc->m_pLibDwg->FindTreeItem(Item);
		if (pObj)
		{
			switch (pObj->GetType())
			{
				case OBJECT_TYPE_SCHLIB:
					break;
				case OBJECT_TYPE_SCH_PART:
					((CCadSchPart *)pObj)->GetView()->GetParentFrame()->ActivateFrame();
					break;
			}
		}
	}
	*pResult = 0;
}

void CJamCadLibView::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	//----------------------------------------------------
	// This is what happens when you click the right
	// button....
	//----------------------------------------------------
	CJamCadLibDoc *pDoc = (CJamCadLibDoc *)GetDocument();
	CPoint p, ClientPos;
	GetCursorPos(&p);
	ClientPos = p;
	ScreenToClient(&ClientPos);
	HTREEITEM Item = GetTreeCtrl().HitTest(ClientPos);
	if (Item)
	{
		CCadObject *pObj = pDoc->m_pLibDwg->FindTreeItem(Item);
		if (pObj)
		{
			switch (pObj->GetType())
			{
				case OBJECT_TYPE_SCHLIB:
					DoSchLibContextMenu(p,Item,pObj);
					break;
				case OBJECT_TYPE_SCH_PART:
					break;
			}
		}
	}
	*pResult = 0;
}

void CJamCadLibView::OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CJamCadLibView::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: Add your control notification handler code here
	char *s = new char[256];
	GetTreeCtrl().GetEditControl()->GetWindowTextA(s, 255);
	MessageBox("End Edit",s);
	delete[]s;
	*pResult = 0;
}

#define SCHLIB_CM_NEW_PART		3100
#define SCHLIB_CM_RENAME		3101

void CJamCadLibView::DoSchLibContextMenu(CPoint pt,HTREEITEM Item,CCadObject *pObjItem)
{
	CMenu ContextMenu;
	int Id;
	CMyMultiDocTemplate  *pTemp;
	CJamCadLibDoc *pLibDoc = (CJamCadLibDoc *)GetDocument();
	CJamCadSchLibPartDoc *pLibPartDoc;

	ContextMenu.CreatePopupMenu();
	ContextMenu.AppendMenuA(MF_STRING, SCHLIB_CM_NEW_PART, "New Part");
	ContextMenu.EnableMenuItem(SCHLIB_CM_NEW_PART, MF_ENABLED);
	ContextMenu.AppendMenuA(MF_STRING, SCHLIB_CM_RENAME, "Rename");
	ContextMenu.EnableMenuItem(SCHLIB_CM_RENAME, MF_ENABLED);
	Id = ContextMenu.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pt.x, pt.y, this);
	switch (Id)
	{
		case SCHLIB_CM_NEW_PART:
			pTemp = (CMyMultiDocTemplate *)theApp.FindDocTemplate(".schLibPart");
			pLibPartDoc = (CJamCadSchLibPartDoc *)pTemp->MakeNewDocument();
			pLibDoc->m_pLibDwg->AddObject(pLibPartDoc->m_pDrawing);
			Item = GetTreeCtrl().InsertItem(pLibPartDoc->m_pDrawing->GetPartNameProp()->GetFieldValue(),Item);
			pLibPartDoc->m_pDrawing->SetTreeItem(Item);
			break;
		case SCHLIB_CM_RENAME:
			break;
	}
}
