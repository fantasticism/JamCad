#include "stdafx.h"
#include "CadLine.h"
#include "JamCad.h"
#include "JamCadDefines.h"
#include "CadPolygon.h"
#include <strsafe.h>
#include "DlgLineAttributes.h"

int CCadLine::m_RenderEnable = 1;


void CCadLine::LoadSettings(LineAttr *pAttrib)
{
	pAttrib->m_Color = AfxGetApp()->GetProfileInt(LPCTSTR("LINE"), LPCTSTR("COLOR"), (int)RGB(0, 0, 0));
	pAttrib->m_Width = AfxGetApp()->GetProfileInt(LPCTSTR("LINE"), LPCTSTR("WIDTH"), 10);
}

void CCadLine::SaveSettings(LineAttr *pAttrib)
{
	AfxGetApp()->WriteProfileInt(LPCTSTR("LINE"), LPCTSTR("COLOR"), pAttrib->m_Color);
	AfxGetApp()->WriteProfileInt(LPCTSTR("LINE"), LPCTSTR("WIDTH"), pAttrib->m_Width);
}

CCadLine::CCadLine():CCadObject(OBJECT_TYPE_LINE)
{
	m_attrib.m_Width = 1;
	m_attrib.m_Color = RGB(0, 0, 0);	//black line
	m_pLinePen = 0;
	m_pPoly = 0;
}

CCadLine::CCadLine(CCadLine &line) :CCadObject(OBJECT_TYPE_LINE)
{
	m_attrib.m_Width = 1;
	m_attrib.m_Color = line.m_attrib.m_Color;
	m_pLinePen = 0;
	m_pPoly = 0;
	m_P1 = line.m_P1;
	m_P2 = line.m_P2;
}


CCadLine::~CCadLine()
{
	if (m_pLinePen) delete m_pLinePen;
	if (m_pPoly) delete m_pPoly;
}

void CCadLine::Move(CPoint p)
{
	//***************************************************
	//	Move
	//		This function is used to move the object
	// by the amount that is passed.
	//
	// parameters:
	//	p.......amount to move the object by
	//
	// return value: none
	//--------------------------------------------------
	CPoint Diff;
	Diff = p - m_P1;
	m_P1 += Diff;
	m_P2 += Diff;
}

void CCadLine::Save(FILE * pO, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This function save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	fprintf(pO, "%s%s(%s(%d,%d),%s(%d,%d),%s(RED(%d),GREEN(%d),BLUE(%d)),%s(%d))",
		MkIndentString(Indent, ' '),
		TokeToString(TOKEN_LINE),
		TokeToString(TOKEN_POINT),
		m_P1.x,m_P1.y,
		TokeToString(TOKEN_POINT),
		m_P2.x,m_P2.y,
		TokeToString(TOKEN_COLOR),
		RED(m_attrib.m_Color),GREEN(m_attrib.m_Color),BLUE(m_attrib.m_Color),
		TokeToString(TOKEN_WIDTH),
		m_attrib.m_Width);
}


void CCadLine::SetVertex(int Vi, CPoint p)
{
	//***************************************************
	// SetVertex
	//	This function is used to change the position of
	// a vertex.
	//
	// parameters:
	// v......index of the vertex
	// p......Amnount to change the vertex by
	//
	// return value: none
	//--------------------------------------------------
	if (Vi)
	{
		m_P2 = p;
	}
	else
		m_P1 = p;
}


int CCadLine::GrabVertex(CPoint point)
{
	//***************************************************
	// GrabVertex
	//	This function checks for a vertex at point p
	//
	// parameters:
	//	point.....point to check for presence of a vertex
	//
	// return value:
	//	returns index of vertex if succesful
	//	returns -1 on fail
	//--------------------------------------------------
	int rV = -1;
	CRect rect;
	CPoint p1, p2, p;
	p = CPoint(4, 4);
	p1 = m_P1 + p;
	p2 = m_P1 - p;
	rect.SetRect(p1, p2);
	rect.NormalizeRect();
	if (rect.PtInRect(point))
		rV = 0;
	else
	{
		p1 = m_P2 + p;
		p2 = m_P2 - p;
		rect.SetRect(p1, p2);
		rect.NormalizeRect();
		if (rect.PtInRect(point))
			rV = 1;
	}
	return rV;
}


void CCadLine::Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale)
{
	//***************************************************
	// Draw
	//	This function draws the document to the device
	// parameters:
	//	pDC.....pointer to the device context
	//	mode....drawing mode
	//	Offset..Offset to draw objects at
	//	Scale..Scale factor to draw objects at
	//
	// return value:none
	//--------------------------------------------------
	CPen *pOld;
	CPoint P1, P2, Diff;
	int Lw;
	CRect rect;

	if (CCadLine::m_RenderEnable)
	{
		P1 = (Scale * m_P1) + Offset;
		P2 = (Scale * m_P2) + Offset;
		Lw = int(Scale.m_ScaleX * m_attrib.m_Width);
		if (Lw < 1) Lw = 1;
		if (!IsLastModeSame(mode) || IsDirty())
		{
			//----------------------------------
			// If mode changed, get rid of pen
			// and make a new one
			//----------------------------------
			if (m_pLinePen) delete m_pLinePen;
			switch (mode)
			{
				case OBJECT_MODE_FINAL:
					m_pLinePen = new CPen(PS_SOLID, Lw, m_attrib.m_Color);
					break;
				case OBJECT_MODE_SELECTED:
					m_pLinePen = new CPen(PS_SOLID, 1, m_attrib.m_Color);
					break;
				case OBJECT_MODE_SKETCH:
					m_pLinePen = new CPen(PS_DOT, 1, m_attrib.m_Color);
					break;
			}
			SetLastMode(mode);
			SetDirty(0);
		}
		switch (mode)
		{
			case OBJECT_MODE_FINAL:
				pOld = pDC->SelectObject(m_pLinePen);
				pDC->MoveTo(P1);
				pDC->LineTo(P2);
				pDC->SelectObject(pOld);
				break;
			case OBJECT_MODE_SELECTED:
				pOld = pDC->SelectObject(m_pLinePen);
				Diff = CPoint(4, 4);

				rect.SetRect(P1 + (-Diff), P1 + Diff);
				pDC->Rectangle(&rect);
				rect.SetRect(P2 + (-Diff), P2 + Diff);
				pDC->Rectangle(&rect);
				pDC->SelectObject(m_pLinePen);
				pDC->MoveTo(P1);
				pDC->LineTo(P2);
				pDC->SelectObject(pOld);
				break;
			case OBJECT_MODE_SKETCH:
				Diff = CPoint(4, 4);
				pOld = pDC->SelectObject(m_pLinePen);
				pDC->MoveTo(P1);
				pDC->LineTo(P2);
				pDC->SelectObject(pOld);
				break;
		}	//end of switch(mode)
	}	//end of if(rederEnabled)
}

void CCadLine::EncliseLineInPolygon(CSize Offset, int w)
{
	if (m_pPoly == 0)
		m_pPoly = new CCadPolygon(4);
	else
		m_pPoly->Reset();

	//-----------------------------
	// enclose the line inside
	// of a polygon
	//-----------------------------
	CPoint P1 = m_P1 + Offset;
	CPoint P2 = m_P2 + Offset;
	int d = this->m_attrib.m_Width / 2 + w;
	if (d < 10) d = 10;
	if ((P1.x < P2.x) && (P1.y < P2.y))
	{
		m_pPoly->AddPoint(CPoint(P1.x - d, P1.y + d));
		m_pPoly->AddPoint(CPoint(P1.x + d, P1.y - d));
		m_pPoly->AddPoint(CPoint(P2.x + d, P2.y - d));
		m_pPoly->AddPoint(CPoint(P2.x - d, P2.y + d));
	}
	else if ((P1.x > P2.x) && (P1.y < P2.y))
	{
		m_pPoly->AddPoint(CPoint(P1.x - d, P1.y - d));
		m_pPoly->AddPoint(CPoint(P1.x + d, P1.y + d));
		m_pPoly->AddPoint(CPoint(P2.x + d, P2.y + d));
		m_pPoly->AddPoint(CPoint(P2.x - d, P2.y - d));
	}
	else if ((P1.x < P2.x) && (P1.y > P2.y))
	{
		m_pPoly->AddPoint(CPoint(P1.x - d, P1.y - d));
		m_pPoly->AddPoint(CPoint(P1.x + d, P1.y + d));
		m_pPoly->AddPoint(CPoint(P2.x + d, P2.y + d));
		m_pPoly->AddPoint(CPoint(P2.x - d, P2.y - d));
	}
	else if ((P1.x > P2.x) && (P1.y > P2.y))
	{
		m_pPoly->AddPoint(CPoint(P1.x - d, P1.y + d));
		m_pPoly->AddPoint(CPoint(P1.x + d, P1.y - d));
		m_pPoly->AddPoint(CPoint(P2.x + d, P2.y - d));
		m_pPoly->AddPoint(CPoint(P2.x - d, P2.y + d));
	}
	else if ((P1.x == P2.x) && (P1.y < P2.y))
	{
		m_pPoly->AddPoint(CPoint(P1.x - d, P1.y - d));
		m_pPoly->AddPoint(CPoint(P1.x + d, P1.y - d));
		m_pPoly->AddPoint(CPoint(P2.x + d, P2.y + d));
		m_pPoly->AddPoint(CPoint(P2.x - d, P2.y + d));
	}
	else if ((P1.x == P2.x) && (P1.y > P2.y))
	{
		m_pPoly->AddPoint(CPoint(P1.x - d, P1.y + d));
		m_pPoly->AddPoint(CPoint(P1.x + d, P1.y + d));
		m_pPoly->AddPoint(CPoint(P2.x + d, P2.y - d));
		m_pPoly->AddPoint(CPoint(P2.x - d, P2.y - d));
	}
	else if ((P1.x > P2.x) && (P1.y == P2.y))
	{
		m_pPoly->AddPoint(CPoint(P1.x + d, P1.y + d));
		m_pPoly->AddPoint(CPoint(P1.x + d, P1.y - d));
		m_pPoly->AddPoint(CPoint(P2.x - d, P2.y - d));
		m_pPoly->AddPoint(CPoint(P2.x - d, P2.y + d));
	}
	else if ((P1.x < P2.x) && (P1.y == P2.y))
	{
		m_pPoly->AddPoint(CPoint(P1.x - d, P1.y + d));
		m_pPoly->AddPoint(CPoint(P1.x - d, P1.y - d));
		m_pPoly->AddPoint(CPoint(P2.x + d, P2.y - d));
		m_pPoly->AddPoint(CPoint(P2.x + d, P2.y + d));
	}
}

int CCadLine::CheckSelected(CPoint p, CSize Offset, CCadObject ** ppSelList , int index, int n, int flag)
{
	//***************************************************
	// CheckSelected
	//	This function is used to see if an object can
	// be selected at point p.
	//
	// parameters:
	//	p...........point to check at
	//	Offset......Offset of drawing
	//	ppSelList...pointer to list of selected objects
	//	index.......current index into the selection list
	//	n...........Total number of spaces in slection list
	//	flag........Determines what sort of objects selected
	//
	// return value:
	//	returns true if point is within object
	//	otherwise, false
	//--------------------------------------------------
	if (index < n || n == 0)
	{
		//---------------------------------------------
		// enclose line in a polygon
		//-----------------------------------------------
		EncliseLineInPolygon(Offset, 10);
		//-----------------------------------------------
		// is the point inside the polygon?
		//-----------------------------------------------
		if (m_pPoly->PointEnclosed(p - Offset))
		{
			if (ppSelList)
			{
				switch (flag)
				{
					case CADDRAWUBG_CHECKSEL_FLAG_ALL:
						ppSelList[index++] = this;
						break;
					case CADDRAWUBG_CHECKSEL_FLAG_UNSEL:
						if (!IsSelected())
							ppSelList[index++] = this;
						break;
					case CADDRAWUBG_CHECKSEL_FLAG_SEL:
						if (IsSelected())
							ppSelList[index++] = this;
						break;
				}
			}
			else
			{
				switch (flag)
				{
					case CADDRAWUBG_CHECKSEL_FLAG_ALL:
						index = 1;
						break;
					case CADDRAWUBG_CHECKSEL_FLAG_UNSEL:
						if (!IsSelected())
							index = 1;
						break;
					case CADDRAWUBG_CHECKSEL_FLAG_SEL:
						if (IsSelected())
							index = 1;
						break;
				}

			}
		}
	}
	return index;
}

void CCadLine::AdjustRefernce(CPoint Ref)
{
	//***************************************************
	// AdjustRefernce
	//	Change the reference point for an object.  This
	// operation needs to change everything else that
	// is referenced to this ppoint as well.
	// parameters:
	//	Ref.......How much to change reference by
	//
	// return value:
	//--------------------------------------------------
	m_P1 -= Ref;
	m_P2 -= Ref;
}

char * CCadLine::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	return (char *)"Line";
}

CCadLine CCadLine::operator=(CCadLine &v)
{
	//***************************************************
	// operator=
	//		Provides the functionality when one object
	// value is assigned to another
	// parameters:
	//	v......reference to object to get value(s) from
	//
	// return value:this
	//--------------------------------------------------
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_attrib.m_Color = v.GetColor();
	m_attrib.m_Width = v.GetWidth();
	return *this;
}

CCadObject * CCadLine::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadLine *pCL = new CCadLine;
	*pCL = *this;
	return pCL;
}

void CCadLine::RenderEnable(int e)
{
	//***************************************************
	// RenderEnable
	//	chhanges the state of the render enable flag.
	// The base class does not contain this flag.
	// The render enable flag is a static member of
	// the derived class.
	// parameters:
	//	e......new state of enable flag
	//
	// return value:
	//--------------------------------------------------
	CCadLine::m_RenderEnable = e;
}

CPoint CCadLine::GetCenter()
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	int x, y;
	x = (m_P1.x + m_P2.x) / 2;
	y = (m_P1.y + m_P2.y) / 2;
	return CPoint(x,y);
}

void CCadLine::ChangeCenter(CSize p)
{
	//***************************************************
	// ChangeCenter
	//	Change the center position of the object
	// parameters:
	//	p......amount to change center by
	//
	// return value:
	//--------------------------------------------------

}

CSize CCadLine::GetSize()
{
	//***************************************************
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	return CSize(abs(m_P1.x - m_P2.x), abs(m_P1.y - m_P2.y));
}

void CCadLine::ChangeSize(CSize Sz)
{
	//***************************************************
	// ChangeSize
	//	Change the size of the object
	// parameters:
	//	sz.....size to change object by (not change to)
	// return value:
	//--------------------------------------------------
	m_P2 += Sz;
}

int CCadLine::Parse(int Token, CLexer *pLex)
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
	Token = pLex->Accept(Token, TOKEN_LINE);
	Token = pLex->Accept(Token, '(');
	Token = pLex->Point(TOKEN_POINT, m_P1, Token);
	Token = pLex->Accept(Token, ',');
	Token = pLex->Point(TOKEN_POINT, m_P2, Token);
	int loop = 1;
	while (loop)
	{
		switch (Token)
		{
			case TOKEN_COLOR:
				Token = pLex->Color(TOKEN_COLOR,m_attrib.m_Color, Token);
				break;
			case TOKEN_WIDTH:
				Token = pLex->ReadDecimalValue(TOKEN_WIDTH, m_attrib.m_Width, Token);
				break;
			case ',':
				Token = pLex->Accept(Token, ',');
				break;
			case ')':
				loop = 0;
				Token = pLex->Accept(Token, ')');
				break;
			default:
				loop = 0;
				pLex->UnexpectedToken(Token);
				Token = -1;	//error token
				break;
		}
	}
	return Token;
}

CCadObject * CCadLine::NewInstance()
{
	//***************************************************
	// NewInstance
	//	This function makes a copy of this object, then
	// puts a pointer to the new object into this objects
	// m_pNextInstance pointer.
	//
	// parameters:none
	//
	// return value:returns pointer to new object.
	//--------------------------------------------------
	CCadLine *pCL = new CCadLine;	//create the new object
	*pCL = *this;	//copy it over;
	
	return pCL;
}

void CCadLine::GetAttributes(LineAttr *pAtr)
{
	pAtr->m_Color = m_attrib.m_Color;
	pAtr->m_Width = m_attrib.m_Width;
}

void CCadLine::CopyAttributes(LineAttr *pAtr)
{
	m_attrib.m_Color = pAtr->m_Color;
	m_attrib.m_Width = pAtr->m_Width;
	ClearNeedsAttributes();
}

int CCadLine::MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
{
	//-------------------------------------------------------
	//	MouseDown
	//		This is the state machine for creating this
	//	object on the screen.  This function is for when
	//	the left mouse button goes down.
	//
	//	parameters:
	//		pASV......pointer to view that is creating object
	//		DrawState.Current state of drawing process
	//		pAdrbData.pointer to default attribute data for object
	//
	//	Returns:
	//		Next Draw State
	//-------------------------------------------------------

	switch (DrawState)
	{
	case DRAWSTATE_SET_ATTRIBUTES:
		CopyAttributes((LineAttr *)pAdrbData);
		break;
	case DRAWSTATE_WAITFORMOUSE_DOWN:
		if(NeedsAttributes()) 
			CopyAttributes((LineAttr *)pAdrbData);
		m_P1 = m_P2 = pASV->m_SnapPos;
		break;
	case DRAWSTATE_PLACE:
		m_P2 = pASV->m_SnapPos;
		break;
	}
	return DrawState;
}

int CCadLine::MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
{
	//-------------------------------------------------------
	// MouseUp
	//		This is the state machine for creating this
	//	object on the screen.  This function is for when
	//	the left mouse button goes up.
	//
	//	parameters:
	//		pASV......pointer to view that is creating object
	//		DrawState.Current state of drawing process
	//
	//	Returns:
	//		Next Draw State
	//-------------------------------------------------------
	CDlgLineAttributes Dlg;
	int Id;

	switch (DrawState)
	{
		case DRAWSTATE_SET_ATTRIBUTES:
			theApp.UpdateStatusBar("LINE:Set Line Attributes");
			GetAttributes(&Dlg.m_Attrb);
			if ((Id = Dlg.DoModal()) == IDOK)
			{
				CopyAttributes(&Dlg.m_Attrb);
				DrawState = DRAWSTATE_WAITFORMOUSE_DOWN;
				theApp.UpdateStatusBar("LINE:Place First Point");
			}
			break;
		case DRAWSTATE_WAITFORMOUSE_DOWN:
			m_P1 = m_P2 = pASV->m_SnapPos;
			pASV->EnableAutoScroll(1);
			DrawState = DRAWSTATE_PLACE;;
			break;
		case DRAWSTATE_PLACE:
			m_P2 = pASV->m_SnapPos;
			GetAttributes((LineAttr *)pAdrbData);
			pASV->AddObjectIntoDoc(this);
			pASV->m_CadObj.pCadObject = 0;
			DrawState = DRAWSTATE_WAITFORMOUSE_DOWN;
			pASV->EnableAutoScroll(0);
			theApp.UpdateStatusBar("LINE:Place First Point");
			pASV->Invalidate();
			break;
	}
	return DrawState;
}

int CCadLine::MouseMove(CAutoScrollView * pASV, int DrawState)
{
	//-------------------------------------------------------
	// MouseMove
	//		This is the state machine for creating this
	//	object on the screen.  This function is for when
	//	the left mouse is moved.
	//
	//	parameters:
	//		pASV......pointer to view that is creating object
	//		DrawState.Current state of drawing process
	//
	//	Returns:
	//		Next Draw State
	//-------------------------------------------------------
	switch (DrawState)
	{
	case DRAWSTATE_SET_ATTRIBUTES:
		break;
	case DRAWSTATE_WAITFORMOUSE_DOWN:
		break;
	case DRAWSTATE_PLACE:
		m_P2 = pASV->m_SnapPos;
		break;
	}
	return DrawState;
}

int CCadLine::EditProperties(void)
{
	CDlgLineAttributes Dlg;
	int Id;
	GetAttributes(&Dlg.m_Attrb);
	if ((Id = Dlg.DoModal()) == IDOK)
	{
		CopyAttributes(&Dlg.m_Attrb);
	}
	return Id;
}