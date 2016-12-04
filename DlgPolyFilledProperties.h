#pragma once
#include "EditDecimal.h"
#include "StaticBitmap.h"
#include "CadPolyFilled.h"


// CDlgPolyFilledProperties dialog

class CDlgPolyFilledProperties : public CDialog
{
	DECLARE_DYNAMIC(CDlgPolyFilledProperties)
public:
	//-----------------------------------
	// Data
	//-----------------------------------
	PolyFilledAttributes m_Attrb;
	//-----------------------------------
	// Controls
	//-----------------------------------
	CEditDecimal m_Edit_Width;
	CStaticBitmap m_Static_Color;
	CStaticBitmap m_Static_FillColor;
public:
	CDlgPolyFilledProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPolyFilledProperties();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_POLYFILLEDPROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedStatiLineColor();
	afx_msg void OnStnClickedStatiFillColor();
};

extern int DoPolygonFilledPropertiesDlg();
