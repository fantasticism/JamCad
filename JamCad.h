
// amCad.h : main header file for the JamCad application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "MainFrm.h"
#include "CadArc.h"
#include "CadArcCent.h"
#include "CadArcPcb.h"
#include "CadArrow.h"
#include "CadBitmap.h"
#include "CadBus.h"
#include "CadBusEntry.h"
#include "CadDimension.h"
#include "CadElispe.h"
#include "CadFillArea.h"
#include "CadHoleRect.h"
#include "CadHoleRnd1Flat.h"
#include "CadHoleRnd2Flat.h"
#include "CadHoleRound.h"
#include "CadJunction.h"
#include "CadLibComp.h"
#include "CadLine.h"
#include "CadObject.h"
#include "CadOrigin.h"
#include "CadPad.h"
#include "CadPcbComp.h"
#include "CadPolyFilled.h"
#include "CadPolygon.h"
#include "CadPower.h"
#include "CadRect.h"
#include "CadRndRect.h"
#include "CadSchPart.h"
#include "CadSheet.h"
#include "CadSheetPin.h"
#include "CadSheetPort.h"
#include "CadText.h"
#include "CadTrack.h"
#include "CadVia.h"
#include "CadWire.h"
#include "MoveObjects.h"


// CJamCadApp:
// See JamCad.cpp for the implementation of this class
//
class CMyMultiDocTemplate;

class CJamCadApp : public CWinApp
{
	int m_ProjCount;
	int m_SchCount;
	int m_LibCount;
	int m_FrontCadCount;
	FILE *m_LogFile;
	CMoveObjects m_ClipBoard;
public:
	//------------------------------------
	// Default Attributes for objects
	//------------------------------------
	ArcAttributes m_ArcAttr;
	ArcCentAttributes m_ArcCentAttr;
	ArcPcbAttributes m_ArcPcbAttr;
	ArrowAttributes m_ArrowAttr;
	BitMapAttribute m_BitmapAttr;
	BusAttributes m_BusAttr;
	BusEntryAttributes m_BussEntryAttr;
	CadDimAttributes m_DimAttr;
	EllipseAttributes m_EllipseAttr;
	FillAreaAttributes m_FillAreaAttr;
	RectHoleAttributes m_RectHoleAttr;
	RndHole1FlatAttributes m_RndHole1FlatAttr;
	RndHole2FlagAttributes m_RndHole2FlatAttr;
	RoundHoleAttributes m_RoundHoleAttr;
	JunctionAttributes m_JunctionAttr;
	LibCompAttrb m_LibCompAttr;
	LineAttr m_LineAttr;
	OriginAttributes m_OriginAttr;
	PadAttributes m_PadAttr;
	PcbCompAttributes m_PcbCompAttr;
	PolyFilledAttributes m_PolyFilledAttr;
	PolyAttributes m_PolyAttr;
	PowerAttributes m_PowerAttr;
	RectAttributes m_RectAttr;
	RoundRectAttributes m_RndRectAttr;
	SchPartAttributes m_SchPartAttr;
	SheetAttributes m_SheetAttr;
	SheetPinAttributes m_SheetPinAttr;
	SheetPortAttributes m_SheetPortAttr;
	TextAttributes m_TextAttr;
	TextAttributes m_SchTextAttr;
	TrackAttributes m_TrackAttr;
	ViaAttributes m_ViaAttr;
	WireAttributes m_WireAttr;
	//---------------------------------------
	// Methodes
	//---------------------------------------
public:
	CJamCadApp();


// Overrides
public:
	void WriteLogFile(char * s);
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void UpdateStatusBar(char *s) { ((CMainFrame *)m_pMainWnd)->SetStatusText(s); }
	void SaveSettings();
	void LoadSettings();
	void CutCopy(CCadObject * pObj, int mode);
	// Implementation
	CMyMultiDocTemplate * FindDocTemplate(char * name);
	afx_msg void OnAppAbout();
	afx_msg void OnFileNewProject();
	afx_msg void OnFileNewSchematic();
	afx_msg void OnFileNewLib();
	afx_msg void OnFileNewFrontpanel();
	void CopyToClipBoard(CCadObject * pObjList);
	void CutToClipboard(CCadObject * pSelList);
	void SetClipboardRef(CPoint p);
	inline int IsClipboardReadyToPaste(void) { return m_ClipBoard.IsReadyToPaste(); }
	inline CMoveObjects *GetClipBoard(void) { return &m_ClipBoard; }
	DECLARE_MESSAGE_MAP()
};

extern CJamCadApp theApp;
extern double ArcTan(double x, double y);
extern int CheckAngle(double Start, double End, double Angle);
extern char * MkIndentString(int count, int c = ' ');
