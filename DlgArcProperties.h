#pragma once
#include "StaticBitmap.h"
#include "EditDecimal.h"
#include "CadArc.h"

// CDlgArcProperties dialog

class CDlgArcProperties : public CDialog
{
	DECLARE_DYNAMIC(CDlgArcProperties)
public:
	//---------------------------
	// Data
	//----------------------------
	ArcAttributes m_Attrb;
	//---------------------------
	// Controls
	//---------------------------
	CStaticBitmap m_Static_Color;
	CEditDecimal m_Edit_EndAngle;
	CEditDecimal m_Edit_EndX;
	CEditDecimal m_Edit_EndY;
	CEditDecimal m_Edit_Width;
	CEditDecimal m_Edit_StartAngle;
	CEditDecimal m_Edit_StartX;
	CEditDecimal m_Edit_StartY;
public:
	CDlgArcProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgArcProperties();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ARCPROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedStaticLinecolor();
};

extern int DoArcPropertiesDlg();
