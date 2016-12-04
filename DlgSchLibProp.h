#pragma once
#include "afxwin.h"
#include "Prop.h"

// CDlgSchLibProp dialog

class CDlgSchLibProp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSchLibProp)
	CPropString *m_pLibName;
public:
	CProp *m_pProp;
	CDlgSchLibProp(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSchLibProp();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SCHLIB_PROP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Edit_LibName;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
