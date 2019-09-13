// CAD_Pen.h: interface for the CCAD_Pen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_PEN_H_)
#define AFX_CAD_PEN_H_

#include "CAD_Constance.h"

class   CCAD_Pen {
public:
	COLORREF		m_rgbColor;
	short			m_nStyle;
	short			m_nThick;
	CAD_FLOAT		m_fScale;

public:
	CCAD_Pen();
	CCAD_Pen( const COLORREF &, const short &, const short &, const CAD_FLOAT &);
	CCAD_Pen( const CCAD_Pen & );

	CCAD_Pen & operator=( const CCAD_Pen & );
	BOOL 	operator==( const CCAD_Pen & ) const ;

	void	CopyFrom(const CCAD_Pen *pPen);
	void	Read(CCAD_File & cFile);
};

 

#endif // !defined(AFX_CAD_PEN_H_)
