#include	"stdafx.h"
//#include	"math.h"
#include	"CAD_Global.h"
#include	"CAD_Angle.h"
#include	"CAD_Vector.h"

CCAD_Vector::CCAD_Vector()
{
	m_fX = 0;
	m_fY = 0;
	m_fZ = 0;
}

CCAD_Vector::CCAD_Vector(const CAD_FLOAT & x, const CAD_FLOAT & y, const CAD_FLOAT & z)
{
	m_fX = x;
	m_fY = y;
	m_fZ = z;
}

CCAD_Vector::CCAD_Vector (const SCAD_POINT & inPt)
{
	m_fX = inPt.m_fX;
	m_fY = inPt.m_fY;
	m_fZ = inPt.m_fZ;
}


CCAD_Vector::CCAD_Vector (const SCAD_POINT& startPt, const SCAD_POINT& endPt)
{
	m_fX = endPt.m_fX - startPt.m_fX;
	m_fY = endPt.m_fY - startPt.m_fY;
	m_fZ = endPt.m_fZ - startPt.m_fZ;
}

CCAD_Vector & CCAD_Vector::operator=(const CCAD_Vector & inPt)
{
	m_fX = inPt.m_fX;
	m_fY = inPt.m_fY;
	m_fZ = inPt.m_fZ;
	return *this;
}

CCAD_Vector & CCAD_Vector::operator=(const SCAD_POINT& inPt)
{
	m_fX = inPt.m_fX;
	m_fY = inPt.m_fY;
	m_fZ = inPt.m_fZ;
	return *this;
}

BOOL CCAD_Vector::Equal(const CCAD_Vector & inPt, const CAD_FLOAT & dAccrancy) const
{
	if ( dAccrancy <= 0 ) {
		return (m_fX == inPt.m_fX && m_fY == inPt.m_fY && m_fZ == inPt.m_fZ);
	}
	else {
		if ( (m_fX - inPt.m_fX) > dAccrancy || (m_fX - inPt.m_fX) < -dAccrancy )
			return false;
		if ( (m_fY - inPt.m_fY) > dAccrancy || (m_fY - inPt.m_fY) < -dAccrancy )
			return false;
		if ( (m_fZ - inPt.m_fZ) > dAccrancy || (m_fZ - inPt.m_fZ) < -dAccrancy )
			return false;
	}
	return true;
}

BOOL CCAD_Vector::EqualX(const CCAD_Vector & inPt, const CAD_FLOAT & dAccrancy) const
{
	if ( dAccrancy <= 0 ) {
		return (m_fX == inPt.m_fX);
	}
	else if ( (m_fX - inPt.m_fX) > dAccrancy || (m_fX - inPt.m_fX) < -dAccrancy ) {
		return false;
	}
	return true;
}

BOOL CCAD_Vector::EqualY(const CCAD_Vector & inPt, const CAD_FLOAT & dAccrancy) const
{
	if ( dAccrancy <= 0 ) {
		return (m_fY == inPt.m_fY);
	}
	else if ( (m_fY - inPt.m_fY) > dAccrancy || (m_fY - inPt.m_fY) < -dAccrancy ) {
		return false;
	}
	return true;
}

BOOL CCAD_Vector::EqualZ(const CCAD_Vector & inPt, const CAD_FLOAT & dAccrancy) const
{
	if ( dAccrancy <= 0 ) {
		return (m_fZ == inPt.m_fZ);
	}
	else if ( (m_fZ - inPt.m_fZ) > dAccrancy || (m_fZ - inPt.m_fZ) < -dAccrancy ) {
		return false;
	}
	return true;
}

void	CCAD_Vector::Move(const CCAD_Vector & cDp)
{
	m_fX += cDp.m_fX;
	m_fY += cDp.m_fY;
	m_fZ += cDp.m_fZ;
}

void CCAD_Vector::Rotate(const CCAD_Vector & inPt, const CAD_FLOAT & dSin, const CAD_FLOAT & dCos)
{
	CAD_FLOAT	dX = m_fX - inPt.m_fX;
	CAD_FLOAT	dY = m_fY - inPt.m_fY;
	m_fX = inPt.m_fX + dX*dCos - dY*dSin;
	m_fY = inPt.m_fY + dX*dSin + dY*dCos;
}

void CCAD_Vector::Mirror(const CCAD_Vector & inPt, const CAD_FLOAT & dSin, const CAD_FLOAT & dCos)
{
	CAD_FLOAT	dX = m_fX - inPt.m_fX;
	CAD_FLOAT	dY = m_fY - inPt.m_fY;
	CAD_FLOAT	x = dX * dCos + dY * dSin;
	CAD_FLOAT	y = dX * dSin - dY * dCos;
	m_fX = inPt.m_fX + x * dCos - y * dSin;
	m_fY = inPt.m_fY + x * dSin + y * dCos;
}

void CCAD_Vector::Scale(const CCAD_Vector & inPt, const CAD_FLOAT & dScale)
{
	m_fX = inPt.m_fX + (m_fX-inPt.m_fX) * dScale;
	m_fY = inPt.m_fY + (m_fY-inPt.m_fY) * dScale;
	m_fZ = inPt.m_fZ + (m_fZ-inPt.m_fZ) * dScale;
}

CAD_FLOAT CCAD_Vector::CountLength(const CCAD_Vector & inPt) const
{
	CAD_FLOAT	d1 = m_fX - inPt.m_fX;
	CAD_FLOAT	d2 = m_fY - inPt.m_fY;
	CAD_FLOAT	d3 = m_fZ - inPt.m_fZ;
	d1 = (CAD_FLOAT)sqrt(d1*d1 + d2*d2 + d3*d3);
	return d1;
}

double CCAD_Vector::Abs() const
{
	double x = m_fX;
	double y = m_fY;
	double z = m_fZ;

	return sqrt(x*x+y*y+z*z);
}

void CCAD_Vector::CountAngle2D(const CCAD_Vector & inPt, CCAD_Angle & outAngle) const
{
	outAngle.SetFromXY(inPt.m_fX - m_fX, inPt.m_fY - m_fY);
}

BYTE CCAD_Vector::CountSide2D(const CCAD_Vector & inPt, const CCAD_Angle & inAngle) const
{
	CAD_FLOAT	fX = m_fX - inPt.m_fX;
	CAD_FLOAT	fY = m_fY - inPt.m_fY;
	fY = fY*inAngle.cos() - fX*inAngle.sin();
	if (fY == 0)
		return NO_DIRECT;
	else if (fY > 0)
		return NOCLOCK_DIRECT;
	return CLOCK_DIRECT;
}

void CCAD_Vector::CountVector2D(CCAD_Vector & outPt, const CAD_FLOAT & dR, const CAD_FLOAT & dSinA, const CAD_FLOAT & dCosA) const
{
	outPt.m_fX = m_fX + dR * dCosA;
	outPt.m_fY = m_fY + dR * dSinA;
	outPt.m_fZ = m_fZ;
}

BOOL CCAD_Vector::IsCrossHorizontal(const CCAD_Vector cPt, const CAD_FLOAT & dHrztY, CAD_FLOAT & dGetX) const
{
	if (m_fY == cPt.m_fY)
		return FALSE;
	else if (m_fY > dHrztY && cPt.m_fY > dHrztY)
		return FALSE;
	else if (m_fY < dHrztY && cPt.m_fY < dHrztY)
		return FALSE;
	else if (m_fX == cPt.m_fX || m_fY == dHrztY)
		dGetX = m_fX;
	else if (cPt.m_fY == dHrztY)
		dGetX = cPt.m_fX;
	else 
		dGetX = m_fX + (dHrztY-m_fY)*(cPt.m_fX-m_fX)/(cPt.m_fY-m_fY);
	return TRUE;
}

BOOL CCAD_Vector::IsCrossVertical(const CCAD_Vector cPt, const CAD_FLOAT & dVrtX, CAD_FLOAT & dGetY) const
{
	if (m_fX == cPt.m_fX)
		return FALSE;
	else if (m_fX > dVrtX && cPt.m_fX > dVrtX)
		return FALSE;
	else if (m_fX < dVrtX && cPt.m_fX < dVrtX)
		return FALSE;
	else if (m_fY == cPt.m_fY || m_fX == dVrtX)
		dGetY = m_fY;
	else if (cPt.m_fX == dVrtX)
		dGetY = cPt.m_fY;
	else 
		dGetY = m_fY + (dVrtX-m_fX)*(cPt.m_fY-m_fY)/(cPt.m_fX-m_fX);
	return TRUE;
}

BOOL CCAD_Vector::IsParallel(const CCAD_Vector & inPt) const
{
	CCAD_Vector cPt = *this * inPt;
	CAD_FLOAT	fScalar = (CAD_FLOAT)cPt.Abs();
	return ( fabs(fScalar) < CCAD_Global::GetSqrtAccy() );
}

void CCAD_Vector::Read(CCAD_File & cFile)
{
	cFile.ReadCAD_Float(&m_fX);
	cFile.ReadCAD_Float(&m_fY);
	cFile.ReadCAD_Float(&m_fZ);
}

void CCAD_Vector::Write(CCAD_File & cFile)
{
	cFile.WriteCadFloat(m_fX);
	cFile.WriteCadFloat(m_fY);
	cFile.WriteCadFloat(m_fZ);
}

CCAD_Vector CCAD_Vector::operator * (const CCAD_Vector& cOther) const
{
	return CCAD_Vector(m_fY * cOther.m_fZ - m_fZ * cOther.m_fY,
					 m_fZ * cOther.m_fX - m_fX * cOther.m_fZ,
					 m_fX * cOther.m_fY - m_fY * cOther.m_fX);
}
void CCAD_Vector::operator *= (const CCAD_Vector& cOther)
{
	CAD_FLOAT dX = m_fY * cOther.m_fZ - m_fZ * cOther.m_fY;
	CAD_FLOAT dY = m_fZ * cOther.m_fX - m_fX * cOther.m_fZ;
	CAD_FLOAT dZ = m_fX * cOther.m_fY - m_fY * cOther.m_fX;

	m_fX = dX;
	m_fY = dY;
	m_fZ = dZ;
}

CCAD_Vector CCAD_Vector::operator * (CAD_FLOAT fConst) const
{
	return CCAD_Vector(	m_fX * fConst,
						m_fY * fConst,
						m_fZ * fConst );

}

CCAD_Vector CCAD_Vector::operator * (int nConst) const
{
	return CCAD_Vector(	m_fX * (CAD_FLOAT)nConst,
						m_fY * (CAD_FLOAT)nConst,
						m_fZ * (CAD_FLOAT)nConst );

}

void CCAD_Vector::operator *= (CAD_FLOAT fConst)
{
	m_fX *= fConst;
	m_fY *= fConst;
	m_fZ *= fConst;
}
void CCAD_Vector::operator *= (int nConst)
{
	m_fX *= ( CAD_FLOAT )nConst;
	m_fY *= ( CAD_FLOAT )nConst;
	m_fZ *= ( CAD_FLOAT )nConst;
}

CAD_FLOAT CCAD_Vector::operator & (const CCAD_Vector& sOther) const
{
	return (m_fX * sOther.m_fX + m_fY * sOther.m_fY + m_fZ * sOther.m_fZ);
}

CCAD_Vector CCAD_Vector::operator - (const CCAD_Vector& cOther) const
{
	return CCAD_Vector(	m_fX - cOther.m_fX,
						m_fY - cOther.m_fY,
						m_fZ - cOther.m_fZ	);
}

CCAD_Vector CCAD_Vector::operator + (const CCAD_Vector& cOther) const
{
	return CCAD_Vector(	m_fX + cOther.m_fX,
						m_fY + cOther.m_fY,
						m_fZ + cOther.m_fZ	);
}

void CCAD_Vector::operator += (const CCAD_Vector& cOther)
{
	m_fX += cOther.m_fX;
	m_fY += cOther.m_fY;
	m_fZ += cOther.m_fZ;
}

void CCAD_Vector::operator -= (const CCAD_Vector& cOther)
{
	m_fX -= cOther.m_fX;
	m_fY -= cOther.m_fY;
	m_fZ -= cOther.m_fZ;
}

BOOL CCAD_Vector::operator == (const CCAD_Vector& cOther)
{
	if( m_fX != cOther.m_fX ||
		m_fY != cOther.m_fY ||
		m_fZ != cOther.m_fZ )
		return false;
	return true;
}

BOOL CCAD_Vector::operator != (const CCAD_Vector& cOther)
{
	if( m_fX != cOther.m_fX ||
		m_fY != cOther.m_fY ||
		m_fZ != cOther.m_fZ )
		return true;
	return false;
}

void CCAD_Vector::Normalize()
{
	CAD_FLOAT A = (CAD_FLOAT)sqrt(m_fX * m_fX + m_fY * m_fY + m_fZ * m_fZ);

	if ( A < CCAD_Global::GetLengthAccy() ) {
		m_fX = 1;
		m_fY = m_fZ = 0;
	}
	else {
		m_fX = m_fX / A;
		m_fY = m_fY / A;
		m_fZ = m_fZ / A;
	}
}
void CCAD_Vector::RotateLeft()
{
	CAD_FLOAT oldX = m_fX;
	m_fX = m_fY;
	m_fY = m_fZ;
	m_fZ = oldX;
}

void CCAD_Vector::RotateRight()
{
	CAD_FLOAT oldZ = m_fZ;
	m_fZ = m_fY;
	m_fY = m_fX;
	m_fX = oldZ;
}

 
