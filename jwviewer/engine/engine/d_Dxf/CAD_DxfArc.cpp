#include "stdafx.h"
#include "CAD_DXFGlobal.h"
#include "DXF_Arc.h"
#include "CAD_ArcEntity.h"
#include "CAD_Uds.h"

CCAD_Entity *ImportDxfArc(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity)
{
	short	nSpaceDocID = -1;
	short	nUds = -1;
	if (pDxfEntity->m_lSpaceType != 0){
		nSpaceDocID = 0;
		nUds = 0;
	}
	CDXF_Arc	*pDxfArc = (CDXF_Arc *)pDxfEntity;
	CCAD_ArcEntity	*pArc = new CCAD_ArcEntity(pDoc, nSpaceDocID, nUds);

	CvtDxfEntityHeadToDrw(pDoc, pDxfEntity, pArc->m_nLayer, &pArc->m_cPen, &pArc->m_cBrush);
	CvtDxfBcsToCCAD_Bcs(pDxfEntity, pArc->m_cGemArc.m_cBcs);
	pArc->m_cGemArc.m_cThick.m_fZ = (CAD_FLOAT)pDxfArc->m_dThickness;
	
	//	CvtDxfCoordToCCAD_Vector(&pDxfArc->m_s3DCenter, pArc->m_cGemArc.m_cCp);
	
	pArc->m_cGemArc.m_cBcs.m_cOrg.m_fX += (float)pDxfArc->m_s3DCenter.dX;
	pArc->m_cGemArc.m_cBcs.m_cOrg.m_fY += (float)pDxfArc->m_s3DCenter.dY;
	pArc->m_cGemArc.m_cBcs.m_cOrg.m_fZ += (float)pDxfArc->m_s3DCenter.dZ;

	pArc->m_cGemArc.m_fR = (CAD_FLOAT)pDxfArc->m_dRadius;
	pArc->m_cGemArc.m_cAlfa.SetFromDegree((CAD_FLOAT)pDxfArc->m_dStartAngle);
	CAD_FLOAT fDelta = (CAD_FLOAT)pDxfArc->m_dEndAngle;
	if (pDxfArc->m_dStartAngle > pDxfArc->m_dEndAngle)
		fDelta = fDelta + (CAD_FLOAT)(LONGDOUBLE_NINETY - pDxfArc->m_dStartAngle);
	else
		fDelta = fDelta - (CAD_FLOAT)pDxfArc->m_dStartAngle;
	pArc->m_cGemArc.m_cDelta.SetFromDegree(fDelta);	
	return (CCAD_Entity *)pArc;
}

