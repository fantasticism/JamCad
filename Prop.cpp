#include "stdafx.h"
#include "Prop.h"
#include "JamCad.h"
#include "JamCadDefines.h"
#include "PropField.h"
#include "PropString.h"
#include "PropInt.h"
#include "PropSelInt.h"
#include "PropSelString.h"
#include "Lexer.h"

CProp::CProp()
{
	m_pHead = 0;
	m_pTail = 0;
}

CProp::~CProp()
{
	CPropField *pPF;
	while (m_pHead)
	{
		pPF = RemoveProperty(m_pHead);
		delete pPF;
	}

}

void CProp::AddProperty(CPropField * pF)
{
	//***************************************************
	// AddProperty
	//	Adds a Property Field object to a List.  
	// The new object is added at the rear of the list.
	//
	// parameters:
	//	pF.....pointer to Property Field object to add.
	//
	// return value:none
	//--------------------------------------------------
	if (m_pHead == 0)	//nothing in List
	{
		m_pHead = pF;
		m_pTail = pF;
	}
	else				//add part to end of List
	{
		m_pTail->SetNext(pF);
		pF->SetPrev(m_pTail);
		m_pTail = pF;
	}
}

CPropField *CProp::RemoveProperty(CPropField * pF)
{
	///------------------------------------------
	/// RemoveProperty
	///			This function removes an object
	/// from the Propery List.  This is a DELETE
	///
	///	parameters:
	///		pF.....pointer to object to remove
	/// Return Value:pF
	///------------------------------------------
	if (pF == m_pHead)
	{
		m_pHead = (CPropField *)m_pHead->GetNext();
		if (m_pHead) m_pHead->SetPrev(0);
	}
	else if (pF == m_pTail)
	{
		m_pTail = (CPropField*)m_pTail->GetPrev();
		if (m_pTail) m_pTail->SetNext(0);
	}
	else
	{
		pF->GetPrev()->SetNext(pF->GetNext());
		pF->GetNext()->SetPrev(pF->GetPrev());
	}
	pF->SetPrev(0);
	pF->SetNext(0);
	return pF;
}

void CProp::InsertProperty(CPropField * pF)
{
	///----------------------------------------
	/// InsertProperty
	///		This function is used to add a new
	/// property to a property list.  It puts the
	/// property at the front of the list so
	/// that it will be found first.
	///
	/// parameters:
	///		pF.....pointer to Property Field
	///----------------------------------------
	if (m_pHead == 0)	//nothing in drawing
	{
		m_pHead = pF;
		m_pTail = pF;
	}
	else				//add object to front
	{
		m_pHead->SetPrev(pF);
		pF->SetNext(m_pHead);
		m_pHead = pF;
	}
}

int CProp::Parse(int LookaheadToken, CLexer * pLex)
{
	int loop = 0;
	union {
		CPropField *pPF;
		CPropString *pPS;
		CPropInt *pPI;
		CPropSelInt *pPSI;
		CPropSelString *pPSS;
	}Pf;
	LookaheadToken = pLex->Accept(LookaheadToken, TOKEN_PROPERTY);
	if (LookaheadToken > 0)
	{
		LookaheadToken = pLex->Accept(LookaheadToken, ')');
		loop = 1;
	}
	while (loop)
	{
		switch (LookaheadToken)
		{
			case TOKEN_STRINGFIELD:
				Pf.pPS = new CPropString();
				LookaheadToken = Pf.pPS->Parse(LookaheadToken,pLex);
				AddProperty(Pf.pPF);
				break;
			case TOKEN_INTFIELD:
				Pf.pPI = new CPropInt();
				LookaheadToken = Pf.pPI->Parse(LookaheadToken, pLex);
				this->AddProperty(Pf.pPF);
				break;
			case TOKEN_INTSELFIELD:
				Pf.pPSI = new CPropSelInt();
				LookaheadToken = Pf.pPSI->Parse(LookaheadToken, pLex);
				AddProperty(Pf.pPF);
				break;
			case TOKEN_STRINGSELFIELD:
				break;
			case ')':
				loop = 0;
				break;
			default:
				loop = 0;
				break;
		}
	}
	return LookaheadToken;
}

void CProp::Save(FILE * pO,int Indent, int flags)
{
	CCadObject *pF = m_pHead;
	int flag = CADOBJECT_SAVE_ADDCOMMA_ATEND;
	fprintf(pO, "%s%s(\n", MkIndentString(Indent,' '), TokeToString(TOKEN_PROPERTY));
	while (pF)
	{
		if(pF->GetNext() == 0)
			flag = 0;
		pF->Save(pO,Indent+1,flag);
		pF = pF->GetNext();
	}
	fprintf(pO, "%s)\n", MkIndentString(Indent, ' '));
}

CPropField * CProp::FindProperty(char * pName)
{
	int loop = 1;
	CCadObject *pF = m_pHead;
	while (pF && loop)
	{
		if (((CPropField *)pF)->IsField(pName))
			loop = 0;
		else
			pF = pF->GetNext();
	}
	return (CPropField *)pF;
}

void CProp::Draw(CDC *pDC, int mode, CSize Offset, CScale Scale)
{
	CCadObject *pPF = m_pHead;
	int Count = 0;
	while (pPF && Count < 5000)
	{
		pPF->Draw(pDC, mode, Offset, Scale);
		++Count;
		pPF = pPF->GetNext();
	}
}

void CProp::Create(FieldNameType *DefFields)
{
	int i;
	union _PropFld {
		CPropString *pSTring;
		CPropSelInt *pSelInt;
		CPropSelString *pSelString;
		CPropInt *pInt;
		CPropField *pPropField;
	}PF;

	for (i = 0; DefFields[i].name; ++i)
	{
		switch (DefFields[i].Type)
		{
			case PROP_STRING:
				PF.pSTring = new CPropString(DefFields[i].name);
				PF.pSTring->CopyAttributes(DefFields[i].TextAttrb);
				PF.pSTring->SetDisplayed(DefFields[i].Display);
				PF.pSTring->SetFontSize(90, 0, FW_NORMAL);
				PF.pSTring->SetValue((char *)DefFields[i].InitData);
				break;
			case PROP_INT:
				PF.pInt = new CPropInt(DefFields[i].name);
				PF.pInt->CopyAttributes(DefFields[i].TextAttrb);
				PF.pInt->SetDisplayed(DefFields[i].Display);
				PF.pInt->SetValue((int)DefFields[i].InitData);
				break;
			case PROP_SELECTOR_INT:
				PF.pSelInt = new CPropSelInt(DefFields[i].name);
				PF.pSelInt->Create((PrepSelIntData *)DefFields[i].InitData);
				break;
			case PROP_SELECTOR_STRING:
				break;
		}
		AddProperty(PF.pPropField);
	}
}

char *CProp::GetPropertyName(int PropertyToken, FieldNameType *DefFields)
{
	int i, loop;
	char *pS = 0;
	for (i = 0, loop = 1; DefFields[i].name && loop; ++i)
	{
		if (PropertyToken == DefFields[i].Token)
		{
			pS = DefFields[i].name;
			loop = 0;
		}
	}
	return pS;
}

void CProp::Move(CProp &src)
{
	//-------------------------------------
	// Moves the property fields from
	// one property container to another
	// by removing them from the source
	//
	// parameter:
	//	src......soure property to move from
	//-------------------------------------
	CPropField *pPF;

	pPF = src.GetHead();
	while (pPF)
	{
		src.RemoveProperty(pPF);
		InsertProperty(pPF);
		pPF = src.GetHead();
	}
}

void CProp::Copy(CProp &src)
{
	//-------------------------------------
	// Copy
	// Copies the property fields from
	// one property container to another
	// 
	//
	// parameter:
	//	src......soure property to copy from
	//-------------------------------------
	CPropField *pPF;

	pPF = src.GetHead();
	while (pPF)
	{
		src.RemoveProperty(pPF);
		InsertProperty(pPF);
		pPF = src.GetHead();
	}
}

void CProp::Get(CProp &dst)
{

}

int CProp::CheckSelected(CPoint p, CSize Offset, CCadObject **ppSelList, int index, int n, int flag)
{
	CCadObject *pObj = GetHead();
	while (pObj && (index < n))
	{
		index = pObj->CheckSelected(p, Offset, ppSelList, index, n, flag);
		pObj = pObj->GetNext();
	}
	return index;
}

CRect CProp::GetRect()
{
	CRect rect, rect2;
	CCadObject *pObj = GetHead();
	while (pObj)
	{
		rect2 = pObj->GetRect();
		rect.UnionRect(&rect, &rect2);
		pObj = pObj->GetNext();
	}
	return rect;
}
