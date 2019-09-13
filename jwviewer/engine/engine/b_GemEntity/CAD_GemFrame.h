#ifndef _CAD_GEM_FRAME_H_
#define _CAD_GEM_FRAME_H_

#include "CAD_GemEntity.h"

#define		PT_INFRAME				0x01
#define		NORMAL_INFRAME			0x02
#define		LEFT_CAPSEL_INFRAME		0x04
#define		RIGHT_CAPSEL_INFRAME	0x08

class  CCAD_GemFrame : public CCAD_GemThickEntity  
{
public:
	CCAD_GemFrame();
	virtual ~CCAD_GemFrame();

public:
	BYTE			m_bDraw;
	short			m_nTotGrp;
	short			m_nTotPt;
	short	*		m_pNum;
	CCAD_Vector *	m_pPt;
	
public:
	virtual void	MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual void	Remove();
	virtual BOOL	Read(CCAD_File& cFile);
	virtual BOOL	IsClosed(){return TRUE;}
	virtual BOOL	Draw(CCAD_DC *pCadDC, BOOL bRedraw = FALSE);
	virtual void	CountBodyRange(CCAD_Rect& cRange);
	virtual void	CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange);
	virtual WORD	GetCurvePoint(SCAD_POINT *spDivPnt = NULL);

	void	Init();
	void	RemoveFrameAll();
};

#endif
