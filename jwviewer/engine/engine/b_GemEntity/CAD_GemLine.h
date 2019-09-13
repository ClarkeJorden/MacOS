#if !defined(AFX_CAD_GEMLINE_H)
#define AFX_CAD_GEMLINE_H

#include "CAD_GemEntity.h"

class   CCAD_GemLine : public CCAD_GemThickEntity
{
public:
	CCAD_GemLine();
	CCAD_GemLine(const CCAD_Coord3D & cBcs, const CCAD_Vector & cSp, const CCAD_Vector & cEp);
	CCAD_GemLine(const CCAD_GemLine & cGemLine);
	CCAD_GemLine(const CCAD_Vector & cSp, const CCAD_Vector & cEp);
	virtual ~CCAD_GemLine();

public:
	CCAD_Vector		m_cSp;
	CCAD_Vector		m_cEp;

public:
	CCAD_GemLine & operator=(const CCAD_GemLine & cGemLine);
	virtual void	MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual BOOL	Read(CCAD_File& cFile);
	virtual BOOL	Draw(CCAD_DC *pCadDC, BOOL bRedraw = FALSE);
	virtual void	CountBodyRange(CCAD_Rect& cRange);
	virtual void	CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange);
	virtual WORD	GetCurvePoint(SCAD_POINT *spDivPnt = NULL);

	CAD_FLOAT	GetLength() const;
	CCAD_Angle	GetAngle() const;
	CAD_FLOAT	CountTriArea(const CCAD_Vector & cCp) const;
	CAD_FLOAT	CountDistanceToPoint(const CCAD_Vector & cPt) const;
	BYTE		PointBySide(const CCAD_Vector & cPt) const;
	BYTE		PointOnInterval(const CCAD_Vector & cPt) const;
	BOOL		IsPtInterval(const CCAD_Vector & cPt, CAD_FLOAT fAccy = SCALA_ACCY) const;
	BOOL		IsCrossHorizontal(const CAD_FLOAT & fHrztY, CAD_FLOAT & fGetX) const;
	BOOL		IsCrossVertical(const CAD_FLOAT & dVrtX, CAD_FLOAT & dGetY) const;
	BYTE		CrossTwoLine(const CCAD_GemLine & cLine) const;
	BYTE		CrossTwoLine(const CCAD_Angle &inAngle, const CCAD_Vector & cPt) const;
	BYTE		PtDirect(const CCAD_Vector & cPt) const;
	BOOL		CountSlope(const CCAD_Vector cPt, const CCAD_Angle &cValue, const CCAD_Angle & cSlope, BYTE bFlag);
	BYTE		CountCrossPtWithLine(const CCAD_GemLine & cLine, CCAD_Vector & cVp) const;
	BOOL		CountCrossPt(CCAD_Linear2D &cLine, CCAD_Vector &ptCross) const;
	BOOL		IsCullFace() { return FALSE;}
};

#endif
