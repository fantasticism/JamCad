#include "stdafx.h"
#include "JamCad.h"
#include "JamCadDefines.h"
#include "CadSchLib.h"
#include "PropField.h"
#include "DlgSchLibProp.h"

static FieldNameType DefPropNames[] = {
	{ PropFieldNames[PF_NAME_UNITS],PROP_INT,TOKEN_NUMUNITS,PROPFIELD_HIDE,(void *)1 ,&theApp.m_SchTextAttr },
	{ PropFieldNames[PF_NAME_SCHLIBNAME], PROP_STRING, TOKEN_SCHLIBNAME,PROPFIELD_DISPLAY ,(void *)"Library",&theApp.m_SchTextAttr },
	{ NULL,PROP_NONE ,0,0,(void *)0 ,0}
};

void MakeSchLibProperties(CProp *pP)
{
	pP->Create(DefPropNames);
}

CCadSchLib::CCadSchLib():CCadObject(OBJECT_TYPE_SCHLIB)
{
}

void CCadSchLib::Create()
{
	m_pNameProp = (CPropString *)GetProperties().FindProperty(PropFieldNames[PF_NAME_SCHLIBNAME]);
}

CCadSchLib::~CCadSchLib()
{
}

void CCadSchLib::LoadSettings(CadSchLibAttr *pAttrib)
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

void CCadSchLib::SaveSettings(CadSchLibAttr *pAttrib)
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

void CCadSchLib::Move(CPoint p)
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

void CCadSchLib::Save(FILE * pO, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This function save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	fprintf(pO, "%s%s[\n",MkIndentString(Indent,' '),TokeToString(TOKEN_SCHLIBRARY));
	GetProperties().Save(pO,Indent+1,flags);
	CCadObject *pObj = GetHead();
	while (pObj)
	{
		pObj->Save(pO,Indent+1,flags);
		pObj = pObj->GetNext();
	}
	fprintf(pO, "%s]\n", MkIndentString(Indent, ' '));
}

int CCadSchLib::Parse(int Token, CLexer *pLex)
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
	Token = pLex->Accept(Token, TOKEN_SCHLIBRARY);
	Token = pLex->Accept(Token, '[');

	return 0;
}

void CCadSchLib::SetVertex(int v, CPoint p)
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


int CCadSchLib::GrabVertex(CPoint p)
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


void CCadSchLib::Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale)
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

int CCadSchLib::CheckSelected(CPoint p, CSize Offset, CCadObject ** ppSelList , int index, int n, int flag)
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

CPoint CCadSchLib::GetReference()
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


void CCadSchLib::SetDirty(int d)
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
	CCadObject *pO = GetHead();
	while (pO)
	{
		pO->SetDirty(d);
		pO = pO->GetNext();
	}
}

int CCadSchLib::IsSelected()
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

void CCadSchLib::SetSelected(int Flag)
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
	CCadObject *pO = GetHead();
	while (pO)
	{
		pO->SetSelected(Flag);
		pO = pO->GetNext();
	}
}

void CCadSchLib::AdjustRefernce(CPoint Ref)
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

CRect CCadSchLib::GetRect(void)
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

char * CCadSchLib::GetTypeString(void)
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

CCadSchLib CCadSchLib::operator=(CCadSchLib &v)
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
	return CCadSchLib();
}

CPoint CCadSchLib::ScalePoint(CPoint p, int Scale, int ScaleDiv)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return CPoint();
}

CCadObject * CCadSchLib::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	return new CCadSchLib;
}

void CCadSchLib::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------

}

void CCadSchLib::RenderEnable(int e)
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

CPoint CCadSchLib::GetCenter()
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

void CCadSchLib::ChangeCenter(CSize p)
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

CSize CCadSchLib::GetSize()
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

void CCadSchLib::ChangeSize(CSize Sz)
{
	//***************************************************
	// ChangeSize
	//	Change the size of the object
	// parameters:
	//	sz.....size to change object by (not change to)
	// return value:
	//--------------------------------------------------
}

CCadObject * CCadSchLib::NewInstance()
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

void CCadSchLib::GetAttributes(CadSchLibAttr *pAttrb)
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
}

void CCadSchLib::CopyAttributes(CadSchLibAttr *pAttrb)
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

int CCadSchLib::MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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

int CCadSchLib::MouseUp(CAutoScrollView * pASV, int DrawState)
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

int CCadSchLib::MouseMove(CAutoScrollView * pASV, int DrawState)
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

CCadObject *CCadSchLib::FindTreeItem(HTREEITEM Item)
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
	CCadObject *pO = this;
	CCadObject *pOChild;
	if (pO->GetTreeItem() != Item)
	{
		int loop = 1;
		pO = GetHead();
		while(loop && pO)
		{
			if (pO->GetTreeItem() == Item)
				loop = 0;
			else
			{
				if (pO->GetHead())	//does it have children?
				{
					if ((pOChild = pO->FindTreeItem(Item)) != NULL)
					{
						loop = 0;
						pO = pOChild;
					}
					else
					{
						pO = pO->GetNext();
					}
				}
				else
				{
					pO = pO->GetNext();
				}
			}
		}

	}
	return pO;
}
