
#include "stdafx.h"
#include "CAD_Global.h"
#include "CAD_Coord3D.h"
#include "CAD_GeoPlan.h"
//#include <math.h>

 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCAD_GeoPlan::CCAD_GeoPlan()
{
	
}
CCAD_GeoPlan::CCAD_GeoPlan(const CCAD_Point& pt1,
						   const CCAD_Point& pt2,
						   const CCAD_Point& pt3)
{
	Create(pt1, pt2, pt3);
}

CCAD_GeoPlan::CCAD_GeoPlan(const CCAD_Coord3D & cCoord)
{
	Create(cCoord);
}

CCAD_GeoPlan::~CCAD_GeoPlan()
{

}
void CCAD_GeoPlan::Create(const CCAD_Point& pt1, 
						  const CCAD_Point& pt2,
						  const CCAD_Point& pt3)
{
	m_Pt1 = pt1;
	m_Pt2 = pt2;
	m_Pt3 = pt3;

	ComputeNormal();
}

void CCAD_GeoPlan::Create(const CCAD_Coord3D & cCoord)
{
	m_Pt1 = cCoord.m_cOrg;
	m_Pt2 = cCoord.m_cOrg + cCoord.m_cX;
	m_Pt3 = cCoord.m_cOrg + cCoord.m_cY;
	ComputeNormal();
}

CCAD_Coord3D CCAD_GeoPlan::CreateCoord3D()
{
	CCAD_Coord3D retCoord;

	retCoord.m_cOrg = m_Pt1;
	// x
	retCoord.m_cX = CCAD_Vector(m_Pt1,m_Pt2);
	retCoord.m_cX.Normalize();
	// z
	retCoord.m_cZ = m_cNormal;
	retCoord.m_cZ.Normalize();
	// y
	retCoord.m_cY = retCoord.m_cZ * retCoord.m_cX;
	retCoord.m_cY.Normalize();

	return retCoord;
}

BOOL CCAD_GeoPlan::IsFront(const CCAD_Point& inPt)
{
	CAD_FLOAT fD = (m_cNormal & inPt);

	if( ( m_fD + fD ) > CAD_ZERO )
		return true;

	return false; 
}

void CCAD_GeoPlan::ComputeNormal()
{
	m_cNormal.m_fX = m_Pt1.m_fY * (m_Pt2.m_fZ - m_Pt3.m_fZ) + 
					 m_Pt2.m_fY * (m_Pt3.m_fZ - m_Pt1.m_fZ) + 
					 m_Pt3.m_fY * (m_Pt1.m_fZ - m_Pt2.m_fZ) ;

	m_cNormal.m_fY = m_Pt1.m_fZ * (m_Pt2.m_fX - m_Pt3.m_fX) + 
					 m_Pt2.m_fZ * (m_Pt3.m_fX - m_Pt1.m_fX) + 
					 m_Pt3.m_fZ * (m_Pt1.m_fX - m_Pt2.m_fX) ;
	
	m_cNormal.m_fZ = m_Pt1.m_fX * (m_Pt2.m_fY - m_Pt3.m_fY) + 
					 m_Pt2.m_fX * (m_Pt3.m_fY - m_Pt1.m_fY) + 
					 m_Pt3.m_fX * (m_Pt1.m_fY - m_Pt2.m_fY) ;

	m_fD = - ( m_cNormal & m_Pt1 );

	double f = m_cNormal.Abs();

	m_cNormal.m_fX = (CAD_FLOAT)(m_cNormal.m_fX / f);
	m_cNormal.m_fY = (CAD_FLOAT)(m_cNormal.m_fY / f);
	m_cNormal.m_fZ = (CAD_FLOAT)(m_cNormal.m_fZ / f);
	m_fD = (CAD_FLOAT)(m_fD / f);
}

BOOL CCAD_GeoPlan::IsCross(const CCAD_Point& pt1,const CCAD_Point& pt2)
{
	CAD_FLOAT fD1 = (m_cNormal & pt1);
	CAD_FLOAT fD2 = (m_cNormal & pt2);
	
	if( m_fD >= fD1 && m_fD >= fD2)
		return false;
	
	if( m_fD < fD1 && m_fD < fD2)
		return false;

	return true;
}

BOOL CCAD_GeoPlan::IsPtOnPlan(const CCAD_Vector & cPt) const
{
	CAD_FLOAT	fDis = m_cNormal.m_fX * cPt.m_fX + m_cNormal.m_fY * cPt.m_fY +
						m_cNormal.m_fZ * cPt.m_fZ + m_fD;
	return (fabs(fDis) <= CCAD_Global::GetLengthAccy());
}

BOOL CCAD_GeoPlan::IsEqual(const CCAD_GeoPlan & cPlan) const
{
	if ( !IsParallel(cPlan) )
		return FALSE;
	return IsPtOnPlan(cPlan.m_Pt1);
}

BOOL CCAD_GeoPlan::IsParallel(const CCAD_GeoPlan & cPlan) const
{
	return m_cNormal.IsParallel(cPlan.m_cNormal);
}

BOOL CCAD_GeoPlan::CrossPt(const CCAD_Point& cPt1,
						   const CCAD_Point& cPt2, 
						   CCAD_Point& CrossPt)
{
	CCAD_Point p1 = cPt1;
	CCAD_Point p2 = cPt2;

	// scalar product
	CAD_FLOAT fScalar1 = m_cNormal.m_fX * p1.m_fX + 
						 m_cNormal.m_fY * p1.m_fY + 
						 m_cNormal.m_fZ * p1.m_fZ;

	CAD_FLOAT fScalar2 = m_cNormal.m_fX * p2.m_fX + 
						 m_cNormal.m_fY * p2.m_fY + 
						 m_cNormal.m_fZ * p2.m_fZ;

	CAD_FLOAT fDeno =  fScalar2 - fScalar1;

	if( IS_ZERO(fDeno) ) 
		return false;

	// cross processing
	CAD_FLOAT t = -( fScalar1 + m_fD ) / fDeno;

	CrossPt.m_fX = p1.m_fX + t * ( p2.m_fX - p1.m_fX );
	CrossPt.m_fY = p1.m_fY + t * ( p2.m_fY - p1.m_fY );
	CrossPt.m_fZ = p1.m_fZ + t * ( p2.m_fZ - p1.m_fZ );
	
	return true;
}

BOOL CCAD_GeoPlan::ClipLine(CCAD_Point& cPt1,
							CCAD_Point& cPt2,
							int nDirect = FRONT_DIRECTION )
{
	CCAD_Point p1 = cPt1;
	CCAD_Point p2 = cPt2;

	// scalar product
	CAD_FLOAT fScalar1 = m_cNormal.m_fX * p1.m_fX + 
						 m_cNormal.m_fY * p1.m_fY + 
						 m_cNormal.m_fZ * p1.m_fZ;

	CAD_FLOAT fScalar2 = m_cNormal.m_fX * p2.m_fX + 
						 m_cNormal.m_fY * p2.m_fY + 
						 m_cNormal.m_fZ * p2.m_fZ;

	BOOL bFront1 = ( (fScalar1 + m_fD) > 0 ) ? true : false;
	BOOL bFront2 = ( (fScalar2 + m_fD) > 0 ) ? true : false;

	if( bFront1 && bFront2 ){
		if( nDirect == FRONT_DIRECTION )
			return true;
		return false;
	}
	
	if( !bFront1 && !bFront2 ){
		if( nDirect == FRONT_DIRECTION )
			return false;
		return true;
	}

	// cross processing
	CAD_FLOAT t = -( fScalar1 + m_fD ) / ( fScalar2 - fScalar1 );

	CAD_FLOAT fCrossX = p1.m_fX + t * ( p2.m_fX - p1.m_fX );
	CAD_FLOAT fCrossY = p1.m_fY + t * ( p2.m_fY - p1.m_fY );
	CAD_FLOAT fCrossZ = p1.m_fZ + t * ( p2.m_fZ - p1.m_fZ );
	
	if( nDirect == FRONT_DIRECTION ){

		if(  bFront1 ){
			cPt2.m_fX = fCrossX;
			cPt2.m_fY = fCrossY;
			cPt2.m_fZ = fCrossZ;
		}else{
			cPt1.m_fX = fCrossX;
			cPt1.m_fY = fCrossY;
			cPt1.m_fZ = fCrossZ;
		}

	}else{

		if( bFront1 ){
			cPt1.m_fX = fCrossX;
			cPt1.m_fY = fCrossY;
			cPt1.m_fZ = fCrossZ;
		}else{
			cPt2.m_fX = fCrossX;
			cPt2.m_fY = fCrossY;
			cPt2.m_fZ = fCrossZ;
		}

	}
	
	return true;
}

 
