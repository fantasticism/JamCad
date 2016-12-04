#pragma once
#include "afxwin.h"
#include "StaticPreview.h"
#include "ComboBitmap.h"
#include "CadPin.h"
#include "EditDecimal.h"

// CDlgPinProp dialog

class CDlgPinProp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPinProp)
public:
	CCadPin *m_pPin;
public:
	CDlgPinProp(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPinProp();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PIN_PROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
//--------------------------------
// Controls
//--------------------------------
public:
	CEdit m_Edit_PinName;
	CEdit m_Edit_PinNumber;
	CComboBox m_Combo_ElectricalType;
	CStaticPreview m_Static_Preview;
	CComboBitmap m_Combo_GraphicStyle;
	CBitmap m_Bitmap_RightPin;
	CBitmap m_Bitmap_LeftPin;
	CBitmap m_Bitmap_UpPin;
	CBitmap m_Bitmap_DownPin;
	CBitmap m_Bitmap_Line;
	CBitmap m_Bitmap_LineInv;
	CBitmap m_Bitmap_Clock;
	CBitmap m_Bitmap_ClockInv;
	CButton m_Check_AutoInc;
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CComboBitmap m_Combo_Orientation;
//	afx_msg void OnSelchangeComboGraphicStyle();
	afx_msg void OnCbnSelchangeComboElectricalType();
	afx_msg void OnSelchangeComboGraphicStyle();
	afx_msg void OnSelchangeComboOrientation();
	CEditDecimal m_Edit_Len;
protected:
	afx_msg LRESULT OnPinProp(WPARAM wParam, LPARAM lParam);
public:
	CEditDecimal m_Edit_LabelInc;
	CEditDecimal m_Edit_PinInc;
	afx_msg void OnKillfocusEditPinName();
	afx_msg void OnKillfocusEditPinNumber();
};
