#include "stdafx.h"
#include <math.h>
#include "JamCad.h"
#include "CadText.h"
#include "DlgTextProperties.h"

int CCadText::m_RenderEnable = 1;

TextAttributes::TextAttributes()
{
	m_pFontName = new char[LF_FACESIZE];
	m_pFontName[0] = 0;
	m_pText = new char[256];
	m_pText[0] = 0;
}

TextAttributes::~TextAttributes()
{
	delete[] m_pFontName;
	delete[] m_pText;
}


void CCadText::LoadSettings(char *Section,TextAttributes *pAttrib)
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
	pAttrib->m_Color = theApp.GetProfileInt(Section, "Color", (int)RGB(0, 0, 0));
	pAttrib->m_BkColor = theApp.GetProfileInt(Section, "BackColor", (int)RGB(255, 255, 255));
	pAttrib->m_Angle = theApp.GetProfileInt(Section, "Angle", 0);
	pAttrib->m_FontHeight = theApp.GetProfileInt(Section, "FontHeight", 60);
	pAttrib->m_FontWidth = theApp.GetProfileInt(Section, "Width", 0);
	pAttrib->m_Format = theApp.GetProfileInt(Section, "Format", DT_BOTTOM | DT_SINGLELINE);
	pAttrib->m_Transparent = theApp.GetProfileInt(Section, "Transparent", 1);
	pAttrib->m_Weight = theApp.GetProfileInt(Section, "Weight", FW_DEMIBOLD);
	strcpy_s(pAttrib->m_pFontName, LF_FACESIZE, LPCTSTR(theApp.GetProfileString(Section, "Font", "Arial")));
}

void CCadText::SaveSettings(char *Section, TextAttributes *pAttrib)
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
	theApp.WriteProfileInt(Section, "Color", (int)pAttrib->m_Color);
	theApp.WriteProfileInt(Section, "BackColor", (int)pAttrib->m_BkColor);
	theApp.WriteProfileInt(Section, "Angle", pAttrib->m_Angle);
	theApp.WriteProfileInt(Section, "FontHeight", pAttrib->m_FontHeight);
	theApp.WriteProfileInt(Section, "Width", pAttrib->m_FontWidth);
	theApp.WriteProfileInt(Section, "Format", pAttrib->m_Format);
	theApp.WriteProfileInt(Section, "Transparent", pAttrib->m_Transparent);
	theApp.WriteProfileInt(Section, "Weight", pAttrib->m_Weight);
	theApp.WriteProfileString(Section, "Font", pAttrib->m_pFontName);
}


CCadText::CCadText():CCadObject(OBJECT_TYPE_TEXT)
{
	m_pFont = 0;
	m_pSelPen = 0;
	m_Attrb.m_Transparent = 1;
}

CCadText::CCadText(CCadText& v):CCadObject(OBJECT_TYPE_TEXT)
{

	m_pFont = 0;
	m_pSelPen = 0;
	m_P1 = v.m_P1;
	CopyAttributes(&v.m_Attrb);
	this->m_SelRect = v.m_SelRect;
}

CCadText::~CCadText()
{
	if (m_pFont) delete m_pFont;
}

void CCadText::Move(CPoint p)
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
	m_P1 = p;
	CRect rect = GetTextRectangle();
	Rotate(m_Attrb.m_Angle, rect, m_SelRect);
}

void CCadText::Save(FILE * pO,int Indent, int flags)
{
	//***************************************************
	// Save
	//		This function save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	fprintf(pO, "%s%s(\"%s\",%s(%d,%d),%s(%d,%d,%d),%s(%d,%d,%d),%s(\"%s\"),%s(%d),%s(%d),%s(%d),%s(%d),%s(%d))\n",
		MkIndentString(Indent),
		TokeToString(TOKEN_TEXT),
		m_Attrb.m_pText,
		TokeToString(TOKEN_POINT),
		m_P1.x, m_P1.y, 
		TokeToString(TOKEN_COLOR),
		RED(m_Attrb.m_Color ),
		GREEN(m_Attrb.m_Color), 
		BLUE(m_Attrb.m_Color),
		TokeToString(TOKEN_BKCOLOR),
		RED(m_Attrb.m_BkColor),
		GREEN(m_Attrb.m_BkColor),
		BLUE(m_Attrb.m_BkColor),
		TokeToString(TOKEN_FONT),
		m_Attrb.m_pFontName,
		TokeToString(TOKEN_WEIGHT),
		m_Attrb.m_Weight,
		TokeToString(TOKEN_HEIGHT),
		m_Attrb.m_FontHeight, 
		TokeToString(TOKEN_WIDTH),
		m_Attrb.m_FontWidth,
		TokeToString(TOKEN_ANGLE),
		m_Attrb.m_Angle,
		TokeToString(TOKEN_TRANSPARENT),
		m_Attrb.m_Transparent);
}

void CCadText::SetVertex(int v, CPoint p)
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


int CCadText::GrabVertex(CPoint p)
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
	return 0;
}


void CCadText::Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale)
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
	CFont *pOldFont;
	CPen *pOldPen;
	COLORREF OldColor, OldBk;
	int FontHeight, FontWidth;
	CPoint P1;
	int OldMode;

	if (CCadText::m_RenderEnable && m_Attrb.m_pText)
	{
		P1 = (Scale * m_P1) + Offset;
		FontHeight = int(Scale.m_ScaleX * m_Attrb.m_FontHeight);
		FontWidth = int(Scale.m_ScaleX * m_Attrb.m_FontWidth);
		if (IsLastModeSame(mode) || IsDirty() || (m_LastScaleX != Scale.m_ScaleX))
		{
			CRect rect = GetTextRectangle(pDC, Scale);
			Rotate(m_Attrb.m_Angle, rect, m_SelRect);
			switch (mode)
			{
			case OBJECT_MODE_SELECTED:
				if (m_pSelPen == 0)
				{
					m_pSelPen = new CPen;
					m_pSelPen->CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				}
			case OBJECT_MODE_SKETCH:
			case OBJECT_MODE_FINAL:
				if (this->m_pFont) delete m_pFont;
				m_pFont = new CFont;
				if (Scale.m_ScaleY > 0.0)
					m_pFont->CreateFont(FontHeight, FontWidth, m_Attrb.m_Angle, m_Attrb.m_Angle,
						m_Attrb.m_Weight, 0, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
						CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH,
						LPCTSTR(m_Attrb.m_pFontName));
				else
					m_pFont->CreateFont(FontHeight, FontWidth, -m_Attrb.m_Angle, -m_Attrb.m_Angle,
						m_Attrb.m_Weight, 0, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
						CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH,
						LPCTSTR(m_Attrb.m_pFontName));

				break;
			}
			SetDirty(0);
			SetLastMode( mode);
			m_LastScaleX = Scale.m_ScaleX;
		}
		OldColor = pDC->SetTextColor(m_Attrb.m_Color);
		OldBk = pDC->SetBkColor(m_Attrb.m_BkColor);
		pOldFont = pDC->SelectObject(m_pFont);
		if (m_Attrb.m_Transparent)
			OldMode = pDC->SetBkMode(TRANSPARENT);
		else
			OldMode = pDC->SetBkMode(OPAQUE);
		pDC->TextOut(P1.x, P1.y, LPCTSTR(m_Attrb.m_pText), strlen(m_Attrb.m_pText));
		pDC->SetBkMode(OldMode);
		if (mode == OBJECT_MODE_SELECTED)
		{
			pOldPen = pDC->SelectObject(m_pSelPen);
			CPoint *pVertex = m_SelRect.GetPoints();
			pDC->MoveTo(Scale * pVertex[0] + Offset);
			pDC->LineTo(Scale * pVertex[1] + Offset);
			pDC->LineTo(Scale * pVertex[2] + Offset);
			pDC->LineTo(Scale * pVertex[3] + Offset);
			pDC->LineTo(Scale * pVertex[0] + Offset);
			pDC->SelectObject(pOldPen);
		}
		pDC->SetBkColor(OldBk);
		pDC->SetTextColor(OldColor);
		pDC->SelectObject(pOldFont);
	}
}

int CCadText::CheckSelected(CPoint p, CSize Offset, CCadObject ** ppSelList , int index, int n, int flag)
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
	if (m_SelRect.GetCount() == 0)	//no valid rectangle yet
	{
		CRect rect = GetTextRectangle();
		this->Rotate(m_Attrb.m_Angle, rect, m_SelRect);
	}
	if (index < n || n == 0)
	{
		if (m_SelRect.PointEnclosed(p - Offset))
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

CPoint CCadText::GetReference()
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

void CCadText::AdjustRefernce(CPoint Ref)
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

CRect CCadText::GetRect(void)
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
	return GetTextRectangle();
}

char * CCadText::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	return (char *)"Text";
}

CCadText CCadText::operator=(CCadText &v)
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
	m_pFont = 0;
	m_pSelPen = 0;
	m_P1 = v.m_P1;
	CopyAttributes(&v.m_Attrb);
	this->m_SelRect = v.m_SelRect;
	return *this;
}

CCadObject * CCadText::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadText *pCT = new CCadText;
	*pCT = *this;
	return pCT;
}

void CCadText::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------

}

void CCadText::RenderEnable(int e)
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
	CCadText::m_RenderEnable = e;
}

CPoint CCadText::GetCenter()
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

void CCadText::ChangeCenter(CSize p)
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

CSize CCadText::GetSize()
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

void CCadText::ChangeSize(CSize Sz)
{
	//***************************************************
	// ChangeSize
	//	Change the size of the object
	// parameters:
	//	sz.....size to change object by (not change to)
	// return value:None
	//--------------------------------------------------
}

int CCadText::Parse(int Token, CLexer *pLex)
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
	return Token;
}

CCadObject * CCadText::NewInstance()
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

void CCadText::GetAttributes(TextAttributes *pAttrib)
{
	/***************************************************
	*	GetAttributes
	*		This function is used to copy the
	*	attributes this object into an outside
	*	desitination
	*
	* Parameters:
	*	pAttrib.....pointer to attributes structure to copy to
	***************************************************/
	pAttrib->m_Color = m_Attrb.m_Color;
	pAttrib->m_BkColor = m_Attrb.m_BkColor;
	pAttrib->m_Angle = m_Attrb.m_Angle;
	pAttrib->m_FontHeight = m_Attrb.m_FontHeight;
	pAttrib->m_FontWidth = m_Attrb.m_FontWidth;
	pAttrib->m_Format = m_Attrb.m_Format;
	pAttrib->m_Transparent = m_Attrb.m_Transparent;
	pAttrib->m_Weight = m_Attrb.m_Weight;
	strcpy_s(pAttrib->m_pFontName , LF_FACESIZE, m_Attrb.m_pFontName);
	strcpy(pAttrib->m_pText,m_Attrb.m_pText);

}

void CCadText::CopyAttributes(TextAttributes *s)
{
	/***************************************************
	*	CopyAttributes
	*		This function is used to copy the
	*	attributes pointed to by the parameter into
	*	this object
	*
	* Parameters:
	*	s.....pointer to attributes structure to copy
	***************************************************/
	m_Attrb.m_Color = s->m_Color;
	m_Attrb.m_BkColor = s->m_BkColor;
	m_Attrb.m_Angle = s->m_Angle;
	m_Attrb.m_FontHeight = s->m_FontHeight;
	m_Attrb.m_FontWidth = s->m_FontWidth;
	m_Attrb.m_Format = s->m_Format;
	m_Attrb.m_Transparent = s->m_Transparent;
	m_Attrb.m_Weight = s->m_Weight;
	strcpy_s(m_Attrb.m_pFontName, LF_FACESIZE, s->m_pFontName);
	strcpy_s(m_Attrb.m_pText, 256, s->m_pText);
	ClearNeedsAttributes();
}

int CCadText::MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
			CopyAttributes((TextAttributes *)pAdrbData);
			m_P1 = m_P2 = pASV->m_SnapPos;
			pASV->Invalidate();
			break;
		case DRAWSTATE_WAITFORMOUSE_DOWN:
			if(NeedsAttributes())
				CopyAttributes((TextAttributes *)pAdrbData);
			m_P1 = m_P2 = pASV->m_SnapPos;
			pASV->Invalidate();
			break;
		case DRAWSTATE_PLACE:
			break;
	}
	return DrawState;
}

int CCadText::MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
	CDlgTextProperties Dlg;

	switch (DrawState)
	{
		case DRAWSTATE_SET_ATTRIBUTES:
			theApp.UpdateStatusBar("TEXT:Set Attributes");
			m_P1 = pASV->m_SnapPos;
			GetAttributes(&Dlg.m_Attr);
			if (IDOK == Dlg.DoModal())
			{
				CopyAttributes(&Dlg.m_Attr);
				DrawState = DRAWSTATE_WAITFORMOUSE_DOWN;
				theApp.UpdateStatusBar("TEXT:Move Text");
			}
			break;
		case DRAWSTATE_WAITFORMOUSE_DOWN:
			m_P1 = pASV->m_SnapPos;
			DrawState = DRAWSTATE_PLACE;
			pASV->EnableAutoScroll(1);
			theApp.UpdateStatusBar("TEXT:Place Text");
			break;
		case DRAWSTATE_PLACE:
			m_P1 = pASV->m_SnapPos;
			GetAttributes((TextAttributes *)pAdrbData);
			pASV->AddObjectIntoDoc(this);
			pASV->m_CadObj.pCadText = 0;
			pASV->EnableAutoScroll(0);
			DrawState = DRAWSTATE_WAITFORMOUSE_DOWN;
			theApp.UpdateStatusBar("TEXT:Move Text");
			pASV->Invalidate();
			break;
	}
	return DrawState;
}

int CCadText::MouseMove(CAutoScrollView * pASV, int DrawState)
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
		case DRAWSTATE_PLACE:
			m_P1 = pASV->m_SnapPos;
			pASV->Invalidate();
			break;
	}
	return DrawState;
}

void CCadText::SetText(char *s)
{
	///----------------------------------------
	/// SetText
	///		This function changes the text that
	/// is displayed by the CCadTexst oibject
	///
	/// parameter:
	///		s......pointer to string to display
	///----------------------------------------
	strcpy_s(m_Attrb.m_pText, 256, s);
	/// create the selection rectangle
	CRect rect = GetTextRectangle();
	Rotate(m_Attrb.m_Angle, rect, m_SelRect);
}

void CCadText::SetAngle(int e)
{
	///----------------------------------------
	/// SetAngle
	///		This function sets the angle to paint
	/// the text at.
	///
	/// parameters:
	///		e.....Angle, in tenths of a degree
	///----------------------------------------
	m_Attrb.m_Angle = e;
	CRect rect = GetTextRectangle();
	this->Rotate(m_Attrb.m_Angle, rect, this->m_SelRect);
}

void CCadText::GetText(char *s, int n)
{
	///---------------------------------------
	/// GetText
	///		This function retrieves the text
	/// thatis being displayed.
	///
	/// parameters:
	///		s.....pointer to array to put text into
	///		n.....maximum number of chars to copy
	///---------------------------------------
	strncpy(s, m_Attrb.m_pText, n);
}

char * CCadText::GetText()
{
	///----------------------------------------
	/// GetText
	///		This function returns a pointer to
	/// the string that is being displayed by
	/// by the text object
	///----------------------------------------
	return this->m_Attrb.m_pText;
}

void CCadText::SetFontName(char *s)
{
	///-----------------------------------------
	/// SetFontName
	///		This function is useed to set the
	/// name of the font that is used to paint
	/// the text.
	///
	/// parameter:
	///		s.....pointer to new font name
	///-----------------------------------------
	strcpy_s(m_Attrb.m_pFontName, LF_FACESIZE, s);
}

void CCadText::Rotate(int Angle, CRect rect, CCadPolygon &Poly)
{
	///----------------------------------------------
	/// Rotate
	///		Thhis function rotates the selection
	/// rectangle.
	///
	///		parameters:
	///			Angle....Angle to rotate by in tenths of a degree
	///			rect.....None Rotated Rectangle
	///			Poly.....Reference to a Polygon that
	///						The rotated rectangle is put in
	///----------------------------------------------
	Poly.Reset();
	double Ang = double(Angle) / 10.0;
	double Pi = atan(1.0) * 4;
	Ang = Ang * Pi / 180.0;
	CPoint p, p1;
	p.x = (int)(rect.Width() * cos(Ang)) + rect.TopLeft().x;
	p.y = -(int)(rect.Width() * sin(Ang)) + rect.TopLeft().y;
	Poly.AddPoint(p);
	Poly.AddPoint(rect.TopLeft());
	p1.x = (int)(rect.Height() * sin(Ang)) + rect.TopLeft().x;
	p1.y = (int)(rect.Height() * cos(Ang)) + rect.TopLeft().y;
	Poly.AddPoint(p1);
	p.x = (int)(rect.Width() * cos(Ang)) + rect.TopLeft().x;
	p.y = -(int)(rect.Width() * sin(Ang)) + rect.TopLeft().y;
	p = p + p1 - rect.TopLeft();
	Poly.AddPoint(p);
}


CRect CCadText::GetTextRectangle()
{
	///------------------------------------------
	/// GetTextRectangle
	///		This function is used to get the
	/// rectangle that surrounds the text displayed
	/// bu the text object.
	///------------------------------------------
	CWnd *pW = AfxGetMainWnd();
//	if ( ((CMainFrame *)pW)->MDIGetActive()->GetActiveView())
//		pW = ((CMainFrame *)pW)->MDIGetActive()->GetActiveView();
	CDC *pDC = pW->GetDC();
	CRect rect = GetTextRectangle(pDC, CScale(1.0, 1.0));
	pW->ReleaseDC(pDC);
	return rect;
}

CRect CCadText::GetTextRectangle(CDC *pDC, CScale Scale)
{
	///------------------------------------------
	/// GetTextRectangle
	///		This function is used to get the
	/// rectangle that surrounds the text displayed
	/// bu the text object.
	///
	///	parameters:
	///		pDC.....pointer to the device context
	///		Scale...Scale factor for display
	///------------------------------------------
	CFont font, *old;
	font.CreateFont(int(m_Attrb.m_FontHeight * Scale.m_ScaleX), int(m_Attrb.m_FontWidth * Scale.m_ScaleX), 0, 0,
		m_Attrb.m_Weight, 0, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH,
		LPCTSTR(m_Attrb.m_pFontName));
	old = pDC->SelectObject(&font);
	CSize cz = pDC->GetTextExtent(LPCTSTR(m_Attrb.m_pText), strlen(m_Attrb.m_pText));
	pDC->SelectObject(old);
	CRect rect;
	Scale.m_ScaleX = 1.0 / Scale.m_ScaleX;
	Scale.m_ScaleY = 1.0 / Scale.m_ScaleY;
	cz = Scale * cz;
	rect.SetRect(m_P1, m_P1 + cz);
	rect.NormalizeRect();
	return rect;
}

CPoint CCadText::CalcTextShiftonRotation(CPoint p1, CPoint Center, double angle)
{
	///--------------------------------------------
	///	CalcTextShiftonRotation
	///		This function is just a little complicated.
	/// What this does is it calculates the point to
	/// draw text at for a rotated text object.  This
	/// makes it so the text apears to be centered
	/// at point p1 at the given angle from the
	/// center.
	///
	/// parameters:
	///		p1...place you want text
	///		Center...Center of the text rectangle
	///		Angle...Angle in degree
	///
	/// returns:
	///		point at wich to paint text.
	///--------------------------------------------
	CPoint rP;
	double r2;
	double dx = double(p1.x - Center.x);
	double dy = double(p1.y - Center.y);

	r2 = sqrt(dx*dx + dy*dy);
	rP.x = int(r2 * cos(angle - 90.0));
	rP.y = int(r2 * sin(angle - 90.0));
	return p1 - rP;
}

void CCadText::UpdateText(void)
{
	CRect rect;

	rect = GetTextRectangle();
	Rotate(m_Attrb.m_Angle, rect, m_SelRect);
}

int CCadText::EditProperties(void)
{
	int Id;
	CDlgTextProperties Dlg;

	GetAttributes(&Dlg.m_Attr);
	Id = Dlg.DoModal();
	if (IDOK == Id)
		CopyAttributes(&Dlg.m_Attr);
	return Id;
}