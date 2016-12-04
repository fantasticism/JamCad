#pragma once


// CPropertyEditor

class CPropertyEditor : public CStatic
{
	DECLARE_DYNAMIC(CPropertyEditor)

public:
	CPropertyEditor();
	virtual ~CPropertyEditor();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


