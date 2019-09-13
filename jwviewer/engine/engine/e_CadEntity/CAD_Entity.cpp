#include "stdafx.h"
#include "CAD_Doc.h"
#include "CAD_UdsMgr.h"
#include "CAD_Entity.h"
#include "CAD_BlockEntity.h"

CCAD_Entity::CCAD_Entity(CCAD_Doc * pDoc, short nSpaceDoc, short nUds)
{
	m_bCode = C_NOENTITY;
	m_bFlag = E_DELETE;

	m_nLayer = DISABLE_INDEX;
	m_pDoc = pDoc;
	m_nSpaceDoc = nSpaceDoc;
	m_nUds = nUds;
	m_cWorldBound.Init();
	m_fMaxBoundSize = 0;
	InitEntity();
}

CCAD_Entity::~CCAD_Entity()
{
	Remove();
}

CCAD_Entity & CCAD_Entity::operator=(const CCAD_Entity & cEntity)
{
	m_bCode = cEntity.m_bCode;
	m_bFlag = cEntity.m_bFlag;
	m_nLayer = cEntity.m_nLayer;
	m_pDoc = cEntity.m_pDoc;
	m_nSpaceDoc = cEntity.m_nSpaceDoc;
	m_nUds = cEntity.m_nUds;
	return *this;
}

void CCAD_Entity::InitEntity(CCAD_Entity * pEntity)
{
	if ( pEntity != NULL ) {
		m_nLayer = pEntity->m_nLayer;
	}
	else if (m_pDoc != NULL) {
		m_nLayer = m_pDoc->m_pLayerMgr->m_nAct;
	}
}

void CCAD_Entity::SetBlockEntityPara(const CCAD_MgrEntity * pMgrEntity)
{
	m_pDoc = pMgrEntity->m_pDoc;
	m_nSpaceDoc = pMgrEntity->m_nSpaceDoc;
	m_nUds = pMgrEntity->m_nUds;
	m_nLayer = DISABLE_INDEX;
}

void CCAD_Entity::Remove()
{
	CCAD_GemEntity	*pGemEntity = GetGemEntity();
	if (pGemEntity == NULL)
		return;
	pGemEntity->Remove();
}

BOOL CCAD_Entity::Read(CCAD_File & cFile)
{
//	cFile.ReadByte(&m_bCode);
	cFile.ReadByte(&m_bFlag);
	m_bFlag = E_DRAWING;

	cFile.ReadShort(&m_nLayer);
	return TRUE;
}

BOOL CCAD_Entity::IsClosed()
{
	CCAD_GemEntity *pGemEntity = GetGemEntity();
	if (pGemEntity == NULL)
		return FALSE;
	return pGemEntity->IsClosed();
}

BOOL CCAD_Entity::IsThick()
{
	CCAD_GemEntity *pGemEntity = GetGemEntity();
	if (pGemEntity == NULL)
		return FALSE;
	return pGemEntity->IsThick();
}

void CCAD_Entity::CopyEntityTo(CCAD_Entity *pEntity)
{
	pEntity->m_nLayer = m_nLayer;
	pEntity->m_pDoc = m_pDoc;
	pEntity->m_nUds = m_nUds;
	pEntity->m_nSpaceDoc = m_nSpaceDoc;
	if (m_bCode == C_BLOCK || m_bCode == C_DIMLINEAR || m_bCode == C_DIMANGULAR || 
		m_bCode == C_DIMRADIAL || m_bCode == C_DIMLEADER )
		return;
	CCAD_GemEntity *pGemThis = GetGemEntity();
	CCAD_GemEntity *pGemEntity = pEntity->GetGemEntity();
	pGemEntity->m_cBcs = pGemThis->m_cBcs;
}

void CCAD_Entity::MemCopyTo(CCAD_Entity * pEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	ASSERT(pEntity != NULL);
	::memmove(pEntity, this, ClassSize());
	CCAD_GemEntity *pGemEntity = GetGemEntity();
	if (pGemEntity == NULL)
		return;
	pGemEntity->MemCopyTo(pEntity->GetGemEntity(), pData1, pData2, pData3);
}

BOOL CCAD_Entity::Draw(CCAD_DC* pCadDC, BOOL blRedraw)
{
	CCAD_GemEntity* pGemEntity = GetGemEntity();
	if (pGemEntity == NULL)
		return FALSE;

	BOOL blResult;
	//	set pen.
	LOGPEN	sLogPen;
	LOGPEN	oldPen;
	CCAD_PenStyle *hOldPenStyle = NULL;
	WORD	wBakPriority = pCadDC->m_wPriority;

	bool blGet;
	if ( blGet = GetLogPen(pCadDC, sLogPen) ) {
		oldPen = pCadDC->SelectPen(sLogPen);
		hOldPenStyle = SelectPenStyle(pCadDC);
	}

	//	set brush.
	LOGBRUSH	sBrush;
	LOGBRUSH	oldBrush;
	int			iOldFillMode, bkMode;
	bool blGetBrush;
	if ( blGetBrush = GetLogBrush(pCadDC, sBrush) )
		oldBrush = pCadDC->SelectBrush(sBrush);

	if (!IsBlockEntity())
		blResult = pGemEntity->Drawing(pCadDC, E_DRAWING, blRedraw);

//  	if ( blGetBrush )
//  		pCadDC->RestoreObject(oldBrush);
	
	if ( blGet ) {
		pCadDC->SelectPenStyle(hOldPenStyle, DISABLE_INDEX, 0);
/* 		pCadDC->RestoreObject(oldPen);*/
	}

	if (IsBlockEntity()) {
		blResult = pGemEntity->Drawing(pCadDC, E_DRAWING, blRedraw);
		pCadDC->m_wPriority = wBakPriority;
	}

	if ( !blResult )
		SetState(E_DELETE);
	return blResult;
}

BOOL CCAD_Entity::IsVisible(BYTE bFlag)
{
	if (m_bCode==C_NOENTITY || m_bFlag<bFlag)
		return FALSE;
	if (m_pDoc == NULL || m_nLayer==DISABLE_INDEX)
		return TRUE;
	return m_pDoc->m_pLayerMgr->IsVisible(m_nLayer);
}

BOOL CCAD_Entity::GetLogPen(CCAD_DC * pCadDC, LOGPEN & sLogPen, BYTE bState)
{
	CCAD_Layer * pLayer = NULL;
	if ( m_nLayer != DISABLE_INDEX )
		pLayer = (CCAD_Layer *)m_pDoc->m_pLayerMgr->GetStyle(m_nLayer);

	sLogPen.lopnStyle = PS_SOLID;

	CAD_FLOAT	fWidth;

	CCAD_Pen *pCadPen = GetCadPen();

	if (pCadPen == NULL) {
		if ((m_bCode >= C_DIMLINEAR && m_bCode <=C_DIMLEADER) ) {
			if (pLayer != NULL) {
				if (bState == E_DELETE)
					sLogPen.lopnColor = RGB(255, 255, 255);
				else if (pLayer->m_wState & PRI_PEN_COLOR) {
					pCadDC->ReSetPriority(PRI_PEN_COLOR, true);
					sLogPen.lopnColor = pLayer->m_cPen.m_rgbColor;
				}

				if (pLayer->m_wState & PRI_PEN_THICK) {
					pCadDC->ReSetPriority(PRI_PEN_THICK, true);
					fWidth = pLayer->m_cPen.m_nThick * pCadDC->m_fScale * 0.1F * pCadDC->m_fPaperScale;
				//	pCadDC->m_fWidth = pLayer->m_cPen.m_nThick * 0.1F;
					sLogPen.lopnWidth.x = sLogPen.lopnWidth.y = (int)(fWidth + 0.5);
				}
				else {
					pCadDC->ReSetPriority(PRI_PEN_THICK, false);
					sLogPen.lopnWidth.x = sLogPen.lopnWidth.y = pCadDC->m_sPen.lopnWidth.x;
				}
				return true;
			}
		}
		return false;
	}

	if(pLayer != NULL) {
		if (bState == E_DELETE)
			sLogPen.lopnColor = RGB(255, 255, 255);
		else {
			if (pLayer->m_wState & PRI_PEN_COLOR) {
				pCadDC->ReSetPriority(PRI_PEN_COLOR, true);
				sLogPen.lopnColor = pLayer->m_cPen.m_rgbColor;
			}
			else {
				pCadDC->ReSetPriority(PRI_PEN_COLOR, false);
				sLogPen.lopnColor = pCadPen->m_rgbColor;
			}
		}

		if (pLayer->m_wState & PRI_PEN_THICK) {
			pCadDC->ReSetPriority(PRI_PEN_THICK, true);
			fWidth = pLayer->m_cPen.m_nThick;
		}
		else {
			pCadDC->ReSetPriority(PRI_PEN_THICK, false);
			fWidth = pCadPen->m_nThick;
		}
		fWidth = fWidth * pCadDC->m_fScale * 0.1F * pCadDC->m_fPaperScale;
		sLogPen.lopnWidth.x = sLogPen.lopnWidth.y = (int)fWidth;
	}
	else {
		if ( bState == E_DELETE)
			sLogPen.lopnColor = RGB(255, 255, 255);
		else {
			if (pCadDC->m_wPriority & PRI_PEN_COLOR)
				sLogPen.lopnColor = pCadDC->m_sPen.lopnColor;
			else
				sLogPen.lopnColor = pCadPen->m_rgbColor;
		}

		if ((pCadDC->m_wPriority & PRI_PEN_THICK) == 0) {
			fWidth = pCadPen->m_nThick * pCadDC->m_fScale * 0.1F * pCadDC->m_fPaperScale;
			sLogPen.lopnWidth.x = sLogPen.lopnWidth.y = (int)fWidth;
		}
		else 
			sLogPen.lopnWidth.x = sLogPen.lopnWidth.y = pCadDC->m_sPen.lopnWidth.x;
	}

	if (m_bCode == C_BLOCK) {
		if ( ((CCAD_BlockEntity*)this)->m_wPriority & PRI_PEN_COLOR )
			pCadDC->AddPriority(PRI_PEN_COLOR);
		if ( ((CCAD_BlockEntity*)this)->m_wPriority & PRI_PEN_THICK )
			pCadDC->AddPriority(PRI_PEN_THICK);
	}
	return true;
}

CCAD_PenStyle *CCAD_Entity::SelectPenStyle(CCAD_DC *pCadDC)
{
	ASSERT(m_pDoc != NULL);

	CCAD_Pen *pCadPen = GetCadPen();
	if (pCadPen == NULL)
		return NULL;

	CCAD_StyleMgr *pStyleMgr = m_pDoc->GetStyleMgr(SS_LINE);

	short		nStyle;
	CAD_FLOAT	fScale;

	CCAD_Layer * pLayer = NULL;
	if ( m_nLayer != DISABLE_INDEX )
		pLayer = (CCAD_Layer *)m_pDoc->m_pLayerMgr->GetStyle(m_nLayer);

	if(pLayer != NULL) {
		if (pLayer->m_wState & PRI_PEN_STYLE) {
			pCadDC->ReSetPriority(PRI_PEN_STYLE, true);
			nStyle = pLayer->m_cPen.m_nStyle;
		}
		else {
			pCadDC->ReSetPriority(PRI_PEN_STYLE, false);
			nStyle = pCadPen->m_nStyle;
		}

		if (pLayer->m_wState & PRI_PEN_SCALE) {
			pCadDC->ReSetPriority(PRI_PEN_SCALE, true);
			fScale = pLayer->m_cPen.m_fScale;
		}
		else {
			pCadDC->ReSetPriority(PRI_PEN_SCALE, false);
			fScale = pCadPen->m_fScale;
		}
	}
	else {
		if (pCadDC->m_wPriority & PRI_PEN_STYLE)
			nStyle = pCadDC->m_nPenStyle;
		else
			nStyle = pCadPen->m_nStyle;

		if (pCadDC->m_wPriority & PRI_PEN_SCALE)
			fScale = pCadDC->m_fPenScale;
		else
			fScale = pCadPen->m_fScale;
	}

	if (m_bCode == C_BLOCK) {
		if ( ((CCAD_BlockEntity*)this)->m_wPriority & PRI_PEN_SCALE )
			pCadDC->AddPriority(PRI_PEN_SCALE);
		if ( ((CCAD_BlockEntity*)this)->m_wPriority & PRI_PEN_STYLE )
			pCadDC->AddPriority(PRI_PEN_STYLE);
	}

	if (pStyleMgr != NULL) {
		CCAD_PenStyle *pPenStyle = (CCAD_PenStyle *) pStyleMgr->GetStyle(nStyle);
		if (pPenStyle != NULL) {
			if (!pPenStyle->IsDeleted()) {
				if (pPenStyle->m_nTotPart > 1 && pPenStyle->m_fLength > 0)
					return pCadDC->SelectPenStyle(pPenStyle, nStyle, fScale);
			}
		}
	}
	return pCadDC->SelectPenStyle(NULL, nStyle, fScale);
}

BOOL CCAD_Entity::GetLogBrush(CCAD_DC *pCadDC, LOGBRUSH & sBrush, BYTE bState)
{
	CCAD_Layer * pLayer = NULL;
	if ( m_nLayer != DISABLE_INDEX )
		pLayer = (CCAD_Layer *)m_pDoc->m_pLayerMgr->GetStyle(m_nLayer);

	sBrush.lbStyle = BS_HATCHED;
	sBrush.lbHatch = 0;
	short nPattern;

	CCAD_Brush *pCadBrush = GetCadBrush();

	if (pCadBrush == NULL) {
		if ((m_bCode >= C_DIMLINEAR && m_bCode <=C_DIMLEADER) ) {
			if (pLayer != NULL) {
				if (bState == E_DELETE)
					sBrush.lbColor = RGB(255, 255, 255);
				else if (pLayer->m_wState & PRI_BRUSH_COLOR) {
					pCadDC->ReSetPriority(PRI_BRUSH_COLOR, true);
					sBrush.lbColor = pLayer->m_cBrush.m_rgbColor;
				}
				else {
					pCadDC->ReSetPriority(PRI_BRUSH_COLOR, false);
					sBrush.lbColor = pCadDC->m_sBrush.lbColor;
				}

				if (pLayer->m_wState & PRI_BRUSH_PATTERN) {
					pCadDC->ReSetPriority(PRI_BRUSH_PATTERN, true);
					nPattern = pLayer->m_cBrush.m_nPattern;
				}
				else {
					pCadDC->ReSetPriority(PRI_BRUSH_PATTERN, false);
					nPattern = pCadDC->m_sBrush.lbStyle;
				}
				switch( nPattern ) {
					case BRS_HORIZONTAL:	sBrush.lbHatch = HS_HORIZONTAL; break;
					case BRS_VERTICAL:		sBrush.lbHatch = HS_VERTICAL;	break;
					case BRS_CROSS:			sBrush.lbHatch = HS_CROSS;		break;
					case BRS_BDIAGONAL:		sBrush.lbHatch = HS_BDIAGONAL;	break;
					case BRS_FDIAGONAL:		sBrush.lbHatch = HS_FDIAGONAL;	break;
					case BRS_DIAGCROSS:		sBrush.lbHatch = HS_DIAGCROSS;	break;
					default:				sBrush.lbHatch = BS_SOLID;		break;
				}
				return true;
			}
		}
		return false;
	}

	if(pLayer != NULL) {
		if ( bState == E_DELETE)
			sBrush.lbColor = RGB(255, 255, 255);
		else {
			if (pLayer->m_wState & PRI_BRUSH_COLOR) {
				pCadDC->ReSetPriority(PRI_BRUSH_COLOR, true);
				sBrush.lbColor = pLayer->m_cBrush.m_rgbColor;
			}
			else {
				pCadDC->ReSetPriority(PRI_BRUSH_COLOR, false);
				sBrush.lbColor = pCadBrush->m_rgbColor;
			}
		}
		if (pLayer->m_wState & PRI_BRUSH_PATTERN) {
			pCadDC->ReSetPriority(PRI_BRUSH_PATTERN, true);
			nPattern = pLayer->m_cBrush.m_nPattern;
		}
		else {
			pCadDC->ReSetPriority(PRI_BRUSH_PATTERN, false);
			nPattern = pCadBrush->m_nPattern;
		}
	}
	else {
		sBrush = pCadDC->m_sBrush;
		if ( bState == E_DELETE)
			sBrush.lbColor = RGB(255, 255, 255);
		else {
			if (pCadDC->m_wPriority & PRI_BRUSH_COLOR)
				sBrush.lbColor = pCadDC->m_sBrush.lbColor;
			else
				sBrush.lbColor = pCadBrush->m_rgbColor;
		}
		if (pCadDC->m_wPriority & PRI_BRUSH_PATTERN) {
			switch (pCadDC->m_sBrush.lbHatch) {
				case HS_HORIZONTAL:		nPattern = BRS_HORIZONTAL;	break;
				case HS_VERTICAL:		nPattern = BRS_VERTICAL;	break;
				case HS_CROSS:			nPattern = BRS_CROSS;		break;
				case HS_FDIAGONAL:		nPattern = BRS_FDIAGONAL;	break;
				case HS_BDIAGONAL:		nPattern = BRS_BDIAGONAL;	break;
				case HS_DIAGCROSS:		nPattern = BRS_DIAGCROSS;	break;
				default:				nPattern = BRS_SOLID;		break;
			}
		}
		else
			nPattern = pCadBrush->m_nPattern;
	}

	if (m_bCode == C_BLOCK) {
		if ( ((CCAD_BlockEntity*)this)->m_wPriority & PRI_BRUSH_PATTERN )
			pCadDC->AddPriority(PRI_BRUSH_PATTERN);
		if ( ((CCAD_BlockEntity*)this)->m_wPriority & PRI_BRUSH_COLOR )
			pCadDC->AddPriority(PRI_BRUSH_COLOR);
	}

	switch (nPattern) {
		case BRS_HORIZONTAL:	sBrush.lbHatch = HS_HORIZONTAL;	break;
		case BRS_VERTICAL:		sBrush.lbHatch = HS_VERTICAL;	break;
		case BRS_CROSS:			sBrush.lbHatch = HS_CROSS;		break;
		case BRS_BDIAGONAL:		sBrush.lbHatch = HS_BDIAGONAL;	break;
		case BRS_FDIAGONAL:		sBrush.lbHatch = HS_FDIAGONAL;	break;
		case BRS_DIAGCROSS:		sBrush.lbHatch = HS_DIAGCROSS;	break;
		default:				sBrush.lbStyle = BS_SOLID;		break;
	}
	return true;
}

CCAD_MgrEntity * CCAD_Entity::GetMgrEntity()
{
	if (m_pDoc == NULL)
		return NULL;
	CCAD_SpaceDoc *pSpaceDoc = m_pDoc->GetSpaceDoc(m_nSpaceDoc);
	if (pSpaceDoc == NULL)
		return NULL;
	return pSpaceDoc->GetUds()->m_pMgrEntity;
}

void CCAD_Entity::CountBodyRange(CCAD_Rect& cRange)
{
	CCAD_GemEntity* pGemEntity = GetGemEntity();
	if (pGemEntity == NULL)
		return;
	pGemEntity->CountBodyRange(cRange);
	
//	convert child to base coordinate.
//	CCAD_Coord3D *pBCS = pGemEntity->GetBCS();
//	ASSERT(pBCS != NULL);
//	pBCS->ConvToBase(cRange);
}

void CCAD_Entity::CountVcsRange(CCAD_DC * pCadDC, CCAD_Rect& cRange)
{
	CCAD_GemEntity* pGemEntity = GetGemEntity();
	if (pGemEntity == NULL)
		return;
	pGemEntity->CountVCSRange(pCadDC, cRange);
}

BOOL CCAD_Entity::GetUdsRange(CCAD_Coord3D* pUCS, CCAD_Rect& cRange)
{
	if (pUCS == NULL)
		return FALSE;
	CCAD_GemEntity* pGemEntity = GetGemEntity();
	if (pGemEntity == NULL)
		return FALSE;
	
	CCAD_Rect	rng;
	pGemEntity->CountBodyRange(rng);
	if ( rng.IsInited() )
		return false;

	CCAD_Coord3D *pBCS = pGemEntity->GetBCS();
	ASSERT(pBCS != NULL);
	
	CCAD_Point	pt1, pt2;
	pt1.m_fX = rng.m_fMinX; pt2.m_fX = rng.m_fMaxX;
	pt1.m_fY = rng.m_fMinY; pt2.m_fY = rng.m_fMaxY;
	pt1.m_fZ = rng.m_fMinZ; pt2.m_fZ = rng.m_fMaxZ;
	
	pBCS->ConvToBase(pt1);	pBCS->ConvToBase(pt2);
	pUCS->ConvFromBase(pt1);	pUCS->ConvFromBase(pt2);
	cRange.SetParameter(pt1, pt2);
	return TRUE;
}

BOOL CCAD_Entity::IsBlockEntity()
{
	return ( m_bCode == C_BLOCK || IsDimensionEntity() );
}

void CCAD_Entity::ConvToBase(const CCAD_Coord3D &cBcs, EXP_MESH *spMesh)
{
	ASSERT(spMesh != NULL);
	CCAD_Vector vect(spMesh->sMesh.m_sNVectMesh);
	cBcs.ConvVectorToBase(vect);
	spMesh->sMesh.m_sNVectMesh = vect;
	
	short i, nCount = ((spMesh->typeMesh & MESH_TYPE_4) != 0) ? 4 : 3;
	for (i = 0; i < nCount; i++)
	{
		cBcs.ConvToBase(spMesh->sMesh.m_sVertex[i]);
		vect = spMesh->sMesh.m_sNVectVertex[i];
		cBcs.ConvVectorToBase(vect);
		spMesh->sMesh.m_sNVectVertex[i] = vect;
	}
}

BOOL CCAD_Entity::CalcTexCoord(SCAD_POINT*	spStartPoint,	SCAD_POINT*	spEndPoint,
														short	nNumVertex,	SCAD_POINT*	spVertex,
														CAD_FLOAT	fSizeTexWidth, CAD_FLOAT fSizeTexHeight,
														SCAD_POINT*	spTexCoord)
{
	ASSERT(spStartPoint && spEndPoint);
	ASSERT(spVertex != NULL);
	if (fSizeTexWidth <= 0.0F || fSizeTexHeight <= 0.0F)
		return FALSE;
	ASSERT(spTexCoord != NULL);
	
	SCAD_POINT		sN;
	if (!CalcNormal(spVertex, &sN))
		return FALSE;
	
	int nPlane = 0;
	double max = fabs(sN.m_fZ);
	
	if (fabs(sN.m_fX) > max)
	{
		max = fabs(sN.m_fX);
		nPlane = 1;
    }
    if (fabs(sN.m_fY) > max)
		nPlane = 2;
	
	CAD_FLOAT fWidth = (CAD_FLOAT)fabs(spEndPoint->m_fX - spStartPoint->m_fX);
	CAD_FLOAT fDepth = (CAD_FLOAT)fabs(spEndPoint->m_fY - spStartPoint->m_fY);
	CAD_FLOAT fHeight = (CAD_FLOAT)fabs(spEndPoint->m_fZ - spStartPoint->m_fZ);
	
	CAD_FLOAT	fNumTexWidth, fNumTexHeight;
	CAD_FLOAT	fWidth1, fHeight1;
	
	switch(nPlane) 
	{
	case 0:	//XY
		{
			if (fWidth == 0.0f || fDepth == 0.0f)
				break;
			
			fNumTexWidth = fWidth / fSizeTexWidth;
			fNumTexHeight = fDepth / fSizeTexHeight;
			
			for (short i = 0; i < nNumVertex; i ++)
			{
				fWidth1 = (CAD_FLOAT)fabs(spEndPoint->m_fX - spVertex[i].m_fX);
				fHeight1 = (CAD_FLOAT)fabs(spEndPoint->m_fY - spVertex[i].m_fY);
				spTexCoord[i].m_fX = fNumTexWidth * (1 - fWidth1 / fWidth);
				spTexCoord[i].m_fY = fNumTexHeight * (1 - fHeight1 / fDepth);
			}
		}
		break;
		
	case 1:	//YZ
		{
			if (fDepth == 0.0f || fHeight == 0.0f)
				break;
			
			fNumTexWidth = fDepth / fSizeTexWidth;
			fNumTexHeight = fHeight / fSizeTexHeight;
			
			for (short i = 0; i < nNumVertex; i ++)
			{
				fWidth1 = (CAD_FLOAT)fabs(spEndPoint->m_fY - spVertex[i].m_fY);
				fHeight1 = (CAD_FLOAT)fabs(spEndPoint->m_fZ - spVertex[i].m_fZ);
				spTexCoord[i].m_fX = fNumTexWidth * (1 - fWidth1 / fDepth);
				spTexCoord[i].m_fY = fNumTexHeight * (1 - fHeight1 / fHeight);
			}
		}
		break;
		
	case 2:	//XZ
		{
			if (fWidth == 0.0f || fHeight == 0.0f)
				break;

			fNumTexWidth = fWidth / fSizeTexWidth;
			fNumTexHeight = fHeight / fSizeTexHeight;
			
			for (short i = 0; i < nNumVertex; i ++)
			{
				fWidth1 = (CAD_FLOAT)fabs(spEndPoint->m_fX - spVertex[i].m_fX);
				fHeight1 = (CAD_FLOAT)fabs(spEndPoint->m_fZ - spVertex[i].m_fZ);
				spTexCoord[i].m_fX = fNumTexWidth * (1 - fWidth1 / fWidth);
				spTexCoord[i].m_fY = fNumTexHeight * (1 - fHeight1 / fHeight);
			}
		}
		break;
	}
	return TRUE;
}

BOOL CCAD_Entity::CalcNormal(const	SCAD_POINT	sVertex[3], SCAD_POINT *n)
{
	SCAD_POINT		tem1, tem2;
	
	tem1.m_fX = sVertex[1].m_fX - sVertex[0].m_fX;
	tem1.m_fY = sVertex[1].m_fY - sVertex[0].m_fY;
	tem1.m_fZ = sVertex[1].m_fZ - sVertex[0].m_fZ;
	
	tem2.m_fX = sVertex[2].m_fX - sVertex[0].m_fX;
	tem2.m_fY = sVertex[2].m_fY - sVertex[0].m_fY;
	tem2.m_fZ = sVertex[2].m_fZ - sVertex[0].m_fZ;
	
	n->m_fX = tem1.m_fY * tem2.m_fZ - tem1.m_fZ * tem2.m_fY;
	n->m_fY = tem1.m_fZ * tem2.m_fX - tem1.m_fX * tem2.m_fZ;
	n->m_fZ = tem1.m_fX * tem2.m_fY - tem1.m_fY * tem2.m_fX;
	
	double length = sqrt(n->m_fX * n->m_fX + n->m_fY * n->m_fY + n->m_fZ * n->m_fZ);
	if (IS_ZERO(length))
		return FALSE;
	n->m_fX = (float)(n->m_fX / length);
	n->m_fY = (float)(n->m_fY / length);
	n->m_fZ = (float)(n->m_fZ / length);
	return TRUE;
}

BOOL	CCAD_Entity::IsIgno(BOOL isBlock)
{
	if (m_bCode == C_NOENTITY)
		return true;
	if ( !isBlock ) {
		if (m_nLayer < 0 || m_pDoc->m_pLayerMgr->TotIndex() <= m_nLayer)
			return true;
	}
	return false;
}

BOOL CCAD_Entity::IsDimensionEntity()
{
	return (m_bCode==C_DIMLINEAR || m_bCode==C_DIMANGULAR || m_bCode==C_DIMRADIAL ||
			m_bCode==C_DIMLEADER );
}

void CCAD_Entity::SetMaxBoundSize()
{
	if (m_cWorldBound.IsInited())
		return;

	CAD_FLOAT fWidth = m_cWorldBound.Width();
	CAD_FLOAT fHeight = m_cWorldBound.Height();

	if (fWidth > fHeight)
		m_fMaxBoundSize = fWidth;
	else
		m_fMaxBoundSize = fHeight;
}

