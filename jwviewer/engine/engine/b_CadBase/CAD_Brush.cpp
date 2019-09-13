#include "stdafx.h"
#include "CAD_Brush.h"


 

CCAD_Brush::CCAD_Brush()
{
	m_rgbColor = RGB(0,0,255);
	m_nPattern = BRS_SOLID;
}

CCAD_Brush::CCAD_Brush( const COLORREF &rgbColor, const short &nPattern )
{
	m_rgbColor = rgbColor;
	m_nPattern = nPattern;
}

CCAD_Brush::CCAD_Brush( const CCAD_Brush &cBrush )
{
	m_rgbColor = cBrush.m_rgbColor;
	m_nPattern = cBrush.m_nPattern;
}

CCAD_Brush & CCAD_Brush::operator=(const CCAD_Brush &cBrush)
{
	m_rgbColor = cBrush.m_rgbColor;
	m_nPattern = cBrush.m_nPattern;
	return *this;
}

BOOL CCAD_Brush::operator==( const CCAD_Brush &cBrush ) const
{
	if (cBrush.m_rgbColor == m_rgbColor && cBrush.m_nPattern == m_nPattern)
		return( TRUE );
	return FALSE;
}

void CCAD_Brush::Read(CCAD_File & cFile)
{	
	cFile.ReadColorRef(&m_rgbColor);
	cFile.ReadShort(&m_nPattern);
}

 
