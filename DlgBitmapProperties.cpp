// DlgBitmapProperties.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "DlgBitmapProperties.h"
#include "afxdialogex.h"


// CDlgBitmapProperties dialog

IMPLEMENT_DYNAMIC(CDlgBitmapProperties, CDialog)

CDlgBitmapProperties::CDlgBitmapProperties(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_BITMAP_PROPERTIES, pParent)
{
	m_Attrb.m_pBMFileName = 0;
}

CDlgBitmapProperties::~CDlgBitmapProperties()
{
	if (m_Attrb.m_pBMFileName) delete m_Attrb.m_pBMFileName;
	m_Attrb.m_MaintainAspectRatio = 1;
	m_Attrb.m_Size = CSize();
}

void CDlgBitmapProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_MAINTAIN_ASPECT_RATIO, m_Check_MaintainAspecRation);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_Edit_FileName);
}


BEGIN_MESSAGE_MAP(CDlgBitmapProperties, CDialog)
	ON_BN_CLICKED(IDC_CHECK_MAINTAIN_ASPECT_RATIO, &CDlgBitmapProperties::OnBnClickedCheckMaintainAspectRatio)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE_FILE, &CDlgBitmapProperties::OnBnClickedButtonChooseFile)
END_MESSAGE_MAP()


// CDlgBitmapProperties message handlers


void CDlgBitmapProperties::OnBnClickedCheckMaintainAspectRatio()
{
	// TODO: Add your control notification handler code here
}

void CDlgBitmapProperties::OnBnClickedButtonChooseFile()
{
	CFileDialog Dlg(true,"*.bmp");

	if (Dlg.DoModal() == IDOK)
	{
		CString name = Dlg.GetPathName();
		strcpy(m_Attrb.m_pBMFileName, LPCTSTR(name));
		m_Edit_FileName.SetWindowTextA(m_Attrb.m_pBMFileName);
	}
}

void CDlgBitmapProperties::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
		case IDOK:
			m_Attrb.m_MaintainAspectRatio = m_Check_MaintainAspecRation.GetCheck();
			m_Edit_FileName.GetWindowTextA(m_Attrb.m_pBMFileName, 512);
			CDialog::OnOK();
			break;
	}
}


BOOL CDlgBitmapProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Check_MaintainAspecRation.SetCheck(m_Attrb.m_MaintainAspectRatio);
	if (!m_Attrb.m_pBMFileName)
	{
		m_Attrb.m_pBMFileName = new char[512];
		m_Attrb.m_pBMFileName[0] = 0;
	}
	if (strlen(m_Attrb.m_pBMFileName))
		m_Edit_FileName.SetWindowTextA(m_Attrb.m_pBMFileName);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

int DoBitmapPropertiesDlg()
{
	int Id;
	CDlgBitmapProperties Dlg;

	Dlg.m_Attrb.m_MaintainAspectRatio = theApp.m_BitmapAttr.m_MaintainAspectRatio;
	strcpy(Dlg.m_Attrb.m_pBMFileName, theApp.m_BitmapAttr.m_pBMFileName);
	Dlg.m_Attrb.m_Size = theApp.m_BitmapAttr.m_Size;
	Id = Dlg.DoModal();
	if (IDOK == Id)
	{
		theApp.m_BitmapAttr.m_MaintainAspectRatio = Dlg.m_Attrb.m_MaintainAspectRatio;
		strcpy(theApp.m_BitmapAttr.m_pBMFileName, Dlg.m_Attrb.m_pBMFileName);
		theApp.m_BitmapAttr.m_Size = Dlg.m_Attrb.m_Size;
	}
	return Id;
}