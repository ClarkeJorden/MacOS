#include	"stdafx.h"
//#include	"math.h"
#include	"CAD_Global.h"
#include	"CAD_Angle.h"
 

CAD_FLOAT CCAD_Angle::m_sinTbl[MAX_ANGLE+1];

CCAD_Angle::CCAD_Angle()
{
	m_nDegree = 0;
}

CCAD_Angle::CCAD_Angle (const short & nDegree)
{
	m_nDegree = nDegree * DIV_ANGLE_NUM;
	if ( m_nDegree > TATAL_ANGLE )
		m_nDegree = TATAL_ANGLE;
	if ( m_nDegree < -TATAL_ANGLE )
		m_nDegree = -TATAL_ANGLE;
}

CCAD_Angle::CCAD_Angle(const CAD_FLOAT & dRadian)
{
	if ( dRadian > 0 )
		m_nDegree = (short)(dRadian * MAX_ANGLE/ PAI + 0.5);
	else 
		m_nDegree = (short)(dRadian * MAX_ANGLE/ PAI - 0.5);
	if ( m_nDegree > TATAL_ANGLE )
		m_nDegree = TATAL_ANGLE;
	if ( m_nDegree < -TATAL_ANGLE )
		m_nDegree = -TATAL_ANGLE;
}

CCAD_Angle::CCAD_Angle(const CAD_FLOAT & dX, const CAD_FLOAT & dY)
{
	SetFromXY( dX, dY );
}


CCAD_Angle & CCAD_Angle::operator=(const CCAD_Angle & inAngle)
{
	m_nDegree = inAngle.m_nDegree;
	return *this;
}

void CCAD_Angle::operator=(const int & nDegree)
{
	m_nDegree = (short)nDegree * DIV_ANGLE_NUM;
	if ( m_nDegree > TATAL_ANGLE )
		m_nDegree = TATAL_ANGLE;
	if ( m_nDegree < -TATAL_ANGLE )
		m_nDegree = -TATAL_ANGLE;
}

void CCAD_Angle::operator=(const CAD_FLOAT & dRadian)
{
	if ( dRadian > 0 )
		m_nDegree = (short)(dRadian * MAX_ANGLE/ PAI + 0.5);
	else 
		m_nDegree = (short)(dRadian * MAX_ANGLE/ PAI - 0.5);
	if ( m_nDegree > TATAL_ANGLE )
		m_nDegree = TATAL_ANGLE;
	if ( m_nDegree < -TATAL_ANGLE )
		m_nDegree = -TATAL_ANGLE;
}

BOOL CCAD_Angle::IsAcuteAngle()
{
#ifdef _WIN32
	return ( HALF_ANGLE >= abs(m_nDegree) );
#else
	return ( HALF_ANGLE >= fabs(m_nDegree) );
#endif
}

void CCAD_Angle::SetFromXY(const CAD_FLOAT & dX, const CAD_FLOAT & dY)
{
	double	dRadAngle = atan2(dY, dX) * MAX_ANGLE / PAI;
	if ( dRadAngle > 0 )
		m_nDegree = (short)(dRadAngle + 0.5);
	else 
		m_nDegree = (short)(dRadAngle - 0.5);
}

void CCAD_Angle::SetFromAsin(const CAD_FLOAT& fR, const CAD_FLOAT& fH)
{
	double	dRadAngle = asin(fH/fR) * MAX_ANGLE / PAI;
	if ( dRadAngle > 0 )
		m_nDegree = (short)(dRadAngle + 0.5);
	else 
		m_nDegree = (short)(dRadAngle - 0.5);
}

void CCAD_Angle::SetFromAcos(const CAD_FLOAT& fR, const CAD_FLOAT& fH)
{
	double	dRadAngle = acos(fH/fR) * MAX_ANGLE / PAI;
	if ( dRadAngle > 0 )
		m_nDegree = (short)(dRadAngle + 0.5);
	else 
		m_nDegree = (short)(dRadAngle - 0.5);
}

void CCAD_Angle::SetFromDegree(const CAD_FLOAT& fDegree)
{
	if ( fDegree > 0 )
		m_nDegree = (short) (fDegree * DIV_ANGLE_NUM + 0.5);
	else 
		m_nDegree = (short) (fDegree * DIV_ANGLE_NUM - 0.5);
	if ( m_nDegree > TATAL_ANGLE )
		m_nDegree = TATAL_ANGLE;
	if ( m_nDegree < -TATAL_ANGLE )
		m_nDegree = -TATAL_ANGLE;
}

CAD_FLOAT CCAD_Angle::GetRadian() const
{
	return ((CAD_FLOAT) m_nDegree / MAX_ANGLE * PAI);
}

CAD_FLOAT CCAD_Angle::GetAbsRadian() const
{
#ifdef _WIN32
	return ((CAD_FLOAT)(abs(m_nDegree)) / MAX_ANGLE * PAI);
#else
	return ((CAD_FLOAT)(fabs(m_nDegree)) / MAX_ANGLE * PAI);
#endif
}

CAD_FLOAT CCAD_Angle::GetRadian360() const
{
	CAD_FLOAT	fDegree = (CAD_FLOAT) m_nDegree / MAX_ANGLE * PAI;
	if ( fDegree < 0 )
		fDegree += (PAI*2);
	return fDegree;
}

CAD_FLOAT CCAD_Angle::GetDegree() const
{
	return ((CAD_FLOAT) m_nDegree / DIV_ANGLE_NUM);
}

CAD_FLOAT CCAD_Angle::GetAbsDegree() const
{
#ifdef _WIN32
	return ((CAD_FLOAT)(abs(m_nDegree)) / DIV_ANGLE_NUM);
#else
	return ((CAD_FLOAT)(fabs(m_nDegree)) / DIV_ANGLE_NUM);
#endif
}

CAD_FLOAT CCAD_Angle::GetDegree360() const
{
	CAD_FLOAT	fDegree = (CAD_FLOAT) m_nDegree / DIV_ANGLE_NUM;
	if ( fDegree < 0 )
		fDegree += LONGDOUBLE_NINETY;
	return fDegree;
}

BOOL CCAD_Angle::Horizontal() const
{
	return ( m_nDegree == 0 || m_nDegree == MAX_ANGLE || m_nDegree == MIN_ANGLE );
}

BOOL CCAD_Angle::IsZero() const
{
	return ( m_nDegree == 0 );
}

BOOL CCAD_Angle::Vertical() const
{
	return ( m_nDegree == HALF_ANGLE || m_nDegree == -HALF_ANGLE );
}

BOOL CCAD_Angle::operator==(const CCAD_Angle & inAngle) const
{
	return ( m_nDegree == inAngle.m_nDegree );
}

BOOL CCAD_Angle::operator>(const CCAD_Angle & inAngle) const
{
	return ( m_nDegree > inAngle.m_nDegree );
}

BOOL CCAD_Angle::operator<(const CCAD_Angle & inAngle) const
{
	return ( m_nDegree < inAngle.m_nDegree );
}

BOOL CCAD_Angle::operator==(CAD_FLOAT fDegree) const
{
	return ( m_nDegree == fDegree*DIV_ANGLE_NUM );
}

BOOL CCAD_Angle::operator>(CAD_FLOAT fDegree) const
{
	return ( m_nDegree > fDegree*DIV_ANGLE_NUM );
}

BOOL CCAD_Angle::operator<(CAD_FLOAT fDegree) const
{
	return ( m_nDegree < fDegree*DIV_ANGLE_NUM );
}

void CCAD_Angle::operator/=(const short & n)
{
	if ( n > 0 )
		m_nDegree /= n;
}

void CCAD_Angle::operator*=(const short & n)
{
	m_nDegree *= n;
	while (m_nDegree >= TATAL_ANGLE ) {
		m_nDegree -= TATAL_ANGLE;
	}
	while ( m_nDegree <= -TATAL_ANGLE ) {
		m_nDegree += TATAL_ANGLE;
	}
}

void CCAD_Angle::operator-=(const CCAD_Angle & inAngle)
{
	m_nDegree -= inAngle.m_nDegree;
	if (m_nDegree >= TATAL_ANGLE)
		m_nDegree -= TATAL_ANGLE;
	else if (m_nDegree <= -TATAL_ANGLE)
		m_nDegree += TATAL_ANGLE;
	
	if (m_nDegree > MAX_ANGLE) {
		m_nDegree = TATAL_ANGLE - m_nDegree;
	}
	else if (m_nDegree <= MIN_ANGLE) {
		m_nDegree += TATAL_ANGLE;
	}
}
void CCAD_Angle::operator+=(const CCAD_Angle & inAngle)
{
	m_nDegree += inAngle.m_nDegree;
	if (m_nDegree >= TATAL_ANGLE)
		m_nDegree -= TATAL_ANGLE;
	else if (m_nDegree <= -TATAL_ANGLE)
		m_nDegree += TATAL_ANGLE;
	
	if (m_nDegree > MAX_ANGLE) {
		m_nDegree = TATAL_ANGLE - m_nDegree;
	}
	else if (m_nDegree <= MIN_ANGLE) {
		m_nDegree += TATAL_ANGLE;
	}
}

void CCAD_Angle::Delta(const CCAD_Angle & inAngle, CCAD_Angle & outAngle) const
{
	outAngle.m_nDegree = m_nDegree - inAngle.m_nDegree;
	if ( outAngle.m_nDegree >= TATAL_ANGLE )
		outAngle.m_nDegree -= TATAL_ANGLE;
	else if ( outAngle.m_nDegree <= -TATAL_ANGLE )
		outAngle.m_nDegree += TATAL_ANGLE;

	if ( outAngle.m_nDegree > MAX_ANGLE ) {
		outAngle.m_nDegree = TATAL_ANGLE - outAngle.m_nDegree;
	}
	else if ( outAngle.m_nDegree <= MIN_ANGLE ) {
		outAngle.m_nDegree += TATAL_ANGLE;
	}
	else if ( outAngle.m_nDegree < 0 ) {
		outAngle.m_nDegree = -outAngle.m_nDegree;
	}
}

void CCAD_Angle::RotateAngle(const CCAD_Angle & inAngle)
{
	m_nDegree += inAngle.m_nDegree;
	while ( m_nDegree > MAX_ANGLE ) {
		m_nDegree -= TATAL_ANGLE;
	}
	while ( m_nDegree <= -MAX_ANGLE ) {
		m_nDegree += TATAL_ANGLE;
	}
}

void CCAD_Angle::RotateDelta(const CCAD_Angle & inAngle)
{
	m_nDegree += inAngle.m_nDegree;
	if ( m_nDegree >= TATAL_ANGLE ) {
		m_nDegree -= TATAL_ANGLE;
	}
	else if ( m_nDegree <= -TATAL_ANGLE ) {
		m_nDegree += TATAL_ANGLE;
	}
}

void CCAD_Angle::InvertRotateAngle(const CCAD_Angle & inAngle)
{
	m_nDegree -= inAngle.m_nDegree;
	while ( m_nDegree > MAX_ANGLE ) {
		m_nDegree -= TATAL_ANGLE;
	}
	while ( m_nDegree <= -MAX_ANGLE ) {
		m_nDegree += TATAL_ANGLE;
	}
}

void CCAD_Angle::InvertRotateDelta(const CCAD_Angle & inAngle)
{
	m_nDegree -= inAngle.m_nDegree;
	if ( m_nDegree >= TATAL_ANGLE ) {
		m_nDegree -= TATAL_ANGLE;
	}
	else if ( m_nDegree <= -TATAL_ANGLE ) {
		m_nDegree += TATAL_ANGLE;
	}
}

void CCAD_Angle::RotateAngle(const short & nDegree)
{
	m_nDegree += (nDegree*DIV_ANGLE_NUM);
	if ( m_nDegree > MAX_ANGLE ) {
		m_nDegree -= TATAL_ANGLE;
	}
	else if ( m_nDegree <= -MAX_ANGLE ) {
		m_nDegree += TATAL_ANGLE;
	}
}

void CCAD_Angle::RotateDelta(const short & nDegree)
{
	m_nDegree += (nDegree*DIV_ANGLE_NUM);
	if ( m_nDegree >= TATAL_ANGLE ) {
		m_nDegree -= TATAL_ANGLE;
	}
	else if ( m_nDegree <= -TATAL_ANGLE ) {
		m_nDegree += TATAL_ANGLE;
	}
}


void CCAD_Angle::Mirror(const CCAD_Angle & inAngle)
{
	m_nDegree = inAngle.m_nDegree*2 - m_nDegree;
	if ( m_nDegree >= TATAL_ANGLE ) {
		m_nDegree -= TATAL_ANGLE;
	}
	else if ( m_nDegree <= -TATAL_ANGLE ) {
		m_nDegree += TATAL_ANGLE;
	}
	if ( m_nDegree >= TATAL_ANGLE ) {
		m_nDegree -= TATAL_ANGLE;
	}
	else if ( m_nDegree <= -TATAL_ANGLE ) {
		m_nDegree += TATAL_ANGLE;
	}
}

void CCAD_Angle::Invert()
{
	m_nDegree = -m_nDegree;
}

void CCAD_Angle::MakeSinTable()
{
	m_sinTbl[0] = 0;
	int i;
	for(i=1; i<HALF_ANGLE; i++) {
		m_sinTbl[i] = (CAD_FLOAT)(::sin(PAI * i / MAX_ANGLE));
	}
	m_sinTbl[HALF_ANGLE] = 1;
}

CAD_FLOAT CCAD_Angle::sin() const
{
	short nAngle = m_nDegree;

	if(nAngle > MAX_ANGLE )
		nAngle -= TATAL_ANGLE;
	else if(nAngle < MIN_ANGLE )
		nAngle += TATAL_ANGLE;

	if(nAngle >= 0 ) {
		if( nAngle <= HALF_ANGLE )
			return m_sinTbl[nAngle];			// 0.00 -> 90.00
		else
			return m_sinTbl[MAX_ANGLE-nAngle];	// 90.01 -> 180.00
	}else{
		nAngle = -nAngle;
		if( nAngle <= HALF_ANGLE )
			return - m_sinTbl[nAngle];				// 0.00 -> -90.00
		else
			return - m_sinTbl[MAX_ANGLE-nAngle];	// -90.01 -> -180.00
	}
}

CAD_FLOAT CCAD_Angle::cos() const
{
	short nAngle = m_nDegree;

	if(nAngle > MAX_ANGLE )
		nAngle -= TATAL_ANGLE;
	else if(nAngle < MIN_ANGLE )
		nAngle += TATAL_ANGLE;

	if( nAngle < 0 )
		nAngle = -nAngle;

	if( nAngle <= HALF_ANGLE )
		return m_sinTbl[HALF_ANGLE-nAngle];			// 0.00 -> 90.00
	else
		return - m_sinTbl[nAngle-HALF_ANGLE];			// 90.01 -> 180.00
}

CAD_FLOAT CCAD_Angle::tan() const
{
	CAD_FLOAT	fRadian = m_nDegree * PAI / MAX_ANGLE;
	fRadian = (CAD_FLOAT)::tan(fRadian);
	return fRadian;
}

void CCAD_Angle::Read(CCAD_File & cFile)
{
	cFile.ReadShort(&m_nDegree);
}

 
