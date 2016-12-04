#pragma once
#include "EditDecimal.h"
#include "StaticBitmap.h"
#include "CadLine.h"

// CDlgLineAttributes dialog

class CDlgLineAttributes : public CDialog
{
	DECLARE_DYNAMIC(CDlgLineAttributes)
public:
		//=---------------------------------
		// Data
		//----------------------------------
	LineAttr m_Attrb;
public:
	CDlgLineAttributes(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLineAttributes();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LINE_ATTRB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CEditDecimal m_Edit_Width;
	afx_msg void OnStnClickedStaticLineColor();
	CStaticBitmap m_Static_LineColor;
};

extern int DoLineAttrbDlg(void);
