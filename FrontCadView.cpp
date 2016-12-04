//---------------------------------------------
// JamCad Project
// FrontCadView.cpp : implementation file
//----------------------------------------------

#include "stdafx.h"
#include "JamCad.h"
#include "FrontCadView.h"
#include "JamCadDefines.h"
#include "FrontCadChildFrame.h"

// CFrontCadView

IMPLEMENT_DYNCREATE(CFrontCadView, CAutoScrollView)

CFrontCadView::CFrontCadView()
{
}

CFrontCadView::~CFrontCadView()
{
}

BEGIN_MESSAGE_MAP(CFrontCadView, CAutoScrollView)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_BUTTON_ZOOMIN, &CFrontCadView::OnButtonZoomin)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ZOOMIN, &CFrontCadView::OnUpdateButtonZoomin)
	ON_COMMAND(ID_BUTTON_ZOOMOUT, &CFrontCadView::OnButtonZoomout)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ZOOMOUT, &CFrontCadView::OnUpdateButtonZoomout)
	ON_COMMAND(IDM_PLACE_ARC, &CFrontCadView::OnPlaceArc)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_ARC, &CFrontCadView::OnUpdatePlaceArc)
	ON_COMMAND(IDM_PLACE_ARCCENTER, &CFrontCadView::OnPlaceArccenter)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_ARCCENTER, &CFrontCadView::OnUpdatePlaceArccenter)
	ON_COMMAND(IDM_PLACE_BITMAP, &CFrontCadView::OnPlaceBitmap)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_BITMAP, &CFrontCadView::OnUpdatePlaceBitmap)
	ON_COMMAND(IDM_PLACE_DIMENSION, &CFrontCadView::OnPlaceDimension)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_DIMENSION, &CFrontCadView::OnUpdatePlaceDimension)
	ON_COMMAND(IDM_PLACE_ELLISPE, &CFrontCadView::OnPlaceEllispe)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_ELLISPE, &CFrontCadView::OnUpdatePlaceEllispe)
	ON_COMMAND(IDM_PLACE_HOLE1FLAT, &CFrontCadView::OnPlaceHole1flat)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_HOLE1FLAT, &CFrontCadView::OnUpdatePlaceHole1flat)
	ON_COMMAND(IDM_PLACE_HOLEROUND, &CFrontCadView::OnPlaceHoleround)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_HOLEROUND, &CFrontCadView::OnUpdatePlaceHoleround)
	ON_COMMAND(IDM_PLACE_LIBPART, &CFrontCadView::OnPlaceLibpart)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_LIBPART, &CFrontCadView::OnUpdatePlaceLibpart)
	ON_COMMAND(IDM_PLACE_LINE, &CFrontCadView::OnPlaceLine)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_LINE, &CFrontCadView::OnUpdatePlaceLine)
	ON_COMMAND(IDM_PLACE_ORIGIN, &CFrontCadView::OnPlaceOrigin)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_ORIGIN, &CFrontCadView::OnUpdatePlaceOrigin)
	ON_COMMAND(IDM_PLACE_POLYGON, &CFrontCadView::OnPlacePolygon)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_POLYGON, &CFrontCadView::OnUpdatePlacePolygon)
	ON_COMMAND(IDM_PLACE_POLYGONFILLED, &CFrontCadView::OnPlacePolygonfilled)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_POLYGONFILLED, &CFrontCadView::OnUpdatePlacePolygonfilled)
	ON_COMMAND(IDM_PLACE_RECT, &CFrontCadView::OnPlaceRect)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_RECT, &CFrontCadView::OnUpdatePlaceRect)
	ON_COMMAND(IDM_PLACE_RECTANGULARHOLE, &CFrontCadView::OnPlaceRectangularhole)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_RECTANGULARHOLE, &CFrontCadView::OnUpdatePlaceRectangularhole)
	ON_COMMAND(IDM_PLACE_ROUNDEDRECT, &CFrontCadView::OnPlaceRoundedrect)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_ROUNDEDRECT, &CFrontCadView::OnUpdatePlaceRoundedrect)
	ON_COMMAND(IDM_PLACE_ROUNDHOLE2FLAT, &CFrontCadView::OnPlaceRoundhole2flat)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_ROUNDHOLE2FLAT, &CFrontCadView::OnUpdatePlaceRoundhole2flat)
END_MESSAGE_MAP()


// CFrontCadView drawing

void CFrontCadView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	pDC->Rectangle(200, 200, 400, 600);
}


// CFrontCadView diagnostics

#ifdef _DEBUG
void CFrontCadView::AssertValid() const
{
	CAutoScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CFrontCadView::Dump(CDumpContext& dc) const
{
	CAutoScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFrontCadView message handlers


void CFrontCadView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	UpdateMousePositions(point);
	CAutoScrollView::OnMouseMove(nFlags, point);
	Invalidate();
}


BOOL CFrontCadView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	UpdateMousePositions(pt);
	return CAutoScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


void CFrontCadView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	EnableAutoScroll(1);
	UpdateMousePositions(point);
	CAutoScrollView::OnLButtonDown(nFlags, point);
}


void CFrontCadView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	UpdateMousePositions(point);
	CAutoScrollView::OnLButtonUp(nFlags, point);
}



void CFrontCadView::OnInitialUpdate()
{
	SetDocSize(CSize(24000, 24000));	//document size is in mils
	CAutoScrollView::OnInitialUpdate();
}

void CFrontCadView::OnAutoScroll(CPoint /*point*/, BOOL /*bBeforeScroll*/)
{
}



void CFrontCadView::OnPlaceArc()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdatePlaceArc(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnPlaceArccenter()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdatePlaceArccenter(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnPlaceBitmap()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdatePlaceBitmap(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnPlaceDimension()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdatePlaceDimension(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnPlaceEllispe()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdatePlaceEllispe(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnPlaceHole1flat()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdatePlaceHole1flat(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnPlaceHoleround()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdatePlaceHoleround(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnPlaceLibpart()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdatePlaceLibpart(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnPlaceLine()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdatePlaceLine(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnPlaceOrigin()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdatePlaceOrigin(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnPlacePolygon()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdatePlacePolygon(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnPlacePolygonfilled()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdatePlacePolygonfilled(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnPlaceRect()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdatePlaceRect(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnPlaceRectangularhole()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdatePlaceRectangularhole(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnPlaceRoundedrect()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdatePlaceRoundedrect(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnPlaceRoundhole2flat()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdatePlaceRoundhole2flat(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}
