// CAD_Color.h: interface for the CCAD_Color class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_COLOR_H_)
#define AFX_CAD_COLOR_H_

#include "CAD_File.h"

class   CCAD_Color {
public:
	BYTE	m_bRed;
	BYTE	m_bBlue;
	BYTE	m_bGreen;

public:
	CCAD_Color();
	CCAD_Color(BYTE bRed, BYTE bGreen, BYTE bBlue)
		{m_bRed = bRed; m_bGreen = bGreen;  m_bBlue = bBlue;}
	virtual ~CCAD_Color();

	CCAD_Color & operator=( const CCAD_Color & );
	BOOL operator==( const CCAD_Color & ) const;

	void		Read(CCAD_File & cFile);
	COLORREF	GetColorRef();
	void		SetColor(const COLORREF & rgb);
};

 

#endif // !defined(AFX_CAD_COLOR_H_)
