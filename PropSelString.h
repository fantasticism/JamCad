#pragma once
#include "PropField.h"


class CPropSelString:public CPropField
{
	char **m_SelNames;
	int m_Selection;
	int m_MaxSelection;
public:
	CPropSelString();
	~CPropSelString();
	void Create(char **ppData);
	int GetSelection();
	char * GetSelectedValue();
	void SetSelection(int i);
	virtual CPropField *Copy();
};

