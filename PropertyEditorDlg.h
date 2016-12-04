#pragma once
#include "PropertyEditor.h"


// CPropertyEditorDlg dialog

class CPropertyEditorDlg : public CDialog
{
	DECLARE_DYNAMIC(CPropertyEditorDlg)

public:
	CPropertyEditorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPropertyEditorDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROPERTYEDITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButtonPropeditorNewcolumn();
	afx_msg void OnClickedButtonPropertyeditorApply();
	afx_msg void OnClickedButtonPropertyeditorDeleteprop();
	afx_msg void OnClickedButtonPropertyeditorDisplay();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CPropertyEditor m_Static_PropertyEditor;
};
