#pragma once
#include "EditDecimal.h"
#include "StaticBitmap.h"
#include "CadArcCent.h"

// CDlgArcCentProperies dialog

class CDlgArcCentProperies : public CDialog
{
	DECLARE_DYNAMIC(CDlgArcCentProperies)
public:
	//--------------------------
	// Data
	//--------------------------
	ArcCentAttributes m_Attrb;
	//--------------------------
	// Controls
	//--------------------------
	CEditDecimal m_Edit_Width;
	CStaticBitmap m_Static_Color;
	CEditDecimal m_Edit_StartX;
	CEditDecimal m_Edit_StartY;
	CEditDecimal m_Edit_EndX;
	CEditDecimal m_Edit_EndY;
	CEditDecimal m_Edit_StartAngle;
	CEditDecimal m_Edit_EndAngle;
public:
	CDlgArcCentProperies(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgArcCentProperies();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ARCCENTPROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedStaticLinecolor();
};

extern int DoArcCenterPropertiesDlg(void);

