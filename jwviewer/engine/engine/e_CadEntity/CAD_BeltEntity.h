#if !defined(AFX_CAD_BELTENTITY_H)
#define AFX_CAD_BELTENTITY_H

#include "CAD_Pen.h"
#include "CAD_Brush.h"
#include "CAD_Entity.h"
#include "CAD_GemBelt.h"
#include "CAD_DC.h"
#include "CAD_Doc.h"

class  CCAD_BeltEntity : public CCAD_Entity
{
public:
	CCAD_BeltEntity(CCAD_Doc *, short, short);
	virtual ~CCAD_BeltEntity();

public:
	CCAD_Pen		m_cPen;
	CCAD_Brush		m_cBrush;
	CCAD_GemBelt	m_cGemBelt;

public:
	virtual	CCAD_GemEntity* GetGemEntity(){return &m_cGemBelt;}
	virtual	BOOL IsIgno(BOOL isBlock);
	virtual CCAD_Pen* GetCadPen(){return &m_cPen;}
	virtual CCAD_Brush* GetCadBrush(){return &m_cBrush;}
	virtual long ClassSize() { return sizeof(CCAD_BeltEntity); }
	virtual long GetSize();
	virtual BOOL Read(CCAD_File& cFile);
	virtual LPSTR	GetData1(LPSTR);
};

#endif
