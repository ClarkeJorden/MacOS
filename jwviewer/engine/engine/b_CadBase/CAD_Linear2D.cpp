
#include "stdafx.h"
#include "math.h"
#include "CAD_Angle.h"
#include "CAD_Vector.h"
#include "CAD_Linear2D.h"


 

#define			COUNTING_ZERO			0.000001

CCAD_Linear2D::CCAD_Linear2D()
{
	m_fA = 0;
	m_fB = 0;
	m_fK = 0;
	m_fZ = 0;
}

CCAD_Linear2D::CCAD_Linear2D(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2)
{
	SetPara(cPt1, cPt2);
}

CCAD_Linear2D::CCAD_Linear2D(const CCAD_Vector & cPt, const CCAD_Angle & cDirect)
{
	SetPara(cPt, cDirect);
}

void CCAD_Linear2D::SetPara(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2)
{
	CAD_FLOAT a = cPt1.m_fY - cPt2.m_fY;
	CAD_FLOAT b = cPt2.m_fX - cPt1.m_fX;
	if (a == 0 && b == 0)
	{
		m_fA = 0.0;
		m_fB = 0.0;
		m_fK = 0.0;
	}
	else if (fabs(a) > fabs(b))
	{
		m_fA = 1.0;
		m_fB = b / a;
		m_fK = cPt1.m_fX + m_fB * cPt1.m_fY;
	}
	else
	{
		m_fA = a / b;
		m_fB = 1.0;
		m_fK = m_fA * cPt1.m_fX + cPt1.m_fY;
	}
	//--------	old code
/*
	m_fA = cPt1.m_fY - cPt2.m_fY;
	m_fB = cPt2.m_fX - cPt1.m_fX;
	m_fK = (cPt1.m_fY * cPt2.m_fX) - (cPt1.m_fX * cPt2.m_fY);
*/
	//--------	End
	
	m_fZ = cPt1.m_fZ;
}

void CCAD_Linear2D::SetPara(const CCAD_Vector & cPt, const CCAD_Angle & cDirect)
{
	if (cDirect.Horizontal()) {
		m_fA = 0;
		m_fB = 1;
		m_fK = cPt.m_fY;
	}
	else if (cDirect.Vertical()) {
		m_fA = 1;
		m_fB = 0;
		m_fK = cPt.m_fX;
	}
	else {
		m_fA = cDirect.sin();
		m_fB = -cDirect.cos();
		m_fK = m_fA * cPt.m_fX + m_fB * cPt.m_fY;
	}
	m_fZ = cPt.m_fZ;
}

void CCAD_Linear2D::SetParaFromDirect(const CCAD_Point & cOrg, const CCAD_Vector & cDirect)
{
	CCAD_Angle cAngle(cDirect.m_fX, cDirect.m_fY);
	SetPara( cOrg, cAngle );
}


void CCAD_Linear2D::SetParaTwoCircle(const CCAD_Vector & cCp1, const CAD_FLOAT & dR1, const CCAD_Vector & cCp2, const CAD_FLOAT & dR2)
{
	m_fA = 2*(cCp2.m_fX - cCp1.m_fX);
	m_fB = 2*(cCp2.m_fY - cCp1.m_fY);
	m_fK = (dR1*dR1 - dR2*dR2) + (cCp2.m_fX*cCp2.m_fX - cCp1.m_fX*cCp1.m_fX) +
			(cCp2.m_fY*cCp2.m_fY - cCp1.m_fY*cCp1.m_fY);
	m_fZ = cCp1.m_fZ;
}

BOOL CCAD_Linear2D::Horizontal() const
{
	if ( m_fB != 0 ) {
		return (m_fA/m_fB <= COUNTING_ZERO && m_fA/m_fB >= -COUNTING_ZERO);
	}
	return FALSE;
}

//-------------------------------------------
BOOL CCAD_Linear2D::Vertical() const
{
	if ( m_fA != 0 ) {
		return (m_fB/m_fA <= COUNTING_ZERO	&& m_fB/m_fA >= -COUNTING_ZERO);
	}
	return FALSE;
}

BOOL CCAD_Linear2D::CountHorizontalPt(CCAD_Vector & cPt, CAD_FLOAT & dY) const
{
	cPt.m_fZ = m_fZ;
	cPt.m_fY = dY;
	if (!Horizontal()) {
		if ( m_fA <= COUNTING_ZERO && m_fA >= -COUNTING_ZERO )
			return FALSE;
		if (Vertical())
			cPt.m_fX = m_fK / m_fA;
		else
			cPt.m_fX = (m_fK - m_fB*dY) / m_fA;
		return TRUE;
	}
	return FALSE;
}

BOOL CCAD_Linear2D::CountVerticalPt(CCAD_Vector & cPt, CAD_FLOAT & dX) const
{
	cPt.m_fZ = m_fZ;
	cPt.m_fX = dX;
	if (!Vertical()) {
		if ( m_fB <= COUNTING_ZERO && m_fB >= -COUNTING_ZERO )
			return FALSE;
		if (Horizontal())
			cPt.m_fY = m_fK / m_fB;
		else
			cPt.m_fY = (m_fK - m_fA*dX) / m_fB;
		return TRUE;
	}
	return FALSE;
}

//-------------------------------------------
BOOL CCAD_Linear2D::CountCrossPt(CCAD_Vector & outPt, CCAD_Linear2D & cLinear) const
{
	CAD_FLOAT	d;
	
	outPt.m_fZ = m_fZ;
	if (Horizontal()) {
		if (Vertical())
			return FALSE;
		if ( m_fB <= COUNTING_ZERO && m_fB >= -COUNTING_ZERO )
			return FALSE;
		d = m_fK / m_fB;
		return cLinear.CountHorizontalPt(outPt, d);
	}
	else if (Vertical()) {
		if ( m_fA <= COUNTING_ZERO && m_fA >= -COUNTING_ZERO )
			return FALSE;
		d = m_fK / m_fA;
		return cLinear.CountVerticalPt(outPt, d);
	}
	else if (cLinear.Horizontal()) {
		if ( cLinear.m_fB <= COUNTING_ZERO && cLinear.m_fB >= -COUNTING_ZERO )
			return FALSE;
		d = cLinear.m_fK / cLinear.m_fB;
		return CountHorizontalPt(outPt, d);
	}
	else if (cLinear.Vertical()) {
		if ( cLinear.m_fA <= COUNTING_ZERO && cLinear.m_fA >= -COUNTING_ZERO )
			return FALSE;
		d = cLinear.m_fK / cLinear.m_fA;
		return CountVerticalPt(outPt, d);
	}
	else {
		if ( (m_fB <= COUNTING_ZERO && m_fB >= -COUNTING_ZERO)  ||
				(cLinear.m_fB <= COUNTING_ZERO && cLinear.m_fB >= -COUNTING_ZERO) )
			return FALSE;
		
		d = (m_fA * cLinear.m_fB) - (cLinear.m_fA * m_fB);
		if ( d > COUNTING_ZERO || d < -COUNTING_ZERO ) {
			outPt.m_fX = (m_fK * cLinear.m_fB - cLinear.m_fK * m_fB) / d;
			if (fabs(m_fB) > fabs(cLinear.m_fB))
				outPt.m_fY = (m_fK - m_fA*outPt.m_fX) / m_fB;
			else
				outPt.m_fY = (cLinear.m_fK - cLinear.m_fA*outPt.m_fX) / cLinear.m_fB;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CCAD_Linear2D::IsPtOn(const CCAD_Point &cPt)
{
	CAD_FLOAT	f = m_fA * cPt.m_fX + m_fB * cPt.m_fY - m_fK;
	return (f < SCALA_ACCY && f > -SCALA_ACCY);
}

 
