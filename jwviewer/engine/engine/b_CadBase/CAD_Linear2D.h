// Linear2D.h: interface for the CCAD_Linear2D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEAR2D_H_)
#define AFX_LINEAR2D_H_

#include	"CAD_Constance.h"
#include	"CAD_Angle.h"
#include	"CAD_Vector.h"

class   CCAD_Linear2D  {
protected:
	CAD_FLOAT		m_fA;
	CAD_FLOAT		m_fB;
	CAD_FLOAT		m_fK;
	CAD_FLOAT		m_fZ;

public:
	CCAD_Linear2D();
	CCAD_Linear2D(const CCAD_Point & cPt1, const CCAD_Point & cPt2);
	CCAD_Linear2D(const CCAD_Vector & cPt, const CCAD_Angle & cDirect);

	void	SetPara(const CCAD_Point & cPt1, const CCAD_Point & cPt2);
	void	SetPara(const CCAD_Vector & cPt, const CCAD_Angle & cDirect);
	void	SetParaFromDirect(const CCAD_Point & cOrg, const CCAD_Vector & cDirect);
	void	SetParaTwoCircle(const CCAD_Vector & cCp1, const CAD_FLOAT & dR1, const CCAD_Vector & cCp2, const CAD_FLOAT & dR2);
	BOOL	Horizontal() const;
	BOOL	Vertical() const;
	BOOL	CountHorizontalPt(CCAD_Vector & cPt, CAD_FLOAT & dY) const;
	BOOL	CountVerticalPt(CCAD_Vector & cPt, CAD_FLOAT & dX) const;
	BOOL	CountCrossPt(CCAD_Vector & cPt, CCAD_Linear2D & CCAD_Linear2D) const;
	BOOL	IsPtOn(const CCAD_Point &cPt);

	friend class CCAD_Equation2D;
	friend class CCAD_CirclePara1;
};

 

#endif // !defined(AFX_LINEAR2D_H_)
