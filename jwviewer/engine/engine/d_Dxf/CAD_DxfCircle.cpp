#include "stdafx.h"
#include "CAD_DXFGlobal.h"
#include "DXF_Circle.h"
#include "DXF_Hatch.h"
#include "CAD_CircleEntity.h"

CCAD_Entity *ImportDxfCircle(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity)
{
	short	nSpaceDocID = -1;
	short	nUds = -1;
	if (pDxfEntity->m_lSpaceType != 0){
		nSpaceDocID = 0;
		nUds = 0;
	}
	CDXF_Circle			*pDxfCircle = (CDXF_Circle *)pDxfEntity;
	CCAD_CircleEntity	*pCircle = new CCAD_CircleEntity(pDoc, nSpaceDocID, nUds);

	CvtDxfEntityHeadToDrw(pDoc, pDxfEntity, pCircle->m_nLayer, &pCircle->m_cPen, &pCircle->m_cBrush);
	CvtDxfBcsToCCAD_Bcs(pDxfEntity, pCircle->m_cGemCircle.m_cBcs);
	pCircle->m_cGemCircle.m_cThick.m_fZ = (CAD_FLOAT)pDxfCircle->m_dThickness;
	CvtDxfCoordToCCAD_Vector(&pDxfCircle->m_s3DCenter, pCircle->m_cGemCircle.m_cCp);
	pCircle->m_cGemCircle.m_fR = (CAD_FLOAT)pDxfCircle->m_dRadius;

	return (CCAD_Entity *)pCircle;
}

CCAD_Entity *ImportDxfHatchToCircle(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity)
{
	short	nSpaceDocID = -1;
	short	nUds = -1;
	if (pDxfEntity->m_lSpaceType != 0){
		nSpaceDocID = 0;
		nUds = 0;
	}
	CDXF_Hatch			*pDxfHatch = (CDXF_Hatch *)pDxfEntity;
	CCAD_CircleEntity	*pCircle = new CCAD_CircleEntity(pDoc, nSpaceDocID, nUds);

	CvtDxfEntityHeadToDrw(pDoc, pDxfEntity, pCircle->m_nLayer, &pCircle->m_cPen, &pCircle->m_cBrush);
	CvtDxfBcsToCCAD_Bcs(pDxfEntity, pCircle->m_cGemCircle.m_cBcs);
	pCircle->m_cGemCircle.m_bDraw = DRAW_PAINT;

	SDXF_HatchArc	sHatchArc = pDxfHatch->m_cHatchArcArray.GetAt(0);
	pCircle->m_cGemCircle.m_cCp.m_fX = (CAD_FLOAT)sHatchArc.sCenter.dX;
	pCircle->m_cGemCircle.m_cCp.m_fY = (CAD_FLOAT)sHatchArc.sCenter.dY;
	pCircle->m_cGemCircle.m_cCp.m_fZ = (CAD_FLOAT)pDxfHatch->m_s3DHeightPoint.dZ;
	pCircle->m_cGemCircle.m_fR = (CAD_FLOAT)sHatchArc.dRadius;

	return (CCAD_Entity *)pCircle;
}
