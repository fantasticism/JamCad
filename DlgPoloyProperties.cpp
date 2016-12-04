// DlgPoloyProperties.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "DlgPoloyProperties.h"
#include "afxdialogex.h"


// CDlgPoloyProperties dialog

IMPLEMENT_DYNAMIC(CDlgPoloyProperties, CDialog)

CDlgPoloyProperties::CDlgPoloyProperties(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_POLYGONPROPERTIES, pParent)
{

}

CDlgPoloyProperties::~CDlgPoloyProperties()
{
}

void CDlgPoloyProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LINEWIDTH, m_Edit_Width);
	DDX_Control(pDX, IDC_STATI_LINE_COLOR, m_Static_Color);
}


BEGIN_MESSAGE_MAP(CDlgPoloyProperties, CDialog)
	ON_STN_CLICKED(IDC_STATI_LINE_COLOR, &CDlgPoloyProperties::OnStnClickedStatiLineColor)
END_MESSAGE_MAP()


// CDlgPoloyProperties message handlers


BOOL CDlgPoloyProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit_Width.SetDecimalPlaces(3);
	m_Edit_Width.SetValue(m_Attrb.m_Width);
	m_Static_Color.SetColor(m_Attrb.m_Color);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgPoloyProperties::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
		case IDOK:
			m_Attrb.m_Color = m_Static_Color.GetColor();
			m_Attrb.m_Width = m_Edit_Width.GetValue();
			CDialog::OnOK();
			break;
	}
}


void CDlgPoloyProperties::OnStnClickedStatiLineColor()
{
	// TODO: Add your control notification handler code here
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_Color.SetColor(Dlg.GetColor());
}

int DoPolygonPropertiesDlg()
{
	CDlgPoloyProperties Dlg;
	int Id;

	Dlg.m_Attrb.m_Color = theApp.m_PolyAttr.m_Color;
	Dlg.m_Attrb.m_Width = theApp.m_PolyAttr.m_Width;
	Id = Dlg.DoModal();
	if (IDOK == Id)
	{
		theApp.m_PolyAttr.m_Color = Dlg.m_Attrb.m_Color;
		theApp.m_PolyAttr.m_Width = Dlg.m_Attrb.m_Width;
	}
	return Id;
}
