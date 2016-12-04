#if !defined(AFX_STATICBITMAP_H__443F2077_C1AA_40B1_BCF7_C261D5FB2CCE__INCLUDED_)
#define AFX_STATICBITMAP_H__443F2077_C1AA_40B1_BCF7_C261D5FB2CCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticBitmap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticBitmap window

class CStaticBitmap : public CStatic
{
// Construction
public:
	CStaticBitmap();

// Attributes
public:

// Operations
public:
	void SetColor(COLORREF c){m_Color = c;Invalidate();}
	COLORREF GetColor(void){return m_Color;}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticBitmap)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetupMessaging(CWnd *pW,int Wmsg,int Smsg);
	void SendChange(void);
	void SetTheMaster(CWnd *W){m_pMaster = W;}
	void SetMsgValue(int m);
	void SetWMsgValue(int m){m_WMsg = m;}
	virtual ~CStaticBitmap();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticBitmap)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_Color;
	CWnd * m_pMaster;
	int m_Msg;
	int m_WMsg;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICBITMAP_H__443F2077_C1AA_40B1_BCF7_C261D5FB2CCE__INCLUDED_)
