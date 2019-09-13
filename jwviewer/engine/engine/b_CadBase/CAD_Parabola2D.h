// CAD_Parabola2D.h: interface for the CCAD_Parabola2D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_PARABOLA2D_H_)
#define AFX_CAD_PARABOLA2D_H_

#include	"CAD_Constance.h"

class   CCAD_Parabola2D  {
protected:
	CAD_FLOAT	m_fA;
	CAD_FLOAT	m_fB;
	CAD_FLOAT	m_fC;
	CAD_FLOAT	m_fK;
	CAD_FLOAT	m_fZ;

public:
	CCAD_Parabola2D();
	void	SetPara(const CCAD_Vector & cPt, const CAD_FLOAT & dHy);
	void	SetPara(const CCAD_Vector & cCp, const CAD_FLOAT & dR, const CAD_FLOAT & dHy, BYTE bPositive);
	void	CountPt(CCAD_Vector & cPt, const CAD_FLOAT & dX) const;

	friend class CCAD_Equation2D;
};

 

#endif // !defined(AFX_CAD_PARABOLA2D_H_)
