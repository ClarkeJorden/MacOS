#if !defined(AFX_CAD_ELLIPSEARCENTITY_H)
#define AFX_CAD_ELLIPSEARCENTITY_H

#include "resource.h"

#include "CAD_Pen.h"
#include "CAD_Brush.h"
#include "CAD_Entity.h"
#include "CAD_GemEllipseArc.h"
#include "CAD_DC.h"
#include "CAD_Doc.h"

class  CCAD_EllipseArcEntity : public CCAD_Entity  
{
public:
	CCAD_EllipseArcEntity(CCAD_Doc *, short, short);
	virtual ~CCAD_EllipseArcEntity();
	CCAD_EllipseArcEntity(const CCAD_EllipseArcEntity &);
	CCAD_EllipseArcEntity & operator=(const CCAD_EllipseArcEntity &);

public:
	CCAD_Pen			m_cPen;
	CCAD_Brush			m_cBrush;
	CCAD_GemEllipseArc	m_cGemEllipseArc;
	
public:
	virtual	CCAD_GemEntity* GetGemEntity(){return &m_cGemEllipseArc;}
	virtual	BOOL IsIgno(BOOL isBlock);
	virtual CCAD_Pen* GetCadPen(){return &m_cPen;}
	virtual CCAD_Brush* GetCadBrush(){return &m_cBrush;}
	virtual long ClassSize() { return sizeof(CCAD_EllipseArcEntity); }
	virtual long GetSize();
	virtual BOOL Read(CCAD_File& cFile);
};

#endif
