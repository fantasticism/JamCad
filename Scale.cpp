// Scale.cpp: implementation of the CScale class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JamCad.h"
#include "Scale.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScale::CScale(double x,double y)
{
	m_ScaleX = x;
	m_ScaleY = y;
}

CScale::~CScale()
{

}

CPoint CScale::operator*(CPoint &p)
{
	CPoint rP;
	rP.x = (int)(p.x * m_ScaleX);
	rP.y = (int)(p.y * m_ScaleY);
	return rP;
}

CSize CScale::operator *(CSize &c)
{
	CSize rC;
	rC.cx = (int)(c.cx * m_ScaleX);
	rC.cy = (int)(c.cy * m_ScaleY);
	return rC;
}
