#pragma once
#include "afxwin.h"
#include "CadBitmap.h"

// CDlgBitmapProperties dialog

class CDlgBitmapProperties : public CDialog
{
	DECLARE_DYNAMIC(CDlgBitmapProperties)
public:
	//Data
	BitMapAttribute m_Attrb;
	//Copntrols
	CButton m_Check_MaintainAspecRation;
	CEdit m_Edit_FileName;
public:
	CDlgBitmapProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgBitmapProperties();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BITMAP_PROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckMaintainAspectRatio();
	afx_msg void OnBnClickedButtonChooseFile();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};

extern int DoBitmapPropertiesDlg();
