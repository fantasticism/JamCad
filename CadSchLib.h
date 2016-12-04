#pragma once
#include "CadObject.h"

struct CadSchLibAttr {
	int dummy;
};

class CCadSchLib:public CCadObject
{
	CPropString *m_pNameProp;
	CPropInt *m_pNumUnits;
public:
	inline CPropString *GetNameProp() { return m_pNameProp; }
	CCadSchLib();
	void Create();
	virtual ~CCadSchLib();
	static void LoadSettings(CadSchLibAttr *pAttrb);
	static void SaveSettings(CadSchLibAttr *pAttrb);
	virtual void Move(CPoint p);
	virtual void Save(FILE * pO, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CPoint p);
	virtual int GrabVertex(CPoint p);
	virtual void Draw(CDC * pDC, int mode, CPoint Offset = CPoint(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual int CheckSelected(CPoint p, CSize Offest = CSize(0, 0), CCadObject ** ppSelList = 0, int index = 0, int n = 0, int flag = CADDRAWUBG_CHECKSEL_FLAG_ALL);
	virtual CPoint GetReference();
	virtual int IsSelected(void);
	virtual void SetDirty(int d);
	virtual void SetSelected(int Flag = 0);
	virtual void AdjustRefernce(CPoint Ref);
	virtual CRect GetRect(void);
	virtual char * GetTypeString(void);
	CCadSchLib operator=(CCadSchLib &v);
	virtual CPoint ScalePoint(CPoint p, int Scale, int ScaleDiv);
	virtual CCadObject * CopyObject(void);
	virtual void SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
	virtual int Parse(int Token, CLexer *pLex);
	virtual CCadObject *NewInstance(void);
	void GetAttributes(CadSchLibAttr *pAttrb);
	void CopyAttributes(CadSchLibAttr *pAttrb);
	virtual int MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseUp(CAutoScrollView * pASV, int DrawState);
	virtual int MouseMove(CAutoScrollView * pASV, int DrawState);
	virtual CCadObject *FindTreeItem(HTREEITEM Item);
};

extern void MakeSchLibProperties(CProp *pP);
