#if !defined(AFX_CAD_GEOPLAN_H_)
#define AFX_CAD_GEOPLAN_H_

#include "CAD_Vector.h"

class	CCAD_Coord3D;

class   CCAD_GeoPlan  
{
public:
	CCAD_GeoPlan();
	CCAD_GeoPlan(const CCAD_Point& pt1,const CCAD_Point& pt2,const CCAD_Point& pt3);
	CCAD_GeoPlan(const CCAD_Coord3D & cCoord);

	virtual ~CCAD_GeoPlan();

	void	Create(const CCAD_Point& pt1,const CCAD_Point& pt2,const CCAD_Point& pt3);
	void	Create(const CCAD_Coord3D & cCoord);
	CCAD_Coord3D	CreateCoord3D();

	BOOL	IsFront(const CCAD_Point& inPt);
	BOOL	IsCross(const CCAD_Point& pt1,const CCAD_Point& pt2);
	BOOL	ClipLine(CCAD_Point& pt1, CCAD_Point& pt2, int nDirect);
	BOOL	CrossPt(const CCAD_Point& Pt1,const CCAD_Point& Pt2, CCAD_Point& crossPt);
	BOOL	IsPtOnPlan(const CCAD_Vector & cPt) const;
	BOOL	IsParallel(const CCAD_GeoPlan & cPlan) const;
	BOOL	IsEqual(const CCAD_GeoPlan & cPlan) const;
	void	GetPoint1(CCAD_Point & cPt) const { cPt = m_Pt1; }
	void	GetPoint2(CCAD_Point & cPt) const { cPt = m_Pt2; }
	void	GetPoint3(CCAD_Point & cPt) const { cPt = m_Pt3; }

public:
	// Normal Vector
	CCAD_Vector		m_cNormal;			// a, b, c
	CAD_FLOAT		m_fD;				// d

private:
	CCAD_Point		m_Pt1;
	CCAD_Point		m_Pt2;
	CCAD_Point		m_Pt3;

	void ComputeNormal();
};

#define FRONT_DIRECTION		1
#define BACK_DIRECTION		0

 

#endif // !defined(AFX_CAD_GEOPLAN_H_)
