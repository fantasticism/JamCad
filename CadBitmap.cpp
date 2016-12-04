#include "stdafx.h"
#include "CadBitmap.h"
#include "JamCad.h"
#include "JamCadDefines.h"
#include "DlgBitmapProperties.h"

int CCadBitmap::m_RenderEnable = 1;

void CCadBitmap::LoadSettings(BitMapAttribute *pAttrib)
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
	pAttrib->m_MaintainAspectRatio = 1;
	pAttrib->m_pBMFileName = new char[512];
	pAttrib->m_pBMFileName[0] = 0;
	pAttrib->m_Size = CSize();
}

void CCadBitmap::SaveSettings(BitMapAttribute *pAttrib)
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


CCadBitmap::CCadBitmap():CCadObject(OBJECT_TYPE_BITMAP)
{
	m_pBM = 0;
	m_Attrb.m_pBMFileName = new char[512];
	m_Attrb.m_pBMFileName[0] = 0;
	m_Attrb.m_MaintainAspectRatio = 1;
}

CCadBitmap::CCadBitmap(CCadBitmap &v) :CCadObject(OBJECT_TYPE_BITMAP)
{
	m_pBM = 0;
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_Attrb.m_pBMFileName = new char[512];
	m_Attrb.m_pBMFileName[0] = 0;
	strcpy_s(m_Attrb.m_pBMFileName, 512, v.m_Attrb.m_pBMFileName);
	LoadImage(m_Attrb.m_pBMFileName);
	m_Attrb.m_MaintainAspectRatio = v.m_Attrb.m_MaintainAspectRatio;
}

CCadBitmap::~CCadBitmap()
{
	if (m_pBM) delete m_pBM;
	delete[] m_Attrb.m_pBMFileName;
}

void CCadBitmap::Move(CPoint p)
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

void CCadBitmap::Save(FILE * pO, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This function save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	fprintf(pO, "%s%s(%s(%d,%d),%s(%d,%d),%s(\"%s\"))\n",
		MkIndentString(Indent,' '),
		TokeToString(TOKEN_BITMAP),
		TokeToString(TOKEN_POINT),
		m_P1.x, m_P1.y,
		TokeToString(TOKEN_POINT),
		m_P2.x, m_P2.y,
		TokeToString(TOKEN_FILE),
		m_Attrb.m_pBMFileName);
}

void CCadBitmap::SetVertex(int Vi, CPoint p)
{
	//***************************************************
	// SetVertex
	//	This function is used to change the position of
	// a vertex.
	//
	// parameters:
	// Vi.....index of the vertex
	// p......Amnount to change the vertex by
	//
	// return value: none
	//--------------------------------------------------
	if (Vi)
		m_P2 += p - m_P2;
	else
		m_P1 += p - m_P1;
	if (m_Attrb.m_MaintainAspectRatio)
		RestoreAspectRatio();
}


int CCadBitmap::GrabVertex(CPoint point)
{
	//***************************************************
	// GrabVertex
	//	This function checks for a vertex at point p
	//
	// parameters:
	//	point.....point to check for presence of a vertex
	//
	// return value:
	//	returns index of vertex if succesful
	//	returns -1 on fail
	//--------------------------------------------------
	int rV = -1;
	CSize Diff(6, 6);
	CRect rect;
	rect.SetRect(m_P1 + Diff, m_P1 - Diff);
	rect.NormalizeRect();
	if (rect.PtInRect(point))
		rV = 0;
	else
	{
		rect.SetRect(m_P2 + Diff, m_P2 - Diff);
		rect.NormalizeRect();
		if (rect.PtInRect(point))
			rV = 1;
	}
	return rV;
}


void CCadBitmap::Draw(CDC * pDC, int mode, CPoint Offset, CScale Scale)
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
	CPen pen, *oldpen;
	CRect rect;
	CPoint P1, P2;

	if (CCadBitmap::m_RenderEnable)
	{
		if (mode == OBJECT_MODE_SKETCH || GetLastMode() == OBJECT_MODE_SKETCH)
		{
			double AspectRatioBM;
			AspectRatioBM = double(m_Attrb.m_Size.cy) / double(m_Attrb.m_Size.cx);
			m_P2.y = int(AspectRatioBM * double(abs(m_P2.x - m_P1.x))) + m_P1.y;
		}
		P1 = Scale * m_P1 + Offset;
		P2 = Scale * m_P2 + Offset;
		rect.SetRect(P1, P2);

		switch (mode)
		{
		case OBJECT_MODE_FINAL:
		{
			CDC bmDC;
			CBitmap *pOld;
			bmDC.CreateCompatibleDC(pDC);
			pOld = bmDC.SelectObject(this->m_pBM);
			pDC->StretchBlt(P1.x, P1.y, rect.Width(), rect.Height(), &bmDC, 0, 0, m_Attrb.m_Size.cx, m_Attrb.m_Size.cy, SRCCOPY);
			bmDC.SelectObject(pOld);
		}
		break;
		case OBJECT_MODE_SELECTED:
		{
			CPen bPen;
			CRect Vrect;
			bPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			pen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
			oldpen = pDC->SelectObject(&pen);
			CDC bmDC;
			CBitmap *pOld;
			bmDC.CreateCompatibleDC(pDC);
			pOld = bmDC.SelectObject(this->m_pBM);
			pDC->StretchBlt(P1.x, P1.y, rect.Width(), rect.Height(), &bmDC, 0, 0, m_Attrb.m_Size.cx, m_Attrb.m_Size.cy, SRCCOPY);
			pDC->MoveTo(P1);
			pDC->LineTo(P1.x, P2.y);
			pDC->LineTo(P2);
			pDC->LineTo(P2.x, P1.y);
			pDC->LineTo(P1);
			pDC->SelectObject(&bPen);
			Vrect.SetRect(P1 + CSize(6, 6), P1 + CSize(-6, -6));
			pDC->Rectangle(Vrect);
			Vrect.SetRect(P2 + CSize(6, 6), P2 + CSize(-6, -6));
			pDC->Rectangle(Vrect);
			bmDC.SelectObject(pOld);
			pDC->SelectObject(oldpen);
		}
		break;
		case OBJECT_MODE_SKETCH:
			pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			oldpen = pDC->SelectObject(&pen);
			pDC->DrawDragRect(&rect, CSize(1, 1), NULL, CSize(1, 1));
			pDC->SelectObject(oldpen);
			break;
		}
		SetLastMode(mode);
	}
}

int CCadBitmap::PtInBitmap(CPoint p, CSize Offset)
{
	CRect rect;
	int rV;
	CPoint P1 = m_P1 + Offset;
	CPoint P2 = m_P2 + Offset;
	rect.SetRect(P1, P2);
	rect.NormalizeRect();
	rV = (int)rect.PtInRect(p);
	return rV;
}


int CCadBitmap::CheckSelected(CPoint p, CSize Offset, CCadObject ** ppSelList , int index, int n, int flag)
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
		if (PtInBitmap(p - Offset))
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

CPoint CCadBitmap::GetReference()
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

void CCadBitmap::AdjustRefernce(CPoint Ref)
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

CRect CCadBitmap::GetRect(void)
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
	CRect rR;
	rR.SetRect(m_P1, m_P2);
	rR.NormalizeRect();
	return rR;
}

char * CCadBitmap::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	return (char *)"Bitmap";
}

CCadBitmap CCadBitmap::operator=(CCadBitmap &v)
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
	m_pBM = 0;
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_Attrb.m_pBMFileName = new char[512];
	strcpy_s(m_Attrb.m_pBMFileName, 512, v.m_Attrb.m_pBMFileName);
	LoadImage(m_Attrb.m_pBMFileName);
	m_Attrb.m_MaintainAspectRatio = v.m_Attrb.m_MaintainAspectRatio;
	return *this;
}

CCadObject * CCadBitmap::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadBitmap *pBM = new CCadBitmap;
	*pBM = *this;
	pBM->CopyAttributes(&m_Attrb);
	return pBM ;
}

void CCadBitmap::RenderEnable(int e)
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
	CCadBitmap::m_RenderEnable = e;
}

CPoint CCadBitmap::GetCenter()
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

void CCadBitmap::ChangeCenter(CSize p)
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

CSize CCadBitmap::GetSize()
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

void CCadBitmap::ChangeSize(CSize Sz)
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

int CCadBitmap::Parse(int Token, CLexer *pLex)
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

CCadObject * CCadBitmap::NewInstance()
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

void CCadBitmap::GetAttributes(BitMapAttribute *pAttrb)
{
	/***************************************************
	*	GetAttributes
	*		This function is used to copy the
	*	attributes from this object into one pointed to 
	*	by the parameter 
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy
	***************************************************/
	pAttrb->m_MaintainAspectRatio = m_Attrb.m_MaintainAspectRatio;
	pAttrb->m_Size = m_Attrb.m_Size;
	if (strlen(m_Attrb.m_pBMFileName))
	{
		if (pAttrb->m_pBMFileName == 0) pAttrb->m_pBMFileName = new char[512];
		strcpy(pAttrb->m_pBMFileName, m_Attrb.m_pBMFileName);
	}
}

void CCadBitmap::CopyAttributes(BitMapAttribute *pAttrb)
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
	m_Attrb.m_MaintainAspectRatio = pAttrb->m_MaintainAspectRatio;
	m_Attrb.m_Size = pAttrb->m_Size;
	if (pAttrb->m_pBMFileName)
	{
		if (strlen(pAttrb->m_pBMFileName))
		{
			if (m_Attrb.m_pBMFileName == 0)
			{
				m_Attrb.m_pBMFileName = new char[512];
			}
			strcpy(m_Attrb.m_pBMFileName, pAttrb->m_pBMFileName);
		}
	}
	ClearNeedsAttributes();
}

int CCadBitmap::MouseDown(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
		CopyAttributes((BitMapAttribute *)pAdrbData);
		m_P1 = pASV->m_SnapPos;
		break;
	case DRAWSTATE_PLACE:
		m_P2 = pASV->m_SnapPos;
		pASV->Invalidate();
		break;
	}
	return DrawState;
}

int CCadBitmap::MouseUp(CAutoScrollView * pASV, int DrawState, void * pAdrbData)
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
	CDlgBitmapProperties Dlg;

	switch (DrawState)
	{
		case DRAWSTATE_SET_ATTRIBUTES:
			theApp.UpdateStatusBar("BITMAP:Choose Bitmap File");
			m_P1 = pASV->m_SnapPos;
			GetAttributes(&Dlg.m_Attrb);
			if (IDOK == Dlg.DoModal())
			{
				CopyAttributes(&Dlg.m_Attrb);
				LoadImage(m_Attrb.m_pBMFileName);
				DrawState = DRAWSTATE_PLACE;
				theApp.UpdateStatusBar("BITMAP:Place Upper Left Corner");
				pASV->Invalidate();
			}
			break;
		case DRAWSTATE_PLACE:
			m_P2 = pASV->m_SnapPos;
			pASV->AddObjectIntoDoc(this);
			pASV->m_CadObj.pCadObject = 0;
			DrawState = DRAWSTATE_SET_ATTRIBUTES;
			pASV->Invalidate();
			break;
	}
	return DrawState;
}

int CCadBitmap::MouseMove(CAutoScrollView * pASV, int DrawState)
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
	case DRAWSTATE_PLACE:
		m_P2 = pASV->m_SnapPos;
		break;
	}
	pASV->Invalidate();
	return DrawState;
}


void CCadBitmap::LoadImage(char *path)
{
	if (m_pBM) delete m_pBM;
	m_pBM = new CMyBitmap;
	m_pBM->LoadBitmap(path);
	strcpy_s(m_Attrb.m_pBMFileName, 512, path);
	BITMAP bm;
	m_pBM->GetBitmap(&bm);
	m_Attrb.m_Size = CSize(bm.bmWidth, bm.bmHeight);
}

void CCadBitmap::RestoreAspectRatio()
{
	CPoint P1, P2;
	double AspectRatioBM;
	AspectRatioBM = double(m_Attrb.m_Size.cy) / double(m_Attrb.m_Size.cx);
	m_P2.y = int(AspectRatioBM * double(abs(m_P2.x - m_P1.x))) + m_P1.y;
	m_Attrb.m_MaintainAspectRatio = 1;
}
