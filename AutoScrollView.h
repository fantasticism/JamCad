#pragma once

#include "FrontCadChildFrame.h"

// CAutoScrollView view

class CAutoScrollView : public CView
{
	DECLARE_DYNCREATE(CAutoScrollView)
private:	//Attributes
	SCROLLINFO m_HScrollInfo;	//Horizontal Scroll Info
	SCROLLINFO m_VScrollInfo;	//Vertical Scroll Info
	int m_HPageSize;	//horizontal page size
	CSize m_DocSize;	//document size
	int m_VPageSize;	//vertical page size
	int m_ShowRulers;	//flag to show rullers
	int m_ZoomFactor;	//screen magnification factor
	int m_HScrollPos;	//position of horizontal scroll bar
	int m_AutoScroll;	//enables auto scrolling flag
	int m_IsScrolling;	//Indicates that the view is auto scrolling
	int m_VScrollPos;	//position of virtical scroll bar
	int m_SnapGrid;
	int m_SnapOff;
	int m_MouseDown;
	int Block;		//blocks reentrance to AutoScroll method
public:
	inline void DisableSnapGrid(int e = 1) { m_SnapOff = e; }
	inline int GetSnapGrid(void) { return m_SnapGrid; }
	inline void SetSnapGrid(int g) { m_SnapGrid = g; }
	inline CSize GetDocSize(void) { return m_DocSize; }
	inline void SetDocSize(CSize sZ) { m_DocSize = sZ; }
	inline int IsScrolling() { return m_IsScrolling; }
	inline void EnableAutoScroll(int flag) { m_AutoScroll = flag; }
	inline CSize GetScrollOffset() {return CSize(m_HScrollPos, m_VScrollPos);}
	inline int IsMouseDown(void) { return m_MouseDown; }
	CScale GetScale(void);
public:
	CADObjectTypes m_CadObj;
	CFrontCadChildFrame *m_pParent;
	CPoint m_CurMousePos;	//mouse position raw
	CPoint m_ScalePos;		//mouse position scaled
	CPoint m_SnapPos;		//mouse position snapped to snap grid
	int m_SelectionCount;
	int m_GridOn;			//enables the grid on the scrren
	CCadObject *m_pSelObjList;
	CCadObject *m_pSelObjEnd;
protected:
	CAutoScrollView();           // protected constructor used by dynamic creation
	virtual ~CAutoScrollView();

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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	CPoint CorrectMousePosition(CPoint point);
	void UpdateScrollbarInfo();
	virtual void OnAutoScroll(CPoint point, BOOL bBeforeScroll);
	void AutoScroll(int nMessage);
	void GetDeviceScrollSizes(int & nMapMode, CSize & sizeTotal, CSize & sizePage, CSize & sizeLine);
	void UpdateRulers(int nMessage, CPoint ScrollPos, CPoint Pos);
	afx_msg void OnButtonZoomin();
	afx_msg void OnUpdateButtonZoomin(CCmdUI *pCmdUI);
	afx_msg void OnButtonZoomout();
	afx_msg void OnUpdateButtonZoomout(CCmdUI *pCmdUI);
	virtual CPoint Snap(CPoint pos,int SnapUnits);
	virtual CPoint SnapToScreen(CPoint npSnap);
	virtual void UpdateMousePositions(CPoint p);
	virtual void InsertObjectIntoDoc(CCadObject *pObj) {};
	virtual void AddObjectIntoDoc(CCadObject *pObj) {};
	CPoint ScaleMouse(CPoint mP);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	afx_msg LRESULT OnStopautoscroll(WPARAM wParam, LPARAM lParam);
public:
	CCadObject *GetTopSelection();
	void AddToSelList(CCadObject *pO);
	void RemoveUnselected();
	void RemoveObject(CCadObject *pO);
	void ClearSelList();
	int SelectAnObject(CCadObject **ppObj, int n, CPoint p);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	void DrawCursor(CDC * pDC, CPoint p, CScale Scale, CSize Offset, CRect *pRect, COLORREF color);
	void DrawGrid(CDC * pDC, int Xspacing, int Yspacing, COLORREF color, int mode, CRect rect);
};


