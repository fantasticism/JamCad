#pragma once

#include "AutoScrollView.h"

// CJamCadSchLibPartView view

class CJamCadSchLibPartView : public CAutoScrollView
{
	DECLARE_DYNCREATE(CJamCadSchLibPartView)
	int m_DrawMode;
	int m_DrawState;
	CDC *m_pMemDC;
	CBitmap *m_pDCbm;
	CCadObject *m_pGrabbedObject;
	CStatusBarCtrl *p_Status;
	CMoveObjects *m_pMoveObjects;
	int m_Unit;
	int m_ControlKeyDown;
	int m_ShiftKeyDown;
protected:
	CJamCadSchLibPartView();           // protected constructor used by dynamic creation
	virtual ~CJamCadSchLibPartView();
public:
	virtual void InsertObjectIntoDoc(CCadObject *pObj);
	virtual void AddObjectIntoDoc(CCadObject *pObj);
public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnInitialUpdate();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnPlaceArc();
	afx_msg void OnUpdatePlaceArc(CCmdUI *pCmdUI);
	afx_msg void OnPlaceArccenter();
	afx_msg void OnUpdatePlaceArccenter(CCmdUI *pCmdUI);
	afx_msg void OnPlaceBitmap();
	afx_msg void OnUpdatePlaceBitmap(CCmdUI *pCmdUI);
	afx_msg void OnPlaceEllispe();
	afx_msg void OnUpdatePlaceEllispe(CCmdUI *pCmdUI);
	afx_msg void OnPlaceLine();
	afx_msg void OnUpdatePlaceLine(CCmdUI *pCmdUI);
	afx_msg void OnPlacePin();
	afx_msg void OnUpdatePlacePin(CCmdUI *pCmdUI);
	afx_msg void OnPlacePolygon();
	afx_msg void OnUpdatePlacePolygon(CCmdUI *pCmdUI);
	afx_msg void OnPlacePolygonfilled();
	afx_msg void OnUpdatePlacePolygonfilled(CCmdUI *pCmdUI);
	afx_msg void OnPlaceRect();
	afx_msg void OnUpdatePlaceRect(CCmdUI *pCmdUI);
	afx_msg void OnPlaceReferencePoint();
	afx_msg void OnUpdatePlaceReferencePoint(CCmdUI *pCmdUI);
	afx_msg void OnPlaceRoundedrect();
	afx_msg void OnUpdatePlaceRoundedrect(CCmdUI *pCmdUI);
	afx_msg void OnPlaceText();
	afx_msg void OnUpdatePlaceText(CCmdUI *pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPlaceArrow();
	void CreateMemDC();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();
};


