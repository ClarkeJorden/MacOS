// CAD_Size2D.h: interface for the CCAD_Size2D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_SIZE2D_H_)
#define AFX_CAD_SIZE2D_H_

#include "CAD_Constance.h"

class   CCAD_Size {
public:
	CAD_FLOAT	m_fW;
	CAD_FLOAT	m_fH;

public:
	CCAD_Size();
	CCAD_Size( const CAD_FLOAT & x, const CAD_FLOAT & y );
	CCAD_Size (const CCAD_Size & s);
	CCAD_Size & operator=(const CCAD_Size & s);

	void	Read(CCAD_File & cFile);
};

#endif // !defined(AFX_CAD_SIZE2D_H_)
