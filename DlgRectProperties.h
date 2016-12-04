#pragma once

#include "CadRect.h"
#include "JamCadDefines.h"
#include "EditDecimal.h"
#include "StaticBitmap.h"

// CDlgRectProperties dialog

class CDlgRectProperties : public CDialog
{
	DECLARE_DYNAMIC(CDlgRectProperties)
public:
	//------------------------------------------
	// Data
	//-------------------------------------------
	RectAttributes m_Attrb;
	//-------------------------------------------
	// Controlls
	//-------------------------------------------
	CEditDecimal m_Edit_Width;
	CStaticBitmap m_Static_FillColor;
	CStaticBitmap m_Static_LineColor;
public:
	CDlgRectProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRectProperties();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RECTPROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedStaticLineColor();
	afx_msg void OnStnClickedStaticFillColor();
};

extern int DoRectPropertiesDlg();
