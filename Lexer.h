#pragma once
#include <stdio.h>

class CProp;

class CLexer
{
	int m_UnGetBuff;
	FILE *m_pIN;
	int m_LexIndex;
	char m_LexBuff[256];
public:
	CLexer();
	CLexer(char *path);
	CLexer(FILE *pIn);
	virtual ~CLexer();
	int Lex();
	int GetChar();
	int UnGetChar(int c);
	int LookUp(char *pS);
	char *TokenToString(int Token);
	int Accept(int LookaHead, int Token);
	int Expect(int LookaHead, int Token);
	//-----------------------------------------------
	// parsing primintives
	//-----------------------------------------------
	int Point(int token,CPoint &p, int lookaheadtoken);
	int Color(int token,COLORREF &c, int lookaheadtoken);
	int Size(int token,CSize &s, int lookaheadtoken);
	int ReadDecimalValue(int typetoken, int &v, int Token);
	int String(char *&s, int Token);
	void UnexpectedToken(int Token);
};