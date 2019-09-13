// CAD_Camera.h: interface for the CCAD_Camera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_CAMERA_H_)
#define AFX_CAD_CAMERA_H_

#include "CAD_Coord3D.h"
#include "CAD_GeoPlan.h"

#define		CAMERA_MIN_NEAR		30.0f 

class   CCAD_Camera : public CCAD_Coord3D  
{
public:
	CCAD_Camera();
	CCAD_Camera(const CCAD_Camera& cOther);
	virtual ~CCAD_Camera();

// class member
public:	
	CAD_FLOAT	m_fDistToEye;
	CAD_FLOAT	m_fWidth;
	CAD_FLOAT	m_fHeight;
	CAD_FLOAT	m_fNear;
	CAD_FLOAT	m_fFar;
	BOOL		m_bPerspect;
	float		m_fModelViewMat[16];

// method
public:
	void operator = (const CCAD_Camera & cOther);
	BOOL operator == (const CCAD_Camera & pOther);
	
	void		Init(CAD_FLOAT fWidth, CAD_FLOAT fHeight);
	void		InitCamera();
	CAD_FLOAT	GetFovyH();
	CAD_FLOAT	GetFovyV();

	// Camera Translation( by Project Plan org  )
	void		SetVCSOrg(const SCAD_POINT& cOrgPt);
	void		MoveVerti(CAD_FLOAT fDelta);
	void		MoveHoriz(CAD_FLOAT fDelta);
	void		MoveNearFar(CAD_FLOAT fDelta);

	// Camera Translation( by self org  )
	void		MoveUpDown(CAD_FLOAT fDelta);
	void		MoveForward(CAD_FLOAT fDelta);
	void		RotateHoriz(const CCAD_Angle& angle);
	void		RotateVerti(const CCAD_Angle& angle);

	// Camera Translation by Focus Point 
	void RotateHorizFocusPoint(const CCAD_Point& sFocusPt, const CCAD_Angle& sAngle);
	void RotateVertiFocusPoint(const CCAD_Point& sFocusPt, const CCAD_Angle& sAngle);
	void SetProjectPlan(const SCAD_POINT& sFocusPt);


	// clipping function
	BOOL		IsInternalPt(const CCAD_Point& cPt);
	BOOL		ClipNear(CCAD_Point& cPt1, CCAD_Point& cPt2);
	BOOL		ClipFar(CCAD_Point& cPt1, CCAD_Point& cPt2);
	BOOL		ClipLeft(CCAD_Point& cPt1, CCAD_Point& cPt2);
	BOOL		ClipRight(CCAD_Point& cPt1, CCAD_Point& cPt2);
	BOOL		ClipTop(CCAD_Point& cPt1, CCAD_Point& cPt2);
	BOOL		ClipBottom(CCAD_Point& cPt1, CCAD_Point& cPt2);

	BOOL		ClipPyramid(	CCAD_Point& cPt1, CCAD_Point& cPt2, 
								BOOL blNearClip = true, BOOL blFarClip = false);



	SCAD_POINT&	ECStoVCS(SCAD_POINT& cPt);

	void		Read(CCAD_File & file);
	void		Write(CCAD_File & file);
	
	BOOL		IsEqualCamera(CCAD_Camera* pOther);

	BOOL SetVcsSize(CAD_FLOAT fWidth = CAMERA_WIDTH, CAD_FLOAT fHeight = CAMERA_HEIGHT);

	void GetMat();
private:
	CCAD_GeoPlan	m_cLeftClipPlan;
	CCAD_GeoPlan	m_cRightClipPlan;
	CCAD_GeoPlan	m_cTopClipPlan;
	CCAD_GeoPlan	m_cBottomClipPlan;

	void	CreateClipPlan();

};

 

#endif 
