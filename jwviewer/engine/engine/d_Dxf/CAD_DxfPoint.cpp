#include "stdafx.h"
#include "CAD_DXFGlobal.h"
#include "DXF_Point.h"
#include "CAD_PointEntity.h"

CCAD_Entity *ImportDxfPoint(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity)
{
	short	nSpaceDocID = -1;
	short	nUds = -1;
	if (pDxfEntity->m_lSpaceType != 0){
		nSpaceDocID = 0;
		nUds = 0;
	}
	CDXF_Point			*pDxfPoint = (CDXF_Point *)pDxfEntity;
	CCAD_PointEntity	*pPoint = new CCAD_PointEntity(pDoc, nSpaceDocID, nUds);

	CvtDxfEntityHeadToDrw(pDoc, pDxfEntity, pPoint->m_nLayer, &pPoint->m_cPen);
	CvtDxfBcsToCCAD_Bcs(pDxfEntity, pPoint->m_cGemPoint.m_cBcs);
	pPoint->m_cGemPoint.m_bType = POINT_PAINT;
	CvtDxfCoordToCCAD_Vector(&pDxfPoint->m_s3DCoord, pPoint->m_cGemPoint.m_cPt);
	return (CCAD_Entity *)pPoint;
}
