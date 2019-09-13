#if !defined(AFX_CAD_ELLIPSEENTITY_H)
#define AFX_CAD_ELLIPSEENTITY_H

#include "resource.h"

#include "CAD_Pen.h"
#include "CAD_Brush.h"
#include "CAD_Entity.h"
#include "CAD_GemEllipse.h"
#include "CAD_EllipseArcEntity.h"
#include "CAD_DC.h"
#include "CAD_Doc.h"

class  CCAD_EllipseEntity : public CCAD_Entity  
{
public:
	CCAD_EllipseEntity(CCAD_Doc *, short, short);
	virtual ~CCAD_EllipseEntity();
	CCAD_EllipseEntity(const CCAD_EllipseEntity &);
	CCAD_EllipseEntity(const CCAD_EllipseArcEntity &);
	CCAD_EllipseEntity & operator=(const CCAD_EllipseEntity &);

public:
	CCAD_Pen		m_cPen;
	CCAD_Brush		m_cBrush;
	CCAD_GemEllipse	m_cGemEllipse;
	
public:
	virtual	CCAD_GemEntity* GetGemEntity(){return &m_cGemEllipse;}
	virtual	BOOL IsIgno(BOOL isBlock);
	virtual CCAD_Pen* GetCadPen(){return &m_cPen;}
	virtual CCAD_Brush* GetCadBrush(){return &m_cBrush;}
	virtual long ClassSize() { return sizeof(CCAD_EllipseEntity); }
	virtual long GetSize();
	virtual BOOL Read(CCAD_File& cFile);

	void CvtEllipseArcEntity(CCAD_EllipseArcEntity & cEllipseArc);
};

#endif
