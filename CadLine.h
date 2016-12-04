#pragma once
#include "CadObject.h"
#include "CadPolygon.h"

//line attributes structure
typedef struct {
	int m_Width;
	COLORREF m_Color;
}LineAttr;

class CCadLine :public CCadObject
{
	LineAttr m_attrib;
	CCadPolygon *m_pPoly;
	CPen *m_pLinePen;
	void EncliseLineInPolygon(CSize Offset,int w);
public:	//public data
	static int m_RenderEnable;
	static void LoadSettings(LineAttr *pAttrib);
	static void SaveSettings(LineAttr *pAttrib);
public: //attribute functions
	inline void SetWidth(int w) { m_attrib.m_Width = w; }
	inline int GetWidth(void) { return m_attrib.m_Width; }
	inline void SetColor(COLORREF c) { m_attrib.m_Color = c; }
	inline COLORREF GetColor(void) { return m_attrib.m_Color; }
	void GetAttributes(LineAttr *pAtr);
	void CopyAttributes(LineAttr *pAtr);
public://public methodes
	CCadLine();
	CCadLine(CCadLine &line);
	virtual ~CCadLine();
	virtual void Move(CPoint p);
	virtual void Save(FILE * pO, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CPoint p);
	virtual int GrabVertex(CPoint p);
	virtual void Draw(CDC * pDC, int mode, CPoint Offset = CPoint(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual int CheckSelected(CPoint p, CSize Offest = CSize(0, 0), CCadObject ** ppSelList = 0, int index = 0, int n = 0, int flag = CADDRAWUBG_CHECKSEL_FLAG_ALL);
	virtual void AdjustRefernce(CPoint Ref);
	virtual char * GetTypeString(void);
	CCadLine operator=(CCadLine &v);
	virtual CCadObject * CopyObject(void);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
	virtual int Parse(int Token, CLexer *pLex);
	virtual CCadObject *NewInstance(void);
	virtual int MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseMove(CAutoScrollView * pASV, int DrawState);
	virtual int EditProperties(void);
};

