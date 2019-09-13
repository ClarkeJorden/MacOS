#include "stdafx.h"
#include "CAD_Box.h"
#include "CAD_Vector.h"
#include "CAD_File.h"
#include "CAD_Global.h"


CCAD_Box::CCAD_Box()
{
	m_fMinX = m_fMinY = m_fMinZ = MAX_DOUBLE;
	m_fMaxX = m_fMaxY = m_fMaxZ = MIN_DOUBLE;
}

CCAD_Box::CCAD_Box(const CCAD_Box & rc)
{
	m_fMinX = rc.m_fMinX;
	m_fMinY = rc.m_fMinY;
	m_fMinZ = rc.m_fMinZ;
	m_fMaxX = rc.m_fMaxX;
	m_fMaxY = rc.m_fMaxY;
	m_fMaxZ = rc.m_fMaxZ;
}

//for processing #define CCAD_Rect CCAD_Box.There is no modules of Read and Write for class CCAD_Rect.
//Because class CCAD_Rect has been defined in project CAD_Core,it can't access to class CCAD_File of project CAD_Base.
CCAD_Box::CCAD_Box(const RECT &rc)
{
	m_fMinX = rc.left;
	m_fMinY = rc.top;
	m_fMinZ = 0;
	m_fMaxX = rc.right;
	m_fMaxY = rc.bottom;
	m_fMaxZ = 0;
}

CCAD_Box::CCAD_Box(const SCAD_POINT & cPt, const CAD_FLOAT & dR)
{
	SetParameter(cPt, dR);
}

CCAD_Box::CCAD_Box(const SCAD_POINT & cPt1, const SCAD_POINT & cPt2)
{
	SetParameter(cPt1, cPt2);
}

CCAD_Box & CCAD_Box::operator=(const CCAD_Box & rc)
{
	m_fMinX = rc.m_fMinX;
	m_fMinY = rc.m_fMinY;
	m_fMinZ = rc.m_fMinZ;
	m_fMaxX = rc.m_fMaxX;
	m_fMaxY = rc.m_fMaxY;
	m_fMaxZ = rc.m_fMaxZ;
	return *this;
}

void CCAD_Box::Init()
{
	m_fMinX = m_fMinY = m_fMinZ = MAX_DOUBLE;
	m_fMaxX = m_fMaxY = m_fMaxZ = MIN_DOUBLE;
}

BOOL CCAD_Box::IsInited() const
{
	return !(	m_fMinX < MAX_DOUBLE && m_fMinY < MAX_DOUBLE && m_fMinZ < MAX_DOUBLE &&
				m_fMaxX > MIN_DOUBLE && m_fMaxY > MIN_DOUBLE && m_fMaxZ > MIN_DOUBLE	);
}

void CCAD_Box::SetParameter(const SCAD_POINT & cPt, CAD_FLOAT dR)
{
	CCAD_Global::SetSignValue(dR, 1);

	m_fMinX = cPt.m_fX - dR;
	m_fMaxX = cPt.m_fX + dR;
	m_fMinY = cPt.m_fY - dR;
	m_fMaxY = cPt.m_fY + dR;
	m_fMinZ = cPt.m_fZ - dR;
	m_fMaxZ = cPt.m_fZ + dR;
}

void CCAD_Box::SetParameter(const SCAD_POINT & cPt1, const SCAD_POINT & cPt2)
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

void CCAD_Box::ResetParameter(const SCAD_POINT & cPt)
{
	if(m_fMinX > cPt.m_fX)
		m_fMinX = cPt.m_fX;
	if(m_fMaxX < cPt.m_fX)
		m_fMaxX = cPt.m_fX;
	
	if(m_fMinY > cPt.m_fY)
		m_fMinY = cPt.m_fY;
	if(m_fMaxY < cPt.m_fY)
		m_fMaxY = cPt.m_fY;
	
	if(m_fMinZ > cPt.m_fZ)
		m_fMinZ = cPt.m_fZ;
	if(m_fMaxZ < cPt.m_fZ)
		m_fMaxZ = cPt.m_fZ;
}

void CCAD_Box::ResetParameter(const CCAD_Box & rc)
{
	if(rc.IsInited())
		return;

	if(m_fMinX > rc.m_fMinX)
		m_fMinX = rc.m_fMinX;
	if(m_fMaxX < rc.m_fMaxX)
		m_fMaxX = rc.m_fMaxX;
	
	if(m_fMinY > rc.m_fMinY)
		m_fMinY = rc.m_fMinY;
	if(m_fMaxY < rc.m_fMaxY)
		m_fMaxY = rc.m_fMaxY;
	
	if(m_fMinZ > rc.m_fMinZ)
		m_fMinZ = rc.m_fMinZ;
	if(m_fMaxZ < rc.m_fMaxZ)
		m_fMaxZ = rc.m_fMaxZ;
}

BOOL CCAD_Box::IsPointInNear(const SCAD_POINT & cPt) const
{
	return (cPt.m_fZ >= m_fMinZ);
}

BOOL CCAD_Box::ClipLineInNear(SCAD_POINT & cPt1, SCAD_POINT & cPt2) const
{
	BOOL blRet = true;

	if(IsPointInNear(cPt1) && !IsPointInNear(cPt2))	{
		cPt2.m_fX = cPt1.m_fX + (m_fMinZ-cPt1.m_fZ) * (cPt2.m_fX-cPt1.m_fX)/(cPt2.m_fZ-cPt1.m_fZ);
		cPt2.m_fY = cPt1.m_fY + (m_fMinZ-cPt1.m_fZ) * (cPt2.m_fY-cPt1.m_fY)/(cPt2.m_fZ-cPt1.m_fZ);
		cPt2.m_fZ = m_fMinZ;
	}
	else if(!IsPointInNear(cPt1) && IsPointInNear(cPt2))	{
		cPt1.m_fX = cPt2.m_fX + (m_fMinZ-cPt2.m_fZ) * (cPt1.m_fX-cPt2.m_fX)/(cPt1.m_fZ-cPt2.m_fZ);
		cPt1.m_fY = cPt2.m_fY + (m_fMinZ-cPt2.m_fZ) * (cPt1.m_fY-cPt2.m_fY)/(cPt1.m_fZ-cPt2.m_fZ);
		cPt1.m_fZ = m_fMinZ;
	}
	else
		blRet = false;
	
	return blRet;
}

BOOL CCAD_Box::IsPointInRect2D(const SCAD_POINT & cPt ) const
{
	return (cPt.m_fX >= m_fMinX &&
			cPt.m_fX <= m_fMaxX &&
			cPt.m_fY >= m_fMinY &&
			cPt.m_fY <= m_fMaxY);
}

BOOL CCAD_Box::IsPointInRect2D_NE( const SCAD_POINT & cPt ) const
{
	return (cPt.m_fX>m_fMinX &&
			cPt.m_fX<m_fMaxX &&
			cPt.m_fY>m_fMinY &&
			cPt.m_fY<m_fMaxY);
}

BOOL CCAD_Box::IsLineInRect2D(const SCAD_POINT & cSp, const SCAD_POINT & cEp) const
{
	return (IsPointInRect2D(cSp) && IsPointInRect2D(cEp));
}

BOOL CCAD_Box::IsCorssWithBox(CCAD_Box &cBox)
{
	if( cBox.m_fMaxX < m_fMinX)
		return false;
	if( cBox.m_fMaxY < m_fMinY)
		return false;
//	if( cBox.m_fMaxZ < m_fMinZ)
//		return false;
	if( cBox.m_fMinX > m_fMaxX)
		return false;
	if( cBox.m_fMinY > m_fMaxY)
		return false;
//	if( cBox.m_fMinZ > m_fMaxZ)
//		return false;
//	if( cBox.m_fMaxX < m_fMaxX && cBox.m_fMinX > m_fMinX
//		&& cBox.m_fMaxY < m_fMaxY && cBox.m_fMinY > m_fMinY){
//		&& cBox.m_fMaxZ < m_fMaxZ && cBox.m_fMinZ > m_fMinZ
//		return false;
//	}
	return true;
}

BOOL CCAD_Box::IsLineInRect2D_NE(const SCAD_POINT & cSp, const SCAD_POINT & cEp) const
{
	return (IsPointInRect2D_NE(cSp) && IsPointInRect2D_NE(cEp));
}

BOOL CCAD_Box::IsCrossable_LineSeg_Range_XY(const SCAD_POINT & pt1, const SCAD_POINT & pt2) const
{
	if(pt1.m_fX < m_fMinX && pt2.m_fX < m_fMinX)
		return false;
	if(pt1.m_fX > m_fMaxX && pt2.m_fX > m_fMaxX)
		return false;
	if(pt1.m_fY < m_fMinY && pt2.m_fY < m_fMinY)
		return false;
	if(pt1.m_fY > m_fMaxY && pt2.m_fY > m_fMaxY)
		return false;

	return true;
}

BOOL CCAD_Box::Clip_LineSeg_Range_XY(SCAD_POINT & pt1, SCAD_POINT & pt2) const
{
	if(!IsCrossable_LineSeg_Range_XY(pt1, pt2))
		return false;

	if(IsPointInRect2D(pt1))
	{
		if(!IsPointInRect2D(pt2))
			CountPointInRect2D(pt1, pt2);
	}
	else
	{
		if(IsPointInRect2D(pt2))
			CountPointInRect2D(pt2, pt1);
		else
		{
			CountPointInRect2D(pt1, pt2);
			if(!IsPointInRect2D(pt2))
				return false;

			CountPointInRect2D(pt2, pt1);
		}
	}

	return true;
}

void CCAD_Box::CountPointInRect2D(const SCAD_POINT & inPt, SCAD_POINT & outPt) const
{
	if(inPt.m_fX == outPt.m_fX)
	{
		if(outPt.m_fY > m_fMaxY)
			outPt.m_fY = m_fMaxY;
		else
		if(outPt.m_fY < m_fMinY)
			outPt.m_fY = m_fMinY;
		else
			ASSERT(false);
	}
	else
	if(inPt.m_fY == outPt.m_fY)
	{
		if(outPt.m_fX > m_fMaxX)
			outPt.m_fX = m_fMaxX;
		else
		if(outPt.m_fX < m_fMinX)
			outPt.m_fX = m_fMinX;
		else
			ASSERT(false);
	}
	else
	{
		if(outPt.m_fX < m_fMinX)
		{
			outPt.m_fY = inPt.m_fY + (m_fMinX-inPt.m_fX) * (outPt.m_fY-inPt.m_fY)/(outPt.m_fX-inPt.m_fX);
			outPt.m_fX = m_fMinX;
		}
		else
		if(outPt.m_fX > m_fMaxX)
		{
			outPt.m_fY = inPt.m_fY + (m_fMaxX-inPt.m_fX) * (outPt.m_fY-inPt.m_fY)/(outPt.m_fX-inPt.m_fX);
			outPt.m_fX = m_fMaxX;
		}
		
		if(outPt.m_fY < m_fMinY)
		{
			outPt.m_fX = inPt.m_fX + (m_fMinY-inPt.m_fY) * (outPt.m_fX-inPt.m_fX)/(outPt.m_fY-inPt.m_fY);
			outPt.m_fY = m_fMinY;
		}
		else
		if(outPt.m_fY > m_fMaxY)
		{
			outPt.m_fX = inPt.m_fX + (m_fMaxY-inPt.m_fY) * (outPt.m_fX-inPt.m_fX)/(outPt.m_fY-inPt.m_fY);
			outPt.m_fY = m_fMaxY;
		}
	}
}

BOOL CCAD_Box::IsCross_LineSeg_Range_XY(const SCAD_POINT &pt1,const SCAD_POINT &pt2)
{
	if(IsCrossable_LineSeg_Range_XY(pt1,pt2))
	{
		CAD_FLOAT A = -(pt2.m_fY - pt1.m_fY);
		CAD_FLOAT B = pt2.m_fX - pt1.m_fX;
		
		if(A == 0 && B == 0)
		{	// pt1̮ pt2
			return IsPointInRect2D(pt1);
		}

		CAD_FLOAT C = -A * pt1.m_fX - B * pt1.m_fY;

		int nDirect = 0;

		if(A * m_fMinX + B * m_fMinY + C > 0)
			nDirect ++;

		if(A * m_fMinX + B * m_fMaxY + C > 0)
			nDirect ++;
		
		if(A * m_fMaxX + B * m_fMinY + C > 0)
			nDirect ++;
		
		if(A * m_fMaxX + B * m_fMaxY + C > 0)
			nDirect ++;

		return (0 < nDirect) && (nDirect < 4);
	}

	return false;
}	

void CCAD_Box::ClipRayInRect2D(BYTE bContinue, SCAD_POINT & cSp, SCAD_POINT & cEp) const
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

BOOL CCAD_Box::IsPointsInRect2D(SCAD_POINT * cRp, short n) const
{
	short	i;
	for (i=0; i<n; i++) {
		if (cRp[i].m_fX > m_fMinX)
			break;
	}
	if (i == n)		return false;

	for (i=0; i<n; i++) {
		if (cRp[i].m_fY > m_fMinY)
			break;
	}
	if (i == n)		return false;

	for (i=0; i<n; i++) {
		if (cRp[i].m_fX < m_fMaxX)
			break;
	}
	if (i == n)		return false;

	for (i=0; i<n; i++) {
		if (cRp[i].m_fY < m_fMaxY)
			break;
	}
	if (i == n)		return false;
	return true;
}

SCAD_POINT CCAD_Box::CenterPt() const
{
	SCAD_POINT retPt;
	
	retPt.m_fX = (m_fMinX + m_fMaxX) / 2 ;
	retPt.m_fY = (m_fMinY + m_fMaxY) / 2 ;
	retPt.m_fZ = (m_fMinZ + m_fMaxZ) / 2 ;

	return retPt;
}

BOOL	CCAD_Box::IsClipCircle(CCAD_Vector & cCp, CAD_FLOAT fR)
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

BOOL	CCAD_Box::IsClipRect(CCAD_Vector * cRp)
{
	int i;
	for ( i=0; i<4; i++ ) {
		if ( IsPointInRect2D(cRp[i]) )
			return true;
	}

	CCAD_Box rc;
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
		if (IsCross_LineSeg_Range_XY(cRp[i], cRp[k]) )
			return true;
	}

	if ( rc.m_fMinX < m_fMinX )
		return false;
	if ( rc.m_fMaxX > m_fMaxX )
		return false;
	if ( rc.m_fMinY < m_fMinY )
		return false;
	if ( rc.m_fMaxY > m_fMaxY )
		return false;

	return true;
}

void CCAD_Box::Write(CCAD_File & cFile)
{
	cFile.WriteCadFloat(m_fMinX);
	cFile.WriteCadFloat(m_fMinY);
	cFile.WriteCadFloat(m_fMaxX);
	cFile.WriteCadFloat(m_fMaxY);
	cFile.WriteCadFloat(m_fMinZ);
	cFile.WriteCadFloat(m_fMaxZ);
}

void CCAD_Box::Read(CCAD_File & cFile)
{
	cFile.ReadCAD_Float(&m_fMinX);
	cFile.ReadCAD_Float(&m_fMinY);
	cFile.ReadCAD_Float(&m_fMaxX);
	cFile.ReadCAD_Float(&m_fMaxY);
	cFile.ReadCAD_Float(&m_fMinZ);
	cFile.ReadCAD_Float(&m_fMaxZ);
}

void CCAD_Box::InitZero ()
{
	m_fMaxX = m_fMaxY = m_fMaxZ = 0;
	m_fMinX = m_fMinY = m_fMinZ = 0;
}

BOOL CCAD_Box::operator == (const CCAD_Box & other) const
{
	if(m_fMinX != other.m_fMinX)
		return false;
	if(m_fMinY != other.m_fMinY)
		return false;
	if(m_fMinZ != other.m_fMinZ)
		return false;
	if(m_fMaxX != other.m_fMaxX)
		return false;
	if(m_fMaxY != other.m_fMaxY)
		return false;
	if(m_fMaxZ != other.m_fMaxZ)
		return false;
	
	return true;
}

BOOL CCAD_Box::operator != (const CCAD_Box & other) const
{
	return !(*this == other);
}

