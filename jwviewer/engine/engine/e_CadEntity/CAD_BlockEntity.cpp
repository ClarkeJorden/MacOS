#include "stdafx.h"
#include "CAD_BlockEntity.h"
#include "CAD_Doc.h"

CCAD_BlockEntity::CCAD_BlockEntity(CCAD_Doc * pDoc, short nSpaceDoc, short nUds)
					: CCAD_Entity(pDoc, nSpaceDoc, nUds), m_cGemBlock(pDoc)
{
	m_bCode = C_BLOCK;
	m_bFlag = E_DRAWING;
	m_wPriority = 0;
}

CCAD_BlockEntity::~CCAD_BlockEntity()
{
}

CCAD_BlockEntity & CCAD_BlockEntity::operator=(const CCAD_BlockEntity & cEntity)
{
	CCAD_Entity::operator =(cEntity);
	m_cPen = cEntity.m_cPen;
	m_cBrush = cEntity.m_cBrush;
	m_cGemBlock = cEntity.m_cGemBlock;
	m_wPriority = cEntity.m_wPriority;
	return *this;
}

void CCAD_BlockEntity::MemCopyTo(CCAD_Entity * pEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	if(pEntity == NULL)
		return;
	::memmove(pEntity, this, ClassSize());
	
	CCAD_GemEntity *pGemEntity = GetGemEntity();
	pGemEntity->MemCopyTo(pEntity->GetGemEntity(), pData1, pData2, pData3);
}

CCAD_GemEntity * CCAD_BlockEntity::GetGemEntity()
{
	CCAD_Block * pBlock = NULL;
	if( m_pDoc != NULL)
		pBlock = (CCAD_Block *)m_pDoc->m_pMgrBlock->GetStyle(m_cGemBlock.m_szName);
	if (pBlock != NULL){
		pBlock->m_cMgrEntity.m_pDoc = m_pDoc;
		pBlock->m_cMgrEntity.m_nSpaceDoc = m_nSpaceDoc;
		pBlock->m_cMgrEntity.m_nUds = m_nUds;
	}
	m_cGemBlock.m_pDoc = m_pDoc;
	return &m_cGemBlock;
}

BOOL CCAD_BlockEntity::IsIgno(BOOL isBlock)
{
	if ( CCAD_Entity::IsIgno(isBlock) )
		return true;

	//CCAD_GemBlock *pGem = (CCAD_GemBlock *)GetGemEntity();
	//if (strlen(pGem->m_szName) <= 0)
	//	return TRUE;
	return FALSE;
}

long CCAD_BlockEntity::GetSize()
{
	return (long)sizeof(CCAD_BlockEntity);
}

BOOL CCAD_BlockEntity::Read(CCAD_File& cFile)
{
	CCAD_Entity::Read(cFile);
	cFile.ReadWord(&m_wPriority);
	m_cPen.Read(cFile);
	m_cBrush.Read(cFile);
	return m_cGemBlock.Read(cFile);
}

WORD CCAD_BlockEntity::GetPriority(WORD wState) const
{
	return (WORD) (m_wPriority & wState);
}

void CCAD_BlockEntity::SetPriority(WORD wState, BOOL blSet)
{
	WORD	wZeroState = (WORD) (0xffff ^ wState);
	m_wPriority &= wZeroState;
	if (blSet)
		m_wPriority |= wState;
}
