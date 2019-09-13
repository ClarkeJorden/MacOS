#if !defined(AFX_CAD_PLINE2DENTITY_H)
#define AFX_CAD_PLINE2DENTITY_H

#include "resource.h"
#include "CAD_Pen.h"
#include "CAD_Entity.h"
#include "CAD_GemPline2D.h"
#include "CAD_DC.h"
#include "CAD_Doc.h"

class  CCAD_Pline2DEntity : public CCAD_Entity  
{
public:
	CCAD_Pline2DEntity(CCAD_Doc *, short, short);
	virtual ~CCAD_Pline2DEntity();

public:
	CCAD_Pen		m_cPen;
	CCAD_GemPline2D	m_cGemPline2D;

public:
	virtual	CCAD_GemEntity* GetGemEntity(){return &m_cGemPline2D;}
	virtual	BOOL IsIgno(BOOL isBlock);
	virtual CCAD_Pen* GetCadPen(){return &m_cPen;}
	virtual long ClassSize() { return sizeof(CCAD_Pline2DEntity); }
	virtual long GetSize();
	virtual BOOL Read(CCAD_File& cFile);
	virtual LPSTR	GetData1(LPSTR);
};

#endif
