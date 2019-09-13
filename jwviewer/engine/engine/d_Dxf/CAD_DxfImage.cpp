#include "stdafx.h"
#include "CAD_DXFGlobal.h"
#include "DXF_Image.h"
#include "CAD_ImageEntity.h"
#include "CAD_Global.h"
#include <string.h>

CCAD_Entity *ImportDxfImage(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity)
{
	short	nSpaceDocID = -1;
	short	nUds = -1;
	if (pDxfEntity->m_lSpaceType != 0){
		nSpaceDocID = 0;
		nUds = 0;
	}
	CDXF_Image	*pDxfImage = (CDXF_Image *)pDxfEntity;
	char		szImageName[256];
	int			ch='\\';
	char	*pPath = strrchr(pDxfImage->m_pszImageName, ch);
	if (pPath != NULL){
		pPath++;
		strcpy(szImageName, pPath);
	}
	else
		strcpy(szImageName, pDxfImage->m_pszImageName);
    //-tempp if (strnicmp(&szImageName[strlen(szImageName)-3], "BMP", 3) != 0)
    if (strncmp(&szImageName[strlen(szImageName)-3], "BMP", 3) != 0)
		return NULL;
	szImageName[strlen(szImageName)-4] = 0;

	CCAD_ImageEntity	*pImage = new CCAD_ImageEntity(pDoc, nSpaceDocID, nUds);
	CvtDxfEntityHeadToDrw(pDoc, pDxfEntity, pImage->m_nLayer, &pImage->m_cPen);
	CvtDxfBcsToCCAD_Bcs(pDxfEntity, pImage->m_cGemImage.m_cBcs);
	pImage->m_cGemImage.m_bDraw = DRAW_PAINT;

	CCAD_Angle	cAlfa((CAD_FLOAT)pDxfImage->m_s3DUVector.dX, (CAD_FLOAT)pDxfImage->m_s3DUVector.dY);
	CCAD_Point	pt0;
	CCAD_Point	cU, cV;
	CvtDxfCoordToCCAD_Vector(&pDxfImage->m_s3DUVector, cU);
	CvtDxfCoordToCCAD_Vector(&pDxfImage->m_s3DVVector, cV);
	CAD_FLOAT	fOldXSize = cU.CountLength(pt0);
	CAD_FLOAT	fOldYSize = cV.CountLength(pt0);

	pt0.m_fX = pt0.m_fY = pt0.m_fZ = 0;
	if (pDxfImage->m_lClipType == DXF_IMAGE_CLIPPING_BOUND_RECT)
		pImage->m_cGemImage.m_nPtNum = 4;
	else
		pImage->m_cGemImage.m_nPtNum = (short)(pDxfImage->m_lClipVertexCnt-1);
	pImage->m_cGemImage.m_pPt = new CCAD_Vector[pImage->m_cGemImage.m_nPtNum];

	CCAD_Vector	cCp((CAD_FLOAT)pDxfImage->m_s3DInsertPoint.dX, (CAD_FLOAT)pDxfImage->m_s3DInsertPoint.dY, (CAD_FLOAT)0);
	CAD_FLOAT	fY;
	CAD_FLOAT	fSin = cAlfa.sin();
	CAD_FLOAT	fCos = cAlfa.cos();

	if (pDxfImage->m_lClipType == DXF_IMAGE_CLIPPING_BOUND_RECT) {
		fY = (CAD_FLOAT)(pDxfImage->m_spClipPoints[0].dY + 0.5);
		pImage->m_cGemImage.m_pPt->m_fX = (CAD_FLOAT)((pDxfImage->m_spClipPoints[0].dX + 0.5) * fOldXSize + pDxfImage->m_s3DInsertPoint.dX);
		pImage->m_cGemImage.m_pPt->m_fY = (CAD_FLOAT)(fY * fOldYSize + pDxfImage->m_s3DInsertPoint.dY);
		pImage->m_cGemImage.m_pPt->Rotate(cCp, fSin, fCos);

		fY = (CAD_FLOAT)(pDxfImage->m_spClipPoints[1].dY + 0.5);
		(pImage->m_cGemImage.m_pPt+1)->m_fX = (CAD_FLOAT)((pDxfImage->m_spClipPoints[0].dX + 0.5) * fOldXSize + pDxfImage->m_s3DInsertPoint.dX);
		(pImage->m_cGemImage.m_pPt+1)->m_fY = (CAD_FLOAT)(fY * fOldYSize + pDxfImage->m_s3DInsertPoint.dY);
		(pImage->m_cGemImage.m_pPt+1)->Rotate(cCp, fSin, fCos);

		fY = (CAD_FLOAT)(pDxfImage->m_spClipPoints[1].dY + 0.5);
		(pImage->m_cGemImage.m_pPt+2)->m_fX = (CAD_FLOAT)((pDxfImage->m_spClipPoints[1].dX + 0.5) * fOldXSize + pDxfImage->m_s3DInsertPoint.dX);
		(pImage->m_cGemImage.m_pPt+2)->m_fY = (CAD_FLOAT)(fY * fOldYSize + pDxfImage->m_s3DInsertPoint.dY);
		(pImage->m_cGemImage.m_pPt+2)->Rotate(cCp, fSin, fCos);

		fY = (CAD_FLOAT)(pDxfImage->m_spClipPoints[0].dY + 0.5);
		(pImage->m_cGemImage.m_pPt+3)->m_fX = (CAD_FLOAT)((pDxfImage->m_spClipPoints[1].dX + 0.5) * fOldXSize + pDxfImage->m_s3DInsertPoint.dX);
		(pImage->m_cGemImage.m_pPt+3)->m_fY = (CAD_FLOAT)(fY * fOldYSize + pDxfImage->m_s3DInsertPoint.dY);
		(pImage->m_cGemImage.m_pPt+3)->Rotate(cCp, fSin, fCos);
	}
	else {
		short	i;
		for (i = 0; i<pImage->m_cGemImage.m_nPtNum; i++){
			fY = (CAD_FLOAT)(pDxfImage->m_s2DSize.dY - (pDxfImage->m_spClipPoints[i].dY + 0.5));
			(pImage->m_cGemImage.m_pPt+i)->m_fX = (CAD_FLOAT)((pDxfImage->m_spClipPoints[i].dX + 0.5) * fOldXSize + pDxfImage->m_s3DInsertPoint.dX);
			(pImage->m_cGemImage.m_pPt+i)->m_fY = (CAD_FLOAT)(fY * fOldYSize + pDxfImage->m_s3DInsertPoint.dY);
			(pImage->m_cGemImage.m_pPt+i)->Rotate(cCp, fSin, fCos);
		}
	}
	CCAD_Image	*pImageData = (CCAD_Image *)pDoc->m_pImageMgr->GetStyle(szImageName);
/*
	if (pImageData == NULL){
		pImageData = new CCAD_Image(pDoc->m_hHeap);
		if (!pImageData->ReadImageFile(pDxfImage->m_pszImageName)){
			delete pImageData;
			delete pImage;
			return NULL;
		}
		sprintf(pImageData->m_szName, "%s", szImageName);
		pDoc->m_pImageMgr->Add(pImageData);
		pImage->m_cGemImage.m_nIndex = pDoc->m_pImageMgr->TotIndex()-1;
		pImage->m_cGemImage.m_pImage = pImageData;
	}
	else {
		pImage->m_cGemImage.m_nIndex = pDoc->m_pImageMgr->GetIndex(pImageData);
		pImage->m_cGemImage.m_pImage = pImageData;
	}
*/
	pImage->m_cGemImage.m_cBcs.ZRotate(cAlfa);
	return (CCAD_Entity *)pImage;
}
