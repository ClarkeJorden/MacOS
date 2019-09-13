#include "stdafx.h"
#include "CAD_Camera.h"
#include "CAD_Global.h"
//#include "math.h"

CCAD_Camera::CCAD_Camera()
{
	Init(CAMERA_WIDTH, CAMERA_HEIGHT);
	m_cOrg.m_fZ = m_fDistToEye;
	m_bPerspect = false;
}

CCAD_Camera::~CCAD_Camera()
{
}

void CCAD_Camera::Init(CAD_FLOAT fWidth, CAD_FLOAT fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	CAD_FLOAT fMaxLen = (fWidth > fHeight) ? fWidth : fHeight;

	m_fDistToEye = fMaxLen * (CAD_FLOAT)0.866; // cos(PAI/6) // fovy = 60

	m_fNear = CAD_ZERO;
	m_fFar = m_fDistToEye + fMaxLen * (CAD_FLOAT)2.0;

	CreateClipPlan();
	GetMat();
}

void	CCAD_Camera::InitCamera()
{
	Init(CAMERA_WIDTH, CAMERA_HEIGHT);
	m_cOrg.m_fZ = m_fDistToEye;
	m_bPerspect = false;

	m_cX = CCAD_Vector(1, 0, 0);
	m_cY = CCAD_Vector(0, 1, 0);
	m_cZ = CCAD_Vector(0, 0, 1);
	GetMat();
}

CCAD_Camera::CCAD_Camera(const CCAD_Camera& cOther)
{
	*this = cOther;
}

void CCAD_Camera::operator = (const CCAD_Camera & cOther)
{

	*((CCAD_Coord3D*)this) = (CCAD_Coord3D&)cOther;

	m_fDistToEye = cOther.m_fDistToEye;
	m_fWidth = cOther.m_fWidth;
	m_fHeight = cOther.m_fHeight;
	m_fNear = cOther.m_fNear;
	m_fFar = cOther.m_fFar;
	m_bPerspect = cOther.m_bPerspect;

	m_cLeftClipPlan = cOther.m_cLeftClipPlan;
	m_cRightClipPlan = cOther.m_cRightClipPlan;
	m_cTopClipPlan = cOther.m_cTopClipPlan;
	m_cBottomClipPlan = cOther.m_cBottomClipPlan;

}

BOOL CCAD_Camera::operator == (const CCAD_Camera & cOther)
{
	if (m_fDistToEye != cOther.m_fDistToEye || 
		m_fWidth != cOther.m_fWidth ||
		m_fHeight != cOther.m_fHeight || 
		m_fNear != cOther.m_fNear ||
		m_fFar != cOther.m_fFar || 
		m_bPerspect != cOther.m_bPerspect || 
		m_cX != cOther.m_cX ||
		m_cY != cOther.m_cY ||
		m_cZ != cOther.m_cZ ||
		m_cOrg.m_fX != cOther.m_cOrg.m_fX || 
		m_cOrg.m_fY != cOther.m_cOrg.m_fY ||
		m_cOrg.m_fZ != cOther.m_cOrg.m_fZ)
		return false;

	return true;
}

CAD_FLOAT	CCAD_Camera::GetFovyH()
{
	
	double x = sqrt(m_fDistToEye * m_fDistToEye + m_fHeight * m_fHeight / 4.0);
	double y = m_fWidth / 2.0;

	return (CAD_FLOAT)(2 * 180.0 / PAI * atan2( y, x ));

}

CAD_FLOAT CCAD_Camera::GetFovyV()
{
	double x,y;
	x = sqrt(m_fDistToEye * m_fDistToEye + m_fWidth * m_fWidth / 4.0);
	y = m_fHeight / 2.0;

	return (CAD_FLOAT)(2 * 180.0 / PAI * atan2( y, x ));
}

void CCAD_Camera::MoveVerti(CAD_FLOAT fDelta)
{
	CCAD_Vector newOrg(0, fDelta, 0);
	ConvToBase(newOrg);

	if( !CCAD_Global::IsOverFloat(newOrg.m_fX) )
		m_cOrg.m_fX = newOrg.m_fX;

	if( !CCAD_Global::IsOverFloat(newOrg.m_fY) )
		m_cOrg.m_fY = newOrg.m_fY;

	if( !CCAD_Global::IsOverFloat(newOrg.m_fZ) )
		m_cOrg.m_fZ = newOrg.m_fZ;

	GetMat();
}

void CCAD_Camera::MoveHoriz(CAD_FLOAT fDelta)
{
	CCAD_Vector newOrg(fDelta, 0, 0);
	ConvToBase(newOrg);
	if( !CCAD_Global::IsOverFloat(newOrg.m_fX) )
		m_cOrg.m_fX = newOrg.m_fX;

	if( !CCAD_Global::IsOverFloat(newOrg.m_fY) )
		m_cOrg.m_fY = newOrg.m_fY;

	if( !CCAD_Global::IsOverFloat(newOrg.m_fZ) )
		m_cOrg.m_fZ = newOrg.m_fZ;
	GetMat();
}

void CCAD_Camera::MoveNearFar(CAD_FLOAT fDelta)
{
	CCAD_Vector newOrg(0, 0, - fDelta);
	ConvToBase(newOrg);

	if( !CCAD_Global::IsOverFloat(newOrg.m_fX) )
		m_cOrg.m_fX = newOrg.m_fX;

	if( !CCAD_Global::IsOverFloat(newOrg.m_fY) )
		m_cOrg.m_fY = newOrg.m_fY;

	if( !CCAD_Global::IsOverFloat(newOrg.m_fZ) )
		m_cOrg.m_fZ = newOrg.m_fZ;
	GetMat();
}

void CCAD_Camera::MoveUpDown(CAD_FLOAT fDelta)
{
	if( !CCAD_Global::IsOverFloat(m_cOrg.m_fZ + fDelta) )
		m_cOrg.m_fZ += fDelta;

/*	if( m_cOrg.m_fZ < 0 )
		m_cOrg.m_fZ = (CAD_FLOAT)0;
*/
	GetMat();
}

void CCAD_Camera::MoveForward(CAD_FLOAT fDelta)
{
	CCAD_Vector cDirect = m_cY;
	cDirect -= m_cZ;		// m_cY + m_cZ
	cDirect.Normalize();
	
	CAD_FLOAT xDelta = cDirect.m_fX * fDelta;
	CAD_FLOAT yDelta = cDirect.m_fY * fDelta;

	if( !CCAD_Global::IsOverFloat(m_cOrg.m_fX + fDelta) )
		m_cOrg.m_fX += xDelta;

	if( !CCAD_Global::IsOverFloat(m_cOrg.m_fY + fDelta) )
		m_cOrg.m_fY += yDelta;
	GetMat();
}

void CCAD_Camera::RotateHoriz(const CCAD_Angle& angle)
{
	CCAD_Coord3D	coordTemp;
	coordTemp.m_cOrg = m_cOrg;
	coordTemp.ZRotateEqualCoord(angle, (CCAD_Coord3D*)this);
	GetMat();
}

void CCAD_Camera::RotateVerti(const CCAD_Angle& angle)
{
	XRotate(angle);
	GetMat();
}

void CCAD_Camera::SetVCSOrg(const SCAD_POINT& cOrgPt)
{
	CCAD_Coord3D tempCoord = *((CCAD_Coord3D*)this);
	tempCoord.m_cOrg = cOrgPt;
	SCAD_POINT sPtNewOrg = {0,0,this->m_fDistToEye};
	tempCoord.ConvToBase(sPtNewOrg);
	m_cOrg = sPtNewOrg;
	GetMat();
}

void CCAD_Camera::RotateHorizFocusPoint(const CCAD_Point& cFocusPt, const CCAD_Angle& cAngle)
{
	CCAD_Point	focusPt = cFocusPt;
	ConvToBase(focusPt);

	CCAD_Coord3D	coordTemp;
	coordTemp.m_cOrg = focusPt;
	coordTemp.ZRotateEqualCoord(cAngle, (CCAD_Coord3D*)this);
	GetMat();
}

void CCAD_Camera::RotateVertiFocusPoint(const CCAD_Point& cFocusPt, const CCAD_Angle& cAngle)
{
	CCAD_Point	focusPt = cFocusPt;
	ConvToBase(focusPt);

	CCAD_Coord3D	coordTemp = *((CCAD_Coord3D*)this);
	coordTemp.m_cOrg = focusPt;
	coordTemp.XRotateEqualCoord(cAngle, (CCAD_Coord3D*)this);
	GetMat();
}

void CCAD_Camera::SetProjectPlan(const SCAD_POINT& sFocusPt)
{
	CCAD_Point pt = sFocusPt;
	CAD_FLOAT fDelta = -(pt.m_fZ + m_fDistToEye);
	MoveNearFar(fDelta);
	
}

BOOL CCAD_Camera::IsInternalPt(const CCAD_Point& cPt)
{
//	CAD_FLOAT fNearZ = m_fDistToEye - m_fNear;
	CAD_FLOAT fNearZ = - m_fNear;

	if( m_bPerspect ){

		if( cPt.m_fZ > fNearZ )
			return false;

		if( !m_cLeftClipPlan.IsFront(cPt) )
			return false;

		if( !m_cRightClipPlan.IsFront(cPt) )
			return false;
		
		if( !m_cTopClipPlan.IsFront(cPt) )
			return false;
		
		if( !m_cBottomClipPlan.IsFront(cPt) )
			return false;

	}else{ // parallel project

		CAD_FLOAT fMaxX = m_fWidth / (CAD_FLOAT)2.0;
		CAD_FLOAT fMinX = - fMaxX;
		CAD_FLOAT fMaxY =  m_fHeight / (CAD_FLOAT)2.0;
		CAD_FLOAT fMinY = - fMaxY;
		
		if( cPt.m_fX > fMaxX || cPt.m_fX < fMinX )
			return false;
		
		if( cPt.m_fY > fMaxY || cPt.m_fY < fMinY )
			return false;

	}

	return true;
}

BOOL CCAD_Camera::ClipPyramid(CCAD_Point& cPt1, CCAD_Point& cPt2, 
							  BOOL blNearClip, 
							  BOOL blFarClip )
{

	if( blNearClip){
		if( !ClipNear(cPt1, cPt2) )
		return false;
	}

	if( !ClipLeft(cPt1, cPt2) )
		return false;

	if( !ClipRight(cPt1, cPt2) )
		return false;

	if( !ClipTop(cPt1, cPt2) )
		return false;

	if( !ClipBottom(cPt1, cPt2) )
		return false;

	if( blFarClip ){
		if( !ClipFar(cPt1, cPt2) )
			return false;
	}

	return true;
	
}

BOOL CCAD_Camera::ClipLeft(CCAD_Point& cPt1, CCAD_Point& cPt2)
{
	if(m_bPerspect){
		return m_cLeftClipPlan.ClipLine(cPt1,cPt2,FRONT_DIRECTION);
	}else{
		CCAD_Vector temp = m_cLeftClipPlan.m_cNormal;
		float fd = m_cLeftClipPlan.m_fD;

		m_cLeftClipPlan.m_cNormal.m_fX = 1.0f;
		m_cLeftClipPlan.m_cNormal.m_fY = 0.0f;
		m_cLeftClipPlan.m_cNormal.m_fZ = 0.0f;
		m_cLeftClipPlan.m_fD = m_fWidth / 2.0f;
		BOOL ret = m_cLeftClipPlan.ClipLine(cPt1,cPt2,FRONT_DIRECTION);

		m_cLeftClipPlan.m_cNormal = temp;
		m_cLeftClipPlan.m_fD = fd;
		return ret;
	}
	
}

BOOL CCAD_Camera::ClipRight(CCAD_Point& cPt1, CCAD_Point& cPt2)
{
	if(m_bPerspect){
		return m_cRightClipPlan.ClipLine(cPt1,cPt2,FRONT_DIRECTION);
	}else{
		CCAD_Vector temp = m_cRightClipPlan.m_cNormal;
		float fd = m_cRightClipPlan.m_fD;

		m_cRightClipPlan.m_cNormal.m_fX = -1.0f;
		m_cRightClipPlan.m_cNormal.m_fY = 0.0f;
		m_cRightClipPlan.m_cNormal.m_fZ = 0.0f;
		m_cRightClipPlan.m_fD = m_fWidth / 2.0f;
		BOOL ret = m_cRightClipPlan.ClipLine(cPt1,cPt2,FRONT_DIRECTION);

		m_cRightClipPlan.m_cNormal = temp;
		m_cRightClipPlan.m_fD = fd;
		return ret;
	}
}

BOOL CCAD_Camera::ClipTop(CCAD_Point& cPt1, CCAD_Point& cPt2)
{
	if(m_bPerspect){
		return m_cTopClipPlan.ClipLine(cPt1,cPt2,FRONT_DIRECTION);
	}else{
		CCAD_Vector temp = m_cTopClipPlan.m_cNormal;
		float fd = m_cTopClipPlan.m_fD;

		m_cTopClipPlan.m_cNormal.m_fX = 0.0f;
		m_cTopClipPlan.m_cNormal.m_fY = -1.0f;
		m_cTopClipPlan.m_cNormal.m_fZ = 0.0f;
		m_cTopClipPlan.m_fD = m_fHeight / 2.0f;
		BOOL ret = m_cTopClipPlan.ClipLine(cPt1,cPt2,FRONT_DIRECTION);

		m_cTopClipPlan.m_cNormal = temp;
		m_cTopClipPlan.m_fD = fd;
		return ret;
	}
}

BOOL CCAD_Camera::ClipBottom(CCAD_Point& cPt1, CCAD_Point& cPt2)
{
	if(m_bPerspect){
		return m_cBottomClipPlan.ClipLine(cPt1,cPt2,FRONT_DIRECTION);
	}else{
		CCAD_Vector temp = m_cBottomClipPlan.m_cNormal;
		float fd = m_cBottomClipPlan.m_fD;

		m_cBottomClipPlan.m_cNormal.m_fX = 0.0f;
		m_cBottomClipPlan.m_cNormal.m_fY = 1.0f;
		m_cBottomClipPlan.m_cNormal.m_fZ = 0.0f;
		m_cBottomClipPlan.m_fD = m_fHeight / 2.0f;
		BOOL ret = m_cBottomClipPlan.ClipLine(cPt1,cPt2,FRONT_DIRECTION);

		m_cBottomClipPlan.m_cNormal = temp;
		m_cBottomClipPlan.m_fD = fd;
		return ret;
	}
}

BOOL CCAD_Camera::ClipNear(CCAD_Point& cPt1, CCAD_Point& cPt2)
{
//	CAD_FLOAT fNearZ = m_fDistToEye - m_fNear;
	CAD_FLOAT fNearZ = - m_fNear;
	if(fNearZ > -CAMERA_MIN_NEAR) fNearZ = -CAMERA_MIN_NEAR;
	
	if( cPt1.m_fZ > fNearZ && cPt2.m_fZ > fNearZ )
		return false;
	
	if( cPt1.m_fZ <= fNearZ && cPt2.m_fZ <= fNearZ )
		return true;

	CAD_FLOAT fDeltaZ = cPt2.m_fZ - cPt1.m_fZ;
	CAD_FLOAT fT = fNearZ - cPt1.m_fZ;

	CAD_FLOAT fCrossY = (cPt2.m_fY - cPt1.m_fY) * fT / fDeltaZ + cPt1.m_fY;
	CAD_FLOAT fCrossX = (cPt2.m_fX - cPt1.m_fX) * fT / fDeltaZ + cPt1.m_fX;

	if( cPt1.m_fZ > fNearZ ){

		cPt1.m_fZ = fNearZ;
		cPt1.m_fY = fCrossY;
		cPt1.m_fX = fCrossX;

	}else{

		cPt2.m_fZ = fNearZ;
		cPt2.m_fY = fCrossY;
		cPt2.m_fX = fCrossX;

	}

	return true;
}

BOOL CCAD_Camera::ClipFar(CCAD_Point& cPt1, CCAD_Point& cPt2)
{
//	CAD_FLOAT fFarZ = m_fDistToEye - m_fFar;
	CAD_FLOAT fFarZ = - m_fFar;
	
	if( cPt1.m_fZ < fFarZ && cPt2.m_fZ < fFarZ )
		return false;
	
	if( cPt1.m_fZ >= fFarZ && cPt2.m_fZ >= fFarZ )
		return true;

	CAD_FLOAT fDeltaZ = cPt2.m_fZ - cPt1.m_fZ;
	CAD_FLOAT fT = fFarZ - cPt1.m_fZ;

	CAD_FLOAT fCrossY = (cPt2.m_fY - cPt1.m_fY) * fT / fDeltaZ + cPt1.m_fY;
	CAD_FLOAT fCrossX = (cPt2.m_fX - cPt1.m_fX) * fT / fDeltaZ + cPt1.m_fX;

	if( cPt1.m_fZ < fFarZ ){

		cPt1.m_fZ = fFarZ;
		cPt1.m_fY = fCrossY;
		cPt1.m_fX = fCrossX;

	}else{

		cPt2.m_fZ = fFarZ;
		cPt2.m_fY = fCrossY;
		cPt2.m_fX = fCrossX;

	}

	return true;
}

SCAD_POINT& CCAD_Camera::ECStoVCS(SCAD_POINT& cPt)
{
	
	if( m_bPerspect ){ 

		CAD_FLOAT fDeltaZ = - m_fDistToEye - cPt.m_fZ;
	
		CAD_FLOAT fT = fDeltaZ / cPt.m_fZ;

		CAD_FLOAT fCrossY = cPt.m_fY * fT + cPt.m_fY;
		CAD_FLOAT fCrossX = cPt.m_fX * fT + cPt.m_fX;

		cPt.m_fX = fCrossX;
		cPt.m_fY = fCrossY;
	}

	cPt.m_fZ = - m_fDistToEye;

	return cPt;
}

void CCAD_Camera::CreateClipPlan()
{
	CAD_FLOAT h = m_fHeight / (CAD_FLOAT)2.0;
	CAD_FLOAT w = m_fWidth / (CAD_FLOAT)2.0;

	CCAD_Point	ptEYE(0,0,0);
	
	CCAD_Point	ptLT(-w, h, -m_fDistToEye);
	CCAD_Point	ptLB(-w,-h, -m_fDistToEye);
	CCAD_Point	ptRT(w, h, -m_fDistToEye);
	CCAD_Point	ptRB(w,-h, -m_fDistToEye);

	m_cLeftClipPlan.Create(ptEYE, ptLB, ptLT );
	m_cRightClipPlan.Create(ptEYE, ptRT, ptRB );
	m_cTopClipPlan.Create(ptEYE, ptLT, ptRT );
	m_cBottomClipPlan.Create(ptEYE, ptRB, ptLB );

}

void CCAD_Camera::Write(CCAD_File & cFile)
{
	m_cOrg.Write(cFile);
	m_cX.Write(cFile);
	m_cY.Write(cFile);
	m_cZ.Write(cFile);

	cFile.WriteCadFloat(m_fDistToEye);
	cFile.WriteCadFloat(m_fWidth);
	cFile.WriteCadFloat(m_fHeight);
	cFile.WriteCadFloat(m_fNear);
	cFile.WriteCadFloat(m_fFar);
	BYTE		bPerspect = 0;
	if (m_bPerspect == TRUE)
		bPerspect = 1;
	cFile.Write(bPerspect);
}

void CCAD_Camera::Read(CCAD_File & cFile)
{
	m_cOrg.Read(cFile);
	m_cX.Read(cFile);
	m_cY.Read(cFile);
	m_cZ.Read(cFile);

	cFile.ReadCAD_Float(&m_fDistToEye);
	cFile.ReadCAD_Float(&m_fWidth);
	cFile.ReadCAD_Float(&m_fHeight);
	cFile.ReadCAD_Float(&m_fNear);
	cFile.ReadCAD_Float(&m_fFar);
	
	BYTE	bPerspect = 0;
	cFile.ReadByte(&bPerspect);
 	if (bPerspect == 1)
 		m_bPerspect = TRUE;
 	else
 		m_bPerspect = FALSE;
//	m_cX = CCAD_Vector(1, 0, 0);
//	m_cY = CCAD_Vector(0, 1, 0);
//	m_cZ = CCAD_Vector(0, 0, 1);
//	m_cOrg.m_fZ = m_fDistToEye;
//	m_bPerspect = FALSE;
	CreateClipPlan();
}

BOOL CCAD_Camera::IsEqualCamera(CCAD_Camera* pOther)
{
	if (pOther == NULL)
		return FALSE;
	
	BOOL bEqual = FALSE;
	
	float scalarX = m_cX.m_fX * pOther->m_cX.m_fX + m_cX.m_fY * pOther->m_cX.m_fY + m_cX.m_fZ * pOther->m_cX.m_fZ;
//	float scalarY = m_cY.m_fX * pOther->m_cY.m_fX + m_cY.m_fY * pOther->m_cY.m_fY + m_cY.m_fZ * pOther->m_cY.m_fZ;
//	float scalarZ = m_cZ.m_fX * pOther->m_cZ.m_fX + m_cZ.m_fY * pOther->m_cZ.m_fY + m_cZ.m_fZ * pOther->m_cZ.m_fZ;
	if((fabs(scalarX - 1.0f) < 1e-4) && (fabs(scalarX - 1.0f) < 1e-4) && (fabs(scalarX - 1.0f) < 1e-4))
	{
		if((fabs(m_cOrg.m_fX - pOther->m_cOrg.m_fX) < 0.5f) &&
			(fabs(m_cOrg.m_fY - pOther->m_cOrg.m_fY) < 0.5f) &&
			(fabs(m_cOrg.m_fZ - pOther->m_cOrg.m_fZ) < 0.5f))
			bEqual = TRUE;
	}
	
	return bEqual;
}

BOOL CCAD_Camera::SetVcsSize( CAD_FLOAT fWidth /*= CAMERA_WIDTH*/, CAD_FLOAT fHeight /*= CAMERA_HEIGHT*/ )
{
	fWidth = (CAD_FLOAT)fabs(fWidth);
	fHeight = (CAD_FLOAT)fabs(fHeight);

	if(IS_ZERO(m_fWidth - fWidth) && IS_ZERO(m_fHeight - fHeight))
		return false;

	// Set Width, Height
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	// Set Near, Far
	CAD_FLOAT fMaxLen = (fWidth > fHeight) ? fWidth : fHeight;

	m_fNear = 1 /*CAD_ZERO*/;
	m_fFar = m_fDistToEye + fMaxLen * (CAD_FLOAT)2.0;

	// Create ViewPyramid
	CreateClipPlan();

	return true;
}

void CCAD_Camera::GetMat()
{
	GetMatrixFromBase(m_fModelViewMat);
}

 
