#pragma once
#include "afxwin.h"
#include "EditDecimal.h"
#include "StaticBitmap.h"
#include "ComboBoxFontWeight.h"
#include "CadText.h"

// CDlgTextProperties dialog

class CDlgTextProperties : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTextProperties)
public:
	//----------------------------------
	// Controls
	//---------------------------------
	CButton m_Button_Font;
	CButton m_Check_Transparent;
	CEditDecimal m_Edit_Angle;
	CEditDecimal m_Edit_Height;
	CEdit m_Edit_TextString;
	CEditDecimal m_Edit_Width;
	CStaticBitmap m_Static_BackGroundColor;
	CStaticBitmap m_Static_TextColor;
	CComboBoxFontWeight m_Combo_FontWeight;
	//----------------------------------------
	// Data
	//----------------------------------------
	TextAttributes m_Attr;
public:
	CDlgTextProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTextProperties();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TEXT_PROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonFont();
	afx_msg void OnStnClickedStaticTextColor();
	afx_msg void OnStnClickedStaticBackgroundColor();
};

extern int DoTextPropertiesDlg();
