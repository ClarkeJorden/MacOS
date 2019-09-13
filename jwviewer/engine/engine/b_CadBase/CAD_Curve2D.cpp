#include "stdafx.h"
//#include "math.h"
#include "CAD_Angle.h"
#include "CAD_Vector.h"
#include "CAD_Curve2D.h"


 


CCAD_Curve2D::CCAD_Curve2D()
{
	m_fA = m_fB = m_fC = m_fD = m_fE = m_fK = 0;
}


void CCAD_Curve2D::SetPara(const CCAD_Vector & cCp, const CAD_FLOAT & dR)
{
	if (dR != 0) {
		m_fA = m_fB = 1;
		m_fC = 0;
		m_fD = -2 * cCp.m_fX;
		m_fE = -2 * cCp.m_fY;
		m_fK = (dR * dR) - (cCp.m_fX * cCp.m_fX) - (cCp.m_fY * cCp.m_fY);
	}
}

void CCAD_Curve2D::SetPara(const CCAD_Vector & cCp, const CAD_FLOAT & dRx, const CAD_FLOAT & dRy, const CCAD_Angle & inDirect)
{
	if (dRx == dRy)
		SetPara(cCp, dRx);
	else if (dRy == 0) {
		m_fA = 0;
		m_fB = 0;
		m_fC = 0;
		m_fD = inDirect.tan();
		m_fE = -1;
		m_fK = m_fD * cCp.m_fX - cCp.m_fY;
	}
	else if (dRx == 0) {
		CCAD_Angle tmp(inDirect);
		m_fA = 0;
		m_fB = 0;
		m_fC = 0;
		tmp.RotateAngle(HALF_ANGLE);
		m_fD = tmp.tan();
		m_fE = -1;
		m_fK = m_fD * cCp.m_fX - cCp.m_fY;
	}
	else {
		CAD_FLOAT	rX = 1 / (dRx * dRx);
		CAD_FLOAT	rY = 1 / (dRy * dRy);
		if (inDirect.Horizontal()) {
			m_fA = rX;
			m_fB = rY;
			m_fC = 0;
			m_fD = -2 * rX * cCp.m_fX;
			m_fE = -2 * rY * cCp.m_fY;
			m_fK = 1 - (rX * cCp.m_fX * cCp.m_fX) - (rY * cCp.m_fY * cCp.m_fY);
		}
		else if (inDirect.Vertical()) {
			m_fA = rY;
			m_fB = rX;
			m_fC = 0;
			m_fD = -2 * m_fB * cCp.m_fY;
			m_fE = -2 * m_fA * cCp.m_fX;
			m_fK = 1 - (rY * cCp.m_fX * cCp.m_fX) - (rX * cCp.m_fY * cCp.m_fY);
		}
		else {
			CAD_FLOAT	dSin = inDirect.sin();
			CAD_FLOAT	dCos = inDirect.cos();
			m_fC = 2 * dSin * dCos * (rX - rY);
			dSin = dSin * dSin;
			dCos = dCos * dCos;
			m_fA = dCos * rX + dSin * rY;
			m_fB = dSin * rX + dCos * rY;
			m_fD = (-2 * m_fA * cCp.m_fX) - (m_fC * cCp.m_fY);
			m_fE = (-2 * m_fB * cCp.m_fY) - (m_fC * cCp.m_fX);
			m_fK = 1 - (m_fA * cCp.m_fX * cCp.m_fX) - (m_fB * cCp.m_fY * cCp.m_fY) - (m_fC * cCp.m_fX * cCp.m_fY);
		}
	}
}

 
