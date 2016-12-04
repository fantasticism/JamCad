#pragma once
#include "EditDecimal.h"
#include "StaticBitmap.h"
#include "CadPolygon.h"

// CDlgPoloyProperties dialog

class CDlgPoloyProperties : public CDialog
{
	DECLARE_DYNAMIC(CDlgPoloyProperties)
public:
	//--------------------
	// Data
	//---------------------
	PolyAttributes m_Attrb;
	//---------------------
	// Controls
	//---------------------
	CEditDecimal m_Edit_Width;
	CStaticBitmap m_Static_Color;
public:
	CDlgPoloyProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPoloyProperties();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_POLYGONPROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedStatiLineColor();
};

extern int DoPolygonPropertiesDlg();
