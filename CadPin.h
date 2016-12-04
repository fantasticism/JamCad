#pragma once
#include "CadObject.h"
#include "Prop.h"
#include "PropInt.h"
#include "PropSelInt.h"
#include "PropString.h"

enum {
	PIN_STYLE_LINE,
	PIN_STYLE_INVERTED,
	PIN_STYLE_CLOCK,
	PIN_STYLE_CLOCKINV
};

enum {
	PIN_TYPE_INPUT,
	PIN_TYPE_OUTPUT,
	PIN_TYPE_BIDIRECTIONAL,
	PIN_TYPE_PASIVE,
	PIN_TYPE_POWER
};

enum pfOrientation {
	PIN_ORIEN_LEFT,		//0 degrees
	PIN_ORIEN_UP,		//90 degrees
	PIN_ORIEN_RIGHT,	//180 degrees
	PIN_ORIEN_DOWN		//270 degrees
};


typedef struct {
	int m_Width;
	COLORREF m_Color;
}PinAttributes;

class CCadPin :public CCadObject
{
	PinAttributes m_Attr;
	CPen *m_pLine,*m_pSelPen;
	//-------------------------------
	// properties we want fast 
	// access to
	//------------------------------
	CPropSelInt *m_pStyle;
	CPropSelInt *m_pType;
	CPropSelInt *m_pRot;
	CPropInt *m_pPinLen;
	CPropString *m_pName;
	CPropString *m_pNumber;
	//------------------------------------
	// AutoIncrement Variables
	//------------------------------------
public:
	static int m_AutoIncEnable;
	static int m_NameInc;
	static int m_PinInc;
	static char m_LastPinName[32];
	static char m_LastPinNumber[32];
	static int m_LastOrientation;
	static int m_LastStyle;
	static int m_LastType;
public:
	static int CCadPin::m_RenderEnable;
	CCadPin();
	virtual ~CCadPin();
	static void LoadSettings(PinAttributes *pAttrb);
	static void SaveSettings(PinAttributes *pAttrb);
	virtual void Move(CPoint p);
	virtual void Save(FILE * pO, int Indent = 0, int flags = 0);
	virtual void Draw(CDC * pDC, int mode, CPoint Offset = CPoint(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual int CheckSelected(CPoint p, CSize Offest = CSize(0, 0), CCadObject ** ppSelList = 0, int index = 0, int n = 0, int flag = CADDRAWUBG_CHECKSEL_FLAG_ALL);
	virtual CPoint GetReference();
	virtual int IsDirty(void);
	virtual int IsSelected(void);
	virtual void SetDirty(int d);
	virtual void SetSelected(int Flag = 0);
	virtual void AdjustRefernce(CPoint Ref);
	virtual CRect GetRect(void);
	virtual char * GetTypeString(void);
	CCadPin operator=(CCadPin &v);
	virtual CCadObject * CopyObject(void);
	virtual void SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	virtual void ChangeCenter(CSize p);
	virtual int Parse(int Token, CLexer *pLex);
	virtual CCadObject *NewInstance(void);
	void GetAttributes(PinAttributes *pAttrb);
	void CopyAttributes(PinAttributes *pAttrb);
	virtual int MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseMove(CAutoScrollView * pASV, int DrawState);
	virtual int EditProperties(void);
	CCadObject *NewPinWithAutoInc(int NameInc, int PinNumInc);
	CPropSelInt *GetPinStyle(void);
	CPropSelInt *GetPinType(void);
	CPropSelInt *GetPinRotation();
	CPropString *GetPinName();
	CPropInt *GetPinLen();
	CPropString *GetPinNumber();
	void DrawLine(CDC *pDC, int mode, CSize Offset, CScale Scale, int Rotation);
	void DrawDot(CDC *pDC, int mode, CSize Offset, CScale Scale, int Rotation);
	void DrawClock(CDC *pDC, int mode, CSize Offset, CScale Scale, int Rotation);
	void DrawDotClock(CDC *pDC, int mode, CSize Offset, CScale Scale, int Rotation);
	void SetProperty(char *name, char *value);
//	inline CProp *GetProperties(void) { return &m_Properties; }
	void DrawConnection(CDC * pDC, int mode, CSize Offset, CScale Scale,CPoint p);
	void Rotate(int Direction);
	void AdjustStrings();
	void SetPinNameFontSize(int Height, int Width, int Weight);
	void SetPinNumberFontSize(int Height, int Width, int Weight);
	void IncString(char * s, int len, int inc);
	CRect GetPinRect();
	virtual char *GetPropertyName(int Token);
	CPropField *FindProperty(char *name);
};

