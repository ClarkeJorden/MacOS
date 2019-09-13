#include "stdafx.h"
#include "CAD_Global.h"
#include "CAD_EllipseArcEntity.h"
#include "CAD_Doc.h"

CCAD_EllipseArcEntity::CCAD_EllipseArcEntity(CCAD_Doc * pDoc, short nSpaceDoc, short nUds)
					: CCAD_Entity(pDoc, nSpaceDoc, nUds)
{
	m_bCode = C_ELLPARC;
	m_bFlag = E_DRAWING;
}

CCAD_EllipseArcEntity::~CCAD_EllipseArcEntity()
{
}

CCAD_EllipseArcEntity::CCAD_EllipseArcEntity(const CCAD_EllipseArcEntity & cEllpArc)
{
	*this = cEllpArc;
}

CCAD_EllipseArcEntity & CCAD_EllipseArcEntity::operator=(const CCAD_EllipseArcEntity & cEllpArc)
{
	CCAD_Entity::operator =(cEllpArc);
	m_cPen = cEllpArc.m_cPen;
	m_cBrush = cEllpArc.m_cBrush;
	m_cGemEllipseArc = cEllpArc.m_cGemEllipseArc;
	return *this;
}

BOOL CCAD_EllipseArcEntity::IsIgno(BOOL isBlock)
{
	if ( CCAD_Entity::IsIgno(isBlock) )
		return true;

	CCAD_GemEllipseArc *pGem = (CCAD_GemEllipseArc *)GetGemEntity();

	if (pGem->m_fRx <= 0 || pGem->m_fRy <= 0 || pGem->m_cDelta.GetAbsDegree() < CCAD_Global::GetAngleAccy())
		return TRUE;
	return FALSE;
}

long CCAD_EllipseArcEntity::GetSize()
{
	return (long)sizeof(CCAD_EllipseArcEntity);
}

BOOL CCAD_EllipseArcEntity::Read(CCAD_File& cFile)
{
	CCAD_Entity::Read(cFile);
	m_cPen.Read(cFile);
	m_cBrush.Read(cFile);
	return m_cGemEllipseArc.Read(cFile);
}
