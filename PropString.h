#pragma once
#include "PropField.h"
#include "CadText.h"

class CPropString :	public CPropField
{
//	CCadText *m_pText;
	CPoint m_Ref;		//master reference point for object.
public:
	CPropString();
	CPropString(char *name);
	virtual ~CPropString();
	virtual void Move(CPoint p);
	virtual void SetValue(char * pValue);
	void SetRotation(int r, CPoint Oritgin);
	void Rotate(int Dir);
	void Flip(CPoint Origin,int Axis);
	void SetFontSize(int Height, int Width, int Weight);
	virtual void Save(FILE *pO, int Indent=0, int flags=0);
	virtual CPropField *Copy();
	virtual int Parse(int Token, CLexer *pLex);
};

