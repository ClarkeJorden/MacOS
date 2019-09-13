#if !defined(AFX_CAD_PROC_H)
#define AFX_CAD_PROC_H

#include "CAD_Vector.h"
#include "Kon_Array.h"

typedef struct _SRGN_TRIANGLE {
	int		nNum[3];
} SRGN_TRIANGLE;

typedef struct _SRGN_CONTOUR {
	int		nCount;
	int		nMinIdx;
	CCAD_Point*	cpPoint;
} SRGN_CONTOUR;

typedef	CKon_Array < SRGN_TRIANGLE, SRGN_TRIANGLE >	CRGN_TriangleArray;
typedef	CKon_Array < SRGN_CONTOUR, SRGN_CONTOUR >	CRGN_ContourArray;

class  CCAD_RgnProc  
{
public:
	CCAD_RgnProc();
	virtual ~CCAD_RgnProc();

public:
	CPointArray		m_cTmpPoints;
	CPointArray		m_cMeasPoints;
	CRGN_ContourArray		m_cContours;
	CRGN_TriangleArray		m_cTriangles;
	
public:
	void		AddPts(int nTotPt, CCAD_Point* cpPts);
	void		AddPts(CPointArray& cPts, int nIndex);
	void		MakeWholeTri();

protected:
	void	MakeOneContour();
	CCAD_Point*	MergeTwoContour(CCAD_Point* cpOuterPoly, int& nCount, SRGN_CONTOUR& sContour);

	void	MakeCounterClockWise();
	void	CounterClockWise(SRGN_TRIANGLE& sTri);

	BOOL	IsIntersectPoint(short nStart, short nEnd, short *npVtxIndex, short nVtxNum);
	BOOL	IsInPolygon(short nTriNum1, short nTriNum2, short nTriNum3, short *npVtxIndex, short nVtxCnt);
	BOOL	IsInPolygon(CCAD_Point centerpnt);
	BOOL	IsInPolygon(CCAD_Point centerpnt, CCAD_Point* cpPts, short* npVtxIndex, short nVtxCnt);
	BOOL	IsIntersectPoint(CCAD_Point pnt1, CCAD_Point pnt2, CCAD_Point pnt3, CCAD_Point pnt4);
	void	GetVerticalPnt(CCAD_Point pnt1, CCAD_Point pnt2, CCAD_Point pnt3, CCAD_Point &result);
	CAD_FLOAT	GetAngle(CAD_FLOAT centerpntx, CAD_FLOAT centerpnty, CAD_FLOAT startpntx, CAD_FLOAT startpnty, CAD_FLOAT endpntx, CAD_FLOAT endpnty);
	BOOL	IsValidTriangle(short nStart, short nMid, short nEnd, short* npVtxIndex);
};

#endif
