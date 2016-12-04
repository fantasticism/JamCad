#pragma once
#include "CadObject.h"
#include "JamCadDefines.h"

typedef struct {
	int m_Width;
	CSize m_CornerRadius;
	COLORREF m_Color;
	COLORREF m_FillColor;
}RoundRectAttributes;

class CCadRndRect:public CCadObject
{
	static int m_RenderEnable;
	RoundRectAttributes m_Attrb;
	CPen *m_pPenLine;
	CBrush *m_pBrush;
public:
	CCadRndRect();
	CCadRndRect(CCadRndRect &r);
	virtual ~CCadRndRect();
	static void LoadSettings(RoundRectAttributes *pAttrb);
	static void SaveSettings(RoundRectAttributes *pAttrb);
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
	CCadRndRect operator=(CCadRndRect &v);
	virtual CCadObject * CopyObject(void);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
	virtual int Parse(int Token, CLexer *pLex);
	virtual CCadObject *NewInstance(void);
	void GetAttributes(RoundRectAttributes *pAttrb);
	void CopyAttributes(RoundRectAttributes *pAttrb);
	virtual int MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseMove(CAutoScrollView * pASV, int DrawState);
	virtual int EditProperties();
};

