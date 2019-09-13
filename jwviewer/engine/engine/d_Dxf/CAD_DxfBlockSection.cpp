#include "stdafx.h"
#include "CAD_DxfGlobal.h"
#include "DXF_Table.h"
#include "DXF_Block.h"


void ImportDxfSetBlockParametre(CCAD_Doc *pDoc, CCAD_Block *pBlock)
{
	long			pos;
	CCAD_Rect		cRange;
	CCAD_MgrEntity * pMgrEntity = &pBlock->m_cMgrEntity;
	CCAD_Entity	*	pEntity = pMgrEntity->GetFirstEntity(pos);

	pBlock->m_lSize = 0;
	pBlock->m_cRc.Init();
	while( pEntity != NULL ) {
		pEntity->CountBodyRange(cRange);
		SCAD_POINT	pt[8] = {
			{cRange.m_fMinX, cRange.m_fMinY, cRange.m_fMinZ}, 
			{cRange.m_fMaxX, cRange.m_fMinY, cRange.m_fMinZ},
			{cRange.m_fMaxX, cRange.m_fMaxY, cRange.m_fMinZ},
			{cRange.m_fMinX, cRange.m_fMaxY, cRange.m_fMinZ},
			{cRange.m_fMinX, cRange.m_fMinY, cRange.m_fMaxZ}, 
			{cRange.m_fMaxX, cRange.m_fMinY, cRange.m_fMaxZ},
			{cRange.m_fMaxX, cRange.m_fMaxY, cRange.m_fMaxZ},
			{cRange.m_fMinX, cRange.m_fMaxY, cRange.m_fMaxZ}};

		CCAD_Point	ptTemp;
		for (int i = 0; i < 8; i++)	{
			ptTemp = pt[i];
			pBlock->m_cRc.ResetParameter(ptTemp);
		}
		pBlock->m_lSize += pEntity->GetSize();
		pEntity = pMgrEntity->GetNextEntity(pos);
	}
}

BOOL ImportDxfBlockSection(CDXF_MemFile * pDxfMemFile, CCAD_Doc *pDoc)
{
	long lCount, i;

	lCount = pDxfMemFile->DXFGetBlockCnt();
	short	nBlock, nDimBlock;
	for (i=0, nBlock = 0, nDimBlock = 0; i<lCount; i++){
		CDXF_Block*	pDxfBlock;
		long		lSubCount;

		pDxfBlock = pDxfMemFile->DXFGetBlock(i);
		lSubCount = pDxfBlock->lGetEntityCount();
		if (lSubCount <= 0)
			continue;
		
		CDXF_Entity *pDxfEntity = NULL;

		CCAD_Block *pBlock = new CCAD_Block(pDoc->m_hHeap, (CCAD_Doc*)pDoc);
		BOOL		blDimBlock = FALSE;
		sprintf(pBlock->m_szName, "%s", pDxfBlock->m_pszBlockName);
		if (strncmp(pDxfBlock->m_pszBlockName, "*D", 2) == 0){
			pDoc->m_pMgrDimBlock->Add(pBlock);
			blDimBlock = TRUE;
			nDimBlock++;
		}
		else{
			pDoc->m_pMgrBlock->Add(pBlock);
			nBlock++;
		}

		for (long j=0; j<lSubCount; j++) {
			short	k;
			pDxfEntity = pDxfBlock->cpGetEntity(j);

			if (blDimBlock)
				k = nDimBlock - 1;
			else
				k = nBlock - 1;
			if (!ImportDxfAddEntity(pDoc, pDxfEntity, (short)k, blDimBlock))
				return FALSE;
		}
		ImportDxfSetBlockParametre(pDoc, pBlock);
	}
	return TRUE;
}
