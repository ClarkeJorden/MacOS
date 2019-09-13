// CAD_Sheet.h: interface for the CCAD_Sheet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_SHEET_H)
#define AFX_CAD_SHEET_H


#include	"CAD_Vector.h"
#include	"CAD_Size2D.h"
#include	"CAD_Rect.h"

 

#define		SHEET_LANDSCAPE			0x00
#define		SHEET_PORTRAIT			0x01
#define		SHEET_BACK_BLACK		0
#define		SHEET_BACK_WHITE		1
#define		SHEET_SHOW_FRAME		0x01

class   CCAD_Sheet  {
public:
	BYTE		m_bOrient;
	BYTE		m_bShowFlag;
	BYTE		m_bBackColor;
	CAD_FLOAT	m_fScale;
	CCAD_Size	m_cSize;
	CCAD_Rect	m_cFrame;
	CCAD_Rect	m_cLimit;

public:
	CCAD_Sheet();
	virtual ~CCAD_Sheet();
	CCAD_Sheet & operator =(const CCAD_Sheet &);

	void	Read( CCAD_File & file );
	
};

 

#endif
