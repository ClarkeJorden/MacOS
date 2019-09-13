
#include "stdafx.h"
#include "CAD_Constance.h"
#include "CAD_Grid.h"


 

CCAD_Grid::CCAD_Grid()
{
	m_cGridSize.m_fX = CAMERA_WIDTH / 2;
	m_cGridSize.m_fY = m_cGridSize.m_fX;
	m_cGridSize.m_fZ = m_cGridSize.m_fX;

	m_cGridStep.m_fX = (CAD_FLOAT)CAMERA_GRID_WIDTH;
	m_cGridStep.m_fY = (CAD_FLOAT)CAMERA_GRID_WIDTH;
	m_cGridStep.m_fZ = (CAD_FLOAT)CAMERA_GRID_WIDTH;

	m_cSnapStep.m_fX = (CAD_FLOAT)CAMERA_SNAP_WIDTH;
	m_cSnapStep.m_fY = (CAD_FLOAT)CAMERA_SNAP_WIDTH;
	m_cSnapStep.m_fZ = (CAD_FLOAT)CAMERA_SNAP_WIDTH;	

	m_rgbColor = RGB(0, 0, 255);
	m_wFlag = GRID_DISPLAY;
}

CCAD_Grid & CCAD_Grid::operator=(const CCAD_Grid & cOther)
{
	m_cGridSize = cOther.m_cGridSize;
	m_cGridStep = cOther.m_cGridStep;
	m_cSnapStep = cOther.m_cSnapStep;

	m_rgbColor = cOther.m_rgbColor;
	m_wFlag = cOther.m_wFlag;
	return *this;
}

void	CCAD_Grid::Read(CCAD_File & cFile)
{
	cFile.ReadWord(&m_wFlag);
	cFile.ReadColorRef(&m_rgbColor);

	m_cGridSize.Read(cFile);
	m_cGridStep.Read(cFile);
	m_cSnapStep.Read(cFile);
}

 
