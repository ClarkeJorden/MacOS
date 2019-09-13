#include "stdafx.h"
#include "CAD_DXFGlobal.h"
#include "DXF_Ellipse.h"
#include "CAD_EllipseArcEntity.h"
#include "CAD_Uds.h"

CCAD_Entity *ImportDxfEllipseArcFromEllipse(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity)
{
	short	nSpaceDocID = -1;
	short	nUds = -1;
	if (pDxfEntity->m_lSpaceType != 0){
		nSpaceDocID = 0;
		nUds = 0;
	}
	CDXF_Ellipse			*pDxfEllipse = (CDXF_Ellipse *)pDxfEntity;
	CCAD_EllipseArcEntity	*pEllipseArc = new CCAD_EllipseArcEntity(pDoc, nSpaceDocID, nUds);

	CvtDxfEntityHeadToDrw(pDoc, pDxfEntity, pEllipseArc->m_nLayer, &pEllipseArc->m_cPen, &pEllipseArc->m_cBrush);
	CvtDxfBcsToCCAD_Bcs(pDxfEntity, pEllipseArc->m_cGemEllipseArc.m_cBcs);
	pEllipseArc->m_cGemEllipseArc.m_cThick.m_fZ = (CAD_FLOAT)pDxfEllipse->m_dThickness;
	CvtDxfCoordToCCAD_Vector(&pDxfEllipse->m_s3DCenter, pEllipseArc->m_cGemEllipseArc.m_cCp);
	CCAD_Point	tmpPt;
	CvtDxfCoordToCCAD_Vector(&pDxfEllipse->m_s3DMajorAxisEndPoint, tmpPt);
	tmpPt.m_fX += pEllipseArc->m_cGemEllipseArc.m_cCp.m_fX;
	tmpPt.m_fY += pEllipseArc->m_cGemEllipseArc.m_cCp.m_fY;
	tmpPt.m_fZ += pEllipseArc->m_cGemEllipseArc.m_cCp.m_fZ;
	pEllipseArc->m_cGemEllipseArc.m_fRx = pEllipseArc->m_cGemEllipseArc.m_cCp.CountLength(tmpPt);
	pEllipseArc->m_cGemEllipseArc.m_fRy = pEllipseArc->m_cGemEllipseArc.m_fRx * (CAD_FLOAT)pDxfEllipse->m_dScale;
	pEllipseArc->m_cGemEllipseArc.m_cCp.CountAngle2D(tmpPt, pEllipseArc->m_cGemEllipseArc.m_cDirect);
	pEllipseArc->m_cGemEllipseArc.m_cAlfa.SetFromDegree((CAD_FLOAT)pDxfEllipse->m_dStartAngle);
	CAD_FLOAT fDelta = (CAD_FLOAT)pDxfEllipse->m_dEndAngle;
	if (pDxfEllipse->m_dStartAngle > pDxfEllipse->m_dEndAngle)
		fDelta = fDelta + (CAD_FLOAT)(LONGDOUBLE_NINETY - pDxfEllipse->m_dStartAngle);
	else
		fDelta = fDelta - (CAD_FLOAT)pDxfEllipse->m_dStartAngle;
	pEllipseArc->m_cGemEllipseArc.m_cDelta.SetFromDegree(fDelta);	
	return (CCAD_Entity *)pEllipseArc;
}
