#include "stdafx.h"
//#include "math.h"
#include "CAD_Constance.h"
#include "CAD_Rect.h"

 

CCAD_Rect::CCAD_Rect()
{
	m_fMinX = m_fMinY = m_fMinZ = MAX_DOUBLE;
	m_fMaxX = m_fMaxY = m_fMaxZ = MIN_DOUBLE;
}

CCAD_Rect::CCAD_Rect(const CCAD_Rect & rc)
{
	m_fMinX = rc.m_fMinX;
	m_fMinY = rc.m_fMinY;
	m_fMinZ = rc.m_fMinZ;
	m_fMaxX = rc.m_fMaxX;
	m_fMaxY = rc.m_fMaxY;
	m_fMaxZ = rc.m_fMaxZ;
}

CCAD_Rect::CCAD_Rect(const CCAD_Point & cPt, const CAD_FLOAT & dR)
{
	SetParameter(cPt, dR);
}

CCAD_Rect::CCAD_Rect(const CCAD_Point & cPt1, const CCAD_Point & cPt2)
{
	SetParameter(cPt1, cPt2);
}

CCAD_Rect::CCAD_Rect(LONG left, LONG top, LONG right, LONG bottom)
{
	CCAD_Point cPt1;
	cPt1.m_fX = left;
	cPt1.m_fY = top;
	CCAD_Point cPt2;
	cPt2.m_fX = right;
	cPt2.m_fY = bottom;
	SetParameter(cPt1, cPt2);
}

CCAD_Rect & CCAD_Rect::operator=(const CCAD_Rect & rc)
{
	m_fMinX = rc.m_fMinX;
	m_fMinY = rc.m_fMinY;
	m_fMinZ = rc.m_fMinZ;
	m_fMaxX = rc.m_fMaxX;
	m_fMaxY = rc.m_fMaxY;
	m_fMaxZ = rc.m_fMaxZ;
	return *this;
}

void CCAD_Rect::Init()
{
	m_fMinX = m_fMinY = m_fMinZ = MAX_DOUBLE;
	m_fMaxX = m_fMaxY = m_fMaxZ = MIN_DOUBLE;
}

BOOL CCAD_Rect::IsInited()
{
	if ( m_fMinX == MAX_DOUBLE || m_fMinY == MAX_DOUBLE || m_fMinZ == MAX_DOUBLE )
		return TRUE;
	if ( m_fMaxX == MIN_DOUBLE || m_fMaxY == MIN_DOUBLE || m_fMaxZ == MIN_DOUBLE )
		return TRUE;
	return FALSE;
}

void CCAD_Rect::InflateRect(CAD_FLOAT fDeltaX, CAD_FLOAT fDeltaY, CAD_FLOAT fDeltaZ/* =0 */)
{
	m_fMinX -= fDeltaX;
	m_fMaxX += fDeltaX;
	m_fMinY -= fDeltaY;
	m_fMaxY += fDeltaY;
	m_fMinZ -= fDeltaZ;
	m_fMaxZ += fDeltaZ;
}
//----

void CCAD_Rect::SetParameter(const CCAD_Point & cPt, const CAD_FLOAT & dR)
{
	m_fMinX = cPt.m_fX - dR;
	m_fMaxX = cPt.m_fX + dR;
	m_fMinY = cPt.m_fY - dR;
	m_fMaxY = cPt.m_fY + dR;
	m_fMinZ = cPt.m_fZ - dR;
	m_fMaxZ = cPt.m_fZ + dR;
}

void CCAD_Rect::SetParameter(const CCAD_Point & cPt1, const CCAD_Point & cPt2)
{
	if (cPt1.m_fX < cPt2.m_fX) {
		m_fMinX = cPt1.m_fX;
		m_fMaxX = cPt2.m_fX;
	}
	else {
		m_fMinX = cPt2.m_fX;
		m_fMaxX = cPt1.m_fX;
	}
	if (cPt1.m_fY < cPt2.m_fY) {
		m_fMinY = cPt1.m_fY;
		m_fMaxY = cPt2.m_fY;
	}
	else {
		m_fMinY = cPt2.m_fY;
		m_fMaxY = cPt1.m_fY;
	}
	if (cPt1.m_fZ < cPt2.m_fZ) {
		m_fMinZ = cPt1.m_fZ;
		m_fMaxZ = cPt2.m_fZ;
	}
	else {
		m_fMinZ = cPt2.m_fZ;
		m_fMaxZ = cPt1.m_fZ;
	}
}

void CCAD_Rect::ResetParameter(const CCAD_Point & cPt)
{
	if (m_fMinX > cPt.m_fX)
		m_fMinX = cPt.m_fX;
	if (m_fMaxX < cPt.m_fX)
		m_fMaxX = cPt.m_fX;
	if (m_fMinY > cPt.m_fY)
		m_fMinY = cPt.m_fY;
	if (m_fMaxY < cPt.m_fY)
		m_fMaxY = cPt.m_fY;
	if (m_fMinZ > cPt.m_fZ)
		m_fMinZ = cPt.m_fZ;
	if (m_fMaxZ < cPt.m_fZ)
		m_fMaxZ = cPt.m_fZ;
}

void CCAD_Rect::ResetParameter(float x, float y)
{
	if (m_fMinX > x)
		m_fMinX = x;
	if (m_fMaxX < x)
		m_fMaxX = x;
	if (m_fMinY > y)
		m_fMinY = y;
	if (m_fMaxY < y)
		m_fMaxY = y;
}

void CCAD_Rect::ResetParameter(const CCAD_Rect & rc)
{
	if (m_fMinX > rc.m_fMinX)
		m_fMinX = rc.m_fMinX;
	if (m_fMaxX < rc.m_fMaxX)
		m_fMaxX = rc.m_fMaxX;
	if (m_fMinY > rc.m_fMinY)
		m_fMinY = rc.m_fMinY;
	if (m_fMaxY < rc.m_fMaxY)
		m_fMaxY = rc.m_fMaxY;
	if (m_fMinZ > rc.m_fMinZ)
		m_fMinZ = rc.m_fMinZ;
	if (m_fMaxZ < rc.m_fMaxZ)
		m_fMaxZ = rc.m_fMaxZ;
}

BOOL	CCAD_Rect::IsPointInNear(const CCAD_Point & cPt) const
{
	return (cPt.m_fZ>=m_fMinZ);
}

BOOL	CCAD_Rect::IsLineInNear(const CCAD_Point & cSp, const CCAD_Point & cEp) const
{
	return (cSp.m_fY >= m_fMinY || cEp.m_fY >= m_fMinY);
}

BOOL	CCAD_Rect::ClipLineInNear(CCAD_Point & cPt1, CCAD_Point &cPt2) const
{
	if (IsPointInNear(cPt1)) {
		if (!IsPointInNear(cPt2)) {
			cPt2.m_fX = cPt1.m_fX + (m_fMinZ-cPt1.m_fZ) * (cPt2.m_fX-cPt1.m_fX)/(cPt2.m_fZ-cPt1.m_fZ);
			cPt2.m_fY = cPt1.m_fY + (m_fMinZ-cPt1.m_fZ) * (cPt2.m_fY-cPt1.m_fY)/(cPt2.m_fZ-cPt1.m_fZ);
			cPt2.m_fZ = m_fMinZ;
		}
	}
	else {
		if (!IsPointInNear(cPt2))
			return FALSE;
		cPt1.m_fX = cPt2.m_fX + (m_fMinZ-cPt2.m_fZ) * (cPt1.m_fX-cPt2.m_fX)/(cPt1.m_fZ-cPt2.m_fZ);
		cPt1.m_fY = cPt2.m_fY + (m_fMinZ-cPt2.m_fZ) * (cPt1.m_fY-cPt2.m_fY)/(cPt1.m_fZ-cPt2.m_fZ);
		cPt1.m_fZ = m_fMinZ;
	}
	return TRUE;
}

BOOL	CCAD_Rect::IsPointsInNear(CCAD_Point * cRp, short n) const
{
	short	i;
	for (i=0; i<n; i++) {
		if (cRp[i].m_fX > m_fMinZ)
			break;
	}
	return  (i < n);
}

void CCAD_Rect::ClipRayInNear(BYTE bContinue, CCAD_Point & cSp, CCAD_Point & cEp) const
{
	if (cSp.m_fZ == cEp.m_fZ)
		return;
	
	if (cSp.m_fZ < cEp.m_fZ) {
		if (bContinue == YES && cSp.m_fZ < m_fMinZ) {
			cSp.m_fX = cEp.m_fX + (m_fMinZ-cEp.m_fZ) * (cSp.m_fX-cEp.m_fX)/(cSp.m_fZ-cEp.m_fZ);
			cSp.m_fY = cEp.m_fY + (m_fMinZ-cEp.m_fZ) * (cSp.m_fY-cEp.m_fY)/(cSp.m_fZ-cEp.m_fZ);
			cSp.m_fZ = m_fMinZ;
		}
	}
	else {
		cEp.m_fX = cSp.m_fX + (m_fMinZ-cSp.m_fZ) * (cEp.m_fX-cSp.m_fX)/(cEp.m_fZ-cSp.m_fZ);
		cEp.m_fY = cSp.m_fY + (m_fMinZ-cSp.m_fZ) * (cEp.m_fY-cSp.m_fY)/(cEp.m_fZ-cSp.m_fZ);
		cEp.m_fZ = m_fMinZ;
	}
}


BOOL CCAD_Rect::IsPointInRect2D( const CCAD_Point & cPt ) const
{
	return (cPt.m_fX>=m_fMinX && \
			cPt.m_fX<=m_fMaxX && \
			cPt.m_fY>=m_fMinY && \
			cPt.m_fY<=m_fMaxY);
}
BOOL CCAD_Rect::IsPointInRect2D_NE( const CCAD_Point & cPt ) const
{
	return (cPt.m_fX>m_fMinX && \
			cPt.m_fX<m_fMaxX && \
			cPt.m_fY>m_fMinY && \
			cPt.m_fY<m_fMaxY);
}

BOOL CCAD_Rect::IsLineInRect2D(const CCAD_Point & cSp, const CCAD_Point & cEp) const
{
	if( !IsPointInRect2D(cSp) )
		return false;
	return IsPointInRect2D(cEp);
}
BOOL CCAD_Rect::IsLineInRect2D_NE(const CCAD_Point & cSp, const CCAD_Point & cEp) const
{
	if( !IsPointInRect2D_NE(cSp) )
		return false;
	return IsPointInRect2D_NE(cEp);
}

BOOL CCAD_Rect::IsEnableLineInRect2D(const CCAD_Point & cSp, const CCAD_Point & cEp) const
{
	if (cSp.m_fX <= m_fMinX && cEp.m_fX <= m_fMinX)
		return FALSE;
	if (cSp.m_fX >= m_fMaxX && cEp.m_fX >= m_fMaxX)
		return FALSE;
	if (cSp.m_fY <= m_fMinY && cEp.m_fY <= m_fMinY)
		return FALSE;
	if (cSp.m_fY >= m_fMaxY && cEp.m_fY >= m_fMaxY)
		return FALSE;
	return TRUE;
}

BOOL CCAD_Rect::IsCrossLine2D(const CCAD_Point &pt1,const CCAD_Point &pt2)
{
	if( !IsEnableLineInRect2D(pt1,pt2) )
		return false;
	
	CAD_FLOAT A = -(pt2.m_fY - pt1.m_fY);
	CAD_FLOAT B = pt2.m_fX - pt1.m_fX;
	CAD_FLOAT C = -A * pt1.m_fX - B * pt1.m_fY;

	//------ 2002.2.26
	if (A == 0.0f && B == 0.0f)		 
		return true;
	//------ 2002.2.26
	
	int nDirect = 0;

	if( (A * m_fMinX + B * m_fMinY + C) > 0.0 )
		nDirect ++;

	if( (A * m_fMinX + B * m_fMaxY + C) > 0.0 )
		nDirect ++;
	
	if( (A * m_fMaxX + B * m_fMinY + C) > 0.0 )
		nDirect ++;
	
	if( (A * m_fMaxX + B * m_fMaxY + C) > 0.0 )
		nDirect ++;

	if( nDirect == 0 || nDirect == 4 ) 
		return false;

	return true;

}	

BOOL CCAD_Rect::ClipLineInRect2D(CCAD_Point & cPt1, CCAD_Point & cPt2) const
{
	float intpos;
	//m_fMinX
	if((cPt1.m_fX > m_fMinX) != (cPt2.m_fX > m_fMinX)){
		intpos = cPt1.m_fY + (cPt2.m_fY - cPt1.m_fY) * (m_fMinX - cPt1.m_fX) / (cPt2.m_fX - cPt1.m_fX);
		if(cPt1.m_fX > m_fMinX){
			cPt2.m_fX = m_fMinX;
			cPt2.m_fY = intpos;
		}else{
			cPt1.m_fX = m_fMinX;
			cPt1.m_fY = intpos;
		}
	}else{
		if(cPt1.m_fX <= m_fMinX) return FALSE;
	}

	//m_fMaxX
	if((cPt1.m_fX < m_fMaxX) != (cPt2.m_fX < m_fMaxX)){
		intpos = cPt1.m_fY + (cPt2.m_fY - cPt1.m_fY) * (m_fMaxX - cPt1.m_fX) / (cPt2.m_fX - cPt1.m_fX);
		if(cPt1.m_fX < m_fMaxX){
			cPt2.m_fX = m_fMaxX;
			cPt2.m_fY = intpos;
		}else{
			cPt1.m_fX = m_fMaxX;
			cPt1.m_fY = intpos;
		}
	}else{
		if(cPt1.m_fX >= m_fMaxX) return FALSE;
	}
	//m_fMinY
	if((cPt1.m_fY > m_fMinY) != (cPt2.m_fY > m_fMinY)){
		intpos = cPt1.m_fX + (cPt2.m_fX - cPt1.m_fX) * (m_fMinY - cPt1.m_fY) / (cPt2.m_fY - cPt1.m_fY);
		if(cPt1.m_fY > m_fMinY){
			cPt2.m_fX = intpos;
			cPt2.m_fY = m_fMinY;
		}else{
			cPt1.m_fX = intpos;
			cPt1.m_fY = m_fMinY;
		}
	}else{
		if(cPt1.m_fY <= m_fMinY) return FALSE;
	}
	//m_fMaxY
	if((cPt1.m_fY < m_fMaxY) != (cPt2.m_fY < m_fMaxY)){
		intpos = cPt1.m_fX + (cPt2.m_fX - cPt1.m_fX) * (m_fMaxY - cPt1.m_fY) / (cPt2.m_fY - cPt1.m_fY);
		if(cPt1.m_fY < m_fMaxY){
			cPt2.m_fX = intpos;
			cPt2.m_fY = m_fMaxY;
		}else{
			cPt1.m_fX = intpos;
			cPt1.m_fY = m_fMaxY;
		}
	}else{
		if(cPt1.m_fY >= m_fMaxY) return FALSE;
	}
	return	TRUE;
}

void CCAD_Rect::CountPointInRect2D(const CCAD_Point & inPt, CCAD_Point & outPt) const
{
	if (inPt.m_fX == outPt.m_fX) {
		if (outPt.m_fY >= m_fMaxY)
			outPt.m_fY = m_fMaxY;
		else if (outPt.m_fY <= m_fMinY)
			outPt.m_fY = m_fMinY;
	}
	else if (inPt.m_fY == outPt.m_fY) {
		if (outPt.m_fX >= m_fMaxX)
			outPt.m_fX = m_fMaxX;
		else if (outPt.m_fX <= m_fMinX)
			outPt.m_fX = m_fMinX;
	}
	if (outPt.m_fX < m_fMinX) {
		outPt.m_fY = inPt.m_fY + (m_fMinX-inPt.m_fX) * (outPt.m_fY-inPt.m_fY)/(outPt.m_fX-inPt.m_fX);
		outPt.m_fX = m_fMinX;
	}
	else if (outPt.m_fX > m_fMaxX) {
		outPt.m_fY = inPt.m_fY + (m_fMaxX-inPt.m_fX) * (outPt.m_fY-inPt.m_fY)/(outPt.m_fX-inPt.m_fX);
		outPt.m_fX = m_fMaxX;
	}
	if (outPt.m_fY < m_fMinY) {
		outPt.m_fX = inPt.m_fX + (m_fMinY-inPt.m_fY) * (outPt.m_fX-inPt.m_fX)/(outPt.m_fY-inPt.m_fY);
		outPt.m_fY = m_fMinY;
	}
	else if (outPt.m_fY > m_fMaxY) {
		outPt.m_fX = inPt.m_fX + (m_fMaxY-inPt.m_fY) * (outPt.m_fX-inPt.m_fX)/(outPt.m_fY-inPt.m_fY);
		outPt.m_fY = m_fMaxY;
	}
}

void CCAD_Rect::ClipRayInRect2D(BYTE bContinue, CCAD_Point & cSp, CCAD_Point & cEp) const
{
	if (cSp.m_fY == cEp.m_fY) {
		if (cSp.m_fX < cEp.m_fX) {
			if (bContinue == YES || cSp.m_fX < m_fMinX)
				cSp.m_fX = m_fMinX;
			cEp.m_fX = m_fMaxX;
		}
		else {
			if (bContinue == YES || cSp.m_fX > m_fMaxX)
				cSp.m_fX = m_fMaxX;
			cEp.m_fX = m_fMinX;
		}
	}
	else if (cSp.m_fX == cEp.m_fX) {
		if (cSp.m_fY < cEp.m_fY) {
			if (bContinue == YES || cSp.m_fY < m_fMinY)
				cSp.m_fY = m_fMinY;
			cEp.m_fY = m_fMaxY;
		}
		else {
			if (bContinue == YES || cSp.m_fY > m_fMaxY)
				cSp.m_fY = m_fMaxY;
			cEp.m_fY = m_fMinY;
		}
	}
	else if (fabs(cEp.m_fX - cSp.m_fX) > fabs(cEp.m_fY - cSp.m_fY)) {
		if (cEp.m_fX < cSp.m_fX) {
			cEp.m_fY = cSp.m_fY + (m_fMinX-cSp.m_fX) * (cEp.m_fY-cSp.m_fY)/(cEp.m_fX-cSp.m_fX);
			cEp.m_fX = m_fMinX;
			if (bContinue == YES || cSp.m_fX > m_fMaxX) {
				cSp.m_fY = cEp.m_fY + (m_fMaxX-cEp.m_fX) * (cSp.m_fY-cEp.m_fY)/(cSp.m_fX-cEp.m_fX);
				cSp.m_fX = m_fMaxX;
			}
		}
		else {
			cEp.m_fY = cSp.m_fY + (m_fMaxX-cSp.m_fX) * (cEp.m_fY-cSp.m_fY)/(cEp.m_fX-cSp.m_fX);
			cEp.m_fX = m_fMaxX;
			if (bContinue == YES || cSp.m_fX < m_fMinX) {
				cSp.m_fY = cEp.m_fY + (m_fMinX-cEp.m_fX) * (cSp.m_fY-cEp.m_fY)/(cSp.m_fX-cEp.m_fX);
				cSp.m_fX = m_fMinX;
			}
		}

		if (cEp.m_fY < m_fMinY && cSp.m_fY > m_fMinY) {
			cEp.m_fX = cSp.m_fX + (m_fMinY-cSp.m_fY) * (cEp.m_fX-cSp.m_fX)/(cEp.m_fY-cSp.m_fY);
			cEp.m_fY = m_fMinY;
		}
		else if (cEp.m_fY > m_fMinY && cSp.m_fY < m_fMinY) {
			cSp.m_fX = cEp.m_fX + (m_fMinY-cEp.m_fY) * (cSp.m_fX-cEp.m_fX)/(cSp.m_fY-cEp.m_fY);
			cSp.m_fY = m_fMinY;
		}
		if (cSp.m_fY < m_fMaxY && cEp.m_fY > m_fMaxY) {
			cEp.m_fX = cSp.m_fX + (m_fMaxY-cSp.m_fY) * (cEp.m_fX-cSp.m_fX)/(cEp.m_fY-cSp.m_fY);
			cEp.m_fY = m_fMaxY;
		}
		else if (cSp.m_fY > m_fMaxY && cEp.m_fY < m_fMaxY) {
			cSp.m_fX = cEp.m_fX + (m_fMaxY-cEp.m_fY) * (cSp.m_fX-cEp.m_fX)/(cSp.m_fY-cEp.m_fY);
			cSp.m_fY = m_fMaxY;
		}
	}
	else {
		if (cEp.m_fY < cSp.m_fY) {
			cEp.m_fX = cSp.m_fX + (m_fMinY-cSp.m_fY) * (cEp.m_fX-cSp.m_fX)/(cEp.m_fY-cSp.m_fY);
			cEp.m_fY = m_fMinY;
			if (bContinue == YES || cSp.m_fY > m_fMaxY) {
				cSp.m_fX = cEp.m_fX + (m_fMaxY-cEp.m_fY) * (cSp.m_fX-cEp.m_fX)/(cSp.m_fY-cEp.m_fY);
				cSp.m_fY = m_fMaxY;
			}
		}
		else {
			cEp.m_fX = cSp.m_fX + (m_fMaxY-cSp.m_fY) * (cEp.m_fX-cSp.m_fX)/(cEp.m_fY-cSp.m_fY);
			cEp.m_fY = m_fMaxY;
			if (bContinue == YES || cSp.m_fY < m_fMinY) {
				cSp.m_fX = cEp.m_fX + (m_fMinY-cEp.m_fY) * (cSp.m_fX-cEp.m_fX)/(cSp.m_fY-cEp.m_fY);
				cSp.m_fY = m_fMinY;
			}
		}

		if (cEp.m_fX < m_fMinX && cSp.m_fX > m_fMinX) {
			cEp.m_fY = cSp.m_fY + (m_fMinX-cSp.m_fX) * (cEp.m_fY-cSp.m_fY)/(cEp.m_fX-cSp.m_fX);
			cEp.m_fX = m_fMinX;
		}
		else if (cEp.m_fX > m_fMinX && cSp.m_fX < m_fMinX) {
			cSp.m_fY = cEp.m_fY + (m_fMinX-cEp.m_fX) * (cSp.m_fY-cEp.m_fY)/(cSp.m_fX-cEp.m_fX);
			cSp.m_fX = m_fMinX;
		}
		if (cSp.m_fX < m_fMaxX && cEp.m_fX > m_fMaxX) {
			cEp.m_fY = cSp.m_fY + (m_fMaxX-cSp.m_fX) * (cEp.m_fY-cSp.m_fY)/(cEp.m_fX-cSp.m_fX);
			cEp.m_fX = m_fMaxX;
		}
		else if (cSp.m_fX > m_fMaxX && cEp.m_fX < m_fMaxX) {
			cSp.m_fY = cEp.m_fY + (m_fMaxX-cEp.m_fX) * (cSp.m_fY-cEp.m_fY)/(cSp.m_fX-cEp.m_fX);
			cSp.m_fX = m_fMaxX;
		}
	}
}

BOOL CCAD_Rect::IsPointsInRect2D(CCAD_Point * cRp, short n) const
{
	short	i;
	for (i=0; i<n; i++) {
		if (cRp[i].m_fX > m_fMinX)
			break;
	}
	if (i == n)		return FALSE;

	for (i=0; i<n; i++) {
		if (cRp[i].m_fY > m_fMinY)
			break;
	}
	if (i == n)		return FALSE;

	for (i=0; i<n; i++) {
		if (cRp[i].m_fX < m_fMaxX)
			break;
	}
	if (i == n)		return FALSE;

	for (i=0; i<n; i++) {
		if (cRp[i].m_fY < m_fMaxY)
			break;
	}
	if (i == n)		return FALSE;
	return TRUE;
}

CCAD_Point CCAD_Rect::CenterPt()
{
	CCAD_Point retPt;
	
	retPt.m_fX = (m_fMinX + m_fMaxX) / 2 ;
	retPt.m_fY = (m_fMinY + m_fMaxY) / 2 ;
	retPt.m_fZ = (m_fMinZ + m_fMaxZ) / 2 ;

	return retPt;
}

BOOL	CCAD_Rect::IsClipCircle(CCAD_Vector & cCp, CAD_FLOAT fR)
{
	if ( cCp.m_fY - fR >= m_fMaxY )
		return false;
	if ( cCp.m_fY + fR <= m_fMinY )
		return false;
	if ( cCp.m_fX - fR >= m_fMaxX )
		return false;
	if ( cCp.m_fX + fR <= m_fMinX )
		return false;
	return true;
}

BOOL	CCAD_Rect::IsClipRect(CCAD_Vector * cRp)
{
	int i;
	for ( i=0; i<4; i++ ) {
		if ( IsPointInRect2D(cRp[i]) )
			return true;
	}

	CCAD_Rect rc;
	for ( i=0; i<4; i++ )
		rc.ResetParameter(cRp[i]);

	if ( rc.m_fMinY >= m_fMaxY )
		return false;
	if ( rc.m_fMaxY <= m_fMinY )
		return false;
	if ( rc.m_fMinX >= m_fMaxX )
		return false;
	if ( rc.m_fMaxX <= m_fMinX )
		return false;
		
	for ( i=0; i<4; i++ ) {
		int k = (i==3) ? 0 : (i+1);
		if (IsCrossLine2D(cRp[i], cRp[k]) )
			return true;
	}

/*	if ( rc.m_fMinX < m_fMinX )
		return false;
	if ( rc.m_fMaxX > m_fMaxX )
		return false;
	if ( rc.m_fMinY < m_fMinY )
		return false;
	if ( rc.m_fMaxY > m_fMaxY )
		return false;
	return true;
*/
	if ( rc.m_fMinX < m_fMinX && rc.m_fMaxX > m_fMaxX &&
		 rc.m_fMinY < m_fMinY && rc.m_fMaxY > m_fMaxY )
		return true;
	return false;
//----	Ended.
}


void CCAD_Rect::Read(CCAD_File & cFile)
{
	cFile.ReadCAD_Float(&m_fMinX);
	cFile.ReadCAD_Float(&m_fMinY);
	cFile.ReadCAD_Float(&m_fMaxX);
	cFile.ReadCAD_Float(&m_fMaxY);
	cFile.ReadCAD_Float(&m_fMinZ);
	cFile.ReadCAD_Float(&m_fMaxZ);
}

 
