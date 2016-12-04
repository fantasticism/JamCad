// DlgSchPartProp.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "DlgSchPartProp.h"
#include "afxdialogex.h"


// CDlgSchPartProp dialog

IMPLEMENT_DYNAMIC(CDlgSchPartProp, CDialog)

CDlgSchPartProp::CDlgSchPartProp(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_SCHPART_PROP, pParent)
{

}

CDlgSchPartProp::~CDlgSchPartProp()
{
}

void CDlgSchPartProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PARTPROP_PARTNAME, m_Edit_PartName);
	DDX_Control(pDX, IDC_EDIT_PARTPROP_REFENCEDES, m_Edit_RefDes);
	DDX_Control(pDX, IDC_EDIT_PARTPROP_UNITS, m_Edit_Units);
	DDX_Control(pDX, IDC_CHECK_PARTPROP_NAMEINSIDE, m_Check_NameInside);
	DDX_Control(pDX, IDC_CHECK_PARTPROP_SHOWPINNUMBERS, m_Check_ShowPinNumber);
	DDX_Control(pDX, IDC_CHECK_PARTPROP_SHOWPINNAMES, m_Check_ShowPinNames);
}


BEGIN_MESSAGE_MAP(CDlgSchPartProp, CDialog)
	ON_BN_CLICKED(IDC_CHECK_PARTPROP_SHOWPINNAMES, &CDlgSchPartProp::OnBnClickedCheckPartpropShowpinnames)
	ON_BN_CLICKED(IDC_CHECK_PARTPROP_SHOWPINNUMBERS, &CDlgSchPartProp::OnBnClickedCheckPartpropShowpinnumbers)
	ON_BN_CLICKED(IDC_CHECK_PARTPROP_NAMEINSIDE, &CDlgSchPartProp::OnBnClickedCheckPartpropNameinside)
END_MESSAGE_MAP()


// CDlgSchPartProp message handlers


BOOL CDlgSchPartProp::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit_Units.SetDecimalPlaces(0);
	m_Edit_Units.SetValue(m_pPartDwg->GetTotalUnitsProp()->GetValue());
	m_Edit_PartName.SetWindowTextA(m_pPartDwg->GetPartNameProp()->GetFieldValue());
	m_Edit_RefDes.SetWindowTextA(m_pPartDwg->GetRefDesignatorProp()->GetFieldValue());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSchPartProp::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	char *s = new char[256];
	switch (Id)
	{
		case IDOK:
			m_pPartDwg->GetTotalUnitsProp()->SetValue(m_Edit_Units.GetValue());
			m_Edit_PartName.GetWindowTextA(s, 255);
			m_pPartDwg->GetPartNameProp()->SetValue(s);
			m_Edit_RefDes.GetWindowTextA(s, 255);
			m_pPartDwg->GetRefDesignatorProp()->SetValue(s);
			CDialog::OnOK();
			break;
		case IDC_EDIT_PARTPROP_PARTNAME:
			GetNextDlgTabItem(pW)->SetFocus();
			break;
		case IDC_EDIT_PARTPROP_REFENCEDES:
			GetNextDlgTabItem(pW)->SetFocus();
			break;
	}
	delete[] s;
}


void CDlgSchPartProp::OnBnClickedCheckPartpropShowpinnames()
{
	// TODO: Add your control notification handler code here
}


void CDlgSchPartProp::OnBnClickedCheckPartpropShowpinnumbers()
{
	// TODO: Add your control notification handler code here
}


void CDlgSchPartProp::OnBnClickedCheckPartpropNameinside()
{
	// TODO: Add your control notification handler code here
}
