#include "stdafx.h"
#include "CAD_AngularDimEntity.h"
#include "CAD_Doc.h"

CCAD_AngularDimEntity::CCAD_AngularDimEntity(CCAD_Doc* pDoc, short nSpaceDoc, short nUds)
	: CCAD_DimensionEntity(pDoc, nSpaceDoc, nUds), m_gemEntity(pDoc)
{
	m_bCode = C_DIMANGULAR;
	m_bFlag = E_DRAWING;
}

//---------------------------------------------
CCAD_AngularDimEntity::~CCAD_AngularDimEntity()
{
}

//---------------------------------------------
CCAD_AngularDimEntity & CCAD_AngularDimEntity::operator=(const CCAD_AngularDimEntity & cEntity)
{
	CCAD_Entity::operator =(cEntity);
	m_gemEntity = cEntity.m_gemEntity;
	return *this;
}
