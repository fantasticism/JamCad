// DlgArcProperties.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "DlgArcProperties.h"
#include "afxdialogex.h"


// CDlgArcProperties dialog

IMPLEMENT_DYNAMIC(CDlgArcProperties, CDialog)

CDlgArcProperties::CDlgArcProperties(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_ARCPROPERTIES, pParent)
{

}

CDlgArcProperties::~CDlgArcProperties()
{
}

void CDlgArcProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LINECOLOR, m_Static_Color);
	DDX_Control(pDX, IDC_EDIT_END_ANGLE, m_Edit_EndAngle);
	DDX_Control(pDX, IDC_EDIT_END_X, m_Edit_EndX);
	DDX_Control(pDX, IDC_EDIT_END_Y, m_Edit_EndY);
	DDX_Control(pDX, IDC_EDIT_LINEWIDTH, m_Edit_Width);
	DDX_Control(pDX, IDC_EDIT_START_ANGLE, m_Edit_StartAngle);
	DDX_Control(pDX, IDC_EDIT_START_X, m_Edit_StartX);
	DDX_Control(pDX, IDC_EDIT_START_Y, m_Edit_StartY);
}


BEGIN_MESSAGE_MAP(CDlgArcProperties, CDialog)
	ON_STN_CLICKED(IDC_STATIC_LINECOLOR, &CDlgArcProperties::OnStnClickedStaticLinecolor)
END_MESSAGE_MAP()


// CDlgArcProperties message handlers


BOOL CDlgArcProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit_EndAngle.SetDecimalPlaces(1);
	m_Edit_EndAngle.SetValue(m_Attrb.m_EndAngle);
	m_Edit_EndX.SetDecimalPlaces(3);
	m_Edit_EndX.SetValue(m_Attrb.m_End.x);
	m_Edit_EndY.SetDecimalPlaces(3);
	m_Edit_EndY.SetValue(m_Attrb.m_End.y);
	m_Edit_StartAngle.SetDecimalPlaces(1);
	m_Edit_StartAngle.SetValue(m_Attrb.m_StartAngle);
	m_Edit_StartX.SetDecimalPlaces(3);
	m_Edit_StartX.SetValue(m_Attrb.m_Start.x);
	m_Edit_StartY.SetDecimalPlaces(3);
	m_Edit_StartY.SetValue(m_Attrb.m_Start.y);
	m_Edit_Width.SetDecimalPlaces(3);
	m_Edit_Width.SetValue(m_Attrb.m_Width);
	m_Static_Color.SetColor(m_Attrb.m_Color);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgArcProperties::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
		case IDOK:
			m_Attrb.m_Color = m_Static_Color.GetColor();
			m_Attrb.m_End.x = m_Edit_EndX.GetValue();
			m_Attrb.m_End.y = m_Edit_EndY.GetValue();
			m_Attrb.m_EndAngle = m_Edit_EndAngle.GetValue();
			m_Attrb.m_Start.x = m_Edit_StartX.GetValue();
			m_Attrb.m_Start.y = m_Edit_StartY.GetValue();
			m_Attrb.m_StartAngle = m_Edit_StartAngle.GetValue();
			m_Attrb.m_Width = m_Edit_Width.GetValue();
			CDialog::OnOK();
			break;
	}

}


void CDlgArcProperties::OnStnClickedStaticLinecolor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_Color.SetColor(Dlg.GetColor());
}

int DoArcPropertiesDlg()
{
	int Id;
	CDlgArcProperties Dlg;

	Dlg.m_Attrb.m_Color = theApp.m_ArcAttr.m_Color;
	Dlg.m_Attrb.m_End = theApp.m_ArcAttr.m_End;
	Dlg.m_Attrb.m_EndAngle = theApp.m_ArcAttr.m_EndAngle;
	Dlg.m_Attrb.m_Start = theApp.m_ArcAttr.m_Start;
	Dlg.m_Attrb.m_StartAngle = theApp.m_ArcAttr.m_StartAngle;
	Dlg.m_Attrb.m_Width = theApp.m_ArcAttr.m_Width;
	Id = Dlg.DoModal();
	if (IDOK == Id)
	{
		theApp.m_ArcAttr.m_Color = Dlg.m_Attrb.m_Color;
		theApp.m_ArcAttr.m_End = Dlg.m_Attrb.m_End;
		theApp.m_ArcAttr.m_EndAngle = Dlg.m_Attrb.m_EndAngle;
		theApp.m_ArcAttr.m_Start = Dlg.m_Attrb.m_Start;
		theApp.m_ArcAttr.m_StartAngle = Dlg.m_Attrb.m_StartAngle;
		theApp.m_ArcAttr.m_Width = Dlg.m_Attrb.m_Width;
	}
	return Id;
}