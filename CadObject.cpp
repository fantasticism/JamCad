/****************************************************************
* CCadObject
*		This is the base class for all of the drawing objects
* They are all the same for this simple reason that any of
* them may exist in any document.  The idea is that they behave
* the same so as to make things just a bit easier on me while
* writing the program.  This class was derived from an earlier
* work that I called FrontCad.  This version is supposedely 
* much more object oriented.
****************************************************************/
#include "stdafx.h"
#include "CadObject.h"
#include "PropField.h"
#include "Lexer.h"
#include "PropInt.h"
#include "PropString.h"
#include "PropSelInt.h"
#include "PropSelString.h"

CCadObject::CCadObject()
{
	//***************************************************
	// CCadObject
	//	This is the default constructor
	// parameters:
	//	none
	// return value:
	//	none
	//--------------------------------------------------
	m_Type = 0;
	m_Dirty = 0;
	m_Selected = 0;
	m_LastMode = -1;
	m_pNext = 0;
	m_pPrev = 0;
	m_pNextSel = 0;
	m_pNextInstance = 0;
	m_P1 = CPoint(0, 0);
	m_P2 = CPoint(0, 0);
	m_NeedsAttributes = 1;
	m_hTreeItem = 0;
	m_ppHead = new CCadObject *[1];
	m_ppTail = new CCadObject *[1];
	m_ppHead[0] = 0;
	m_ppTail[0] = 0;
}

CCadObject::CCadObject(int Type,int nUnits)
{
	//***************************************************
	//	CCadObject
	//	This is the constructor that will generally be
	// used by the derived classes.
	//
	// parameters:
	//	Type.....Indicates the type of object.
	//	nUnits...Number of units in child link list
	// return value:
	//	none
	//--------------------------------------------------
	m_Type = Type;
	m_Dirty = 0;
	m_Selected = 0;
	m_LastMode = -1;
	m_pNext = 0;
	m_pPrev = 0;
	m_pNextSel = 0;
	m_pNextInstance = 0;
	m_P1 = CPoint(0, 0);
	m_P2 = CPoint(0, 0);
	m_NeedsAttributes = 1;
	m_hTreeItem = 0;
	m_ppHead = new CCadObject *[nUnits];
	m_ppTail = new CCadObject *[nUnits];
	for (int i = 0; i < nUnits; ++i)
	{
		m_ppHead[i] = 0;
		m_ppTail[i] = 0;
	}
	
}


CCadObject::~CCadObject()
{
	//***************************************************
	// ~CCadObject
	//		This is the base object destructor
	//
	// parameters:
	//	none
	// return value:
	//	none
	//--------------------------------------------------
	delete[] m_ppHead;
	delete[] m_ppTail;
}

void CCadObject::Move(CPoint p)
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

void CCadObject::Save(FILE * pO, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This function save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//	Indent..Number of spaces to indent from left margin
	//	flags...Various flags to control output
	//
	// return value:none
	//--------------------------------------------------
}

void CCadObject::SetVertex(int v, CPoint p)
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


int CCadObject::GrabVertex(CPoint p, CScale scale)
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


void CCadObject::Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale)
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

int CCadObject::CheckSelected(CPoint p, CSize Offset, CCadObject ** ppSelList , int index, int n, int flag)
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
	int rV = 0;
	CRect rect = CRect(m_P1, m_P2);
	rect.NormalizeRect();
	rV = rect.PtInRect(CPoint(p-Offset));
	return rV;
}

CPoint CCadObject::GetReference()
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

void CCadObject::AddObject(CCadObject *pObj,int Unit)
{
	//***************************************************
	// AddObject
	//	Adds object to the tail of the list
	//	Adds a child object to an object.  This function
	// is not supported by all objects, because they just
	// can have children.  A good example of an object
	// that has children would be a library object
	//
	// parameters:
	//	pO.....pointer to object to add.
	//	Unit...Which Unit the child is in
	//
	// return value:none
	//--------------------------------------------------
	if (m_ppHead[Unit] == 0)	//nothing in drawing
	{
		m_ppHead[Unit] = pObj;
		m_ppTail[Unit] = pObj;
	}
	else				//add part to end of drawing
	{
		m_ppTail[Unit]->SetNext(pObj);
		pObj->SetPrev(m_ppTail[Unit]);
		m_ppTail[Unit] = pObj;
	}
}

void CCadObject::InsertObject(CCadObject *pObj, int Unit)
{
	//***************************************************
	// InsertObject
	//	Add the object to the Head of the list
	//	Adds a child object to an object.  This function
	// is not supported by all objects, because they just
	// can have children.  A good example of an object
	// that has children would be a library object
	//
	// parameters:
	//	pO.....pointer to object to add.
	//	Unit...Which Unit the child is in
	//
	// return value:none
	//--------------------------------------------------
	if (m_ppHead[Unit] == 0)	//nothing in drawing
	{
		m_ppHead[Unit] = pObj;
		m_ppTail[Unit] = pObj;
	}
	else				//add object to front
	{
		m_ppHead[Unit]->SetPrev(pObj);
		pObj->SetNext(m_ppHead[Unit]);
		m_ppHead[Unit] = pObj;
	}
}

void CCadObject::RemoveObject(CCadObject *pObj,int Unit)
{
	//***************************************************
	// RemoveObject
	//	Removes a child object from an object
	//
	// parameters:
	//	pO.....pointer to object to remove
	//	Unit...which unit to remove object
	// return value:none
	//--------------------------------------------------
	int rV = 0;
	if (pObj == m_ppHead[Unit])
	{
		m_ppHead[Unit] = (CCadObject *)m_ppHead[Unit]->GetNext();
		if (m_ppHead[Unit]) m_ppHead[Unit]->SetPrev(0);
		rV = 1;
	}
	else if (pObj == m_ppTail[Unit])
	{
		m_ppTail[Unit] = (CCadObject*)m_ppTail[Unit]->GetPrev();
		if (m_ppTail[Unit]) m_ppTail[Unit]->SetNext(0);
	}
	else
	{
		pObj->GetPrev()->SetNext(pObj->GetNext());
		pObj->GetNext()->SetPrev(pObj->GetPrev());
	}
	pObj->SetPrev(0);
	pObj->SetNext(0);
}

CCadObject *CCadObject::GetHead(int Unit)
{
	//***************************************************
	// GetHead
	//	Retrieves the head pointer to a list of children
	// in the object
	// parameters:
	//	Unit.....Unit to get the head pointer from
	//
	// return value: Head pointer to children
	//--------------------------------------------------
	return m_ppHead[Unit];
}

int CCadObject::IsDirty(void)
{
	//***************************************************
	// IsDirty
	//	returns the status of the Dirty State of the
	// object
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return m_Dirty;
}

void CCadObject::SetDirty(int d)
{
	//***************************************************
	// SetDirty
	// Changes the state of the dirty flag
	// parameters:
	//	d.......new state of dirty flag.
	//
	// return value:
	//--------------------------------------------------
	m_Dirty = d;
}

int CCadObject::IsSelected()
{
	//***************************************************
	// IsSelected
	//	returns the state of the selected flag
	// parameters:
	//
	// return value:State of the selected flag
	//--------------------------------------------------
	return m_Selected;
}

void CCadObject::SetSelected(int Flag)
{
	//***************************************************
	// SetSelected
	//	Changes the state of the selcted flag.
	// parameters:
	// Flag......New state of the selected flag
	//
	// return value:
	//--------------------------------------------------
	m_Selected = Flag;
}

void CCadObject::AdjustRefernce(CPoint Ref)
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

CRect CCadObject::GetRect(void)
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
	Rect.SetRect(m_P1, m_P2);
	Rect.NormalizeRect();
	return Rect;
}

char * CCadObject::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	return (char *)"Base Class";
}

CCadObject CCadObject::operator=(CCadObject &v)
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
	return CCadObject();
}

CPoint CCadObject::ScalePoint(CPoint p, int Scale, int ScaleDiv)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	int r,x,y;
	p.x *= Scale;
	p.y *= Scale;
	x = p.x / ScaleDiv;
	r = p.x % ScaleDiv;
	if ((2 * r) > ScaleDiv) ++x;
	y = p.y / ScaleDiv;
	r = p.y % ScaleDiv;
	if ((2 * r) > ScaleDiv) y++;
	return CPoint(x,y);
}

CCadObject * CCadObject::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	return new CCadObject;
}

void CCadObject::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	if (((P1.x > P2.x) && (P1.y > P2.y)))
	{
		CPoint temp;
		temp = P2;
		P2 = P1;
		P1 = temp;
	}
	else if (((P1.x > P2.x) && (P1.y < P2.y)))
	{
		CPoint t1, t2;
		t1 = P1;
		t2 = P2;
		P1.x = t2.x;
		P2.x = t1.x;
	}
	else if ((P1.x < P2.x) && (P1.y > P2.y))
	{
		CPoint t1, t2;
		t1 = P1;
		t2 = P2;
		P1.y = t2.y;
		P2.y = t1.y;
	}
	rect.SetRect(P1 + Lw, P2 - Lw + CSize(1, 1));

}

void CCadObject::RenderEnable(int e)
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

CCadObject *CCadObject::PopHead()
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return 0;
}

CCadObject *CCadObject::PopTail()
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return 0;
}

CPoint CCadObject::GetCenter()
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

void CCadObject::ChangeCenter(CSize p)
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

CSize CCadObject::GetSize()
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

void CCadObject::ChangeSize(CSize Sz)
{
	//***************************************************
	// ChangeSize
	//	Change the size of the object
	// parameters:
	//	sz.....size to change object by (not change to)
	// return value:
	//--------------------------------------------------
}

int CCadObject::Parse(int Token,CLexer *pLex)
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

CCadObject * CCadObject::NewInstance()
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

int CCadObject::MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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

int CCadObject::MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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

int CCadObject::MouseMove(CAutoScrollView * pASV, int DrawState)
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

CPoint CCadObject::RotatePoint(CPoint Origin ,CPoint p, int Rotation)
{
	//-------------------------------------------------------
	// RotatePoint
	//	This function rotates a point about the reference
	// point Origin.  It will rotate by 0, 90, 180, and 270
	// degrees
	// Does not work on scaled points.
	//
	// parameters:
	//  Origin....point to rotate around
	//	p.........point to rotate
	//	Rotation..Amount to rotate by in the CCW direction
	//
	// returns:Rotated point
	//-------------------------------------------------------
	CPoint point;

	switch (Rotation)
	{
		case ROTATION_0:	//0 degrees
			point = p;
			break;
		case ROTATION_90:	//90 degrees
			point = CPoint((Origin.y-p.y)+Origin.x, (p.x-Origin.x)+Origin.y);
			break;
		case ROTATION_180:	//180 degrees
			point = CPoint((Origin.x - p.x) + Origin.x,(Origin.y-p.y) + Origin.y);
			break;
		case ROTATION_270:	//270 degrees
			point = CPoint((p.y-Origin.y)+Origin.x, (Origin.x-p.x)+Origin.y);
			break;
	}
	return point;
 }

void CCadObject::Rotate(CPoint Origin, int Rotation)
{
	m_P1 = RotatePoint(Origin, m_P1, Rotation);
	m_P2 = RotatePoint(Origin, m_P2, Rotation);
}

void CCadObject::Flip(CPoint Origin, int Direction)
{
	//-------------------------------------------------
	// Flip
	//		Flips the object along an axis
	//
	// parameters:
	//	Origin.........specifies either x or y axis
	//	Direction......0=X Axis, 1 = Y Axis
	//-------------------------------------------------
	m_P1 = FlipPoint(Origin, m_P1, Direction);
	m_P2 = FlipPoint(Origin, m_P2, Direction);
}

CPoint CCadObject::FlipPoint(CPoint Origin, CPoint p, int Direction)
{
	//-------------------------------------------------
	// FlipPoint
	//		Flip a point about an axis
	//
	// parameters:
	//	Origin........specifies either x or y axis
	//	p.............point to flip
	//	Direction......0=X Axis, 1 = Y Axis
	//
	// return value:
	//	returns new point flipped around axis
	//-------------------------------------------------
	CPoint rP;
	if (Direction)	//flip around Y axis
		rP = CPoint((Origin.x -p.x) + Origin.x,p.y);
	else	//flip around X axis
		rP = CPoint(p.x, (Origin.y - p.y) + Origin.y);
	return rP;
}

int CCadObject::EditProperties(void)
{
	return 0;
}

CRect CCadObject::ScaleRect(CRect rect, CScale scale, CSize Offset)
{
	CPoint P1 = scale * rect.TopLeft() + Offset;
	CPoint P2 = scale * rect.BottomRight() + Offset;
	rect.SetRect(P1, P2);
	rect.NormalizeRect();
	return rect;
}

CCadObject *CCadObject::FindTreeItem(HTREEITEM Item)
{
	//------------------------------------------------
	//	FindTreeItem
	//		This function returns back the object
	//	pointer that has the appropriate HTREEITEM
	//	tag
	//
	// parameter:
	//	Item......HTREEITEM handle to search for
	//
	// Returns:
	//		Pointer to object on success
	//		NULL on fail
	//-----------------------------------------------
	return 0;
}

void CCadObject::CopyProperties(CProp &Src)
{
	CPropField *pPropField;
	CCadObject *pCopy;

	pPropField = Src.GetHead();
	while (pPropField)
	{
		pCopy = pPropField->CopyObject();
		m_Properties.AddProperty((CPropField *)pCopy);
		pPropField = (CPropField *)pPropField->GetNext();
	}
}

void CCadObject::CopyPropertyValues(CProp &Src)
{
	CPropField *pPropField,*pPFCopy;
	char *name;

	pPropField = Src.GetHead();
	while (pPropField)
	{
		name = pPropField->GetFieldName();
		pPFCopy = GetProperties().FindProperty(name);
		switch (pPropField->GetFieldType())
		{
			case PROP_STRING:
				pPFCopy->SetValue(pPropField->GetFieldValue());
				break;
			case PROP_INT:
				((CPropInt *)pPFCopy)->SetValue(((CPropInt *)pPropField)->GetValue());
				break;
			case PROP_SELECTOR_INT:
				((CPropSelInt *)pPFCopy)->SetSelection(((CPropSelInt *)pPropField)->GetSelection());
				break;
			case PROP_SELECTOR_STRING:
				break;
		}
		pPropField = (CPropField *)pPropField->GetNext();
	}
}
