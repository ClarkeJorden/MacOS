#include "stdafx.h"
#include "CAD_DXFGlobal.h"
#include "DXF_Ellipse.h"
#include "DXF_Hatch.h"
#include "CAD_EllipseEntity.h"
#include "CAD_Uds.h"

extern CCAD_Entity *ImportDxfEllipseArcFromEllipse(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity);

CCAD_Entity *ImportDxfEllipse(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity)
{
	if (((CDXF_Ellipse *)pDxfEntity)->m_dStartAngle != 0)
		return ImportDxfEllipseArcFromEllipse(pDoc, pDxfEntity);
	short	nSpaceDocID = -1;
	short	nUds = -1;
	if (pDxfEntity->m_lSpaceType != 0){
		nSpaceDocID = 0;
		nUds = 0;
	}
	CDXF_Ellipse		*pDxfEllipse = (CDXF_Ellipse *)pDxfEntity;
	CCAD_EllipseEntity	*pEllipse = new CCAD_EllipseEntity(pDoc, nSpaceDocID, nUds);

	CvtDxfEntityHeadToDrw(pDoc, pDxfEntity, pEllipse->m_nLayer, &pEllipse->m_cPen, &pEllipse->m_cBrush);
	CvtDxfBcsToCCAD_Bcs(pDxfEntity, pEllipse->m_cGemEllipse.m_cBcs);
	pEllipse->m_cGemEllipse.m_cThick.m_fZ = (CAD_FLOAT)pDxfEllipse->m_dThickness;
	CvtDxfCoordToCCAD_Vector(&pDxfEllipse->m_s3DCenter, pEllipse->m_cGemEllipse.m_cCp);
	CCAD_Point	tmpPt;
	CvtDxfCoordToCCAD_Vector(&pDxfEllipse->m_s3DMajorAxisEndPoint, tmpPt);
	tmpPt.m_fX += pEllipse->m_cGemEllipse.m_cCp.m_fX;
	tmpPt.m_fY += pEllipse->m_cGemEllipse.m_cCp.m_fY;
	tmpPt.m_fZ += pEllipse->m_cGemEllipse.m_cCp.m_fZ;
	pEllipse->m_cGemEllipse.m_fRx = pEllipse->m_cGemEllipse.m_cCp.CountLength(tmpPt);
	pEllipse->m_cGemEllipse.m_fRy = pEllipse->m_cGemEllipse.m_fRx * (CAD_FLOAT)pDxfEllipse->m_dScale;
	pEllipse->m_cGemEllipse.m_cCp.CountAngle2D(tmpPt, pEllipse->m_cGemEllipse.m_cDirect);

	return (CCAD_Entity *)pEllipse;
}

CCAD_Entity *ImportDxfHatchToEllipse(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity)
{
	short	nSpaceDocID = -1;
	short	nUds = -1;
	if (pDxfEntity->m_lSpaceType != 0){
		nSpaceDocID = 0;
		nUds = 0;
	}
	CDXF_Hatch			*pDxfHatch = (CDXF_Hatch *)pDxfEntity;
	CCAD_EllipseEntity	*pEllipse = new CCAD_EllipseEntity(pDoc, nSpaceDocID, nUds);

	CvtDxfEntityHeadToDrw(pDoc, pDxfEntity, pEllipse->m_nLayer, &pEllipse->m_cPen, &pEllipse->m_cBrush);
	CvtDxfBcsToCCAD_Bcs(pDxfEntity, pEllipse->m_cGemEllipse.m_cBcs);
	pEllipse->m_cGemEllipse.m_bDraw = DRAW_PAINT;

	SDXF_HatchEllipse	sHatchEllipse = pDxfHatch->m_cHatchEllipseArray.GetAt(0);
	pEllipse->m_cGemEllipse.m_cCp.m_fX = (CAD_FLOAT)sHatchEllipse.sCenter.dX;
	pEllipse->m_cGemEllipse.m_cCp.m_fY = (CAD_FLOAT)sHatchEllipse.sCenter.dY;

	CCAD_Point	tmpPt;
	tmpPt.m_fX = (CAD_FLOAT)sHatchEllipse.sEndPt.dX;
	tmpPt.m_fY = (CAD_FLOAT)sHatchEllipse.sEndPt.dY;
	tmpPt.m_fZ = (CAD_FLOAT)pEllipse->m_cGemEllipse.m_cCp.m_fZ;
	tmpPt.m_fX += pEllipse->m_cGemEllipse.m_cCp.m_fX;
	tmpPt.m_fY += pEllipse->m_cGemEllipse.m_cCp.m_fY;
	tmpPt.m_fZ += pEllipse->m_cGemEllipse.m_cCp.m_fZ;
	pEllipse->m_cGemEllipse.m_fRx = pEllipse->m_cGemEllipse.m_cCp.CountLength(tmpPt);
	pEllipse->m_cGemEllipse.m_fRy = pEllipse->m_cGemEllipse.m_fRx * (CAD_FLOAT)sHatchEllipse.dMainLen;
	pEllipse->m_cGemEllipse.m_cCp.CountAngle2D(tmpPt, pEllipse->m_cGemEllipse.m_cDirect);
	return (CCAD_Entity *)pEllipse;
}

