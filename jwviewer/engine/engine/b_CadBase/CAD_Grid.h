// CAD_Grid.h: interface for the CCAD_Grid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_GRID_H_)
#define AFX_CAD_GRID_H_

#include "CAD_Constance.h"
#include "CAD_Vector.h"

#define		GRID_DISPLAY		0x0001
#define		GRID_SNAP_FLAG		0x0002	

class   CCAD_Grid {
public:
	WORD		m_wFlag;
	COLORREF	m_rgbColor;				

	CCAD_Vector		m_cGridSize;
	CCAD_Vector		m_cGridStep;
	CCAD_Vector		m_cSnapStep;

public:
	CCAD_Grid();
	CCAD_Grid & operator=(const CCAD_Grid &);

	void	Read(CCAD_File & cFile);
};

 

#endif // !defined(AFX_CAD_GRID_H_)
