#pragma once


// CPcbLibView view

class CPcbLibView : public CView
{
	DECLARE_DYNCREATE(CPcbLibView)

protected:
	CPcbLibView();           // protected constructor used by dynamic creation
	virtual ~CPcbLibView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


