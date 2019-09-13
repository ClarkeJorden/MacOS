// CAD_Coord3D.cpp: implementation of the CCAD_Coord3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CAD_Constance.h"
#include "CAD_Angle.h"
#include "CAD_GeoPlan.h"
#include "CAD_Coord3D.h"
//#include "math.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCAD_Coord3D::CCAD_Coord3D()
{
	Init();
}

CCAD_Coord3D::CCAD_Coord3D(const CCAD_GeoPlan & cPlan)
{
	cPlan.GetPoint1(m_cOrg);
	cPlan.GetPoint2(m_cX);

	m_cX -= m_cOrg;
	m_cX.Normalize();
	m_cZ = cPlan.m_cNormal;
	m_cZ.Normalize();
	m_cY = m_cZ * m_cX;

	m_fScale = 1;
}

// In Base Plan
CCAD_Coord3D::CCAD_Coord3D(const CCAD_Point & cStart, const CCAD_Point & cEnd)
{
	m_fScale = 1;
	SetCoord(cStart, CCAD_Vector(cStart, cEnd));
}

CCAD_Coord3D::~CCAD_Coord3D()
{

}

void CCAD_Coord3D::Init()
{
	m_cOrg.m_fX = m_cOrg.m_fY = m_cOrg.m_fZ = 0.0;
	m_cX.m_fX = m_cY.m_fY = m_cZ.m_fZ = 1.0;
	
	m_cX.m_fY = m_cX.m_fZ = 0;
	m_cY.m_fX = m_cY.m_fZ = 0;
	m_cZ.m_fX = m_cZ.m_fY = 0;

	m_fScale = 1;
}
void CCAD_Coord3D::Read(CCAD_File & cFile)
{
	m_cOrg.Read(cFile);
	m_cX.Read(cFile);
	m_cY.Read(cFile);
	m_cZ.Read(cFile);
}

void CCAD_Coord3D::Write(CCAD_File & cFile)
{
	m_cOrg.Write(cFile);
	m_cX.Write(cFile);
	m_cY.Write(cFile);
	m_cZ.Write(cFile);
}

////////////////////////////////////////
void CCAD_Coord3D::SetCoord(const CCAD_Point & cOrg, const CCAD_Vector & cAxisX)
{
	m_cOrg = cOrg;

	m_cX = cAxisX;
	m_cX.m_fZ = 0;
	m_cX.Normalize();

	m_cZ.m_fX = m_cZ.m_fY = 0;
	m_cZ.m_fZ = 1;

	m_cY = m_cZ * m_cX;
}

void CCAD_Coord3D::operator = (const CCAD_Coord3D& cOther)
{
	m_cOrg = cOther.m_cOrg;
	m_cX = cOther.m_cX;
	m_cY = cOther.m_cY;
	m_cZ = cOther.m_cZ;

	m_fScale = cOther.m_fScale;
}

void CCAD_Coord3D::Normalize()
{
	CAD_FLOAT X = (CAD_FLOAT)sqrt(m_cX.m_fX * m_cX.m_fX + m_cX.m_fY * m_cX.m_fY + m_cX.m_fZ * m_cX.m_fZ);
	CAD_FLOAT Y = (CAD_FLOAT)sqrt(m_cY.m_fX * m_cY.m_fX + m_cY.m_fY * m_cY.m_fY + m_cY.m_fZ * m_cY.m_fZ);
	CAD_FLOAT Z = (CAD_FLOAT)sqrt(m_cZ.m_fX * m_cZ.m_fX + m_cZ.m_fY * m_cZ.m_fY + m_cZ.m_fZ * m_cZ.m_fZ);

	if ( IS_ZERO(X) || IS_ZERO(Y) || IS_ZERO(Z) ) {
		m_cX.m_fX = m_cY.m_fY = m_cZ.m_fZ = 1;
		m_cX.m_fY = m_cX.m_fZ = m_cY.m_fX = m_cY.m_fZ = m_cZ.m_fX = m_cZ.m_fY = 0;
	}
	else {
		m_cX.m_fX = m_cX.m_fX / X;
		m_cX.m_fY = m_cX.m_fY / X;
		m_cX.m_fZ = m_cX.m_fZ / X;

		m_cY.m_fX = m_cY.m_fX / Y;
		m_cY.m_fY = m_cY.m_fY / Y;
		m_cY.m_fZ = m_cY.m_fZ / Y;

		m_cZ.m_fX = m_cZ.m_fX / Z;
		m_cZ.m_fY = m_cZ.m_fY / Z;
		m_cZ.m_fZ = m_cZ.m_fZ / Z;
	}
}


void CCAD_Coord3D::ConvToBase(SCAD_POINT& sOldPt) const
{
	CAD_FLOAT fOldX = sOldPt.m_fX;
	CAD_FLOAT fOldY = sOldPt.m_fY;
	CAD_FLOAT fOldZ = sOldPt.m_fZ;

	sOldPt.m_fX = m_cOrg.m_fX + (fOldX * m_cX.m_fX + fOldY * m_cY.m_fX + fOldZ * m_cZ.m_fX) * m_fScale;
	sOldPt.m_fY = m_cOrg.m_fY + (fOldX * m_cX.m_fY + fOldY * m_cY.m_fY + fOldZ * m_cZ.m_fY) * m_fScale;
	sOldPt.m_fZ = m_cOrg.m_fZ + (fOldX * m_cX.m_fZ + fOldY * m_cY.m_fZ + fOldZ * m_cZ.m_fZ) * m_fScale;
}

void CCAD_Coord3D::ConvToBaseX(SCAD_POINT& sOldPt, BOOL bIsVtr) const
{
	CAD_FLOAT fOldX = sOldPt.m_fX;
	CAD_FLOAT fOldY = sOldPt.m_fY;
	CAD_FLOAT fOldZ = sOldPt.m_fZ;

	if(bIsVtr){
		sOldPt.m_fX = fOldX * m_cX.m_fX + fOldY * m_cY.m_fX + fOldZ * m_cZ.m_fX;
		sOldPt.m_fY = fOldX * m_cX.m_fY + fOldY * m_cY.m_fY + fOldZ * m_cZ.m_fY;
		sOldPt.m_fZ = fOldX * m_cX.m_fZ + fOldY * m_cY.m_fZ + fOldZ * m_cZ.m_fZ;
	}else{
		if(m_fScale == 1.0f){
			sOldPt.m_fX = m_cOrg.m_fX + fOldX * m_cX.m_fX + fOldY * m_cY.m_fX + fOldZ * m_cZ.m_fX;
			sOldPt.m_fY = m_cOrg.m_fY + fOldX * m_cX.m_fY + fOldY * m_cY.m_fY + fOldZ * m_cZ.m_fY;
			sOldPt.m_fZ = m_cOrg.m_fZ + fOldX * m_cX.m_fZ + fOldY * m_cY.m_fZ + fOldZ * m_cZ.m_fZ;
		}else{
			sOldPt.m_fX = m_cOrg.m_fX + (fOldX * m_cX.m_fX + fOldY * m_cY.m_fX + fOldZ * m_cZ.m_fX) * m_fScale;
			sOldPt.m_fY = m_cOrg.m_fY + (fOldX * m_cX.m_fY + fOldY * m_cY.m_fY + fOldZ * m_cZ.m_fY) * m_fScale;
			sOldPt.m_fZ = m_cOrg.m_fZ + (fOldX * m_cX.m_fZ + fOldY * m_cY.m_fZ + fOldZ * m_cZ.m_fZ) * m_fScale;
		}
	}
}

void CCAD_Coord3D::ConvVectorToBase(CCAD_Vector& sOldPt) const
{
	CCAD_Vector cDir = sOldPt;

	sOldPt.m_fX = (cDir.m_fX * m_cX.m_fX + cDir.m_fY * m_cY.m_fX + cDir.m_fZ * m_cZ.m_fX) * m_fScale;
	sOldPt.m_fY = (cDir.m_fX * m_cX.m_fY + cDir.m_fY * m_cY.m_fY + cDir.m_fZ * m_cZ.m_fY) * m_fScale;
	sOldPt.m_fZ = (cDir.m_fX * m_cX.m_fZ + cDir.m_fY * m_cY.m_fZ + cDir.m_fZ * m_cZ.m_fZ) * m_fScale;
}

void CCAD_Coord3D::ConvFromBase(SCAD_POINT& oldPt) const
{
//	if(this == &g_pModelView->m_cCamera){
//		float *cmat = g_pModelView->m_cCamera.m_fModelViewMat;
//		SCAD_POINT tpnt;
//		tpnt.m_fX = cmat[0] * oldPt.m_fX + cmat[1] * oldPt.m_fY + cmat[2] * oldPt.m_fZ + cmat[3];
//		tpnt.m_fY = cmat[4] * oldPt.m_fX + cmat[5] * oldPt.m_fY + cmat[6] * oldPt.m_fZ + cmat[7];
//		tpnt.m_fZ = cmat[8] * oldPt.m_fX + cmat[9] * oldPt.m_fY + cmat[10] * oldPt.m_fZ + cmat[11];
//		oldPt = tpnt;
//		return;
//	}

	CCAD_Vector	cDir(m_cOrg, oldPt);
	CAD_FLOAT	fD = 1.0F;

	fD = m_cX.m_fX * (m_cY.m_fY*m_cZ.m_fZ - m_cY.m_fZ*m_cZ.m_fY)
		+ m_cX.m_fY * (m_cY.m_fZ*m_cZ.m_fX - m_cY.m_fX*m_cZ.m_fZ)
		+ m_cX.m_fZ * (m_cY.m_fX*m_cZ.m_fY - m_cY.m_fY*m_cZ.m_fX);
	fD = ( fD > 0 ) ? 1.0F : -1.0F;
	
	oldPt.m_fX = (	cDir.m_fX*(m_cY.m_fY*m_cZ.m_fZ-m_cY.m_fZ*m_cZ.m_fY) +
					cDir.m_fY*(m_cY.m_fZ*m_cZ.m_fX-m_cY.m_fX*m_cZ.m_fZ) +
					cDir.m_fZ*(m_cY.m_fX*m_cZ.m_fY-m_cY.m_fY*m_cZ.m_fX) ) / m_fScale * fD;

	oldPt.m_fY = (	cDir.m_fX*(m_cX.m_fZ*m_cZ.m_fY-m_cX.m_fY*m_cZ.m_fZ) +
					cDir.m_fY*(m_cX.m_fX*m_cZ.m_fZ-m_cX.m_fZ*m_cZ.m_fX) +
					cDir.m_fZ*(m_cX.m_fY*m_cZ.m_fX-m_cX.m_fX*m_cZ.m_fY) ) / m_fScale * fD; 

	oldPt.m_fZ = (	cDir.m_fX*(m_cX.m_fY*m_cY.m_fZ-m_cX.m_fZ*m_cY.m_fY) +
					cDir.m_fY*(m_cX.m_fZ*m_cY.m_fX-m_cX.m_fX*m_cY.m_fZ) +
					cDir.m_fZ*(m_cX.m_fX*m_cY.m_fY-m_cX.m_fY*m_cY.m_fX) ) / m_fScale * fD;
}

void CCAD_Coord3D::ConvVectorFromBase(CCAD_Vector& oldPt) const
{
//	if(this == &g_pModelView->m_cCamera){
//		CCAD_Vector tvtr;
//		float *cmat = g_pModelView->m_cCamera.m_fModelViewMat;
//		tvtr.m_fX = cmat[0] * oldPt.m_fX + cmat[1] * oldPt.m_fY + cmat[2] * oldPt.m_fZ;
//		tvtr.m_fY = cmat[4] * oldPt.m_fX + cmat[5] * oldPt.m_fY + cmat[6] * oldPt.m_fZ;
//		tvtr.m_fZ = cmat[8] * oldPt.m_fX + cmat[9] * oldPt.m_fY + cmat[10] * oldPt.m_fZ;
//		oldPt = tvtr;
//		return;
//	}
	CCAD_Vector	cDir = oldPt;
	CAD_FLOAT	fD = 1.0F;

	fD = m_cX.m_fX * (m_cY.m_fY*m_cZ.m_fZ - m_cY.m_fZ*m_cZ.m_fY)
		+ m_cX.m_fY * (m_cY.m_fZ*m_cZ.m_fX - m_cY.m_fX*m_cZ.m_fZ)
		+ m_cX.m_fZ * (m_cY.m_fX*m_cZ.m_fY - m_cY.m_fY*m_cZ.m_fX);
	fD = ( fD > 0 ) ? 1.0F : -1.0F;

	oldPt.m_fX = (	cDir.m_fX*(m_cY.m_fY*m_cZ.m_fZ-m_cY.m_fZ*m_cZ.m_fY) +
					cDir.m_fY*(m_cY.m_fZ*m_cZ.m_fX-m_cY.m_fX*m_cZ.m_fZ) +
					cDir.m_fZ*(m_cY.m_fX*m_cZ.m_fY-m_cY.m_fY*m_cZ.m_fX) ) / m_fScale * fD; 

	oldPt.m_fY = (	cDir.m_fX*(m_cX.m_fZ*m_cZ.m_fY-m_cX.m_fY*m_cZ.m_fZ) +
					cDir.m_fY*(m_cX.m_fX*m_cZ.m_fZ-m_cX.m_fZ*m_cZ.m_fX) +
					cDir.m_fZ*(m_cX.m_fY*m_cZ.m_fX-m_cX.m_fX*m_cZ.m_fY) ) / m_fScale * fD; 

	oldPt.m_fZ = (	cDir.m_fX*(m_cX.m_fY*m_cY.m_fZ-m_cX.m_fZ*m_cY.m_fY) +
					cDir.m_fY*(m_cX.m_fZ*m_cY.m_fX-m_cX.m_fX*m_cY.m_fZ) +
					cDir.m_fZ*(m_cX.m_fX*m_cY.m_fY-m_cX.m_fY*m_cY.m_fX) ) / m_fScale * fD;
}


void CCAD_Coord3D::ChildToThis(CCAD_Coord3D* pOther) const
{
	ConvToBase(pOther->m_cOrg);
	ConvVectorToBase(pOther->m_cX);
	ConvVectorToBase(pOther->m_cY);
	ConvVectorToBase(pOther->m_cZ);
	pOther->m_fScale *= m_fScale;

	pOther->Normalize(); 
}

void CCAD_Coord3D::ThisToChild(CCAD_Coord3D* pOther) 
{
	ConvFromBase(pOther->m_cOrg);
	ConvVectorFromBase(pOther->m_cX);
	ConvVectorFromBase(pOther->m_cY);
	ConvVectorFromBase(pOther->m_cZ);
	pOther->m_fScale /= m_fScale;

	pOther->Normalize();
}

/***************************************************************
	DESCRIPT :	Rotate a Point around X axis.
			Mathematical Formula of Rotation based X-Axis
			{X,Y,Z)		: Old Point 
			{X',Y',Z'}	: New Point

			|X'| |1    0       0   ||X|
			|Y'|=|0  cos(R) -sin(R)||Y|
			|Z'| |0  sin(R)  cos(R)||Z|

****************************************************************/
void CCAD_Coord3D::XRotate(const CCAD_Angle& cAngle,	// rotate angle
						 CCAD_Vector& cPoint)		// Point to be rotated
{
	CAD_FLOAT fY = cPoint.m_fY;
	CAD_FLOAT fZ = cPoint.m_fZ;

	CAD_FLOAT fCos = cAngle.cos();
	CAD_FLOAT fSin = cAngle.sin();

	cPoint.m_fY = fY * fCos - fZ * fSin;
	cPoint.m_fZ = fY * fSin + fZ * fCos;
}

/***************************************************************
	DESCRIPT :	Rotate a Point around Y axis.
			Mathematical Formula of Rotation based Y-Axis
			{X,Y,Z)		: Old Point 
			{X',Y',Z'}	: New Point

			|X'| |cos(R)    0   sin(R)||X|
			|Y'|=|  0		1	 0	  ||Y|
			|Z'| |sin(R)	0	cos(R)||Z|
****************************************************************/
void CCAD_Coord3D::YRotate(const CCAD_Angle& cAngle,	// rotate angle
						 CCAD_Vector& cPoint)		// Point to be rotated
{

	CAD_FLOAT fZ = cPoint.m_fZ;
	CAD_FLOAT fX = cPoint.m_fX;

	CAD_FLOAT fCos = cAngle.cos();
	CAD_FLOAT fSin = cAngle.sin();

	cPoint.m_fZ = fZ * fCos - fX * fSin;
	cPoint.m_fX = fZ * fSin + fX * fCos;
}

/***************************************************************
	DESCRIPT :	Rotate a Point around Z axis.
			Mathematical Formula of Rotation based Z-Axis
			{X,Y,Z)		: Old Point
			{X',Y',Z'}	: New Point
			R			: Angle By Radian
			|X'| |cos(R)  -sin(R) 0||X|
			|Y'|=|sin(R)  cos(R)  0||Y|
			|Z'| |  0		0	  1||Z|
****************************************************************/
void CCAD_Coord3D::ZRotate(const CCAD_Angle& cAngle,	// rotate angle
						 CCAD_Vector& cPoint)		// Point to be rotated
{
	CAD_FLOAT fX = cPoint.m_fX;
	CAD_FLOAT fY = cPoint.m_fY;

	CAD_FLOAT fCos = cAngle.cos();
	CAD_FLOAT fSin = cAngle.sin();

	cPoint.m_fX = fX * fCos - fY * fSin;
	cPoint.m_fY = fX * fSin + fY * fCos;
}

void CCAD_Coord3D::XRotate(const CCAD_Angle& cAngle)
{

	CCAD_Vector cNewY((CAD_FLOAT)0.0, cAngle.cos(), cAngle.sin());
	CCAD_Vector cNewZ((CAD_FLOAT)1.0, (CAD_FLOAT)0.0, (CAD_FLOAT)0.0);
	cNewZ *= cNewY;

	ConvVectorToBase(cNewY);
	ConvVectorToBase(cNewZ);

	m_cY = cNewY;
	m_cZ = cNewZ;
	Normalize();
}

void CCAD_Coord3D::YRotate(const CCAD_Angle& cAngle)
{
	CCAD_Vector cNewZ(cAngle.sin(), (CAD_FLOAT)0.0, cAngle.cos());
	CCAD_Vector cNewX((CAD_FLOAT)0.0, (CAD_FLOAT)1.0, (CAD_FLOAT)0.0);
	cNewX *= cNewZ;	// X = Y * Z
	
	ConvVectorToBase(cNewX);
	ConvVectorToBase(cNewZ);

	m_cX = cNewX;
	m_cZ = cNewZ;
	Normalize();
}

void CCAD_Coord3D::ZRotate(const CCAD_Angle& cAngle)
{
	CCAD_Vector cNewX( cAngle.cos(), cAngle.sin(), (CAD_FLOAT)0.0);
	CCAD_Vector cNewY((CAD_FLOAT)0.0, (CAD_FLOAT)0.0, (CAD_FLOAT)1.0);
	cNewY *= cNewX;	// Y = X * Z
	
	ConvVectorToBase(cNewX);
	ConvVectorToBase(cNewY);

	m_cX = cNewX;
	m_cY = cNewY;
	Normalize();
}

void CCAD_Coord3D::XRotateChildCoord(const CCAD_Angle& cAngle, CCAD_Coord3D* pChild)
{
	BOOL	isMirror = pChild->IsMirrored();

	XRotate( cAngle, pChild->m_cOrg );
	XRotate( cAngle, pChild->m_cX );
	XRotate( cAngle, pChild->m_cY );
	
	if (!isMirror)
	{
		pChild->m_cZ = pChild->m_cX * pChild->m_cY;
		pChild->m_cX = pChild->m_cY * pChild->m_cZ;
	}
	else
	{
		pChild->m_cZ = pChild->m_cX * pChild->m_cY;
		pChild->m_cX = pChild->m_cY * pChild->m_cZ;
		pChild->m_cZ *= -1.0F;
	}
	
	pChild->Normalize(); 
	
}
void CCAD_Coord3D::YRotateChildCoord(const CCAD_Angle& cAngle, CCAD_Coord3D* pChild)
{
	BOOL	isMirror = pChild->IsMirrored();

	YRotate( cAngle, pChild->m_cOrg );
	YRotate( cAngle, pChild->m_cX );
	YRotate( cAngle, pChild->m_cY );

	if (!isMirror)
	{
		pChild->m_cZ = pChild->m_cX * pChild->m_cY;
		pChild->m_cX = pChild->m_cY * pChild->m_cZ;
	}
	else
	{
		pChild->m_cZ = pChild->m_cX * pChild->m_cY;
		pChild->m_cX = pChild->m_cY * pChild->m_cZ;
		pChild->m_cZ *= -1.0F;
	}
	
	pChild->Normalize(); 
}
void CCAD_Coord3D::ZRotateChildCoord(const CCAD_Angle& cAngle, CCAD_Coord3D* pChild)
{
	BOOL	isMirror = pChild->IsMirrored();

	ZRotate( cAngle, pChild->m_cOrg );
	ZRotate( cAngle, pChild->m_cX );
	ZRotate( cAngle, pChild->m_cY );

	if (!isMirror)
	{
		pChild->m_cZ = pChild->m_cX * pChild->m_cY;
		pChild->m_cX = pChild->m_cY * pChild->m_cZ;
	}
	else
	{
		pChild->m_cZ = pChild->m_cX * pChild->m_cY;
		pChild->m_cX = pChild->m_cY * pChild->m_cZ;
		pChild->m_cZ *= -1.0F;
	}
	
	pChild->Normalize(); 
}

void CCAD_Coord3D::XRotateEqualCoord(const CCAD_Angle& cAngle,
									 CCAD_Coord3D* pOther)

{
	if( pOther == NULL ) return;

	ThisToChild(pOther);
	XRotateChildCoord(cAngle, pOther);
	ChildToThis(pOther);
}

void CCAD_Coord3D::YRotateEqualCoord(const CCAD_Angle& cAngle,
									 CCAD_Coord3D* pOther)

{
	if( pOther == NULL ) return;

	ThisToChild(pOther);
	YRotateChildCoord(cAngle, pOther);
	ChildToThis(pOther);
}
void CCAD_Coord3D::ZRotateEqualCoord(const CCAD_Angle& cAngle,
									 CCAD_Coord3D* pOther)

{
	if( pOther == NULL ) return;

	ThisToChild(pOther);
	ZRotateChildCoord(cAngle, pOther);
	ChildToThis(pOther);
}

BOOL CCAD_Coord3D::IsParallelPlan(const CCAD_Coord3D & cCoord) const
{
	return (cCoord.m_cZ.m_fX == m_cZ.m_fX && cCoord.m_cZ.m_fY == m_cZ.m_fY && cCoord.m_cZ.m_fZ == m_cZ.m_fZ);
}

BOOL CCAD_Coord3D::IsEqualPlan(const CCAD_Coord3D & cCoord) const
{
	if (!IsParallelPlan(cCoord))
		return false;

	CCAD_Vector	cOp(m_cOrg);	// org in base
	cCoord.ConvFromBase(cOp);
	return (fabs(cOp.m_fZ) < DEFAULT_ACCURANCE);
}

BOOL CCAD_Coord3D::ConvertAngleFromCoord(const CCAD_Coord3D & cOrgCoord, CCAD_Angle & cAngle)
{
	if ( !IsEqualPlan(cOrgCoord) )
		return FALSE;

	CCAD_Vector	cDir(cOrgCoord.m_cX);
	cOrgCoord.ConvToBase( cDir );
	ConvFromBase(cDir);

	CCAD_Angle	cDelta;
	cDelta.SetFromXY(cDir.m_fX, cDir.m_fY);
	cAngle.RotateAngle(cDelta);
	return TRUE;
}

void CCAD_Coord3D::ConvToBase(CCAD_Rect& cRect) const
{
	SCAD_POINT	pt[8] = {
		{cRect.m_fMinX, cRect.m_fMinY, cRect.m_fMinZ}, 
		{cRect.m_fMaxX, cRect.m_fMinY, cRect.m_fMinZ},
		{cRect.m_fMaxX, cRect.m_fMaxY, cRect.m_fMinZ},
		{cRect.m_fMinX, cRect.m_fMaxY, cRect.m_fMinZ},
		{cRect.m_fMinX, cRect.m_fMinY, cRect.m_fMaxZ}, 
		{cRect.m_fMaxX, cRect.m_fMinY, cRect.m_fMaxZ},
		{cRect.m_fMaxX, cRect.m_fMaxY, cRect.m_fMaxZ},
		{cRect.m_fMinX, cRect.m_fMaxY, cRect.m_fMaxZ}};

	cRect.Init();
	CCAD_Point	ptTemp;
	int i;
	for (i = 0; i < 8; i++)	{
		ptTemp = pt[i];
		ConvToBase(ptTemp);
		cRect.ResetParameter(ptTemp);
	}
}

void CCAD_Coord3D::ConvFromBase(CCAD_Rect& cRect) const
{
	SCAD_POINT	pt[8] = {
		{cRect.m_fMinX, cRect.m_fMinY, cRect.m_fMinZ}, 
		{cRect.m_fMaxX, cRect.m_fMinY, cRect.m_fMinZ},
		{cRect.m_fMaxX, cRect.m_fMaxY, cRect.m_fMinZ},
		{cRect.m_fMinX, cRect.m_fMaxY, cRect.m_fMinZ},
		{cRect.m_fMinX, cRect.m_fMinY, cRect.m_fMaxZ}, 
		{cRect.m_fMaxX, cRect.m_fMinY, cRect.m_fMaxZ},
		{cRect.m_fMaxX, cRect.m_fMaxY, cRect.m_fMaxZ},
		{cRect.m_fMinX, cRect.m_fMaxY, cRect.m_fMaxZ}};

	cRect.Init();
	CCAD_Point	ptTemp;
	int i;
	for (i = 0; i < 8; i++)	{
		ptTemp = pt[i];
		ConvFromBase(ptTemp);
		cRect.ResetParameter(ptTemp);
	}
}

BOOL CCAD_Coord3D::GetRotatedAngle(CCAD_Angle& cXAngle,
						CCAD_Angle& cZAngle, CCAD_Angle& cYAngle)	const
{
	CAD_FLOAT fYyz = (CAD_FLOAT)sqrt(m_cY.m_fY * m_cY.m_fY + m_cY.m_fZ * m_cY.m_fZ);
	
	// X Angle
	cXAngle.SetFromXY(m_cY.m_fY, m_cY.m_fZ);
	// Z Angle
	cZAngle.SetFromXY(fYyz, -m_cY.m_fX);

	// Y Angle
	CCAD_Coord3D tempCoord;
	tempCoord.XRotate(cXAngle);
	tempCoord.ZRotate(cZAngle);

	CCAD_Coord3D thisCoord = *this;
	thisCoord.ThisToChild(&tempCoord);
	cYAngle.SetFromXY(tempCoord.m_cX.m_fX, tempCoord.m_cX.m_fZ);// 1, 4 patition

	return TRUE;

}

BOOL CCAD_Coord3D::GetRotatedX(CCAD_Angle& cAngle) const
{
	cAngle.SetFromXY(m_cY.m_fY, m_cY.m_fZ);
	return m_cX.EqualX(CCAD_Vector(1.0, 0.0, 0.0));
}

BOOL CCAD_Coord3D::GetRotatedY(CCAD_Angle& cAngle) const
{
	cAngle.SetFromXY(m_cZ.m_fZ, m_cZ.m_fX);
	return m_cY.EqualY(CCAD_Vector(0.0, 1.0, 0.0));
}

BOOL CCAD_Coord3D::GetRotatedZ(CCAD_Angle& cAngle) const
{
	cAngle.SetFromXY(m_cX.m_fX, m_cX.m_fY);
	return m_cZ.EqualZ(CCAD_Vector(0.0, 0.0, 1.0));
}

void CCAD_Coord3D::MirrorSelf()
{
	m_cY.m_fX = -m_cY.m_fX;
	m_cY.m_fY = -m_cY.m_fY;
	m_cY.m_fZ = -m_cY.m_fZ;
}

BOOL CCAD_Coord3D::IsMirrored() const
{
	CCAD_Vector cVp = m_cX * m_cY;

	if ( cVp.CountLength(m_cZ) > 1 )
		return true;
	return false;
}

void CCAD_Coord3D::MirrorCoord(CAD_FLOAT fScaleX, CAD_FLOAT fScaleY, CAD_FLOAT fScaleZ)
{
	if ( fScaleX < 0 ) {
		m_cX.m_fX = -m_cX.m_fX;
		m_cX.m_fY = -m_cX.m_fY;
		m_cX.m_fZ = -m_cX.m_fZ;
	}
	if ( fScaleY < 0 ) {
		m_cY.m_fX = -m_cY.m_fX;
		m_cY.m_fY = -m_cY.m_fY;
		m_cY.m_fZ = -m_cY.m_fZ;
	}
	if ( fScaleZ < 0 ) {
		m_cZ.m_fX = -m_cZ.m_fX;
		m_cZ.m_fY = -m_cZ.m_fY;
		m_cZ.m_fZ = -m_cZ.m_fZ;
	}
}

void CCAD_Coord3D::GetMatrixFromBase(float *mat)
{
	float fD = m_cX.m_fX * (m_cY.m_fY * m_cZ.m_fZ - m_cY.m_fZ * m_cZ.m_fY)
				+ m_cX.m_fY * (m_cY.m_fZ * m_cZ.m_fX - m_cY.m_fX * m_cZ.m_fZ)
				+ m_cX.m_fZ * (m_cY.m_fX * m_cZ.m_fY - m_cY.m_fY * m_cZ.m_fX);
	fD = ( fD > 0 ) ? 1.0F : -1.0F;
	mat[0] = (m_cY.m_fY * m_cZ.m_fZ - m_cY.m_fZ * m_cZ.m_fY) / m_fScale * fD;
	mat[1] = (m_cY.m_fZ * m_cZ.m_fX - m_cY.m_fX * m_cZ.m_fZ) / m_fScale * fD;
	mat[2] = (m_cY.m_fX * m_cZ.m_fY - m_cY.m_fY * m_cZ.m_fX) / m_fScale * fD;
	mat[3] = -(m_cOrg.m_fX * (m_cY.m_fY * m_cZ.m_fZ - m_cY.m_fZ * m_cZ.m_fY) +
				m_cOrg.m_fY * (m_cY.m_fZ * m_cZ.m_fX - m_cY.m_fX * m_cZ.m_fZ) +
				m_cOrg.m_fZ * (m_cY.m_fX * m_cZ.m_fY - m_cY.m_fY * m_cZ.m_fX) ) / m_fScale * fD;
	mat[4] = (m_cX.m_fZ * m_cZ.m_fY - m_cX.m_fY * m_cZ.m_fZ) / m_fScale * fD;
	mat[5] = (m_cX.m_fX * m_cZ.m_fZ - m_cX.m_fZ * m_cZ.m_fX) / m_fScale * fD;
	mat[6] = (m_cX.m_fY * m_cZ.m_fX - m_cX.m_fX * m_cZ.m_fY) / m_fScale * fD;
	mat[7] = -(m_cOrg.m_fX * (m_cX.m_fZ * m_cZ.m_fY - m_cX.m_fY * m_cZ.m_fZ) +
				m_cOrg.m_fY * (m_cX.m_fX * m_cZ.m_fZ - m_cX.m_fZ * m_cZ.m_fX) +
				m_cOrg.m_fZ * (m_cX.m_fY * m_cZ.m_fX - m_cX.m_fX * m_cZ.m_fY) ) / m_fScale * fD; 
	mat[8] = (m_cX.m_fY * m_cY.m_fZ - m_cX.m_fZ * m_cY.m_fY) / m_fScale * fD;
	mat[9] = (m_cX.m_fZ * m_cY.m_fX - m_cX.m_fX * m_cY.m_fZ) / m_fScale * fD;
	mat[10] = (m_cX.m_fX * m_cY.m_fY - m_cX.m_fY * m_cY.m_fX) / m_fScale * fD;
	mat[11] = -(m_cOrg.m_fX * (m_cX.m_fY * m_cY.m_fZ - m_cX.m_fZ * m_cY.m_fY) +
				m_cOrg.m_fY * (m_cX.m_fZ * m_cY.m_fX - m_cX.m_fX * m_cY.m_fZ) +
				m_cOrg.m_fZ * (m_cX.m_fX * m_cY.m_fY - m_cX.m_fY * m_cY.m_fX) ) / m_fScale * fD;
	mat[12] = 0.0f;
	mat[13] = 0.0f;
	mat[14] = 0.0f;
	mat[15] = 1.0f;
}

void CCAD_Coord3D::GetMatrixToBase(float *mat)
{
	mat[0] = m_cX.m_fX * m_fScale;
	mat[1] = m_cY.m_fX * m_fScale;
	mat[2] = m_cZ.m_fX * m_fScale;
	mat[3] = m_cOrg.m_fX;

	mat[4] = m_cX.m_fY * m_fScale;
	mat[5] = m_cY.m_fY * m_fScale;
	mat[6] = m_cZ.m_fY * m_fScale;
	mat[7] = m_cOrg.m_fY;

	mat[8] = m_cX.m_fZ * m_fScale;
	mat[9] = m_cY.m_fZ * m_fScale;
	mat[10] = m_cZ.m_fZ * m_fScale;
	mat[11] = m_cOrg.m_fZ;

	mat[12] = 0.0f;
	mat[13] = 0.0f;
	mat[14] = 0.0f;
	mat[15] = 1.0f;
}

void CCAD_Coord3D::GetMulMatrix(float *mat, float fScale)
{
	mat[0] *= fScale;
	mat[1] *= fScale;
	mat[2] *= fScale;

	mat[4] *= fScale;
	mat[5] *= fScale;
	mat[6] *= fScale;

	mat[8] *= fScale;
	mat[9] *= fScale;
	mat[10] *= fScale;
}

