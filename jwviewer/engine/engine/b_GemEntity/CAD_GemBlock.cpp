#include "stdafx.h"
#include "CAD_GemBlock.h"
#include "CAD_Doc.h"

CCAD_GemBlock::CCAD_GemBlock(CCAD_Doc * pDoc)
{
	m_pDoc = pDoc;
	Init();
}

CCAD_GemBlock::~CCAD_GemBlock()
{
	;
}

void CCAD_GemBlock::Init()
{
	m_szName[0] = '\0';
    m_nNumber = DISABLE_INDEX;
}

//---------------------------------------------
CCAD_Block * CCAD_GemBlock::GetBlock()
{
	if ( m_pDoc == NULL )
		return NULL;
	return (CCAD_Block *)m_pDoc->m_pMgrBlock->GetStyle(m_szName);
}


//---------------------------------------------
void CCAD_GemBlock::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_Gem3DEntity::MemCopyTo(pGemEntity, pData1, pData2, pData3);
	sprintf(((CCAD_GemBlock *)pGemEntity)->m_szName, "%s", m_szName);
	((CCAD_GemBlock *)pGemEntity)->m_pDoc = m_pDoc;
}

//---------------------------------------------
BOOL CCAD_GemBlock::Read(CCAD_File & cFile)
{
	CCAD_Gem3DEntity::Read(cFile);
	
	cFile.ReadString(m_szName, EXT_LENGTH_NAME);

	cFile.ReadCAD_Float(&m_cBcs.m_fScale);
	if ( m_cBcs.m_fScale < CAD_ZERO )
		m_cBcs.m_fScale = 1.0F;
	return true;
}

//---------------------------------------------
BOOL CCAD_GemBlock::Draw(CCAD_DC *pDC, BOOL blRedraw)
{
	CCAD_Block * pBlock = GetBlock();
	if ( pBlock == NULL )
		return false;

	long pos;
	CCAD_Entity	* pEntity = pBlock->m_cMgrEntity.GetFirstEntity(pos);
	while (pEntity != NULL) {
		pEntity->SetBlockEntityPara(&pBlock->m_cMgrEntity);
		pEntity->Draw(pDC, blRedraw);
		pEntity = pBlock->m_cMgrEntity.GetNextEntity(pos);
	}
	return true;
}

//----------------------------------------------------------
void CCAD_GemBlock::CountBodyRange(CCAD_Rect &cRange)
{
	CCAD_Block * pBlock = GetBlock();
	if ( pBlock == NULL )
		return;

	long pos;
	CCAD_Entity	* pEntity = pBlock->m_cMgrEntity.GetFirstEntity(pos);
	while (pEntity != NULL) {
		CCAD_Rect	cRc;
		cRc.Init();
		pEntity->CountBodyRange(cRc);

		if (!cRc.IsInited() ) {
			CCAD_Coord3D *pBCS = pEntity->GetGemEntity()->GetBCS();
			ASSERT(pBCS != NULL);
			SCAD_POINT	pt[8] = {
				{cRc.m_fMinX, cRc.m_fMinY, cRc.m_fMinZ}, 
				{cRc.m_fMaxX, cRc.m_fMinY, cRc.m_fMinZ},
				{cRc.m_fMaxX, cRc.m_fMaxY, cRc.m_fMinZ},
				{cRc.m_fMinX, cRc.m_fMaxY, cRc.m_fMinZ},
				{cRc.m_fMinX, cRc.m_fMinY, cRc.m_fMaxZ}, 
				{cRc.m_fMaxX, cRc.m_fMinY, cRc.m_fMaxZ},
				{cRc.m_fMaxX, cRc.m_fMaxY, cRc.m_fMaxZ},
				{cRc.m_fMinX, cRc.m_fMaxY, cRc.m_fMaxZ}};

			CCAD_Point	ptTemp;
			int	i;
			for (i = 0; i < 8; i++)	{
				ptTemp = pt[i];
				pBCS->ConvToBase(ptTemp);
				cRange.ResetParameter(ptTemp);
			}
		}
		pEntity = pBlock->m_cMgrEntity.GetNextEntity(pos);
	}
}

//----------------------------------------------------------
void CCAD_GemBlock::CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect &cRange)
{
	CCAD_Block * pBlock = GetBlock();
	if ( pBlock == NULL )
		return;

	long pos;
	CCAD_Entity	* pEntity = pBlock->m_cMgrEntity.GetFirstEntity(pos);
	while (pEntity != NULL) {
		pEntity->CountVcsRange(pCadDC, cRange);
		pEntity = pBlock->m_cMgrEntity.GetNextEntity(pos);
	}
}

//----------------------------------------------------------
BOOL CCAD_GemBlock::IsRender()
{
	return (GetBlock() != NULL);
}
