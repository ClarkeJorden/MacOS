
#include "stdafx.h"
//#include <math.h>
#include "CAD_Pen.h"

 

CCAD_Pen::CCAD_Pen()
{
	m_rgbColor = RGB(0, 0, 0);
	m_nStyle = 0;
	m_nThick = 0;
	m_fScale = 1.0F;
}

CCAD_Pen::CCAD_Pen( const COLORREF &rgbColor, const short &nStyle, const short &nThick, const CAD_FLOAT &dScale)
{
	m_rgbColor = rgbColor;
	m_nStyle = nStyle;
	m_nThick = nThick;
	m_fScale = dScale;
}

CCAD_Pen::CCAD_Pen( const CCAD_Pen &cPen )
{
	CopyFrom(&cPen);
}

CCAD_Pen & CCAD_Pen::operator=(const CCAD_Pen &cPen)
{
	CopyFrom(&cPen);

	return *this;
}

BOOL CCAD_Pen::operator==( const CCAD_Pen &cPen ) const
{
	if (m_rgbColor == cPen.m_rgbColor && m_nStyle == cPen.m_nStyle && m_nThick == cPen.m_nThick && m_fScale == cPen.m_fScale )
		return( TRUE );
	return( FALSE );
}

void CCAD_Pen::CopyFrom(const CCAD_Pen *pPen)
{
	m_rgbColor = pPen->m_rgbColor;
	m_nStyle = pPen->m_nStyle;
	m_nThick = pPen->m_nThick;
	m_fScale = pPen->m_fScale;
}

void CCAD_Pen::Read(CCAD_File & cFile)
{	
	cFile.ReadColorRef(&m_rgbColor);
	cFile.ReadShort(&m_nStyle);
	cFile.ReadShort(&m_nThick);
	cFile.ReadCAD_Float(&m_fScale);
	if (fabs(m_fScale) < CAD_ZERO)
		m_fScale = 1.0F;
}

 
