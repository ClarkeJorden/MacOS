// CAD_Brush.h: interface for the CCAD_Brush class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_BRUSH_H_)
#define AFX_CAD_BRUSH_H_

#include	"CAD_Constance.h"

class   CCAD_Brush {
public:
	COLORREF	m_rgbColor;
	short		m_nPattern;
public:
	CCAD_Brush();
	CCAD_Brush( const COLORREF &, const short &);
	CCAD_Brush( const CCAD_Brush & );
	
	CCAD_Brush & operator=( const CCAD_Brush & );
	BOOL operator==( const CCAD_Brush & ) const;

	void	Read(CCAD_File & cFile);
};

 

#endif // !defined(AFX_CAD_BRUSH_H_)
