#include "stdafx.h"
#include "CAD_DXFGlobal.h"
#include "DXF_Hatch.h"
#include "CAD_CircleEntity.h"
#include "CAD_ArcEntity.h"
#include "CAD_EllipseEntity.h"
#include "CAD_EllipseArcEntity.h"
#include "CAD_PaintEntity.h"
#include "CAD_CurveEntity.h"
//#include "CAD_HatchEntity.h"

CCAD_Entity *ImportDxfHatchToCircle(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity);
CCAD_Entity *ImportDxfHatchToEllipse(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity);
CCAD_Entity *ImportDxfHatchToCurve(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity);
CCAD_Entity *ImportDxfHatchToPaint(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity);
CCAD_Entity *ImportDxfHatchToHatch(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity);

CCAD_Entity *ImportDxfHatch(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity)
{
	CDXF_Hatch	*pDxfHatch = (CDXF_Hatch *)pDxfEntity;
	return NULL;//-tempp
/*
	if (strcmp(pDxfHatch->m_pszHatchPatternName, "SOLID") == 0){
		int iArcSize = pDxfHatch->m_cHatchArcArray.GetSize();
		int iLineSize = pDxfHatch->m_cHatchLineArray.GetSize();
		int	iEllipseSize = pDxfHatch->m_cHatchEllipseArray.GetSize();
		int iSplineSize = pDxfHatch->m_cHatchSplineArray.GetSize();
		int iPlineSize = pDxfHatch->m_cHatchPolylineArray.GetSize();
		if (iArcSize == 1 && iLineSize == 0 && iEllipseSize == 0 && iSplineSize == 0 && iPlineSize == 0)
			return ImportDxfHatchToCircle(pDoc, pDxfEntity);
		else if (iArcSize == 0 && iLineSize == 0 && iEllipseSize == 1 && iSplineSize == 0 && iPlineSize == 0)
			return ImportDxfHatchToEllipse(pDoc, pDxfEntity);
		else if (iArcSize == 0 && iLineSize == 0 && iEllipseSize == 0 && iSplineSize == 1 && iPlineSize == 0)
			return ImportDxfHatchToCurve(pDoc, pDxfEntity);
		else if (iSplineSize == 0)
			return ImportDxfHatchToPaint(pDoc, pDxfEntity);
	}

	CCAD_HatchEntity	*pHatch = NULL;
	if (pDxfHatch->m_cHatchSplineArray.GetSize() == 0)
		pHatch = (CCAD_HatchEntity *)ImportDxfHatchToHatch(pDoc, pDxfEntity);
	if (pHatch == NULL)
		return NULL;

	pHatch->m_cGemHatch.m_bDraw = DRAW_PAINT;
	if (pDxfHatch->m_lHatchStyle == DXF_HATCH_STYLE_ODD)
		pHatch->m_cGemHatch.m_bDrawMode = HATCH_DRAW_NORMAL;
	else if (pDxfHatch->m_lHatchStyle == DXF_HATCH_STYLE_ENTIRE)
		pHatch->m_cGemHatch.m_bDrawMode = HATCH_DRAW_ALL;
	else if (pDxfHatch->m_lHatchStyle == DXF_HATCH_STYLE_OUTMOST)
		pHatch->m_cGemHatch.m_bDrawMode = HATCH_DRAW_CAPSEL;
	pHatch->m_cGemHatch.m_cAngle.SetFromDegree((CAD_FLOAT)pDxfHatch->m_dHatchPatternAngle);
	pHatch->m_cGemHatch.m_fScale = (CAD_FLOAT)pDxfHatch->m_dHatchPatternScaleOrOffset;
	int i, j;
	if (pDoc->m_pHpMgr->GetStyle(pDxfHatch->m_pszHatchPatternName) == NULL){
		CCAD_HatchPattern	cHatchPattern;
		sprintf(cHatchPattern.m_szName, "%s", pDxfHatch->m_pszHatchPatternName);
		for (i=0; i<pDxfHatch->m_lHatchPatternCnt; i++){
			DXF_DOUBLE	dSX = pDxfHatch->m_spHatchPattern[i].sPatternStart.dX;
			DXF_DOUBLE	dSY = pDxfHatch->m_spHatchPattern[i].sPatternStart.dY;
			CCAD_Angle	cElemAngle;
			cElemAngle.SetFromDegree((CAD_FLOAT)pDxfHatch->m_spHatchPattern[i].dPatternAngle);
			CCAD_HpElement	cHpElem;
			if (pDxfHatch->m_spHatchPattern[i].lDashCnt != 0){
				for (j=0; j<pDxfHatch->m_spHatchPattern[i].lDashCnt; j+=2){
					cHpElem.m_bRayFlag = NO;
					cHpElem.m_fSX = (CAD_FLOAT)dSX;
					cHpElem.m_fSY = (CAD_FLOAT)dSY;
					CAD_FLOAT	fDashLen = (CAD_FLOAT)(fabs(pDxfHatch->m_spHatchPattern[i].dpDashes[j]) +
											fabs(pDxfHatch->m_spHatchPattern[i].dpDashes[j+1]));
					cHpElem.m_fDX1 = cElemAngle.cos() * fDashLen;
					cHpElem.m_fDY1 = cElemAngle.sin() * fDashLen;
					cHpElem.m_fDX2 = (CAD_FLOAT)pDxfHatch->m_spHatchPattern[i].sPatternOffset.dX;
					cHpElem.m_fDY2 = (CAD_FLOAT)pDxfHatch->m_spHatchPattern[i].sPatternOffset.dY;
					cHpElem.m_fLX = (CAD_FLOAT)(cElemAngle.cos() * fabs(pDxfHatch->m_spHatchPattern[i].dpDashes[j]));
					cHpElem.m_fLY = (CAD_FLOAT)(cElemAngle.sin() * fabs(pDxfHatch->m_spHatchPattern[i].dpDashes[j]));
					cHatchPattern.Add(&cHpElem);
					dSX += (DXF_DOUBLE)cHpElem.m_fDX1;
					dSY += (DXF_DOUBLE)cHpElem.m_fDY1;
				}
			}
			else {
				cHpElem.m_bRayFlag = YES;
				cHpElem.m_fSX = (CAD_FLOAT)dSX;
				cHpElem.m_fSY = (CAD_FLOAT)dSY;
				cHpElem.m_fDX1 = (CAD_FLOAT)pDxfHatch->m_spHatchPattern[i].sPatternOffset.dX;
				cHpElem.m_fDY1 = (CAD_FLOAT)pDxfHatch->m_spHatchPattern[i].sPatternOffset.dY;
				cHpElem.m_fDX2 = cHpElem.m_fDY2 = 0;
				cHpElem.m_fLX = (CAD_FLOAT)(cElemAngle.cos() * 10);
				cHpElem.m_fLY = (CAD_FLOAT)(cElemAngle.sin() * 10);
				cHatchPattern.Add(&cHpElem);
			}
		}
		pDoc->m_pHpMgr->Add((CCAD_Style *)&cHatchPattern);
	}
	pHatch->m_cGemHatch.m_nPattern = pDoc->m_pHpMgr->GetNameIndex(pDxfHatch->m_pszHatchPatternName);
	pHatch->m_cGemHatch.CountPt();
	if (!pHatch->m_cGemHatch.CreateLines(*pDoc->m_pHpMgr)){
		pHatch->m_cGemHatch.RemoveAll();
		delete pHatch;
		pHatch = NULL;
	}
	return (CCAD_Entity *)pHatch;
*/
}