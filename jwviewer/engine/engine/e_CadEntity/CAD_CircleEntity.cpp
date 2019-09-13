#include "stdafx.h"
#include "CAD_CircleEntity.h"
#include "CAD_Doc.h"

CCAD_CircleEntity::CCAD_CircleEntity(CCAD_Doc * pDoc, short nSpaceDoc, short nUds)
					: CCAD_Entity(pDoc, nSpaceDoc, nUds)
{
	m_bCode = C_CIRCLE;
	m_bFlag = E_DRAWING;
}

CCAD_CircleEntity::~CCAD_CircleEntity()
{
}

//---------------------------------------------
CCAD_CircleEntity::CCAD_CircleEntity(const CCAD_CircleEntity & cCircle)
{
	*this = cCircle;
}

//---------------------------------------------
CCAD_CircleEntity::CCAD_CircleEntity(const CCAD_ArcEntity & cArc)
{
	CCAD_Entity::operator =(cArc);
	m_bCode = C_CIRCLE; 
	m_cPen = cArc.m_cPen;
	m_cBrush = cArc.m_cBrush;
	m_cGemCircle.m_cBcs = cArc.m_cGemArc.m_cBcs;
	m_cGemCircle.m_cThick = cArc.m_cGemArc.m_cThick;
	m_cGemCircle.m_cCp = cArc.m_cGemArc.m_cCp;
	m_cGemCircle.m_fR = cArc.m_cGemArc.m_fR;
	m_cGemCircle.m_bDraw = cArc.m_cGemArc.m_bDraw;
}

//---------------------------------------------
CCAD_CircleEntity & CCAD_CircleEntity::operator=(const CCAD_CircleEntity & cCircle)
{
	CCAD_Entity::operator =(cCircle);
	m_cPen = cCircle.m_cPen;
	m_cBrush = cCircle.m_cBrush;
	m_cGemCircle = cCircle.m_cGemCircle;
	return *this;
}

//---------------------------------------------
void CCAD_CircleEntity::CvtArcEntity(CCAD_ArcEntity & cArc)
{
	cArc.m_nLayer = m_nLayer;
	cArc.m_nSpaceDoc = m_nSpaceDoc;
	cArc.m_nUds = m_nUds;
	cArc.m_pDoc = m_pDoc;
	cArc.m_cPen = m_cPen;
	cArc.m_cBrush = m_cBrush;
	cArc.m_cGemArc.m_cBcs = m_cGemCircle.m_cBcs;
	cArc.m_cGemArc.m_cThick = m_cGemCircle.m_cThick;
	cArc.m_cGemArc.m_cCp = m_cGemCircle.m_cCp;
	cArc.m_cGemArc.m_fR = m_cGemCircle.m_fR;
	cArc.m_cGemArc.m_bDraw = m_cGemCircle.m_bDraw;
	cArc.m_cGemArc.m_cAlfa = 0;
	cArc.m_cGemArc.m_cDelta = LONGDOUBLE_NINETY;
}

BOOL CCAD_CircleEntity::IsIgno(BOOL isBlock)
{
	if ( CCAD_Entity::IsIgno(isBlock) )
		return true;

	CCAD_GemCircle *pGem = (CCAD_GemCircle *)GetGemEntity();
	if (pGem->m_fR <= 0)
		return TRUE;
	return FALSE;
}

long CCAD_CircleEntity::GetSize()
{
	return (long)sizeof(CCAD_CircleEntity);
}

BOOL CCAD_CircleEntity::Read(CCAD_File& cFile)
{
	CCAD_Entity::Read(cFile);
	m_cPen.Read(cFile);
	m_cBrush.Read(cFile);
	return m_cGemCircle.Read(cFile);
}



