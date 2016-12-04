#include "stdafx.h"
#include "JamCad.h"
#include "PropString.h"


CPropString::CPropString()
{
}

CPropString::CPropString(char *name) :CPropField(name, PROP_STRING)
{
}

CPropString::~CPropString()
{
}

void CPropString::Move(CPoint p)
{
	m_Ref = p;
}

void CPropString::SetRotation(int Rotation,CPoint Origin)
{
	CPropField::SetRotation(Rotation);
	CRect rect;
	rect  = CCadText::GetTextRectangle();
	switch (Rotation)
	{
		case ROTATION_0:
			m_P1 = m_Ref;
			break;
		case ROTATION_90:
			m_P1 = RotatePoint(Origin, m_Ref+CSize(rect.Width(),0), Rotation);
			break;
		case ROTATION_180:
			m_P1 = RotatePoint(Origin, m_Ref + CSize(rect.Width(), 0), Rotation);
			break;
		case ROTATION_270:
			m_P1 = RotatePoint(Origin, m_Ref , Rotation);
			break;
	}
	CPropField::SetRotation(Rotation);
}

void CPropString::Rotate(int Dir)
{

}

void CPropString::Flip(CPoint Origin,int Axis)
{
	//-------------------------------------------
	// Flip
	//		flip object about an axis
	//
	// Paramters:
	//	Origin.......specifies location of axis
	//	Axis.........0=X axis, 1 = Y Axis
	//--------------------------------------------
	if (Axis)	//Filp about y axis, change X
		m_P1 = CPoint((Origin.x - m_Ref.x) + Origin.x, m_Ref.y);
	else	//Flip about x axis, change Y
		m_P1 = CPoint(m_Ref.x, (Origin.y - m_Ref.y) + Origin.y);
	switch (CPropField::GetRotation())
	{
		case ROTATION_0:
			CPropField::SetRotation(ROTATION_180);
			break;
		case ROTATION_90:
			CPropField::SetRotation(ROTATION_270);
			break;
		case ROTATION_180:
			CPropField::SetRotation(ROTATION_0);
			break;
		case ROTATION_270:
			CPropField::SetRotation(ROTATION_90);
			break;
	}
}


void CPropString::SetFontSize(int Height, int Width, int Weight)
{
	SetFontHeight(Height);
	SetFontWidth(Width);
	SetFontWeight(Weight);
	UpdateText();
}

void CPropString::SetValue(char * pValue)
{
	SetText(pValue);
	CPropField::SetValue(pValue);
}

void CPropString::Save(FILE *pO, int Indent, int flags)
{
	int eol = ' ';
	if (flags & CADOBJECT_SAVE_ADDCOMMA_ATEND)
		eol = ',';
	fprintf(pO, "%s%s(\"%s\",%s(%d),%s(%d)) {\n",
			MkIndentString(Indent, ' '),
			TokeToString(TOKEN_STRINGFIELD),
			GetFieldName(),
			TokeToString(TOKEN_DISPLAY),
			IsDisplayed(),
			TokeToString(TOKEN_ROTATION),
			this->GetRotation()
	);
	CCadText::Save(pO, Indent + 1);
	fprintf(pO, "%s}%c\n", MkIndentString(Indent),eol);
}

int CPropString::Parse(int LookaheadToken, CLexer *pLex)
{
	char *s;
	LookaheadToken = pLex->Accept(LookaheadToken, TOKEN_STRINGFIELD);
	LookaheadToken = pLex->Accept(LookaheadToken, '(');
	s = GetFieldName();
	LookaheadToken = pLex->String(s, LookaheadToken);
	LookaheadToken = pLex->Accept(LookaheadToken, ')');
	s = GetFieldValue();
	LookaheadToken = pLex->String(s, LookaheadToken);
	LookaheadToken = pLex->Accept(LookaheadToken, ')');
	return LookaheadToken;
}

CPropField *CPropString::Copy()
{
	CPropString *pNewPropString = new CPropString(GetFieldName());
	CCadText::GetAttributes(pNewPropString->pAttributes());
	pNewPropString->SetFieldValue(GetFieldValue());
	return pNewPropString;
}
