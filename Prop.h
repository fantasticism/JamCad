#pragma once
#include "Lexer.h"
#include "Scale.h"
//#include "PropField.h"

class CPropField;
struct FieldNameType;
class CCadObject;

class CProp
{
	CPropField *m_pHead;
	CPropField *m_pTail;
public:
	CProp();
	virtual ~CProp();
	void Create(FieldNameType *DefFields);
	virtual void Draw(CDC *pDC, int mode, CSize Offset, CScale Scale);
	void AddProperty(CPropField * pF);
	void InsertProperty(CPropField * pF);
	CPropField *RemoveProperty(CPropField * pF);
	int Parse(int Token, CLexer * pLex);
	void Save(FILE * pO, int Indent, int flags);
	CPropField * FindProperty(char * pName);
	char *GetPropertyName(int PropertyToken, FieldNameType *DefFields);
	void Move(CProp &src);
	void Copy(CProp &src);
	void Get(CProp &dst);
	inline CPropField *GetHead(void) { return m_pHead; }
	int CheckSelected(CPoint p, CSize Offset, CCadObject **ppSelList, int index, int n, int flag);
	CRect GetRect();
};

