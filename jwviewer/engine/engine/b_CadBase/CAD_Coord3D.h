// CAD_Coord3D.h: interface for the CCAD_Coord3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_COORD3D_H_)
#define AFX_CAD_COORD3D_H_

#include "CAD_Angle.h"
#include "CAD_Vector.h"
#include "CAD_Rect.h"

class	CCAD_GeoPlan;

class   CCAD_Coord3D
{
public:
	CCAD_Vector		m_cOrg;		// odinary Pointer
	CCAD_Vector		m_cX;		// X vector
	CCAD_Vector		m_cY;		// Y vector
	CCAD_Vector		m_cZ;		// Z vector

	CAD_FLOAT		m_fScale;	// default = 1

public:
	CCAD_Coord3D();
	CCAD_Coord3D(const CCAD_GeoPlan & cPlan);
	CCAD_Coord3D(const CCAD_Point & cStart, const CCAD_Point & cEnd);	// In Base Plan
	void Init();

// Operations
public:
	void Read(CCAD_File & cFile);
	void Write(CCAD_File & cFile);

	void operator = (const CCAD_Coord3D& cOther);
	void SetCoord(const CCAD_Point & cOrg, const CCAD_Vector & cAxisX);

	void ConvToBase(SCAD_POINT& sOldPt) const;
	void ConvToBaseX(SCAD_POINT& sOldPt, BOOL bIsVtr = FALSE) const;
	void ConvFromBase(SCAD_POINT& oldPt) const;
	void ConvVectorToBase(CCAD_Vector& sOldPt) const;
	void ConvVectorFromBase(CCAD_Vector& oldPt) const;
	
	void ConvToBase(CCAD_Rect& cRect) const;
	void ConvFromBase(CCAD_Rect& cRect) const;

	void ChildToThis(CCAD_Coord3D* pOther) const;
	void ThisToChild(CCAD_Coord3D* pOther);

	void XRotate(const CCAD_Angle& cAngle, CCAD_Vector& cPoint);
	void YRotate(const CCAD_Angle& cAngle, CCAD_Vector& cPoint);
	void ZRotate(const CCAD_Angle& cAngle, CCAD_Vector& cPoint);

	void XRotate(const CCAD_Angle& cAngle);
	void YRotate(const CCAD_Angle& cAngle);
	void ZRotate(const CCAD_Angle& cAngle);

	void XRotateChildCoord(const CCAD_Angle& cAngle, CCAD_Coord3D* pChild);
	void YRotateChildCoord(const CCAD_Angle& cAngle, CCAD_Coord3D* pChild);
	void ZRotateChildCoord(const CCAD_Angle& cAngle, CCAD_Coord3D* pChild);

	void XRotateEqualCoord(const CCAD_Angle& cAngle, CCAD_Coord3D* pChild);
	void YRotateEqualCoord(const CCAD_Angle& cAngle, CCAD_Coord3D* pChild);
	void ZRotateEqualCoord(const CCAD_Angle& cAngle, CCAD_Coord3D* pChild);

	BOOL	IsEqualPlan(const CCAD_Coord3D & cCoord) const;
	BOOL	IsParallelPlan(const CCAD_Coord3D & cCoord) const;
	BOOL	ConvertAngleFromCoord(const CCAD_Coord3D & cCoord, CCAD_Angle & cAngle);

	//	for render.(2000/12/20)
	BOOL	GetRotatedAngle(CCAD_Angle& cX, CCAD_Angle& cY, CCAD_Angle& cZ) const;
	BOOL	GetRotatedX(CCAD_Angle& cAngle) const;
	BOOL	GetRotatedY(CCAD_Angle& cAngle) const;
	BOOL	GetRotatedZ(CCAD_Angle& cAngle) const;

	void	Normalize();
	void	MirrorSelf();
	BOOL	IsMirrored() const;
	void	MirrorCoord(CAD_FLOAT fScaleX, CAD_FLOAT fScaleY, CAD_FLOAT fScaleZ);

	void	GetMatrixFromBase(float *mat);
	void	GetMatrixToBase(float *mat);
	void	GetMulMatrix(float *mat, float fScale);

// Implementation
public:
	virtual ~CCAD_Coord3D();
};

 

#endif // !defined(AFX_CAD_COORD3D_H_)
