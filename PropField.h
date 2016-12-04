#pragma once

#include "CadText.h"
//#include "Lexer.h"
//#include "Scale.h"

enum pfType{
	PROP_NONE=0,
	PROP_STRING,
	PROP_INT,
	PROP_SELECTOR_INT,
	PROP_SELECTOR_STRING
};

enum propNameStrings {	//list of built in properties type
	PF_NAME_UNITS,		//number of units in a drawing object
	PF_NAME_COMPNAME,	//name of componment
	PF_NAME_REFDES,		//reference designator for schematic component
	PF_NAME_PINNAME,	//name of a component pin
	PF_NAME_PINNUMBER,	//pin number of a compoennt pin
	PF_NAME_PINSTYLE,	//stye of pin
	PF_NAME_PINTYPE,	//type of pin
	PF_NAME_PINROTATION,	//pin rotation
	PF_NAME_PINLEN,		//Pin Length
	PF_NAME_COMPREF,	//component reference number
	PF_NAME_SCHLIBNAME,	//schematic library name
};

extern char *PropFieldNames[];

struct FieldNameType {
	char *name;
	int Type;
	int Token;
	int Display;
	void *InitData;
	TextAttributes *TextAttrb;
};

#define PROPFIELD_MAX_STRINGLEN		32

#define PROPFIELD_DISPLAY	1
#define PROPFIELD_HIDE		0

class CPropField:public CCadText
{
	unsigned m_FieldType : 8;		//specifies the Field type
	unsigned m_Display : 1;		//indicates if property is to be displayed
	unsigned m_Rotation : 2;	//Rotation of displayed propery
	char *m_pName;				//property name
public:
	inline void SetFieldType(unsigned T) { m_FieldType = T; }
	inline unsigned GetFieldType(void) { return m_FieldType; }
	inline unsigned IsDisplayed(void) { return m_Display; }
	inline void SetDisplayed(int f) { m_Display = f; }
	inline unsigned GetRotation(void) { return m_Rotation; }
	inline void SetRotation(int r) { m_Rotation = r; }
	inline char *GetFieldName(void) { return m_pName; }
	inline char *GetFieldValue(void) { return CCadText::GetText(); }
	CPropField();
	CPropField(char *name, int type);
	virtual ~CPropField();
	virtual int Parse(int Token, CLexer *pLex);
	virtual void Save(FILE *pO, int Indent=0, int flags=0);
	virtual int IsField(char * pName);
	virtual void SetValue(char * pValue);
	virtual char *GetTypeString(void);
	virtual void SetFieldName(char *name);
	virtual void SetFieldValue(char *name);
	virtual void Draw(CDC *pDC, int mode, CPoint Offset, CScale scale);
	virtual CPropField *Copy();
};

