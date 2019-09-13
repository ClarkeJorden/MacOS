#if !defined(AFX_CAD_PAINTENTITY_H)
#define AFX_CAD_PAINTENTITY_H

#include "CAD_Pen.h"
#include "CAD_Brush.h"
#include "CAD_Entity.h"
#include "CAD_GemPaint.h"
#include "CAD_DC.h"
#include "CAD_Doc.h"

class  CCAD_PaintEntity : public CCAD_Entity  
{
public:
	CCAD_PaintEntity(CCAD_Doc *, short, short);
	virtual ~CCAD_PaintEntity();

public:
	CCAD_Pen		m_cPen;
	CCAD_Brush		m_cBrush;
	CCAD_GemPaint	m_cGemPaint;
	
public:
	virtual	CCAD_GemEntity* GetGemEntity(){return &m_cGemPaint;}
	virtual	BOOL IsIgno(BOOL isBlock);
	virtual CCAD_Pen* GetCadPen(){return &m_cPen;}
	virtual CCAD_Brush* GetCadBrush(){return &m_cBrush;}
	virtual long ClassSize() { return sizeof(CCAD_PaintEntity); }
	virtual long GetSize();
	virtual BOOL Read(CCAD_File& cFile);
	virtual LPSTR	GetData1(LPSTR);
	virtual LPSTR	GetData2(LPSTR);
};

#endif
