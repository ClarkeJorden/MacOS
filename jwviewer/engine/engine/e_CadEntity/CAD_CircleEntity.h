// CAD_CircleEntity.h: interface for the CCAD_CircleEntity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_CIRCLEENTITY_H)
#define AFX_CAD_CIRCLEENTITY_H

#include "resource.h"

#include "CAD_Pen.h"
#include "CAD_Brush.h"
#include "CAD_Entity.h"
#include "CAD_GemCircle.h"
#include "CAD_ArcEntity.h"
#include "CAD_DC.h"
#include "CAD_Doc.h"

class  CCAD_CircleEntity : public CCAD_Entity
{
public:
	CCAD_CircleEntity(CCAD_Doc *, short, short);
	virtual ~CCAD_CircleEntity();
	CCAD_CircleEntity(const CCAD_CircleEntity &);
	CCAD_CircleEntity(const CCAD_ArcEntity & cArc);
	CCAD_CircleEntity & operator=(const CCAD_CircleEntity &);

public:
	CCAD_Pen		m_cPen;
	CCAD_Brush		m_cBrush;
	CCAD_GemCircle	m_cGemCircle;
	
public:
	virtual	CCAD_GemEntity* GetGemEntity(){return &m_cGemCircle;}
	virtual	BOOL IsIgno(BOOL isBlock);
	virtual CCAD_Pen* GetCadPen(){return &m_cPen;}
	virtual CCAD_Brush* GetCadBrush(){return &m_cBrush;}
	virtual long ClassSize() { return sizeof(CCAD_CircleEntity); }
	virtual long GetSize();
	virtual BOOL Read(CCAD_File& cFile);

	void CvtArcEntity(CCAD_ArcEntity & cArc);
};

#endif
