// CAD_DimensionEntity.cpp: implementation of the CCAD_DimensionEntity class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CAD_DimensionEntity.h"
#include "CAD_Doc.h"

//////////////////////////////////////////////////////////////////////
// CCAD_DimensionEntity class

CCAD_DimensionEntity::CCAD_DimensionEntity(CCAD_Doc* pDoc, short nSpaceDoc, short nUds)
	: CCAD_Entity(pDoc, nSpaceDoc, nUds)
{
}

CCAD_DimensionEntity::~CCAD_DimensionEntity()
{
}

//---------------------------------------------
BOOL CCAD_DimensionEntity::Read(CCAD_File& cFile)
{
	CCAD_Entity::Read(cFile);

	CCAD_GemDimension	*pGemDimension = GetGemDimension();
	ASSERT (pGemDimension != NULL);

	return pGemDimension->Read(cFile);
}

CCAD_GemEntity* CCAD_DimensionEntity::GetGemEntity()
{
	CCAD_GemDimension	*pGem = GetGemDimension();
	if (pGem == NULL)
		return NULL;
	CCAD_Block	*pBlock = (CCAD_Block *) m_pDoc->m_pMgrDimBlock->GetStyle(pGem->m_szName);
	if (pBlock != NULL){
		pBlock->m_cMgrEntity.m_pDoc = m_pDoc;
		pBlock->m_cMgrEntity.m_nSpaceDoc = m_nSpaceDoc;
		pBlock->m_cMgrEntity.m_nUds = m_nUds;
	}
	pGem->m_pDoc = m_pDoc;
	return pGem;
}

//---------------------------------------------
CCAD_Block* CCAD_DimensionEntity::CreateDimensionBlock()
{
	CCAD_Block* pBlock = new CCAD_Block(m_pDoc->m_hHeap, NULL);
	if(pBlock != NULL)
	{
		for(short i = 0; TRUE; i++)
		{
			sprintf(pBlock->m_szName, "*D%d", i);
			if(m_pDoc->m_pMgrDimBlock->GetStyle(pBlock->m_szName) == NULL)
				break;
		}

		CCAD_GemDimension* pGemEntity = (CCAD_GemDimension*) GetGemEntity();
		strcpy(pGemEntity->m_szName, pBlock->m_szName);
		pGemEntity->AttachElementToDimBlock(pBlock);

		if(!m_pDoc->m_pMgrDimBlock->Add(pBlock))
		{
			delete pBlock;
			pBlock = NULL;
		}
	}
	return pBlock;
}


