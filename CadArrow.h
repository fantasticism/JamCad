#pragma once
#include "CadObject.h"
#include "JamCadDefines.h"

typedef struct {
	COLORREF m_Color;
	int m_L1;
	int m_L2;
	int m_Width;
}ArrowAttributes;

class CCadArrow :public CCadObject
{
	static int m_RenderEnable;
	ArrowAttributes m_Attrb;
	CPen *m_pPen;
	CBrush *m_pBrFill;
	CCadPolygon *EncloseArrowInPolygon(CSize Offset);
public:
	CCadArrow();
	CCadArrow(CCadArrow &ca);
	virtual ~CCadArrow();
	static void LoadSettings(ArrowAttributes *pAttrb);
	static void SaveSettings(ArrowAttributes *pAttrb);
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
	CCadArrow operator=(CCadArrow &v);
	virtual CCadObject * CopyObject(void);
	virtual void SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
	virtual int Parse(int Token, CLexer *pLex);
	virtual CCadObject *NewInstance(void);
	void GetAttributes(ArrowAttributes *pAttrb);
	void CopyAttributes(ArrowAttributes *pAttrb);
	virtual int MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseMove(CAutoScrollView * pASV, int DrawState);
	void CalcPoints(CPoint & a, CPoint & b, CPoint &c ,CSize Offset, CScale Scale);
};

