#pragma once
#include "CadObject.h"

typedef struct {
	int m_Width;
	COLORREF m_Color;
	COLORREF m_FillColor;
}EllipseAttributes;

class CCadElispe:public CCadObject
{
	CPen *	m_pPenLine;
	CBrush *m_pBrFill;
	EllipseAttributes m_Attrb;
public:
	static int CCadElispe::m_RenderEnable;
	CCadElispe();
	CCadElispe(CCadElispe &e);
	virtual ~CCadElispe();
	static void LoadSettings(EllipseAttributes *pAttrb);
	static void SaveSettings(EllipseAttributes *pAttrb);
	virtual void Move(CPoint p);
	virtual void Save(FILE * pO, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CPoint p);
	virtual int GrabVertex(CPoint p);
	virtual void Draw(CDC * pDC, int mode, CPoint Offset = CPoint(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual int CheckSelected(CPoint p, CSize Offest = CSize(0, 0), CCadObject ** ppSelList = 0, int index = 0, int n = 0, int flag = CADDRAWUBG_CHECKSEL_FLAG_ALL);
	virtual CPoint GetReference();
	virtual void AdjustRefernce(CPoint Ref);
	virtual CRect GetRect(void);
	virtual char * GetTypeString(void);
	CCadElispe operator=(CCadElispe &v);
	virtual CCadObject * CopyObject(void);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
	virtual int Parse(int Token, CLexer *pLex);
	virtual CCadObject *NewInstance(void);
	void GetAttributes(EllipseAttributes *pAttrb);
	void CopyAttributes(EllipseAttributes *pAttrb);
	virtual int MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseMove(CAutoScrollView * pASV, int DrawState);
	int PtInElipse(CPoint p, CSize Offset = CSize(0,0));
};

