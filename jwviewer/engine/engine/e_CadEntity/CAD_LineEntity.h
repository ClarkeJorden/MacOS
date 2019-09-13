// CAD_LineEntity.h: Definition of the CCAD_LineEntity class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_LINEENTITY_H)
#define AFX_CAD_LINEENTITY_H

#include "resource.h"

#include "CAD_Pen.h"
#include "CAD_Entity.h"
#include "CAD_GemLine.h"
#include "CAD_DC.h"
#include "CAD_Doc.h"

 

/////////////////////////////////////////////////////////////////////////////
// CCAD_LineEntity

class   CCAD_LineEntity : public CCAD_Entity
{
public:
	CCAD_LineEntity(CCAD_Doc *, short, short);
	virtual ~CCAD_LineEntity();
	CCAD_LineEntity(const CCAD_LineEntity &);
	CCAD_LineEntity & operator=(const CCAD_LineEntity &);

public:
	CCAD_Pen m_cPen;
	CCAD_GemLine m_cGemLine;
	
public:
	virtual	CCAD_GemEntity* GetGemEntity(){return &m_cGemLine;}
	virtual CCAD_Pen* GetCadPen(){return &m_cPen;}
	virtual long ClassSize() { return sizeof(CCAD_LineEntity); }
	virtual long GetSize();
	virtual BOOL Read(CCAD_File& cFile);

	virtual	BOOL	IsIgno(BOOL isBlock);
};

#endif
