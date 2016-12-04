#pragma once
#include "CadObject.h"

typedef struct
{
	int m_Width;
	COLORREF m_Color;
}PolyAttributes;

class CCadPolygon :	public CCadObject
{
	PolyAttributes m_Attrb;
	CPen *m_pPenLine;
	int m_MaxY;
	int m_MinY;
	int m_MaxX;
	int m_MinX;
	int m_PolyID;
	CPoint * m_pVertex;
	int m_size;
	int m_Count;
	int m_NextPoint;
public:
	static int m_RenderEnable;
	static void LoadSettings(PolyAttributes *pAttrib);
	static void SaveSettings(PolyAttributes *pAttrib);
	CCadPolygon();
	CCadPolygon(int nVertex);
	CCadPolygon(CCadPolygon &v);
	virtual ~CCadPolygon();
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
	CCadPolygon operator=(CCadPolygon &v);
	virtual CCadObject * CopyObject(void);
	virtual void SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
	virtual int Parse(int Token, CLexer *pLex);
	virtual CCadObject *NewInstance(void);
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
	virtual int MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseMove(CAutoScrollView * pASV, int DrawState);
	virtual void Rotate(CPoint Origin, int Rotation);
	virtual void Flip(CPoint Origin, int Direction);
	virtual int EditProperties();
	inline PolyAttributes *GetAttributes(void) { return &m_Attrb; }
	inline void SetCount(int cnt) { m_Count = cnt; }
	inline void SetPolySize(int sz) { m_size = sz; }
	inline int GetPolySize(void) { return m_size; }
	void CopyAttributes(PolyAttributes *pPA);
	void GetAttributes(PolyAttributes *pA);
	inline void SetCurrentVertex(CPoint p) { m_pVertex[m_Count] = p; }
	inline void SetNextVertex(CPoint p) { m_pVertex[m_Count + 1] = p; }
};

