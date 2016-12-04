#pragma once
#include "CadObject.h"
#include "AutoScrollView.h"
#include "MyBitmap.h"

typedef struct {
	char *m_pBMFileName;
	int m_MaintainAspectRatio;
	CSize m_Size;
}BitMapAttribute;

class CCadBitmap :public CCadObject
{
	static int m_RenderEnable;
	BitMapAttribute m_Attrb;
	CMyBitmap *m_pBM;
public:
	CCadBitmap();
	CCadBitmap(CCadBitmap &v);
	virtual ~CCadBitmap();
	static void LoadSettings(BitMapAttribute *pAttrb);
	static void SaveSettings(BitMapAttribute*pAttrb);
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
	CCadBitmap operator=(CCadBitmap &v);
	virtual CCadObject * CopyObject(void);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
	virtual int Parse(int Token, CLexer *pLex);
	virtual CCadObject *NewInstance(void);
	void GetAttributes(BitMapAttribute *pAttrb);
	void CopyAttributes(BitMapAttribute *pAttrb);
	virtual int MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseMove(CAutoScrollView * pASV, int DrawState);
	void RestoreAspectRatio();
	void LoadImage(char *path);
	int PtInBitmap(CPoint p, CSize Offset = CSize(0, 0));
};

