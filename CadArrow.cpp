#include "stdafx.h"
#include "CadArrow.h"
#include "CadPolygon.h"
#include "JamCad.h"
#include "DlgArrowProperties.h"

int CCadArrow::m_RenderEnable = 1;

void CCadArrow::LoadSettings(ArrowAttributes *pAttrib)
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
	pAttrib->m_L1 = 400;
	pAttrib->m_L2 = 100;
	pAttrib->m_Width = 200;
}

void CCadArrow::SaveSettings(ArrowAttributes *pAttrib)
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
}


CCadArrow::CCadArrow()
{
	m_Attrb.m_L1 = 0;
	m_Attrb.m_L2 = 0;
	m_Attrb.m_Width = 0;
	m_Attrb.m_Color = RGB(0, 0, 0);
	m_pPen = 0;
	m_pBrFill = 0;
}

CCadArrow::CCadArrow(CCadArrow &ca) :CCadObject(OBJECT_TYPE_ARROW)
{
	m_Attrb.m_L1 = ca.m_Attrb.m_L1;
	m_Attrb.m_L2 = ca.m_Attrb.m_L2;
	m_Attrb.m_Width = ca.m_Attrb.m_Width;
	m_Attrb.m_Color = ca.m_Attrb.m_Color;
	m_pPen = 0;
	m_pBrFill = 0;
	m_P1 = ca.m_P1;
	m_P2 = ca.m_P2;
}


CCadArrow::~CCadArrow()
{
	if (m_pPen) delete m_pPen;
	if (m_pBrFill) delete m_pBrFill;
}

void CCadArrow::Move(CPoint p)
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
	CSize Diff = p - m_P1;
	m_P1 += Diff;
	m_P2 += Diff;
}

void CCadArrow::Save(FILE * pO, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This function save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	fprintf(pO, "%s%s(%s(%d,%d),%s(%d,%d),%s(%d),%s(%d),%s(%d,%d,%d))\n",
		MkIndentString(Indent),
		TokeToString(TOKEN_ARROW),
		TokeToString(TOKEN_POINT),
		m_P1.x, m_P1.y,
		TokeToString(TOKEN_POINT),
		m_P2.x, m_P2.y,
		TokeToString(TOKEN_LENGTH),
		m_Attrb.m_L1, 
		TokeToString(TOKEN_WIDTH),
		m_Attrb.m_Width,
		TokeToString(TOKEN_COLOR),
		RED(m_Attrb.m_Color),
		GREEN(m_Attrb.m_Color),
		BLUE(m_Attrb.m_Color)
	);
}

void CCadArrow::SetVertex(int Vi, CPoint p)
{
	//***************************************************
	// SetVertex
	//	This function is used to change the position of
	// a vertex.
	//
	// parameters:
	// Vi......index of the vertex
	// p......Amnount to change the vertex by
	//
	// return value: none
	//--------------------------------------------------
	if (Vi)
		m_P2 += p - m_P2;
	else
		m_P1 += p - m_P1;
}


int CCadArrow::GrabVertex(CPoint p, CScale scale)
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


void CCadArrow::Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale)
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
	CPen *pOldPen;
	CBrush *pOldBr;
	CRect rect;
	CPoint P1, P2, P3, P4,P5;

	if (CCadArrow::m_RenderEnable)
	{
		P1 = Scale * m_P1 + Offset;
		P2 = Scale * m_P2 + Offset;
		if (mode == OBJECT_MODE_FINAL)
			CalcPoints(P3, P4,P5, Offset, Scale);
		else
			CalcPoints(P3, P4,P5, Offset, Scale);
		CPoint *pP = new CPoint[4];
		pP[0] = P1;
		pP[1] = P3;
		pP[2] = P5;
		pP[3] = P4;

		if ((!IsLastModeSame(mode)) || IsDirty())
		{
			SetDirty(0);
			switch (mode)
			{
			case OBJECT_MODE_FINAL:
				if (m_pPen) delete m_pPen;
				m_pPen = new CPen();
				m_pPen->CreatePen(PS_SOLID, 1, m_Attrb.m_Color);
				if (m_pBrFill) delete m_pBrFill;
				m_pBrFill = new CBrush(m_Attrb.m_Color);
				break;
			case OBJECT_MODE_SELECTED:
				if (m_pPen) delete m_pPen;
				m_pPen = new CPen();
				m_pPen->CreatePen(PS_SOLID, 1, m_Attrb.m_Color ^ 0x00ff00);
				if (m_pBrFill) delete m_pBrFill;
				m_pBrFill = new CBrush(m_Attrb.m_Color ^ 0x00ff00);
				break;
			case OBJECT_MODE_SKETCH:
				if (m_pPen) delete m_pPen;
				m_pPen = new CPen();
				m_pPen->CreatePen(PS_SOLID, 1, m_Attrb.m_Color);
				if (m_pBrFill) delete m_pBrFill;
				m_pBrFill = new CBrush(m_Attrb.m_Color);
				break;
			}
		}
		switch (mode)
		{
		case OBJECT_MODE_FINAL:
		case OBJECT_MODE_SELECTED:
			pOldPen = pDC->SelectObject(m_pPen);
			pOldBr = pDC->SelectObject(m_pBrFill);
			pDC->Polygon(pP, 4);
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBr);
			break;
		case OBJECT_MODE_SKETCH:
			pOldPen = pDC->SelectObject(m_pPen);
			pOldBr = pDC->SelectObject(m_pBrFill);
			pDC->Polygon(pP, 4);
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBr);
			break;
		}
		SetLastMode(mode);
	}
}

CCadPolygon *CCadArrow::EncloseArrowInPolygon(CSize Offset)
{
	CCadPolygon *Cp = new CCadPolygon(4);
	Cp->AddPoint(m_P1 + Offset);
	CPoint P3, P4, P5;
	CalcPoints(P3, P4, P5, Offset, CScale(1.0, 1.0));
	Cp->AddPoint(P3);
	Cp->AddPoint(P5);
	Cp->AddPoint(P4);
	return Cp;
}

int CCadArrow::CheckSelected(CPoint p, CSize Offset, CCadObject ** ppSelList , int index, int n, int flag)
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
	CCadPolygon *pCp;

	if (index < n || n == 0)
	{
		//---------------------------------------
		// enclise arrow in a polygon
		//---------------------------------------
		pCp = EncloseArrowInPolygon(Offset);
		//---------------------------------------
		// is point in polygon/arrow
		//---------------------------------------
		if (pCp->PointEnclosed(p - Offset))
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
		delete pCp;
	}
	return index;
}

CPoint CCadArrow::GetReference()
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

void CCadArrow::AdjustRefernce(CPoint Ref)
{
	//***************************************************
	// AdjustRefernce
	//	Change the reference point for an object.  This
	// operation needs to change everything else that
	// is referenced to this ppoint as well.
	// parameters:
	//	Ref.......How much to change reference by
	//
	// return value:none
	//--------------------------------------------------
	m_P1 -= Ref;
	m_P2 -= Ref;
}

CRect CCadArrow::GetRect(void)
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
	CRect r;
	int i;
	CPoint P3, P4, P5,Points[4];
	CalcPoints(P3, P4, P5, CPoint(0, 0), CScale(1.0, 1.0));
	Points[0] = m_P1;
	Points[1] = P3;
	Points[2] = P5;
	Points[3] = P4;
	int MinX, MinY, MaxX, MaxY;
	MinX = MaxX = m_P1.x;
	MinY = MaxY = m_P1.y;
	for (i = 1; i < 4; ++i)
	{
		if (MinX > Points[i].x) MinX = Points[i].x;
		if (MinY > Points[i].y) MinY = Points[i].y;
		if (MaxX < Points[i].x) MaxX = Points[i].x;
		if (MaxY < Points[i].y) MaxY = Points[i].y;
	}
	r.SetRect(MinX, MinY, MaxX, MaxY);
	r.NormalizeRect();
	return r;
}

char * CCadArrow::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	return (char *)"Arrow";
}

CCadArrow CCadArrow::operator=(CCadArrow &Ca)
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
	m_Attrb.m_Color = Ca.m_Attrb.m_Color;
	m_Attrb.m_L1 = Ca.m_Attrb.m_L1;
	m_Attrb.m_L2 = Ca.m_Attrb.m_L2;
	m_Attrb.m_Width = Ca.m_Attrb.m_Width;
	m_P1 = Ca.m_P1;
	m_P2 = Ca.m_P2;
	return *this;
}

CCadObject * CCadArrow::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadArrow *pArrow = new CCadArrow;
	*pArrow = *this;
	return pArrow;
}

void CCadArrow::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------

}

void CCadArrow::RenderEnable(int e)
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
	CCadArrow::m_RenderEnable = e;
}

CPoint CCadArrow::GetCenter()
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	CRect rect = this->GetRect();
	return rect.CenterPoint();
}

void CCadArrow::ChangeCenter(CSize p)
{
	//***************************************************
	// ChangeCenter
	//	Change the center position of the object
	// parameters:
	//	p......amount to change center by
	//
	// return value:
	//--------------------------------------------------
	m_P1 -= p;
	m_P2 -= p;
}

CSize CCadArrow::GetSize()
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

void CCadArrow::ChangeSize(CSize Sz)
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

int CCadArrow::Parse(int Token, CLexer *pLex)
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

CCadObject * CCadArrow::NewInstance()
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


void CCadArrow::GetAttributes(ArrowAttributes *pAttrb)
{
	/***************************************************
	*	GetAttributes
	*		This function is used to copy the
	*	attributes from this object into an
	*	attributes structure pointed to by the parameter
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy into
	***************************************************/
	pAttrb->m_Color = m_Attrb.m_Color;
	pAttrb->m_L1 = m_Attrb.m_L1;
	pAttrb->m_L2 = m_Attrb.m_L2;
	pAttrb->m_Width = m_Attrb.m_Width;
}

void CCadArrow::CopyAttributes(ArrowAttributes *pAttrb)
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
	m_Attrb.m_L1 = pAttrb->m_L1;
	m_Attrb.m_L2 = pAttrb->m_L2;
	m_Attrb.m_Width = pAttrb->m_Width;
	ClearNeedsAttributes();
}

int CCadArrow::MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
			CopyAttributes((ArrowAttributes *)pAdrbData);
			m_P1 = m_P2 = pASV->m_SnapPos;
			break;
		case DRAWSTATE_WAITFORMOUSE_DOWN:
			if(NeedsAttributes()) CopyAttributes((ArrowAttributes *)pAdrbData);
			m_P1 = m_P2 = pASV->m_SnapPos;
			break;
		case DRAWSTATE_PLACE:
			m_P2 = pASV->m_SnapPos;
			break;
	}
	return DrawState;
}

int CCadArrow::MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
	CDlgArrowProperties Dlg;

	switch (DrawState)
	{
		case DRAWSTATE_SET_ATTRIBUTES:
			m_P1 = m_P2 = pASV->m_SnapPos;
			GetAttributes(&Dlg.m_Attrb);
			if (IDOK == Dlg.DoModal())
			{
				CopyAttributes(&Dlg.m_Attrb);
				DrawState = DRAWSTATE_WAITFORMOUSE_DOWN;
				theApp.UpdateStatusBar("Arrow:Locate Arrow Point");
			}
			break;
		case DRAWSTATE_WAITFORMOUSE_DOWN:
			m_P1 = m_P2 = pASV->m_SnapPos;
			DrawState = DRAWSTATE_PLACE;
			theApp.UpdateStatusBar("Arrow:Set Arrow Angle");
			break;
		case DRAWSTATE_PLACE:
			m_P2 = pASV->m_SnapPos;
			GetAttributes((ArrowAttributes *)pAdrbData);
			pASV->AddObjectIntoDoc(this);
			pASV->m_CadObj.pCadObject = 0;
			DrawState = DRAWSTATE_WAITFORMOUSE_DOWN;
			theApp.UpdateStatusBar("Arrow:Locate Arrow Point");
			pASV->Invalidate();
			break;
	}
	return DrawState;
}

int CCadArrow::MouseMove(CAutoScrollView * pASV, int DrawState)
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
		case DRAWSTATE_WAITFORMOUSE_DOWN:
			m_P1 = m_P2 = pASV->m_SnapPos;
			break;
		case DRAWSTATE_PLACE:
			m_P2 = pASV->m_SnapPos;
			break;
	}
	pASV->Invalidate();
	return DrawState;
}

void CCadArrow::CalcPoints(CPoint & a, CPoint & b,CPoint &c, CSize Offset, CScale Scale)
{
	//-----------------------------------------------
	// CalcPoints
	//
	//	This function is used to calculate the
	// two other points that make up the triange
	// for the arrow.  We use m_P1 and m_P2 to do
	// this.
	//
	// parameters:
	//		a.....reference to the second ppoint
	//		b.....reference to the third point
	//		c.....reference to the fourth point
	//		Offset....Reference Point
	//		Scale.....Scale factor
	//----------------------------------------------
	CSize diff;

	diff = m_P1 - m_P2;	//calculate difference

	if (diff.cy == 0)	//arrow pointing left or right works
	{
		int dx = (diff.cx > 0) ? m_Attrb.m_L1 : -m_Attrb.m_L1;
		int dx2 = (diff.cx > 0) ? m_Attrb.m_L2 : -m_Attrb.m_L2;
		a = Scale * CPoint(m_P1.x - dx - dx2, m_P2.y + m_Attrb.m_Width / 2) + Offset;
		b = Scale * CPoint(m_P1.x - dx - dx2, m_P2.y - m_Attrb.m_Width / 2) + Offset;
		c = Scale * CPoint(m_P1.x - dx, m_P1.y) + Offset;
	}
	else if (diff.cx == 0)	//arrow pointing up or down works
	{
		int dy = (diff.cy > 0) ? m_Attrb.m_L1 : -m_Attrb.m_L1;
		int dy2 = (diff.cy > 0) ? m_Attrb.m_L2 : -m_Attrb.m_L2;
		a = Scale * CPoint(m_P2.x + m_Attrb.m_Width / 2, m_P1.y - dy - dy2) + Offset;
		b = Scale * CPoint(m_P2.x - m_Attrb.m_Width / 2, m_P1.y - dy- dy2) + Offset;
		c = Scale * CPoint(m_P1.x, m_P1.y - dy);
	}
	else	//one of these doesn't work
	{
		double Angle;
		double x, y, dx, dy,dx2,dy2;
		//arrow pointing at some angle
		if (m_P1.x > m_P2.x && m_P1.y > m_P2.y)	//works
		{
			Angle = -atan(double(m_P1.y - m_P2.y) / double(m_P1.x - m_P2.x));
			dx = m_Attrb.m_L1 * cos(Angle);
			dy = m_Attrb.m_L1 * sin(Angle);
			dx2 = (m_Attrb.m_L1 + m_Attrb.m_L2)* cos(Angle);
			dy2 = (m_Attrb.m_L1 + m_Attrb.m_L2)* sin(Angle);
			Angle += 3.14159 / 2.0;
			x = double(m_Attrb.m_Width / 2) * cos(Angle);
			y = double(m_Attrb.m_Width / 2) * sin(Angle);
			a = Scale * CPoint(m_P1.x - int(dx2 - x), m_P1.y + int(dy2 - y)) + Offset;
			b = Scale * CPoint(m_P1.x - int(dx2 + x), m_P1.y + int(dy2 + y)) + Offset;
			c = Scale * CPoint(m_P1.x - int(dx), m_P1.y + int(dy)) + Offset;
		}
		else if (m_P1.x > m_P2.x && m_P1.y < m_P2.y)	//works
		{
			Angle = -atan(double(m_P2.y - m_P1.y) / double(m_P1.x - m_P2.x));
			dx = m_Attrb.m_L1 * cos(Angle);
			dy = m_Attrb.m_L1 * sin(Angle);
			dx2 = (m_Attrb.m_L1 + m_Attrb.m_L2)* cos(Angle);
			dy2 = (m_Attrb.m_L1 + m_Attrb.m_L2)* sin(Angle);
			Angle += 3.14159 / 2.0;
			x = double(m_Attrb.m_Width / 2) * cos(Angle);
			y = double(m_Attrb.m_Width / 2) * sin(Angle);
			a = Scale * CPoint(m_P1.x - int(dx2 + x), m_P1.y - int(dy2 + y)) + Offset;
			b = Scale * CPoint(m_P1.x - int(dx2 - x), m_P1.y - int(dy2 - y)) + Offset;
			c = Scale * CPoint(m_P1.x - int(dx), m_P1.y - int(dy)) + Offset;
		}
		else if (m_P1.x < m_P2.x && m_P1.y > m_P2.y)	//wprls
		{
			Angle = -atan(double(m_P1.y - m_P2.y) / double(m_P1.x - m_P2.x));
			dx = m_Attrb.m_L1 * cos(Angle);
			dy = m_Attrb.m_L1 * sin(Angle);
			dx2 = (m_Attrb.m_L1 + m_Attrb.m_L2)* cos(Angle);
			dy2 = (m_Attrb.m_L1 + m_Attrb.m_L2)* sin(Angle);
			Angle += 3.14159 / 2.0;
			x = double(m_Attrb.m_Width / 2) * cos(Angle);
			y = double(m_Attrb.m_Width / 2) * sin(Angle);
			a = Scale * CPoint(m_P1.x + int(dx2 + x), m_P1.y - int(dy2 + y)) + Offset;
			b = Scale * CPoint(m_P1.x + int(dx2 - x), m_P1.y - int(dy2 - y)) + Offset;
			c = Scale * CPoint(m_P1.x + int(dx), m_P1.y - int(dy)) + Offset;
		}
		else if (m_P1.x < m_P2.x && m_P1.y < m_P2.y)
		{
			Angle = -atan(double(m_P1.y - m_P2.y) / double(m_P1.x - m_P2.x));
			dx = m_Attrb.m_L1 * cos(Angle);
			dy = m_Attrb.m_L1 * sin(Angle);
			dx2 = (m_Attrb.m_L1 + m_Attrb.m_L2)* cos(Angle);
			dy2 = (m_Attrb.m_L1 + m_Attrb.m_L2)* sin(Angle);
			Angle += 3.14159 / 2.0;
			x = double(m_Attrb.m_Width / 2) * cos(Angle);
			y = double(m_Attrb.m_Width / 2) * sin(Angle);
			a = Scale * CPoint(m_P1.x + int(dx2 - x), m_P1.y - int(dy2 - y)) + Offset;
			b = Scale * CPoint(m_P1.x + int(dx2 + x), m_P1.y - int(dy2 + y)) + Offset;
			c = Scale * CPoint(m_P1.x + int(dx), m_P1.y - int(dy)) + Offset;
		}
	}
}

