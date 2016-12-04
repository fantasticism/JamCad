#include "stdafx.h"
#include "RulerView.h"
#include "JamCad.h"
#include "JamCadDefines.h"

/////////////////////////////////////////////////////////////////////////////
// CRulerView

IMPLEMENT_DYNCREATE(CRulerView, CView)

CRulerView::CRulerView()
{
	m_rulerType = RT_HORIZONTAL;
	m_scrollPos = 0;
	m_lastPos = CPoint(-1,-1);
	m_fZoomFactor = 1.0;
	m_pBitMap = 0;
	m_pMemDC = 0;
	m_pHFont = 0;
	m_pVFont = 0;
	m_ppTickLabels = 0;
	m_nTickLables = 0;
	m_pHFont = new CFont();
	m_pVFont = new CFont();
	m_pVFont->CreateFont(10, 0, 900, 900, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY, VARIABLE_PITCH | FF_ROMAN, LPCTSTR("Times New Roman"));
	m_pHFont->CreateFont(12, 0, 000, 000, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY, VARIABLE_PITCH | FF_ROMAN, LPCTSTR("Times New Roman"));
}

CRulerView::~CRulerView()
{
	if (m_pBitMap) delete m_pBitMap;
	if (m_pMemDC) delete m_pMemDC;
	if (m_pHFont) delete m_pHFont;
	if (m_pVFont) delete m_pVFont;
}


BEGIN_MESSAGE_MAP(CRulerView, CView)
	//{{AFX_MSG_MAP(CRulerView)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRulerView drawing
void CRulerView::UpdateRulersInfo(stRULER_INFO RulerInfo)
{
	CDC* pDC;
	int i;

	m_DocSize = RulerInfo.DocSize;
	m_fZoomFactor = (float)RulerInfo.fZoomFactor;
	m_scrollPos = RulerInfo.ScrollPos;
	switch (RulerInfo.uMessage)
	{
		case RW_SETLABELS:
			if (m_ppTickLabels)
			{
				for (i = 0; i < m_nTickLables; ++i)
					if (m_ppTickLabels[i])
						delete[] m_ppTickLabels[i];
				delete[] m_ppTickLabels;
			}
			m_ppTickLabels = new char *[RulerInfo.nLabels];
			m_nTickLables = RulerInfo.nLabels;
			for (i = 0; i < m_nTickLables; ++i)
			{
				m_ppTickLabels[i] = new char[32];
				strcpy(m_ppTickLabels[i], RulerInfo.ppLabels[i]);
			}
			break;
		case RW_POSITION:
			DrawCursorPos(RulerInfo.Pos - RulerInfo.ScrollPos);
			break;
		case RW_ZOOM:
		case RW_HSCROLL:
		case RW_VSCROLL:
			pDC = GetDC();
			OnDraw(pDC);
			ReleaseDC(pDC);
			break;
		default:
			Invalidate();
			break;
	}
}

void CRulerView::OnDraw(CDC* pDC)
{
	m_lastPos = CPoint(-1,-1);
	// set the map mode right
	int oldMapMode = pDC->SetMapMode(MM_TEXT);


	CFont* pOldFont = pDC->SelectObject((m_rulerType == RT_HORIZONTAL) ? m_pHFont : m_pVFont);
	int oldTextAlign = pDC->SetTextAlign((m_rulerType == RT_HORIZONTAL) ? (TA_RIGHT | TA_TOP) : (TA_LEFT | TA_TOP));
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);

	CRect rulerRect;
	GetClientRect(&rulerRect);

	if (m_rulerType == RT_HORIZONTAL)
	{
		rulerRect.right = (int)(m_DocSize.cx / m_fZoomFactor);
	}
	else //(m_rulerType==RT_VERTICAL)
	{
		rulerRect.bottom = (int)(m_DocSize.cy / m_fZoomFactor);
	}

	pDC->FillSolidRect(rulerRect, RGB(255, 255, 255));

	DrawTicker(pDC, rulerRect, 100);

	if (this->m_fZoomFactor >= 0.50)
	{

		DrawTicker(pDC, rulerRect, 10, 10, FALSE);

	}
	// restore DC settings
	pDC->SetMapMode(oldMapMode);
	pDC->SelectObject(pOldFont);
	pDC->SetTextAlign(oldTextAlign);
	pDC->SetBkMode(oldBkMode);
}

void CRulerView::DrawTicker(CDC* pDC, CRect rulerRect, int nFactor, int nBegin, BOOL bShowPos)
{
	//--------------------------------------------------------
	// DrawTicker
	//
	//	This function draws the ticks for the ruller.
	//
	// parameters:
	//	pDC...........pointer to the device context
	//	ruleRect......Rectangle that encloses the ruler
	//	nFactor......Number of pixels per tick
	//	nBegin.......Determines how long tick is 0=Full Size
	//	bShowPos.....Flag that determins if text lables
	//---------------------------------------------------------
	// First find the long dimension of ruler
	// For a horizontal ruler, this is the right side
	// For a Vertical ruler, this is the bottom side
	int nSize = (m_rulerType == RT_HORIZONTAL) ? rulerRect.right : rulerRect.bottom;
	//-----------------------------------------------------------
	// Now we need to find the total number of ticks.?????
	//-----------------------------------------------------------
	int nTick = (int)(nFactor*m_fZoomFactor);
	//Allocate space to print strings
	char *s = new char[1024];

	int count = nSize / nFactor;

	for (int i = 1; i <= count; i++)
	{
		if(bShowPos)sprintf_s(s, 1024, "%d", i*nFactor);

		if (m_rulerType == RT_HORIZONTAL)
		{
			pDC->PatBlt(nTick*i - m_scrollPos.x, rulerRect.top + nBegin, 1, rulerRect.bottom, BLACKNESS);
			if (bShowPos)
				pDC->TextOut(nTick*i - m_scrollPos.x, rulerRect.top, CString(s));
		}
		else //(m_rulerType==RT_VERTICAL)
		{
			pDC->PatBlt(rulerRect.left + nBegin, nTick*i - m_scrollPos.y, rulerRect.right, 1, BLACKNESS);
			if (bShowPos)
				pDC->TextOut(rulerRect.left, nTick*i - m_scrollPos.y, CString(s));
		}
	}
	delete[] s;
}

void CRulerView::DrawCursorPos(CPoint NewPos)
{
	if (((m_rulerType == RT_HORIZONTAL) && (NewPos.x > m_DocSize.cx*m_fZoomFactor)) ||
		((m_rulerType == RT_VERTICAL) && ((NewPos.y) > m_DocSize.cy*m_fZoomFactor)))
		return;

	CDC* pDC = GetDC();
	// set the map mode right
	int oldMapMode = pDC->SetMapMode(MM_TEXT);

	CRect clientRect;
	GetClientRect(&clientRect);
	if (m_rulerType == RT_HORIZONTAL)
	{
		// erase the previous position
		if(m_lastPos.x >= 0) pDC->PatBlt(m_lastPos.x, clientRect.top, 1, clientRect.bottom, DSTINVERT);
		// draw the new position
		m_lastPos.x = NewPos.x;
		pDC->PatBlt(m_lastPos.x, clientRect.top, 1, clientRect.bottom, DSTINVERT);
	}
	else // (m_rulerType==RT_VERTICAL)
	{
		// erase the previous position
		if(m_lastPos.y >= 0)pDC->PatBlt(clientRect.left, m_lastPos.y, clientRect.right, 1, DSTINVERT);
		// draw the new position
		m_lastPos.y = NewPos.y;
		pDC->PatBlt(clientRect.left, m_lastPos.y, clientRect.right, 1, DSTINVERT);
	}

	pDC->SetMapMode(oldMapMode);
	ReleaseDC(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CRulerView diagnostics

#ifdef _DEBUG
void CRulerView::AssertValid() const
{
	CView::AssertValid();
}

void CRulerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRulerView message handlers

BOOL CRulerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// create a brush using the same color as the background
	if (cs.lpszClass == NULL)
	{
		HBRUSH hBr = CreateSolidBrush(GetSysColor(COLOR_MENU));
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_BYTEALIGNWINDOW, NULL, hBr);
	}

	return CView::PreCreateWindow(cs);
}

void CRulerView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	((CSplitterWnd*)GetParent())->SetActivePane(1, 1);
}

void CRulerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	CreateMemDC();
}


void CRulerView::CreateMemDC()
{
	CDC *pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	if (m_pMemDC) delete m_pMemDC;
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(pDC);
	if (m_pBitMap) delete m_pBitMap;
	m_pBitMap = new CBitmap();
	m_pBitMap->CreateBitmap(rect.Width(), rect.Height(), 1, 32, nullptr);
	m_pMemDC->SelectObject(m_pBitMap);
	ReleaseDC(pDC);
}
