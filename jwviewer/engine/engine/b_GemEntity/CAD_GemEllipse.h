#ifndef _CAD_GEM_ELLIPSE_H_
#define _CAD_GEM_ELLIPSE_H_

#include "CAD_GemEntity.h"
#include "CAD_GemLine.h"
#include "CAD_GemArc.h"

class  CCAD_GemEllipse : public CCAD_GemThickEntity
{
public:
	CCAD_GemEllipse(){};
	virtual ~CCAD_GemEllipse(){};
	CCAD_GemEllipse(const CCAD_GemEllipse & cEllp);
	CCAD_GemEllipse(const CCAD_Coord3D & cBcs, const CCAD_Vector & cCp, const CAD_FLOAT & fRx, const CAD_FLOAT & fRy, const CCAD_Angle & cDirect, BYTE bDraw);
	CCAD_GemEllipse & operator=(const CCAD_GemEllipse & cEllp);
	CCAD_GemEllipse & operator=(const CCAD_GemCircle & cCircle);

public:
	BYTE		m_bDraw;
	CCAD_Point	m_cCp;
	CAD_FLOAT	m_fRx;
	CAD_FLOAT	m_fRy;
	CCAD_Angle	m_cDirect;

public:
	virtual void MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual BOOL	Read(CCAD_File& cFile);
	virtual BOOL	IsClosed(){return TRUE;}

	virtual BOOL	Draw(CCAD_DC *pCadDC, BOOL bRedraw = FALSE);
	virtual void	CountBodyRange(CCAD_Rect& cRange);
	virtual void	CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange);
	virtual WORD	GetCurvePoint(SCAD_POINT *spDivPnt = NULL);


	BYTE InSidePt(const CCAD_Vector & cPt)  const;
	void CountEllpPt(CCAD_Vector & cPt, const CCAD_Angle & cAlfa, const CAD_FLOAT & dSin, const CAD_FLOAT & dCos)  const;

protected:
	void	FrameDraw(CCAD_DC *pDC, short n, CCAD_Point *pPt);
};

#endif
