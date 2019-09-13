#include "stdafx.h"
#include "CAD_Sheet.h"

 CCAD_Sheet::CCAD_Sheet()
{
	m_bOrient = SHEET_LANDSCAPE;
	m_bShowFlag = SHEET_SHOW_FRAME;
	m_bBackColor = SHEET_BACK_WHITE;
	m_fScale = 100;
	m_cSize.m_fH = 21000;
	m_cSize.m_fW = 29700;
	m_cFrame.m_fMinX = 0;
	m_cFrame.m_fMinY = 0;
	m_cFrame.m_fMaxX = 0;
	m_cFrame.m_fMaxY = 0;
	m_cLimit.m_fMinX = 0;
	m_cLimit.m_fMinY = 0;
	m_cLimit.m_fMaxX = m_cSize.m_fW;
	m_cLimit.m_fMaxY = m_cSize.m_fH;
}

CCAD_Sheet::~CCAD_Sheet()
{
}

CCAD_Sheet &CCAD_Sheet::operator =(const CCAD_Sheet &cSheet)
{
	m_bOrient = cSheet.m_bOrient;
	m_bShowFlag = cSheet.m_bShowFlag;
	m_bBackColor = cSheet.m_bBackColor;
	m_fScale = cSheet.m_fScale;
	m_cSize = cSheet.m_cSize;
	m_cFrame = cSheet.m_cFrame;
	m_cLimit = cSheet.m_cLimit;

	return *this;
}

void	CCAD_Sheet::Read( CCAD_File & file )
{
	file.ReadByte(&m_bOrient);
	file.ReadByte(&m_bShowFlag);
	file.ReadByte(&m_bBackColor);
	file.ReadCAD_Float(&m_fScale);
	m_cSize.Read(file);
	m_cFrame.Read(file);
	m_cLimit.Read(file);
}

 
