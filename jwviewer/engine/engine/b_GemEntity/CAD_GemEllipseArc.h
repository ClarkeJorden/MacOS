#ifndef _CAD_GEM_ELLIPSE_ARC_H_
#define _CAD_GEM_ELLIPSE_ARC_H_

#include "CAD_GemEllipse.h"

class  CCAD_GemEllipseArc : public CCAD_GemEllipse
{
public:
	CCAD_GemEllipseArc(){};
	virtual ~CCAD_GemEllipseArc(){};
	CCAD_GemEllipseArc(const CCAD_GemEllipseArc & cEllpArc);
	CCAD_GemEllipseArc(const CCAD_GemEllipse & cEllp);
	CCAD_GemEllipseArc(const CCAD_GemEllipse & cEllp, const CCAD_Angle & cAlfa, const CCAD_Angle & cDelta);
	CCAD_GemEllipseArc & operator=(const CCAD_GemEllipseArc & cEllpArc);
	CCAD_GemEllipseArc & operator=(const CCAD_GemArc & cArc);

public:
	BYTE		m_bType;
	CCAD_Angle	m_cAlfa;
	CCAD_Angle	m_cDelta;

private:
	BYTE		m_bEllipse;

public:
	virtual void	MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual BOOL	Read(CCAD_File& cFile);
	virtual BOOL	IsClosed();

	virtual BOOL	Draw(CCAD_DC *pCadDC, BOOL bRedraw = FALSE);
	virtual void	CountBodyRange(CCAD_Rect& cRange);
	virtual void	CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange);
	virtual WORD	GetCurvePoint(SCAD_POINT *spDivPnt = NULL);

protected:
	void	FrameDraw(CCAD_DC *pDC, short n, const CCAD_Angle &cDelta);
};

#endif
