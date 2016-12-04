#pragma once
//----------------------------------------------------
// JamCad the free EDA package
//----------------------------------------------------
#include "CadObject.h"

typedef struct {
	int m_Width;
	COLORREF m_Color;
	CPoint m_Start;
	CPoint m_End;
	int m_StartAngle;
	int m_EndAngle;
}ArcAttributes;


class CCadArc :public CCadObject
{
	static int m_RenderEnable;
	ArcAttributes m_Attrb;
	CPen *m_pPenLine;
public:
	CCadArc();
	CCadArc(CCadArc &arc);
	virtual ~CCadArc();
	static void LoadSettings(ArcAttributes *pAttrb);
	static void SaveSettings(ArcAttributes *pAttrb);
	virtual void Move(CPoint p);
	virtual void Save(FILE * pO, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CPoint p);
	virtual int GrabVertex(CPoint p, CScale scale);
	virtual void Draw(CDC * pDC, int mode, CPoint Offset = CPoint(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual int CheckSelected(CPoint p, CSize Offest = CSize(0, 0), CCadObject ** ppSelList = 0, int index = 0, int n = 0, int flag = CADDRAWUBG_CHECKSEL_FLAG_ALL);
	virtual CPoint GetReference();
	virtual void AdjustRefernce(CPoint Ref);
	virtual CRect GetRect(void);
	virtual char * GetTypeString(void);
	CCadArc operator=(CCadArc &v);
	virtual CCadObject * CopyObject(void);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
	virtual int Parse(int Token, CLexer *pLex);
	virtual CCadObject *NewInstance(void);
	void GetAttributes(ArcAttributes *pAttrb);
	void CopyAttributes(ArcAttributes *pAttrb);
	virtual int MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseMove(CAutoScrollView * pASV, int DrawState);
	virtual int EditProperties(void);
	void SetStartPoint(CPoint p);
	void SetEndPoint(CPoint p);
	void SetEndAngle(int Angle);
	void SetStartAngle(int Angle);
	inline int GetWidth(void) { return m_Attrb.m_Width; }
	int PtInArc(CPoint p, CSize Offset=CSize(0,0));
};

