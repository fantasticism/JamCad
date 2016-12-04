#include "stdafx.h"
#include "PropField.h"
#include <string.h>
#include "JamCadDefines.h"
#include "JamCad.h"

char *PropFieldNames[] = {
	"Units",		//number of units in a drawing object
	"Component Name",	//name of componment
	"Reference Des",		//reference designator for schematic component
	"Pin Name",	//name of a component pin
	"Pin Number",	//pin number of a compoennt pin
	"Pin Style",	//stye of pin
	"Pin Type",	//type of pin
	"Pin Rotation",	//pin rotation
	"Pen Length",		//Pin Length
	"Reference",	//component reference number
	"Sch Lib Name",	//schematic library name
	NULL
};

CPropField::CPropField():CCadText()
{
	m_pName = new char[PROPFIELD_MAX_STRINGLEN];
	m_pName[0] = 0;
	m_FieldType = PROP_NONE;
	CCadText::CopyAttributes(&theApp.m_SchTextAttr);
	CCadText::SetText("");
}

CPropField::CPropField(char *name, int type):CCadText()
{
	m_pName = new char[PROPFIELD_MAX_STRINGLEN];
	strcpy(m_pName, name);
	m_FieldType = type;
	CCadText::CopyAttributes(&theApp.m_SchTextAttr);
	CCadText::SetText("");
}

CPropField::~CPropField()
{
	if (m_pName) delete[] m_pName;
}

int CPropField::Parse(int Token, CLexer *pLex)
{
	Token = pLex->Accept(Token, TOKEN_FIELD);
	Token = pLex->Accept(Token, '(');
	Token = pLex->String(m_pName, Token);
	Token = pLex->Accept(Token, ',');
	Token = pLex->String(Attributes().m_pText, Token);
	Token = pLex->Accept(Token, ')');
	return Token;
}

void CPropField::Save(FILE *pO,int Indent, int flags)
{
	fprintf(pO, "%s%s(\"%s\",\"%s\")\n", MkIndentString(Indent, ' '), TokeToString(TOKEN_FIELD), m_pName, Attributes().m_pText);
}

int CPropField::IsField(char * pName)
{
	//-------------------------------------------
	//	IsField
	//		Checks the field name against the
	// the parameter and returns true if there
	// is a match
	//
	// parameters:
	//		pName......field name we are looking for
	//------------------------------------------
	int rV = 0;
	if (strcmp(m_pName, pName) == 0) rV = 1;
	return rV;
}

void CPropField::SetValue(char * pValue)
{
	SetFieldValue(pValue);
}

void CPropField::SetFieldValue(char *name)
{
	CCadText::SetText(name);
}

char *CPropField::GetTypeString(void)
{
	return "Prop Field";
}

void CPropField::SetFieldName(char *name)
{
	strcpy_s(m_pName, PROPFIELD_MAX_STRINGLEN, name);
}

void CPropField::Draw(CDC *pDC, int mode, CPoint Offset, CScale scale)
{
	if (IsDisplayed())
		CCadText::Draw(pDC, mode, Offset, scale);
}

CPropField *CPropField::Copy()
{
	return 0;
}
