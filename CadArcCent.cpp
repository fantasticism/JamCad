#include "stdafx.h"
#include "CadArcCent.h"
#include "JamCadDefines.h"
#include "JamCad.h"
#include "DlgArcCentProperies.h"

int CCadArcCent::m_RenderEnable = 1;

void CCadArcCent::LoadSettings(ArcCentAttributes *pAttrib)
{
	/************************************************
	* LoadSetting
	*	This function loads the default settings
	* for this object.
	*
	* Parameter:
	*	pAttrib......pointer to attribute structure
	*				to load settings into
	************************************************/
	pAttrib->m_Color = theApp.GetProfileIntA("ARCCENT", "COLOR", RGB(0, 0, 0));
	pAttrib->m_Width = theApp.GetProfileIntA("ARCCENT", "WIDTH", 60);
}

void CCadArcCent::SaveSettings(ArcCentAttributes *pAttrib)
{
	/************************************************
	* SaveSettings
	*	This function saves the default settings
	* for this object to the registry.
	*
	* Parameter:
	*	pAttrib......pointer to attribute structure
	*				to save settings from
	************************************************/
	theApp.WriteProfileInt("ARCCENT", "COLOR", pAttrib->m_Color);
	theApp.WriteProfileInt("ARCCENT", "WIDTH", pAttrib->m_Width);
}


CCadArcCent::CCadArcCent():CCadObject(OBJECT_TYPE_ARCCENTERED)
{
	m_pPenLine = 0;
}

CCadArcCent::CCadArcCent(CCadArcCent &arc) : CCadObject(OBJECT_TYPE_ARCCENTERED)
{
	m_P1 = arc.m_P1;
	m_P2 = arc.m_P2;
	m_Attrb.m_End = arc.m_Attrb.m_End;
	m_Attrb.m_EndAngle = arc.m_Attrb.m_EndAngle;
	m_Attrb.m_Color = arc.m_Attrb.m_Color;
	m_Attrb.m_Start = arc.m_Attrb.m_Start;
	m_Attrb.m_StartAngle = arc.m_Attrb.m_StartAngle;
	m_Attrb.m_Width = arc.m_Attrb.m_Width;
	m_pPenLine = 0;
}


CCadArcCent::~CCadArcCent()
{
	if (m_pPenLine) delete m_pPenLine;
}


void CCadArcCent::Move(CPoint p)
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
	CPoint Diff = p - m_P1;
	m_P1 += Diff;
	m_P2 += Diff;
	m_Attrb.m_Start += Diff;
	m_Attrb.m_End += Diff;
}

void CCadArcCent::Save(FILE * pO, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This function save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	fprintf(pO, "%s%s(%s(%d,%d),%s(%d,%d),%s(%d,%d),%s(%d,%d),%s(%d,%d,%d),%s(%d))\n",
		MkIndentString(Indent, ' '),
		TokeToString(TOKEN_ARCCENTERED),
		TokeToString(TOKEN_POINT),
		m_P1.x, m_P1.y,
		TokeToString(TOKEN_POINT),
		m_P2.x, m_P2.y,
		TokeToString(TOKEN_POINT),
		m_Attrb.m_Start.x, m_Attrb.m_Start.y,
		TokeToString(TOKEN_POINT),
		m_Attrb.m_End.x, m_Attrb.m_End.y,
		TokeToString(TOKEN_COLOR),
		RED(m_Attrb.m_Color),
		GREEN(m_Attrb.m_Color),
		BLUE(m_Attrb.m_Color),
		TokeToString(TOKEN_WIDTH),
		m_Attrb.m_Width);
}

void CCadArcCent::SetVertex(int v, CPoint p)
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
}


int CCadArcCent::GrabVertex(CPoint p, CScale scale)
{
	//***************************************************
	// GrabVertex
	//	This function checks for a vertex at point p
	//
	// parameters:
	//	p.....point to check for presence of a vertex
	//	scale....scale factor
	//
	// return value:
	//	returns index of vertex if succesful
	//	returns -1 on fail
	//--------------------------------------------------
	return -1;
}


void CCadArcCent::Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale)
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
	CRect rect;
	CSize rectLWcomp;
	CPoint p1, p2;
	int dx, dy;
	CPoint P1, P2, Start, End;
	int Lw;

	if (CCadArcCent::m_RenderEnable)
	{
		P1 = Scale * m_P1 + Offset;
		P2 = Scale * m_P2 + Offset;
		Start = Scale * m_Attrb.m_Start + Offset;
		End = Scale * m_Attrb.m_End + Offset;
		Lw = int(m_Attrb.m_Width * Scale.m_ScaleX);
		if (Lw <= 1 || OBJECT_MODE_SKETCH == mode)
		{
			Lw = 1;
			rectLWcomp = CSize(0, 0);
		}
		else
			rectLWcomp = CSize(Lw / 2, Lw / 2);

		if (P1.x >= P2.x)
		{
			if (P1.y > P2.y)
			{
				dx = P1.x - P2.x;
				dy = P1.y - P2.y;
			}
			else if (P1.y == P2.y)
			{
				dx = P1.x - P2.x;
				dy = P1.y - P2.y;
			}
			else
			{
				dx = P1.x - P2.x;
				dy = P2.y - P1.y;
			}
		}
		else
		{
			if (P1.y > P2.y)
			{
				dx = P2.x - P1.x;
				dy = P1.y - P2.y;
			}
			else if (P1.y == P2.y)
			{
				dx = P2.x - P1.x;
				dy = P1.y - P2.y;
			}
			else
			{
				dx = P2.x - P1.x;
				dy = P2.y - P1.y;
			}
		}
		p1 = P1 + CPoint(-dx, -dy);
		p2 = P1 + CPoint(dx, dy);
		SetRect(rect, p1, p2, rectLWcomp);
		//	rect.SetRect(p1,p2);
		if ((!IsLastModeSame(mode)) || IsDirty())
		{
			if (m_pPenLine) delete m_pPenLine;
			switch (mode)
			{
			case OBJECT_MODE_FINAL:
				m_pPenLine = new CPen(PS_SOLID, Lw, m_Attrb.m_Color);
				break;
			case OBJECT_MODE_SELECTED:
				m_pPenLine = new CPen(PS_SOLID, Lw, RGB(200, 50, 50));
				break;
			case OBJECT_MODE_SKETCH:
				m_pPenLine = new CPen(PS_DOT, 1, m_Attrb.m_Color);
				break;
			case OBJECT_MODE_ARCSTART:
			case OBJECT_MODE_ARCEND:
				m_pPenLine = new CPen(PS_DOT, 1, m_Attrb.m_Color);
				break;
			}
			SetDirty(0);
		}
		switch (mode)
		{
		case OBJECT_MODE_FINAL:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Arc(&rect, Start, End);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_SELECTED:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Arc(&rect, Start, End);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_SKETCH:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Rectangle(&rect);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_ARCSTART:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Ellipse(&rect);
			pDC->MoveTo(P1);
			pDC->LineTo(Start);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_ARCEND:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Arc(&rect, Start, End);
			pDC->MoveTo(P1);
			pDC->LineTo(End);
			pDC->SelectObject(pOld);
			break;
		}
		SetLastMode(mode);
	}
}

int CCadArcCent::PtInArc(CPoint p, CSize Offset)
{
	double StartAngle, EndAngle, Angle;
	int rV = 0;
	CPoint P1, P2, Start, End;

	P1 = m_P1 + Offset;
	P2 = m_P2 + Offset;
	Start = m_Attrb.m_Start + Offset;
	End = m_Attrb.m_End + Offset;
	StartAngle = ArcTan(double(Start.x - P1.x), double(P1.y - Start.y));
	EndAngle = ArcTan(double(End.x - P1.x), double(P1.y - End.y));
	Angle = ArcTan(double(p.x - P1.x), double(P1.y - p.y));
	rV = CheckAngle(StartAngle, EndAngle, Angle);

	/// 1 = x^2/A^2 + y^2/B^2
	double W = double(GetWidth()) / 2.0;
	if (W < 50.0) W = 50.0;
	double A, B;
	double X, Y, X1, Y1, X2, Y2;
	A = double(P2.x - P1.x);
	B = double(P2.y - P1.y);
	X = double(p.x) - double(P1.x);
	Y = double(p.y) - double(P1.y);
	X1 = double(P2.x) - double(P1.x) + W;
	Y1 = 0.0;
	X2 = double(P2.x) - double(P1.x) - W;
	Y2 = 0.0;
	double Rad = (X*X) / (A*A) + (Y*Y) / (B*B);
	double Rad1 = (X1*X1) / (A*A) + (Y1 * Y1) / (B*B);
	double Rad2 = (X2*X2) / (A*A) + (Y2 * Y2) / (B*B);
	if (Rad < Rad2 || Rad > Rad1) rV = 0;

	return rV;
}


int CCadArcCent::CheckSelected(CPoint p, CSize Offset, CCadObject ** ppSelList , int index, int n, int flag)
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
		//---------------------------------------
		// is point in Ellipse
		//---------------------------------------
		if (PtInArc(p - Offset))
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

CPoint CCadArcCent::GetReference()
{
	//***************************************************
	// GetReference
	//	This function returns the reference point for
	// the object
	// parameters:none
	//
	// return value:reference point
	//--------------------------------------------------
	return m_P1;
}

void CCadArcCent::AdjustRefernce(CPoint Ref)
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
	m_Attrb.m_Start -= Ref;
	m_Attrb.m_End -= Ref;
}

CRect CCadArcCent::GetRect(void)
{
	//***************************************************
	// GetRect
	//	Returns the rectangle that will enclose the
	// the object
	// parameters:
	//
	// return value:Returns the rectangle that encloses
	// the object
	//--------------------------------------------------
	CRect rect;
	CSize Diff;
	Diff = m_P1 - m_P2;
	rect.SetRect(m_P1 + Diff, m_P1 - Diff);
	rect.NormalizeRect();
	return rect;
}

char * CCadArcCent::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	return (char *)"Arc Centered";
}

CCadArcCent CCadArcCent::operator=(CCadArcCent &v)
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
	m_Attrb.m_Color = v.m_Attrb.m_Color;
	m_Attrb.m_End = v.m_Attrb.m_End;
	m_Attrb.m_Start = v.m_Attrb.m_Start;
	m_Attrb.m_Width = v.m_Attrb.m_Width;
	return *this;
}

CPoint CCadArcCent::ScalePoint(CPoint p, int Scale, int ScaleDiv)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return CPoint();
}

CCadObject * CCadArcCent::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadArcCent *pArc = new CCadArcCent;
	*pArc = *this;
	pArc->CopyAttributes(&m_Attrb);
	return pArc;
}

void CCadArcCent::RenderEnable(int e)
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
	CCadArcCent::m_RenderEnable = e;
}

CPoint CCadArcCent::GetCenter()
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	return m_P1;
}

void CCadArcCent::ChangeCenter(CSize p)
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

CSize CCadArcCent::GetSize()
{
	//***************************************************
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	CRect rect = GetRect();
	return rect.Size();
}

void CCadArcCent::ChangeSize(CSize Sz)
{
	//***************************************************
	// ChangeSize
	//	Change the size of the object
	// parameters:
	//	sz.....size to change object by (not change to)
	// return value:
	//--------------------------------------------------
	m_P2.x += Sz.cx;
	m_P2.y += Sz.cy;
}

int CCadArcCent::Parse(int Token, CLexer *pLex)
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
	return 0;
}

CCadObject * CCadArcCent::NewInstance()
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
	return 0;
}

void CCadArcCent::GetAttributes(ArcCentAttributes *pAttrb)
{
	/***************************************************
	*	GetAttributes
	*		This function is used to copy the
	*	attributes from this object into one pointed
	*	to by the parameter.
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy into
	***************************************************/
	pAttrb->m_Color = m_Attrb.m_Color;
	pAttrb->m_End = m_Attrb.m_End;
	pAttrb->m_EndAngle = m_Attrb.m_EndAngle;
	pAttrb->m_Start = m_Attrb.m_Start;
	pAttrb->m_StartAngle = m_Attrb.m_StartAngle;
	pAttrb->m_Width = m_Attrb.m_Width;
}

void CCadArcCent::CopyAttributes(ArcCentAttributes *pAttrb)
{
	/***************************************************
	*	CopyAttributes
	*		This function is used to copy the
	*	attributes pointed to by the parameter into
	*	this object
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy
	***************************************************/
	m_Attrb.m_Color = pAttrb->m_Color;
	m_Attrb.m_End = pAttrb->m_End;
	m_Attrb.m_EndAngle = pAttrb->m_EndAngle;
	m_Attrb.m_Start = pAttrb->m_Start;
	m_Attrb.m_StartAngle = pAttrb->m_StartAngle;
	m_Attrb.m_Width = pAttrb->m_Width;
	ClearNeedsAttributes();

}

int CCadArcCent::MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
		m_P2 = m_P1 = pASV->m_SnapPos;
		CopyAttributes((ArcCentAttributes *)pAdrbData);
		break;
	case DRAWSTATE_WAITFORMOUSE_DOWN:
		m_P2 = m_P1 = pASV->m_SnapPos;
		if(NeedsAttributes())
			CopyAttributes((ArcCentAttributes *)pAdrbData);
		break;
	case DRAWSTATE_PLACE:
		m_P2 = pASV->m_SnapPos;
		break;
	case DRAWSTATE_ARCSTART:
		m_Attrb.m_Start = pASV->m_SnapPos;
		break;
	case DRAWSTATE_ARCEND:
		m_Attrb.m_End = pASV->m_SnapPos;
		break;
	}
	return DrawState;
}

int CCadArcCent::MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
	CDlgArcCentProperies Dlg;

	switch (DrawState)
	{
	case DRAWSTATE_SET_ATTRIBUTES:
		m_P1 = m_P2 = pASV->m_SnapPos;
		GetAttributes(&Dlg.m_Attrb);
		if (IDOK == Dlg.DoModal())
		{
			CopyAttributes(&Dlg.m_Attrb);
			DrawState = DRAWSTATE_WAITFORMOUSE_DOWN;
			theApp.UpdateStatusBar("ARC CENTERED:Place Center Point for Arc");
		}
		break;
	case DRAWSTATE_WAITFORMOUSE_DOWN:
		m_P1 = pASV->m_SnapPos;
		DrawState = DRAWSTATE_PLACE;
		pASV->EnableAutoScroll(1);
		theApp.UpdateStatusBar("ARC CENTERED:Place Corner of Circle");
		break;
	case DRAWSTATE_PLACE:
		m_P2 = pASV->m_SnapPos;
		DrawState = DRAWSTATE_ARCSTART;
		theApp.UpdateStatusBar("ARC CENTERED:Locate Start of Arc");
		break;
	case DRAWSTATE_ARCSTART:
		m_Attrb.m_Start = pASV->m_SnapPos;
		DrawState = DRAWSTATE_ARCEND;
		theApp.UpdateStatusBar("ARC CENTERED:Locate End of Arc");
		break;
	case DRAWSTATE_ARCEND:
		m_Attrb.m_End = pASV->m_SnapPos;
		pASV->AddObjectIntoDoc(this);
		GetAttributes((ArcCentAttributes *)pAdrbData);
		pASV->m_CadObj.pCadObject = 0;
		pASV->EnableAutoScroll(0);
		DrawState = DRAWSTATE_WAITFORMOUSE_DOWN;
		theApp.UpdateStatusBar("ARC CENTERED:Locate Center Point of Arc");
		pASV->Invalidate();
		break;
	}
	return DrawState;
}

int CCadArcCent::MouseMove(CAutoScrollView * pASV, int DrawState)
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
		m_P1 = m_P2 = pASV->m_SnapPos;
		break;
	case DRAWSTATE_WAITFORMOUSE_DOWN:
		m_P1 = m_P2 = pASV->m_SnapPos;
		break;
	case DRAWSTATE_PLACE:
		m_P2 = pASV->m_SnapPos;
		break;
	case DRAWSTATE_ARCSTART:
		m_Attrb.m_Start = pASV->m_SnapPos;
		break;
	case DRAWSTATE_ARCEND:
		m_Attrb.m_End = pASV->m_SnapPos;
		break;
	}
	pASV->Invalidate();
	return DrawState;
}


void CCadArcCent::SetStartAngle(int Angle)
{

}

void CCadArcCent::SetEndAngle(int Angle)
{

}

void CCadArcCent::SetStartPoint(CPoint p)
{
	m_Attrb.m_Start = p;
	//----------------------------
	// calculate the Angle
	// associated with this start
	// point
	//---------------------------
	double Pi = atan(1) * 4.0;
	double A = ArcTan(double(m_Attrb.m_Start.x) - double(m_P1.x), double(m_P1.y) - double(m_Attrb.m_Start.y));

	A *= 180.0 / Pi;
	m_Attrb.m_StartAngle = int(A * 10.0);
}

void CCadArcCent::SetEndPoint(CPoint p)
{
	m_Attrb.m_End = p;
	double Pi = atan(1) * 4.0;
	double A = ArcTan(double(m_Attrb.m_End.x) - double(m_P1.x), double(m_P1.y) - double(m_Attrb.m_End.y));
	A *= 180.0 / Pi;
	m_Attrb.m_EndAngle = int(A * 10.0);
}
