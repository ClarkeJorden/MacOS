#ifndef _CAD_GEM_POINT_H_
#define _CAD_GEM_POINT_H_

#include "CAD_GemCircle.h"

#define		POINT_NUM		20
#define		POINT_PAINT		0x01
#define		POINT_CROSS1	0x02
#define		POINT_CROSS2	0x04
#define		POINT_XCROSS	0x08
#define		POINT_BAR		0x10
#define		POINT_CIRCLE	0x20
#define		POINT_RECT		0x40

class  CCAD_GemPoint : public CCAD_Gem2DEntity  
{
public:
	CCAD_GemPoint();
	virtual ~CCAD_GemPoint();
	CCAD_GemPoint(const CCAD_GemPoint & cPoint);
	CCAD_GemPoint(const CCAD_Coord3D &cBcs, const BYTE bType, const CCAD_Vector & cPt, const CAD_FLOAT & fR);

public:
	BYTE		m_bType;
	CCAD_Vector	m_cPt;
	CAD_FLOAT	m_fR;
	CCAD_Point		*m_pTriPoints;
	int							m_nTriCount;

public:
	CCAD_GemPoint & operator=(const CCAD_GemPoint & cPoint);
	virtual void	MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual BOOL	Read(CCAD_File& cFile);
	virtual BOOL	Draw(CCAD_DC *pCadDC, BOOL bRedraw = FALSE);
	virtual void	CountBodyRange(CCAD_Rect& cRange);
	virtual void	CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange);
	
	void SetType(BYTE & bType) const;
	void DrawLittle(CCAD_DC *pDC) const;
	void DrawRect(CCAD_DC *pDC, const CCAD_GemCircle & cCircle) const;
	void CountRectVcsRange(CCAD_DC *pDC, CCAD_Rect &cRange, const CCAD_GemCircle & cCircle) const;
	void DrawCross(CCAD_DC *pDC, const CCAD_GemCircle & cCircle) const;
	void DrawXcross(CCAD_DC *pDC, const CCAD_GemCircle & cCircle) const;
	void DrawBar(CCAD_DC *pDC, const CCAD_GemCircle & cCircle) const;
	void CountBarVcsRange(CCAD_DC *pDC, CCAD_Rect &cRange, const CCAD_GemCircle & cCircle) const;
	void DrawCircle(CCAD_DC *pDC, const CCAD_GemCircle & cCircle) const;
	void DrawPaint(CCAD_DC *pDC, const CCAD_GemCircle & cCircle, BOOL blPaint = TRUE);
};

#endif
