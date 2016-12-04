// DlgRndRectProperties.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "DlgRndRectProperties.h"
#include "afxdialogex.h"


// CDlgRndRectProperties dialog

IMPLEMENT_DYNAMIC(CDlgRndRectProperties, CDialog)

CDlgRndRectProperties::CDlgRndRectProperties(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_ROUNDEDRECT_PROPERTIES, pParent)
{

}

CDlgRndRectProperties::~CDlgRndRectProperties()
{
}

void CDlgRndRectProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CORNERRADIUS, m_Edit_Radius);
	DDX_Control(pDX, IDC_EDIT_LINEWIDTH, m_Edit_Width);
	DDX_Control(pDX, IDC_STATIC_FILLCOLOR, m_Static_FillColor);
	DDX_Control(pDX, IDC_STATIC_LINECOLOR, m_Static_LineColor);
	DDX_Control(pDX, IDC_EDIT_CORNERRADIUS_Y, m_Edit_RadiusY);
}


BEGIN_MESSAGE_MAP(CDlgRndRectProperties, CDialog)
	ON_STN_CLICKED(IDC_STATIC_FILLCOLOR, &CDlgRndRectProperties::OnStnClickedStaticFillcolor)
	ON_STN_CLICKED(IDC_STATIC_LINECOLOR, &CDlgRndRectProperties::OnStnClickedStaticLinecolor)
END_MESSAGE_MAP()


// CDlgRndRectProperties message handlers


BOOL CDlgRndRectProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit_Radius.SetDecimalPlaces(3);
	m_Edit_Radius.SetValue(m_Attrb.m_CornerRadius.cx);
	m_Edit_RadiusY.SetDecimalPlaces(3);
	m_Edit_RadiusY.SetValue(m_Attrb.m_CornerRadius.cy);
	m_Edit_Width.SetDecimalPlaces(3);
	m_Edit_Width.SetValue(m_Attrb.m_Width);
	m_Static_LineColor.SetColor(m_Attrb.m_Color);
	m_Static_FillColor.SetColor(m_Attrb.m_FillColor);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgRndRectProperties::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
		case IDOK:
			m_Attrb.m_Color = m_Static_LineColor.GetColor();
			m_Attrb.m_CornerRadius.cx = m_Edit_Radius.GetValue();
			m_Attrb.m_CornerRadius.cy = m_Edit_RadiusY.GetValue();
			m_Attrb.m_FillColor = m_Static_FillColor.GetColor();
			m_Attrb.m_Width = m_Edit_Width.GetValue();
			CDialog::OnOK();
			break;
	}
}

void CDlgRndRectProperties::OnStnClickedStaticFillcolor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_FillColor.SetColor(Dlg.GetColor());
}


void CDlgRndRectProperties::OnStnClickedStaticLinecolor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_LineColor.SetColor(Dlg.GetColor());
}

int DoRndRectPropertiesDlg()
{
	CDlgRndRectProperties Dlg;
	int Id;

	Dlg.m_Attrb.m_Color = theApp.m_RndRectAttr.m_Color;
	Dlg.m_Attrb.m_FillColor = theApp.m_RndRectAttr.m_FillColor;
	Dlg.m_Attrb.m_Width = theApp.m_RndRectAttr.m_Width;
	Id = Dlg.DoModal();
	if (IDOK == Id)
	{
		theApp.m_RndRectAttr.m_Color = Dlg.m_Attrb.m_Color;
		theApp.m_RndRectAttr.m_FillColor = Dlg.m_Attrb.m_FillColor;
		theApp.m_RndRectAttr.m_Width = Dlg.m_Attrb.m_Width;
	}
	return Id;
}
