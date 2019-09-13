// CAD_Equation2D.h: interface for the CCAD_Equation2D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_EQUATION2D_H_)
#define AFX_CAD_EQUATION2D_H_

#include	"CAD_Constance.h"

class   CCAD_Equation2D  {
protected:
	CAD_FLOAT		m_fA;
	CAD_FLOAT		m_fB;
	CAD_FLOAT		m_fC;

public:
	BYTE		m_bBase;

public:
	CCAD_Equation2D();
	CCAD_Equation2D(CAD_FLOAT & a, CAD_FLOAT & b, CAD_FLOAT & c);
	BOOL	SetPara(const CCAD_Curve2D & inCurve, BYTE bBase, const CAD_FLOAT & dV);
	BOOL	SetPara(const CCAD_Curve2D & inCurve, const CCAD_Linear2D & cLinear);
	BOOL	SetPara(const CCAD_Parabola2D & cParabola, const CCAD_Linear2D & cLinear);
	BOOL	SetPara(const CCAD_Parabola2D & cPara1, const CCAD_Parabola2D & cPara2);
	BYTE	CountRoot(CAD_FLOAT & dV1, CAD_FLOAT & dV2) const;
};

 

#endif // !defined(AFX_CAD_EQUATION2D_H_)
