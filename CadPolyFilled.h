#pragma once
#include "CadObject.h"
#include "JamCadDefines.h"

typedef struct {
	int m_Width;
	COLORREF m_Color;
	COLORREF m_FillColor;
}PolyFilledAttributes;

class CCadPolyFilled:public CCadObject
{
	static int m_RenderEnable;
	PolyFilledAttributes m_Attrb;
	int m_MaxY;
	int m_MinY;
	int m_MaxX;
	int m_MinX;
	int m_PolyID;
	CPoint * m_pVertex;
	CPen *m_pPenLine;
	CBrush *m_pBrFill;
	int m_size;
	int m_Count;
	int m_NextPoint;
public:
	CCadPolyFilled();
	CCadPolyFilled(int nVertex);
	CCadPolyFilled(CCadPolyFilled &v);
	virtual ~CCadPolyFilled();
	static void LoadSettings(PolyFilledAttributes *pAttrb);
	static void SaveSettings(PolyFilledAttributes *pAttrb);
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
	CCadPolyFilled operator=(CCadPolyFilled &v);
	virtual CCadObject * CopyObject(void);
	virtual void SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
	virtual int Parse(int Token, CLexer *pLex);
	virtual CCadObject *NewInstance(void);
	void GetAttributes(PolyFilledAttributes *pA);
	void CopyAttributes(PolyFilledAttributes *pAttrb);
	virtual int MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseMove(CAutoScrollView * pASV, int DrawState);
	virtual int EditProperties();
	//---------------------------------------
	// Other methodes
	//---------------------------------------
	void UpdateMinMax(void);
	void SetCurPoint(CPoint p);
	void Reset(void);
	CPoint * GetPoints(void);
	int CompareToLast(CPoint nP);
	int DeleteLastPoint(void);
	int GetCount(void);
	void AddPoint(CPoint nP);
	BOOL PointEnclosed(CPoint, CPoint Offset = CPoint(0, 0));
	inline PolyFilledAttributes *GetAttributes(void) { return &m_Attrb; }
	inline void SetCount(int cnt) { m_Count = cnt; }
	inline void SetPolySize(int sz) { m_size = sz; }
	inline int GetPolySize(void) { return m_size; }
	inline void SetCurrentVertex(CPoint p) { m_pVertex[m_Count] = p; }
	inline void SetNextVertex(CPoint p) { m_pVertex[m_Count + 1] = p; }
};

