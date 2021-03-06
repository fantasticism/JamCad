#include "stdafx.h"
#include "RulerCornerView.h"

/////////////////////////////////////////////////////////////////////////////
// CRulerCornerView

IMPLEMENT_DYNCREATE(CRulerCornerView, CView)

CRulerCornerView::CRulerCornerView()
{
}

CRulerCornerView::~CRulerCornerView()
{
}


BEGIN_MESSAGE_MAP(CRulerCornerView, CView)
	//{{AFX_MSG_MAP(CRulerCornerView)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRulerCornerView drawing

void CRulerCornerView::OnDraw(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CBrush br;
	br.CreateSolidBrush(RGB(0, 255, 0));
	pDC->FillRect(&rect, &br);
}

/////////////////////////////////////////////////////////////////////////////
// CRulerCornerView diagnostics

#ifdef _DEBUG
void CRulerCornerView::AssertValid() const
{
	CView::AssertValid();
}

void CRulerCornerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRulerCornerView message handlers

BOOL CRulerCornerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// create a brush using the same color as the background
	if (cs.lpszClass == NULL)
	{
		HBRUSH hBr = CreateSolidBrush(GetSysColor(COLOR_MENU));
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_BYTEALIGNWINDOW, NULL, hBr);
	}

	return CView::PreCreateWindow(cs);
}

void CRulerCornerView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	((CSplitterWnd*)GetParent())->SetActivePane(1, 1);
}
