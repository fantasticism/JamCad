// DlgRectProperites.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "DlgRectProperties.h"
#include "afxdialogex.h"


// CDlgRectProperties dialog

IMPLEMENT_DYNAMIC(CDlgRectProperties, CDialogEx)

CDlgRectProperties::CDlgRectProperties(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_RECTPROPERTIES, pParent)
{

}

CDlgRectProperties::~CDlgRectProperties()
{
}

void CDlgRectProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LINE_WIDTH, m_Edit_Width);
	DDX_Control(pDX, IDC_STATIC_FILL_COLOR, m_Static_FillColor);
	DDX_Control(pDX, IDC_STATIC_LINE_COLOR, m_Static_LineColor);
}


BEGIN_MESSAGE_MAP(CDlgRectProperties, CDialog)
	ON_STN_CLICKED(IDC_STATIC_LINE_COLOR, &CDlgRectProperties::OnStnClickedStaticLineColor)
	ON_STN_CLICKED(IDC_STATIC_FILL_COLOR, &CDlgRectProperties::OnStnClickedStaticFillColor)
END_MESSAGE_MAP()


// CDlgRectProperties message handlers


BOOL CDlgRectProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit_Width.SetDecimalPlaces(3);
	m_Edit_Width.SetValue(m_Attrb.m_Width);
	m_Static_FillColor.SetColor(m_Attrb.m_FillColor);
	m_Static_LineColor.SetColor(m_Attrb.m_Color);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgRectProperties::OnOK()
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


void CDlgRectProperties::OnStnClickedStaticLineColor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_LineColor.SetColor(Dlg.GetColor());
}


void CDlgRectProperties::OnStnClickedStaticFillColor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_FillColor.SetColor(Dlg.GetColor());
}

int DoRectPropertiesDlg()
{
	CDlgRectProperties Dlg;
	int Id;

	Dlg.m_Attrb.m_Color = theApp.m_RectAttr.m_Color;
	Dlg.m_Attrb.m_FillColor = theApp.m_RectAttr.m_FillColor;
	Dlg.m_Attrb.m_Width = theApp.m_RectAttr.m_Width;
	Id = Dlg.DoModal();
	if (IDOK == Id)
	{
		theApp.m_RectAttr.m_Color = Dlg.m_Attrb.m_Color;
		theApp.m_RectAttr.m_FillColor = Dlg.m_Attrb.m_FillColor;
		theApp.m_RectAttr.m_Width = Dlg.m_Attrb.m_Width;
	}
	return Id;
}