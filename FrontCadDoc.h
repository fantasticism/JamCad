#pragma once

// CFrontCadDoc document

class CFrontCadDoc : public CDocument
{
	DECLARE_DYNCREATE(CFrontCadDoc)

public:
	CFrontCadDoc();
	virtual ~CFrontCadDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
