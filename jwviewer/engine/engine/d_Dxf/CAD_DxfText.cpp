#include "stdafx.h"
#include "CAD_DXFGlobal.h"
#include "DXF_Text.h"
#include "CAD_TextEntity.h"
#include "CAD_LeaderDimEntity.h"
#include "CAD_Uds.h"

CCAD_Entity *ImportDxfText(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity)
{
	short	nSpaceDocID = -1;
	short	nUds = -1;
	if (pDxfEntity->m_lSpaceType != 0){
		nSpaceDocID = 0;
		nUds = 0;
	}
	CDXF_Text		*pDxfText = (CDXF_Text *)pDxfEntity;
	CCAD_TextEntity	*pText = new CCAD_TextEntity(pDoc, nSpaceDocID, nUds);

	CCAD_Brush	cBrush;
	CvtDxfEntityHeadToDrw(pDoc, pDxfEntity, pText->m_nLayer, &pText->m_cPen, &cBrush);
	CvtDxfBcsToCCAD_Bcs(pDxfEntity, pText->m_cGemText.m_cBcs);
	pText->m_cGemText.m_cThick.m_fZ = (CAD_FLOAT)pDxfText->m_dThickness;
	pText->m_cGemText.m_cDirect.SetFromDegree((CAD_FLOAT)pDxfText->m_dRotateAngle);
	pText->m_cGemText.m_cOblique.SetFromDegree((CAD_FLOAT)pDxfText->m_dObliqueAngle);
	pText->m_cGemText.m_fHeight = (CAD_FLOAT)pDxfText->m_dTextHeight;
	pText->m_cGemText.m_fWidth = (CAD_FLOAT)(pText->m_cGemText.m_fHeight * pDxfText->m_dRelativeXScale);
	pText->m_cGemText.m_nStyle = pDoc->m_pTextStyleMgr->GetNameIndex(pDxfText->m_pszTextStyleName);
	pText->m_cGemText.m_pTextStyle = (CCAD_TextStyle *)(pDoc->m_pTextStyleMgr->GetStyle(pText->m_cGemText.m_nStyle));
	pText->m_cGemText.m_nLength = strlen(pDxfText->m_pszText);
	pText->m_cGemText.m_pStr = new char[pText->m_cGemText.m_nLength];
	memcpy(pText->m_cGemText.m_pStr, pDxfText->m_pszText, pText->m_cGemText.m_nLength);

/*	
	if (pDxfText->m_lVertJustCode == DXF_TEXT_VERTJUST_BASELINE || pDxfText->m_lVertJustCode == DXF_TEXT_VERTJUST_BOTTOM){
		if (pDxfText->m_lHoriJustCode == DXF_TEXT_HORZJUST_LEFT || pDxfText->m_lHoriJustCode == DXF_TEXT_HORZJUST_ALIGNED || pDxfText->m_lHoriJustCode == DXF_TEXT_HORZJUST_FIT)
			pText->m_cGemText.m_bPosFlag = POS_LB;
		else if(pDxfText->m_lHoriJustCode == DXF_TEXT_HORZJUST_CENTER)
			pText->m_cGemText.m_bPosFlag = POS_CB;
		else if(pDxfText->m_lHoriJustCode == DXF_TEXT_HORZJUST_RIGHT)
			pText->m_cGemText.m_bPosFlag = POS_RB;
		else if(pDxfText->m_lHoriJustCode == DXF_TEXT_HORZJUST_MIDDLE)
			pText->m_cGemText.m_bPosFlag = POS_CENTER;
	}
	else if (pDxfText->m_lVertJustCode == DXF_TEXT_VERTJUST_MIDDLE){
		if (pDxfText->m_lHoriJustCode == DXF_TEXT_HORZJUST_LEFT)
			pText->m_cGemText.m_bPosFlag = POS_LC;
		else if(pDxfText->m_lHoriJustCode == DXF_TEXT_HORZJUST_CENTER)
			pText->m_cGemText.m_bPosFlag = POS_CENTER;
		else
			pText->m_cGemText.m_bPosFlag = POS_RC;
	}
	if (pDxfText->m_lVertJustCode == DXF_TEXT_VERTJUST_MIDDLE) {
		if (pDxfText->m_lHoriJustCode == DXF_TEXT_HORZJUST_LEFT)
			pText->m_cGemText.m_bPosFlag = POS_LT;
		else if(pDxfText->m_lHoriJustCode == DXF_TEXT_HORZJUST_CENTER)
			pText->m_cGemText.m_bPosFlag = POS_CT;
		else
			pText->m_cGemText.m_bPosFlag = POS_RT;
	}*/
	pText->m_cGemText.m_bPosFlag = POS_LB;
	
	CvtDxfCoordToCCAD_Vector(&pDxfText->m_s3DFirstAlignPoint, pText->m_cGemText.m_cPt);

	return (CCAD_Entity *)pText;
}
