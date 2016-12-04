#include "stdafx.h"
#include "JamSch.h"
#include "Lexer.h"
#include "JamCadDefines.h"

CJamSch::CJamSch()
{
	m_pHead = 0;
	m_pTail = 0;
	m_pNextInstance = 0;
	m_pParent = 0;
	m_SchProperties = 0;
}


CJamSch::~CJamSch()
{
	if (m_SchProperties) delete m_SchProperties;
}


void CJamSch::AddObject(CCadObject * pObj)
{
	//***************************************************
	// AddObject
	//	Adds a child object to an object.  This function
	// is not supported by all objects, because they just
	// can have children.  A good example of an object
	// that has children would be a library object.
	// The new object is added at the rear of the list.
	//
	// parameters:
	//	pObj.....pointer to object to add.
	//
	// return value:none
	//--------------------------------------------------
	if (m_pHead == 0)	//nothing in drawing
	{
		m_pHead = pObj;
		m_pTail = pObj;
	}
	else				//add part to end of drawing
	{
		m_pTail->SetNext(pObj);
		pObj->SetPrev(m_pTail);
		m_pTail = pObj;
	}
	if (m_pNextInstance)
		m_pNextInstance->InsertObject(pObj->NewInstance());
}


void CJamSch::InsertObject(CCadObject * pObj)
{
	///----------------------------------------
	/// InsertObject
	///		This function is used to add a new
	/// object to a drawing.  It puts the
	/// object at the front of the list so
	/// that it will be drawwn first.
	///
	/// parameters:
	///		pObj.....pointer to CCad Object
	///----------------------------------------
	if (m_pHead == 0)	//nothing in drawing
	{
		m_pHead = pObj;
		m_pTail = pObj;
	}
	else				//add object to front
	{
		m_pHead->SetPrev(pObj);
		pObj->SetNext(m_pHead);
		m_pHead = pObj;
	}
	if (m_pNextInstance)
		m_pNextInstance->InsertObject(pObj->NewInstance());
}


int CJamSch::RemoveObject(CCadObject * pObj)
{
	///------------------------------------------
	/// RemoveObject
	///			This function removes an object
	/// from the drawing.  This is a DELETE
	///
	///	parameters:
	///		pObj.....pointer to object to remove
	/// Return Value:True on success, 0 on fail
	///------------------------------------------
	int rV = 0;
	if (pObj == m_pHead)
	{
		m_pHead = (CCadObject *)m_pHead->GetNext();
		if (m_pHead) m_pHead->SetPrev(0);
		rV = 1;
	}
	else if (pObj == m_pTail)
	{
		m_pTail = (CCadObject*)m_pTail->GetPrev();
		if (m_pTail) m_pTail->SetNext(0);
	}
	else
	{
		pObj->GetPrev()->SetNext(pObj->GetNext() );
		pObj->GetNext()->SetPrev(pObj->GetPrev() );
	}
	if (m_pNextInstance)
		m_pNextInstance->RemoveObject(pObj->GetNextInstance());
	pObj->SetPrev(0);
	pObj->SetNext(0);
	return rV;
}


int CJamSch::Parse(int Token,CLexer * pLex)
{
	//***************************************************
	// Parse
	//	This function is used to parse this 
	// object out of an input stream
	//
	// parameters:
	//	Token....Token for this object
	//	pLex.....Pointer to Lexer for the data stream
	//
	// return value:
	//	returns lookahead token on success, or
	//			negative value on error
	//--------------------------------------------------
	return Token;
}


void CJamSch::Save(FILE * pO,int Indent)
{
//	fprintf(pO, "%s(\"%s\",\"%s\"){\n", 
//		TokeToString(TOKEN_SCHEMATIC));
	CCadObject *pOb = m_pHead;
	while (pOb)
	{
		pOb->Save(pO,Indent+1);
		pOb = pOb->GetNext();
	}
}


void CJamSch::Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale)
{
	CCadObject *pO = m_pHead;
	while (pO)
	{
		pO->Draw(pDC, mode, Offset, Scale);
		pO = pO->GetNext();
	}
}
