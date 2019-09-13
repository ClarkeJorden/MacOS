#if !defined(AFX_CAD_ARCENTITY_H)
#define AFX_CAD_ARCENTITY_H

#include "resource.h"

#include "CAD_Entity.h"
#include "CAD_GemArc.h"

class CCAD_ArcEntity : public CCAD_Entity
{
public:
	CCAD_ArcEntity(CCAD_Doc *, short, short);
	virtual ~CCAD_ArcEntity();
	CCAD_ArcEntity(const CCAD_ArcEntity &);
	CCAD_ArcEntity & operator=(const CCAD_ArcEntity &);

public:
	CCAD_Pen	m_cPen;
	CCAD_Brush	m_cBrush;
	CCAD_GemArc m_cGemArc;
	
public:
	virtual	CCAD_GemEntity* GetGemEntity(){return &m_cGemArc;}
	virtual	BOOL IsIgno(BOOL isBlock);
	virtual CCAD_Pen* GetCadPen(){return &m_cPen;}
	virtual CCAD_Brush* GetCadBrush(){return &m_cBrush;}
	virtual long ClassSize() { return sizeof(CCAD_ArcEntity); }
	virtual long GetSize();
	virtual BOOL Read(CCAD_File& cFile);
};

#endif
