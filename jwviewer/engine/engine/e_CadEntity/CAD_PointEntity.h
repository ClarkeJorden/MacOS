#if !defined(AFX_CAD_POINTENTITY_H)
#define AFX_CAD_POINTENTITY_H

#include "CAD_Pen.h"
#include "CAD_Brush.h"
#include "CAD_Entity.h"
#include "CAD_GemPoint.h"
#include "CAD_DC.h"
#include "CAD_Doc.h"

class  CCAD_PointEntity : public CCAD_Entity  
{
public:
	CCAD_PointEntity(CCAD_Doc *, short, short);
	virtual ~CCAD_PointEntity();
	CCAD_PointEntity(const CCAD_PointEntity &);
	CCAD_PointEntity & operator=(const CCAD_PointEntity &);

public:
	CCAD_Pen		m_cPen;
	CCAD_GemPoint	m_cGemPoint;

public:
	virtual	CCAD_GemEntity* GetGemEntity(){return &m_cGemPoint;}
	virtual	BOOL IsIgno(BOOL isBlock);
	virtual CCAD_Pen* GetCadPen(){return &m_cPen;}
	virtual long ClassSize() { return sizeof(CCAD_PointEntity); }
	virtual long GetSize();
	virtual BOOL Read(CCAD_File& cFile);
	virtual	BOOL Draw(CCAD_DC* pCadDC, BOOL blRedraw = FALSE);
};

#endif
