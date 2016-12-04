#pragma once
#include "EditDecimal.h"
#include "StaticBitmap.h"
#include "CadRndRect.h"

// CDlgRndRectProperties dialog

class CDlgRndRectProperties : public CDialog
{
	DECLARE_DYNAMIC(CDlgRndRectProperties)
public:
	//----------------------------
	// Data
	//-----------------------------
	RoundRectAttributes m_Attrb;
	//------------------------------
	// Controls
	//------------------------------
	CEditDecimal m_Edit_Radius;
	CEditDecimal m_Edit_Width;
	CStaticBitmap m_Static_FillColor;
	CStaticBitmap m_Static_LineColor;
	CEditDecimal m_Edit_RadiusY;
public:
	CDlgRndRectProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRndRectProperties();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ROUNDEDRECT_PROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedStaticFillcolor();
	afx_msg void OnStnClickedStaticLinecolor();
};

extern int DoRndRectPropertiesDlg();
