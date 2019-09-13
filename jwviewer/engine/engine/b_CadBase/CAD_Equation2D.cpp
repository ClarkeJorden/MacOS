

#include "stdafx.h"
//#include "math.h"
#include "CAD_Vector.h"
#include "CAD_Linear2D.h"
#include "CAD_Curve2D.h"
#include "CAD_Parabola2D.h"
#include "CAD_Equation2D.h"

 

CCAD_Equation2D::CCAD_Equation2D()
{
	m_fA = 0;
	m_fB = 0;
	m_fC = 0;
}

CCAD_Equation2D::CCAD_Equation2D(CAD_FLOAT & a, CAD_FLOAT & b, CAD_FLOAT & c)
{
	m_fA = a;
	m_fB = b;
	m_fC = c;
}

BOOL CCAD_Equation2D::SetPara(const CCAD_Curve2D & cCurve, BYTE bBase, const CAD_FLOAT & dV)
{
	m_bBase = bBase;
	if (bBase == X_BASE) {		// dV = y !
		m_fA = cCurve.m_fA;
		m_fB = (cCurve.m_fC * dV) + cCurve.m_fD;
		m_fC = (cCurve.m_fB * dV * dV) + (cCurve.m_fE * dV) - cCurve.m_fK;
	}
	else {						// dV = x !
		m_fA = cCurve.m_fB;
		m_fB = (cCurve.m_fC * dV) + cCurve.m_fE;
		m_fC = (cCurve.m_fA * dV * dV) + (cCurve.m_fD * dV) - cCurve.m_fK;
	}
	return (m_fA != 0 || m_fB != 0);
}

BOOL CCAD_Equation2D::SetPara(const CCAD_Curve2D & cCurve, const CCAD_Linear2D & cLinear)
{
	if (cLinear.m_fA == 0 && cLinear.m_fB == 0)
		return FALSE;
	if (cLinear.m_fA == 0) {
		CAD_FLOAT	dY = cLinear.m_fK / cLinear.m_fB;
		return SetPara(cCurve, X_BASE, dY);
	}
	else if (cLinear.m_fB == 0) {
		CAD_FLOAT	dX = cLinear.m_fK / cLinear.m_fA;
		return SetPara(cCurve, Y_BASE, dX);
	}
	else if (fabs(cLinear.m_fA) < fabs(cLinear.m_fB)) {
		m_bBase = X_BASE;
		CAD_FLOAT dAk = cLinear.m_fA / cLinear.m_fB;
		CAD_FLOAT dCk = cLinear.m_fK / cLinear.m_fB;
		m_fA = cCurve.m_fA + (dAk*dAk)*cCurve.m_fB - dAk*cCurve.m_fC;
		m_fB = cCurve.m_fD + dCk*cCurve.m_fC - 2*(dAk*dCk)*cCurve.m_fB - dAk*cCurve.m_fE;
		m_fC = (dCk*dCk)*cCurve.m_fB + dCk*cCurve.m_fE - cCurve.m_fK;
	}
	else {
		m_bBase = Y_BASE;
		CAD_FLOAT dBk = cLinear.m_fB / cLinear.m_fA;
		CAD_FLOAT dCk = cLinear.m_fK / cLinear.m_fA;
		m_fA = cCurve.m_fB + (dBk*dBk)*cCurve.m_fA - dBk*cCurve.m_fC;
		m_fB = cCurve.m_fE + dCk*cCurve.m_fC - 2*(dBk*dCk)*cCurve.m_fA - dBk*cCurve.m_fD;
		m_fC = (dCk*dCk)*cCurve.m_fA + dCk*cCurve.m_fD - cCurve.m_fK;
	}
	return (m_fA != 0 || m_fB != 0);
}

BOOL CCAD_Equation2D::SetPara(const CCAD_Parabola2D & cParabola, const CCAD_Linear2D & cLinear)
{
	if (cLinear.m_fA == 0 && cLinear.m_fB == 0)
		return FALSE;
	if (cLinear.m_fA == 0) {
		m_bBase = X_BASE;
		CAD_FLOAT	dY = cLinear.m_fK / cLinear.m_fB;
		m_fA = cParabola.m_fA;
		m_fB = cParabola.m_fB;
		m_fC = cParabola.m_fC*dY - cParabola.m_fK;
	}
	else if (cLinear.m_fB == 0) {
		m_bBase = Y_BASE;
		CAD_FLOAT	dX = cLinear.m_fK / cLinear.m_fA;
		m_fA = 0;
		m_fB = cParabola.m_fC;
		m_fC = cParabola.m_fA*dX*dX + cParabola.m_fB*dX - cParabola.m_fK;
	}
	else {
		m_bBase = X_BASE;
		m_fA = cParabola.m_fA;
		m_fB = cParabola.m_fB - cParabola.m_fC*cLinear.m_fA/cLinear.m_fB;
		m_fC = cParabola.m_fC*cLinear.m_fK/cLinear.m_fB - cParabola.m_fK;
	}
	return (m_fA != 0 || m_fB != 0);
}

BOOL CCAD_Equation2D::SetPara(const CCAD_Parabola2D & cPara1, const CCAD_Parabola2D & cPara2)
{
	m_bBase = X_BASE;
	m_fA = cPara2.m_fC * cPara1.m_fA - cPara1.m_fC * cPara2.m_fA;
	m_fB = cPara2.m_fC * cPara1.m_fB - cPara1.m_fC * cPara2.m_fB;
	m_fC = cPara1.m_fC * cPara2.m_fK - cPara2.m_fC * cPara1.m_fK;
	return (m_fA != 0 || m_fB != 0);
}

BYTE CCAD_Equation2D::CountRoot(CAD_FLOAT & dV1, CAD_FLOAT & dV2) const
{
	if (m_fA == 0) {
		if (m_fB == 0)
			return ROOT_NO;
		dV1 = dV2 = -m_fC / m_fB;
		return ROOT_ONE;
	}
	CAD_FLOAT	dRoot = (m_fB * m_fB) - (4 * m_fA * m_fC);
	if (dRoot < 0)
		return ROOT_NO;
	if (dRoot == 0) {
		dV1 = dV2 = -m_fB / (2*m_fA);
		return ROOT_ONE;
	}
	dV1 = (-m_fB + (CAD_FLOAT)sqrt(dRoot)) / (2*m_fA);
	dV2 = (-m_fB - (CAD_FLOAT)sqrt(dRoot)) / (2*m_fA);
	return ROOT_TWO;
}

 
