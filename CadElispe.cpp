#include "stdafx.h"
#include "CadElispe.h"
#include "JamCadDefines.h"
#include "JamCad.h"
#include "DlgEllispeProperties.h"

int CCadElispe::m_RenderEnable = 1;

void CCadElispe::LoadSettings(EllipseAttributes *pAttrib)
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
	pAttrib->m_Color = AfxGetApp()->GetProfileInt("ELLISPE", "LINECOLOR", RGB(0, 0, 0));
	pAttrib->m_FillColor = AfxGetApp()->GetProfileInt("ELLISPE", "FILLCOLOR", RGB(0, 0, 255));
	pAttrib->m_Width = AfxGetApp()->GetProfileInt("ELLISPE", "LINEWIDTH", 40);
}

void CCadElispe::SaveSettings(EllipseAttributes *pAttrib)
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
	theApp.WriteProfileInt("ELLISPE", "LINECOLOR", pAttrib->m_Color);
	theApp.WriteProfileInt("ELLISPE", "FILLCOLOR", pAttrib->m_FillColor);
	theApp.WriteProfileInt("ELLISPE", "LINEWIDTH", pAttrib->m_Width);
}

CCadElispe::CCadElispe():CCadObject(OBJECT_TYPE_ELISPE)
{
	m_pPenLine = 0;
	m_pBrFill = 0;
}

CCadElispe::CCadElispe(CCadElispe &e) :CCadObject(OBJECT_TYPE_ELISPE)
{
	m_P1 = e.m_P1;
	m_P2 = e.m_P2;
	m_Attrb.m_FillColor = e.m_Attrb.m_FillColor;
	m_Attrb.m_Color = e.m_Attrb.m_Color;
	m_Attrb.m_Width = e.m_Attrb.m_Width;
	m_pPenLine = 0;
	m_pBrFill = 0;
}

CCadElispe::~CCadElispe()
{
	if (m_pPenLine) delete m_pPenLine;
	if (m_pBrFill) delete m_pBrFill;
}

void CCadElispe::Move(CPoint p)
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

void CCadElispe::Save(FILE * pO, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This function save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	fprintf(pO, "%s%s(%s(%d,%d),%s(%d,%d),%s(%d,%d,%d),%s(%d,%d,%d),%s(%d))\n",
		MkIndentString(Indent,' '),
		TokeToString(TOKEN_ELISPE),
		TokeToString(TOKEN_POINT),
		m_P1.x, m_P1.y, 
		TokeToString(TOKEN_POINT),
		m_P2.x, m_P2.y,
		TokeToString(TOKEN_COLOR),
		RED(m_Attrb.m_Color),
		GREEN(m_Attrb.m_Color),
		BLUE(m_Attrb.m_Color),
		TokeToString(TOKEN_FILLCOLOR),
		RED(m_Attrb.m_FillColor),
		GREEN(m_Attrb.m_FillColor),
		BLUE(m_Attrb.m_FillColor),
		TokeToString(TOKEN_WIDTH),
		m_Attrb.m_Width);
}

void CCadElispe::SetVertex(int v, CPoint p)
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
	if (v)
		m_P2 += p - m_P2;
	else
		m_P1 += p - m_P1;
}


int CCadElispe::GrabVertex(CPoint p)
{
	//***************************************************
	// GrabVertex
	//	This function checks for a vertex at point p
	//
	// parameters:
	//	p.....point to check for presence of a vertex
	//
	// return value:
	//	returns index of vertex if succesful
	//	returns -1 on fail
	//--------------------------------------------------
	int rV = -1;
	CSize Diff(4, 4);
	CRect rect;
	rect.SetRect(m_P1 + Diff, m_P1 - Diff);
	rect.NormalizeRect();
	if (rect.PtInRect(p))
		rV = 0;
	else
	{
		rect.SetRect(m_P2 + Diff, m_P2 - Diff);
		rect.NormalizeRect();
		if (rect.PtInRect(p))
			rV = 1;
	}
	return rV;
}


void CCadElispe::Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale)
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
	CBrush *pOldBr;
	CRect rect;
	CSize rectLWcomp;
	CPoint P1, P2;
	int Lw;

	if (CCadElispe::m_RenderEnable)
	{
		P1 = Scale * m_P1 + Offset;
		P2 = Scale * m_P2 + Offset;
		if ((Lw = int(Scale.m_ScaleX * m_Attrb.m_Width)) < 1) Lw = 1;
		if (Lw <= 1 || OBJECT_MODE_SKETCH == mode)
		{
			Lw = 1;
			rectLWcomp = CSize(0, 0);
		}
		else
			rectLWcomp = CSize(Lw / 2, Lw / 2);
		if ((!IsLastModeSame(mode) )|| IsDirty())
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
		}
		if (m_pBrFill == 0)
			m_pBrFill = new CBrush(m_Attrb.m_FillColor);
		else if (IsDirty())
		{
			if (m_pBrFill) delete m_pBrFill;
			m_pBrFill = new CBrush(m_Attrb.m_FillColor);
			SetDirty(0);
		}
		SetRect(rect, P1, P2, rectLWcomp);
		switch (mode)
		{
		case OBJECT_MODE_FINAL:
			pOld = pDC->SelectObject(m_pPenLine);
			pOldBr = pDC->SelectObject(m_pBrFill);
			pDC->Ellipse(&rect);
			pDC->SelectObject(pOldBr);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_SELECTED:
		{
			CPen SelPen;
			CBrush SelBrush;
			SelPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			SelBrush.CreateSolidBrush(RGB(255, 0, 0));
			pOld = pDC->SelectObject(m_pPenLine);
			pOldBr = pDC->SelectObject(m_pBrFill);
			pDC->Ellipse(&rect);
			pDC->SelectObject(&SelPen);
			pDC->SelectObject(&SelBrush);
			CRect rect;
			CSize p = CSize(4, 4);
			rect.SetRect(P1 - p, P1 + p);
			pDC->Rectangle(&rect);
			rect.SetRect(P2 - p, P2 + p);
			pDC->Rectangle(&rect);
			pDC->SelectObject(pOldBr);
			pDC->SelectObject(pOld);
		}
		break;
		case OBJECT_MODE_SKETCH:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Ellipse(&rect);
			pDC->SelectObject(pOld);
			break;
		}
		SetLastMode(mode);
	}
}

int CCadElispe::PtInElipse(CPoint p, CSize Offset)
{
	double a, b, xo, yo, v;
	int rV;
	static int count = 0;
	CPoint P1 = m_P1 + Offset;
	CPoint P2 = m_P2 + Offset;

	a = double(P2.x - P1.x) / 2.0;
	b = double(P2.y - P1.y) / 2.0;
	xo = p.x - (a + P1.x);
	yo = p.y - (b + P1.y);
	v = (xo * xo) / (a * a) + (yo * yo) / (b * b);
	if (v < 1.0) rV = TRUE;
	else rV = FALSE;
	return rV;
}

int CCadElispe::CheckSelected(CPoint p, CSize Offset, CCadObject ** ppSelList , int index, int n, int flag)
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
		if (PtInElipse(p - Offset))
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

CPoint CCadElispe::GetReference()
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

void CCadElispe::AdjustRefernce(CPoint Ref)
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

CRect CCadElispe::GetRect(void)
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
	return CRect();
}

char * CCadElispe::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	return (char *)"Ellipse";
}

CCadElispe CCadElispe::operator=(CCadElispe &v)
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
	m_Attrb.m_FillColor = v.m_Attrb.m_FillColor;
	m_Attrb.m_Color = v.m_Attrb.m_Color;
	m_Attrb.m_Width = v.m_Attrb.m_Width;
	return *this;
}

CCadObject * CCadElispe::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadElispe *pCE = new CCadElispe;
	*pCE = *this;
	return pCE;
}

void CCadElispe::RenderEnable(int e)
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
	CCadElispe::m_RenderEnable = e;
}

CPoint CCadElispe::GetCenter()
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

void CCadElispe::ChangeCenter(CSize p)
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

CSize CCadElispe::GetSize()
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

void CCadElispe::ChangeSize(CSize Sz)
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

int CCadElispe::Parse(int Token, CLexer *pLex)
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

CCadObject * CCadElispe::NewInstance()
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

void CCadElispe::GetAttributes(EllipseAttributes *pAttrb)
{
	/***************************************************
	*	GetAttributes
	*		This function is used to copy the
	*	attributes from this object into
	*	an extern attribute structure
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy into
	***************************************************/
	pAttrb->m_Color = m_Attrb.m_Color;
	pAttrb->m_FillColor = m_Attrb.m_FillColor;
	pAttrb->m_Width = m_Attrb.m_Width;
}


void CCadElispe::CopyAttributes(EllipseAttributes *pAttrb)
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
	m_Attrb.m_FillColor = pAttrb->m_FillColor;
	m_Attrb.m_Width = pAttrb->m_Width;
	ClearNeedsAttributes();
}

int CCadElispe::MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
			CopyAttributes((EllipseAttributes *)pAdrbData);
			break;
		case DRAWSTATE_WAITFORMOUSE_DOWN:
			if(NeedsAttributes()) CopyAttributes((EllipseAttributes *)pAdrbData);
			m_P1 = m_P2 = pASV->m_SnapPos;
			break;
		case DRAWSTATE_PLACE:
			break;
	}
	return DrawState;
}

int CCadElispe::MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
	CDlgEllispeProperties Dlg;

	switch (DrawState)
	{
		case DRAWSTATE_SET_ATTRIBUTES:
			theApp.UpdateStatusBar("ELLISPE:Set Attributes");
			GetAttributes(&Dlg.m_Attrb);
			if (IDOK == Dlg.DoModal())
			{
				CopyAttributes(&Dlg.m_Attrb);
				DrawState = DRAWSTATE_WAITFORMOUSE_DOWN;
				theApp.UpdateStatusBar("ELLISPE:Place First Corner");
			}
			break;
		case DRAWSTATE_WAITFORMOUSE_DOWN:
			m_P1 = m_P2 = pASV->m_SnapPos;
			DrawState = DRAWSTATE_PLACE;
			pASV->EnableAutoScroll(1);
			theApp.UpdateStatusBar("ELLISPE:Place Second Corner");
			break;
		case DRAWSTATE_PLACE:
			m_P1 = pASV->m_SnapPos;
			GetAttributes((EllipseAttributes *)pAdrbData);
			pASV->AddObjectIntoDoc(this);
			pASV->m_CadObj.pCadElispe = 0;
			DrawState = DRAWSTATE_WAITFORMOUSE_DOWN;
			pASV->EnableAutoScroll(0);
			theApp.UpdateStatusBar("ELLISPE:Place First Corner");
			pASV->Invalidate();
			break;
	}
	return DrawState;
}

int CCadElispe::MouseMove(CAutoScrollView * pASV, int DrawState)
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
			m_P1 = pASV->m_SnapPos;
			pASV->Invalidate();
			break;
	}
	return DrawState;
}