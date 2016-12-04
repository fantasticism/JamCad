
// JamCad.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include "JamCad.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include <strsafe.h>
#include "MyMultiDocTemplate.h"

#include "ChildFrm.h"
#include "JamCadProjDoc.h"
#include "JamCadProjView.h"
#include "JamCadSchView.h"
#include "JamCadSchDoc.h"
#include "JamCadLibDoc.h"
#include "JamCadLibView.h"
#include "FrontCadDoc.h"
#include "FrontCadView.h"
#include "PcbDoc.h"
#include "PcbView.h"
#include "JamCadSchLibPartDoc.h"
#include "JamCadSchLibPartView.h"


#include "JamCadDefines.h"


#include "FrontCadChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJamCadApp

BEGIN_MESSAGE_MAP(CJamCadApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CJamCadApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(IDM_FILE_NEW_PROJECT, &CJamCadApp::OnFileNewProject)
	ON_COMMAND(IDM_FILE_NEW_SCHEMATIC, &CJamCadApp::OnFileNewSchematic)
	ON_COMMAND(IDM_FILE_NEW_LIB, &CJamCadApp::OnFileNewLib)
	ON_COMMAND(IDM_FILE_NEW_FRONTPANEL, &CJamCadApp::OnFileNewFrontpanel)
END_MESSAGE_MAP()


// CJamCadApp construction

CJamCadApp::CJamCadApp()
{
	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("JamCad.AppID.NoVersion"));

	// TODO: add construction code here,
	m_SchCount = 0;
	m_ProjCount = 0;
	m_LibCount = 0;
	m_FrontCadCount = 0;
	m_LogFile = fopen("LogFile.Txt", "w");
	// Place all significant initialization in InitInstance
}

// The one and only CJamCadApp object

CJamCadApp theApp;


// CJamCadApp initialization

BOOL CJamCadApp::InitInstance()
{
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	LoadSettings();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMyMultiDocTemplate* pDocTemplate;
//----------------------------------------------------------
	/// project document type
	pDocTemplate = new CMyMultiDocTemplate(IDR_ProjectTYPE,
		RUNTIME_CLASS(CJamCadProjDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CJamCadProjView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
//-----------------------------------------------------------------
	///schematic document type
	pDocTemplate = new CMyMultiDocTemplate(IDR_JamCadSchType,
		RUNTIME_CLASS(CJamCadSchDoc),
		RUNTIME_CLASS(CFrontCadChildFrame),
		RUNTIME_CLASS(CJamCadSchView)
		);
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
//-----------------------------------------------------------
	/// Library Document Type
	pDocTemplate = new CMyMultiDocTemplate(IDR_JamCadLibType,
		RUNTIME_CLASS(CJamCadLibDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CJamCadLibView)
	);
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
//---------------------------------------------------------------
	//PCB Document
	pDocTemplate = new CMyMultiDocTemplate(IDR_JamCadPCB_TYPE,
		RUNTIME_CLASS(CPcbDoc),
		RUNTIME_CLASS(CFrontCadChildFrame),
		RUNTIME_CLASS(CPcbView)
	);
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
//----------------------------------------------------------------	
	//Schematic Library Part Document
	pDocTemplate = new CMyMultiDocTemplate(IDR_JamCadSchLibPart_TYPE,
		RUNTIME_CLASS(CJamCadSchLibPartDoc),
		RUNTIME_CLASS(CFrontCadChildFrame),
		RUNTIME_CLASS(CJamCadSchLibPartView)
	);
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
//----------------------------------------------------------------
//FrontCad Document
	pDocTemplate = new CMyMultiDocTemplate(IDR_FrontCad_TYPE,
		RUNTIME_CLASS(CFrontCadDoc),
		RUNTIME_CLASS(CFrontCadChildFrame),
		RUNTIME_CLASS(CFrontCadView)
	);
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
//--------------------------------------------------------------
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	//---------------------------------------
	// supress calling OnFileNew
	//---------------------------------------
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)   // actually none
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CJamCadApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	SaveSettings();
	fclose(m_LogFile);
	return CWinApp::ExitInstance();
}

// CJamCadApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL OnInitDialog();
//	virtual void OnOK();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_SIZE()
END_MESSAGE_MAP()

// App command to run the dialog
void CJamCadApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CJamCadApp message handlers




void CJamCadApp::OnFileNewProject()
{
	// TODO: Add your command handler code here
	CDocTemplate *pTemp;
	TCHAR s[256];
	CDocument *pDoc;

	pTemp = FindDocTemplate(".jcproj");
	if (pTemp)
	{
		StringCbPrintf(s, sizeof(s), ("Project%d"), ++m_ProjCount);
		pDoc = pTemp->CreateNewDocument();
		pDoc->SetTitle(s);
		CFrameWnd *pF = pTemp->CreateNewFrame(pDoc, (CFrameWnd *)m_pMainWnd);
		pTemp->InitialUpdateFrame(pF, pDoc);
	}
}


void CJamCadApp::OnFileNewSchematic()
{
	// TODO: Add your command handler code here
	CDocTemplate *pTemp;
	TCHAR s[256];
	CDocument *pDoc;

	pTemp = FindDocTemplate(".jcsch");
	if (pTemp)
	{
		StringCbPrintf(s,sizeof(s),("SHEET%d"),++m_SchCount);
//		sprintf(s, "%s%d", "SHEET", 1);
		pDoc = pTemp->CreateNewDocument();
		pDoc->SetTitle(s);
		CFrameWnd *pF = pTemp->CreateNewFrame(pDoc, (CFrameWnd *)m_pMainWnd);
		pTemp->InitialUpdateFrame(pF, pDoc);
	}
}

void CJamCadApp::OnFileNewLib()
{
	CMyMultiDocTemplate *pTemp;
	CJamCadLibDoc *pDoc;

	pTemp = FindDocTemplate(".jclib");
	if (pTemp)
	{
		pDoc = (CJamCadLibDoc *)pTemp->MakeNewDocument();
		pDoc->SetTitle(pDoc->m_pLibDwg->GetNameProp()->GetFieldValue());
	}
}

void CJamCadApp::OnFileNewFrontpanel()
{
	CDocTemplate *pTemp;
	TCHAR s[256];
	CDocument *pDoc;

	pTemp = FindDocTemplate(".fpc");
	if (pTemp)
	{
		StringCbPrintf(s, sizeof(s), ("FrontPanel%d"), ++m_FrontCadCount);
		pDoc = pTemp->CreateNewDocument();
		pDoc->SetTitle(s);
		CFrameWnd *pF = pTemp->CreateNewFrame(pDoc, (CFrameWnd *)m_pMainWnd);
		pTemp->InitialUpdateFrame(pF, pDoc);
	}
}

void CJamCadApp::SaveSettings()
{
	CCadArc::SaveSettings(&m_ArcAttr);
	CCadArcCent::SaveSettings(&m_ArcCentAttr);
	CCadArcPcb::SaveSettings(&m_ArcPcbAttr);
	CCadArrow::SaveSettings(&m_ArrowAttr);
	CCadBitmap::SaveSettings(&m_BitmapAttr);
	CCadBus::SaveSettings(&m_BusAttr);
	CCadBusEntry::SaveSettings(&m_BussEntryAttr);
	CCadDimension::SaveSettings(&m_DimAttr);
	CCadElispe::SaveSettings(&m_EllipseAttr);
	CCadFillArea::SaveSettings(&m_FillAreaAttr);
	CCadHoleRect::SaveSettings(&m_RectHoleAttr);
	CCadHoleRnd1Flat::SaveSettings(&m_RndHole1FlatAttr);
	CCadHoleRnd2Flat::SaveSettings(&m_RndHole2FlatAttr);
	CCadHoleRound::SaveSettings(&m_RoundHoleAttr);
	CCadJunction::SaveSettings(&m_JunctionAttr);
	CCadLibComp::SaveSettings(&m_LibCompAttr);
	CCadLine::SaveSettings(&m_LineAttr);
	CCadOrigin::SaveSettings(&m_OriginAttr);
	CCadPad::SaveSettings(&m_PadAttr);
	CCadPcbComp::SaveSettings(&m_PcbCompAttr);
	CCadPolyFilled::SaveSettings(&m_PolyFilledAttr);
	CCadPolygon::SaveSettings(&m_PolyAttr);
	CCadPower::SaveSettings(&m_PowerAttr);
	CCadRect::SaveSettings(&m_RectAttr);
	CCadRndRect::SaveSettings(&m_RndRectAttr);
	CCadSchPart::SaveSettings(&m_SchPartAttr);
	CCadSheet::SaveSettings(&m_SheetAttr);
	CCadSheetPin::SaveSettings(&m_SheetPinAttr);
	CCadSheetPort::SaveSettings(&m_SheetPortAttr);
	CCadText::SaveSettings("Text", &m_TextAttr);
	CCadText::SaveSettings("SchText",&m_SchTextAttr);
	CCadTrack::SaveSettings(&m_TrackAttr);
	CCadVia::SaveSettings(&m_ViaAttr);
	CCadWire::SaveSettings(&m_WireAttr);
}


void CJamCadApp::LoadSettings()
{
	CCadArc::LoadSettings(&m_ArcAttr);
	CCadArcCent::LoadSettings(&m_ArcCentAttr);
	CCadArcPcb::LoadSettings(&m_ArcPcbAttr);
	CCadArrow::LoadSettings(&m_ArrowAttr);
	CCadBitmap::LoadSettings(&m_BitmapAttr);
	CCadBus::LoadSettings(&m_BusAttr);
	CCadBusEntry::LoadSettings(&m_BussEntryAttr);
	CCadDimension::LoadSettings(&m_DimAttr);
	CCadElispe::LoadSettings(&m_EllipseAttr);
	CCadFillArea::LoadSettings(&m_FillAreaAttr);
	CCadHoleRect::LoadSettings(&m_RectHoleAttr);
	CCadHoleRnd1Flat::LoadSettings(&m_RndHole1FlatAttr);
	CCadHoleRnd2Flat::LoadSettings(&m_RndHole2FlatAttr);
	CCadHoleRound::LoadSettings(&m_RoundHoleAttr);
	CCadJunction::LoadSettings(&m_JunctionAttr);
	CCadLibComp::LoadSettings(&m_LibCompAttr);
	CCadLine::LoadSettings(&m_LineAttr);
	CCadOrigin::LoadSettings(&m_OriginAttr);
	CCadPad::LoadSettings(&m_PadAttr);
	CCadPcbComp::LoadSettings(&m_PcbCompAttr);
	CCadPolyFilled::LoadSettings(&m_PolyFilledAttr);
	CCadPolygon::LoadSettings(&m_PolyAttr);
	CCadPower::LoadSettings(&m_PowerAttr);
	CCadRect::LoadSettings(&m_RectAttr);
	CCadRndRect::LoadSettings(&m_RndRectAttr);
	CCadSchPart::LoadSettings(&m_SchPartAttr);
	CCadSheet::LoadSettings(&m_SheetAttr);
	CCadSheetPin::LoadSettings(&m_SheetPinAttr);
	CCadSheetPort::LoadSettings(&m_SheetPortAttr);
	CCadText::LoadSettings("Text",&m_TextAttr);
	CCadText::LoadSettings("SchText",&m_SchTextAttr);
	CCadTrack::LoadSettings(&m_TrackAttr);
	CCadVia::LoadSettings(&m_ViaAttr);
	CCadWire::LoadSettings(&m_WireAttr);
}

//--------------------------------------------------------


void CJamCadApp::WriteLogFile(char * s)
{
	if (m_LogFile) fprintf(m_LogFile, "%s\n", s);
}


void CJamCadApp::CutCopy(CCadObject * pObj, int mode)
{
	//---------------------------------------------------
	//	CutCopy
	//		This function will cut/copy a chain of 
	//	objects to the clipboard object.  The copy mode
	//	actually makes a copy of the objects where cut
	//	does not.
	//
	// parameters:
	//	pObj...........pointer to an to put in clipboard
	//	mode...........0=Don't Copy (Cut), 1 = Copy
	//----------------------------------------------------

	if (mode)	///copy the object
		m_ClipBoard.AddObject(pObj->CopyObject());
	else
		m_ClipBoard.AddObject(pObj);
}


void CJamCadApp::CopyToClipBoard(CCadObject * pObjList)
{
	CCadObject *pO = pObjList;

	while (pO)
	{
		CutCopy(pO, 1);
		pO = pO->GetNextSel();
	}
}


void CJamCadApp::CutToClipboard(CCadObject * pSelList)
{
	CCadObject *pO = pSelList;

	while (pO)
	{
		CutCopy(pO, 0);
		pO = pO->GetNextSel();
	}
}


void CJamCadApp::SetClipboardRef(CPoint p)
{
	m_ClipBoard.SetRef(p);
}


CMyMultiDocTemplate * CJamCadApp::FindDocTemplate(char * name)
{
	CMyMultiDocTemplate *pTemp;
	POSITION pos;
	int loop = 1;

	pos = GetFirstDocTemplatePosition();
	do
	{
		CString Dstring;
		pTemp = (CMyMultiDocTemplate *)GetNextDocTemplate(pos);
		if (pTemp)
		{
			pTemp->GetDocString(Dstring, CDocTemplate::filterExt);
			if (Dstring.Compare(name) == 0) loop = 0;
		}
	} while (loop && pTemp);
	return pTemp;
}
