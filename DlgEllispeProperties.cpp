// DlgEllispeProperties.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "DlgEllispeProperties.h"
#include "afxdialogex.h"


// CDlgEllispeProperties dialog

IMPLEMENT_DYNAMIC(CDlgEllispeProperties, CDialog)

CDlgEllispeProperties::CDlgEllispeProperties(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_ELLIPSE_PROPERTIES, pParent)
{

}

CDlgEllispeProperties::~CDlgEllispeProperties()
{
}

void CDlgEllispeProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LINE_WIDTH, m_Edit_Width);
	DDX_Control(pDX, IDC_STATIC_FILLCOLOR, m_Static_FillColor);
	DDX_Control(pDX, IDC_STATIC_LINECOLOR, m_Static_LineColor);
}


BEGIN_MESSAGE_MAP(CDlgEllispeProperties, CDialog)
	ON_STN_CLICKED(IDC_STATIC_FILLCOLOR, &CDlgEllispeProperties::OnStnClickedStaticFillcolor)
	ON_STN_CLICKED(IDC_STATIC_LINECOLOR, &CDlgEllispeProperties::OnStnClickedStaticLinecolor)
END_MESSAGE_MAP()


// CDlgEllispeProperties message handlers


BOOL CDlgEllispeProperties::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_Edit_Width.SetDecimalPlaces(3);
	m_Edit_Width.SetValue(m_Attrb.m_Width);
	m_Static_LineColor.SetColor(m_Attrb.m_Color);
	m_Static_FillColor.SetColor(m_Attrb.m_FillColor);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgEllispeProperties::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
	case IDOK:
		m_Attrb.m_Color = m_Static_LineColor.GetColor();
		m_Attrb.m_FillColor = m_Static_FillColor.GetColor();
		m_Attrb.m_Width = m_Edit_Width.GetValue();
		CDialog::OnOK();
		break;
	}
}

void CDlgEllispeProperties::OnStnClickedStaticFillcolor()
{
	// TODO: Add your control notification handler code here
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_FillColor.SetColor(Dlg.GetColor());
}


void CDlgEllispeProperties::OnStnClickedStaticLinecolor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_LineColor.SetColor(Dlg.GetColor());
}

int DoEllipsePropertiesDlg()
{
	CDlgEllispeProperties Dlg;
	int Id;

	Dlg.m_Attrb.m_Color = theApp.m_EllipseAttr.m_Color;
	Dlg.m_Attrb.m_FillColor = theApp.m_EllipseAttr.m_FillColor;
	Dlg.m_Attrb.m_Width = theApp.m_EllipseAttr.m_Width;
	Id = Dlg.DoModal();
	if (IDOK == Id)
	{
		theApp.m_EllipseAttr.m_Color = Dlg.m_Attrb.m_Color;
		theApp.m_EllipseAttr.m_FillColor = Dlg.m_Attrb.m_FillColor;
		theApp.m_EllipseAttr.m_Width = Dlg.m_Attrb.m_Width;
	}
	return Id;
}