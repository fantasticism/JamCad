#include "stdafx.h"
#include "CadPin.h"
#include "PropField.h"
#include "PropString.h"
#include "PropSelInt.h"
#include "PropSelString.h"
#include "PropInt.h"
#include "CadText.h"
#include "JamCad.h"
#include "DlgPinProp.h"

int CCadPin::m_RenderEnable = 1;
//--------------------------------------
//for defining an inversion circle
//---------------------------------------
static CRect rInvDot = CRect(CPoint(80,-40),CPoint(0,40));
//---------------------------------------
// Indicator for clock input
//---------------------------------------
static CPoint tClock[3] = {
	CPoint(2,48),
	CPoint(-50,0),
	CPoint(2,-48)
};
//---------------------------------------
// shows where the connection point on
// the pin is
//--------------------------------------
static CSize connectionRect = CSize(20, 20);


PrepSelIntData PinStyleInitData[] = {
	{ "Line",PIN_STYLE_LINE },
	{ "Inverted",PIN_STYLE_INVERTED },
	{ "Clock",PIN_STYLE_CLOCK },
	{ "Inverted Clock",PIN_STYLE_CLOCKINV },
	{ 0,0 }
};

PrepSelIntData PinTypeInitData[] = {
	{"Input",PIN_TYPE_INPUT },
	{"Output",PIN_TYPE_OUTPUT },
	{"Bidirectional",PIN_TYPE_BIDIRECTIONAL },
	{"Passive",PIN_TYPE_PASIVE },
	{"Power",PIN_TYPE_POWER },
	{0,0}
};

PrepSelIntData PinRotationData[] = {
	{"Left",PIN_ORIEN_LEFT },
	{"Right",PIN_ORIEN_RIGHT },
	{"Up",PIN_ORIEN_UP},
	{"Down",PIN_ORIEN_DOWN},
	{0,0}
};

//-------------------------------------------
// Default Propery field names
//-------------------------------------------
static FieldNameType DefPropNames[] = {
	{ PropFieldNames [PF_NAME_UNITS],PROP_INT,TOKEN_NUMUNITS,PROPFIELD_DISPLAY,(void *)1,&theApp.m_SchTextAttr },
	{ PropFieldNames[PF_NAME_PINNAME],PROP_STRING,TOKEN_PINNAME,PROPFIELD_DISPLAY,(void *)(""),&theApp.m_SchTextAttr },
	{ PropFieldNames[PF_NAME_PINNUMBER],PROP_STRING,TOKEN_PINNUMBER,PROPFIELD_DISPLAY,(void *)("") ,&theApp.m_SchTextAttr },
	{ PropFieldNames[PF_NAME_PINLEN],PROP_INT,TOKEN_PINLENGTH,PROPFIELD_DISPLAY,(void *)300,&theApp.m_SchTextAttr },
	{ PropFieldNames[PF_NAME_PINROTATION],PROP_SELECTOR_INT,TOKEN_PINROTATION,PROPFIELD_DISPLAY,(void *)PinRotationData ,&theApp.m_SchTextAttr },
	{ PropFieldNames[PF_NAME_PINTYPE],PROP_SELECTOR_INT,TOKEN_PINTYPE,PROPFIELD_DISPLAY,(void *)PinTypeInitData ,&theApp.m_SchTextAttr },
	{ PropFieldNames[PF_NAME_PINSTYLE],PROP_SELECTOR_INT,TOKEN_PINSTYLE,PROPFIELD_DISPLAY,(void *)PinStyleInitData ,&theApp.m_SchTextAttr },
	{0,PROP_NONE }
};

int CCadPin::m_AutoIncEnable;
int CCadPin::m_NameInc;
int CCadPin::m_PinInc;
char CCadPin::m_LastPinName[32] = "";
char CCadPin::m_LastPinNumber[32] = "";
int CCadPin::m_LastOrientation=0;
int CCadPin::m_LastStyle=0;
int CCadPin::m_LastType=0;

CCadPin::CCadPin():CCadObject(OBJECT_TYPE_PIN)
{
	m_pLine = 0;
	m_pSelPen = 0;
	//------------------------------
	// add in standard properties
	//------------------------------
	GetProperties().Create(DefPropNames);
	m_pStyle = 0;
	m_pRot = 0;
	m_pType = 0;
	m_pPinLen = 0;
	m_pName = 0;
	m_pNumber = 0;
	m_pNumber = GetPinNumber();
	m_pName = GetPinName();
	m_pRot = GetPinRotation();
	m_pStyle = GetPinStyle();
	m_pType = GetPinType();
	m_pPinLen = GetPinLen();
	m_pName->m_P1 = CPoint(-300, -30);
	m_pNumber->m_P1 = CPoint(0, -60);
	m_pNumber->SetValue(CCadPin::m_LastPinNumber);
	m_pName->SetValue(CCadPin::m_LastPinName);
	if (CCadPin::m_AutoIncEnable)	//do the auto increment thing
	{
		m_pNumber->SetValue(CCadPin::m_LastPinNumber);
		IncString(m_pNumber->GetFieldValue(), 32, CCadPin::m_PinInc);
		m_pNumber->SetText(m_pNumber->GetFieldValue());
		m_pName->SetValue(CCadPin::m_LastPinName);
		IncString(m_pName->GetFieldValue(), 32, CCadPin::m_NameInc);
		m_pName->SetText(m_pName->GetFieldValue());
	}
	GetPinType()->SetSelection(CCadPin::m_LastType);
	GetPinStyle()->SetSelection(CCadPin::m_LastStyle);
	GetPinRotation()->SetSelection(CCadPin::m_LastOrientation);
	AdjustStrings();
}

CCadPin::~CCadPin()
{
}

void CCadPin::LoadSettings(PinAttributes *pAttrib)
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
	pAttrib->m_Color = theApp.GetProfileIntA("Pin", "Color", RGB(0, 0, 0));
	pAttrib->m_Width = theApp.GetProfileIntA("Pin", "Width", 6);
}

void CCadPin::SaveSettings(PinAttributes *pAttrib)
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
	theApp.WriteProfileInt("Pin", "Color", pAttrib->m_Color);
	theApp.WriteProfileInt("Pin","Width", pAttrib->m_Width);
}

void CCadPin::Move(CPoint p)
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
}

void CCadPin::Save(FILE * pO, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This function save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	fprintf(pO, "%s%s(%s(%d,%d))[\n",
		MkIndentString(Indent),
		TokeToString(TOKEN_PIN),
		TokeToString(TOKEN_POINT),
		m_P1.x,m_P1.y
		);
	GetProperties().Save(pO, Indent + 1,flags);
	fprintf(pO, "%s]\n", MkIndentString(Indent));
}

void CCadPin::Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale)
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
	CPropSelInt *pPS;
	CPropSelInt *pPT;
	CPropSelInt *pPR;

	if (CCadPin::m_RenderEnable)
	{
		//-------------------------------
		// Display main part of pin
		//-------------------------------
		pPS = GetPinStyle();
		pPT = GetPinType();
		pPR = GetPinRotation();
		switch (pPS->GetSelection())
		{
			case PIN_STYLE_LINE:
				DrawLine(pDC, mode, Offset, Scale, pPR->GetSelectedValue());
				break;
			case PIN_STYLE_INVERTED:
				DrawDot(pDC, mode, Offset, Scale, pPR->GetSelectedValue());
				break;
			case PIN_STYLE_CLOCK:
				DrawClock(pDC, mode, Offset, Scale, pPR->GetSelectedValue());
				break;
			case PIN_STYLE_CLOCKINV:
				DrawDotClock(pDC, mode, Offset, Scale, pPR->GetSelectedValue());
				break;
		}
		//--------------------------------
		// Display Properites
		//--------------------------------
		GetProperties().Draw(pDC, mode, Scale * m_P1 + Offset, Scale);
		//-------------------------------
		// Draw object retangle for DEBUG
		//-------------------------------
/*
		CPen Green,*pPold;
		Green.CreatePen(PS_SOLID, 10, RGB(0, 255,0));
		CBrush Br,*pBrOld;
		Br.CreateStockObject(NULL_BRUSH);
		pBrOld = pDC->SelectObject(&Br);
		pPold = pDC->SelectObject(&Green);
		CRect r = GetRect();
		CRect rD;
		CPoint ul, lr;
		ul = r.TopLeft();
		lr = r.BottomRight();
		ul = Scale * ul + Offset;
		lr = Scale * lr + Offset;
		rD.SetRect(ul, lr);
		pDC->Rectangle(&rD);
		pDC->SelectObject(pBrOld);
		pDC->SelectObject(pPold);
		*/
	}
}

int CCadPin::CheckSelected(CPoint p, CSize Offset, CCadObject ** ppSelList , int index, int n, int flag)
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
	if ((index < n) || n == 0)
	{
		rect = GetRect();
		if (rect.PtInRect(p + Offset))
		{
			if (ppSelList)
			{
				switch (flag)
				{
					case CADDRAWUBG_CHECKSEL_FLAG_ALL:
						ppSelList[index++] = this;
						break;
					case CADDRAWUBG_CHECKSEL_FLAG_SEL:
						if(IsSelected())
							ppSelList[index++] = this;
						break;
					case CADDRAWUBG_CHECKSEL_FLAG_UNSEL:
						if(!IsSelected())
							ppSelList[index++] = this;
						break;
				}
			}
		}
	}
	return index;
}

CPoint CCadPin::GetReference()
{
	//***************************************************
	// GetReference
	//	This function returns the reference point for
	// the object
	// parameters:none
	//
	// return value:reference point
	//--------------------------------------------------
	return CPoint();
}

int CCadPin::IsDirty(void)
{
	//***************************************************
	// IsDirty
	//	returns the status of the Dirty State of the
	// object
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return CCadObject::IsDirty();
}

void CCadPin::SetDirty(int d)
{
	//***************************************************
	// SetDirty
	// Changes the state of the dirty flag
	// parameters:
	//	d.......new state of dirty flag.
	//
	// return value:
	//--------------------------------------------------
	CCadObject::SetDirty(d);
}

int CCadPin::IsSelected()
{
	//***************************************************
	// IsSelected
	//	returns the state of the selected flag
	// parameters:
	//
	// return value:State of the selected flag
	//--------------------------------------------------
	return CCadObject::IsSelected();
}

void CCadPin::SetSelected(int Flag)
{
	//***************************************************
	// SetSelected
	//	Changes the state of the selcted flag.
	// parameters:
	// Flag......New state of the selected flag
	//
	// return value:
	//--------------------------------------------------
	CCadObject::SetSelected(Flag);
}

void CCadPin::AdjustRefernce(CPoint Ref)
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

CRect CCadPin::GetRect(void)
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
	CRect rectProp = GetProperties().GetRect();
	rectProp.OffsetRect(m_P1);
	CRect rect, rectPin;
	rectPin = GetPinRect();
	rect.UnionRect(&rectPin, &rectProp);
	return rect;
}

char * CCadPin::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	return (char *)"Pin";
}

CCadPin CCadPin::operator=(CCadPin &v)
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
	CopyPropertyValues(v.GetProperties());
	return *this;
}

CCadObject * CCadPin::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadPin *pCP = new CCadPin;
	pCP->m_P1 = m_P1;
	pCP->m_P2 = m_P1;
	pCP->CopyPropertyValues(GetProperties());
	return pCP;
}

void CCadPin::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------

}

void CCadPin::RenderEnable(int e)
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
	CCadPin::m_RenderEnable = e;
}

CPoint CCadPin::GetCenter()
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

void CCadPin::ChangeCenter(CSize p)
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

int CCadPin::Parse(int Token, CLexer *pLex)
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

CCadObject * CCadPin::NewInstance()
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

void CCadPin::GetAttributes(PinAttributes *pAttrb)
{
	/***************************************************
	*	GetAttributes
	*		This function is used to copy the
	*	attributes from this object into the one pointed 
	**  to by the parameter
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy
	***************************************************/
}

void CCadPin::CopyAttributes(PinAttributes *pAttrb)
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
	ClearNeedsAttributes();
}

int CCadPin::MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
			m_P1 = pASV->m_SnapPos;
			break;
		case DRAWSTATE_WAITFORMOUSE_DOWN:
			m_P1 = pASV->m_SnapPos;
			break;
		case DRAWSTATE_PLACE:
			m_P1 = pASV->m_SnapPos;
			break;
		case DRAWSTATE_PLACE_AUTO:
			m_P1 = pASV->m_SnapPos;
			pASV->Invalidate();
			break;
	}
	return DrawState;
}

int CCadPin::MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
	switch (DrawState)
	{
		case DRAWSTATE_SET_ATTRIBUTES:
			{
				theApp.UpdateStatusBar("PIN:Set Pin Properties");
				CDlgPinProp dlg;
				m_P1 = CPoint(0, 0);
				dlg.m_pPin = this;
				if (IDOK == dlg.DoModal())
				{
					strcpy(CCadPin::m_LastPinNumber, GetPinNumber()->GetFieldValue());
					strcpy(CCadPin::m_LastPinName, GetPinName()->GetFieldValue());
					CCadPin::m_LastOrientation = GetPinRotation()->GetSelection();
					CCadPin::m_LastStyle = GetPinStyle()->GetSelection();
					CCadPin::m_LastType = GetPinType()->GetSelection();
					if (CCadPin::m_AutoIncEnable)
					{
						DrawState = DRAWSTATE_PLACE_AUTO;
						theApp.UpdateStatusBar("PIN AUTO INC:Place Pin");
					}
					else
					{
						DrawState = DRAWSTATE_PLACE;
						theApp.UpdateStatusBar("PIN:Place Pin");
					}
					m_P1 = pASV->m_SnapPos;
				}
			}
			break;
		case DRAWSTATE_WAITFORMOUSE_DOWN:
			m_P1 = pASV->m_SnapPos;
			if (CCadPin::m_AutoIncEnable)
			{
				DrawState = DRAWSTATE_PLACE_AUTO;
				theApp.UpdateStatusBar("PIN AUTO INC:Place Pin");
			}
			else
			{
				DrawState = DRAWSTATE_PLACE;
				theApp.UpdateStatusBar("PIN:Place Pin");
			}
			pASV->Invalidate();
			break;
		case DRAWSTATE_PLACE:
			m_P1 = pASV->m_SnapPos;
			pASV->AddObjectIntoDoc(this);
			strcpy(CCadPin::m_LastPinNumber, GetPinNumber()->GetFieldValue());
			strcpy(CCadPin::m_LastPinName, GetPinName()->GetFieldValue());
			pASV->m_CadObj.pCadObject = 0;
			DrawState = DRAWSTATE_SET_ATTRIBUTES;
			theApp.UpdateStatusBar("PIN:Place Pin");
			pASV->Invalidate();
			break;
		case DRAWSTATE_PLACE_AUTO:
			m_P1 = pASV->m_SnapPos;
			pASV->AddObjectIntoDoc(this);
			strcpy(CCadPin::m_LastPinNumber, GetPinNumber()->GetFieldValue());
			strcpy(CCadPin::m_LastPinName, GetPinName()->GetFieldValue());
			pASV->m_CadObj.pCadObject = 0;
			DrawState = DRAWSTATE_WAITFORMOUSE_DOWN;
			pASV->Invalidate();
			break;
	}
	return DrawState;
}

int CCadPin::MouseMove(CAutoScrollView * pASV, int DrawState)
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
			m_P1 = pASV->m_SnapPos;
			break;
		case DRAWSTATE_WAITFORMOUSE_DOWN:
			m_P1 = pASV->m_SnapPos;
			break;
		case DRAWSTATE_PLACE:
			m_P1 = pASV->m_SnapPos;
			break;
		case DRAWSTATE_PLACE_AUTO:
			m_P1 = pASV->m_SnapPos;
			break;
	}
	pASV->Invalidate();
	return DrawState;
}

CCadObject *CCadPin::NewPinWithAutoInc(int NameInc,int PinNumInc)
{
	CCadPin *pCP = new CCadPin;
	return (CCadObject *)pCP;
}

CPropSelInt *CCadPin::GetPinStyle(void)
{
	CPropSelInt *pPS = m_pStyle;
	if (!pPS)
	{
		//------------------------------------
		// this is to just speed things up
		// in future references to this property
		//-------------------------------------
		m_pStyle = pPS = (CPropSelInt *)GetProperties().FindProperty(GetPropertyName(TOKEN_PINSTYLE));
	}
	return pPS;
}

CPropSelInt *CCadPin::GetPinType(void)
{
	CPropSelInt *pPT = m_pType;
	if (!pPT)
	{
		pPT = m_pType = (CPropSelInt *)GetProperties().FindProperty(GetPropertyName(TOKEN_PINTYPE));
	}
	return pPT;
}

CPropInt *CCadPin::GetPinLen()
{
	CPropInt *pPI = m_pPinLen;
	if (!pPI)
		pPI = m_pPinLen = (CPropInt *)GetProperties().FindProperty(GetPropertyName(TOKEN_PINLENGTH));
	return pPI;
}

CPropString *CCadPin::GetPinNumber()
{
	CPropString *pPN = m_pNumber;
	if (pPN == 0)
		pPN = m_pNumber = (CPropString *)GetProperties().FindProperty(GetPropertyName(TOKEN_PINNUMBER));
	return pPN;
}

CPropString *CCadPin::GetPinName()
{
	CPropString *pPN = m_pName;
	if (pPN == 0)
		pPN = m_pName = (CPropString *)GetProperties().FindProperty(GetPropertyName(TOKEN_PINNAME));
	return pPN;
}

CPropSelInt *CCadPin::GetPinRotation()
{
	CPropSelInt *pPR = m_pRot;
	if (!pPR)
		pPR = m_pRot = (CPropSelInt *)GetProperties().FindProperty(GetPropertyName(TOKEN_PINROTATION));
	return pPR;
}

void CCadPin::DrawLine(CDC *pDC, int mode, CSize Offset, CScale Scale,int Rotation)
{
	CPoint P1,P2;
	CSize Len;
	CPen *pOld;
	CBrush *pOldBr;
	CRect rect;

	P1 = Scale*RotatePoint(m_P1,m_P1,Rotation) + Offset;
	Len = CSize(m_pPinLen->GetValue(), 0);
	P2 = Scale * RotatePoint(m_P1, m_P1 + Len, Rotation) +Offset;

	if (!IsLastModeSame(mode) || IsDirty())
	{
		switch (mode)
		{
			case OBJECT_MODE_SELECTED:
				if (m_pSelPen) delete m_pSelPen;
				m_pSelPen = new CPen;
				m_pSelPen->CreatePen(PS_SOLID, 1, RGB(55, 100, 255));
			case OBJECT_MODE_FINAL:
				if (m_pLine) delete m_pLine;
				m_pLine = new CPen;
				m_pLine->CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
				break;
			case OBJECT_MODE_SKETCH:
				break;
		}
		SetDirty(0);
		SetLastMode(mode);
	}
	switch (mode)
	{
		case OBJECT_MODE_FINAL:
		case OBJECT_MODE_SKETCH:
			pOld = pDC->SelectObject(m_pLine);
			pDC->MoveTo(P1);
			pDC->LineTo(P2);
			pDC->SelectObject(pOld);
			DrawConnection(pDC, mode, Offset, Scale, RotatePoint(m_P1,m_P1 + CPoint( m_pPinLen->GetValue(),0),Rotation) );
			break;
		case OBJECT_MODE_SELECTED:
			pOldBr = (CBrush *)pDC->SelectStockObject(NULL_BRUSH);
			pOld = pDC->SelectObject(m_pSelPen);
			rect = ScaleRect(GetRect(), Scale, Offset);
			pDC->Rectangle(&rect);
			pDC->SelectObject(pOldBr);
			pDC->SelectObject(m_pLine);
			pDC->MoveTo(P1);
			pDC->LineTo(P2);
			pDC->SelectObject(pOld);
			DrawConnection(pDC, mode, Offset, Scale, RotatePoint(m_P1, m_P1 + CPoint(m_pPinLen->GetValue(), 0), Rotation));
			break;
	}
}

void CCadPin::DrawDot(CDC *pDC, int mode, CSize Offset, CScale Scale, int Rotation)
{
	CPen *pOld;
	CPoint P1,P2;
	CSize Len;
	CRect DotRect;
	P1 = Scale*(RotatePoint(m_P1, m_P1 - CSize(rInvDot.Width(),0),Rotation)) + Offset;
	Len = CSize(m_pPinLen->GetValue(), 0);
	P2 = Scale * RotatePoint(m_P1, m_P1 + Len,Rotation) + Offset;
	DotRect.SetRect(Scale * RotatePoint(m_P1, m_P1 + rInvDot.TopLeft(),Rotation) + Offset, Scale * RotatePoint(m_P1, m_P1 + rInvDot.BottomRight(),Rotation)+Offset);
	DotRect.NormalizeRect();

	Len = Scale * Len;

	if (!IsLastModeSame(mode) || IsDirty())
	{
		switch (mode)
		{
			case OBJECT_MODE_FINAL:
				if (m_pLine) delete m_pLine;
				m_pLine = new CPen;
				m_pLine->CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
				break;
			case OBJECT_MODE_SKETCH:
				break;
			case OBJECT_MODE_SELECTED:
				break;
		}
		SetDirty(0);
		SetLastMode(mode);
	}
	CRect rT;
	switch (mode)
	{
		case OBJECT_MODE_FINAL:
		case OBJECT_MODE_SKETCH:
			pOld = pDC->SelectObject(m_pLine);
			pDC->Ellipse(&DotRect);
			pDC->MoveTo(P1);
			pDC->LineTo(P2);
			pDC->SelectObject(pOld);
			DrawConnection(pDC, mode, Offset, Scale, RotatePoint(m_P1,m_P1 + CPoint(m_pPinLen->GetValue(), 0),Rotation));
			break;
		case OBJECT_MODE_SELECTED:
			break;
	}
}

void CCadPin::DrawClock(CDC *pDC, int mode, CSize Offset, CScale Scale, int Rotation)
{
	CPoint P1,P2;
	CSize Len;
	CPen *pOld;
	P1 = Scale*(RotatePoint(m_P1, m_P1 , Rotation)) + Offset;
	Len = CSize(m_pPinLen->GetValue(), 0);
	P2 = Scale * RotatePoint(m_P1, m_P1 + Len, Rotation) + Offset;

	Len = Scale * Len;

	if (!IsLastModeSame(mode) || IsDirty())
	{
		switch (mode)
		{
		case OBJECT_MODE_FINAL:
			if (m_pLine) delete m_pLine;
			m_pLine = new CPen;
			m_pLine->CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
			break;
		case OBJECT_MODE_SKETCH:
			break;
		case OBJECT_MODE_SELECTED:
			break;
		}
		SetDirty(0);
		SetLastMode(mode);
	}
	switch (mode)
	{
	case OBJECT_MODE_FINAL:
		//			break;
	case OBJECT_MODE_SKETCH:
		pOld = pDC->SelectObject(m_pLine);
		pDC->MoveTo(P1);	//start of pin
		pDC->LineTo(P2);	// end of pin
		// Draw clock thingy
		pDC->MoveTo(Scale * RotatePoint(m_P1, m_P1 + tClock[0], Rotation) + Offset);
		pDC->LineTo(Scale * RotatePoint(m_P1, m_P1 + tClock[1], Rotation) + Offset);
		pDC->LineTo(Scale * RotatePoint(m_P1, m_P1 + tClock[2], Rotation) + Offset);
		pDC->SelectObject(pOld);
		DrawConnection(pDC, mode, Offset, Scale, RotatePoint(m_P1, m_P1 + CPoint(m_pPinLen->GetValue(), 0),Rotation));
		break;
	case OBJECT_MODE_SELECTED:
		break;
	}
}

void CCadPin::DrawDotClock(CDC *pDC, int mode, CSize Offset, CScale Scale, int Rotation)
{
	CPen *pOld;
	CPoint P1,P2;
	CSize Len;
	CRect DotRect;
	P1 = Scale*(RotatePoint(m_P1, m_P1 - CSize(rInvDot.Width(), 0), Rotation)) + Offset;
	Len = CSize(m_pPinLen->GetValue(), 0);
	P2 = Scale * RotatePoint(m_P1, m_P1 + Len, Rotation) + Offset;
	DotRect.SetRect(Scale * RotatePoint(m_P1, m_P1 + rInvDot.TopLeft(), Rotation) + Offset, Scale * RotatePoint(m_P1,m_P1 + rInvDot.BottomRight(), Rotation) + Offset);
	DotRect.NormalizeRect();

	Len = Scale * Len;

	if (!IsLastModeSame(mode) || IsDirty())
	{
		switch (mode)
		{
		case OBJECT_MODE_FINAL:
			if (m_pLine) delete m_pLine;
			m_pLine = new CPen;
			m_pLine->CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
			break;
		case OBJECT_MODE_SKETCH:
			break;
		case OBJECT_MODE_SELECTED:
			break;
		}
		SetDirty(0);
		SetLastMode(mode);
	}
	CRect rT;
	switch (mode)
	{
	case OBJECT_MODE_FINAL:
	case OBJECT_MODE_SKETCH:
		pOld = pDC->SelectObject(m_pLine);
		pDC->Ellipse(&DotRect);	//DRAW DOT
		pDC->MoveTo(P1);	//start of pin
		pDC->LineTo(P2);	//end of pin
		// Draw clock thingy
		pDC->MoveTo(Scale * RotatePoint(m_P1,m_P1 + tClock[0],Rotation) + Offset);
		pDC->LineTo(Scale * RotatePoint(m_P1, m_P1 + tClock[1], Rotation) + Offset);
		pDC->LineTo(Scale * RotatePoint(m_P1, m_P1 + tClock[2], Rotation) + Offset);
		pDC->SelectObject(pOld);
		DrawConnection(pDC, mode, Offset, Scale, RotatePoint(m_P1, m_P1 + CPoint(m_pPinLen->GetValue(), 0),Rotation));
		break;
	case OBJECT_MODE_SELECTED:
		break;
	}
}

void CCadPin::SetProperty(char *name, char *value)
{
	CPropField *pF = GetProperties().FindProperty(name);
	if (pF)
	{
		switch (pF->GetType())
		{
			case PROP_STRING:
				pF->SetValue(value);
				break;
			case PROP_INT:
				((CPropInt *)pF)->SetValue(atoi(value));
				break;
			case PROP_SELECTOR_INT:
				break;
			case PROP_SELECTOR_STRING:
				break;
		}
	}
}

void CCadPin::DrawConnection(CDC * pDC, int mode, CSize Offset, CScale Scale,CPoint point)
{
	CRect rect;
	CPoint p1 = Scale*(point - connectionRect) + Offset;
	CPoint p2 = Scale*(point + connectionRect) + Offset;
	CBrush br,*pOldBr;
	br.CreateStockObject(NULL_BRUSH);
	pOldBr = pDC->SelectObject(&br);
	rect.SetRect(p1, p2);
	pDC->Rectangle(&rect);
	pDC->SelectObject(pOldBr);
}

void CCadPin::Rotate(int Direction)
{
	CPropSelInt *pRot = GetPinRotation();
	pRot->SetSelection(pRot->GetSelection() + Direction);
}

void CCadPin::AdjustStrings()
{
	CPropString *pName = GetPinName();
	CPropString *pNumber = GetPinNumber();
	CRect rectNum,rectName;
	int TextRot;
	int wNum, wName,hNum, hName;
	int Rotation = GetPinRotation()->GetSelectedValue();
	int Rot;
	//-----------------------------------------------------
	// because we want to be able to read the text, we
	// need to limit the oritentations
	//-----------------------------------------------------
	switch (Rotation)
	{
		case ROTATION_0:
			Rot = 0;
			TextRot = Rotation;
			break;
		case ROTATION_90:
			Rot = 900;
			TextRot = Rotation;
			break;
		case ROTATION_180:
			Rot = 0;
			TextRot = ROTATION_0;
			break;
		case ROTATION_270:
			Rot = 900;
			TextRot = ROTATION_90;
			break;
	}
	rectName = pName->GetTextRectangle();
	rectNum = pNumber->GetTextRectangle();
	switch (GetPinStyle()->GetSelectedValue())
	{
		case PIN_STYLE_LINE:
			switch (Rotation)
			{
				case ROTATION_0:
					hName = -rectName.Height() / 2;
					hNum = -rectNum.Height();
					wName = 20 + rectName.Width();
					wNum = 20;
					break;
				case ROTATION_90:	//Up facing pin
					hName = rectName.Height() / 2;
					hNum = rectNum.Height();
					wName = 20;
					wNum = 20 + rectNum.Width();
					break;
				case ROTATION_180:
					hName = -rectName.Height() / 2;
					hNum = -rectNum.Height();
					wName = -20;
					wNum = -20-rectNum.Width();
					break;
				case ROTATION_270:	//Down facing pin
					hName = rectName.Height() / 2;
					hNum = rectNum.Height();
					wName = -40 - rectName.Width();
					wNum = -20 ;
					break;
			}
			break;
		case PIN_STYLE_INVERTED:
			switch (Rotation)
			{
				case ROTATION_0:
					hName = -rectName.Height() / 2;
					hNum = -rectNum.Height();
					wName = 20 + rectName.Width();
					wNum = 80;
					break;
				case ROTATION_90:	//Up facing pin
					hName = rectName.Height() / 2;
					hNum = rectNum.Height();
					wName = 20;
					wNum = 80 + rectNum.Width();
					break;
				case ROTATION_180:
					hName = -rectName.Height() / 2;
					hNum = -rectNum.Height();
					wName = -20;
					wNum = -80 - rectNum.Width();
					break;
				case ROTATION_270:	//Down facing pin
					hName = rectName.Height() / 2;
					hNum = rectNum.Height();
					wName = -40 - rectName.Width();
					wNum = -80;
					break;
			}
			break;
		case PIN_STYLE_CLOCK:
			switch (Rotation)
			{
				case ROTATION_0:
					hName = -rectName.Height() / 2;
					hNum = -rectNum.Height();
					wName = 70 + rectName.Width();
					wNum = 20;
					break;
				case ROTATION_90:	//Up facing pin
					hName = rectName.Height() / 2;
					hNum = rectNum.Height();
					wName = 70;
					wNum = 20 + rectNum.Width();
					break;
				case ROTATION_180:
					hName = -rectName.Height() / 2;
					hNum = -rectNum.Height();
					wName = -70;
					wNum = -20 - rectNum.Width();
					break;
				case ROTATION_270:	//Down facing pin
					hName = rectName.Height() / 2;
					hNum = rectNum.Height();
					wName = -70 - rectName.Width();
					wNum = 20;
					break;
				}
			break;
		case PIN_STYLE_CLOCKINV:
			switch (Rotation)
			{
				case ROTATION_0:
					hName = -rectName.Height() / 2;
					hNum = -rectNum.Height();
					wName = 70 + rectName.Width();
					wNum = 80;
					break;
				case ROTATION_90:	//Up facing pin
					hName = rectName.Height() / 2;
					hNum = rectNum.Height();
					wName = 70;
					wNum = 80 + rectNum.Width();
					break;
				case ROTATION_180:
					hName = -rectName.Height() / 2;
					hNum = -rectNum.Height();
					wName = -70;
					wNum = -80 - rectNum.Width();
					break;
				case ROTATION_270:	//Down facing pin
					hName = rectName.Height() / 2;
					hNum = rectNum.Height();
					wName = -70 - rectName.Width();
					wNum = -80;
					break;
			}
			break;
	}
	pName->m_P1 = RotatePoint(m_P1, CPoint(-wName,hName),TextRot);
	pName->SetAngle(Rot);
	pNumber->m_P1 = RotatePoint(m_P1, CPoint(wNum,hNum ), TextRot);
	pNumber->SetAngle(Rot);
}


void CCadPin::SetPinNameFontSize(int Height, int Width, int Weight)
{
	GetPinName()->SetFontSize(Height, Width, Weight);
	AdjustStrings();
}


void CCadPin::SetPinNumberFontSize(int Height, int Width, int Weight)
{
	GetPinNumber()->SetFontSize(Height, Width, Weight);
	AdjustStrings();
}


void CCadPin::IncString(char * s, int len, int inc)
{
	//----------------------------------------------
	// IncString
	//		This function is used to autoincrement
	//	a string.
	//
	// parameter:
	//	s..........string to increment
	//	len........size of array of s
	//	inc........amount to imcrement by
	//----------------------------------------------
	char *pNum = 0;
	int Digit = 0;
	int l = strlen(s)-1;
	int i;
	int loop;
	for (i = l,loop = 1; i >= 0 && loop; --i)
	{
		if (isdigit(s[i]))
		{
			if (i == 0 && l == 0)
			{
				loop = 0;
				pNum = s;
			}
			else if (i == 0 && Digit)	//we have reachced the end of the string
			{
				loop = 0;
				pNum = s;
			}
			Digit = 1;
		}
		else if(Digit)
		{
			//end of numberic part.
			pNum = &s[i + 1];
			loop = 0;
		}
	}
	if(pNum)
	{
		int v = atoi(pNum);
		v += inc;
		char *num = new char[32];
		sprintf_s(num,32, "%d", v);
		*pNum = 0;
		strcat(s, num);
		delete[] num;
	}
}

int CCadPin::EditProperties(void)
{
	CDlgPinProp dlg;
	int Id;
	m_P1 = CPoint(0, 0);
	dlg.m_pPin = this;
	if ((Id = dlg.DoModal()) == IDOK)
	{
		strcpy(CCadPin::m_LastPinNumber, GetPinNumber()->GetFieldValue());
		strcpy(CCadPin::m_LastPinName, GetPinName()->GetFieldValue());
		CCadPin::m_LastOrientation = GetPinRotation()->GetSelection();
		CCadPin::m_LastStyle = GetPinStyle()->GetSelection();
		CCadPin::m_LastType = GetPinType()->GetSelection();
	}
	return Id;
}

CRect CCadPin::GetPinRect()
{
	CPoint P1, P2;
	P1 = m_P1 - CSize(0, 40);
	P2 = m_P1 + CSize(m_pPinLen->GetValue(), 40);
	CRect rect;
	rect.SetRect(RotatePoint(m_P1, P1, m_pRot->GetSelectedValue()), RotatePoint(m_P1, P2, m_pRot->GetSelectedValue()));
	rect.NormalizeRect();
	return rect;
}

char *CCadPin::GetPropertyName(int Token)
{
	return GetProperties().GetPropertyName(Token, DefPropNames);
}

CPropField *CCadPin::FindProperty(char *name)
{
	return GetProperties().FindProperty(name);
}
