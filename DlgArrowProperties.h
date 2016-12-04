#pragma once
#include "EditDecimal.h"
#include "StaticBitmap.h"
#include "StaticPreview.h"
#include "CadArrow.h"

// CDlgArrowProperties dialog

class CDlgArrowProperties : public CDialog
{
	DECLARE_DYNAMIC(CDlgArrowProperties)
	CCadArrow *m_pArrow;
public:
	//--------------------------------
	// Data
	//--------------------------------
	ArrowAttributes m_Attrb;
	ArrowAttributes m_AttrbOrig;
	//--------------------------------
	// Controls
	//--------------------------------
	CEditDecimal m_Edit_L1;
	CEditDecimal m_Edit_L2;
	CStaticBitmap m_Static_Color;
	CStaticPreview m_Static_Preview;
	CEditDecimal m_Edit_Width;
public:
	CDlgArrowProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgArrowProperties();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ARROW_ATTRIBUTES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedStaticColor();
	virtual void OnCancel();
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
protected:
	afx_msg LRESULT OnArrowAttrbValues(WPARAM wParam, LPARAM lParam);
};

extern int DoArrowPropertiesDlg();
