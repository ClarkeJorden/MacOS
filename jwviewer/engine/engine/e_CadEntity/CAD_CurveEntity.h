#if !defined(AFX_CAD_CURVEENTITY_H)
#define AFX_CAD_CURVEENTITY_H

#include "CAD_Pen.h"
#include "CAD_Brush.h"
#include "CAD_Entity.h"
#include "CAD_GemCurve.h"
#include "CAD_DC.h"
#include "CAD_Doc.h"

class  CCAD_CurveEntity : public CCAD_Entity  
{
public:
	CCAD_CurveEntity(CCAD_Doc *, short, short);
	virtual ~CCAD_CurveEntity();

public:
	CCAD_Pen		m_cPen;
	CCAD_Brush		m_cBrush;
	CCAD_GemCurve	m_cGemCurve;
	
public:
	virtual	CCAD_GemEntity* GetGemEntity(){return &m_cGemCurve;}
	virtual	BOOL IsIgno(BOOL isBlock);
	virtual CCAD_Pen* GetCadPen(){return &m_cPen;}
	virtual CCAD_Brush* GetCadBrush(){return &m_cBrush;}
	virtual long ClassSize() { return sizeof(CCAD_CurveEntity); }
	virtual long GetSize();
	virtual BOOL Read(CCAD_File& cFile);
	virtual LPSTR	GetData1(LPSTR);
	virtual LPSTR	GetData2(LPSTR);
	virtual LPSTR	GetData3(LPSTR);
};

#endif
