#pragma once
#include "EditDecimal.h"
#include "StaticBitmap.h"
#include "CadElispe.h"

// CDlgEllispeProperties dialog

class CDlgEllispeProperties : public CDialog
{
	DECLARE_DYNAMIC(CDlgEllispeProperties)
public:	//Data
	EllipseAttributes m_Attrb;
		//Controls
	CEditDecimal m_Edit_Width;
	CStaticBitmap m_Static_FillColor;
	CStaticBitmap m_Static_LineColor;
public:
	CDlgEllispeProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgEllispeProperties();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ELLIPSE_PROPERTIES };
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

extern int DoEllipsePropertiesDlg();
