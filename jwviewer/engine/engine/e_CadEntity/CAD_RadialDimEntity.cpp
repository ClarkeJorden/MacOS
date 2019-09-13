#include "stdafx.h"
#include "CAD_RadialDimEntity.h"
#include "CAD_Doc.h"

CCAD_RadialDimEntity::CCAD_RadialDimEntity(CCAD_Doc* pDoc, short nSpaceDoc, short nUds)
	: CCAD_DimensionEntity(pDoc, nSpaceDoc, nUds), m_gemEntity(pDoc)
{
	m_bCode = C_DIMRADIAL;
	m_bFlag = E_DRAWING;
}

CCAD_RadialDimEntity::~CCAD_RadialDimEntity()
{
}

CCAD_RadialDimEntity & CCAD_RadialDimEntity::operator=(const CCAD_RadialDimEntity & cEntity)
{
	CCAD_Entity::operator =(cEntity);
	m_gemEntity = cEntity.m_gemEntity;
	return *this;
}
