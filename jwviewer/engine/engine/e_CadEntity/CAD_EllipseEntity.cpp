#include "stdafx.h"
#include "CAD_EllipseEntity.h"
#include "CAD_Doc.h"

CCAD_EllipseEntity::CCAD_EllipseEntity(CCAD_Doc * pDoc, short nSpaceDoc, short nUds)
					: CCAD_Entity(pDoc, nSpaceDoc, nUds)
{
	m_bCode = C_ELLIPSE;
	m_bFlag = E_DRAWING;
}

CCAD_EllipseEntity::~CCAD_EllipseEntity()
{
}

CCAD_EllipseEntity::CCAD_EllipseEntity(const CCAD_EllipseEntity & cEllipse)
{
	*this = cEllipse;
}

CCAD_EllipseEntity::CCAD_EllipseEntity(const CCAD_EllipseArcEntity & cEllipseArc)
{
	CCAD_Entity::operator =(cEllipseArc);
	m_bCode = C_ELLIPSE;
	m_cPen = cEllipseArc.m_cPen;
	m_cBrush = cEllipseArc.m_cBrush;
	m_cGemEllipse.m_cBcs = cEllipseArc.m_cGemEllipseArc.m_cBcs;
	m_cGemEllipse.m_cThick = cEllipseArc.m_cGemEllipseArc.m_cThick;
	m_cGemEllipse.m_bDraw = cEllipseArc.m_cGemEllipseArc.m_bDraw;
	m_cGemEllipse.m_cCp =  cEllipseArc.m_cGemEllipseArc.m_cCp;
	m_cGemEllipse.m_fRx = cEllipseArc.m_cGemEllipseArc.m_fRx;
	m_cGemEllipse.m_fRy = cEllipseArc.m_cGemEllipseArc.m_fRy;
	m_cGemEllipse.m_cDirect = cEllipseArc.m_cGemEllipseArc.m_cDirect;
}

CCAD_EllipseEntity & CCAD_EllipseEntity::operator=(const CCAD_EllipseEntity & cEllipse)
{
	CCAD_Entity::operator =(cEllipse);
	m_cPen = cEllipse.m_cPen;
	m_cBrush = cEllipse.m_cBrush;
	m_cGemEllipse = cEllipse.m_cGemEllipse;
	return *this;
}

void CCAD_EllipseEntity::CvtEllipseArcEntity(CCAD_EllipseArcEntity & cEllipseArc)
{
	cEllipseArc.m_nLayer = m_nLayer;
	cEllipseArc.m_nSpaceDoc = m_nSpaceDoc;
	cEllipseArc.m_nUds = m_nUds;
	cEllipseArc.m_pDoc = m_pDoc;
	cEllipseArc.m_cPen = m_cPen;
	cEllipseArc.m_cBrush = m_cBrush;
	cEllipseArc.m_cGemEllipseArc.m_cBcs = m_cGemEllipse.m_cBcs;
	cEllipseArc.m_cGemEllipseArc.m_cThick = m_cGemEllipse.m_cThick;
	cEllipseArc.m_cGemEllipseArc.m_cCp = m_cGemEllipse.m_cCp;
	cEllipseArc.m_cGemEllipseArc.m_fRx = m_cGemEllipse.m_fRx;
	cEllipseArc.m_cGemEllipseArc.m_fRy = m_cGemEllipse.m_fRy;
	cEllipseArc.m_cGemEllipseArc.m_bDraw = m_cGemEllipse.m_bDraw;
	cEllipseArc.m_cGemEllipseArc.m_cDirect = m_cGemEllipse.m_cDirect;
	cEllipseArc.m_cGemEllipseArc.m_cAlfa = 0;
	cEllipseArc.m_cGemEllipseArc.m_cDelta = LONGDOUBLE_NINETY;
}

BOOL CCAD_EllipseEntity::IsIgno(BOOL isBlock)
{
	if ( CCAD_Entity::IsIgno(isBlock) )
		return true;

	CCAD_GemEllipseArc *pGem = (CCAD_GemEllipseArc *)GetGemEntity();

	if (pGem->m_fRx <= 0 || pGem->m_fRy <= 0)
		return TRUE;
	return FALSE;
}

long CCAD_EllipseEntity::GetSize()
{
	return (long)sizeof(CCAD_EllipseEntity);
}

BOOL CCAD_EllipseEntity::Read(CCAD_File& cFile)
{
	CCAD_Entity::Read(cFile);
	m_cPen.Read(cFile);
	m_cBrush.Read(cFile);
	return m_cGemEllipse.Read(cFile);
}
