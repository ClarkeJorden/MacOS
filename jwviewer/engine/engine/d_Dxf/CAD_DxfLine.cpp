#include "stdafx.h"
#include "CAD_DXFGlobal.h"
#include "DXF_Line.h"
#include "CAD_LineEntity.h"

CCAD_Entity *ImportDxfLine(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity)
{
	short	nSpaceDocID = -1;
	short	nUds = -1;
	if (pDxfEntity->m_lSpaceType != 0){
		nSpaceDocID = 0;
		nUds = 0;
	}
	CDXF_Line		*pDxfLine = (CDXF_Line *)pDxfEntity;
	CCAD_LineEntity	*pLine = new CCAD_LineEntity(pDoc, nSpaceDocID, nUds);

	CvtDxfEntityHeadToDrw(pDoc, pDxfEntity, pLine->m_nLayer, &pLine->m_cPen);
	CvtDxfBcsToCCAD_Bcs(pDxfEntity, pLine->m_cGemLine.m_cBcs);
	pLine->m_cGemLine.m_cThick.m_fZ = (CAD_FLOAT)pDxfLine->m_dThickness;
	CvtDxfCoordToCCAD_Vector(&pDxfLine->m_s3DStartPoint, pLine->m_cGemLine.m_cSp);
	CvtDxfCoordToCCAD_Vector(&pDxfLine->m_s3DEndPoint, pLine->m_cGemLine.m_cEp);
	return (CCAD_Entity *)pLine;
}
