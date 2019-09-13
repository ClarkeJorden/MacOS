#if !defined(AFX_CAD_TEXTENTITY_H)
#define AFX_CAD_TEXTENTITY_H

#include "CAD_Pen.h"
#include "CAD_Entity.h"
#include "CAD_GemText.h"
#include "CAD_DC.h"
#include "CAD_Doc.h"

class  CCAD_TextEntity : public CCAD_Entity  
{
public:
	CCAD_TextEntity(CCAD_Doc *, short, short);
	virtual ~CCAD_TextEntity();

public:
	CCAD_Pen		m_cPen;
	CCAD_GemText	m_cGemText;

public:
	virtual	CCAD_GemEntity* GetGemEntity(){return &m_cGemText;}
	virtual	BOOL IsIgno(BOOL isBlock);
	virtual CCAD_Pen* GetCadPen(){return &m_cPen;}
	virtual long ClassSize() { return sizeof(CCAD_TextEntity); }
	virtual long GetSize();
	virtual void MemCopyTo(CCAD_Entity * pEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual BOOL Read(CCAD_File& cFile);
	virtual LPSTR	GetData1(LPSTR);
	virtual BOOL Draw(CCAD_DC* pCadDC, BOOL blRedraw=FALSE);
	virtual void RemoveLocal();
};

#endif
