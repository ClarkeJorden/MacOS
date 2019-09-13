#include "stdafx.h"
#include "math.h"
#include "CAD_Constance.h"
#include "CAD_Angle.h"
#include "CAD_Vector.h"
#include "CAD_Parabola2D.h"

 

CCAD_Parabola2D::CCAD_Parabola2D()
{
	m_fA = m_fB = m_fC = m_fK = m_fZ = 0;
}

void CCAD_Parabola2D::SetPara(const CCAD_Vector & cPt, const CAD_FLOAT & dHy)
{
	if (cPt.m_fY == dHy) {
		m_fA = 0;
		m_fB = 1;
		m_fC = 0;
		m_fK = cPt.m_fX;
	}
	else {
		m_fA = 1;
		m_fB = -2*cPt.m_fX;
		m_fC = 2*(dHy-cPt.m_fY);
		m_fK = dHy*dHy - cPt.m_fY*cPt.m_fY - cPt.m_fX*cPt.m_fX;
	}
	m_fZ = cPt.m_fZ;
}

void CCAD_Parabola2D::SetPara(const CCAD_Vector & cCp, const CAD_FLOAT & dR, const CAD_FLOAT & dHy, BYTE bPositive)
{
	if (dR == fabs(dHy-cCp.m_fY)) {
		m_fA = 0;
		m_fB = 1;
		m_fC = 0;
		m_fK = cCp.m_fX;
	}
	else if (bPositive == YES) {
		m_fA = 1;
		m_fB = -2*cCp.m_fX;
		m_fC = 2*(dHy-cCp.m_fY-dR);
		m_fK = dHy*dHy + dR*dR - cCp.m_fX*cCp.m_fX - cCp.m_fY*cCp.m_fY - 2*dR*dHy;
	}
	else {
		m_fA = 1;
		m_fB = -2*cCp.m_fX;
		m_fC = 2*(dHy-cCp.m_fY+dR);
		m_fK = dHy*dHy + dR*dR - cCp.m_fX*cCp.m_fX - cCp.m_fY*cCp.m_fY + 2*dR*dHy;
	}
	m_fZ = cCp.m_fZ;
}

void CCAD_Parabola2D::CountPt(CCAD_Vector & cPt, const CAD_FLOAT & dX) const
{
	cPt.m_fX = dX;
	cPt.m_fY = (m_fK - m_fA*dX*dX - m_fB*dX)/m_fC;
	cPt.m_fZ = m_fZ;
}

 
