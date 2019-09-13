#ifndef _CAD_GEM_PAINT_H_
#define _CAD_GEM_PAINT_H_

#include "CAD_GemFrame.h"

class  CCAD_GemPaint : public CCAD_GemFrame
{
public:
	CCAD_GemPaint();
	virtual ~CCAD_GemPaint();

	CCAD_Point		*m_pTriPoints;
	int							m_nTriCount;

public:
	virtual void	Remove();
	virtual BOOL	Draw(CCAD_DC *pDC, BOOL blRedraw = FALSE);
	virtual void	CountBodyRange(CCAD_Rect& cRange);
	virtual void	CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange);
	
	virtual BOOL	IsThick() {return FALSE;}

	virtual CAD_FLOAT GetThick() const {return 0;}
	virtual void SetThick(CAD_FLOAT fThick){m_cThick.m_fZ = 0;}
};

#endif
