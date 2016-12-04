#include "stdafx.h"
#include "CadRect.h"
#include "JamCad.h"
#include "DlgRectProperties.h"

int CCadRect::m_RenderEnable = 1;

void CCadRect::LoadSettings(RectAttributes *pAttrib)
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
	pAttrib->m_Color = AfxGetApp()->GetProfileInt("RECTANGLE", "LINECOLOR", RGB(0, 0, 0));
	pAttrib->m_FillColor = AfxGetApp()->GetProfileInt("RECTANGLE", "FILLCOLOR", RGB(128, 0, 255));
	pAttrib->m_Width = AfxGetApp()->GetProfileInt("RECTANGLE", "LINEWIDTH",40 );
}

void CCadRect::SaveSettings(RectAttributes *pAttrib)
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
	theApp.WriteProfileInt("RECTANGLE", "LINECOLOR", pAttrib->m_Color);
	theApp.WriteProfileInt("RECTANGLE","FILLCOLOR", pAttrib->m_FillColor);
	theApp.WriteProfileInt("RECTANGLE", "LINEWIDTH", pAttrib->m_Width);
}


CCadRect::CCadRect() :CCadObject(OBJECT_TYPE_RECT)
{
	m_pPenLine = 0;
	m_pBrFill = 0;
}

CCadRect::CCadRect(CCadRect &r) :CCadObject(OBJECT_TYPE_RECT)
{
	m_pPenLine = 0;
	m_pBrFill = 0;
	m_Attrb.m_FillColor = r.m_Attrb.m_FillColor;
	m_Attrb.m_Color = r.m_Attrb.m_Color;
	m_Attrb.m_Width = r.m_Attrb.m_Width;
	m_P1 = r.m_P1;
	m_P2 = r.m_P2;
}

CCadRect::~CCadRect()
{
	if (m_pPenLine) delete m_pPenLine;
	if (m_pBrFill) delete m_pBrFill;
}

void CCadRect::Move(CPoint p)
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

void CCadRect::Save(FILE * pO, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This function save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	fprintf(pO, "%s%s(%s(%d,%d),%s(%d,%d)){%s(%d,%d,%d),%s(%d,%d,%d),%s(%d)}\n",
		MkIndentString(Indent,' '),
		TokeToString(TOKEN_RECT),
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

void CCadRect::SetVertex(int v, CPoint p)
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


int CCadRect::GrabVertex(CPoint p)
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


void CCadRect::Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale)
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

	if (CCadRect::m_RenderEnable)
	{
		P1 = Scale * m_P1 + Offset;
		P2 = Scale * m_P2 + Offset;
		Lw = int(m_Attrb.m_Width * Scale.m_ScaleX);
		if (Lw <= 1 || OBJECT_MODE_SKETCH == mode)
		{
			Lw = 1;
			rectLWcomp = CSize(0, 0);
		}
		else
			rectLWcomp = CSize(Lw / 2, Lw / 2);
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
		}
		if (IsDirty())
		{
			if (m_pBrFill) delete m_pBrFill;
			m_pBrFill = new CBrush(m_Attrb.m_FillColor);
			SetDirty(0);
		}
		if (m_pBrFill == 0) m_pBrFill = new CBrush(m_Attrb.m_FillColor);
		SetRect(rect, P1, P2, rectLWcomp);
		switch (mode)
		{
		case OBJECT_MODE_FINAL:
			pOld = pDC->SelectObject(m_pPenLine);
			pOldBr = pDC->SelectObject(m_pBrFill);
			pDC->Rectangle(&rect);
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
			pDC->Rectangle(&rect);
			pDC->SelectObject(&SelPen);
			pDC->SelectObject(&SelBrush);
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
			pDC->Rectangle(&rect);
			pDC->SelectObject(pOld);
			break;
		}
		SetLastMode(mode);
	}
}

int CCadRect::CheckSelected(CPoint p, CSize Offset, CCadObject ** ppSelList, int index, int n, int flag)
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
	CRect rect;
	int rV = index;
	if (index < n || n==0)
	{
		rect.SetRect(m_P1, m_P2);
		rect.NormalizeRect();
		if (rect.PtInRect(p - Offset))
		{
			if (ppSelList)
			{
				switch (flag)
				{
					case CADDRAWUBG_CHECKSEL_FLAG_ALL:
						ppSelList[rV++] = this;
						break;
					case CADDRAWUBG_CHECKSEL_FLAG_UNSEL:
						if(!IsSelected())
							ppSelList[rV++] = this;
						break;
					case CADDRAWUBG_CHECKSEL_FLAG_SEL:
						if(IsSelected())
							ppSelList[rV++] = this;
						break;
				}
			}
			else
			{
				switch (flag)
				{
					case CADDRAWUBG_CHECKSEL_FLAG_ALL:
						rV = 1;
						break;
					case CADDRAWUBG_CHECKSEL_FLAG_UNSEL:
						if (!IsSelected())
							rV = 1;
						break;
					case CADDRAWUBG_CHECKSEL_FLAG_SEL:
						if (IsSelected())
							rV = 1;
						break;
				}

			}
		}
	}
	return rV;
}

CPoint CCadRect::GetReference()
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

void CCadRect::AddObject(CCadObject *pO)
{
	//***************************************************
	// AddObject
	//	Adds a child object to an object.  This function
	// is not supported by all objects, because they just
	// can have children.  A good example of an object
	// that has children would be a library object
	//
	// parameters:
	//	pO.....pointer to object to add.
	//
	// return value:none
	//--------------------------------------------------
}

void CCadRect::RemoveObject(CCadObject *pO)
{
	//***************************************************
	// RemoveObject
	//	Removes a child object from an object
	//
	// parameters:
	//	pO.....pointer to object to remove
	//
	// return value:none
	//--------------------------------------------------

}

CCadObject *CCadRect::GetHead(void)
{
	//***************************************************
	// GetHead
	//	Retrieves the head pointer to a list of children
	// in the object
	// parameters:
	//
	// return value: Head pointer to children
	//--------------------------------------------------
	return 0;
}


void CCadRect::AdjustRefernce(CPoint Ref)
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

CRect CCadRect::GetRect(void)
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
	CRect rect(m_P1, m_P2);
	rect.NormalizeRect();
	return rect;
}

char * CCadRect::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	return (char *)"Rectangle";
}

CCadRect CCadRect::operator=(CCadRect &v)
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

CPoint CCadRect::ScalePoint(CPoint p, int Scale, int ScaleDiv)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return CPoint();
}

CCadObject * CCadRect::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy.
	// This function DOES not make a true copy in
	// that the pointers for linked lists are set
	// to NULL
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadRect *pRect = new CCadRect;
	*pRect = *this;
	return pRect;
}

void CCadRect::RenderEnable(int e)
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
	CCadRect::m_RenderEnable = e;
}

CCadObject *CCadRect::PopHead()
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return 0;
}

CCadObject *CCadRect::PopTail()
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return 0;
}

CPoint CCadRect::GetCenter()
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

void CCadRect::ChangeCenter(CSize p)
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

CSize CCadRect::GetSize()
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

void CCadRect::ChangeSize(CSize Sz)
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

int CCadRect::Parse(int Token, CLexer *pLex)
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
	Token = pLex->Accept (Token, TOKEN_RECT);
	Token = pLex->Accept(Token, '(');
	Token = pLex->Point(TOKEN_POINT,m_P1, Token);
	Token = pLex->Accept(Token, ',' );
	Token = pLex->Point(TOKEN_POINT, m_P2, Token);
	Token = pLex->Accept(Token, ')');
	Token = pLex->Accept(Token, '{');
	int loop = 1;
	while (loop)
	{
		switch (Token)
		{
			case TOKEN_COLOR:
				Token = pLex->Color(Token, m_Attrb.m_Color, Token);
				break;
			case TOKEN_FILLCOLOR:
				Token = pLex->Color(Token, m_Attrb.m_FillColor, Token);
				break;
			case TOKEN_WIDTH:
				Token = pLex->ReadDecimalValue(TOKEN_WIDTH,m_Attrb.m_Width, Token);
				break;
			case ',':
				Token = pLex->Accept(Token, ',' );
				break;
			case '}':
				loop = 0;	//done parsing attributes of Rect
				break;
		}
	}
	return Token;
}

CCadObject * CCadRect::NewInstance()
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

void CCadRect::GetAttributes(RectAttributes *pAttrb)
{
	/***************************************************
	*	GetAttributes
	*		This function is used to copy the
	*	attributes from this object into
	*	an external attributes stucture
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy
	***************************************************/
	pAttrb->m_Color = m_Attrb.m_Color;
	pAttrb->m_FillColor = m_Attrb.m_FillColor;
	pAttrb->m_Width = m_Attrb.m_Width;
}


void CCadRect::CopyAttributes(RectAttributes *pAttrb)
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

int CCadRect::MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
	RectAttributes *pRA = (RectAttributes *)pAdrbData;
	switch (DrawState)
	{
		case DRAWSTATE_SET_ATTRIBUTES:
			CopyAttributes(pRA);
			m_P1 = pASV->m_SnapPos;
			m_P2 = m_P1;
			break;
		case DRAWSTATE_WAITFORMOUSE_DOWN:
			if(NeedsAttributes())
				CopyAttributes(pRA);
			m_P1 = pASV->m_SnapPos;
			m_P2 = m_P1;
			break;
		case DRAWSTATE_PLACE:
			break;
	}
	return DrawState;
}

int CCadRect::MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
	CDlgRectProperties Dlg;

	switch (DrawState)
	{
		case DRAWSTATE_SET_ATTRIBUTES:
			theApp.UpdateStatusBar("RECTANGLE:Set Attributes");
			GetAttributes(&Dlg.m_Attrb);
			if (IDOK == Dlg.DoModal())
			{
				CopyAttributes(&Dlg.m_Attrb);
				DrawState = DRAWSTATE_WAITFORMOUSE_DOWN;
				theApp.UpdateStatusBar("RECTANGLE:Place First Corner");
			}
			break;
		case DRAWSTATE_WAITFORMOUSE_DOWN:
			m_P1 = pASV->m_SnapPos;
			DrawState = DRAWSTATE_PLACE;
			pASV->EnableAutoScroll(1);
			theApp.UpdateStatusBar("RECTANGLE:Place Second Corner");
			break;
		case DRAWSTATE_PLACE:
			m_P2 = pASV->m_SnapPos;
			GetAttributes((RectAttributes *)pAdrbData);
			pASV->AddObjectIntoDoc(this);
			pASV->m_CadObj.pCadObject = 0;
			pASV->EnableAutoScroll(0);
			DrawState = DRAWSTATE_WAITFORMOUSE_DOWN;
			theApp.UpdateStatusBar("RECTANGLE:Place First Corner");
			pASV->Invalidate();
			break;
	}
	return DrawState;
}

int CCadRect::MouseMove(CAutoScrollView * pASV, int DrawState)
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
		pASV->Invalidate();
		break;
	case DRAWSTATE_PLACE:
		m_P2 = pASV->m_SnapPos;
		pASV->Invalidate();
		break;
	}
	return DrawState;
}

int CCadRect::EditProperties()
{
	int Id;
	CDlgRectProperties Dlg;

	GetAttributes(&Dlg.m_Attrb);
	Id = Dlg.DoModal();
	if (IDOK == Id)
		CopyAttributes(&Dlg.m_Attrb);
	return Id;
}