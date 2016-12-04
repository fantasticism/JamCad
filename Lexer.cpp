#include "stdafx.h"
#include "Lexer.h"
#include "JamCadDefines.h"
#include <strsafe.h>
#include "Prop.h"
#include "PropField.h"
#include "PropSelInt.h"
#include "PropInt.h"
#include "PropSelString.h"
#include "PropString.h"

KEYWORD KeyWords[] =
{
	{TOKEN_LINE,"LINE"},
	{TOKEN_RECT,"RECT"},
	{TOKEN_ELISPE,"ELISPE"},
	{TOKEN_ROUNDRECT,"ROUNDRECT"},
	{TOKEN_POLYGON,"POLYGON"},
	{TOKEN_POLYGONFILLED,"POLYFILL"},
	{TOKEN_LIBPART,"LIBPART"},
	{TOKEN_HOLEROUND,"HOLEROUND"},
	{TOKEN_HOLERECT,"HOLERECT"},
	{TOKEN_HOLERND1FLAT,"HOLERND1FLAT"},
	{TOKEN_HOLERND2FLAT,"HOLDRND2FLAT"},
	{TOKEN_TEXT,"TEXT"},
	{TOKEN_ARC,"ARC"},
	{TOKEN_ARCCENTERED,"ARCCENT"},
	{TOKEN_BITMAP,"BITMAP"},
	{TOKEN_ARROW,"ARROW"},
	{TOKEN_ORG,"ORIGIN"},
	{TOKEN_DIMENSION,"DIMENSION"},
	{TOKEN_WIRE,"WIRE"},
	{TOKEN_BUS,"BUS"},
	{TOKEN_SCH_PART,"PART"},
	{TOKEN_POWER,"POWER"},
	{TOKEN_SHEET,"SHEET"},
	{TOKEN_SHEETPORT,"PORT"},
	{TOKEN_SHEETPIN,"SHEETPIN"},
	{TOKEN_BUSENTRY,"BUSSENTRY"},
	{TOKEN_JUNCTION,"JUNCTION"},
	{ TOKEN_PIN ,"PIN"},
	{TOKEN_COLOR,"COLOR"},
	{ TOKEN_BKCOLOR ,"COLORBK"},
	{ TOKEN_FILLCOLOR , "FILLCOLOR"},
	{ TOKEN_WIDTH , "WIDTH"},
	{ TOKEN_POINT ,"POINT"},
	{ TOKEN_SIZE ,"SIZE"},
	{ TOKEN_FIELD ,"FIELD"},
	{ TOKEN_PROPERTY ,"PROPERTY"},
	{ TOKEN_SCHEMATIC ,"SCHEMATIC"},
	{ TOKEN_DRAWING , "DRAWING"},
	{ TOKEN_RED , "RED"},
	{ TOKEN_GREEN, "GREEN"},
	{ TOKEN_BLUE , "BLUE"},
	{ TOKEN_RADIUS,"RADIUS"},
	{TOKEN_ROTATION,"ROTATION"},		//Property Selector Type
	{TOKEN_PINSTYLE,"PINSTYLE"},		//property selector type
	{TOKEN_PINTYPE,"PINTYPE" },			//Property Selector Type
	{ TOKEN_PINROTATION ,"PINROTATION"},//Property Selector Type
	{ TOKEN_PINLENGTH , "PINLENGTH"},	//Property Int Type
	{ TOKEN_PINNUMBER , "PINNUMBER"},
	{ TOKEN_PINNAME , "PINNAME"},
	{ TOKEN_PARTNAME,"PARTNAME"},
	{ TOKEN_NUMUNITS,"NUMBEROFUNITS"},
	{ TOKEN_REFDESIGNATOR,"REFERENCEDESIGNATOR"},
	{TOKEN_NUMUNITS,"NUMOFUNITS"},		//Property Int Type
	{TOKEN_REFNUMBER,"REFNUMBER"},	//property Int Type
	{ TOKEN_SCHLIBRARY ,"SCHLIBRARY"},
	{ TOKEN_INTFIELD ,"INTFIELD"},
	{ TOKEN_SCHPART,"SCHPART"},
	{ TOKEN_STRINGFIELD ,"STRINGFIELD"},
	{ TOKEN_LENGTH ,"LENGTH"},
	{ TOKEN_FILE ,"FILE"},
	{ TOKEN_INTSELFIELD ,"INTSELFIELD"},
	{ TOKEN_VERTEX , "VERTEX"},
	{ TOKEN_FONT ,"FONT"},
	{ TOKEN_WEIGHT ,"WEIGHT"},
	{ TOKEN_HEIGHT ,"HEIGHT"},
	{ TOKEN_ANGLE ,"ANGLE"},
	{ TOKEN_TRANSPARENT ,"TRANSPARENT" },
	{ TOKEN_STRINGSELFIELD ,"STRINGSELFIELD"},
	{TOKEN_SELECTION,"SEL"},
	{TOKEN_VALUE,"VALUE"},
	{ TOKEN_SELTITLES ,"SELTITLES"},
	{ TOKEN_DISPLAY ,"DISPLAY"},
	{0,0}
};

char *TokeToString(int Token)
{
	char *pString = 0;
	int i;
	int loop = 1;
	for (i = 0; KeyWords[i].m_pName && loop; ++i)
	{
		if (Token == KeyWords[i].m_Token)
		{
			pString = KeyWords[i].m_pName;
			loop = 0;
		}
	}
	return pString;

}

CLexer::CLexer()
{
	m_pIN = 0;
	m_UnGetBuff = 0;
}

CLexer::CLexer(char *path)
{
	m_UnGetBuff = 0;
	m_pIN = fopen(path, "r");
	if (m_pIN == NULL)
		::MessageBox(NULL, (path), ("ERROR:Can\'t Open"), MB_ICONHAND | MB_OK);
}

CLexer::CLexer(FILE *pIn)
{
	m_UnGetBuff = 0;
	m_pIN = pIn;
}

CLexer::~CLexer()
{
}

int CLexer::GetChar()
{
	int rV;

	if (m_UnGetBuff)
	{
		rV = m_UnGetBuff;
		m_UnGetBuff = 0;
	}
	else
	{
		rV = fgetc(m_pIN);
	}
	return rV;
}

int CLexer::UnGetChar(int c)
{
	m_UnGetBuff = c;
	return 0;
}

char *CLexer::TokenToString(int Token)
{
	return TokeToString(Token);
}


int CLexer::LookUp(char *pKW)
{
	int rV = -1;
	int i, loop;
	for (i = 0, loop = 1; KeyWords[i].m_pName && loop; ++i)
	{
		if (strcmp(KeyWords[i].m_pName, pKW) == 0)
		{
			loop = 0;
			rV = KeyWords[i].m_Token;
		}
	}
	return rV;
}

int CLexer::Accept(int LookaHead, int Token)
{
	/********************************************
	** Accept
	**
	** this function is used to match
	** the expected token with the current
	** token.  If they match, get the
	** next token.
	**
	** parameter:
	**	LookAHead...Look Ahead Token
	**	Token..expected token
	**	pDoc....pointer to document instance
	**
	** Returns:Next token, or -1 on error
	********************************************/
	int rV;

	if (Expect(LookaHead, Token))
		rV = Lex();
	else
		rV = -1;
	return rV;
}


int CLexer::Expect(int Lookahead, int Token)
{
	int rV = 0;

	if (Token == Lookahead)
		rV = 1;
	else
	{
		char *s = new char[256];
		sprintf(s, "Expected %s  Got %s", TokenToString(Token), TokenToString(Lookahead));
		MessageBox(NULL, (s), ("Error"), MB_OK | MB_ICONHAND);
		delete[] s;
	}
	return rV;
}

int CLexer::Lex()
{
	int loop, rV;
	int c;
	int token;
	loop = 1;
	while (loop)
	{
		c = GetChar();
		switch (c)
		{
		case EOF:
			loop = 0;
			rV = c;
			break;
		case ' ': case '\t':	//whitespace
			break;
		case '\n':	//end of line
			break;
		case '\"':	//string
			m_LexIndex = 0;
			while ((c = GetChar()) != '\"')
			{
				m_LexBuff[m_LexIndex++] = c;
			}
			m_LexBuff[m_LexIndex] = 0;
			rV = TOKEN_STRING;
			loop = 0;
			break;
		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
		case '8': case '9':	case '-': //deccimal number
			m_LexIndex = 0;
			do
			{
				m_LexBuff[m_LexIndex++] = c;
			} while (isdigit(c = GetChar()));
			UnGetChar(c);
			m_LexBuff[m_LexIndex] = 0;
			loop = 0;
			rV = TOKEN_NUM;
			break;
		case ',': case '(': case ')': case '{': case '}':
			rV = c;
			loop = 0;
			break;
		default:	//keywords
			m_LexIndex = 0;
			m_LexBuff[m_LexIndex++] = c;
			while (isalnum(c = GetChar()))
			{
				m_LexBuff[m_LexIndex++] = c;
			}
			UnGetChar(c);
			m_LexBuff[m_LexIndex] = 0;
			token = LookUp(m_LexBuff);
			if (token >= 0)
				rV = token;
			else
				rV = -1;
			loop = 0;
			break;
		}
	}
	return rV;
}

//--------------------------------------------------
// Misc Parsing Functions that come in handy
//--------------------------------------------------

int CLexer::Point(int Token, CPoint &p, int LookAheadToken)
{
	int x, y;

	LookAheadToken = Accept(Token, LookAheadToken);
	if (LookAheadToken >= 0)LookAheadToken = Accept('(', LookAheadToken);
	if (LookAheadToken >= 0)x = atoi(m_LexBuff);
	if (LookAheadToken >= 0)LookAheadToken = Accept(TOKEN_NUM, LookAheadToken);
	if (LookAheadToken >= 0)LookAheadToken = Accept(',', LookAheadToken);
	if (LookAheadToken >= 0)y = atoi(m_LexBuff);
	if (LookAheadToken >= 0)LookAheadToken = Accept(TOKEN_NUM, LookAheadToken);
	if (LookAheadToken >= 0)LookAheadToken = Accept(')', LookAheadToken);
	if (LookAheadToken >= 0)p = CPoint(x, y);
	return LookAheadToken;
}

int CLexer::Color(int token,COLORREF &c, int LookaheadToken)
{
	int Red=0, Blue=0, Green=0;
	int loop = 1;

	LookaheadToken = Accept(token, LookaheadToken);
	if (LookaheadToken >= 0)LookaheadToken = Accept('(', LookaheadToken);
	while (loop)
	{
		switch (LookaheadToken)
		{
			case TOKEN_RED:
				LookaheadToken = ReadDecimalValue(TOKEN_RED, Red, LookaheadToken);
				break;
			case TOKEN_GREEN:
				LookaheadToken = ReadDecimalValue(TOKEN_GREEN, Green, LookaheadToken);
				break;
			case TOKEN_BLUE:
				LookaheadToken = this->ReadDecimalValue(TOKEN_BLUE, Blue, LookaheadToken);
				break;
			case ',':
				LookaheadToken = Accept(',', LookaheadToken);
				break;
			case ')':
				loop = 0;
				LookaheadToken = Accept(LookaheadToken, ')');
				break;
			default:
				loop = 0;
				UnexpectedToken(LookaheadToken);
				LookaheadToken = -1;	//error token
				break;
		}

	}
	if (LookaheadToken >= 0)c = RGB(Red, Green, Blue);
	return LookaheadToken;
}

int CLexer::Size(int Token,CSize &s, int LookAheadToken)
{
	int x, y;

	LookAheadToken = Accept(Token, LookAheadToken);
	if (LookAheadToken >= 0)LookAheadToken = Accept('(', LookAheadToken);
	if (LookAheadToken >= 0)x = atoi(m_LexBuff);
	if (LookAheadToken >= 0)LookAheadToken = Accept(TOKEN_NUM, LookAheadToken);
	if (LookAheadToken >= 0)LookAheadToken = Accept(',', LookAheadToken);
	if (LookAheadToken >= 0)y = atoi(m_LexBuff);
	if (LookAheadToken >= 0)LookAheadToken = Accept(TOKEN_NUM, LookAheadToken);
	if (LookAheadToken >= 0)LookAheadToken = Accept(')', LookAheadToken);
	if (LookAheadToken >= 0)s = CSize(x, y);
	return LookAheadToken;
}

int CLexer::ReadDecimalValue(int typetoken, int &v, int Token)
{
	Token = Accept(typetoken, Token);
	if (Token >= 0)Token = Accept('(', Token);
	if (Token >= 0)v = atoi(m_LexBuff);
	if (Token >= 0)Token = Accept(TOKEN_NUM, Token);
	if (Token >= 0)Token = Accept(')', Token);
	return Token;
}

int CLexer::String(char *&s, int Token)
{
	s = new char[strlen(m_LexBuff) + 1];
	strcpy(s, m_LexBuff);
	Token = Accept(TOKEN_STRING, Token);
	return Token;
}

void CLexer::UnexpectedToken(int Token)
{
	TCHAR *pStr;
	pStr = (TCHAR *)new char[256];
	StringCbPrintf(pStr, 256, LPCTSTR(L"Unexpected Token %s"), TokeToString(Token));
	::MessageBox(NULL, pStr, LPCTSTR(L"Syntax Error"),MB_OK | MB_ICONHAND);
	delete[] pStr;
}
