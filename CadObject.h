#pragma once

#include "Scale.h"
#include "Lexer.h"
#include "Prop.h"
#include "AutoScrollView.h"

//-------------------------------------------
// Defines for Save flags parameter
//-------------------------------------------

#define CADOBJECT_SAVE_ADDCOMMA_ATEND		1

class CCadObject
{
	int m_NeedsAttributes;	//indicates if attributes have been loaded
	int m_Type;		//specifies the type of object
	int m_Dirty;	//flag that indicates object needs to be redrawn
	int m_Selected;	//indicates that object is in the selected state
	int m_LastMode;	//last display mode object was in
	CCadObject *m_pNext;	//next object in drawing
	CCadObject *m_pPrev;	//prefious object in drawing
	CCadObject *m_pNextSel;	//next selected object
	CCadObject *m_pNextInstance;	//next instance of object
	CCadObject **m_ppHead;			//head pointer of child list(s)
	CCadObject **m_ppTail;			//tail pointer of child list(s)
	CProp m_Properties;			// properties of the object
	HTREEITEM m_hTreeItem;
public:
	CPoint m_P1;	//upper left hand corner
	CPoint m_P2;	//lower right hand corner
public:
	inline int GetType(void) { return m_Type; }
	inline bool IsLastModeSame(int m) { return (m == m_LastMode); }
	inline void SetLastMode(int m) { m_LastMode = m; }
	inline int GetLastMode(void) { return m_LastMode; }
	inline CCadObject *GetNext(void) { return m_pNext; }
	inline void SetNext(CCadObject *pO) { m_pNext = pO; }
	inline CCadObject *GetPrev(void) { return m_pPrev; }
	inline void SetPrev(CCadObject *pO) { m_pPrev = pO; }
	inline CCadObject *GetNextSel(void) { return m_pNextSel; }
	inline void SetNextSel(CCadObject *pO) { m_pNextSel = pO; }
	inline CCadObject* GetNextInstance() { return m_pNextInstance; }
	inline int NeedsAttributes(void) {return m_NeedsAttributes;}
	inline void ClearNeedsAttributes(void) { m_NeedsAttributes = 0; }
	inline HTREEITEM GetTreeItem(void) { return m_hTreeItem; }
	inline void SetTreeItem(HTREEITEM tI) { m_hTreeItem = tI; }
	inline CProp &GetProperties(void) { return m_Properties; }
	inline CProp *GetPropertiesPointer() { return &m_Properties; }
	CCadObject();
	CCadObject(int Type,int nUnits=1);
	virtual ~CCadObject();
	virtual void Move(CPoint p);
	virtual void Save(FILE * pO, int Indent=0,int flags=0);
	virtual void SetVertex(int v, CPoint p);
	virtual int GrabVertex(CPoint p,CScale scale);
	virtual void Draw(CDC * pDC, int mode, CPoint Offset=CPoint(0,0), CScale Scale=CScale(0.1,0.1));
	virtual int CheckSelected(CPoint p, CSize Offest = CSize(0, 0), CCadObject ** ppSelList=0, int index =0, int n=0, int flag= CADDRAWUBG_CHECKSEL_FLAG_ALL);
	virtual CPoint GetReference();
	virtual void AddObject(CCadObject *pO,int Unit=0);
	virtual void InsertObject(CCadObject *pO, int Unit = 0);
	virtual void RemoveObject(CCadObject *pO, int Unit = 0);
	virtual CCadObject *GetHead(int Unit=0);
	virtual int IsDirty(void);
	virtual int IsSelected(void);
	virtual void SetDirty(int d);
	virtual void SetSelected(int Flag = 0);
	virtual void AdjustRefernce(CPoint Ref);
	virtual CRect GetRect(void);
	virtual char * GetTypeString(void);
	CCadObject operator=(CCadObject &v);
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
	virtual int Parse( int Token, CLexer *pLex);
	virtual CCadObject *NewInstance(void);
	virtual int MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseMove(CAutoScrollView * pASV, int DrawState);
	virtual CPoint RotatePoint(CPoint Origin, CPoint p, int Rotation);
	virtual void Rotate(CPoint Origin, int Rotation);
	virtual void Flip(CPoint Origin, int Direction);
	virtual CPoint FlipPoint(CPoint Origin, CPoint p, int Direction);
	virtual int EditProperties(void);
	virtual CCadObject *FindTreeItem(HTREEITEM Item);
	virtual void CopyProperties(CProp &Properties);
	virtual void CopyPropertyValues(CProp &Src);
	CRect ScaleRect(CRect rect, CScale scale, CSize Offset);
};

