// DlgPinProp.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "DlgPinProp.h"
#include "afxdialogex.h"
#include "resource.h"
#include "PropString.h"

// CDlgPinProp dialog

IMPLEMENT_DYNAMIC(CDlgPinProp, CDialogEx)

CDlgPinProp::CDlgPinProp(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PIN_PROPERTIES, pParent)
{

}

CDlgPinProp::~CDlgPinProp()
{
}

void CDlgPinProp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PIN_NAME, m_Edit_PinName);
	DDX_Control(pDX, IDC_EDIT_PIN_NUMBER, m_Edit_PinNumber);
	DDX_Control(pDX, IDC_COMBO_ELECTRICAL_TYPE, m_Combo_ElectricalType);
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_Static_Preview);
	DDX_Control(pDX, IDC_COMBO_GRAPHIC_STYLE, m_Combo_GraphicStyle);
	DDX_Control(pDX, IDC_COMBO_ORIENTATION, m_Combo_Orientation);
	DDX_Control(pDX, IDC_EDIT_PINLEN, m_Edit_Len);
	DDX_Control(pDX, IDC_CHECK_ENABLE_AUTOINC, m_Check_AutoInc);
	DDX_Control(pDX, IDC_EDIT_AUTOINC_LABEL, m_Edit_LabelInc);
	DDX_Control(pDX, IDC_EDIT_AUTOINC_PIN, m_Edit_PinInc);
}


BEGIN_MESSAGE_MAP(CDlgPinProp, CDialogEx)
//	ON_CBN_SELCHANGE(IDC_COMBO_GRAPHIC_STYLE, &CDlgPinProp::OnSelchangeComboGraphicStyle)
ON_CBN_SELCHANGE(IDC_COMBO_ELECTRICAL_TYPE, &CDlgPinProp::OnCbnSelchangeComboElectricalType)
ON_CBN_SELCHANGE(IDC_COMBO_GRAPHIC_STYLE, &CDlgPinProp::OnSelchangeComboGraphicStyle)
ON_CBN_SELCHANGE(IDC_COMBO_ORIENTATION, &CDlgPinProp::OnSelchangeComboOrientation)
ON_MESSAGE(WM_PINPROP, &CDlgPinProp::OnPinProp)
ON_EN_KILLFOCUS(IDC_EDIT_PIN_NAME, &CDlgPinProp::OnKillfocusEditPinName)
ON_EN_KILLFOCUS(IDC_EDIT_PIN_NUMBER, &CDlgPinProp::OnKillfocusEditPinNumber)
END_MESSAGE_MAP()


// CDlgPinProp message handlers


BOOL CDlgPinProp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//------------------------------------
	// Initialize Bitmaps
	//------------------------------------
	m_Bitmap_RightPin.LoadBitmap(IDB_PIN_RIGHT);
	m_Bitmap_LeftPin.LoadBitmap(IDB_PIN_LEFT);
	m_Bitmap_UpPin.LoadBitmap(IDB_PINUP);
	m_Bitmap_DownPin.LoadBitmap(IDB_PINDOWN);

	m_Bitmap_Clock.LoadBitmap(IDB_CLOCK);
	m_Bitmap_ClockInv.LoadBitmap(IDB_CLOCKINV);
	m_Bitmap_Line.LoadBitmap(IDB_LINE);
	m_Bitmap_LineInv.LoadBitmap(IDB_LINEINV);

	//----------------------------------
	// Initialize Combo Boxes
	//-----------------------------------
	m_Combo_ElectricalType.AddString("Input");
	m_Combo_ElectricalType.AddString("Output");
	m_Combo_ElectricalType.AddString("Bidirectional");
	m_Combo_ElectricalType.AddString("Passive");
	m_Combo_ElectricalType.AddString("Power");
	m_Combo_ElectricalType.SetCurSel(m_pPin->GetPinType()->GetSelection());


	m_Combo_Orientation.AddBitmap(&m_Bitmap_LeftPin,"Left Facing Pin");
	m_Combo_Orientation.AddBitmap(&m_Bitmap_RightPin, "Right Facing Pin");
	m_Combo_Orientation.AddBitmap(&m_Bitmap_UpPin, "Up Facing Pin");
	m_Combo_Orientation.AddBitmap(&m_Bitmap_DownPin, "Down Facing Pin");
	m_Combo_Orientation.SetCurSel(m_pPin->GetPinRotation()->GetSelection());

	m_Combo_GraphicStyle.AddBitmap(&m_Bitmap_Line, "Line");
	m_Combo_GraphicStyle.AddBitmap(&m_Bitmap_LineInv, "Inverted");
	m_Combo_GraphicStyle.AddBitmap(&m_Bitmap_Clock, "Clock");
	m_Combo_GraphicStyle.AddBitmap(&m_Bitmap_ClockInv, "Clock Inverted");
	m_Combo_GraphicStyle.SetCurSel(m_pPin->GetPinStyle()->GetSelection());
	//-----------------------------------------
	// Initialize Edit Boxes
	//------------------------------------------
	m_Edit_PinName.SetWindowTextA(m_pPin->FindProperty(m_pPin->GetPropertyName(TOKEN_PINNAME))->GetFieldValue());

	m_Edit_PinNumber.SetWindowTextA(m_pPin->FindProperty(m_pPin->GetPropertyName(TOKEN_PINNUMBER))->GetFieldValue());

	m_Check_AutoInc.SetCheck(CCadPin::m_AutoIncEnable);

	m_Edit_Len.SetDecimalPlaces(3);
	m_Edit_Len.SetValue( ((CPropInt *)(m_pPin->GetProperties().FindProperty(m_pPin->GetPropertyName(TOKEN_PINLENGTH))))->GetValue() );
	m_Edit_Len.SetupMessaging(this, WM_PINPROP, WM_PINPROP_PINLEN);

	m_Edit_LabelInc.SetDecimalPlaces(0);
	m_Edit_LabelInc.SetValue(CCadPin::m_NameInc);

	m_Edit_PinInc.SetDecimalPlaces(0);
	m_Edit_PinInc.SetValue(CCadPin::m_PinInc);

	m_Static_Preview.Init();
	m_Static_Preview.m_pObj = m_pPin;
	m_Static_Preview.AutoScale(m_pPin->GetRect());
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgPinProp::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	GetNextDlgTabItem(pW)->SetFocus();
	
	char *s = new char[256];
	switch (Id)
	{
		case IDC_EDIT_PIN_NAME:
			OnKillfocusEditPinName();
			break;
		case IDC_EDIT_PIN_NUMBER:
			OnKillfocusEditPinNumber();
			break;
		case IDOK:
			CCadPin::m_AutoIncEnable = m_Check_AutoInc.GetCheck();
			CCadPin::m_PinInc = m_Edit_PinInc.GetValue();
			CCadPin::m_NameInc = m_Edit_LabelInc.GetValue();
			m_Edit_PinNumber.GetWindowTextA(s, 255);
			m_pPin->GetPinNumber()->SetValue(s);
			CDialogEx::OnOK();
			break;
	}
	delete[] s;
}

void CDlgPinProp::OnCbnSelchangeComboElectricalType()
{
	int index = m_Combo_ElectricalType.GetCurSel();
	m_pPin->GetPinType()->SetSelection(index);
	m_Static_Preview.AutoScale(m_pPin->GetRect());
	m_Static_Preview.Invalidate();
}


void CDlgPinProp::OnSelchangeComboGraphicStyle()
{
	int index = m_Combo_GraphicStyle.GetCurSel();
	m_pPin->GetPinStyle()->SetSelection(index);
	m_pPin->AdjustStrings();
	m_Static_Preview.AutoScale(m_pPin->GetRect());
	m_Static_Preview.Invalidate();
}


void CDlgPinProp::OnSelchangeComboOrientation()
{
	// TODO: Add your control notification handler code here
	int sel = m_Combo_Orientation.GetCurSel();
	m_pPin->GetPinRotation()->SetSelection(sel);
	m_pPin->AdjustStrings();
	m_Static_Preview.AutoScale(m_pPin->GetRect());
	m_Static_Preview.Invalidate();
}


afx_msg LRESULT CDlgPinProp::OnPinProp(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case WM_PINPROP_PINLEN:
			((CPropInt *)m_pPin->GetProperties().FindProperty(m_pPin->GetPropertyName(TOKEN_PINLENGTH)))->SetValue(lParam);
			m_pPin->AdjustStrings();
			m_Static_Preview.AutoScale(m_pPin->GetRect());
			m_Static_Preview.Invalidate();
			break;
	}
	return 0;
}


void CDlgPinProp::OnKillfocusEditPinName()
{
	char *s = new char[256];
	m_Edit_PinName.GetWindowTextA(s, 255);
	((CPropString *)m_pPin->GetProperties().FindProperty(m_pPin->GetPropertyName(TOKEN_PINNAME)))->SetValue(s);
	m_pPin->AdjustStrings();
	m_Static_Preview.AutoScale(m_pPin->GetRect());
	m_Static_Preview.Invalidate();
	delete[] s;
}

void CDlgPinProp::OnKillfocusEditPinNumber()
{
	char *s = new char[256];
	m_Edit_PinNumber.GetWindowTextA(s, 255);
	((CPropString *)m_pPin->GetProperties().FindProperty(m_pPin->GetPropertyName(TOKEN_PINNUMBER)))->SetValue(s);
	m_pPin->AdjustStrings();
	m_Static_Preview.AutoScale(m_pPin->GetRect());
	m_Static_Preview.Invalidate();
	delete[] s;
}

int DoPinPropertiesDlg()
{
	return 0;
}