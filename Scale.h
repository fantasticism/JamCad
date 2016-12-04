// Scale.h: interface for the CScale class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALE_H__7A26F6C7_087C_4789_AC92_36DEF953BE19__INCLUDED_)
#define AFX_SCALE_H__7A26F6C7_087C_4789_AC92_36DEF953BE19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CScale  
{
public:
	CSize operator *(CSize &c);
	double m_ScaleX;
	double m_ScaleY;
	CScale(){};
	CScale(double x,double y);
	virtual ~CScale();
	CPoint operator*(CPoint &p);
};

#endif // !defined(AFX_SCALE_H__7A26F6C7_087C_4789_AC92_36DEF953BE19__INCLUDED_)
