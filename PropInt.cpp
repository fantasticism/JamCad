#include "stdafx.h"
#include "JamCad.h"
#include "PropInt.h"


CPropInt::CPropInt()
{
	m_Value = 0;
}

CPropInt::CPropInt(char *pN):CPropField(pN,PROP_INT)
{
	m_Value = 0;
}

CPropInt::~CPropInt()
{
}

void CPropInt::SetValue(int v)
{
	m_Value = v;
	char *s = new char[PROPFIELD_MAX_STRINGLEN];
	sprintf_s(s, PROPFIELD_MAX_STRINGLEN, "%d", m_Value);
	CCadText::SetText(s);
	delete[] s;
}

int CPropInt::GetValue(void)
{
	return m_Value;
}

void CPropInt::Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale)
{

}

void CPropInt::Save(FILE *pO,int Indent, int flags)
{
	int eol = ' ';
	if (flags & CADOBJECT_SAVE_ADDCOMMA_ATEND)
		eol = ',';
	fprintf(pO, "%s%s(\"%s\",%s(%d),%s(%d)){\n",
		MkIndentString(Indent, ' '),
		TokeToString(TOKEN_INTFIELD),
		GetFieldName(),
		TokeToString(TOKEN_DISPLAY),
		IsDisplayed(),
		TokeToString(TOKEN_ROTATION),
		this->GetRotation()
	);
	CCadText::Save(pO, Indent + 1, 0);
	fprintf(pO, "%s}%c\n", MkIndentString(Indent), eol);
}

int CPropInt::Parse(int Token, CLexer *pLex)
{
	char *s;
	Token = pLex->Accept(Token, TOKEN_INTFIELD);
	Token = pLex->Accept(Token, '(');
	s = GetFieldName();
	Token = pLex->String(s, Token);
	Token = pLex->Accept(Token, ',');
	s = GetFieldValue();
	Token = pLex->String(s, Token);
	SetValue(atoi(s));
	return Token;
}

CPropField *CPropInt::Copy()
{
	CPropInt *pNewPropInt;
	pNewPropInt = new CPropInt(this->GetFieldName());
	pNewPropInt->SetValue(this->GetValue());
	return pNewPropInt;
}