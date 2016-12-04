#include "stdafx.h"
#include "PropSelString.h"


CPropSelString::CPropSelString()
{
}


CPropSelString::~CPropSelString()
{
}

void CPropSelString::Create(char **ppData)
{
	int i, count;
	for (count = 0; ppData[count]; ++count);
	m_SelNames = new char *[count];
	for (i = 0; ppData[i]; ++i)
	{
		m_SelNames[i] = ppData[i];
	}
}

int CPropSelString::GetSelection()
{
	return m_Selection;
}


char * CPropSelString::GetSelectedValue()
{
	return m_SelNames[m_Selection];
}

void CPropSelString::SetSelection(int i)
{
	if (i >= m_MaxSelection) i = 0;
	if (i < 0) i = m_MaxSelection-1;
	m_Selection = i;
}

CPropField *CPropSelString::Copy()
{
	CPropSelString *pNewPropString = new CPropSelString;
	return pNewPropString;
}

