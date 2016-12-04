//-----------------------------------------
// CCadPolygon.cpp
//
// This is one of my oldest drawing objects
// It dates to the Risk Game Map Editor
// I wrote a while back.  Its name has
// changed, but some of the klugy code that
// was in the original is still here.
//------------------------------------------

#include "stdafx.h"
#include "JamCadDefines.h"
#include "JamCad.h"
#include "CadPolygon.h"
#include "DlgPoloyProperties.h"

int CCadPolygon::m_RenderEnable = 1;
static int IDcount = 0;

void CCadPolygon::LoadSettings(PolyAttributes *pAttrib)
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
	pAttrib->m_Color = AfxGetApp()->GetProfileInt(LPCTSTR("POLYGON"), LPCTSTR("COLOR"), (int)RGB(0, 0, 0));
	pAttrib->m_Width = AfxGetApp()->GetProfileInt(LPCTSTR("POLYGON"), LPCTSTR("WIDTH"), 10);
}

void CCadPolygon::SaveSettings(PolyAttributes *pAttrib)
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
	AfxGetApp()->WriteProfileInt(LPCTSTR("POLYGON"), LPCTSTR("COLOR"), pAttrib->m_Color);
	AfxGetApp()->WriteProfileInt(LPCTSTR("POLYGON"), LPCTSTR("WIDTH"), pAttrib->m_Width);
}

CCadPolygon::CCadPolygon():CCadObject(OBJECT_TYPE_POLYGON)
{
	m_pVertex = new CPoint[256];		//pointer to array of vertexes
	m_Count = 0;
	m_size = 256;
	m_PolyID = ++IDcount;
	SetSelected(0);	//initial not selected
	m_MinX = m_MinY = m_MaxX = m_MaxY = 0;
	m_pPenLine = 0;
	m_NextPoint = 0;
}

CCadPolygon::CCadPolygon(int nVertex) : CCadObject(OBJECT_TYPE_POLYGON)
{
	m_pVertex = new CPoint[nVertex];		//pointer to array of vertexes
	m_Count = 0;
	m_size = nVertex;
	m_PolyID = ++IDcount;
	SetSelected(0);	//initial not selected
	m_MinX = m_MinY = m_MaxX = m_MaxY = 0;
	m_pPenLine = 0;
	m_NextPoint = 0;
}

CCadPolygon::CCadPolygon(CCadPolygon &v):CCadObject(OBJECT_TYPE_POLYGON)
{
	m_pVertex = new CPoint[256];		//pointer to array of vertexes
	m_Count = v.m_Count;
	m_size = 256;
	m_PolyID = ++IDcount;
	m_MinX = v.m_MinX;
	m_MinY = v.m_MinY;
	m_MaxX = v.m_MaxX;
	m_MaxY = v.m_MaxY;
	m_pPenLine = 0;
	m_NextPoint = 0;
	int i;
	for (i = 0; i<m_Count; ++i)
		m_pVertex[i] = v.m_pVertex[i];
}

CCadPolygon::~CCadPolygon()
{
	delete[] m_pVertex;
	if (m_pPenLine) delete m_pPenLine;
}

void CCadPolygon::Move(CPoint p)
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

	Diff = p - m_pVertex[0];
	int i;
	for (i = 0; i<m_Count; ++i)
		m_pVertex[i] += Diff;
	UpdateMinMax();
}

void CCadPolygon::Save(FILE * pO, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This function save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	fprintf(pO, "%s%s(%s(%d),", 
		MkIndentString(Indent),
		TokeToString(TOKEN_POLYGON),
		TokeToString(TOKEN_VERTEX),
		m_Count);

	int i;
	for (i = 0; i<m_Count; ++i)
	{
		fprintf(pO, "%s(%d,%d),",
			TokeToString(TOKEN_POINT),
			m_pVertex[i].x,
			m_pVertex[i].y);
	}
	fprintf(pO, "%s(%d,%d,%d),%s(%d))\n",
		TokeToString(TOKEN_COLOR),
		RED(m_Attrb.m_Color),
		GREEN(m_Attrb.m_Color),
		BLUE(m_Attrb.m_Color),
		TokeToString(TOKEN_WIDTH),
		m_Attrb.m_Width);
}

void CCadPolygon::SetVertex(int v, CPoint p)
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
	this->m_pVertex[v] = p;
	UpdateMinMax();
}


int CCadPolygon::GrabVertex(CPoint point, CScale scale)
{
	//***************************************************
	// GrabVertex
	//	This function checks for a vertex at point p
	//
	// parameters:
	//	point.....point to check for presence of a vertex
	//	scale....scale factor
	//
	// return value:
	//	returns index of vertex if succesful
	//	returns -1 on fail
	//--------------------------------------------------
	int i, loop, rV = -1;;
	CRect rect;
	CPoint p1, p2, p;

	for (i = 0, loop = 1; (i<this->m_Count) && loop; ++i)
	{
		p = m_pVertex[i];
		p1 = p + CPoint(-4, -4);
		p2 = p + CPoint(4, 4);
		rect.SetRect(p1, p2);
		rect.NormalizeRect();
		if (rect.PtInRect(point))
		{
			rV = i;
			loop = 0;
		}
	}
	return rV;
}


void CCadPolygon::Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale)
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
	if (CCadPolygon::m_RenderEnable)
	{
		CPen *oldpen;
		int Lw;
		if ((Lw = int(Scale.m_ScaleX * m_Attrb.m_Width)) < 1) Lw = 1;
		int i = 0;

		if (!IsLastModeSame(mode) || IsDirty())
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
			}
			SetDirty(0);
		}

		switch (mode)
		{
		case OBJECT_MODE_FINAL:
			oldpen = pDC->SelectObject(m_pPenLine);
			pDC->MoveTo(Scale * m_pVertex[i] + Offset);
			for (i = 1; i < this->m_Count; ++i)
				pDC->LineTo(Scale * m_pVertex[i] + Offset);
			pDC->LineTo(Scale * m_pVertex[0] + Offset);
			pDC->SelectObject(oldpen);
			SetLastMode(mode);
			break;
		case OBJECT_MODE_SELECTED:
			oldpen = pDC->SelectObject(m_pPenLine);
			pDC->MoveTo(Scale * m_pVertex[i] + Offset);
			for (i = 1; i < this->m_Count; ++i)
				pDC->LineTo(Scale * m_pVertex[i] + Offset);
			pDC->LineTo(Scale * m_pVertex[0] + Offset);
			for (i = 0; i < this->m_Count; ++i)
			{
				CPoint p1, p2, p;
				p = Scale * m_pVertex[i] + Offset;
				p1 = p + CPoint(4, 4);
				p2 = p - CPoint(4, 4);
				CRect rect;
				rect.SetRect(p2, p1);
				pDC->Rectangle(&rect);
			}
			pDC->SelectObject(oldpen);
			SetLastMode(mode);
			break;
		case OBJECT_MODE_SKETCH:
			oldpen = pDC->SelectObject(m_pPenLine);
			pDC->MoveTo(Scale * m_pVertex[i] + Offset);
			for (i = 1; i < this->m_Count; ++i)
				pDC->LineTo(Scale * m_pVertex[i] + Offset);
			if (m_NextPoint)
				pDC->LineTo(Scale * m_pVertex[i] + Offset);
			SetLastMode(mode);
			break;
		}
	}
}

int CCadPolygon::CheckSelected(CPoint p, CSize Offset, CCadObject ** ppSelList , int index, int n, int flag)
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
		if (PointEnclosed(p, Offset))
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

CPoint CCadPolygon::GetReference()
{
	//***************************************************
	// GetReference
	//	This function returns the reference point for
	// the object
	// parameters:none
	//
	// return value:reference point
	//--------------------------------------------------
	return m_pVertex[0];
}


void CCadPolygon::AdjustRefernce(CPoint Ref)
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
	int i;
	for (i = 0; i<m_Count; ++i)
		m_pVertex[i] -= Ref;
	UpdateMinMax();
}

CRect CCadPolygon::GetRect(void)
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
	CRect Rect;
	Rect.SetRect(m_MinX, m_MinY, m_MaxX, m_MaxY);
	Rect.NormalizeRect();
	return Rect;
}

char * CCadPolygon::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	return (char *)"Polygon";
}

CCadPolygon CCadPolygon::operator=(CCadPolygon &v)
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
	this->m_pPenLine = 0;
	m_size = v.m_size;
	m_Count = v.m_Count;
	m_Attrb.m_Width = v.m_Attrb.m_Width;
	m_Attrb.m_Color = v.m_Attrb.m_Color;
	m_MaxY = v.m_MaxY;
	m_MinY = v.m_MinY;
	m_MaxX = v.m_MaxX;
	m_MinX = v.m_MinX;
	m_PolyID = v.m_PolyID;
	for (int i = 0; i<m_Count; ++i)
		m_pVertex[i] = v.m_pVertex[i];
	return *this;
}

CCadObject * CCadPolygon::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	int i;
	CCadPolygon *pP = new CCadPolygon;
	SetPolySize(pP->GetPolySize());
	pP->m_Count = m_Count;
	pP->m_MinX = m_MinX;
	pP->m_MinY = m_MinY;
	pP->m_MaxX = m_MaxX;
	pP->m_MaxY = m_MaxY;
	pP->CopyAttributes(&this->m_Attrb);
	for (i = 0; i<m_Count; ++i)
	{
		pP->m_pVertex[i] = m_pVertex[i];
	}
	return pP;
}

void CCadPolygon::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------

}

void CCadPolygon::RenderEnable(int e)
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
	CCadPolygon::m_RenderEnable = e;
}

CPoint CCadPolygon::GetCenter()
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	CRect rect = GetRect();
	return rect.CenterPoint();
}

void CCadPolygon::ChangeCenter(CSize p)
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
	int i;
	for (i = 0; i<m_Count; ++i)
		m_pVertex[i] -= p;
	UpdateMinMax();
}

CSize CCadPolygon::GetSize()
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

void CCadPolygon::ChangeSize(CSize Sz)
{
	//***************************************************
	// ChangeSize
	//	Change the size of the object Not Implemented
	// for Polygons
	// parameters:
	//	sz.....size to change object by (not change to)
	// return value:
	//--------------------------------------------------
}

int CCadPolygon::Parse(int Token, CLexer *pLex)
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

CCadObject * CCadPolygon::NewInstance()
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

int CCadPolygon::MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
			CopyAttributes((PolyAttributes *)pAdrbData);
			break;
		case DRAWSTATE_WAITFORMOUSE_DOWN:
			if(NeedsAttributes()) CopyAttributes((PolyAttributes *)pAdrbData);
			m_P1 = pASV->m_SnapPos;
			SetCurrentVertex(pASV->m_SnapPos);
			break;
		case DRAWSTATE_PLACE:
			SetCurrentVertex(pASV->m_SnapPos);
			break;
	}
	return DrawState;
}

int CCadPolygon::MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
	CDlgPoloyProperties Dlg;

	switch (DrawState)
	{
		case DRAWSTATE_SET_ATTRIBUTES:
			theApp.UpdateStatusBar("POLYGON:Set Attributes");
			GetAttributes(&Dlg.m_Attrb);
			if (Dlg.DoModal() == IDOK)
			{
				CopyAttributes(&Dlg.m_Attrb);
				DrawState = DRAWSTATE_WAITFORMOUSE_DOWN;
				theApp.UpdateStatusBar("POLYGON:Place First Point");
			}
			break;
		case DRAWSTATE_WAITFORMOUSE_DOWN:
			m_P1 = pASV->m_SnapPos;
			AddPoint(m_P1);
			DrawState = DRAWSTATE_PLACE;
			pASV->EnableAutoScroll(1);
			theApp.UpdateStatusBar("POLYGON:Place Next Point");
			break;
		case DRAWSTATE_PLACE:
			if ((m_pVertex[0] == pASV->m_SnapPos) && m_Count)	//is figure closed?
			{
				GetAttributes((PolyAttributes *)pAdrbData);
				pASV->AddObjectIntoDoc(this);
				pASV->m_CadObj.pCadObject = 0;
				pASV->EnableAutoScroll(0);
				DrawState = DRAWSTATE_WAITFORMOUSE_DOWN;
				theApp.UpdateStatusBar("POLYGON:Place First Point");
				pASV->Invalidate();
			}
			else
			{
				AddPoint(pASV->m_SnapPos);
				pASV->Invalidate();
			}
			m_NextPoint = 0;
			break;
	}
	return DrawState;
}

int CCadPolygon::MouseMove(CAutoScrollView * pASV, int DrawState)
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
			pASV->Invalidate();
			break;
		case DRAWSTATE_WAITFORMOUSE_DOWN:
			m_P1 = pASV->m_SnapPos;
			SetCurrentVertex(pASV->m_SnapPos);
			pASV->Invalidate();
			break;
		case DRAWSTATE_PLACE:
			SetCurrentVertex(pASV->m_SnapPos);
			m_NextPoint = 1;
			pASV->Invalidate();
			break;
	}
	return DrawState;
}

void CCadPolygon::UpdateMinMax(void)
{
	int i=0;
	m_MinX = m_pVertex[i].x;
	m_MaxX = m_pVertex[i].x;
	m_MinY = m_pVertex[i].y;
	m_MaxY = m_pVertex[i].y;
	for (i = 1; i<m_Count; ++i)
	{
		if (m_pVertex[i].x > m_MaxX) m_MaxX = m_pVertex[i].x;
		else if (m_MinX > m_pVertex[i].x) m_MinX = m_pVertex[i].x;
		if (m_pVertex[i].y > m_MaxY) m_MaxY = m_pVertex[i].y;
		else if (m_MinY > m_pVertex[i].y) m_MinY = m_pVertex[i].y;
	}
}

void CCadPolygon::SetCurPoint(CPoint p)
{
	m_pVertex[m_Count] = p;
	++m_Count;
}

void CCadPolygon::Reset(void)
{
	m_Count = 0;
	SetSelected(0);	//initial not selected
}

CPoint * CCadPolygon::GetPoints(void)
{
	/*************************************
	** GetPoints
	**
	** Returns the list of points that
	** define the polygon.
	*************************************/
	return m_pVertex;
}

int CCadPolygon::CompareToLast(CPoint nP)
{
	/*********************************
	** CompareToLast
	**	This function compares a point
	** to the last point that was added
	**to a polygon.
	**
	** parameter:
	**	nP.....point to compare
	**
	** REtuns: TRUE if equal
	**	       FALSE if not equal.
	*********************************/
	int rV = 0;
	if (nP == m_pVertex[m_Count - 1])
		rV = 1;
	return rV;
}

int CCadPolygon::DeleteLastPoint(void)
{
	/**********************************
	** DeleteLastPoint
	**	Deletes last point added to
	** polygon.
	** RETURNS:new vertex count.
	**********************************/
	if (m_Count > 0)
		--m_Count;
	return m_Count;
}

int CCadPolygon::GetCount(void)
{
	/***********************************
	** GetCount
	**	Retturns the number of points
	** (verticies) in the polygon
	***********************************/
	return m_Count;
}

void CCadPolygon::AddPoint(CPoint nP)
{
	/*************************************
	** AddPoint
	**
	** Adds a new vertex to the polygon.
	**
	** parameters:
	**	nP....point of the new vertex.
	*************************************/
	m_pVertex[m_Count] = nP;
	//------------------------------------------
	// Create a rectangle that enbloses polygon
	//------------------------------------------
	if (m_Count == 0)
	{
		m_MinX = nP.x;
		m_MaxX = nP.x;
		m_MinY = nP.y;
		m_MaxY = nP.y;
	}
	else
	{
		if (nP.x > m_MaxX) m_MaxX = nP.x;
		else if (m_MinX > nP.x) m_MinX = nP.x;
		if (nP.y > m_MaxY) m_MaxY = nP.y;
		else if (m_MinY > nP.y) m_MinY = nP.y;
	}
	++m_Count;
}

BOOL CCadPolygon::PointEnclosed(CPoint point, CPoint Offset)
{
	/*****************************************
	** PointEnclosed
	**	This function determines if a point
	** is enclosed within a polygon.
	**
	** parameters:
	**	point....point to test
	** Returns: TRUE if point inside
	**          FALSE if point is outside
	*****************************************/
	int   i, j = m_Count - 1;
	BOOL  Enclosed = 0;
	int Xintercept;
	CPoint *vert = new CPoint[m_Count];
	for (i = 0; i<m_Count; ++i)
		vert[i] = m_pVertex[i] + Offset;
	//--------------------------------------------
	// Do a precheck agains the rectangle that
	// encloses the polygon
	//--------------------------------------------
	if (m_MinX < point.x && m_MaxX > point.x && m_MinY < point.y && m_MaxY > point.y)
	{
		//----------------------------------
		// very good chance now that point
		// is in polygon, so make a
		// thorough check
		//---------------------------------
		for (i = 0; i<m_Count; i++)
		{
			if (((vert[i].y< point.y && vert[j].y >= point.y)
				|| (vert[j].y< point.y && vert[i].y >= point.y))
				&& (vert[i].x <= point.x || vert[j].x <= point.x))
			{
				Xintercept = vert[i].x + ((point.y - vert[i].y)*(vert[j].x - vert[i].x)) / (vert[j].y - vert[i].y);
				Enclosed ^= Xintercept < point.x;
			}
			j = i;
		}
	}
	delete vert;
	return Enclosed;
}

void CCadPolygon::CopyAttributes(PolyAttributes *pPA)
{
	m_Attrb.m_Color = pPA->m_Color;
	m_Attrb.m_Width = pPA->m_Width;
	ClearNeedsAttributes();
}

void CCadPolygon::GetAttributes(PolyAttributes *pA)
{
	pA->m_Color = m_Attrb.m_Color;
	pA->m_Width = m_Attrb.m_Width;
}


void CCadPolygon::Rotate(CPoint Origin, int Rotation)
{
	int i;

	for (i = 0; i < m_Count; ++i)
		m_pVertex[i] = RotatePoint(Origin, m_pVertex[i], Rotation);
	UpdateMinMax();
}

void CCadPolygon::Flip(CPoint Origin, int Direction)
{
	//-------------------------------------------------
	// Flip
	//		Flips the object along an axis
	//
	// parameters:
	//	Origin.........specifies either x or y axis
	//	Direction......0=X Axis, 1 = Y Axis
	//-------------------------------------------------
	int i;

	for (i = 0; i < m_Count; ++i)
		m_pVertex[i] = FlipPoint(Origin, m_pVertex[i], Direction);
	UpdateMinMax();
}

int CCadPolygon::EditProperties()
{
	int Id;
	CDlgPoloyProperties Dlg;

	GetAttributes(&Dlg.m_Attrb);
	Id = Dlg.DoModal();
	if (IDOK == Id)
		CopyAttributes(&Dlg.m_Attrb);
	return Id;
}