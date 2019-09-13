#include "stdafx.h"
#include "CAD_Linear2D.h"
#include "CAD_GemLine.h"

CCAD_GemLine::CCAD_GemLine()
{
}

CCAD_GemLine::~CCAD_GemLine()
{
}

CCAD_GemLine::CCAD_GemLine(const CCAD_Coord3D & cBcs, const CCAD_Vector &cSp, const CCAD_Vector &cEp)
{
	m_cBcs = cBcs;
	m_cSp = cSp;
	m_cEp = cEp;
}

//-----------------------------------------------------
CCAD_GemLine::CCAD_GemLine(const CCAD_GemLine & cLine)
{
	*this = cLine;
}

CCAD_GemLine::CCAD_GemLine(const CCAD_Vector & cSp, const CCAD_Vector & cEp)
{
	m_cBcs.m_cOrg = cSp;
	m_cEp = cEp;
	m_cBcs.ConvFromBase(m_cEp);
}

//-----------------------------------------------------
CCAD_GemLine & CCAD_GemLine::operator=(const CCAD_GemLine & cLine)
{
	CCAD_GemThickEntity::operator =(cLine);
	m_cSp = cLine.m_cSp;
	m_cEp = cLine.m_cEp;
	return *this;
}

void CCAD_GemLine::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_GemThickEntity::MemCopyTo(pGemEntity, pData1, pData2, pData3);
	*(CCAD_GemLine *)pGemEntity  = *this;
}

BOOL CCAD_GemLine::Read(CCAD_File& cFile)
{
	CCAD_GemThickEntity::Read(cFile);

	m_cSp.Read(cFile);
	m_cEp.Read(cFile);
	return TRUE;
}

//---------------------------------------------------
BOOL CCAD_GemLine::Draw(CCAD_DC *pCadDC, BOOL bRedraw)
{
	pCadDC->MoveTo(m_cSp);
	pCadDC->LineTo(m_cEp);
	return TRUE;
}

void CCAD_GemLine::CountBodyRange(CCAD_Rect& cRange)
{
	CCAD_GemThickEntity::CountBodyRange(cRange);

	cRange.ResetParameter(m_cSp);
	cRange.ResetParameter(m_cEp);
}

void CCAD_GemLine::CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange)
{
	ASSERT(pCadDC != NULL);
	
	pCadDC->CountVcsRange(m_cSp, cRange);
	pCadDC->CountVcsRange(m_cEp, cRange);
}

//----------------------------------------------------------
WORD CCAD_GemLine::GetCurvePoint(SCAD_POINT *spDivPnt)
{
	if (spDivPnt != NULL)
	{
		spDivPnt[0] = m_cSp;
		spDivPnt[1] = m_cEp;
		m_cBcs.ConvToBase(spDivPnt[0]);
		m_cBcs.ConvToBase(spDivPnt[1]);
	}
	return 2;
}

//-----------------------------------------------------
CAD_FLOAT CCAD_GemLine::GetLength() const
{
	return m_cSp.CountLength(m_cEp);
};

//-----------------------------------------------------
CCAD_Angle CCAD_GemLine::GetAngle() const
{
	CCAD_Angle	cAngle;
	m_cSp.CountAngle2D(m_cEp, cAngle);
	return cAngle;
}

CAD_FLOAT CCAD_GemLine::CountTriArea(const CCAD_Vector & cCp) const
{
	CAD_FLOAT	d1 = (m_cSp.m_fX-cCp.m_fX)*(m_cEp.m_fY-cCp.m_fY);
	CAD_FLOAT	d2 = (m_cEp.m_fX-cCp.m_fX)*(m_cSp.m_fY-cCp.m_fY);
	d1 = (d1 - d2) / 2;
	return d1;
}

CAD_FLOAT CCAD_GemLine::CountDistanceToPoint(const CCAD_Vector & cPt) const
{
	CAD_FLOAT	fArea = (CAD_FLOAT)fabs(CountTriArea(cPt));
	if (fArea == 0)
		return 0;
	CAD_FLOAT	fL = GetLength();
	return (fArea * 2 / fL);
}

BYTE CCAD_GemLine::PointBySide(const CCAD_Vector & cPt) const
{
	CAD_FLOAT	d1 = cPt.CountLength(m_cSp);
	CAD_FLOAT	d2 = cPt.CountLength(m_cEp);
	if (d1 < d2)
		return INTERVAL_START;
	return INTERVAL_END;
}

BYTE CCAD_GemLine::PointOnInterval(const CCAD_Vector & cPt) const
{
	if (fabs(m_cEp.m_fX - m_cSp.m_fX) > fabs(m_cEp.m_fY - m_cSp.m_fY)) {
		if (m_cSp.m_fX < m_cEp.m_fX) {
			if (cPt.m_fX <= m_cSp.m_fX)
				return INTERVAL_START;
			else if (cPt.m_fX >= m_cEp.m_fX)
				return INTERVAL_END;
		}
		else {
			if (cPt.m_fX <= m_cEp.m_fX)
				return INTERVAL_END;
			else if (cPt.m_fX >= m_cSp.m_fX)
				return INTERVAL_START;
		}
	}
	else {
		if (m_cSp.m_fY < m_cEp.m_fY) {
			if (cPt.m_fY <= m_cSp.m_fY)
				return INTERVAL_START;
			else if (cPt.m_fY >= m_cEp.m_fY)
				return INTERVAL_END;
		}
		else {
			if (cPt.m_fY <= m_cEp.m_fY)
				return INTERVAL_END;
			else if (cPt.m_fY >= m_cSp.m_fY)
				return INTERVAL_START;
		}
	}
	return INTERVAL_IN;
}

BOOL CCAD_GemLine::IsPtInterval(const CCAD_Vector & cPt, CAD_FLOAT fAccy) const
{
	//if (fabs(m_cEp.m_fX - m_cSp.m_fX) > fabs(m_cEp.m_fY - m_cSp.m_fY))
	{
		if (m_cSp.m_fX < m_cEp.m_fX) {
			if (cPt.m_fX <= (m_cSp.m_fX-fAccy))
				return FALSE;
			else if (cPt.m_fX >= (m_cEp.m_fX+fAccy))
				return FALSE;
		}
		else {
			if (cPt.m_fX <= (m_cEp.m_fX-fAccy))
				return FALSE;
			else if (cPt.m_fX >= (m_cSp.m_fX+fAccy))
				return FALSE;
		}
	}
	//else
	{
		if (m_cSp.m_fY < m_cEp.m_fY) {
			if (cPt.m_fY <= (m_cSp.m_fY-fAccy))
				return FALSE;
			else if (cPt.m_fY >= (m_cEp.m_fY+fAccy))
				return FALSE;
		}
		else {
			if (cPt.m_fY <= (m_cEp.m_fY-fAccy))
				return FALSE;
			else if (cPt.m_fY >= (m_cSp.m_fY+fAccy))
				return FALSE;
		}
	}
	return TRUE;
}

BOOL CCAD_GemLine::IsCrossHorizontal(const CAD_FLOAT & fHrztY, CAD_FLOAT & fGetX) const
{
	return m_cSp.IsCrossHorizontal(m_cEp, fHrztY, fGetX);
}

BOOL CCAD_GemLine::IsCrossVertical(const CAD_FLOAT & fVrtX, CAD_FLOAT & fGetY) const
{
	return m_cSp.IsCrossVertical(m_cEp, fVrtX, fGetY);
}

BYTE CCAD_GemLine::CrossTwoLine(const CCAD_GemLine & cLine) const
{
	if (m_cSp.m_fX == m_cEp.m_fX && m_cSp.m_fY == m_cEp.m_fY)
		return LINES_NOCROSS;
	if (cLine.m_cSp.m_fX == cLine.m_cEp.m_fX && cLine.m_cSp.m_fY == cLine.m_cEp.m_fY)
		return LINES_NOCROSS;
	
	CCAD_Point	cPt;
	cPt = cLine.m_cSp;
	cLine.m_cBcs.ConvToBase(cPt);
	m_cBcs.ConvFromBase(cPt);
	CAD_FLOAT	d1 = CountTriArea(cPt);

	cPt = cLine.m_cEp;
	cLine.m_cBcs.ConvToBase(cPt);
	m_cBcs.ConvFromBase(cPt);
	CAD_FLOAT	d2 = CountTriArea(cPt);

	cPt = m_cSp;
	m_cBcs.ConvToBase(cPt);
	cLine.m_cBcs.ConvFromBase(cPt);
	CAD_FLOAT	d3 = cLine.CountTriArea(cPt);

	cPt = m_cEp;
	m_cBcs.ConvToBase(cPt);
	cLine.m_cBcs.ConvFromBase(cPt);
	CAD_FLOAT	d4 = cLine.CountTriArea(cPt);

	if ((d1 == 0 && d2 == 0) || (d3 == 0 && d4 == 0))
		return TWO_ONLINE;
	else if (d1 == 0 || d3 == 0)
		return FIRST_ONLINE;
	else if (d2 == 0 || d4 == 0)
		return SECOND_ONLINE;
	if ((d1<0 && d2<0) || (d1>0 && d2>0))
		return LINES_NOCROSS;
	if ((d3<0 && d4<0) || (d3>0 && d4>0))
		return LINES_NOCROSS;
	return LINES_CROSS;
}

BYTE CCAD_GemLine::CrossTwoLine(const CCAD_Angle &inAngle, const CCAD_Vector & cPt) const
{
	if (m_cSp.m_fX == m_cEp.m_fX && m_cSp.m_fY == m_cEp.m_fY)
		return LINES_NOCROSS;
	BYTE	bFirst = m_cSp.CountSide2D(cPt, inAngle);
	BYTE	bSecond = m_cEp.CountSide2D(cPt, inAngle);
	if (bFirst == NO_DIRECT && bSecond == NO_DIRECT)
		return TWO_ONLINE;
	else if (bFirst == NO_DIRECT)
		return FIRST_ONLINE;
	else if (bSecond == NO_DIRECT)
		return SECOND_ONLINE;
	if (bFirst == bSecond)
		return LINES_NOCROSS;
	return LINES_CROSS;
}

BYTE CCAD_GemLine::PtDirect(const CCAD_Vector & cPt) const
{
	CAD_FLOAT	sum = (cPt.m_fY-m_cSp.m_fY)*(m_cEp.m_fX-m_cSp.m_fX) - 
						(cPt.m_fX-m_cSp.m_fX)*(m_cEp.m_fY-m_cSp.m_fY);
	if (sum == 0)
		return NO_DIRECT;
	else if (sum > 0)
		return NOCLOCK_DIRECT;
	return CLOCK_DIRECT;
}

BOOL CCAD_GemLine::CountSlope(const CCAD_Vector cPt, const CCAD_Angle &cValue, const CCAD_Angle & cSlope, BYTE bFlag)
{
	CCAD_Angle	cDir;
	if (bFlag == 0x01) {	//Circle
		cPt.CountAngle2D(m_cSp, cDir);
		cDir.RotateAngle(NINETY);
		cDir.RotateAngle(cSlope);
	}
	else { // Ray
		cDir = cValue;
		cDir.RotateAngle(cSlope);
	}
	CCAD_Linear2D cLinear1(m_cSp, cDir);
	cDir.RotateAngle(NINETY);
	CCAD_Linear2D cLinear2(m_cEp, cDir);
	return cLinear1.CountCrossPt(m_cEp, cLinear2);
}

BOOL CCAD_GemLine::CountCrossPt(CCAD_Linear2D &cLine, CCAD_Vector &ptCross) const
{
	CCAD_Vector	cSp = m_cSp;
	CCAD_Vector	cEp = m_cEp;
	m_cBcs.ConvToBase(cSp);
	m_cBcs.ConvToBase(cEp);

	CCAD_Linear2D	cThis(cSp, cEp);
	if (cThis.CountCrossPt(ptCross, cLine)) {
		m_cBcs.ConvFromBase(ptCross);
		if (IsPtInterval(ptCross)) {
			m_cBcs.ConvToBase(ptCross);
			return TRUE;
		}
	}
	return FALSE;
}

BYTE CCAD_GemLine::CountCrossPtWithLine(const CCAD_GemLine & cLine, CCAD_Vector & cVp) const
{
	CCAD_Point	cSp = cLine.m_cSp;
	CCAD_Point	cEp = cLine.m_cEp;

	cLine.m_cBcs.ConvToBase(cSp);
	cLine.m_cBcs.ConvToBase(cEp);
	m_cBcs.ConvFromBase(cSp);
	m_cBcs.ConvFromBase(cEp);

	if (cSp == m_cSp || cSp == m_cEp) {
		if (cEp == m_cEp || cEp == m_cSp)
			return ROOT_NO;
		cVp = cSp;
		m_cBcs.ConvToBase(cVp);
		return ROOT_ONE;
	}
	if (cEp == m_cSp || cEp == m_cEp) {
		if (cSp == m_cEp || cSp == m_cSp)
			return ROOT_NO;
		cVp = cEp;
		m_cBcs.ConvToBase(cVp);
		return ROOT_ONE;
	}

	if (m_cSp.m_fX < cSp.m_fX && m_cEp.m_fX < cSp.m_fX && 
					m_cSp.m_fX < cEp.m_fX && m_cEp.m_fX < cEp.m_fX)
		return ROOT_NO;
	if (m_cSp.m_fX > cSp.m_fX && m_cEp.m_fX > cSp.m_fX && 
					m_cSp.m_fX > cEp.m_fX && m_cEp.m_fX > cEp.m_fX)
		return ROOT_NO;
	if (m_cSp.m_fY < cSp.m_fY && m_cEp.m_fY < cSp.m_fY && 
					m_cSp.m_fY < cEp.m_fY && m_cEp.m_fY < cEp.m_fY)
		return ROOT_NO;
	if (m_cSp.m_fY > cSp.m_fY && m_cEp.m_fY > cSp.m_fY && 
					m_cSp.m_fY > cEp.m_fY && m_cEp.m_fY > cEp.m_fY)
		return ROOT_NO;

	CCAD_Linear2D	cLinear1(m_cSp, m_cEp);
	CCAD_Linear2D	cLinear2(cSp, cEp);
	if (cLinear1.CountCrossPt(cVp, cLinear2)) {
		if (IsPtInterval(cVp)) {
			m_cBcs.ConvToBase(cVp);
			cLine.m_cBcs.ConvFromBase(cVp);
			if (cLine.IsPtInterval(cVp)) {
				cLine.m_cBcs.ConvToBase(cVp);
				return ROOT_ONE;
			}
		}
	}
	return ROOT_NO;
} 
