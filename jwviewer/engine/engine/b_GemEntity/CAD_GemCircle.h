#ifndef _CAD_GEM_CIRCLE_H_
#define _CAD_GEM_CIRCLE_H_

#include "CAD_GemEntity.h"
#include "CAD_GemLine.h"

class CCAD_GemCircle : public CCAD_GemThickEntity
{
public:
	CCAD_GemCircle();
	CCAD_GemCircle(const CCAD_GemCircle & cCircle);
	CCAD_GemCircle(const CCAD_Coord3D & cBcs, const CCAD_Vector & cCp, const CAD_FLOAT & fR, BYTE bDraw);
	virtual ~CCAD_GemCircle();


public:
	CCAD_Point		m_cCp;
	BYTE			m_bDraw;
	CAD_FLOAT		m_fR;

public:
	CCAD_GemCircle &operator=(const CCAD_GemCircle & cCircle);
	virtual void MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual BOOL	Read(CCAD_File& cFile);
	virtual BOOL	IsClosed(){return TRUE;}

	virtual BOOL	Draw(CCAD_DC *pCadDC, BOOL bRedraw = FALSE);
	virtual void	CountBodyRange(CCAD_Rect& cRange);
	virtual void	CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange);
	virtual WORD	GetCurvePoint(SCAD_POINT *spDivPnt = NULL);

	void	CountCirclePt(CCAD_Vector & cPt, const CAD_FLOAT &fSin, const CAD_FLOAT &fCos) const;
	BYTE	CountTangentPt(const CCAD_Vector & cPt, CCAD_Vector & cGp1, CCAD_Vector & cGp2) const;
	BOOL	CountFromRadius(const CAD_FLOAT &fR);
	BOOL	CountFrom2Entity(const CCAD_Vector &cPt1, const CCAD_Vector &cPt2);
	BOOL	Count2P(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2);
	BOOL	CountPP(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2);
	BYTE	CountRootFromLinear(const CCAD_Equation2D & cEquation, const CCAD_Linear2D & cLinear);
	BYTE	CountRootFromParabola(const CCAD_Equation2D & cEquation, const CCAD_Parabola2D & cPara);
	BOOL	CountFrom3Entity(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2, const CCAD_Vector & cPt3);
	BOOL	CountPPP(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2, const CCAD_Vector & cPt3);
	BOOL	CountWithCenterFrom2Entity(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2, const CCAD_Vector & cCp);
	BOOL	CountWithCenterPP(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2, const CCAD_Vector & cCp);
	BOOL	CountWithRadiusFrom2Entity(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2, const CAD_FLOAT & dR);
	BOOL	CountWithRadiusPP(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2, const CAD_FLOAT & dR);
	BYTE	CountCrossPtWithLine(const CCAD_GemLine & cLine, CCAD_Vector & cPt1, CCAD_Vector & cPt2) const;
	BYTE	CountCrossPtWithLineRay(const CCAD_GemLine & cLine, CCAD_Vector & cPt1, CCAD_Vector & cPt2) const;
	BYTE	CountCrossPtWithCircle(const CCAD_GemCircle & cCircle, CCAD_Vector & cPt1, CCAD_Vector & cPt2) const;
	BYTE	CountCrossPtWithCircle(const CCAD_Vector & cCp, const CAD_FLOAT & fR, CCAD_Vector & cPt1, CCAD_Vector & cPt2) const;

protected:
	void	FrameDraw(CCAD_DC *pDC, short n, CCAD_Point *pPt);
};

#endif
