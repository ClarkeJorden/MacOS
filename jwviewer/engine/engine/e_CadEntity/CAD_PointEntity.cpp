#include "stdafx.h"
#include "CAD_PointEntity.h"
#include "CAD_Doc.h"

CCAD_PointEntity::CCAD_PointEntity(CCAD_Doc * pDoc, short nSpaceDoc, short nUds)
					: CCAD_Entity(pDoc, nSpaceDoc, nUds)
{
	m_bCode = C_POINT;
	m_bFlag = E_DRAWING;
}

CCAD_PointEntity::~CCAD_PointEntity()
{
}

CCAD_PointEntity::CCAD_PointEntity(const CCAD_PointEntity & cPoint)
{
	*this = cPoint;
}

CCAD_PointEntity & CCAD_PointEntity::operator=(const CCAD_PointEntity & cPoint)
{
	CCAD_Entity::operator =(cPoint);
	m_cPen = cPoint.m_cPen;
	m_cGemPoint = cPoint.m_cGemPoint;
	return *this;
}

BOOL CCAD_PointEntity::IsIgno(BOOL isBlock)
{
	if ( CCAD_Entity::IsIgno(isBlock) )
		return true;

	CCAD_GemPoint *pGem = (CCAD_GemPoint *)GetGemEntity();

	if (pGem->m_fR < 0)
		return TRUE;
	return FALSE;
}

long CCAD_PointEntity::GetSize()
{
	return (long)sizeof(CCAD_PointEntity);
}

BOOL CCAD_PointEntity::Read(CCAD_File& cFile)
{
	CCAD_Entity::Read(cFile);
	m_cPen.Read(cFile);
	return m_cGemPoint.Read(cFile);
}

BOOL CCAD_PointEntity::Draw(CCAD_DC* pCadDC, BOOL blRedraw)
{
	ASSERT(pCadDC != NULL);
	
	CCAD_GemEntity* pGemEntity = GetGemEntity();
	if (pGemEntity == NULL)
		return TRUE;
	
	//	set pen.
	LOGPEN	sLogPen;
	LOGPEN oldPen;
	bool blGet;
	if ( blGet = GetLogPen(pCadDC, sLogPen) )
		 oldPen = pCadDC->SelectPen(sLogPen);
	//	set brush.
	LOGBRUSH oldBrush = pCadDC->SelectColorBrush(sLogPen.lopnColor);
	BOOL blResult = pGemEntity->Drawing(pCadDC, E_DRAWING, blRedraw);
	
	pCadDC->RestoreObject(oldBrush);
	
	if ( blGet )
		pCadDC->RestoreObject(oldPen);
	return blResult;
}
