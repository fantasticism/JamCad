#pragma once

#include "CadObject.h"
#include "Prop.h"

class CJamSch
{
	CCadObject *m_pHead;	//chain of objects in schematic
	CCadObject *m_pTail;
	CJamSch *m_pNextInstance;	//next instance of schematic
	CJamSch *m_pParent;			//parent shcematic sheet
	CProp *m_SchProperties;
public:
	CJamSch();
	virtual ~CJamSch();
	void AddObject(CCadObject * pObj);
	void InsertObject(CCadObject * pObj);
	int RemoveObject(CCadObject * pObj);
	int Parse(int Token, CLexer * pLex);
	void Save(FILE * pO, int Indent);
	void Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale);
};

