#include "stdafx.h"
#include "CAD_PaintEntity.h"
#include "CAD_Doc.h"


CCAD_PaintEntity::CCAD_PaintEntity(CCAD_Doc * pDoc, short nSpaceDoc, short nUds)
					: CCAD_Entity(pDoc, nSpaceDoc, nUds)
{
	m_bCode = C_PAINT;
	m_bFlag = E_DRAWING;
}

CCAD_PaintEntity::~CCAD_PaintEntity()
{
}

BOOL CCAD_PaintEntity::IsIgno(BOOL isBlock)
{
	if ( CCAD_Entity::IsIgno(isBlock) )
		return true;

	CCAD_GemPaint *pGem = (CCAD_GemPaint *)GetGemEntity();

	if (pGem->m_nTotGrp <= 0 || pGem->m_nTotPt < 3 || pGem->m_pNum == NULL ||
			pGem->m_pPt == NULL)
		return TRUE;
	return FALSE;
}

long CCAD_PaintEntity::GetSize()
{
	long lSize = sizeof(CCAD_PaintEntity);
	lSize += sizeof(CCAD_Point)*m_cGemPaint.m_nTotPt;
	lSize += sizeof(short)*m_cGemPaint.m_nTotGrp;
	return lSize;
}

LPSTR CCAD_PaintEntity::GetData1(LPSTR pEntity)
{
	return (pEntity + sizeof(CCAD_PaintEntity));
}

LPSTR CCAD_PaintEntity::GetData2(LPSTR pEntity)
{
	long	lSize = sizeof(CCAD_PaintEntity);
	lSize += sizeof(short)*m_cGemPaint.m_nTotGrp;
	return (pEntity + lSize);
}

BOOL CCAD_PaintEntity::Read(CCAD_File& cFile)
{
	CCAD_Entity::Read(cFile);
	m_cPen.Read(cFile);
	m_cBrush.Read(cFile);
	return m_cGemPaint.Read(cFile);
}
