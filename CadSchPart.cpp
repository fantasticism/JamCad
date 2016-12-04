#include "stdafx.h"
#include "PropField.h"
#include "JamCad.h"
#include "CadSchPart.h"
#include "DlgSchPartProp.h"

//-------------------------------------------
// component part properties
//-------------------------------------------

static FieldNameType DefPropNames[] = {
	{ PropFieldNames[PF_NAME_UNITS],PROP_INT,TOKEN_NUMUNITS,PROPFIELD_HIDE,(void *)1,&theApp.m_SchTextAttr },
	{ PropFieldNames [PF_NAME_COMPNAME],PROP_STRING,TOKEN_PARTNAME,PROPFIELD_DISPLAY ,(void *)"PART",&theApp.m_SchTextAttr },
	{ PropFieldNames [PF_NAME_REFDES],PROP_STRING,TOKEN_REFDESIGNATOR,PROPFIELD_DISPLAY,(void *)"U",&theApp.m_SchTextAttr },
	{ PropFieldNames [PF_NAME_COMPREF],PROP_INT,TOKEN_REFNUMBER,PROPFIELD_HIDE, (void *)0,&theApp.m_SchTextAttr },
	{NULL,PROP_NONE ,0,0,(void *)0,0}
};

void CCadSchPart::LoadSettings(SchPartAttributes *pAttrib)
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

void CCadSchPart::SaveSettings(SchPartAttributes *pAttrib)
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


CCadSchPart::CCadSchPart():CCadObject(OBJECT_TYPE_SCH_PART)
{
	m_OpenFlag = 0;
	m_pView = 0;
}

void CCadSchPart::Create(void)
{
	CDlgSchPartProp Dlg;

	GetProperties().Create(DefPropNames);
	m_pPartName = (CPropString *)GetProperties().FindProperty(GetProperties().GetPropertyName(TOKEN_PARTNAME, DefPropNames));
	m_pReferenceDesignator = (CPropString *)GetProperties().FindProperty(GetProperties().GetPropertyName(TOKEN_REFDESIGNATOR, DefPropNames));
	m_pTotalUnits = (CPropInt *)GetProperties().FindProperty(GetProperties().GetPropertyName(TOKEN_NUMUNITS, DefPropNames));
	Dlg.m_pPartDwg = this;
	if (IDOK == Dlg.DoModal())
	{

	}
}

CCadSchPart::~CCadSchPart()
{
}

void CCadSchPart::Move(CPoint p)
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

void CCadSchPart::Save(FILE * pO, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This function save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	fprintf(pO, "%s%s[\n", MkIndentString(Indent, ' '),TokeToString(TOKEN_SCHPART));
	GetProperties().Save(pO,Indent+1,flags);
	CCadObject *pObj = GetHead();
	while (pObj)
	{
		pObj->Save(pO,Indent +1,flags);
		pObj = pObj->GetNext();
	}
	fprintf(pO, "%s]\n", MkIndentString(Indent, ' '));
}

void CCadSchPart::SetVertex(int v, CPoint p)
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


int CCadSchPart::GrabVertex(CPoint p)
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


void CCadSchPart::Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale)
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
	CCadObject *pO = GetHead();

	while(pO)
	{
		if (pO->IsSelected())
			mode = OBJECT_MODE_SELECTED;
		else
			mode = OBJECT_MODE_FINAL;
		pO->Draw(pDC, mode, Offset, Scale);
		pO = pO->GetNext();
	}
	pO = GetProperties().GetHead();
	while (pO)
	{
		if (pO->IsSelected())
			mode = OBJECT_MODE_SELECTED;
		else
			mode = OBJECT_MODE_FINAL;
		pO->Draw(pDC, mode, Offset, Scale);
		pO = pO->GetNext();
	}
}

int CCadSchPart::CheckSelected(CPoint p, CSize Offset, CCadObject ** ppSelList, int index, int n, int flag)
{
	//***************************************************
	// CheckSelected
	//	This function is used to see if an object can
	// be selected at point p.
	//
	// Fist thing we want to do here is check to see if the
	// point is anywhere withing the area covered by the part.
	// If it is, we check to see if any of the specific parts
	// are being selected, such as property fields, pins, etc.
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
	CCadObject *pCO = GetHead();
	//----------------------------------
	// Start out by seeing if pointer is
	// within the bounds of all the things
	// that make it up.
	//-----------------------------------
	if (GetRect().PtInRect(p))
	{
		if (ppSelList)
		{
			ppSelList[index++] = this;
			while (pCO && (index < n))
			{
				index = pCO->CheckSelected(p, Offset, ppSelList, index, n, flag);
				pCO = pCO->GetNext();
			}
			pCO = GetProperties().GetHead();
			while (pCO && (index < n))
			{
				index = pCO->CheckSelected(p, Offset, ppSelList, index, n, flag);
				pCO = pCO->GetNext();
			}
		}
		else
			index = 1;
	}
	return index;
}

CPoint CCadSchPart::GetReference()
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

int CCadSchPart::IsDirty(void)
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

void CCadSchPart::SetDirty(int d)
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

int CCadSchPart::IsSelected()
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

void CCadSchPart::SetSelected(int Flag)
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

void CCadSchPart::AdjustRefernce(CPoint Ref)
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

CRect CCadSchPart::GetRect(void)
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
	CRect rect,rect2;
	CCadObject *pOb = GetHead();
	while (pOb)
	{
		rect2 = pOb->GetRect();
		rect.UnionRect(&rect,&rect2);
		pOb = pOb->GetNext();
	}
	rect2 = GetProperties().GetRect();
	rect.UnionRect(&rect, &rect2);
	return rect;
}

char * CCadSchPart::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	return (char *)"Part";
}

CCadSchPart CCadSchPart::operator=(CCadSchPart &v)
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
	return CCadSchPart();
}

CPoint CCadSchPart::ScalePoint(CPoint p, int Scale, int ScaleDiv)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return CPoint();
}

CCadObject * CCadSchPart::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadSchPart *pCS = new CCadSchPart;
	*pCS = *this;
	return pCS;
}

void CCadSchPart::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------

}

void CCadSchPart::RenderEnable(int e)
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

CCadObject *CCadSchPart::PopHead()
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return 0;
}

CCadObject *CCadSchPart::PopTail()
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return 0;
}

CPoint CCadSchPart::GetCenter()
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

void CCadSchPart::ChangeCenter(CSize p)
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

CSize CCadSchPart::GetSize()
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

void CCadSchPart::ChangeSize(CSize Sz)
{
	//***************************************************
	// ChangeSize
	//	Change the size of the object
	// parameters:
	//	sz.....size to change object by (not change to)
	// return value:
	//--------------------------------------------------
}

int CCadSchPart::Parse(int Token, CLexer *pLex)
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

CCadObject * CCadSchPart::NewInstance()
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

void CCadSchPart::CopyAttributes(SchPartAttributes *pAttrb)
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

int CCadSchPart::MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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

int CCadSchPart::MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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

int CCadSchPart::MouseMove(CAutoScrollView * pASV, int DrawState)
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