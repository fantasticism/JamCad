// DlgTextProperties.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "DlgTextProperties.h"
#include "afxdialogex.h"


// CDlgTextProperties dialog

IMPLEMENT_DYNAMIC(CDlgTextProperties, CDialogEx)

CDlgTextProperties::CDlgTextProperties(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_TEXT_PROPERTIES, pParent)
{
}

CDlgTextProperties::~CDlgTextProperties()
{
}

void CDlgTextProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_FONT, m_Button_Font);
	DDX_Control(pDX, IDC_CHECK_TRANSPARENT, m_Check_Transparent);
	DDX_Control(pDX, IDC_EDIT_ANGLE, m_Edit_Angle);
	DDX_Control(pDX, IDC_EDIT_TEXT_HEIGHT, m_Edit_Height);
	DDX_Control(pDX, IDC_EDIT_TEXT_STRING, m_Edit_TextString);
	DDX_Control(pDX, IDC_EDIT_TEXT_WIDTH, m_Edit_Width);
	DDX_Control(pDX, IDC_STATIC_BACKGROUND_COLOR, m_Static_BackGroundColor);
	DDX_Control(pDX, IDC_STATIC_TEXT_COLOR, m_Static_TextColor);
	DDX_Control(pDX, IDC_COMBO_FONTWEIGHT, m_Combo_FontWeight);
}


BEGIN_MESSAGE_MAP(CDlgTextProperties, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FONT, &CDlgTextProperties::OnBnClickedButtonFont)
	ON_STN_CLICKED(IDC_STATIC_TEXT_COLOR, &CDlgTextProperties::OnStnClickedStaticTextColor)
	ON_STN_CLICKED(IDC_STATIC_BACKGROUND_COLOR, &CDlgTextProperties::OnStnClickedStaticBackgroundColor)
END_MESSAGE_MAP()


// CDlgTextProperties message handlers


BOOL CDlgTextProperties::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//---------------------------------------
	// Initialize ComboBox
	//---------------------------------------
	m_Combo_FontWeight.Init();
	m_Combo_FontWeight.SetFontWeight(m_Attr.m_Weight);

	m_Edit_Height.SetDecimalPlaces(3);
	m_Edit_Height.SetValue(m_Attr.m_FontHeight);

	m_Edit_Width.SetDecimalPlaces(3);
	m_Edit_Width.SetValue(m_Attr.m_FontWidth);

	m_Edit_Angle.SetDecimalPlaces(1);
	m_Edit_Angle.SetValue(m_Attr.m_Angle);

	m_Static_BackGroundColor.SetColor(m_Attr.m_BkColor);

	m_Static_TextColor.SetColor(m_Attr.m_Color);

	m_Button_Font.SetWindowTextA(m_Attr.m_pFontName);

	m_Check_Transparent.SetCheck(m_Attr.m_Transparent);

	if (m_Attr.m_pText)
		m_Edit_TextString.SetWindowTextA(m_Attr.m_pText);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgTextProperties::OnOK()
{
	CWnd *pW = GetFocus();
	int id = pW->GetDlgCtrlID();
	switch (id)
	{
		case IDOK:
			CDialogEx::OnOK();
			//---------------------------------
			// Update Data from controls
			//---------------------------------
			m_Attr.m_Angle = m_Edit_Angle.GetValue();
			m_Attr.m_BkColor = m_Static_BackGroundColor.GetColor();
			m_Attr.m_Color = m_Static_TextColor.GetColor();
			m_Attr.m_FontHeight = m_Edit_Height.GetValue();
			m_Attr.m_FontWidth = m_Edit_Width.GetValue();
			m_Button_Font.GetWindowTextA(m_Attr.m_pFontName, 32);
			if (m_Attr.m_pText)
			{
				delete[] m_Attr.m_pText;
			}
			m_Attr.m_pText = new char[256];
			m_Edit_TextString.GetWindowTextA(m_Attr.m_pText, 255);
			m_Attr.m_Transparent = m_Check_Transparent.GetCheck();
			m_Attr.m_Weight = m_Combo_FontWeight.GetFontWeight();
			break;
	}
}

void CDlgTextProperties::OnBnClickedButtonFont()
{
	CFontDialog Dlg;
	int Id;
	LOGFONT lF;

	Id = Dlg.DoModal();
	if (IDOK == Id)
	{
		Dlg.GetCurrentFont(&lF);
		m_Button_Font.SetWindowTextA(lF.lfFaceName);
		strcpy(m_Attr.m_pFontName, lF.lfFaceName);
	}
}


void CDlgTextProperties::OnStnClickedStaticTextColor()
{
	CColorDialog Dlg;
	int Id;

	Id = Dlg.DoModal();
	if (IDOK == Id)
		m_Static_TextColor.SetColor(Dlg.GetColor());
}


void CDlgTextProperties::OnStnClickedStaticBackgroundColor()
{
	CColorDialog Dlg;
	int Id;

	Id = Dlg.DoModal();
	if (IDOK == Id)
		m_Static_BackGroundColor.SetColor(Dlg.GetColor());
}

int DoTextPropertiesDlg()
{
	CDlgTextProperties Dlg;
	int Id;

	Dlg.m_Attr.m_BkColor = theApp.m_TextAttr.m_BkColor;
	Dlg.m_Attr.m_Angle = theApp.m_TextAttr.m_Angle;
	Dlg.m_Attr.m_Color = theApp.m_TextAttr.m_Color;
	Dlg.m_Attr.m_FontHeight = theApp.m_TextAttr.m_FontHeight;
	Dlg.m_Attr.m_FontWidth = theApp.m_TextAttr.m_FontWidth;
	Dlg.m_Attr.m_Format = theApp.m_TextAttr.m_Format;
	Dlg.m_Attr.m_Transparent = theApp.m_TextAttr.m_Transparent;
	strcpy(Dlg.m_Attr.m_pFontName, theApp.m_TextAttr.m_pFontName);
	Dlg.m_Attr.m_Weight = theApp.m_TextAttr.m_Weight;
	strcpy(Dlg.m_Attr.m_pText, theApp.m_TextAttr.m_pText);
	Id = Dlg.DoModal();
	if (IDOK == Id)
	{
		theApp.m_TextAttr.m_Color = Dlg.m_Attr.m_Color;
		theApp.m_TextAttr.m_Angle = Dlg.m_Attr.m_Angle;
		theApp.m_TextAttr.m_BkColor = Dlg.m_Attr.m_BkColor;
		theApp.m_TextAttr.m_FontHeight = Dlg.m_Attr.m_FontHeight;
		theApp.m_TextAttr.m_FontWidth = Dlg.m_Attr.m_FontWidth;
		theApp.m_TextAttr.m_Format = Dlg.m_Attr.m_Format;
		theApp.m_TextAttr.m_Transparent = Dlg.m_Attr.m_Transparent;
		theApp.m_TextAttr.m_Weight = Dlg.m_Attr.m_Weight;
		strcpy(theApp.m_TextAttr.m_pFontName, Dlg.m_Attr.m_pFontName);
		strcpy(theApp.m_TextAttr.m_pText, Dlg.m_Attr.m_pText);
	}
	return Id;
}