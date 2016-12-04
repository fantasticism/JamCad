// DlgLineAttributes.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "DlgLineAttributes.h"
#include "afxdialogex.h"


// CDlgLineAttributes dialog

IMPLEMENT_DYNAMIC(CDlgLineAttributes, CDialog)

CDlgLineAttributes::CDlgLineAttributes(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_LINE_ATTRB, pParent)
{

}

CDlgLineAttributes::~CDlgLineAttributes()
{
}

void CDlgLineAttributes::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LINE_WIDTH, m_Edit_Width);
	DDX_Control(pDX, IDC_STATIC_LINE_COLOR, m_Static_LineColor);
}


BEGIN_MESSAGE_MAP(CDlgLineAttributes, CDialog)
	ON_STN_CLICKED(IDC_STATIC_LINE_COLOR, &CDlgLineAttributes::OnStnClickedStaticLineColor)
END_MESSAGE_MAP()


// CDlgLineAttributes message handlers


BOOL CDlgLineAttributes::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit_Width.SetDecimalPlaces(3);
	m_Edit_Width.SetValue(m_Attrb.m_Width);

	m_Static_LineColor.SetColor(m_Attrb.m_Color);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgLineAttributes::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
		case IDOK:
			m_Attrb.m_Color = m_Static_LineColor.GetColor();
			m_Attrb.m_Width = m_Edit_Width.GetValue();
			CDialog::OnOK();
			break;
	}

}


void CDlgLineAttributes::OnStnClickedStaticLineColor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_LineColor.SetColor(Dlg.GetColor());
}

int DoLineAttrbDlg(void)
{
	int Id;
	CDlgLineAttributes Dlg;

	Dlg.m_Attrb.m_Color = theApp.m_LineAttr.m_Color;
	Dlg.m_Attrb.m_Width = theApp.m_LineAttr.m_Width;
	Id = Dlg.DoModal();
	if (IDOK == Id)
	{
		theApp.m_LineAttr.m_Color = Dlg.m_Attrb.m_Color;
		theApp.m_LineAttr.m_Width = Dlg.m_Attrb.m_Width;
	}
	return Id;
}