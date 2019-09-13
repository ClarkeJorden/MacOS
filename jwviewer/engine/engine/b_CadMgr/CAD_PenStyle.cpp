
#include "stdafx.h"
#include "CAD_Global.h"
#include "CAD_PenStyle.h"
#include "resource.h"


 

CCAD_PenStyle::CCAD_PenStyle()
{
	Init();
}

CCAD_PenStyle::CCAD_PenStyle(const CCAD_PenStyle *pStyle)
{
	CopyFrom(pStyle);
}


CCAD_PenStyle & CCAD_PenStyle::operator=(const CCAD_PenStyle &cStyle)
{
	CopyFrom(&cStyle);

	return *this;
}


void CCAD_PenStyle::Init()
{
	CCAD_Style::Init();

	m_nTotPart = 0;
	m_fLength = 0;
	m_fPart[0] = 0;
}


void CCAD_PenStyle::CopyFrom(const CCAD_Style *pStyle)
{
	CCAD_Style::CopyFrom(pStyle);

	CCAD_PenStyle	*pPen = (CCAD_PenStyle *) pStyle;
	m_nTotPart = pPen->m_nTotPart;
	m_fLength = pPen->m_fLength;
	short i;
	for (i = 0; i < m_nTotPart; i ++)
		m_fPart[i] = pPen->m_fPart[i];
}


BOOL CCAD_PenStyle::Read(CCAD_File & cFile)
{
	CCAD_Style::Read(cFile);

	cFile.ReadShort(&m_nTotPart);
	cFile.ReadCAD_Float(&m_fLength);
	for (int i=0; i<m_nTotPart; i++)
		cFile.ReadCAD_Float(&m_fPart[i]);
	return TRUE;
}


 
