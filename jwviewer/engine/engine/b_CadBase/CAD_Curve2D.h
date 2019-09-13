// CAD_Curve2D.h: interface for the CCAD_Curve2D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_CURVE2D_H_)
#define AFX_CAD_CURVE2D_H_

#include	"CAD_Constance.h"

class   CCAD_Curve2D  {
protected:
	CAD_FLOAT	m_fA;
	CAD_FLOAT	m_fB;
	CAD_FLOAT	m_fC;
	CAD_FLOAT	m_fD;
	CAD_FLOAT	m_fE;
	CAD_FLOAT	m_fK;

public:
	CCAD_Curve2D();
	void	SetPara(const CCAD_Vector &, const CAD_FLOAT &);
	void	SetPara(const CCAD_Vector &, const CAD_FLOAT &, const CAD_FLOAT &, const CCAD_Angle &);

	friend class CCAD_Equation2D;
	friend class CCAD_CirclePara2;
};

 

#endif // !defined(AFX_CAD_CURVE2D_H_)
