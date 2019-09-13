#include "stdafx.h"
#include "CAD_Global.h"
#include "CAD_Doc.h"
#include "CAD_ArcEntity.h"

CCAD_ArcEntity::CCAD_ArcEntity(CCAD_Doc * pDoc, short nSpaceDoc, short nUds)
					: CCAD_Entity(pDoc, nSpaceDoc, nUds)
{
	m_bCode = C_ARC;
	m_bFlag = E_DRAWING;
}

CCAD_ArcEntity::~CCAD_ArcEntity()
{
}

//---------------------------------------------
CCAD_ArcEntity::CCAD_ArcEntity(const CCAD_ArcEntity & cArc)
{
	*this = cArc;
}

//---------------------------------------------
CCAD_ArcEntity & CCAD_ArcEntity::operator=(const CCAD_ArcEntity & cArc)
{
	CCAD_Entity::operator =(cArc);
	m_cPen = cArc.m_cPen;
	m_cBrush = cArc.m_cBrush;
	m_cGemArc = cArc.m_cGemArc;
	return *this;
}

BOOL CCAD_ArcEntity::IsIgno(BOOL isBlock)
{
	if ( CCAD_Entity::IsIgno(isBlock) )
		return true;

	CCAD_GemArc		*pGem = (CCAD_GemArc *)GetGemEntity();
	if (pGem->m_fR <= 0 || pGem->m_cDelta.GetAbsDegree() < CCAD_Global::GetAngleAccy())
		return TRUE;
	return FALSE;
}

long CCAD_ArcEntity::GetSize()
{
	return (long)sizeof(CCAD_ArcEntity);
}

BOOL CCAD_ArcEntity::Read(CCAD_File& cFile)
{
	CCAD_Entity::Read(cFile);
	m_cPen.Read(cFile);
	m_cBrush.Read(cFile);
	return m_cGemArc.Read(cFile);
}
