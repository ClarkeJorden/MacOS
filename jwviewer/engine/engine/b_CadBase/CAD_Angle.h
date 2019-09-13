#ifndef _CAD_ANGLE_H_
#define _CAD_ANGLE_H_

#include	"CAD_Constance.h"

class   CCAD_Angle {

#define ARC_DELTA		50
#define MAX_ANGLE		9000
#define MIN_ANGLE		-9000
#define TATAL_ANGLE		18000
#define HALF_ANGLE		4500
#define DIV_ANGLE_NUM	50
#define DIV_ANGLE_VALUE	0.00035 // sin(1)
						//6.09234839569648399987180721941088e-6F	// sin(1)

private:
	static	CAD_FLOAT	m_sinTbl[];	// 0.01 degree
	short	m_nDegree;				// degree range (-180.00, 180.00 )

public:
	CCAD_Angle();
	CCAD_Angle(const short & nDegree);
	CCAD_Angle(const CAD_FLOAT & dRadian);				// for old version
//	CCAD_Angle(const CCAD_Angle & inAngle);
	CCAD_Angle(const CAD_FLOAT& fX, const CAD_FLOAT& fY);

									
	CCAD_Angle & operator=(const CCAD_Angle & inAngle);
	void operator=(const int & nDegree);
	void operator=(const CAD_FLOAT & dRadian);			// for old version

	BOOL	IsAcuteAngle();
								
	void	SetFromDegree(const CAD_FLOAT& fDegree);
	void	SetFromXY(const CAD_FLOAT& fX, const CAD_FLOAT& fY);
	void	SetFromAsin(const CAD_FLOAT& fR, const CAD_FLOAT& fH);
	void	SetFromAcos(const CAD_FLOAT& fR, const CAD_FLOAT& fH);
	BOOL	Horizontal() const;
	BOOL	Vertical() const;
	BOOL	IsZero() const;

	CAD_FLOAT	GetRadian() const;
	CAD_FLOAT	GetAbsRadian() const;
	CAD_FLOAT	GetRadian360() const;
	CAD_FLOAT	GetDegree() const;
	CAD_FLOAT	GetAbsDegree() const;
	CAD_FLOAT	GetDegree360() const;


	BOOL	operator==(const CCAD_Angle & inAngle) const;
	BOOL	operator>(const CCAD_Angle & inAngle) const;
	BOOL	operator<(const CCAD_Angle & inAngle) const;
	BOOL	operator==(CAD_FLOAT fDegree) const;
	BOOL	operator>(CAD_FLOAT fDegree) const;
	BOOL	operator<(CAD_FLOAT fDegree) const;
	void	operator/=(const short & n);
	void	operator*=(const short & n);
	void operator-=(const CCAD_Angle & inAngle);
	void operator+=(const CCAD_Angle & inAngle);

	void	Delta(const CCAD_Angle & inAngle, CCAD_Angle & outAngle) const;
	void	RotateAngle(const CCAD_Angle & inAngle);
	void	InvertRotateAngle(const CCAD_Angle & inAngle);
	void	RotateAngle(const short & nDegree);
	void	RotateDelta(const CCAD_Angle & inAngle);
	void	InvertRotateDelta(const CCAD_Angle & inAngle);
	void	RotateDelta(const short & nDegree);
	void	Mirror(const CCAD_Angle & inAngle);
	void	Invert();

	static	void MakeSinTable();
	CAD_FLOAT	sin() const;
	CAD_FLOAT	cos() const;
	CAD_FLOAT	tan() const;

	void	Read(CCAD_File & cFile);
};

 

#endif 