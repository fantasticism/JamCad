// DlgSchLibProp.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "DlgSchLibProp.h"
#include "afxdialogex.h"


// CDlgSchLibProp dialog

IMPLEMENT_DYNAMIC(CDlgSchLibProp, CDialogEx)

CDlgSchLibProp::CDlgSchLibProp(CWnd* pParent /*=NULL*/): CDialogEx(IDD_DIALOG_SCHLIB_PROP, pParent)
{

}

CDlgSchLibProp::~CDlgSchLibProp()
{
}

void CDlgSchLibProp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_Edit_LibName);
}


BEGIN_MESSAGE_MAP(CDlgSchLibProp, CDialogEx)
END_MESSAGE_MAP()


// CDlgSchLibProp message handlers


BOOL CDlgSchLibProp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pLibName = (CPropString *)m_pProp->FindProperty(PropFieldNames[PF_NAME_SCHLIBNAME]);
	char *s = m_pLibName->GetFieldValue();
	m_Edit_LibName.SetWindowTextA(s);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSchLibProp::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pWnd = GetFocus();
	int Id = pWnd->GetDlgCtrlID();
	char *s = new char[256];
	switch (Id)
	{
		case IDOK:
			m_Edit_LibName.GetWindowTextA(s, 255);
			m_pLibName->SetValue(s);
			CDialogEx::OnOK();

			break;
	}
	delete[] s;
}
