#pragma once
//-------------------------------------
// This is the library object for
// the FrontCad documents
//--------------------------------------
#include "CadObject.h"
#include "JamCadDefines.h"

typedef struct {

}LibCompAttrb;

class CCadLibComp:public CCadObject
{
	LibCompAttrb m_Attrb;
public:
	CCadLibComp();
	virtual ~CCadLibComp();
	static void LoadSettings(LibCompAttrb *pAttrb);
	static void SaveSettings(LibCompAttrb *pAttrb);
	virtual void Move(CPoint p);
	virtual void Save(FILE * pO, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CPoint p);
	virtual int GrabVertex(CPoint p);
	virtual void Draw(CDC * pDC, int mode, CPoint Offset = CPoint(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual int CheckSelected(CPoint p, CSize Offest = CSize(0, 0), CCadObject ** ppSelList = 0, int index = 0, int n = 0, int flag = CADDRAWUBG_CHECKSEL_FLAG_ALL);
	virtual CPoint GetReference();
	virtual void AddObject(CCadObject *pO);
	virtual void RemoveObject(CCadObject *pO);
	virtual CCadObject *GetHead(void);
	virtual int IsDirty(void);
	virtual int IsSelected(void);
	virtual void SetDirty(int d);
	virtual void SetSelected(int Flag = 0);
	virtual void AdjustRefernce(CPoint Ref);
	virtual CRect GetRect(void);
	virtual char * GetTypeString(void);
	CCadLibComp operator=(CCadLibComp &v);
	virtual CPoint ScalePoint(CPoint p, int Scale, int ScaleDiv);
	virtual CCadObject * CopyObject(void);
	virtual void SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw);
	virtual void RenderEnable(int e);
	virtual CCadObject * PopHead();
	virtual CCadObject * PopTail();
	virtual CPoint GetCenter();
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
	virtual int Parse(int Token, CLexer *pLex);
	virtual CCadObject *NewInstance(void);
	void CopyAttributes(LibCompAttrb *pAttrb);
	virtual int MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseMove(CAutoScrollView * pASV, int DrawState);
};

