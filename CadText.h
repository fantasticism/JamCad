#pragma once
#include "CadObject.h"
#include "JamCadDefines.h"
#include "CadPolygon.h"

#define TEXT_STRING_LOCAL		1
#define TEXT_STRING_EXTERNAL	0


struct TextAttributes {
	COLORREF m_Color;
	COLORREF m_BkColor;
	int m_FontHeight;
	int m_FontWidth;
	int m_Angle;
	int m_Transparent;
	int m_Weight;
	UINT m_Format;
	char *m_pFontName;
	char *m_pText;
	TextAttributes();
	~TextAttributes();
};

class CCadText:public CCadObject
{
	TextAttributes m_Attrb;
	CFont *m_pFont;
	CPen *m_pSelPen;
	CCadPolygon m_SelRect;
	double m_LastScaleX;
public:
	static int m_RenderEnable;
	CCadText();
	CCadText(CCadText& v);
	virtual ~CCadText();
	static void LoadSettings(char *Section,TextAttributes *pAttrb);
	static void SaveSettings(char *Section, TextAttributes *pAttrb);
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
	CCadText operator=(CCadText &v);
	virtual CCadObject * CopyObject(void);
	virtual void SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw);
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
	virtual int Parse(int Token, CLexer *pLex);
	virtual CCadObject *NewInstance(void);
	void CopyAttributes(TextAttributes *pAttrb);
	void GetAttributes(TextAttributes *pAttrb);
	virtual int MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData);
	virtual int MouseMove(CAutoScrollView * pASV, int DrawState);
	virtual int EditProperties(void);
	//-------------------Specific To This class---------------------
	void SetText(char *pS);
	void GetText(char *s, int n);
	char * GetText(void);
	CRect GetTextRectangle();
	CRect GetTextRectangle(CDC *pDC, CScale Scale);
	void SetFontName(char *s);
	void Rotate(int Angle, CRect rect, CCadPolygon &Poly);
	CPoint CalcTextShiftonRotation(CPoint p1, CPoint Center, double angle);
	void SetAngle(int e);
	inline void SetFontWeight(int w) { m_Attrb.m_Weight = w; }
	inline void SetFontHeight(int h) { m_Attrb.m_FontHeight = h; }
	inline void SetFontWidth(int w) { m_Attrb.m_FontWidth = w; }
	void UpdateText(void);
	inline TextAttributes &Attributes() { return m_Attrb; }
	inline TextAttributes *pAttributes() { return &m_Attrb; }
};

