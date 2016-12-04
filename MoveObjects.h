// MoveObjects.h: interface for the CMoveObjects class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CCadObject;
class CScale;

class CMoveObjects  
{
	CPoint m_Ref;
	int m_nObjs;		//number of objects in list
	CCadObject * m_pHead;
	CCadObject * m_pTail;
public:
	CMoveObjects();
	virtual ~CMoveObjects();
	void Clear(int nDeleteFlag=0);
	void SetRef(CPoint Ref);
	void Draw(CDC *pDC,CPoint Off,CScale Scale);
	CCadObject *GetHead(void){return m_pHead;}
	CCadObject * RemoveObject(CCadObject *pO);
	void AddObject(CCadObject *pO);
	void Copy(CMoveObjects & Src);// Copy a MoveObjects class
	void Copy(CMoveObjects *pSrc);// Copy a MoveObjects class
	CCadObject * RemoveFromHead();// Removes object from front of list
	CCadObject * RemoveFromTail();
	inline int IsReadyToPaste(void) { return m_nObjs; }
};

