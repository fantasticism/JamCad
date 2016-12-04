#pragma once
#include "CadObject.h"
#include "Lexer.h"

class CFrontCadDwg
{
	CCadObject *m_pHead;
	CCadObject *m_pTail;
public:
	CFrontCadDwg();
	virtual ~CFrontCadDwg();
	void AddObject(CCadObject * pObj);
	void InsertObject(CCadObject * pObj);
	CCadObject * RemoveObject(CCadObject * pObj);
	int Parse(int Token, CLexer * pLexer);
	void Save(FILE * pO, int Indent);
};

