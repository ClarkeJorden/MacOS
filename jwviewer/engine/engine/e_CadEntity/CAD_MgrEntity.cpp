#include "stdafx.h"
#include "Common.h"
#include "CAD_Coord3D.h"
#include "CAD_MgrEntity.h"
#include "CAD_UdsMgr.h"
#include "CAD_LineEntity.h"
#include "CAD_ArcEntity.h"
#include "CAD_CircleEntity.h"
#include "CAD_EllipseEntity.h"
#include "CAD_EllipseArcEntity.h"
#include "CAD_Pline2DEntity.h"
#include "CAD_PointEntity.h"
#include "CAD_BeltEntity.h"
#include "CAD_CurveEntity.h"
#include "CAD_PaintEntity.h"
#include "CAD_BlockEntity.h"
#include "CAD_TextEntity.h"
#include "CAD_ImageEntity.h"
#include "CAD_Doc.h"

#include "CAD_LinearDimEntity.h"
#include "CAD_AngularDimEntity.h"
#include "CAD_RadialDimEntity.h"
#include "CAD_LeaderDimEntity.h"

#include <sys/timeb.h>
#include "CAD_PaperDoc.h"
#include "CAD_ViewMain.h"

extern int	g_nRenderProNum;

CCAD_MgrEntity::CCAD_MgrEntity(HANDLE hHeap, CCAD_Doc * pDoc, short nSpaceDoc, short nUds, BOOL isDoc)
{
	if (isDoc)
		m_lHeapSize = DOC_HEAP_SIZE;
	else 
		m_lHeapSize = BLOCK_HEAP_SIZE;
	m_cArray.RemoveAll();
	m_hHeap = hHeap;
	m_pHead = NULL;
	m_pDoc = pDoc;
	m_nSpaceDoc = nSpaceDoc;
	m_nUds = nUds;
	m_pDrawState = NULL;
}

CCAD_MgrEntity::~CCAD_MgrEntity()
{
	RemoveAll();
}

HEAPHEAD *	CCAD_MgrEntity::AllocHeap(long lSize)
{
	if (lSize > m_lHeapSize)
		lSize += sizeof(HEAPHEAD);
	else 
		lSize = m_lHeapSize;

	HEAPHEAD * pHeap = (HEAPHEAD *)HeapAlloc(m_hHeap, HEAP_ZERO_MEMORY, lSize);
	if (pHeap != NULL) {
		pHeap->lSize = lSize;
		pHeap->lSaved = sizeof(HEAPHEAD);
		pHeap->pNext = NULL;
	}
	return pHeap;
}

CCAD_Entity * CCAD_MgrEntity::AddToHeap(long lSize)
{
	if (m_hHeap == NULL)
		return NULL;

	try {
		HEAPHEAD * pHeap;
		if (m_pHead == NULL) {
			m_pHead = AllocHeap(lSize);
			pHeap = m_pHead;
		}
		else {
			pHeap = m_pHead;
			while ((pHeap->lSaved+lSize) > pHeap->lSize) {
				if (pHeap->pNext == NULL) {
					HEAPHEAD *pNewHeap = AllocHeap(lSize);
					pHeap->pNext = pNewHeap;
				}
				pHeap = pHeap->pNext;
			}
		}

		pHeap->lSaved += lSize;
		return (CCAD_Entity *)((LPSTR)pHeap + pHeap->lSaved - lSize);
	}
	catch(...) {
	}

	return NULL;
}

CCAD_Entity * CCAD_MgrEntity::GetFirstEntity(long & pos) const
{
	pos = 0;
	if ( pos >= m_cArray.GetSize() )
		return NULL;
	return (CCAD_Entity *)m_cArray[pos];
}

CCAD_Entity * CCAD_MgrEntity::GetEndEntity(long & pos) const
{
	pos = m_cArray.GetSize() -1;
	if ( pos < 0 )
		return NULL;
	return (CCAD_Entity *)m_cArray[pos];
}

CCAD_Entity * CCAD_MgrEntity::GetNextEntity(long & pos) const
{
	pos++;
	if ( pos >= m_cArray.GetSize() )
		return NULL;
	return (CCAD_Entity *)m_cArray[pos];
}

CCAD_Entity	* CCAD_MgrEntity::GetPrevEntity(long & pos) const
{
	pos--;
	if ( pos < 0 )
		return NULL;
	return (CCAD_Entity *)m_cArray[pos];
}

long	CCAD_MgrEntity::GetPosition(CCAD_Entity * pEntity) const
{
	int	i;
	for ( i=0; i<m_cArray.GetSize(); i++ ) {
		if ( pEntity == (CCAD_Entity *)m_cArray[i] )
			return i;
	}
	return -1;
}

CCAD_Entity * CCAD_MgrEntity::GetEntity(long pos) const
{
	if ( pos < 0 || pos >= m_cArray.GetSize() )
		return NULL;
	return (CCAD_Entity *)m_cArray[pos];
}

CCAD_Entity * CCAD_MgrEntity::NewEntity(BYTE bCode, BOOL isBlock) const
{
	CCAD_Entity * pEntity = NULL;
	CCAD_Doc * pDoc = (isBlock) ? NULL : m_pDoc;

	try {
		switch(bCode) {
 		case C_POINT:		pEntity = new CCAD_PointEntity(pDoc, m_nSpaceDoc, m_nUds);
 							break;
		case C_LINE:		pEntity = new CCAD_LineEntity(pDoc, m_nSpaceDoc, m_nUds);
							break;
 		case C_ARC:			pEntity = new CCAD_ArcEntity(pDoc, m_nSpaceDoc, m_nUds);
 							break;
 		case C_CIRCLE:		pEntity = new CCAD_CircleEntity(pDoc, m_nSpaceDoc, m_nUds);
 							break;
 		case C_ELLPARC:		pEntity = new CCAD_EllipseArcEntity(pDoc, m_nSpaceDoc, m_nUds);
 							break;
 		case C_ELLIPSE:		pEntity = new CCAD_EllipseEntity(pDoc, m_nSpaceDoc, m_nUds);
 							break;
 		case C_PLINE:		pEntity = new CCAD_Pline2DEntity(pDoc, m_nSpaceDoc, m_nUds);
 							break;
 		case C_BELT:		pEntity = new CCAD_BeltEntity(pDoc, m_nSpaceDoc, m_nUds);
 							break;
 		case C_CURVE:		pEntity = new CCAD_CurveEntity(pDoc, m_nSpaceDoc, m_nUds);
 							break;
 		case C_PAINT:		pEntity = new CCAD_PaintEntity(pDoc, m_nSpaceDoc, m_nUds);
 							break;
 		case C_BLOCK:		pEntity = new CCAD_BlockEntity(pDoc, m_nSpaceDoc, m_nUds);
 							break;
 		case C_TEXT:		pEntity = new CCAD_TextEntity(pDoc, m_nSpaceDoc, m_nUds);
 							break;
 		case C_IMAGE:		pEntity = new CCAD_ImageEntity(pDoc, m_nSpaceDoc, m_nUds);
 							break;
		case C_DIMLINEAR:	pEntity = (CCAD_Entity*) new CCAD_LinearDimEntity(pDoc, m_nSpaceDoc, m_nUds);
							break;
		case C_DIMANGULAR:	pEntity = (CCAD_Entity*) new CCAD_AngularDimEntity(pDoc, m_nSpaceDoc, m_nUds);
							break;
		case C_DIMRADIAL:	pEntity = (CCAD_Entity*) new CCAD_RadialDimEntity(pDoc, m_nSpaceDoc, m_nUds);
							break;
		case C_DIMLEADER:	pEntity = (CCAD_Entity*) new CCAD_LeaderDimEntity(pDoc, m_nSpaceDoc, m_nUds);
							break;
		default :
							break;
		}
	}
	catch(...) {
		return NULL;
	}

	return pEntity;
}

void CCAD_MgrEntity::RemoveAll()
{
	for(int i = 0; i < m_cArray.GetSize(); i++)
	{
		CCAD_Entity *pEntity = (CCAD_Entity*)m_cArray.GetAt(i);
		if(pEntity)
			delete pEntity;
	}

	m_cArray.RemoveAll();

	if(m_pDrawState)
		delete[] m_pDrawState;
	m_pDrawState = NULL;
	m_pHead = NULL;
}

void CCAD_MgrEntity::CopyFromOther(const CCAD_MgrEntity & other)
{
	RemoveAll();
	long pos;

	CCAD_Entity	* pEntity = other.GetFirstEntity(pos);
	while (pEntity != NULL) {
		if (pEntity->IsVisible()) {
			AddEntity(pEntity);
		}
		pEntity = other.GetNextEntity(pos);
	}
}

long CCAD_MgrEntity::AddEntity(CCAD_Entity * pEntity)
{
#if 1//NEW_ENTITY
 	CCAD_Entity * pNewEntity = NewEntity(pEntity->m_bCode);
 	if ( pNewEntity == NULL )
 		return -1;
 	pEntity->MemCopyTo( pNewEntity, NULL, NULL, NULL );
#else	
	CCAD_Entity * pNewEntity = pEntity;
#endif	
	m_cArray.Add(pNewEntity);
	pNewEntity->m_nIndex = m_cArray.GetSize() - 1;
	return (m_cArray.GetSize()-1);
}

long CCAD_MgrEntity::CountEntity(BYTE bFlag)
{
	long size = m_cArray.GetSize();
	long count = 0;
	int	i;

	for (i=0; i<size; i++) {
		CCAD_Entity * pEntity = (CCAD_Entity *)m_cArray[i];
		if ( pEntity->m_bFlag >= bFlag )
			count++;
	}
	return count;
}

BOOL CCAD_MgrEntity::Read(CCAD_File & cFile, BOOL isBlock)
{
	for (; ;) {
		BYTE	bCode;
		cFile.ReadByte(&bCode);

	//	if ( bCode == F_END )
		{
			m_pDrawState = new BYTE[m_cArray.GetSize()];
			memset(m_pDrawState, 0, sizeof(BYTE) * m_cArray.GetSize());
			return TRUE;
		}

		CCAD_Entity * pEntity = NewEntity(bCode, isBlock);
		if ( pEntity == NULL )
			return FALSE;
        
		if ( !pEntity->Read(cFile) )
			return FALSE;
		if (isBlock) {
			pEntity->m_pDoc = m_pDoc;
			pEntity->m_nLayer = DISABLE_INDEX;
		}

 		switch(pEntity->m_bCode) {
		case C_BLOCK:
			{
				((CCAD_BlockEntity*)pEntity)->m_pDoc = m_pDoc;
				((CCAD_BlockEntity*)pEntity)->m_cGemBlock.m_pDoc = m_pDoc;
			}
			break;
		case C_TEXT:
			{	
				CCAD_TextStyleMgr * pStyleMgr = (CCAD_TextStyleMgr *)m_pDoc->GetStyleMgr(SS_TEXT);
				((CCAD_TextEntity*)pEntity)->m_cGemText.m_pTextStyle = (CCAD_TextStyle *)pStyleMgr->GetStyle(((CCAD_TextEntity*)pEntity)->m_cGemText.m_nStyle);
			}
			break;
		case C_IMAGE:
			{
				CCAD_ImageMgr * pImageMgr = (CCAD_ImageMgr *)m_pDoc->GetStyleMgr(SS_IMAGE);
				CCAD_GemImage * pGemImage = &((CCAD_ImageEntity*)pEntity)->m_cGemImage;
				pGemImage->m_pImage = (CCAD_Image *)pImageMgr->GetStyle(((CCAD_ImageEntity*)pEntity)->m_cGemImage.m_nIndex);
			}
			break;
		case C_DIMLINEAR:
		case C_DIMANGULAR:
		case C_DIMRADIAL:
		case C_DIMLEADER:
			{
				((CCAD_DimensionEntity*)pEntity)->m_pDoc = m_pDoc;
				((CCAD_GemDimension*)pEntity->GetGemEntity())->m_pDoc = m_pDoc;
				CCAD_DimStyleMgr * pStyleMgr = (CCAD_DimStyleMgr*)m_pDoc->GetStyleMgr(SS_DIMENSION);
				CCAD_GemDimension * pGem = (CCAD_GemDimension*)pEntity->GetGemEntity();

				pGem->m_pStyle = (CCAD_DimStyle *)pStyleMgr->GetStyle(pGem->m_nStyle);
				pGem->m_pTxtStyle = (CCAD_TextStyle *)m_pDoc->m_pTextStyleMgr->GetStyle(pGem->m_pStyle->m_sDimText.nStyle);
			}
			break;
 		}
 
		if ( !pEntity->IsIgno(isBlock) ) {
			if (AddEntity(pEntity) == -1) {
				delete pEntity;
				return FALSE;
			}
		}
#if 1//NEW_ENTITY
		delete pEntity;
#endif		
	}
	return FALSE;
}


BOOL CCAD_MgrEntity::DrawEntity(CCAD_DC * pDC, BOOL blRedraw, BOOL blViewport)
{
	ASSERT(pDC != NULL);
	long	pos;
	CCAD_Entity	* pEntity = GetFirstEntity(pos);

	BYTE bDraw;
	while (pEntity != NULL)
	{
		if (pEntity->IsVisible())
		{
			bDraw = 1;
			if(!blViewport)
			{
				bDraw = m_pDrawState[pEntity->m_nIndex];
			}

			if(bDraw == 1)
				pEntity->Draw(pDC, blRedraw);
			
			if (pDC->CheckMessage())
				return FALSE;
		}
		pEntity = GetNextEntity(pos);
	}
	return true;
}

void CCAD_MgrEntity::CountWorldRange(CCAD_DC * pDC, CCAD_Rect & cOutRange, BYTE bFlag) const
{
	long pos;
	CCAD_Entity	* pEntity = GetFirstEntity(pos);

	while (pEntity != NULL) {
		CCAD_Rect	cRc;
		if (pEntity->IsVisible(bFlag)) {
			cRc.Init();
			pEntity->CountBodyRange(cRc);
			if (!cRc.IsInited()) {
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
					pEntity->m_cWorldBound.ResetParameter(ptTemp);
				}
				cOutRange.ResetParameter(pEntity->m_cWorldBound);
				pEntity->SetMaxBoundSize();
			}
		}
		pEntity = GetNextEntity(pos);
	}
}

void CCAD_MgrEntity::CountVcsRange(CCAD_DC * pDC, CCAD_Rect & cOutRange) const
{
	long pos;
	CCAD_Entity	* pEntity = GetFirstEntity(pos);

	while (pEntity != NULL) {
		if (pEntity->IsVisible(E_DRAWING)) {
			pEntity->CountVcsRange(pDC, cOutRange);
		}
		pEntity = GetNextEntity(pos);
	}
}

long CCAD_MgrEntity::CountBlockSize(BYTE bFlag) const
{
	long pos, lCount = 0;
	CCAD_Entity	* pEntity = GetFirstEntity(pos);

	while (pEntity != NULL) {
		if (pEntity->IsVisible(bFlag))
			lCount += pEntity->GetSize();
		pEntity = GetNextEntity(pos);
	}
	return lCount;
}

BOOL CCAD_MgrEntity::IsEntities()
{
	long	pos;
	CCAD_Entity	* pEntity = GetFirstEntity(pos);

	while (pEntity != NULL) {
		if (!pEntity->IsDeleted())
			return true;
		pEntity = GetNextEntity(pos);
	}
	return false;
}

// added
void CCAD_MgrEntity::QuickSort_BoundSize(CCAD_PtrArray & entityAry, int nStart, int nEnd)
{
	if(nStart < 0)
		return;
	if(nEnd <= nStart)
		return;
	if(entityAry.GetCount() < nEnd)
		return;

	CCAD_Entity * pEntity_Base = (CCAD_Entity *)entityAry[nStart];
	CCAD_Entity * pEntity_Left = NULL;
	CCAD_Entity * pEntity_Right = NULL;
	int iLeft = nStart;
	int iRight = nEnd;

	while(iRight > iLeft)
	{
		do
		{
			iLeft++;
			if(iLeft == entityAry.GetCount())	break;
			pEntity_Left = (CCAD_Entity *)entityAry[iLeft];
		} while( pEntity_Base->m_fMaxBoundSize < pEntity_Left->m_fMaxBoundSize);

		do
		{
			iRight--;
			if(iRight < 0)	break;
			pEntity_Right = (CCAD_Entity *)entityAry[iRight];
		} while( pEntity_Right->m_fMaxBoundSize < pEntity_Base->m_fMaxBoundSize);

		if(iRight > iLeft)
		{
			entityAry.SetAt(iLeft, pEntity_Right);
			entityAry.SetAt(iRight, pEntity_Left);
		}
		else
		{
			entityAry.SetAt(nStart, pEntity_Right);
			entityAry.SetAt(iRight, pEntity_Base);

			QuickSort_BoundSize(entityAry, nStart, iRight);

			QuickSort_BoundSize(entityAry, iRight+1, nEnd);
		}
	}
}

