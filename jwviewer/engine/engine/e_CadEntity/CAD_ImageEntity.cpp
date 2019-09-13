#include "stdafx.h"
#include "CAD_ImageEntity.h"
#include "CAD_Doc.h"

CCAD_ImageEntity::CCAD_ImageEntity(CCAD_Doc * pDoc, short nSpaceDoc, short nUds)
					: CCAD_Entity(pDoc, nSpaceDoc, nUds)
{
	m_bCode = C_IMAGE;
	m_bFlag = E_DRAWING;
}

CCAD_ImageEntity::~CCAD_ImageEntity()
{
}

BOOL CCAD_ImageEntity::IsIgno(BOOL isBlock)
{
	if ( CCAD_Entity::IsIgno(isBlock) )
		return true;

	CCAD_GemImage *pGem = (CCAD_GemImage *)GetGemEntity();

	if (pGem->m_nIndex == -1 || pGem->m_nPtNum <= 2 || 
			pGem->m_pImage == NULL || pGem->m_pPt == NULL )
		return TRUE;
	return FALSE;
}

long CCAD_ImageEntity::GetSize()
{
	long lSize = sizeof(CCAD_ImageEntity);
	lSize += sizeof(CCAD_Point)*m_cGemImage.m_nPtNum;
	return lSize;
}

CCAD_GemEntity* CCAD_ImageEntity::GetGemEntity()
{
	if ( m_pDoc != NULL )
		m_cGemImage.m_pImage = (CCAD_Image *)m_pDoc->m_pImageMgr->GetStyle(m_cGemImage.m_nIndex);
	return &m_cGemImage;
}

LPSTR CCAD_ImageEntity::GetData1(LPSTR pEntity)
{
	return (pEntity + sizeof(CCAD_ImageEntity));
}

BOOL CCAD_ImageEntity::Read(CCAD_File& cFile)
{
	CCAD_Entity::Read(cFile);
	m_cPen.Read(cFile);
	return m_cGemImage.Read(cFile);
}
