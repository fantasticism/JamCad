#pragma once
#include "PropField.h"

class CPropInt:public CPropField
{
	int m_Value;
public:
	CPropInt();
	CPropInt(char *pN);
	~CPropInt();
	void SetValue(int v);
	int GetValue(void);
	virtual void Draw(CDC * pDC, int mode, CPoint Offset = CPoint(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual void Save(FILE *pO, int Indent=0, int flags=0);
	virtual int Parse(int Token, CLexer *pLex);
	virtual CPropField *Copy();
};

