// CAD_Proc.cpp: implementation of the CCAD_RgnProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CAD_GemTContour.h"
#include "CAD_RgnDelauProc.h"

CCAD_RgnProc::CCAD_RgnProc()
{
	m_cMeasPoints.RemoveAll();
	m_cTriangles.RemoveAll();
	m_cContours.RemoveAll();
}

CCAD_RgnProc::~CCAD_RgnProc()
{
	m_cMeasPoints.RemoveAll();
	m_cTriangles.RemoveAll();

	int i, nSize = m_cContours.GetSize();
	for (i = 0; i < nSize; i++)
	{
		CCAD_Point* cpPt = m_cContours[i].cpPoint;
		delete [] cpPt;
	}
	m_cContours.RemoveAll();
}

void CCAD_RgnProc::AddPts(CPointArray& cPts, int nIndex)
{
	int	nSize = cPts.GetSize();
	CCAD_Point* cpPoint = new CCAD_Point [nSize];
	if (cpPoint == NULL)
		return;
	cpPoint[0] = cPts[0];
	CAD_FLOAT	fMinX = cpPoint[0].m_fX;
	int			i, nMinIdx = 0;

	for (i = 1; i < nSize; i++)
	{
		cpPoint[i] = cPts[i];
		if (fMinX > cpPoint[i].m_fX)
		{
			nMinIdx = i;
			fMinX = cpPoint[i].m_fX;
		}
	}

	SRGN_CONTOUR	sContour;
	sContour.nCount = nSize;
	sContour.nMinIdx = nMinIdx;
	sContour.cpPoint = cpPoint;

	nSize = m_cContours.GetSize();
	for (i = 1; i < nSize; i++)
	{
		int nIdx = m_cContours[i].nMinIdx;
		CCAD_Point* cpPt = m_cContours[i].cpPoint;
		if (fMinX < cpPt[nIdx].m_fX)
		{
			break;
		}
	}
	if (i < nSize)
		m_cContours.InsertAt(i, sContour);
	else
		m_cContours.Add(sContour);
}

void CCAD_RgnProc::AddPts(int nTotPt, CCAD_Point* cpPts)
{
	short	i;

	for (i = 0; i < nTotPt; i++)
	{
		m_cMeasPoints.Add(cpPts[i]);
	}
}

CCAD_Point*	CCAD_RgnProc::MergeTwoContour(CCAD_Point* cpOuterPoly, int& nCount, SRGN_CONTOUR& sContour)
{
	CCAD_Point	cRefPt = sContour.cpPoint[sContour.nMinIdx];
	CAD_FLOAT	fRefY = cRefPt.m_fY, fRefX = cRefPt.m_fX;
	CCAD_Point	cCrossPt;
	CAD_FLOAT	fMinX = MIN_DOUBLE;
	int			nCrossIdx = -1;
	int			i, nCrossState = 0;	// 0 : none, 1 : node, 2 : middle

	for (i = 0; i < nCount; i++)
	{
		CCAD_Point	cPt0 = cpOuterPoly[i % nCount];
		CCAD_Point	cPt1 = cpOuterPoly[(i+1) % nCount];
		if (cPt0.m_fY < fRefY && cPt1.m_fY < fRefY)
			continue;
		if (cPt0.m_fY > fRefY && cPt1.m_fY > fRefY)
			continue;
		if (cPt0.m_fX > fRefX && cPt1.m_fX > fRefX)
			continue;
		if (IS_ZERO(fRefY - cPt0.m_fY) && IS_ZERO(fRefY - cPt1.m_fY))
		{
			if (cPt0.m_fX > cPt1.m_fX)
			{
				if (fMinX < cPt0.m_fX)
				{
					nCrossState = 1;
					cCrossPt = cPt0;
					nCrossIdx = i;
					fMinX = cCrossPt.m_fX;
				}
			}
			else
			{
				if (fMinX < cPt1.m_fX)
				{
					nCrossState = 1;
					cCrossPt = cPt1;
					nCrossIdx = i+1;
					fMinX = cCrossPt.m_fX;
				}
			}
			continue;
		}
		else if (IS_ZERO(fRefY - cPt0.m_fY))
		{
			if (cPt0.m_fX > fMinX)
			{
				nCrossState = 1;
				cCrossPt = cPt0;
				nCrossIdx = i;
				fMinX = cCrossPt.m_fX;
			}
			continue;
		}
		else if (IS_ZERO(fRefY - cPt1.m_fY))
		{
			if (cPt1.m_fX > fMinX)
			{
				nCrossState = 1;
				cCrossPt = cPt1;
				nCrossIdx = i+1;
				fMinX = cCrossPt.m_fX;
			}
			continue;
		}
		else
		{
			CAD_FLOAT	fY0 = cPt0.m_fY - fRefY;
			CAD_FLOAT	fY1 = cPt1.m_fY - fRefY;
			CAD_FLOAT	fX0 = cPt0.m_fX;
			CAD_FLOAT	fX1 = cPt1.m_fX;
			CAD_FLOAT	fRatio;
			CAD_FLOAT	fCrossX;
			if (fY0 < 0)
			{
				fRatio = fY1 / (fY1 - fY0);
				fCrossX = fX1 - (fX1 - fX0) * fRatio;
			}
			else
			{
				fRatio = fY0 / (fY0 - fY1);
				fCrossX = fX0 - (fX0 - fX1) * fRatio;
			}
			if (fCrossX > fMinX && fCrossX < fRefX)
			{
				nCrossState = 2;
				cCrossPt.m_fX = fCrossX;
				cCrossPt.m_fY = fRefY;
				fMinX = fCrossX;
				nCrossIdx = i+1;
			}
		}
	}


	int	nNum = 0;
	CCAD_Point* cpRetPoint = new CCAD_Point [nCount + sContour.nCount + nCrossState+2];
	if (cpRetPoint == NULL)
		return NULL;

	// Add Outerpoly Point
	{

		if (nCrossState == 2)
			cpRetPoint[nNum++] = cCrossPt;
		for (i = 0; i < nCount; i++)
		{
			cpRetPoint[nNum++] = cpOuterPoly[(i+nCrossIdx) % nCount];
		}
		if (nCrossState == 2)
			cpRetPoint[nNum++] = cCrossPt;
		else if (nCrossState == 1)
			cpRetPoint[nNum++] = cpOuterPoly[nCrossIdx];
	}
	// Append InnerPoly point
	{
		CCAD_Point* cpContourPt = sContour.cpPoint;
		int			nMinIdx = sContour.nMinIdx;
		int			nCount = sContour.nCount;
		for (i = 0; i <= nCount; i++)
		{
			cpRetPoint[nNum++] = cpContourPt[(i+nMinIdx) % nCount];
		}
	}

	nCount = nNum;
	delete [] cpOuterPoly;

	return cpRetPoint;
}

void CCAD_RgnProc::MakeOneContour()
{
	int				nSize = m_cContours.GetSize();
	CCAD_Point*		cpOuterPoly = NULL;

	int	nCount = m_cContours[0].nCount;
	try {
		cpOuterPoly = new CCAD_Point [m_cContours[0].nCount];
	}
	catch(...)
	{
		return;
	}
	memcpy(cpOuterPoly, m_cContours[0].cpPoint, sizeof(CCAD_Point) * m_cContours[0].nCount);

	int	i;
	for (i = 1; i < nSize; i++)
	{
		cpOuterPoly = MergeTwoContour(cpOuterPoly, nCount, m_cContours[i]);
	}

	m_cMeasPoints.RemoveAll();
	for (i = 0; i < nCount; i++)
	{
		m_cMeasPoints.Add(cpOuterPoly[i]);
	}

	delete [] cpOuterPoly;
}

void CCAD_RgnProc::MakeWholeTri()
{
	m_cMeasPoints.RemoveAll();
	m_cTriangles.RemoveAll();

	MakeOneContour();

	int	nVtxCnt = m_cMeasPoints.GetSize();
	if (nVtxCnt < 3)
		return;
	
	short*	npVtxIndex = NULL;
	try {
		npVtxIndex = new short[nVtxCnt];
	}
	catch(...)
	{
		return;
	}

	int	i;
	for (i = 0; i < nVtxCnt; ++i)
	{
		npVtxIndex[i] = i;
	}

	short	nRoopNum = 0;
	for (i = 0; ; )
	{
		if (nVtxCnt == 3)
		{
			SRGN_TRIANGLE	sTri;
			sTri.nNum[0] = npVtxIndex[0];
			sTri.nNum[1] = npVtxIndex[1];
			sTri.nNum[2] = npVtxIndex[2];
			m_cTriangles.Add(sTri);
			break;
		}

		BOOL	blDecrease = FALSE;
		short	nStart = (i - 1 + nVtxCnt) % nVtxCnt;
		short	nMid = i % nVtxCnt;
		short	nEnd = (i + 1) % nVtxCnt;

		// Check ValidTriangle
		if (!IsValidTriangle(nStart, nMid, nEnd, npVtxIndex))
		{
			memmove(npVtxIndex+nMid, npVtxIndex+nMid+1, sizeof(short) * (nVtxCnt - nMid - 1));
			nVtxCnt--;
			blDecrease = TRUE;
			continue;
		}
		if (!IsIntersectPoint(nStart, nEnd, npVtxIndex, nVtxCnt))
		{
			CCAD_Point cCenterPt;
			// Debug
			CCAD_Point	cStart = m_cMeasPoints[npVtxIndex[nStart]];
			CCAD_Point	cEnd = m_cMeasPoints[npVtxIndex[nEnd]];
			CCAD_Point	cMid = m_cMeasPoints[npVtxIndex[nMid]];

			cCenterPt.m_fX = (cStart.m_fX + cEnd.m_fX) / 2.0f;
			cCenterPt.m_fY = (cStart.m_fY + cEnd.m_fY) / 2.0f;
			cCenterPt.m_fZ = (cStart.m_fZ + cEnd.m_fZ) / 2.0f;
			if (IsInPolygon(cCenterPt, NULL, npVtxIndex, nVtxCnt))
			{
				cCenterPt.m_fX = (cStart.m_fX + cEnd.m_fX + cMid.m_fX) / 3.0f;
				cCenterPt.m_fY = (cStart.m_fY + cEnd.m_fY + cMid.m_fY) / 3.0f;
				cCenterPt.m_fZ = (cStart.m_fZ + cEnd.m_fZ + cMid.m_fZ) / 3.0f;
				if (IsInPolygon(cCenterPt, NULL, npVtxIndex, nVtxCnt))
				{
					if (!IsInPolygon(nStart, nMid, nEnd, npVtxIndex, nVtxCnt))
					{
						SRGN_TRIANGLE	sTri;
						sTri.nNum[0] = npVtxIndex[nStart];
						sTri.nNum[1] = npVtxIndex[nMid];
						sTri.nNum[2] = npVtxIndex[nEnd];
						m_cTriangles.Add(sTri);
						memmove(npVtxIndex+nMid, npVtxIndex+nMid+1, sizeof(short) * (nVtxCnt - nMid - 1));
						nVtxCnt--;
						blDecrease = TRUE;
					}
				}
			}
		}
		if (blDecrease)
		{
			nRoopNum = 0;
		}
		else
		{
			nRoopNum++;
		}
		if (nRoopNum > 2*nVtxCnt)
			break;
		i = (i + 1) % nVtxCnt;
	}
	delete [] npVtxIndex;

	MakeCounterClockWise();
}

BOOL CCAD_RgnProc::IsValidTriangle(short nStart, short nMid, short nEnd, short* npVtxIndex)
{
	CCAD_Point cPt1, cPt2, cPt3;

	cPt1 = m_cMeasPoints[npVtxIndex[nStart]];
	cPt2 = m_cMeasPoints[npVtxIndex[nMid]];
	cPt3 = m_cMeasPoints[npVtxIndex[nEnd]];

	CCAD_Vector	cVec1(cPt2, cPt1);
	cVec1.Normalize();
	CCAD_Vector	cVec2(cPt2, cPt3);
	cVec2.Normalize();
	CAD_FLOAT fScalar = cVec1 & cVec2;
	
	if ((1 - fabs(fScalar)) < CAD_ZERO)
		return FALSE;
	return TRUE;
}

BOOL CCAD_RgnProc::IsIntersectPoint(short nStart, short nEnd, short *npVtxIndex, short nVtxNum)
{
	short	i;
	for (i = 0; i < nVtxNum; ++i)
	{
		short curStartNum = i;
		short curEndNum = (i + 1) % nVtxNum;
		if (((curStartNum != nStart) && (curStartNum == nEnd)) || ((curEndNum != nStart) || (curEndNum != nEnd)))
		{
			if (IsIntersectPoint(m_cMeasPoints[npVtxIndex[nStart]], m_cMeasPoints[npVtxIndex[nEnd]],
				m_cMeasPoints[npVtxIndex[curStartNum]], m_cMeasPoints[npVtxIndex[curEndNum]]))
			{
				break;
			}
		}
	}
	return (i < nVtxNum);
}

BOOL CCAD_RgnProc::IsInPolygon(short nTriNum1, short nTriNum2, short nTriNum3,
				 short *npVtxIndex, short nVtxCnt)
{
	CCAD_Point cpTriPnt[3];
	cpTriPnt[0] = m_cMeasPoints[npVtxIndex[nTriNum1]];
	cpTriPnt[1] = m_cMeasPoints[npVtxIndex[nTriNum2]];
	cpTriPnt[2] = m_cMeasPoints[npVtxIndex[nTriNum3]];

	short	npTriIndex[3] = {0, 1, 2};
	short	i;

	for (i = 0; i < nVtxCnt; ++i)
	{
		CCAD_Point	cPt = m_cMeasPoints[npVtxIndex[i]];
		if (!cPt.Equal(cpTriPnt[0]) && !cPt.Equal(cpTriPnt[1]) && !cPt.Equal(cpTriPnt[2]))
		{
			if (IsInPolygon(m_cMeasPoints[npVtxIndex[i]], cpTriPnt, npTriIndex, 3))
				break;
		}
	}

	return (i < nVtxCnt);
}

BOOL CCAD_RgnProc::IsInPolygon(CCAD_Point centerpnt)
{
	CAD_FLOAT	fSumAng = 0.0f;
	short	nVtxCnt = m_cMeasPoints.GetSize();
	short	i;

	for (i = 0; i < nVtxCnt; ++i)
	{
		CAD_FLOAT fLineAng;
		fLineAng = GetAngle(centerpnt.m_fX, centerpnt.m_fY,
							 m_cMeasPoints[i].m_fX,
							 m_cMeasPoints[i].m_fY,
							 m_cMeasPoints[(i + 1) % nVtxCnt].m_fX, 
							 m_cMeasPoints[(i + 1) % nVtxCnt].m_fY);
		fSumAng += fLineAng;
	}

	return (fabs(fSumAng) > PAI);
}

#define		DPAI	PAI * 2.0F

BOOL CCAD_RgnProc::IsInPolygon(CCAD_Point centerpnt, CCAD_Point* cpPts, short* npVtxIndex, short nVtxCnt)
{
	CAD_FLOAT	fSumAng = 0.0f;
	short		i;
	if (cpPts != NULL)
	{
		for (i = 0; i < nVtxCnt; ++i)
		{
			CAD_FLOAT fLineAng;
			fLineAng = GetAngle(centerpnt.m_fX, centerpnt.m_fY,
							 cpPts[npVtxIndex[i]].m_fX,
							 cpPts[npVtxIndex[i]].m_fY,
							 cpPts[npVtxIndex[(i + 1) % nVtxCnt]].m_fX, 
							 cpPts[npVtxIndex[(i + 1) % nVtxCnt]].m_fY);
			fSumAng += fLineAng;
		}
	}
	else
	{
		for (i = 0; i < nVtxCnt; ++i)
		{
			CAD_FLOAT fLineAng;
			fLineAng = GetAngle(centerpnt.m_fX, centerpnt.m_fY,
							 m_cMeasPoints[npVtxIndex[i]].m_fX,
							 m_cMeasPoints[npVtxIndex[i]].m_fY,
							 m_cMeasPoints[npVtxIndex[(i + 1) % nVtxCnt]].m_fX, 
							 m_cMeasPoints[npVtxIndex[(i + 1) % nVtxCnt]].m_fY);
			fSumAng += fLineAng;
		}
	}
	return (fabs(fSumAng) > PAI);
}

CAD_FLOAT CCAD_RgnProc::GetAngle(CAD_FLOAT centerpntx, CAD_FLOAT centerpnty,
					CAD_FLOAT startpntx, CAD_FLOAT startpnty,
					CAD_FLOAT endpntx, CAD_FLOAT endpnty)
{
	CAD_FLOAT startang = (CAD_FLOAT)atan2(startpnty - centerpnty, startpntx - centerpntx);
	if (startang < 0.0f)
		startang += DPAI;

	CAD_FLOAT endang = (CAD_FLOAT)atan2(endpnty - centerpnty, endpntx - centerpntx);
	if (endang < 0.0f)
		endang += DPAI;

	CAD_FLOAT resultang = endang - startang;

	if (resultang > PAI)
		resultang -= DPAI;
	if (resultang < -PAI)
		resultang += DPAI;
	//if (resultang == 0.0F)
	//	resultang = PAI;

	return resultang;
}

BOOL CCAD_RgnProc::IsIntersectPoint(CCAD_Point cSPos1, CCAD_Point cEPos1, CCAD_Point cSPos2, CCAD_Point cEPos2)
{
	if (cSPos1.Equal(cSPos2))
		return false;
	if (cSPos1.Equal(cEPos2))
		return false;
	if (cEPos1.Equal(cSPos2))
		return false;
	if (cEPos1.Equal(cEPos2))
		return false;

	BOOL bIsIntersect = FALSE;
	CAD_FLOAT x1, y1, x2, y2, x3, y3, x4, y4;
	x1 = cSPos1.m_fX;	y1 = cSPos1.m_fY;
	x2 = cEPos1.m_fX;	y2 = cEPos1.m_fY;
	x3 = cSPos2.m_fX;	y3 = cSPos2.m_fY;
	x4 = cEPos2.m_fX;	y4 = cEPos2.m_fY;

	CAD_FLOAT den = (y1 - y2) * (x3 - x4) - (x1 - x2) * (y3 - y4);
	if (!IS_ZERO(den))
	{
		CAD_FLOAT intx = ((x1 - x2) * (x3 * y4 - x4 * y3) -
				(x3 - x4) * (x1 * y2 - x2 * y1)) / den;
		CAD_FLOAT inty = ((y1 - y2) * (x3 * y4 - x4 * y3) -
				(y3 - y4) * (x1 * y2 - x2 * y1)) / den;
		if (fabs(x1 - x2) > fabs(y1 - y2))
		{
			bIsIntersect = (_min(x1, x2) + CAD_ZERO < intx) && (intx < _max(x1, x2) - CAD_ZERO);
		}
		else
		{
			bIsIntersect = (_min(y1, y2) + CAD_ZERO < inty) && (inty < _max(y1, y2) - CAD_ZERO);
		}
		if (bIsIntersect)
		{
			if(fabs(x3 - x4) > fabs(y3 - y4))
			{
				bIsIntersect = (_min(x3, x4) + CAD_ZERO < intx) && (intx < _max(x3, x4) - CAD_ZERO);
			}
			else
			{
				bIsIntersect = (_min(y3, y4) + CAD_ZERO < inty) && (inty < _max(y3, y4) - CAD_ZERO);
			}
		}
	}
	else
	{
		cSPos1.m_fX = x1; cSPos1.m_fY = y1;
		cEPos1.m_fX = x2; cEPos1.m_fY = y2;
		cSPos2.m_fX = x3; cSPos2.m_fY = y3;

		CCAD_Point newpnt3;
		GetVerticalPnt(cSPos1, cEPos1, cSPos2, newpnt3);

		CAD_FLOAT len = (CAD_FLOAT)sqrt((cSPos2.m_fX - newpnt3.m_fX) * (cSPos2.m_fX - newpnt3.m_fX) +
						(cSPos2.m_fY - newpnt3.m_fY) * (cSPos2.m_fY - newpnt3.m_fY));
		if (IS_ZERO(len))
		{
			CAD_FLOAT maxval, minval;
			if (fabs(x1 - x2) > fabs(y1 - y2))
			{
				maxval = _min(_max(x1, x2), _max(x3, x4));
				minval = _max(_min(x1, x2), _min(x3, x4));
				if(maxval > minval + CAD_ZERO)
				{
					bIsIntersect = TRUE;
				}
			}
			else
			{
				maxval = _min(_max(y1, y2), _max(y3, y4));
				minval = _max(_min(y1, y2), _min(y3, y4));
				if(maxval > minval + CAD_ZERO)
				{
					bIsIntersect = TRUE;
				}
			}
		}
	}
	return bIsIntersect;
}

void CCAD_RgnProc::GetVerticalPnt(CCAD_Point pnt1, CCAD_Point pnt2, 
							   CCAD_Point pnt3, CCAD_Point &result)
{
	CAD_FLOAT dx12 = pnt1.m_fX - pnt2.m_fX;
	CAD_FLOAT dy12 = pnt1.m_fY - pnt2.m_fY;
	CAD_FLOAT dy31 = pnt3.m_fY - pnt1.m_fY;
	CAD_FLOAT den = dx12 * dx12 + dy12 * dy12;

	if ( fabs(den) > CAD_ZERO )
	{
		result.m_fX = (dx12 * dx12 * pnt3.m_fX + dy12 * dy12 * pnt1.m_fX +
			dy12 * dy31 * dx12) / den;
		if (fabs(dx12) > 0.001f)
		{
			result.m_fY = pnt1.m_fY - dy12 * (pnt1.m_fX - result.m_fX) / dx12;
		}
		else
		{
			result.m_fY = pnt3.m_fY;
		}
	}
	else
	{
		result = pnt3;
	}
}

void CCAD_RgnProc::MakeCounterClockWise()
{
	short nSize = m_cTriangles.GetSize();

	int	i;
	for (i = 0; i < nSize; i++)
	{
		CounterClockWise(m_cTriangles[i]);
	}
}

void CCAD_RgnProc::CounterClockWise(SRGN_TRIANGLE& sTri)
{
	short	n0 = sTri.nNum[0];
	short	n1 = sTri.nNum[1];
	short	n2 = sTri.nNum[2];
	short	n;
	SCAD_POINT	sPt0 = m_cMeasPoints[n0];
	SCAD_POINT	sPt1 = m_cMeasPoints[n1];
	SCAD_POINT	sPt2 = m_cMeasPoints[n2];
	CAD_FLOAT	fVal;
	
	fVal = (sPt0.m_fX - sPt1.m_fX) * (sPt2.m_fY - sPt1.m_fY) -
	      (sPt2.m_fX - sPt1.m_fX) * (sPt0.m_fY - sPt1.m_fY);
	if (fVal)
	{
		n = sTri.nNum[1];
		sTri.nNum[1] = sTri.nNum[2];
		sTri.nNum[2] = n;
	}
}


