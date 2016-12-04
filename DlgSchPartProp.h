#pragma once
#include "afxwin.h"
#include "EditDecimal.h"
#include "CadSchPart.h"


// CDlgSchPartProp dialog

class CDlgSchPartProp : public CDialog
{
	DECLARE_DYNAMIC(CDlgSchPartProp)

public:
	CCadSchPart *m_pPartDwg;
	CDlgSchPartProp(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSchPartProp();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SCHPART_PROP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
//---------------------------
// controls
//---------------------------
public:
	CEdit m_Edit_PartName;
	CEdit m_Edit_RefDes;
	CEditDecimal m_Edit_Units;

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CButton m_Check_NameInside;
	CButton m_Check_ShowPinNumber;
	afx_msg void OnBnClickedCheckPartpropShowpinnames();
	CButton m_Check_ShowPinNames;
	afx_msg void OnBnClickedCheckPartpropShowpinnumbers();
	afx_msg void OnBnClickedCheckPartpropNameinside();
};
