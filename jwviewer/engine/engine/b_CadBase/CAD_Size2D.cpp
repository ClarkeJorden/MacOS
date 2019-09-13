
#include "stdafx.h"
#include "CAD_Size2D.h"

 

CCAD_Size::CCAD_Size()
{
	m_fW = m_fH = 0;
}

CCAD_Size::CCAD_Size( const CAD_FLOAT & x, const CAD_FLOAT & y )
{
	m_fW = x;
	m_fH = y;
}

CCAD_Size::CCAD_Size (const CCAD_Size & s)
{
	m_fW = s.m_fW;
	m_fH = s.m_fH;
}

CCAD_Size & CCAD_Size::operator=(const CCAD_Size & s)
{
	m_fW = s.m_fW;
	m_fH = s.m_fH;
	return *this;
}

void	CCAD_Size::Read(CCAD_File & cFile)
{
	cFile.ReadCAD_Float(&m_fW);
	cFile.ReadCAD_Float(&m_fH);
}

 
