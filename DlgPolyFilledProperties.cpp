// DlgPolyFilledProperties.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "DlgPolyFilledProperties.h"
#include "JamCadDefines.h"
#include "afxdialogex.h"


// CDlgPolyFilledProperties dialog

IMPLEMENT_DYNAMIC(CDlgPolyFilledProperties, CDialog)

CDlgPolyFilledProperties::CDlgPolyFilledProperties(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_POLYFILLEDPROPERTIES, pParent)
{

}

CDlgPolyFilledProperties::~CDlgPolyFilledProperties()
{
}

void CDlgPolyFilledProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LINEWIDTH, m_Edit_Width);
	DDX_Control(pDX, IDC_STATI_LINE_COLOR, m_Static_Color);
	DDX_Control(pDX, IDC_STATI_FILL_COLOR, m_Static_FillColor);
}


BEGIN_MESSAGE_MAP(CDlgPolyFilledProperties, CDialog)
	ON_STN_CLICKED(IDC_STATI_LINE_COLOR, &CDlgPolyFilledProperties::OnStnClickedStatiLineColor)
	ON_STN_CLICKED(IDC_STATI_FILL_COLOR, &CDlgPolyFilledProperties::OnStnClickedStatiFillColor)
END_MESSAGE_MAP()


// CDlgPolyFilledProperties message handlers


BOOL CDlgPolyFilledProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Initialize controls
	m_Edit_Width.SetDecimalPlaces(3);
	m_Edit_Width.SetValue(m_Attrb.m_Width);
	m_Static_Color.SetColor(m_Attrb.m_Color);
	m_Static_FillColor.SetColor(m_Attrb.m_FillColor);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgPolyFilledProperties::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
		case IDOK:
			m_Attrb.m_Color = m_Static_Color.GetColor();
			m_Attrb.m_FillColor = m_Static_FillColor.GetColor();
			m_Attrb.m_Width = m_Edit_Width.GetValue();
			CDialog::OnOK();
			break;
	}
}


void CDlgPolyFilledProperties::OnStnClickedStatiLineColor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_Color.SetColor(Dlg.GetColor());
}


void CDlgPolyFilledProperties::OnStnClickedStatiFillColor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_FillColor.SetColor(Dlg.GetColor());
}

int DoPolygonFilledPropertiesDlg()
{
	CDlgPolyFilledProperties Dlg;
	int Id;

	Dlg.m_Attrb.m_Color = theApp.m_PolyFilledAttr.m_Color;
	Dlg.m_Attrb.m_Width = theApp.m_PolyFilledAttr.m_Width;
	Dlg.m_Attrb.m_FillColor = theApp.m_PolyFilledAttr.m_FillColor;
	Id = Dlg.DoModal();
	if (IDOK == Id)
	{
		theApp.m_PolyFilledAttr.m_Color = Dlg.m_Attrb.m_Color;
		theApp.m_PolyFilledAttr.m_Width = Dlg.m_Attrb.m_Width;
		theApp.m_PolyFilledAttr.m_FillColor = Dlg.m_Attrb.m_FillColor;
	}
	return Id;
}