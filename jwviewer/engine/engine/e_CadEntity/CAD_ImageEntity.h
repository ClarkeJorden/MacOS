#if !defined(AFX_CAD_IMAGEENTITY_H)
#define AFX_CAD_IMAGEENTITY_H

#include "CAD_Pen.h"
#include "CAD_Brush.h"
#include "CAD_Entity.h"
#include "CAD_GemImage.h"
#include "CAD_DC.h"
#include "CAD_Doc.h"

class  CCAD_ImageEntity : public CCAD_Entity  
{
public:
	CCAD_ImageEntity(CCAD_Doc *, short, short);
	virtual ~CCAD_ImageEntity();

public:
	CCAD_Pen		m_cPen;
	CCAD_GemImage	m_cGemImage;
	
public:
	virtual	CCAD_GemEntity* GetGemEntity();
	virtual CCAD_Pen*	GetCadPen(){return &m_cPen;}
	virtual	BOOL IsIgno(BOOL isBlock);
	virtual long		ClassSize() { return sizeof(CCAD_ImageEntity); }
	virtual long		GetSize();
	virtual BOOL		Read(CCAD_File& cFile);
	virtual LPSTR		GetData1(LPSTR);
};

#endif
