// PropertyEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "PropertyEditorDlg.h"
#include "afxdialogex.h"


// CPropertyEditorDlg dialog

IMPLEMENT_DYNAMIC(CPropertyEditorDlg, CDialog)

CPropertyEditorDlg::CPropertyEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_PROPERTYEDITOR, pParent)
{

}

CPropertyEditorDlg::~CPropertyEditorDlg()
{
}

void CPropertyEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PROPERTYEDITOR_GRID, m_Static_PropertyEditor);
}


BEGIN_MESSAGE_MAP(CPropertyEditorDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PROPEDITOR_NEWCOLUMN, &CPropertyEditorDlg::OnClickedButtonPropeditorNewcolumn)
	ON_BN_CLICKED(IDC_BUTTON_PROPERTYEDITOR_APPLY, &CPropertyEditorDlg::OnClickedButtonPropertyeditorApply)
	ON_BN_CLICKED(IDC_BUTTON_PROPERTYEDITOR_DELETEPROP, &CPropertyEditorDlg::OnClickedButtonPropertyeditorDeleteprop)
	ON_BN_CLICKED(IDC_BUTTON_PROPERTYEDITOR_DISPLAY, &CPropertyEditorDlg::OnClickedButtonPropertyeditorDisplay)
	ON_BN_CLICKED(IDOK, &CPropertyEditorDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPropertyEditorDlg message handlers


void CPropertyEditorDlg::OnClickedButtonPropeditorNewcolumn()
{
	// TODO: Add your control notification handler code here
}


void CPropertyEditorDlg::OnClickedButtonPropertyeditorApply()
{
	// TODO: Add your control notification handler code here
}


void CPropertyEditorDlg::OnClickedButtonPropertyeditorDeleteprop()
{
	// TODO: Add your control notification handler code here
}


void CPropertyEditorDlg::OnClickedButtonPropertyeditorDisplay()
{
	// TODO: Add your control notification handler code here
}


void CPropertyEditorDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


BOOL CPropertyEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
