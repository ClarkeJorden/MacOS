
#include "stdafx.h"
#include "CAD_Color.h"

 

CCAD_Color::CCAD_Color()
{
	m_bRed = 0xff;
	m_bBlue = 0xff;
	m_bGreen = 0xff;
}

CCAD_Color::~CCAD_Color()
{
}

CCAD_Color & CCAD_Color::operator=(const CCAD_Color &cColor)
{
	m_bRed = cColor.m_bRed;
	m_bGreen = cColor.m_bGreen;
	m_bBlue = cColor.m_bBlue;
	return *this;
}

BOOL CCAD_Color::operator==( const CCAD_Color &cColor ) const
{
	return ( m_bRed == cColor.m_bRed && m_bGreen == cColor.m_bGreen && m_bBlue == cColor.m_bBlue );
}

void CCAD_Color::Read(CCAD_File & cFile)
{	
	cFile.ReadByte(&m_bRed);
	cFile.ReadByte(&m_bGreen);
	cFile.ReadByte(&m_bBlue);
}

COLORREF CCAD_Color::GetColorRef()
{
	return RGB(m_bRed, m_bGreen, m_bBlue);
}

void CCAD_Color::SetColor(const COLORREF & rgb)
{
	m_bBlue = (BYTE)((rgb & 0xff0000) >> 16);
	m_bGreen = (BYTE)((rgb & 0x00ff00) >> 8);
	m_bRed = (BYTE)(rgb & 0x0000ff);
}

 
