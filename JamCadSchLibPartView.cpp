// JamCadSchLibPartView.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "JamCadSchLibPartView.h"
#include "MainFrm.h"

#include "CadArc.h"
#include "CadArcCent.h"
#include "CadArcPcb.h"
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
#include "CadPin.h"
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

#include "DlgArcCentProperies.h"
#include "DlgArcProperties.h"
#include "DlgArrowProperties.h"
#include "DlgBitmapProperties.h"
#include "DlgEllispeProperties.h"
#include "DlgLineAttributes.h"
#include "DlgPinProp.h"
#include "DlgPoloyProperties.h"
#include "DlgPolyFilledProperties.h"
#include "DlgRectProperties.h"
#include "DlgRndRectProperties.h"
#include "DlgTextProperties.h"

#include "JamCadDefines.h"
#include "JamCadSchLibPartDoc.h"

#include "DlgPinProp.h"

// CJamCadSchLibPartView

IMPLEMENT_DYNCREATE(CJamCadSchLibPartView, CAutoScrollView)

CJamCadSchLibPartView::CJamCadSchLibPartView()
{
	m_CadObj.pCadObject = 0;
	m_pDCbm = 0;
	m_pMemDC = 0;
	m_pGrabbedObject = 0;
	m_pMoveObjects = 0;
	m_Unit = 0;
	m_ControlKeyDown = 0;
	m_ShiftKeyDown = 0;
}

CJamCadSchLibPartView::~CJamCadSchLibPartView()
{
}

BEGIN_MESSAGE_MAP(CJamCadSchLibPartView, CAutoScrollView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_PLACE_ARC, &CJamCadSchLibPartView::OnPlaceArc)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_ARC, &CJamCadSchLibPartView::OnUpdatePlaceArc)
	ON_COMMAND(IDM_PLACE_ARCCENTER, &CJamCadSchLibPartView::OnPlaceArccenter)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_ARCCENTER, &CJamCadSchLibPartView::OnUpdatePlaceArccenter)
	ON_COMMAND(IDM_SCH_PLACE_BITMAP, &CJamCadSchLibPartView::OnPlaceBitmap)
	ON_UPDATE_COMMAND_UI(IDM_SCH_PLACE_BITMAP, &CJamCadSchLibPartView::OnUpdatePlaceBitmap)
	ON_COMMAND(IDM_PLACE_ELLIPSE, &CJamCadSchLibPartView::OnPlaceEllispe)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_ELLIPSE, &CJamCadSchLibPartView::OnUpdatePlaceEllispe)
	ON_COMMAND(IDM_PLACE_LINE, &CJamCadSchLibPartView::OnPlaceLine)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_LINE, &CJamCadSchLibPartView::OnUpdatePlaceLine)
	ON_COMMAND(IDM_PLACE_PIN, &CJamCadSchLibPartView::OnPlacePin)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_PIN, &CJamCadSchLibPartView::OnUpdatePlacePin)
	ON_COMMAND(IDM_SCH_PLACE_POLYGON, &CJamCadSchLibPartView::OnPlacePolygon)
	ON_UPDATE_COMMAND_UI(IDM_SCH_PLACE_POLYGON, &CJamCadSchLibPartView::OnUpdatePlacePolygon)
	ON_COMMAND(IDM_SCH_PLACE_FILLEDPOLY, &CJamCadSchLibPartView::OnPlacePolygonfilled)
	ON_UPDATE_COMMAND_UI(IDM_SCH_PLACE_FILLEDPOLY, &CJamCadSchLibPartView::OnUpdatePlacePolygonfilled)
	ON_COMMAND(IDM_PLACE_RECTANGLE, &CJamCadSchLibPartView::OnPlaceRect)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_RECTANGLE, &CJamCadSchLibPartView::OnUpdatePlaceRect)
	ON_COMMAND(IDM_PLACE_REFERENCE_POINT, &CJamCadSchLibPartView::OnPlaceReferencePoint)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_REFERENCE_POINT, &CJamCadSchLibPartView::OnUpdatePlaceReferencePoint)
	ON_COMMAND(IDM_PLACE_ROUNDEDRECT, &CJamCadSchLibPartView::OnPlaceRoundedrect)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_ROUNDEDRECT, &CJamCadSchLibPartView::OnUpdatePlaceRoundedrect)
	ON_COMMAND(IDM_PLACE_TEXT, &CJamCadSchLibPartView::OnPlaceText)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_TEXT, &CJamCadSchLibPartView::OnUpdatePlaceText)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDM_PLACE_ARROW, &CJamCadSchLibPartView::OnPlaceArrow)
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_COPY, &CJamCadSchLibPartView::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CJamCadSchLibPartView::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &CJamCadSchLibPartView::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CJamCadSchLibPartView::OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, &CJamCadSchLibPartView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CJamCadSchLibPartView::OnUpdateEditPaste)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CJamCadSchLibPartView drawing

void CJamCadSchLibPartView::OnDraw(CDC* pDC)
{
	CJamCadSchLibPartDoc* pDoc = (CJamCadSchLibPartDoc*)GetDocument();
	// TODO: add draw code here
	CCadObject *pO;
	CRect rect;
	CScale scale = GetScale();
	GetClientRect(&rect);
	CBrush br;
	int mode = OBJECT_MODE_FINAL;
	
	br.CreateSolidBrush(RGB(255, 255, 255));
	m_pMemDC->FillRect(&rect, &br);
	pO = pDoc->GetDrawing();
	CSize Offset = -GetScrollOffset();
	DrawGrid(m_pMemDC, 0, 0, RGB(0, 0, 255), 0, rect);
	if (pO) pO->Draw(m_pMemDC, mode, Offset, scale);
	if (m_CadObj.pCadObject)	//is an object being draw?
	{
		switch (m_DrawState)
		{
			case DRAWSTATE_SET_ATTRIBUTES:
			case DRAWSTATE_WAITFORMOUSE_DOWN:
			case DRAWSTATE_PLACE:
			case DRAWSTATE_PLACE_AUTO:
				mode = OBJECT_MODE_SKETCH;
				break;
			case DRAWSTATE_ARCSTART:
				mode = OBJECT_MODE_ARCSTART;
				break;
			case DRAWSTATE_ARCEND:
				mode = OBJECT_MODE_ARCEND;
				break;
		}
		m_CadObj.pCadObject->Draw(m_pMemDC, mode, Offset, scale);
	}
	if (m_pMoveObjects)	//are there any objects being moved?
		m_pMoveObjects->Draw(m_pMemDC, SnapToScreen(m_SnapPos) - Offset, scale);
	DrawCursor(m_pMemDC, SnapToScreen(m_SnapPos), scale, Offset, &rect, RGB(0, 0, 0));
	pDC->BitBlt(0, 0, rect.right, rect.bottom, m_pMemDC, 0, 0, SRCCOPY);
}


// CJamCadSchLibPartView diagnostics

#ifdef _DEBUG
void CJamCadSchLibPartView::AssertValid() const
{
	CAutoScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CJamCadSchLibPartView::Dump(CDumpContext& dc) const
{
	CAutoScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CJamCadSchLibPartView message handlers


void CJamCadSchLibPartView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CJamCadSchLibPartDoc *pDoc = (CJamCadSchLibPartDoc *)GetDocument();
	CCadObject **pSel,*pObjSel;
	int n;

	UpdateMousePositions(point);
	switch (m_DrawMode)
	{
		case DRAWMODE_SELECT:
			switch (m_DrawState)
			{
				case DRAWSTATE_SELECT_NOTHING:
					break;
				case DRAWSTATE_SELECT_MOVE:
					break;
				case DRAWSTATE_SELECT_COPY:
					break;
			}
			Invalidate();
			break;
		case DRAWMODE_LINE:
			switch (m_DrawState)
			{
				case DRAWSTATE_SET_ATTRIBUTES:
					m_CadObj.pCadLine = new CCadLine;
					break;
				case DRAWSTATE_WAITFORMOUSE_DOWN:
					if(!m_CadObj.pCadLine)m_CadObj.pCadLine = new CCadLine;
					break;
				case DRAWSTATE_PLACE:
					break;
			}
			m_DrawState = m_CadObj.pCadLine->MouseDown(this, m_DrawState, &theApp.m_LineAttr);
			break;
		case DRAWMODE_RECT:
			switch (m_DrawState)
			{
				case DRAWSTATE_SET_ATTRIBUTES:
					m_CadObj.pCadRect = new CCadRect;
					break;
				case DRAWSTATE_WAITFORMOUSE_DOWN:
					if (!m_CadObj.pCadLine)m_CadObj.pCadRect = new CCadRect;
					break;
				case DRAWSTATE_PLACE:
					break;
			}
			m_DrawState = m_CadObj.pCadRect->MouseDown(this, m_DrawState, &theApp.m_RectAttr);
			break;
		case DRAWMODE_ELIPSE:
			switch (m_DrawState)
			{
				case DRAWSTATE_SET_ATTRIBUTES:
					m_CadObj.pCadElispe = new CCadElispe;
					break;
				case DRAWSTATE_WAITFORMOUSE_DOWN:
					if (!m_CadObj.pCadElispe)m_CadObj.pCadElispe = new CCadElispe;
					break;
				case DRAWSTATE_PLACE:
					break;
			}
			m_DrawState = m_CadObj.pCadElispe->MouseDown(this, m_DrawState, &theApp.m_EllipseAttr);
			break;
		case DRAWMODE_RNDRECT:
			switch (m_DrawState)
			{
				case DRAWSTATE_SET_ATTRIBUTES:
					m_CadObj.pCadRndRect = new CCadRndRect;
					break;
				case DRAWSTATE_WAITFORMOUSE_DOWN:
					if (!m_CadObj.pCadRndRect)m_CadObj.pCadRndRect = new CCadRndRect;
					break;
				case DRAWSTATE_PLACE:
					break;
			}
			m_DrawState = m_CadObj.pCadRndRect->MouseDown(this, m_DrawState, &theApp.m_RndRectAttr);
			break;
		case DRAWMODE_ARC:
			switch (m_DrawState)
			{
				case DRAWSTATE_SET_ATTRIBUTES:
					m_CadObj.pCadArc = new CCadArc;
					break;
				case DRAWSTATE_WAITFORMOUSE_DOWN:
					if (!m_CadObj.pCadArc)m_CadObj.pCadArc = new CCadArc;
					break;
				case DRAWSTATE_PLACE:
					break;
			}
			m_DrawState = m_CadObj.pCadArc->MouseDown(this, m_DrawState, &theApp.m_ArcAttr);
			break;
		case DRAWMODE_POLYGON:
			switch (m_DrawState)
			{
				case DRAWSTATE_SET_ATTRIBUTES:
					m_CadObj.pCadPolygon = new CCadPolygon;
					break;
				case DRAWSTATE_WAITFORMOUSE_DOWN:
					if (!m_CadObj.pCadPolygon) m_CadObj.pCadPolygon = new CCadPolygon;
					break;
				case DRAWSTATE_PLACE:
					break;
			}
			m_DrawState = m_CadObj.pCadPolygon->MouseDown(this, m_DrawState, &theApp.m_PolyAttr);
			break;
		case DRAWMODE_POLYFILL:
			switch (m_DrawState)
			{
			case DRAWSTATE_SET_ATTRIBUTES:
				m_CadObj.pCadPolyFilled = new CCadPolyFilled;
				break;
			case DRAWSTATE_WAITFORMOUSE_DOWN:
				if (!m_CadObj.pCadPolygon) m_CadObj.pCadPolyFilled = new CCadPolyFilled;
				break;
			case DRAWSTATE_PLACE:
				break;
			}
			m_DrawState = m_CadObj.pCadPolyFilled->MouseDown(this, m_DrawState, &theApp.m_PolyFilledAttr);
			break;
		case DRAWMODE_ARC_CENTER:
			switch (m_DrawState)
			{
				case DRAWSTATE_SET_ATTRIBUTES:
					m_CadObj.pCadArcCent = new CCadArcCent;
					break;
				case DRAWSTATE_WAITFORMOUSE_DOWN:
					if (!m_CadObj.pCadArcCent)m_CadObj.pCadArcCent = new CCadArcCent;
					break;
				case DRAWSTATE_PLACE:
					break;
			}
			m_DrawState = m_CadObj.pCadArcCent->MouseDown(this, m_DrawState, &theApp.m_ArcCentAttr);
			break;
		case DRAWMODE_TEXT:
			switch (m_DrawState)
			{
				case DRAWSTATE_SET_ATTRIBUTES:
					m_CadObj.pCadText = new CCadText;
					break;
				case DRAWSTATE_WAITFORMOUSE_DOWN:
					if (!m_CadObj.pCadText)m_CadObj.pCadText = new CCadText;
					break;
				case DRAWSTATE_PLACE:
					break;
			}
			m_DrawState = m_CadObj.pCadText->MouseDown(this, m_DrawState, &theApp.m_TextAttr);
			break;
		case DRAWMODE_MOVE:
			break;
		case DRAWMODE_COPY:
			break;
		case DRAWMODE_CUT:
			break;
		case DRAWMODE_PASTE:
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITFORMOUSE_DOWN:
					m_pMoveObjects = new CMoveObjects;
					m_pMoveObjects->Copy(theApp.GetClipBoard());
					Invalidate();
					break;
				case DRAWSTATE_PLACE:
					break;
			}
			break;
		case DRAWMODE_SELECTREGION:
			break;
		case DRAWMODE_BITMAP:
			switch (m_DrawState)
			{
			case DRAWSTATE_SET_ATTRIBUTES:
				m_CadObj.pCadBitmap = new CCadBitmap;
				break;
			case DRAWSTATE_WAITFORMOUSE_DOWN:
				if (!m_CadObj.pCadBitmap)m_CadObj.pCadBitmap = new CCadBitmap;
				break;
			case DRAWSTATE_PLACE:
				break;
			}
			m_DrawState = m_CadObj.pCadBitmap->MouseDown(this, m_DrawState, &theApp.m_BitmapAttr);
			break;
		case DRAWMODE_ARROW:
			switch (m_DrawState)
			{
				case DRAWSTATE_SET_ATTRIBUTES:
					m_CadObj.pCadArrow = new CCadArrow;
					break;
				case DRAWSTATE_WAITFORMOUSE_DOWN:
					if (!m_CadObj.pCadArrow)m_CadObj.pCadArrow = new CCadArrow;
					break;
				case DRAWSTATE_PLACE:
					break;
			}
			m_DrawState = m_CadObj.pCadBitmap->MouseDown(this, m_DrawState, &theApp.m_ArrowAttr);
			break;
		case DRAWMODE_PIN:
			switch (m_DrawState)
			{
				case DRAWSTATE_SET_ATTRIBUTES:
					m_CadObj.pCadPin = new CCadPin;
					break;
				case DRAWSTATE_WAITFORMOUSE_DOWN:
					if (!m_CadObj.pCadPin)m_CadObj.pCadPin = new CCadPin;
					break;
				case DRAWSTATE_PLACE:
					break;
				case DRAWSTATE_PLACE_AUTO:
					if (!m_CadObj.pCadPin)m_CadObj.pCadPin = new CCadPin;
					break;
			}
			m_DrawState = m_CadObj.pCadPin->MouseDown(this, m_DrawState, &theApp.m_ArcAttr);
			break;
	}
	CAutoScrollView::OnLButtonDown(nFlags, point);
}


void CJamCadSchLibPartView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CJamCadSchLibPartDoc *pDoc = (CJamCadSchLibPartDoc *)GetDocument();
	CCadObject **pSel, *pObjSel, *pObj;
	int n;
	//-------------------------------------------------------------
	//	OnLButtonUp
	//		This is the method that processes all of the Left MOuse
	//	button UP messages
	//  
	//-------------------------------------------------------------
	UpdateMousePositions(point);
	switch (m_DrawMode)
	{
		case DRAWMODE_SELECT:
			switch (m_DrawState)
			{
				case DRAWSTATE_SELECT_NOTHING:
					pSel = new CCadObject *[8];
					if (n = pDoc->CheckSelected(m_ScalePos, pSel, 8, CADDRAWUBG_CHECKSEL_FLAG_ALL))
					{
						//we have found some objects to select
						if (n > 1)
						{
							int index = SelectAnObject(pSel, n, point);
							if (index < 0)
								break;
							pObjSel = pSel[index];
						}
						else
							pObjSel = pSel[0];
						if (pObjSel->IsSelected())
						{
							pObjSel->SetSelected(0);	//deselect object
							RemoveObject(pObjSel);
						}
						else
						{
							if (m_SelectionCount && !m_ControlKeyDown)
							{
								CCadObject *pO = GetTopSelection();
								while (pO)
								{
									RemoveObject(pO);	//remove object from selection list
									pO->SetSelected(0);	//set to unselected
									pO = GetTopSelection();	//get next object, if any
								}
							}
							pObjSel->SetSelected(1);
							AddToSelList(pObjSel);
						}
						Invalidate();
					}//end of if(n = pDoc->CheckSelected(m_ScalePos, pSel, 8, CADDRAWUBG_CHECKSEL_FLAG_ALL))
					else
					{
						//--------------------------------
						// No objects were selected
						// So, just to irritate the user
						// we will deselect all of the objects
						// that have already been selected
						//---------------------------------
						if (m_SelectionCount && !m_ControlKeyDown)
						{
							CCadObject *pO = GetTopSelection();
							while (pO)
							{
								RemoveObject(pO);	//remove object from selection list
								pO->SetSelected(0);	//set to unselected
								pO = GetTopSelection();	//get next object, if any
							}
							Invalidate();
						}
					}
					break;
				case DRAWSTATE_SELECT_MOVE:
				case DRAWSTATE_SELECT_COPY:
					pObj = m_pMoveObjects->RemoveFromHead();
					while (pObj)
					{
						pObj->AdjustRefernce(-m_SnapPos);
						pObj->SetSelected(0);
						pDoc->AddObject(pObj);
						pObj = m_pMoveObjects->RemoveFromHead();
					}
					delete m_pMoveObjects;
					m_pMoveObjects = 0;
					m_DrawState = DRAWSTATE_SELECT_NOTHING;
					Invalidate();
					break;
			}
			if (m_pGrabbedObject)
			{
				m_pGrabbedObject->Move(m_SnapPos);
				m_pGrabbedObject = 0; 
			}
			break;
		case DRAWMODE_LINE:
			m_DrawState = m_CadObj.pCadLine->MouseUp(this, m_DrawState, &theApp.m_LineAttr);
			break;
		case DRAWMODE_RECT:
			m_DrawState = m_CadObj.pCadRect->MouseUp(this, m_DrawState, &theApp.m_RectAttr);
			break;
		case DRAWMODE_ELIPSE:
			m_DrawState = m_CadObj.pCadElispe->MouseUp(this, m_DrawState, &theApp.m_EllipseAttr);
			break;
		case DRAWMODE_RNDRECT:
			m_DrawState = m_CadObj.pCadRndRect->MouseUp(this, m_DrawState, &theApp.m_RndRectAttr);
			break;
		case DRAWMODE_ARC:
			m_DrawState = m_CadObj.pCadArc->MouseUp(this, m_DrawState, &theApp.m_ArcAttr);
			break;
		case DRAWMODE_POLYGON:
			m_DrawState = m_CadObj.pCadPolygon->MouseUp(this, m_DrawState, &theApp.m_PolyAttr);
			break;
		case DRAWMODE_POLYFILL:
			m_DrawState = m_CadObj.pCadPolyFilled->MouseUp(this, m_DrawState, &theApp.m_PolyFilledAttr);
			break;
		case DRAWMODE_ARC_CENTER:
			m_DrawState = m_CadObj.pCadArcCent->MouseUp(this, m_DrawState, &theApp.m_ArcCentAttr);
			break;
		case DRAWMODE_TEXT:
			m_DrawState = m_CadObj.pCadText->MouseUp(this, m_DrawState, &theApp.m_TextAttr);
			break;
		case DRAWMODE_MOVE:
			break;
		case DRAWMODE_COPY:
			theApp.GetClipBoard()->Clear();		//clear old contents from clipboard
			theApp.CopyToClipBoard(m_pSelObjList);
			theApp.SetClipboardRef(m_SnapPos);
			Invalidate();
			break;
		case DRAWMODE_CUT:
			{
				CJamCadSchLibPartDoc *pDoc = (CJamCadSchLibPartDoc *)GetDocument();
				theApp.CutToClipboard(this->m_pSelObjList);
				theApp.SetClipboardRef(m_SnapPos);
				CCadObject *pO = GetTopSelection();
				while (pO)
				{
					pDoc->RemoveObject(pO);
					this->RemoveObject(pO);
					pO = GetTopSelection();
				}
				Invalidate();
			}
			break;
		case DRAWMODE_PASTE:	//Mouse Button Up
			switch (m_DrawState)
			{
				case DRAWSTATE_WAITFORMOUSE_DOWN:
					theApp.UpdateStatusBar("PASTE:Position Objects");
					m_DrawState = DRAWSTATE_PLACE;
					break;
				case DRAWSTATE_PLACE:
					theApp.UpdateStatusBar("SELECT an Object");
					m_DrawMode = DRAWMODE_SELECT;
					m_DrawState = DRAWSTATE_SELECT_NOTHING;
					{
						CJamCadSchLibPartDoc *pDoc = (CJamCadSchLibPartDoc *)GetDocument();
						CCadObject *pO = m_pMoveObjects->RemoveFromHead();
						while (pO)
						{
							pO->SetSelected(0);
							pO->AdjustRefernce(-m_SnapPos);
							pDoc->AddObject(pO);
							pO = m_pMoveObjects->RemoveFromHead();
						}
						delete m_pMoveObjects;
						m_pMoveObjects = 0;
					}
					break;
			}
			break;
		case DRAWMODE_SELECTREGION:
			break;
		case DRAWMODE_BITMAP:
			m_DrawState = m_CadObj.pCadBitmap->MouseUp(this, m_DrawState, &theApp.m_BitmapAttr);
			break;
		case DRAWMODE_ARROW:
			m_DrawState = m_CadObj.pCadArrow->MouseUp(this, m_DrawState, &theApp.m_ArrowAttr);
			break;
		case DRAWMODE_PIN:
			m_DrawState = m_CadObj.pCadPin->MouseUp(this, m_DrawState, NULL);
			break;
	}
	CAutoScrollView::OnLButtonUp(nFlags, point);
}


void CJamCadSchLibPartView::OnMouseMove(UINT nFlags, CPoint point)
{
	CJamCadSchLibPartDoc *pDoc = (CJamCadSchLibPartDoc *)GetDocument();

	UpdateMousePositions(point);
	switch (m_DrawMode)
	{
		case DRAWMODE_SELECT:
			switch (m_DrawState)
			{
				case DRAWSTATE_SELECT_NOTHING:
					//-----------------------------------
					// If the mouse moves,
					// while down, we are then
					// going to grab and either move or
					// copy the objects
					//------------------------------------
					if (IsMouseDown() && m_SelectionCount)
					{
						if (m_ShiftKeyDown)	//copy selection
						{
							m_pMoveObjects = new CMoveObjects();
							CCadObject *pObj = GetTopSelection();
							while (pObj)
							{
								m_pMoveObjects->AddObject(pObj->CopyObject());
								pObj = pObj->GetNextSel();
							}
							m_DrawState = DRAWSTATE_SELECT_COPY;
						}
						else  //move selection
						{
							m_pMoveObjects = new CMoveObjects();
							CCadObject *pObj = GetTopSelection();
							while (pObj)
							{
								RemoveObject(pObj);
								pDoc->RemoveObject(pObj);
								m_pMoveObjects->AddObject(pObj);
								pObj = GetTopSelection();
							}
							m_DrawState = DRAWSTATE_SELECT_MOVE;
						}
						m_pMoveObjects->SetRef(m_SnapPos);
					}	//end of if MouseDown
					break;
				case DRAWSTATE_SELECT_MOVE:
				case DRAWSTATE_SELECT_COPY:
					break;
			}
			Invalidate();
			if (IsMouseDown())
			{
				if (m_pGrabbedObject)
					m_pGrabbedObject->Move(m_SnapPos);
			}
			break;
		case DRAWMODE_LINE:
			if (m_CadObj.pCadLine) m_CadObj.pCadLine->MouseMove(this, m_DrawState);
			Invalidate();
			break;
		case DRAWMODE_RECT:
			if(m_CadObj.pCadRect)
				m_DrawState = m_CadObj.pCadRect->MouseMove(this, m_DrawState);
			Invalidate();
			break;
		case DRAWMODE_ELIPSE:
			if (m_CadObj.pCadElispe)
				m_DrawState = m_CadObj.pCadElispe->MouseMove(this, m_DrawState);
			Invalidate();
			break;
		case DRAWMODE_RNDRECT:
			if (m_CadObj.pCadRndRect)
				m_DrawState = m_CadObj.pCadRndRect->MouseMove(this, m_DrawState);
			break;
		case DRAWMODE_ARC:
			if(m_CadObj.pCadArc)
				m_DrawState = m_CadObj.pCadArc->MouseMove(this, m_DrawState);
			break;
		case DRAWMODE_POLYGON:
			if (m_CadObj.pCadPolygon)
				m_DrawState = m_CadObj.pCadPolygon->MouseMove(this, m_DrawState);
			break;
		case DRAWMODE_POLYFILL:
			if (m_CadObj.pCadPolyFilled)
				m_DrawState = m_CadObj.pCadPolyFilled->MouseMove(this, m_DrawState);
			break;
		case DRAWMODE_ARC_CENTER:
			if (m_CadObj.pCadArcCent)
				m_DrawState = m_CadObj.pCadArcCent->MouseMove(this, m_DrawState);
			break;
		case DRAWMODE_TEXT:
			if (m_CadObj.pCadRect)
				m_DrawState = m_CadObj.pCadText->MouseMove(this, m_DrawState);
			Invalidate();
			break;
		case DRAWMODE_MOVE:
			break;
		case DRAWMODE_COPY:
			Invalidate();
			break;
		case DRAWMODE_CUT:
			Invalidate();
			break;
		case DRAWMODE_PASTE:
			Invalidate();
			break;
		case DRAWMODE_SELECTREGION:
			break;
		case DRAWMODE_BITMAP:
			if(m_CadObj.pCadBitmap) m_DrawState = m_CadObj.pCadBitmap->MouseMove(this, m_DrawState);
			break;
		case DRAWMODE_ARROW:
			if (m_CadObj.pCadArrow) m_DrawState = m_CadObj.pCadArrow->MouseMove(this, m_DrawState);
			break;
		case DRAWMODE_PIN:
			if (m_CadObj.pCadPin) m_DrawState = m_CadObj.pCadPin->MouseMove(this, m_DrawState);
			break;
	}
	CAutoScrollView::OnMouseMove(nFlags, point);
}


void CJamCadSchLibPartView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
		case VK_CONTROL:
			m_ControlKeyDown = 1;
			switch (m_DrawMode)
			{
				case DRAWMODE_SELECT:
					switch (m_DrawState)
					{
						case DRAWSTATE_SELECT_MOVE:
							break;
						case DRAWSTATE_SELECT_NOTHING:
							break;
						case DRAWSTATE_SELECT_COPY:
							break;

					}	//end of switch(m_DrawState
					break;
			}	//end of switch(m_DrawMode)
			break;
		case VK_SHIFT:
			m_ShiftKeyDown = 1;
			switch (m_DrawMode)
			{
				case DRAWMODE_SELECT:
					switch (m_DrawState)
					{
						case DRAWSTATE_SELECT_MOVE:
							break;
						case DRAWSTATE_SELECT_NOTHING:
							break;
						case DRAWSTATE_SELECT_COPY:
							break;
					}	//end of switch(m_DrawState
					break;
			}	//end of switch(m_DrawMode)
			break;

	}
	CAutoScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CJamCadSchLibPartView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CCadObject **ppSelList;
	int n;

	CJamCadSchLibPartDoc *pDoc = (CJamCadSchLibPartDoc *)GetDocument();
	switch (nChar)
	{
		case VK_ESCAPE:
//			if (IsScrolling())
			{
				CAutoScrollView::EnableAutoScroll(0);
//				CAutoScrollView::PostMessage(WM_STOPAUTOSCROLL);
			}
			if (DRAWSTATE_PLACE == m_DrawState 
				|| DRAWSTATE_WAITFORMOUSE_DOWN == m_DrawState
				|| DRAWSTATE_PLACE_AUTO == m_DrawState)
			{
				switch (m_DrawMode)
				{
					case DRAWMODE_ARC:
					case DRAWMODE_ARC_CENTER:
					case DRAWMODE_ARROW:
					case DRAWMODE_BITMAP:
					case DRAWMODE_ELIPSE:
					case DRAWMODE_LINE:
					case DRAWMODE_PIN:
					case DRAWMODE_POLYFILL:
					case DRAWMODE_POLYGON:
					case DRAWMODE_RECT:
					case DRAWMODE_RNDRECT:
					case DRAWMODE_TEXT:
						m_DrawState = DRAWSTATE_SET_ATTRIBUTES;
						if (m_CadObj.pCadObject)delete m_CadObj.pCadObject;
						m_CadObj.pCadObject = 0;
						break;
				}
			}
			break;
		case 'R':	//rotate object counter clock wise
			ppSelList = new CCadObject *[8];
			n = pDoc->CheckSelected(m_ScalePos, ppSelList, 8, CADDRAWUBG_CHECKSEL_FLAG_ALL);
			if (n > 0)
			{
				ppSelList[0]->Rotate(ppSelList[0]->GetCenter(), ROTATION_90);
			}
			delete[]ppSelList;
			Invalidate();
			break;
		case 'X':
			ppSelList = new CCadObject *[8];
			n = pDoc->CheckSelected(m_ScalePos, ppSelList, 8, CADDRAWUBG_CHECKSEL_FLAG_ALL);
			if (n > 0)
			{
				ppSelList[0]->Flip(ppSelList[0]->GetCenter(), 0);
			}
			delete[]ppSelList;
			Invalidate();
			break;
		case 'Y':
			ppSelList = new CCadObject *[8];
			n = pDoc->CheckSelected(m_ScalePos, ppSelList, 8, CADDRAWUBG_CHECKSEL_FLAG_ALL);
			if (n > 0)
			{
				ppSelList[0]->Flip(ppSelList[0]->GetCenter(), 1);
			}
			delete[]ppSelList;
			Invalidate();
			break;
		case VK_CONTROL:
			m_ControlKeyDown = 0;
			switch (m_DrawMode)
			{
				case DRAWMODE_SELECT:
					switch (m_DrawState)
					{
						case DRAWSTATE_SELECT_MOVE:
							break;
						case DRAWSTATE_SELECT_NOTHING:
							break;
						case DRAWSTATE_SELECT_COPY:
							break;
					}	//end of switch(m_DrawState)
					break;
			}	//end of switch(m_DrawMode)
			break;
		case VK_SHIFT:
			m_ShiftKeyDown = 0;
			switch (m_DrawMode)
			{
				case DRAWMODE_SELECT:
					switch (m_DrawState)
					{
						case DRAWSTATE_SELECT_MOVE:
							break;
						case DRAWSTATE_SELECT_NOTHING:
							break;
						case DRAWSTATE_SELECT_COPY:
							break;
					}	//end of switch(m_DrawState
					break;
			}	//end of switch(m_DrawMode)
			break;
	}
	CAutoScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CJamCadSchLibPartView::OnInitialUpdate()
{
	CJamCadSchLibPartDoc *pDoc = (CJamCadSchLibPartDoc *)GetDocument();
	SetDocSize(CSize(24000, 24000));	//document size is in mils
	CAutoScrollView::OnInitialUpdate();
	SetSnapGrid(100);
	CreateMemDC();
	pDoc->m_pDrawing->SetView(this);
}

#define ID_CM_FORWARD				3100
#define ID_CM_BACKWARD				3101
#define ID_CM_DELETE				3102
#define ID_CM_MOVE					3103
#define ID_CM_RESTOREASPECTRATIO	3104
#define ID_CM_ENDDRAWMODE			3105
#define ID_CM_DESELECT				3106
#define ID_CM_DESELECT_ALL			3107
#define ID_CM_EDIT_PROPERIES		3108

void CJamCadSchLibPartView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CJamCadSchLibPartDoc *pDoc = (CJamCadSchLibPartDoc *)GetDocument();
	CMenu ConTexMenu;
	int id;
	CPoint m_MousePos = point;
	ScreenToClient(&m_MousePos);
	m_MousePos = CorrectMousePosition(m_MousePos);
	CCadObject *pObj = GetTopSelection();;
	ConTexMenu.CreatePopupMenu();
	if (m_DrawMode != DRAWMODE_SELECT)
	{
		ConTexMenu.AppendMenuA(MF_STRING, ID_CM_ENDDRAWMODE, "End Draw Mode");
		ConTexMenu.EnableMenuItem(ID_CM_ENDDRAWMODE, MF_ENABLED);
		ConTexMenu.AppendMenuA(MF_STRING, ID_CM_EDIT_PROPERIES, "Edit Properties");
		ConTexMenu.EnableMenuItem(ID_CM_EDIT_PROPERIES, MF_ENABLED);
		ConTexMenu.AppendMenu(MF_SEPARATOR);
	}
	ConTexMenu.AppendMenu(MF_STRING, ID_CM_FORWARD, "Send Obj Forward");
	ConTexMenu.AppendMenu(MF_STRING, ID_CM_BACKWARD, "Send Obj Back");
	ConTexMenu.AppendMenu(MF_STRING, ID_CM_DELETE, "Delete");
	ConTexMenu.AppendMenu(MF_SEPARATOR);
	ConTexMenu.AppendMenu(MF_STRING, ID_CM_MOVE, "Move");
	if (pObj)	//is anything slected?
	{
		ConTexMenu.EnableMenuItem(ID_CM_FORWARD, MF_ENABLED);
		ConTexMenu.EnableMenuItem(ID_CM_BACKWARD, MF_ENABLED);
		ConTexMenu.EnableMenuItem(ID_CM_DELETE, MF_ENABLED);
		ConTexMenu.EnableMenuItem(ID_CM_MOVE, MF_ENABLED);
		if (pObj->GetType() == OBJECT_TYPE_BITMAP)
		{
			ConTexMenu.AppendMenu(MF_STRING, ID_CM_RESTOREASPECTRATIO, "Restore Aspect Ratio");
			ConTexMenu.EnableMenuItem(ID_CM_RESTOREASPECTRATIO, MF_ENABLED);
		}
	}
	else
	{
		ConTexMenu.EnableMenuItem(ID_CM_FORWARD, MF_GRAYED);
		ConTexMenu.EnableMenuItem(ID_CM_BACKWARD, MF_GRAYED);
		ConTexMenu.EnableMenuItem(ID_CM_DELETE, MF_GRAYED);
		ConTexMenu.EnableMenuItem(ID_CM_MOVE, MF_GRAYED);
	}

	id = ConTexMenu.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
	ConTexMenu.DestroyMenu();
	switch (id)
	{
		case ID_CM_ENDDRAWMODE:
			m_DrawMode = DRAWMODE_SELECT;
			m_DrawState = DRAWSTATE_SELECT_NOTHING;
			if (m_CadObj.pCadObject) delete m_CadObj.pCadObject;
			EnableAutoScroll(0);
			break;
		case ID_CM_EDIT_PROPERIES:
			switch (m_DrawMode)
			{
				case DRAWMODE_LINE:
					if (m_CadObj.pCadLine)
						m_CadObj.pCadLine->EditProperties();
					else
						DoLineAttrbDlg();
					break;
				case DRAWMODE_RECT:
					if (m_CadObj.pCadRect)
						m_CadObj.pCadRect->EditProperties();
					else
						DoRectPropertiesDlg();
					break;
				case DRAWMODE_ELIPSE:
					if (m_CadObj.pCadElispe)
						m_CadObj.pCadElispe->EditProperties();
					else
						DoEllipsePropertiesDlg();
					break;
				case DRAWMODE_RNDRECT:
					if (m_CadObj.pCadRndRect)
						m_CadObj.pCadRndRect->EditProperties();
					else
						DoRndRectPropertiesDlg();
					break;
				case DRAWMODE_ARC:
					if (m_CadObj.pCadArc)
						m_CadObj.pCadArc->EditProperties();
					else
						DoArcPropertiesDlg();
					break;
				case DRAWMODE_POLYGON:
					if (m_CadObj.pCadPolygon)
						m_CadObj.pCadPolygon->EditProperties();
					else
						DoPolygonPropertiesDlg();
					break;
				case DRAWMODE_POLYFILL:
					if (m_CadObj.pCadPolyFilled)
						m_CadObj.pCadPolyFilled->EditProperties();
					else
						DoPolygonFilledPropertiesDlg();
					break;
				case DRAWMODE_ARC_CENTER:
					if (m_CadObj.pCadArcCent)
						m_CadObj.pCadArcCent->EditProperties();
					else
						DoArcCenterPropertiesDlg();
					break;
				case DRAWMODE_TEXT:
					if (m_CadObj.pCadText)
						m_CadObj.pCadText->EditProperties();
					else
						DoTextPropertiesDlg();
					break;
				case DRAWMODE_BITMAP:
					if (m_CadObj.pCadBitmap)
						m_CadObj.pCadBitmap->EditProperties();
					else
						DoBitmapPropertiesDlg();
					break;
				case DRAWMODE_ARROW:
					if (m_CadObj.pCadArrow)
						m_CadObj.pCadArrow->EditProperties();
					else
						DoArrowPropertiesDlg();
					break;
				case DRAWMODE_PIN:
					if (m_CadObj.pCadPin)
						m_CadObj.pCadPin->EditProperties();
					else
					{
						m_CadObj.pCadPin = new CCadPin;
						m_CadObj.pCadPin->EditProperties();
						if (m_CadObj.pCadPin->m_AutoIncEnable)
							m_DrawState = DRAWSTATE_PLACE_AUTO;
						else
							m_DrawState = DRAWSTATE_PLACE;
					}
					break;
				case ID_CM_FORWARD:		//make object draw last
					break;
				case ID_CM_BACKWARD:	//make object draw first
					break;
			}	//end of Switch(m_DrawMode)
			break;
	}
}

void CJamCadSchLibPartView::OnPlaceArc()
{
	m_DrawState = DRAWSTATE_SET_ATTRIBUTES;
	m_DrawMode = DRAWMODE_ARC;
	theApp.UpdateStatusBar("Place Arc");
}


void CJamCadSchLibPartView::OnUpdatePlaceArc(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}


void CJamCadSchLibPartView::OnPlaceArccenter()
{
	m_DrawState = DRAWSTATE_SET_ATTRIBUTES;
	m_DrawMode = DRAWMODE_ARC_CENTER;
	theApp.UpdateStatusBar("Place Arc Arround Center Point");
}


void CJamCadSchLibPartView::OnUpdatePlaceArccenter(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}


void CJamCadSchLibPartView::OnPlaceBitmap()
{
	// TODO: Add your command handler code here
	m_DrawMode = DRAWMODE_BITMAP;
	m_DrawState = DRAWSTATE_SET_ATTRIBUTES;
	theApp.UpdateStatusBar("Place Bitmap Image");
}


void CJamCadSchLibPartView::OnUpdatePlaceBitmap(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}


void CJamCadSchLibPartView::OnPlaceEllispe()
{
	// TODO: Add your command handler code here
	m_DrawMode = DRAWMODE_ELIPSE;
	m_DrawState = DRAWSTATE_SET_ATTRIBUTES;
	theApp.UpdateStatusBar("Place Ellipse");
}


void CJamCadSchLibPartView::OnUpdatePlaceEllispe(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}


void CJamCadSchLibPartView::OnPlaceLine()
{
	// TODO: Add your command handler code here
	m_DrawMode = DRAWMODE_LINE;
	m_DrawState = DRAWSTATE_SET_ATTRIBUTES;
	theApp.UpdateStatusBar("Place Line");
}

void CJamCadSchLibPartView::OnUpdatePlaceLine(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CJamCadSchLibPartView::OnPlacePin()
{
	// TODO: Add your command handler code here
	m_DrawMode = DRAWMODE_PIN;
	m_DrawState = DRAWSTATE_SET_ATTRIBUTES;
	theApp.UpdateStatusBar("Place Pin");
}


void CJamCadSchLibPartView::OnUpdatePlacePin(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}


void CJamCadSchLibPartView::OnPlacePolygon()
{
	// TODO: Add your command handler code here
	m_DrawMode = DRAWMODE_POLYGON;
	m_DrawState = DRAWSTATE_SET_ATTRIBUTES;
	theApp.UpdateStatusBar("Place Polygon");
}


void CJamCadSchLibPartView::OnUpdatePlacePolygon(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
}


void CJamCadSchLibPartView::OnPlacePolygonfilled()
{
	m_DrawMode = DRAWMODE_POLYFILL;
	m_DrawState = DRAWSTATE_SET_ATTRIBUTES;
	theApp.UpdateStatusBar("Place Filled Polygon");
}


void CJamCadSchLibPartView::OnUpdatePlacePolygonfilled(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}


void CJamCadSchLibPartView::OnPlaceRect()
{
	m_DrawMode = DRAWMODE_RECT;
	m_DrawState = DRAWSTATE_SET_ATTRIBUTES;
	theApp.UpdateStatusBar("Place Rectangle");
}


void CJamCadSchLibPartView::OnUpdatePlaceRect(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(true);
}


void CJamCadSchLibPartView::OnPlaceReferencePoint()
{
	theApp.UpdateStatusBar("Place Anchor Point");
}


void CJamCadSchLibPartView::OnUpdatePlaceReferencePoint(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CJamCadSchLibPartView::OnPlaceRoundedrect()
{
	m_DrawMode = DRAWMODE_RNDRECT;
	m_DrawState = DRAWSTATE_SET_ATTRIBUTES;
	theApp.UpdateStatusBar("Place Rounded Rectangle");
}


void CJamCadSchLibPartView::OnUpdatePlaceRoundedrect(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}

void CJamCadSchLibPartView::InsertObjectIntoDoc(CCadObject *pObj)
{
	CJamCadSchLibPartDoc *pDoc = (CJamCadSchLibPartDoc *)GetDocument();
	pDoc->InsertObject(pObj);
}

void CJamCadSchLibPartView::AddObjectIntoDoc(CCadObject *pObj)
{
	CJamCadSchLibPartDoc *pDoc = (CJamCadSchLibPartDoc *)GetDocument();
	pDoc->AddObject(pObj);
}


void CJamCadSchLibPartView::OnPlaceText()
{
	// TODO: Add your command handler code here
	m_DrawMode = DRAWMODE_TEXT;
	m_DrawState = DRAWSTATE_SET_ATTRIBUTES;
	theApp.UpdateStatusBar("Place Text");
}


void CJamCadSchLibPartView::OnUpdatePlaceText(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(1);
}


BOOL CJamCadSchLibPartView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

//	return CAutoScrollView::OnEraseBkgnd(pDC);
	return true;
}


void CJamCadSchLibPartView::OnPlaceArrow()
{
	m_DrawMode = DRAWMODE_ARROW;
	m_DrawState = DRAWSTATE_SET_ATTRIBUTES;
	theApp.UpdateStatusBar("Place Arrow");
}


void CJamCadSchLibPartView::CreateMemDC()
{
	//----------------------------------------
	// Create a memory device context
	//----------------------------------------
	CRect clientrect;
	GetClientRect(&clientrect);
	CDC *pDC = GetDC();
	if (m_pMemDC) delete m_pMemDC;
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(pDC);
	if (m_pDCbm) delete m_pDCbm;
	m_pDCbm = new CBitmap;
	m_pDCbm->CreateBitmap(clientrect.Width(), clientrect.Height(), 1, 32, NULL);
	m_pMemDC->SelectObject(m_pDCbm);
	ReleaseDC(pDC);
}


void CJamCadSchLibPartView::OnSize(UINT nType, int cx, int cy)
{
	CAutoScrollView::OnSize(nType, cx, cy);
	CreateMemDC();
	Invalidate();
}


void CJamCadSchLibPartView::OnEditCopy()
{
	theApp.UpdateStatusBar("COPY:SelectReferencePoint");
	m_DrawMode = DRAWMODE_COPY;
}


void CJamCadSchLibPartView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CJamCadSchLibPartView::OnEditCut()
{
	theApp.UpdateStatusBar("CUT:SelectReferencePoint");
	m_DrawMode = DRAWMODE_CUT;
}


void CJamCadSchLibPartView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CJamCadSchLibPartView::OnEditPaste()
{
	theApp.UpdateStatusBar("PASTE:Place Objects");
	m_DrawMode = DRAWMODE_PASTE;
	m_DrawState = DRAWSTATE_WAITFORMOUSE_DOWN;
}


void CJamCadSchLibPartView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.IsClipboardReadyToPaste() ? true : false);
}


void CJamCadSchLibPartView::OnDestroy()
{
	EnableAutoScroll(0);
	CAutoScrollView::OnDestroy();
}
