#include "stdafx.h"
#include "CAD_DXFGlobal.h"
#include "CAD_PaintEntity.h"
#include <math.h>
/*
void ImportDxfHatchGetCountInform(CCAD_Doc *pDoc, CDXF_Hatch *pDxfHatch, CCAD_GemFrame *pGemFrame)
{
	pGemFrame->m_nTotGrp = (short)pDxfHatch->m_cHatchPath.GetSize();
	pGemFrame->m_pNum = new short[pGemFrame->m_nTotGrp];
	pGemFrame->m_nTotPt = 0;
	int	i, j, k;
	short	nSubTot;
	CAD_FLOAT fDelta;
	for (i=0; i<pGemFrame->m_nTotGrp; i++){
		SDXF_HatchPath sHatchPath = pDxfHatch->m_cHatchPath.GetAt(i);
		nSubTot = 0;
		for (j=0; j<sHatchPath.lEdgeCnt; j++){
			if (sHatchPath.spEdges[j].lType == DXF_HATCH_EDGE_LINE)
				nSubTot++;
// 			else if (sHatchPath.spEdges[j].lType == DXF_HATCH_EDGE_ARC){
// 				SDXF_HatchArc	sHatchArc = pDxfHatch->m_cHatchArcArray.GetAt(sHatchPath.spEdges[j].lIndex);
// 				ImportDxfHatchGetPtFromArc(pDoc, sHatchArc, (CCAD_GemFrame *)&pPaint->m_cGemPaint, k);
// 			}
// 			else if (sHatchPath.spEdges[j].lType == DXF_HATCH_EDGE_ELLIPARC){
// 				ImportDxfHatchGetPtFromEllipse(pDoc, sHatchEllipse, (CCAD_GemFrame *)&pPaint->m_cGemPaint, k);
// 			}
// 			else if (sHatchPath.spEdges[j].lType == DXF_HATCH_EDGE_POLYLINE){
// 				SDXF_HatchPolyline	sHatchPline = pDxfHatch->m_cHatchPolylineArray.GetAt(sHatchPath.spEdges[j].lIndex);
// 				ImportDxfHatchGetPtFromPline(pDoc, sHatchPline, (CCAD_GemFrame *)&pPaint->m_cGemPaint, k);
// 			}

			else if (sHatchPath.spEdges[j].lType == DXF_HATCH_EDGE_ARC) {
				SDXF_HatchArc	sHatchArc = pDxfHatch->m_cHatchArcArray.GetAt(sHatchPath.spEdges[j].lIndex);
				if (sHatchArc.dStartAngle > sHatchArc.dEndAngle)
					fDelta = (CAD_FLOAT)(sHatchArc.dEndAngle + LONGDOUBLE_NINETY - sHatchArc.dStartAngle);
				else
					fDelta = (CAD_FLOAT)(sHatchArc.dEndAngle - sHatchArc.dStartAngle);
				fDelta = (CAD_FLOAT)( fDelta * PAI / DOUBLE_NINETY );
				nSubTot += CountArcDivide(fDelta);
			}
			else if (sHatchPath.spEdges[j].lType == DXF_HATCH_EDGE_ELLIPARC) {
				SDXF_HatchEllipse	sHatchEllipse = pDxfHatch->m_cHatchEllipseArray.GetAt(sHatchPath.spEdges[j].lIndex);
				if (sHatchEllipse.dStartAngle > sHatchEllipse.dEndAngle)
					fDelta = (CAD_FLOAT)(sHatchEllipse.dEndAngle + LONGDOUBLE_NINETY - sHatchEllipse.dStartAngle);
				else
					fDelta = (CAD_FLOAT)(sHatchEllipse.dEndAngle - sHatchEllipse.dStartAngle);
				fDelta = (CAD_FLOAT)( fDelta * PAI / DOUBLE_NINETY );
				nSubTot += CountArcDivide(fDelta);
			}
			else if (sHatchPath.spEdges[j].lType == DXF_HATCH_EDGE_POLYLINE){
				SDXF_HatchPolyline	sHatchPline = pDxfHatch->m_cHatchPolylineArray.GetAt(sHatchPath.spEdges[j].lIndex);
				for (k=0; k<sHatchPline.lVertexCnt; k++){
					fDelta = (CAD_FLOAT)(atan(sHatchPline.spVertices[k].dBulge) * 4); 
					nSubTot += CountArcDivide(fDelta);
				}
			}
		}
		pGemFrame->m_pNum[i] = nSubTot;
		pGemFrame->m_nTotPt += nSubTot;
	}
	pGemFrame->m_pPt = new CCAD_Point[pGemFrame->m_nTotPt];
}
*/