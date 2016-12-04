// PropertyEditor.cpp : implementation file
//

#include "stdafx.h"
#include "JamCad.h"
#include "PropertyEditor.h"


// CPropertyEditor

IMPLEMENT_DYNAMIC(CPropertyEditor, CStatic)

CPropertyEditor::CPropertyEditor()
{

}

CPropertyEditor::~CPropertyEditor()
{
}


BEGIN_MESSAGE_MAP(CPropertyEditor, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CPropertyEditor message handlers




void CPropertyEditor::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CStatic::OnPaint() for painting messages
	CRect rect;
	GetClientRect(&rect);
	CBrush br;
	br.CreateSolidBrush(RGB(70, 125, 220));
	dc.FillRect(&rect, &br);
}


void CPropertyEditor::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CStatic::OnLButtonDown(nFlags, point);
}


void CPropertyEditor::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	MessageBox(LPCTSTR("ButtonUp"), LPCTSTR("DEBUG"));
	CStatic::OnLButtonUp(nFlags, point);
}
