#pragma once

#include "FrontCadChildFrame.h"
#include "AutoScrollView.h"

// CFrontCadView view

class CFrontCadView : public CAutoScrollView
{
	DECLARE_DYNCREATE(CFrontCadView)
protected:
	CFrontCadView();           // protected constructor used by dynamic creation
	virtual ~CFrontCadView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:	//Attributes

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	virtual void OnAutoScroll(CPoint point, BOOL bBeforeScroll);
	afx_msg void OnPlaceArc();
	afx_msg void OnUpdatePlaceArc(CCmdUI *pCmdUI);
	afx_msg void OnPlaceArccenter();
	afx_msg void OnUpdatePlaceArccenter(CCmdUI *pCmdUI);
	afx_msg void OnPlaceBitmap();
	afx_msg void OnUpdatePlaceBitmap(CCmdUI *pCmdUI);
	afx_msg void OnPlaceDimension();
	afx_msg void OnUpdatePlaceDimension(CCmdUI *pCmdUI);
	afx_msg void OnPlaceEllispe();
	afx_msg void OnUpdatePlaceEllispe(CCmdUI *pCmdUI);
	afx_msg void OnPlaceHole1flat();
	afx_msg void OnUpdatePlaceHole1flat(CCmdUI *pCmdUI);
	afx_msg void OnPlaceHoleround();
	afx_msg void OnUpdatePlaceHoleround(CCmdUI *pCmdUI);
	afx_msg void OnPlaceLibpart();
	afx_msg void OnUpdatePlaceLibpart(CCmdUI *pCmdUI);
	afx_msg void OnPlaceLine();
	afx_msg void OnUpdatePlaceLine(CCmdUI *pCmdUI);
	afx_msg void OnPlaceOrigin();
	afx_msg void OnUpdatePlaceOrigin(CCmdUI *pCmdUI);
	afx_msg void OnPlacePolygon();
	afx_msg void OnUpdatePlacePolygon(CCmdUI *pCmdUI);
	afx_msg void OnPlacePolygonfilled();
	afx_msg void OnUpdatePlacePolygonfilled(CCmdUI *pCmdUI);
	afx_msg void OnPlaceRect();
	afx_msg void OnUpdatePlaceRect(CCmdUI *pCmdUI);
	afx_msg void OnPlaceRectangularhole();
	afx_msg void OnUpdatePlaceRectangularhole(CCmdUI *pCmdUI);
	afx_msg void OnPlaceRoundedrect();
	afx_msg void OnUpdatePlaceRoundedrect(CCmdUI *pCmdUI);
	afx_msg void OnPlaceRoundhole2flat();
	afx_msg void OnUpdatePlaceRoundhole2flat(CCmdUI *pCmdUI);
};


