// DlgArrowProperties.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "DlgArrowProperties.h"
#include "JamCadDefines.h"
#include "afxdialogex.h"


// CDlgArrowProperties dialog

IMPLEMENT_DYNAMIC(CDlgArrowProperties, CDialog)

CDlgArrowProperties::CDlgArrowProperties(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_ARROW_ATTRIBUTES, pParent)
{
	m_pArrow = new CCadArrow;
	m_pArrow->m_P1 = CPoint(0, 0);
	m_pArrow->m_P2 = CPoint(500, 0);
}

CDlgArrowProperties::~CDlgArrowProperties()
{
	delete m_pArrow;
}

void CDlgArrowProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_L1, m_Edit_L1);
	DDX_Control(pDX, IDC_EDIT_L2, m_Edit_L2);
	DDX_Control(pDX, IDC_STATIC_COLOR, m_Static_Color);
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_Static_Preview);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_Edit_Width);
}


BEGIN_MESSAGE_MAP(CDlgArrowProperties, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COLOR, &CDlgArrowProperties::OnStnClickedStaticColor)
//	ON_WM_ACTIVATE()
ON_MESSAGE(WM_ARROWATTRB, &CDlgArrowProperties::OnArrowAttrbValues)
END_MESSAGE_MAP()


// CDlgArrowProperties message handlers


BOOL CDlgArrowProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit_L1.SetDecimalPlaces(3);
	m_Edit_L1.SetupMessaging(this, WM_ARROWATTRB, WM_ARROWATTRB_VALUE_L1);
	m_Edit_L1.SetValue(m_Attrb.m_L1);
	m_Edit_L2.SetDecimalPlaces(3);
	m_Edit_L2.SetupMessaging(this, WM_ARROWATTRB, WM_ARROWATTRB_VALUE_L2);
	m_Edit_L2.SetValue(m_Attrb.m_L2);
	m_Edit_Width.SetDecimalPlaces(3);
	m_Edit_Width.SetupMessaging(this, WM_ARROWATTRB, WM_ARROWATTRB_VALUE_W);
	m_Edit_Width.SetValue(m_Attrb.m_Width);
	m_Static_Color.SetColor(m_Attrb.m_Color);
	m_Static_Preview.Init();
	if (m_pArrow)
	{
		m_Static_Preview.m_pObj = m_pArrow;
		m_pArrow->CopyAttributes(&m_Attrb);
		m_Static_Preview.SetOffsetMode(STATICPREVIEW_OFFSET_X10);
		m_Static_Preview.AutoScale(m_pArrow->GetRect());
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgArrowProperties::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
		case IDC_EDIT_L1:
			m_Attrb.m_L1 = m_Edit_L1.GetValue();
			m_pArrow->CopyAttributes(&m_Attrb);
			m_Static_Preview.Invalidate();
			break;
		case IDC_EDIT_L2:
			m_Attrb.m_L2 = m_Edit_L2.GetValue();
			m_pArrow->CopyAttributes(&m_Attrb);
			m_Static_Preview.Invalidate();
			break;
		case IDC_EDIT_WIDTH:
			m_Attrb.m_Width = m_Edit_Width.GetValue();
			m_pArrow->CopyAttributes(&m_Attrb);
			m_Static_Preview.Invalidate();
			break;
		case IDOK:
			m_Attrb.m_Color = m_Static_Color.GetColor();
			m_Attrb.m_L1 = m_Edit_L1.GetValue();
			m_Attrb.m_L2 = m_Edit_L2.GetValue();
			m_Attrb.m_Width = m_Edit_Width.GetValue();
			CDialog::OnOK();
			break;
	}

}


void CDlgArrowProperties::OnStnClickedStaticColor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
	{
		m_Static_Color.SetColor(Dlg.GetColor());
		m_Attrb.m_Color = Dlg.GetColor();
		m_pArrow->CopyAttributes(&m_Attrb);
		m_pArrow->SetDirty(1);
		m_Static_Preview.Invalidate();
	}
}


void CDlgArrowProperties::OnCancel()
{
	// Undo any changes to the object
//	m_pArrow->CopyAttributes(&m_AttrbOrig);

	CDialog::OnCancel();
}

afx_msg LRESULT CDlgArrowProperties::OnArrowAttrbValues(WPARAM SubMsg, LPARAM value)
{
	switch (SubMsg)
	{
		case WM_ARROWATTRB_VALUE_L1:
			m_Attrb.m_L1 = value;
			break;
		case WM_ARROWATTRB_VALUE_L2:
			m_Attrb.m_L2 = value;
			break;
		case WM_ARROWATTRB_VALUE_W:
			m_Attrb.m_Width = value;
			break;
	}
	m_pArrow->CopyAttributes(&m_Attrb);
	m_Static_Preview.AutoScale(m_pArrow->GetRect());
	m_Static_Preview.Invalidate();
	return 0;
}

int DoArrowPropertiesDlg()
{
	int Id;
	CDlgArrowProperties Dlg;

	Dlg.m_Attrb.m_Color = theApp.m_ArrowAttr.m_Color;
	Dlg.m_Attrb.m_L1 = theApp.m_ArrowAttr.m_L1;
	Dlg.m_Attrb.m_L2 = theApp.m_ArrowAttr.m_L2;
	Dlg.m_Attrb.m_Width = theApp.m_ArrowAttr.m_Width;
	Id = Dlg.DoModal();
	if (IDOK == Id)
	{
		theApp.m_ArrowAttr.m_Color = Dlg.m_Attrb.m_Color;
		theApp.m_ArrowAttr.m_L1 = Dlg.m_Attrb.m_L1;
		theApp.m_ArrowAttr.m_L2 = Dlg.m_Attrb.m_L2;
		theApp.m_ArrowAttr.m_Width = Dlg.m_Attrb.m_Width;
	}
	return Id;
}
