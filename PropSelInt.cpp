#include "stdafx.h"
#include "PropSelInt.h"
#include "JamCad.h"
#include "JamCadDefines.h"

CPropSelInt::CPropSelInt():CPropField()
{
	m_Value = 0;
	m_SelNames = 0;
}

CPropSelInt::CPropSelInt(char *name) : CPropField(name,PROP_SELECTOR_INT)
{
	m_Value = 0;
	m_SelNames = 0;
	m_Selection = 0;
}


CPropSelInt::~CPropSelInt()
{
	if (m_Value) delete[] m_Value;
	if (m_SelNames) delete[] m_SelNames;
}

void CPropSelInt::Create(PrepSelIntData *pData)
{
	int i,count;
	for (count = 0; pData[count].label; ++count);
	m_MaxSelection = count;
	m_SelNames = new char *[count];
	m_Value = new int[count];
	for (i = 0; pData[i].label; ++i)
	{
		m_SelNames[i] = pData[i].label;
		m_Value[i] = pData[i].Value;
	}
}

int CPropSelInt::GetSelection()
{
	return m_Selection;
}


int CPropSelInt::GetSelectedValue()
{
	return m_Value[m_Selection];
}

void CPropSelInt::SetSelection(int i)
{
	if (i >= m_MaxSelection) i = 0;
	if (i < 0) i = m_MaxSelection - 1;
	m_Selection = i;
	char *s = new char[256];
	sprintf_s(s, 256, "%d", m_Value[m_Selection]);
	CCadText::SetText(s);
	delete[] s;
}

void CPropSelInt::Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale)
{
	if (IsDisplayed())
		CCadText::Draw(pDC, mode, Offset, Scale);
}

void CPropSelInt::Save(FILE *pO, int Indent, int flags)
{
	int i;
	int eol = ' ';
	if (flags & CADOBJECT_SAVE_ADDCOMMA_ATEND)
		eol = ',';
	fprintf(pO, "%s%s(\"%s\",%s(%d)){\n",
		MkIndentString(Indent, ' '),
		TokeToString(TOKEN_INTSELFIELD),
		GetFieldName(),
		TokeToString(TOKEN_SELECTION),
		GetSelection()
	);
	fprintf(pO, "%s%s[\n",
		MkIndentString(Indent+1),
		TokeToString(TOKEN_SELTITLES)
		);
	for (i = 0; i < this->m_MaxSelection; ++i)
	{
		if (m_MaxSelection - 1 == i)
			fprintf(pO, "%s%d(\"%s\",%s(%d))]\n",
				MkIndentString(Indent + 2),
				i,
				this->m_SelNames[i],
				TokeToString(TOKEN_VALUE),
				m_Value[i]
			);
		else
			fprintf(pO, "%s%d(\"%s\",%s(%d)),\n",
				MkIndentString(Indent + 2),
				i,
				this->m_SelNames[i],
				TokeToString(TOKEN_VALUE),
				m_Value[i]
			);
	}
	CCadText::Save(pO, Indent + 1);
}

int CPropSelInt :: Parse(int Token, CLexer *pLex)
{
	char *s;
	Token = pLex->Accept(Token, TOKEN_INTSELFIELD);
	Token = pLex->Accept(Token, ')');
	s = GetFieldName();
	Token = pLex->String(s, Token);
	Token = pLex->Accept(Token, ',');
	Token = pLex->ReadDecimalValue(TOKEN_SELECTION, m_Selection, Token);
	Token = pLex->Accept(Token, ')');
	return Token;
}

