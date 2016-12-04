#pragma once
#include "PropField.h"

typedef struct {
	char *label;
	int Value;
}PrepSelIntData;

class CPropSelInt:public CPropField
{
	char **m_SelNames;
	int *m_Value;
	int m_Selection;
	int m_MaxSelection;
public:
	CPropSelInt();
	CPropSelInt(char *name);
	~CPropSelInt();
	void Create(PrepSelIntData *pData);
	int GetSelection();
	int GetSelectedValue();
	void SetSelection(int i);
	virtual void Draw(CDC * pDC, int mode, CPoint Offset = CPoint(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual void Save(FILE *pO, int Indent=0, int flags=0);
	virtual int Parse(int Token, CLexer *pLex);
};

