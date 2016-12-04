#include "stdafx.h"
#include "CadPcbComp.h"

void CCadPcbComp::LoadSettings(PcbCompAttributes *pAttrib)
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
}

void CCadPcbComp::SaveSettings(PcbCompAttributes *pAttrib)
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


CCadPcbComp::CCadPcbComp()
{
}


CCadPcbComp::~CCadPcbComp()
{
}

void CCadPcbComp::Move(CPoint p)
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

void CCadPcbComp::Save(FILE * pO, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This function save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
}

void CCadPcbComp::SetVertex(int v, CPoint p)
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


int CCadPcbComp::GrabVertex(CPoint p)
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


void CCadPcbComp::Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale)
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
}

int CCadPcbComp::CheckSelected(CPoint p, CSize Offset, CCadObject ** ppSelList , int index, int n, int flag)
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
	return 0;
}

CPoint CCadPcbComp::GetReference()
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

void CCadPcbComp::AddObject(CCadObject *pO)
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

void CCadPcbComp::RemoveObject(CCadObject *pO)
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

CCadObject *CCadPcbComp::GetHead(void)
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

int CCadPcbComp::IsDirty(void)
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

void CCadPcbComp::SetDirty(int d)
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

int CCadPcbComp::IsSelected()
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

void CCadPcbComp::SetSelected(int Flag)
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

void CCadPcbComp::AdjustRefernce(CPoint Ref)
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

}

CRect CCadPcbComp::GetRect(void)
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

char * CCadPcbComp::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	return (char *)"Lib Part";
}

CCadPcbComp CCadPcbComp::operator=(CCadPcbComp &v)
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
	return CCadPcbComp();
}

CPoint CCadPcbComp::ScalePoint(CPoint p, int Scale, int ScaleDiv)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return CPoint();
}

CCadObject * CCadPcbComp::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadPcbComp *pPC = new CCadPcbComp;
	*pPC = *this;
	return pPC;
}

void CCadPcbComp::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------

}

void CCadPcbComp::RenderEnable(int e)
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

}

CCadObject *CCadPcbComp::PopHead()
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return 0;
}

CCadObject *CCadPcbComp::PopTail()
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return 0;
}

CPoint CCadPcbComp::GetCenter()
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	return CPoint();
}

void CCadPcbComp::ChangeCenter(CSize p)
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

CSize CCadPcbComp::GetSize()
{
	//***************************************************
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	return CSize();
}

void CCadPcbComp::ChangeSize(CSize Sz)
{
	//***************************************************
	// ChangeSize
	//	Change the size of the object
	// parameters:
	//	sz.....size to change object by (not change to)
	// return value:
	//--------------------------------------------------
}

int CCadPcbComp::Parse(int Token, CLexer *pLex)
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

CCadObject * CCadPcbComp::NewInstance()
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

void CCadPcbComp::CopyAttributes(PcbCompAttributes *pAttrb)
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

int CCadPcbComp::MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
	return DrawState;
}

int CCadPcbComp::MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
	return DrawState;
}

int CCadPcbComp::MouseMove(CAutoScrollView * pASV, int DrawState)
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
	return DrawState;
}
