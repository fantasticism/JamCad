// StaticPreview.cpp : implementation file
//

#include "stdafx.h"
#include "StaticPreview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticPreview

CStaticPreview::CStaticPreview()
{
	m_pObj = 0;
	m_X = 0;
	m_Y = 1;
	m_Scale = CScale(0.3, 0.3);
	m_OffsetMode = STATICPREVIEW_OFFSET_CENTER;
}

CStaticPreview::~CStaticPreview()
{
}


BEGIN_MESSAGE_MAP(CStaticPreview, CStatic)
	//{{AFX_MSG_MAP(CStaticPreview)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticPreview message handlers

void CStaticPreview::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	

	// TODO: Add your message handler code here
	CRect rect;
	CBrush br(RGB(255,255,255));


	GetClientRect(&rect);
	dc.FillRect(&rect,&br);
	CCadObject *pO = m_pObj;
	CPen pen,*oldPen;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 128, 240));
	oldPen = dc.SelectObject(&pen);
	if (m_Y)	//Y axis enabled
	{
		dc.MoveTo(m_Offset.x, 0);
		dc.LineTo(m_Offset.x, rect.BottomRight().y);
	}
	if (m_X)	//X axis enabled
	{
		dc.MoveTo(0, m_Offset.y);
		dc.LineTo(rect.BottomRight().x, m_Offset.y);
	}
	dc.SelectObject(oldPen);
	while(pO)
	{
		pO->Draw(&dc, 0, m_Offset, m_Scale);
		pO = pO->GetNext();
	}
}

void CStaticPreview::Init()
{
	CRect rect;

	GetClientRect(&rect);
	m_Offset = rect.CenterPoint();
}


void CStaticPreview::SetViewOffset(CPoint Point)
{
	m_Offset = Point;
}


BOOL CStaticPreview::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL rV = CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
	if (rV) Init();
	return rV;
}


BOOL CStaticPreview::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL rV = CStatic::PreCreateWindow(cs);
	if (rV) Init();
	return rV;
}

void CStaticPreview :: AutoScale(CRect &rect)
{
	//-------------------------------------------------
	// AutoScale
	// This function uses the rectangle that surronds
	// the object that is desired to sho and calculates
	// and optimized offset and scale
	//
	// parameters:
	//	rect.......rectangle that surrounds object
	//---------------------------------------------------
	CRect RectClient;
	GetClientRect(&RectClient);
	double scale,rh,rw;
	double ORw, ORh;
	CPoint cp;
	CPoint Ref = m_pObj->GetReference();
	cp = RectClient.CenterPoint();


	rh = (double(RectClient.Height()) / double(rect.Height()));
	rw = (double(RectClient.Width()) / double(rect.Width()));
	if ( rw > rh )
	{
		scale = rh;
		scale *= 0.9;
	}
	else
	{
		scale = rw;
		scale *= 0.9;
	}
	m_Scale = CScale(scale, scale);
	ORw = double(Ref.x - rect.left) / double(rect.Width());
	ORh = double(Ref.y - rect.top) / double(rect.Height());
	int Offx = int(RectClient.Width() * ORw);
	m_Offset = CPoint(Offx,int(RectClient.Height() * ORh));
}
