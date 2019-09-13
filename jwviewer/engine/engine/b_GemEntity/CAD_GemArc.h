#ifndef _CAD_GEM_ARC_H_
#define _CAD_GEM_ARC_H_

#include "CAD_GemCircle.h"

class  CCAD_GemArc : public CCAD_GemCircle
{
public:
	CCAD_GemArc();
	CCAD_GemArc(const CCAD_Coord3D & cBcs, const CCAD_Point & cCp, const CAD_FLOAT &fR);
	CCAD_GemArc(CCAD_GemCircle * pCircle);
	CCAD_GemArc(const CCAD_GemCircle & cCircle, const CCAD_Angle & cAlfa, const CCAD_Angle & cDelta, BYTE bType);
	CCAD_GemArc(const CCAD_GemArc& cArc);
	virtual ~CCAD_GemArc();

public:
	BYTE			m_bType;
	CCAD_Angle		m_cAlfa;
	CCAD_Angle		m_cDelta;
	CCAD_Point		*m_pTriPoints;
	int							m_nTriCount;

public:
	CCAD_GemArc &	operator=(const CCAD_GemArc & cArc);
	virtual void MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual BOOL	Read(CCAD_File& cFile);
	virtual BOOL	IsClosed();
	virtual BOOL	IsCullFace() {return FALSE;}

	virtual BOOL	Draw(CCAD_DC *pCadDC, BOOL bRedraw = FALSE);
	virtual void	CountBodyRange(CCAD_Rect& cRange);
	virtual void	CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange);
	virtual WORD	GetCurvePoint(SCAD_POINT *spDivPnt = NULL);

protected:
	void			FrameDraw(CCAD_DC *pDC, short n, const CCAD_Angle &cDelta);
};

#endif
