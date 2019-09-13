#include "stdafx.h"
#include "CAD_BeltEntity.h"
#include "CAD_Doc.h"

CCAD_BeltEntity::CCAD_BeltEntity(CCAD_Doc * pDoc, short nSpaceDoc, short nUds)
					: CCAD_Entity(pDoc, nSpaceDoc, nUds)
{
	m_bCode = C_BELT;
	m_bFlag = E_DRAWING;
}

CCAD_BeltEntity::~CCAD_BeltEntity()
{

}

BOOL CCAD_BeltEntity::Read(CCAD_File & cFile)
{
	CCAD_Entity::Read(cFile);
	m_cPen.Read(cFile);
	m_cBrush.Read(cFile);
	return m_cGemBelt.Read(cFile);
}

BOOL CCAD_BeltEntity::IsIgno(BOOL isBlock)
{
	if ( CCAD_Entity::IsIgno(isBlock) )
		return true;

	CCAD_GemBelt	*pGem = (CCAD_GemBelt *)GetGemEntity();
	if (pGem->m_pVertex == NULL || pGem->m_nTot < 2)
		return TRUE;
	return FALSE;
}

long CCAD_BeltEntity::GetSize()
{
	long lSize = sizeof(CCAD_BeltEntity);
	lSize += sizeof(CCAD_GemBvertex)*m_cGemBelt.m_nTot;
	return lSize;
}

LPSTR CCAD_BeltEntity::GetData1(LPSTR pEntity)
{
	return (pEntity + sizeof(CCAD_BeltEntity));
}
