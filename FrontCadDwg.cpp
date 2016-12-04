#include "stdafx.h"
#include "FrontCadDwg.h"
#include "JamCadDefines.h"

CFrontCadDwg::CFrontCadDwg()
{
}


CFrontCadDwg::~CFrontCadDwg()
{
}


void CFrontCadDwg::AddObject(CCadObject * pObj)
{
}


void CFrontCadDwg::InsertObject(CCadObject * pObj)
{
}


CCadObject * CFrontCadDwg::RemoveObject(CCadObject * pObj)
{
	return nullptr;
}


int CFrontCadDwg::Parse(int Token, CLexer * pLexer)
{
	Token = pLexer->Accept(TOKEN_DRAWING,Token);
	return Token;
}


void CFrontCadDwg::Save(FILE * pO,int Indent)
{
	CCadObject *pObj = m_pHead;
	fprintf(pO, "%s\n", TokeToString(TOKEN_DRAWING));
	while (pObj)
	{
		pObj->Save(pO,Indent+1);
		pObj = pObj->GetNext();
	}
}
