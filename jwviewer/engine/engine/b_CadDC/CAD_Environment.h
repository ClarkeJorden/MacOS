// CAD_Environment.h: interface for the CCAD_Environment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_ENVIRONMENT_H)
#define AFX_CAD_ENVIRONMENT_H


#include "CAD_Constance.h"
#include "CAD_Angle.h"
#include "CAD_Color.h"
#include "CAD_Pen.h"
#include "CAD_Brush.h"

 

class	CCAD_Image;


typedef struct tagPlanInfo {
	short		nLengthUnit;
	short		nAngleUnit;
	short		nCircleDivide;
	short		nEllpDivide;
	short		nCurveDivide;
	CAD_FLOAT	fEntityThick;
	void	Read(CCAD_File &cFile)
	{
		cFile.ReadShort(&nLengthUnit);
		cFile.ReadShort(&nAngleUnit);
		cFile.ReadShort(&nCircleDivide);
		cFile.ReadShort(&nEllpDivide);
		cFile.ReadShort(&nCurveDivide);
		cFile.ReadCAD_Float(&fEntityThick);
	};
} SCAD_PlanInfo;


class   CCAD_Environment {
public:
	SCAD_PlanInfo	m_sPlanInfo;
	
	CCAD_Pen		m_cPen;						// current pen
	CCAD_Brush		m_cBrush;					// current brush

public:
	CCAD_Environment();
	virtual ~CCAD_Environment();

	void	Init();
	BOOL	Read(CCAD_File &cFile);
};

 

#endif // !defined(AFX_CAD_ENVIRONMENT_H__27C800C2_BB9F_11D4_B478_00A0D2F05B87__INCLUDED_)
