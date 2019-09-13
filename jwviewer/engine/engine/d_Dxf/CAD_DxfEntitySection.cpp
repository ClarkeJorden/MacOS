#include "stdafx.h"
#include "CAD_DxfGlobal.h"
#include "CAD_Entity.h"
#include "CAD_LineEntity.h"
#include "CAD_CircleEntity.h"
#include "CAD_ArcEntity.h"
#include "CAD_EllipseEntity.h"
#include "CAD_EllipseArcEntity.h"
#include "CAD_PaintEntity.h"
#include "CAD_Block.h"
#include "CAD_Uds.h"
#include "CAD_PaperDoc.h"

CCAD_Entity * ImportDxfLine(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity);
CCAD_Entity * ImportDxfArc(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity);
CCAD_Entity * ImportDxfCircle(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity);
CCAD_Entity * ImportDxfEllipse(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity);
CCAD_Entity * ImportDxfEllipseArcFromEllipse(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity);
CCAD_Entity * ImportDxfPoint(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity);
CCAD_Entity * ImportDxfText(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity);
CCAD_Entity * ImportDxfImage(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity);
//CCAD_Entity * ImportDxfDimension(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity);

BOOL ImportDxfAddEntity(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity, short nBlockID, BOOL blDimBlock)
{
	CCAD_Entity	*pEntity = NULL;
	switch (pDxfEntity->m_nEntityType){
		case DXF_LINE:
			pEntity = ImportDxfLine(pDoc, pDxfEntity);
			break;
		case DXF_ARC:
			pEntity = ImportDxfArc(pDoc, pDxfEntity);
			break;
		case DXF_CIRCLE:
			pEntity = ImportDxfCircle(pDoc, pDxfEntity);
			break;
		case DXF_ELLIPSE:
			pEntity = ImportDxfEllipse(pDoc, pDxfEntity);
			break;
		case DXF_POINT:
			pEntity = ImportDxfPoint(pDoc, pDxfEntity);
			break;
		case DXF_TEXT:
			pEntity = ImportDxfText(pDoc, pDxfEntity);
			break;
		case DXF_IMAGE:
			pEntity = ImportDxfImage(pDoc, pDxfEntity);
			break;
// 		case DXF_DIMENSION:
// 			pEntity = ImportDxfDimension(pDoc, pDxfEntity);
// 			break;
		default:
			return FALSE;
	}
	if (pEntity != NULL){
		CCAD_MgrEntity * pMgrEntity = pDoc->GetSpaceDoc(pEntity->m_nSpaceDoc)->GetUds()->m_pMgrEntity;
		CCAD_Block * pBlock;
		if (!blDimBlock)
			pBlock = (CCAD_Block *)pDoc->m_pMgrBlock->GetStyle(nBlockID);
		else
			pBlock = (CCAD_Block *)pDoc->m_pMgrDimBlock->GetStyle(nBlockID);
		if (pBlock != NULL)
			pMgrEntity = &pBlock->m_cMgrEntity;

		if (-1 == pMgrEntity->AddEntity(pEntity)){
			delete pEntity;
			return FALSE;
		}
#if 1//NEW_ENTITY
		delete pEntity;
#endif		
	}
	return TRUE;
}

BOOL ImportDxfEntitySection(CDXF_MemFile * pDxfMemFile, CCAD_Doc *pDoc)
{
	long	lCount, i;

	CDXF_Entity*	pDxfEntity = NULL;
	lCount = pDxfMemFile->DXFGetEntityCnt();
	for (i=0; i<lCount; i++){
		pDxfEntity = pDxfMemFile->DXFGetEntity(i);
		if (pDxfEntity == NULL)
			continue;
//		if (i == 1343)
//			MessageBeep(1);
		if (!ImportDxfAddEntity(pDoc, pDxfEntity))
			return FALSE;
	}
	return TRUE;
}
