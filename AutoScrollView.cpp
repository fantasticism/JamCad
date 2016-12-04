//---------------------------------------------
// JamCad Project
// AutoScrollView.cpp : implementation file
//
// This class implements autoscrolling,
// Zooming, and rulers for the JAMCAD EDA
// package.
//----------------------------------------------

#include "stdafx.h"
#include "JamCad.h"
#include "AutoScrollView.h"
#include "JamCadDefines.h"
#include "FrontCadChildFrame.h"

double ScaleFactor[MAX_ZOOM] = {
	2.0,
	2.5,
	2.0,
	2.0,
	2.5,
	2.0
};

double ZF[MAX_ZOOM] = {
	10.0,		//1000 pixles per inch
	5.0		,	//500 pixels per inch
	2.0,		//200 pixels per inch
	1.0,		//100 pixels per inch
	0.50,		// 50 pixels per inch
	0.20,		// 20 pixels per inch
	0.10		// 10 pixels per inch
};

double Scale[MAX_ZOOM] = {
	1.0,
	0.5,
	0.2,
	0.1,
	0.05,
	0.02,
	0.01
};

int ZoomScaleDiv[MAX_ZOOM] = {
	1,
	2,
	5,
	10,
	20,
	50,
	100
};
// CAutoScrollView

IMPLEMENT_DYNCREATE(CAutoScrollView, CView)

CAutoScrollView::CAutoScrollView()
{

	m_VScrollPos = 0;
	m_HScrollPos = 0;
	m_HPageSize = 0;
	m_VPageSize = 0;
	m_AutoScroll = 0;
	m_IsScrolling = 0;
	m_ShowRulers = 0;
	m_ZoomFactor = 3;
	m_SnapOff = 0;
	m_CurMousePos = CPoint(0, 0);
	m_SnapPos = CPoint(0, 0);
	m_ScalePos = CPoint(0, 0);
	m_SnapGrid = 250;
	m_MouseDown = 0;
	Block = 0;
	m_SelectionCount = 0;
	m_pSelObjList = 0;
	m_pSelObjEnd = 0;
	m_GridOn = 1;
}

CAutoScrollView::~CAutoScrollView()
{
}

CScale CAutoScrollView::GetScale()
{
	return CScale(Scale[m_ZoomFactor], Scale[m_ZoomFactor]);
}

BEGIN_MESSAGE_MAP(CAutoScrollView, CView)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_COMMAND(ID_BUTTON_ZOOMIN, &CAutoScrollView::OnButtonZoomin)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ZOOMIN, &CAutoScrollView::OnUpdateButtonZoomin)
	ON_COMMAND(ID_BUTTON_ZOOMOUT, &CAutoScrollView::OnButtonZoomout)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ZOOMOUT, &CAutoScrollView::OnUpdateButtonZoomout)
	ON_WM_SIZE()
	//ON_MESSAGE(WM_STOPAUTOSCROLL, &CAutoScrollView::OnStopautoscroll)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CAutoScrollView drawing

void CAutoScrollView::OnDraw(CDC* pDC)
{
	// Empty
}


// CAutoScrollView diagnostics

#ifdef _DEBUG
void CAutoScrollView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CAutoScrollView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAutoScrollView message handlers


void CAutoScrollView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CView::OnMouseMove(nFlags, point);
	UpdateRulers(RW_POSITION, CPoint(m_HScrollPos, m_VScrollPos), SnapToScreen(m_SnapPos));
	AutoScroll(WM_STOPAUTOSCROLL);
}


BOOL CAutoScrollView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CAutoScrollView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_MouseDown = 1;
	CView::OnLButtonDown(nFlags, point);
}


void CAutoScrollView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_MouseDown = 0;
	CView::OnLButtonUp(nFlags, point);
}

void CAutoScrollView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	///------------------------------------------------
	/// OnVScroll
	///		This is the function that is called when
	/// the user presses the VerticalScroll bar in
	/// the view.
	///
	/// parameters:
	///		nSBCode.....part of the scrollbar clicked
	///		nPos........Position of the scrollbar
	///		pScrollBar..pointer to the scrollbar object
	///------------------------------------------------
	int Delta = 0;
	int Update = 1;

	switch (nSBCode)
	{
		case SB_LINEUP:
			Delta = -1;
			break;
		case SB_PAGEUP:
			Delta = -50;
			break;
		case SB_THUMBTRACK:
			Delta = nPos - m_VScrollPos;
			break;
		case SB_LINEDOWN:
			Delta = 1;
			break;
		case SB_PAGEDOWN:
			Delta = 50;
			break;
		case SB_ENDSCROLL:
			Update = 0;
			break;
	}
	if (Delta && Update)
	{
		int ScrollPos = m_VScrollPos + Delta;
		int MaxPos = m_DocSize.cy - m_VPageSize;
		if (ScrollPos < 0)
			Delta = -m_VScrollPos;
		else if (ScrollPos > MaxPos)
			Delta = MaxPos - m_VScrollPos;
		m_VScrollPos += Delta;
		SetScrollPos(SB_VERT, m_VScrollPos, TRUE);
	}
	if(Update)UpdateRulers(RW_VSCROLL,CPoint(m_HScrollPos, m_VScrollPos),CPoint());
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
	Invalidate(false);
}


void CAutoScrollView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	///------------------------------------------------
	/// OnHScroll
	///		This is the function that is called when
	/// the user presses the horizonalscroll bar in
	/// the view.
	///
	/// parameters:
	///		nSBCode.....part of the scrollbar clicked
	///		nPos........Position of the scrollbar
	///		pScrollBar..pointer to the scrollbar object
	///------------------------------------------------
	// TODO: Add your message handler code here and/or call default
	int Delta = 0;
	int Update = 1;


	switch (nSBCode)
	{
		case SB_LINELEFT:
			Delta = -1;
			break;
		case SB_PAGELEFT:
			Delta = -50;
			break;
		case SB_THUMBTRACK:
			Delta = nPos - m_HScrollPos;
			break;
		case SB_LINERIGHT:
			Delta = 1;
			break;
		case SB_PAGERIGHT:
			Delta = 50;
			break;
		case SB_ENDSCROLL:
			Update = 0;
			break;
	}
	if (Delta && Update)
	{
		int ScrollPos = m_HScrollPos + Delta;
		int MaxPos = m_DocSize.cx - m_HPageSize;
		if (ScrollPos < 0)
			ScrollPos = 0;
		else if (ScrollPos > m_HScrollInfo.nMax)
			ScrollPos = m_HScrollInfo.nMax;
		m_HScrollPos = ScrollPos;
		SetScrollPos(SB_HORZ, m_HScrollPos, TRUE);
	}

	if(Update) UpdateRulers(RW_HSCROLL, CPoint(m_HScrollPos, m_VScrollPos), CPoint());
	CView::OnHScroll(nSBCode, nPos, pScrollBar);
	Invalidate(false);
}


void CAutoScrollView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	ShowScrollBar(SB_BOTH, TRUE);
	UpdateScrollbarInfo();
	EnableScrollBarCtrl(SB_VERT, TRUE);
	EnableScrollBarCtrl(SB_HORZ, TRUE);
	m_pParent = (CFrontCadChildFrame *)GetParentFrame();
	m_pParent->ShowRulers(true);
	UpdateRulers(RW_ZOOM, CPoint(0, 0), CPoint(0, 0));
}


void CAutoScrollView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class
//	UpdateRulerInfo(RW_POSITION, CPoint(m_HScrollPos, m_VScrollPos));
	Invalidate();
}


CPoint CAutoScrollView::CorrectMousePosition(CPoint point)
{
	point += CSize(m_HScrollPos, m_VScrollPos);
	return point;
}


void CAutoScrollView::UpdateScrollbarInfo()
{
	CRect clientrect;
	GetClientRect(&clientrect);
	int cx = clientrect.Width();
	int cy = clientrect.Height();
	int HScrollMax = 0;
	m_HPageSize = 0;
	if (cx < (m_DocSize.cx / ZoomScaleDiv[m_ZoomFactor])) {
		HScrollMax = (m_DocSize.cx / ZoomScaleDiv[m_ZoomFactor])  - 1;
		m_HPageSize = cx;
		m_HScrollPos = min(m_HScrollPos, (m_DocSize.cx / ZoomScaleDiv[m_ZoomFactor]) - m_HPageSize - 1);
	}

	m_HScrollInfo.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	m_HScrollInfo.nMin = 0;
	m_HScrollInfo.nMax = HScrollMax;
	m_HScrollInfo.nPos = m_HScrollPos;
	m_HScrollInfo.nPage = m_HPageSize;
	SetScrollInfo(SB_HORZ, &m_HScrollInfo, TRUE);
	int VScrollMax = 0;
	m_VPageSize = 0;

	if (cy < (m_DocSize.cy / ZoomScaleDiv[m_ZoomFactor])) {
		VScrollMax = (m_DocSize.cy / ZoomScaleDiv[m_ZoomFactor]) - 1;
		m_VPageSize = cy;
		m_VScrollPos = min(m_VScrollPos, (m_DocSize.cy / ZoomScaleDiv[m_ZoomFactor]) - m_VPageSize - 1);
	}
	m_VScrollInfo.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	m_VScrollInfo.nMin = 0;
	m_VScrollInfo.nMax = VScrollMax;
	m_VScrollInfo.nPos = m_VScrollPos;
	m_VScrollInfo.nPage = m_VPageSize;

	SetScrollInfo(SB_VERT, &m_VScrollInfo, TRUE);
}


void CAutoScrollView::OnAutoScroll(CPoint /*point*/, BOOL /*bBeforeScroll*/)
{
}


void CAutoScrollView::AutoScroll(int nRefMessage)
{
	MSG   msg;				// dummmy message structure to process incoming
							// messages while autoscrolling.
	CPoint 	ptDevScrollPos,	// Current scroll position - device units
		ptCursorPos;		// Current mouse cursor position
	CRect  rc; 				// Client area
	int   dx, dy;			// Scrolling increment
	CSize   sizeTotal,      // CView scroll data
		sizePage,
		sizeLine;
	int    nMapMode,       // Mapping mode
		xMin, xMax,
		yMin, yMax;   // Scroll range
	if (!m_AutoScroll) return;
	if (Block) return;
	++Block;

	msg.message = 0;   // forces at least one loop.
	SetCapture();
	GetDeviceScrollSizes(nMapMode, sizeTotal, sizePage, sizeLine);

	// We keep track of the scroll range because CView::ScrollToPosition always
	// try to scroll even if the scroll limit has been reached. This results in screen
	// flickering when ScrollWindow is called.
	GetScrollRange(SB_HORZ, &xMin, &xMax);
	GetScrollRange(SB_VERT, &yMin, &yMax);

	// Process all messages until the relevant mouse button
	// has been released. nRefMessage depends on which button
	// has been used to trigger autoscrolling.
	//   while (msg.message != nRefMessage)
	bool bSawRefMessage = false;

	while (!bSawRefMessage && m_AutoScroll)
	{
		// Process incoming messages until autoscroll button is released

		ptDevScrollPos = CPoint(m_HScrollPos,m_VScrollPos);
		GetCursorPos(&ptCursorPos);
		ScreenToClient(&ptCursorPos);
		GetClientRect(&rc);
		dx = 0;
		dy = 0;

		if ((ptCursorPos.y < 0) && (ptDevScrollPos.y != yMin))
			// Cursor is above client area
			dy = min(-sizeLine.cy, max(-sizePage.cy, (ptCursorPos.y / 10) * sizeLine.cy));
		else if ((ptCursorPos.y > rc.bottom) && (ptDevScrollPos.y != yMax))
			// Cursor is below client area
			dy = max(sizeLine.cy, min(sizePage.cy, ((ptCursorPos.y - rc.bottom) / 10) * sizeLine.cy));
		// otherwise we can't scroll anyway

		if ((ptCursorPos.x < 0) && (ptDevScrollPos.x != xMin))
			// Cursor is on the left of the client area
			dx = min(-sizeLine.cx, max(-sizePage.cx, (ptCursorPos.x / 10) * sizeLine.cx));
		else if ((ptCursorPos.x > rc.right) && (ptDevScrollPos.x != xMax))
			// Cursor is on the right of the client area
			dx = max(sizeLine.cx, min(sizePage.cx, ((ptCursorPos.x - rc.right) / 10) * sizeLine.cx));
		// otherwise we can't scroll anyway

		// if mouse cursor is dragging outside the client area, scrolling occurs
		if ((dx != 0) || (dy != 0))
		{

//-------------------------------------------------------
			int MaxH,MaxV,PosH,PosV;
			MaxV = m_VScrollInfo.nMax - m_VScrollInfo.nPage;
			MaxH = m_HScrollInfo.nMax - m_HScrollInfo.nPage;
			PosH = m_HScrollPos + dx;
			PosV = m_VScrollPos + dy;
			if (PosH < 0)
				m_HScrollPos = 0;
			else if (PosH > MaxH)
				m_HScrollPos = MaxH;
			else
				m_HScrollPos = PosH;
			if (PosV < 0)
				m_VScrollPos = 0;
			else if (PosV > MaxV)
				m_VScrollPos = MaxV;
			else
				m_VScrollPos = PosV;
//-----------------------------------------------------
			m_IsScrolling = TRUE;
			OnAutoScroll(ptCursorPos, TRUE);
			SetScrollPos(SB_VERT, m_VScrollPos);
			SetScrollPos(SB_HORZ, m_HScrollPos);

			UpdateRulers(RW_VSCROLL, CPoint(m_HScrollPos, m_VScrollPos),CPoint());
			UpdateRulers(RW_HSCROLL, CPoint(m_HScrollPos, m_VScrollPos), CPoint());

			while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))	//message pump 2
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				//            if (msg.message == nRefMessage) break;
				if (msg.message == nRefMessage) 
					bSawRefMessage = TRUE;
			}	//end of message pump 2

				//         if (!bSawRefMessage)
			OnAutoScroll(ptCursorPos, FALSE);
		}
		else
		{
			while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))	//message pump #3
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				//if (msg.message == nRefMessage) break;
				if (msg.message == nRefMessage) 
					bSawRefMessage = TRUE;
			}	//end of message pump #3

			m_IsScrolling = FALSE;
		}	//end of if-else
	}	//end of while loop while (!bSawRefMessage && m_AutoScroll)
	ReleaseCapture();
	m_IsScrolling = FALSE;
	--Block;
}


void CAutoScrollView::GetDeviceScrollSizes(int & nMapMode, CSize & sizeTotal, CSize & sizePage, CSize & sizeLine)
{
	//--------------------------------------
	// Get mapping mode
	//--------------------------------------
	SCROLLINFO Vert, Horz;
	CDC *pDC = GetDC();
	nMapMode = pDC->GetMapMode();
	GetScrollInfo(SB_VERT, &Vert);
	GetScrollInfo(SB_HORZ, &Horz);
	sizeTotal = m_DocSize;
	sizePage.cx = Horz.nPage;
	sizePage.cy = Vert.nPage;
	sizeLine = CSize(10, 10);
	ReleaseDC(pDC);
}


void CAutoScrollView::UpdateRulers(int nMessage, CPoint ScrollPos, CPoint Pos)
{
	if (!m_pParent)
		return;
/*
	if (m_ZoomFactor < ZOOM_NORMAL) {
		m_ShowRulers = FALSE;
		m_pParent->ShowRulers(FALSE);
	}
*/
	stRULER_INFO pRulerInfo;
	pRulerInfo.uMessage = nMessage;
	pRulerInfo.ScrollPos = ScrollPos;
	pRulerInfo.Pos = Pos;
	pRulerInfo.DocSize = m_DocSize;
	pRulerInfo.fZoomFactor = ZF[m_ZoomFactor];

	m_pParent->UpdateRulersInfo(pRulerInfo);
}


void CAutoScrollView::OnButtonZoomin()
{
	m_ZoomFactor--;
	if (m_ZoomFactor < 0) m_ZoomFactor = 0;
	m_HScrollPos = (int)(m_HScrollPos * ScaleFactor[m_ZoomFactor]);
	m_VScrollPos = (int)(m_VScrollPos * ScaleFactor[m_ZoomFactor]);
	UpdateScrollbarInfo();
	UpdateRulers(RW_ZOOM, CPoint(m_HScrollPos, m_VScrollPos), CPoint(0, 0));
	Invalidate();
}


void CAutoScrollView::OnUpdateButtonZoomin(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}


void CAutoScrollView::OnButtonZoomout()
{
	// TODO: Add your command handler code here
	m_HScrollPos = (int)(m_HScrollPos / ScaleFactor[m_ZoomFactor]);
	m_VScrollPos = (int)(m_VScrollPos / ScaleFactor[m_ZoomFactor]);
	m_ZoomFactor++;
	if (m_ZoomFactor > MAX_ZOOM) m_ZoomFactor = MAX_ZOOM;
	UpdateScrollbarInfo();
	UpdateRulers(RW_ZOOM, CPoint(m_HScrollPos, m_VScrollPos), CPoint(0, 0));
	Invalidate();
}


void CAutoScrollView::OnUpdateButtonZoomout(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}

CPoint CAutoScrollView::Snap(CPoint nP,int SnapUnits)
{
	//-----------------------------------------------------
	// Snap
	// sets the position to nearest snap grid position
	//
	// Parameters:
	//	nP........Current corrected cursor position
	//	SnapUnits.Number of units for the snap grid
	//
	// Returns:
	//	Cursor position in real units for the display
	//-----------------------------------------------------
	int x, y;
	int HalfSnap;
	int off;

	x = nP.x;
	y =nP.y;
	if (!m_SnapOff)
	{
		HalfSnap = SnapUnits / 2;
		off = x % SnapUnits;
		if (off > HalfSnap)
			x += SnapUnits - off;
		else
			x -= off;

		off = y % SnapUnits;
		if (off > HalfSnap)
			y += SnapUnits - off;
		else
			y -= off;
	}
	return CPoint(x, y);
}

CPoint CAutoScrollView::SnapToScreen(CPoint npSnap)
{
	//------------------------------------------------------
	// SnapToScreen
	//	This function is used to turn the actual coordinates
	// of a point into points on the screen.
	//
	// parameters:
	//	nSnap.......point that was calculated by the function Snap
	//
	//	Returns
	//		A scalled point
	//---------------------------------------------------------
	int x, y;

	x = int(npSnap.x / ZoomScaleDiv[m_ZoomFactor]);
	y = int(npSnap.y / ZoomScaleDiv[m_ZoomFactor]);
	return CPoint(x, y);
}


void CAutoScrollView::UpdateMousePositions(CPoint point)
{
	//---------------------------------------------
	// First, we need to correct the mouse position
	// for the fact that our virtual work surface
	// is much larger that the screen
	//--------------------------------------------
	m_CurMousePos = CorrectMousePosition(point);
	//--------------------------------------------
	// Next, we need to scale the mouse position
	// so that 1000 units equals one inch
	// This corrects for the screen Zoom Level
	//-------------------------------------------
	m_ScalePos = ScaleMouse(m_CurMousePos);
	//------------------------------------------
	// And finally, we need to snap the position
	// to the snap grid
	//------------------------------------------
	m_SnapPos = Snap(m_ScalePos, m_SnapGrid);
}

CPoint CAutoScrollView::ScaleMouse(CPoint mP)
{
	int x, y;

	x = int(mP.x * ZoomScaleDiv[m_ZoomFactor]);
	y = int(mP.y * ZoomScaleDiv[m_ZoomFactor]);
	return CPoint(x, y);
}



void CAutoScrollView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	UpdateScrollbarInfo();
}


afx_msg LRESULT CAutoScrollView::OnStopautoscroll(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

CCadObject * CAutoScrollView::GetTopSelection()
{
	///-------------------------------------------
	///	GetTopSelection
	///		Returns the object that is at the
	/// top of the Selected Objects List
	///-------------------------------------------
	return m_pSelObjList;
}

void CAutoScrollView::AddToSelList(CCadObject *pO)
{
	///----------------------------------------
	///	AddToSelList
	///		This function adds an object to the
	/// selection list.
	///
	///	parameter:
	///		pO.....Object to add to list
	///---------------------------------------
	if (this->m_pSelObjList)
	{
		pO->SetNextSel(m_pSelObjList);
		m_pSelObjList = pO;
	}
	else
	{
		m_pSelObjList = pO;
		m_pSelObjEnd = pO;
	}
	m_SelectionCount++;

}

void CAutoScrollView::RemoveUnselected()
{
	///-------------------------------------
	/// RemoveUnselected
	///remove all unselected objects
	///--------------------------------------
	CCadObject *pO, *pOr, *pPrev = 0;

	pO = m_pSelObjList;
	while (pO)
	{
		if (!pO->IsSelected())
		{
			m_SelectionCount--;

			pOr = pO;
			if (!pPrev)
			{
				m_pSelObjList = pO->GetNextSel();
				pO = m_pSelObjList;
				pOr->SetNextSel(0);
			}
			else
			{
				//pPrev will stay the same
				pO = pO->GetNextSel();
				pPrev->SetNextSel(pOr->GetNextSel());
				pOr->SetNextSel(0);
			}
		}
		else
		{
			pPrev = pO;
			pO = pO->GetNextSel();
		}

	}
	if (m_pSelObjList == 0) m_pSelObjEnd = 0;
}

void CAutoScrollView::RemoveObject(CCadObject *pO)
{
	///----------------------------------------------
	///	RemoveObject
	/// This fuction removes the specified object
	/// from the selected list
	///		parameters:
	///			pO.....pointer to object to remove
	///----------------------------------------------
	CCadObject *pL, *pPrev = 0;
	int loop = 1;

	pL = m_pSelObjList;
	while (pL && loop)
	{
		if (pL == pO)
		{
			m_SelectionCount--;
			loop = 0;
			if (pPrev == 0)
			{
				m_pSelObjList = pL->GetNextSel();
				pL->SetNextSel(0);
			}
			else
			{
				pPrev->SetNextSel(pL->GetNextSel());
				pO->SetNextSel(0);
			}
		}
		else
		{
			pPrev = pL;
			pL = pL->GetNextSel();
		}

	}
	if (m_pSelObjList == 0) m_pSelObjEnd = 0;
}

void CAutoScrollView::ClearSelList()
{
	CCadObject *pO = GetTopSelection();
	CCadObject *next;
	while (pO)
	{
		next = pO->GetNextSel();
		pO->SetSelected(0);
		RemoveObject(pO);
		pO = next;
	}
	m_SelectionCount = 0;
}

int CAutoScrollView::SelectAnObject(CCadObject **ppObj, int n, CPoint p)
{
	int i,Id;
	char *s = new char[256];
	CPoint point = p;
	ClientToScreen(&point);
	CMenu ConTexMenu;
	ConTexMenu.CreatePopupMenu();
	for (i = 0; i < n; ++i)
	{
		sprintf_s(s, 256, "%s (%d,%d)", ppObj[i]->GetTypeString(), ppObj[i]->m_P1.x, ppObj[i]->m_P1.y);
		ConTexMenu.AppendMenu(MF_STRING, 3000+i, s);
		ConTexMenu.EnableMenuItem(3000 + i, MF_ENABLED);;
	}
	Id = ConTexMenu.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
	ConTexMenu.DestroyMenu();
	delete[] s;
	if (Id >= 3000) Id -= 3000;
	else Id = -1;
	return Id;
}


void CAutoScrollView::OnClose()
{
	EnableAutoScroll(0);
	if (Block)
		Sleep(1);
	CView::OnClose();
}


void CAutoScrollView::OnDestroy()
{
	EnableAutoScroll(0);
	PostMessage(WM_STOPAUTOSCROLL);
//	while (Block)
//		Sleep(1);
	CView::OnDestroy();

	// TODO: Add your message handler code here
}

void CAutoScrollView::DrawCursor(CDC * pDC, CPoint pos, CScale Scale, CSize Offset,CRect *pRect,COLORREF color)
{
	CPen pen, *pOldPen;
	pen.CreatePen(PS_SOLID, 1, color);
	pOldPen = pDC->SelectObject(&pen);
	int w, h;
	pos -= CPoint(m_HScrollPos, m_VScrollPos);
	w = pRect->Width();
	h = pRect->Height();
	pDC->MoveTo(0, pos.y);
	pDC->LineTo(w, pos.y);
	pDC->MoveTo(pos.x, 0);
	pDC->LineTo(pos.x, h);
	pDC->SelectObject(pOldPen);
}

void CAutoScrollView::DrawGrid(CDC * pDC, int Xspacing, int Yspacing, COLORREF color,int mode, CRect rect)
{
	int i;
	if (this->m_GridOn)
	{
		double gd = m_SnapGrid * ZF[m_ZoomFactor];
		CPen pen, *oldpen;
		pen.CreatePen(PS_SOLID, 1, color);
		oldpen = pDC->SelectObject(&pen);
		int H, W;
		W = rect.Width();
		H = rect.Height();
		int N = int(W / gd) + 1;
		int offset = m_HScrollPos % int(gd);
		for (int i = 0; i<N; ++i)
		{
			pDC->MoveTo(CPoint(int((i + 1)*gd) - offset, 0));
			pDC->LineTo(CPoint(int((i + 1)*gd) - offset, H - 1));
		}
		N = int(H / gd) + 1;
		offset = m_VScrollPos % int(gd);

		for (i = 0; i<N; ++i)
		{
			pDC->MoveTo(CPoint(0, int((i + 1) * gd) - offset));
			pDC->LineTo(CPoint(W - 1, int((i + 1) * gd) - offset));
		}
		pDC->SelectObject(oldpen);
	}
}
