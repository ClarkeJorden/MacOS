#include "stdafx.h"
#include "CAD_Sheet.h"
#include "CAD_DxfGlobal.h"
#include "DXF_HeaderSection.h"

void ImportDxfHeader(CCAD_Doc *pDoc, CDXF_HeaderSection *pDxfHeader)
{
	pDoc->SetPenScale((CAD_FLOAT)pDxfHeader->m_dCELTSCALE);
	pDoc->m_pPenStyleMgr->m_nAct = pDoc->m_pPenStyleMgr->GetNameIndex(pDxfHeader->m_pszCELTYPE);
	pDoc->m_pLayerMgr->m_nAct = pDoc->m_pLayerMgr->GetNameIndex(pDxfHeader->m_pszCLAYER);
	pDoc->m_pTextStyleMgr->m_nAct = pDoc->m_pTextStyleMgr->GetNameIndex(pDxfHeader->m_pszTEXTSTYLE);
	pDoc->m_pDimStyleMgr->m_nAct = pDoc->m_pDimStyleMgr->GetNameIndex(pDxfHeader->m_pszDIMSTYLE);

	CCAD_Sheet	*pSheet = pDoc->GetSpaceDoc(0)->GetSheet();
	
	double dScale = pDxfHeader->m_dDIMSCALE;
	if (IS_ZERO(dScale))
		dScale = 1.0f;
	pSheet->m_fScale = (float)dScale;
	
	pSheet->m_cSize.m_fW = (float)pDxfHeader->m_sW2DLIMMAX.dX;
	pSheet->m_cSize.m_fH = (float)pDxfHeader->m_sW2DLIMMAX.dY;
	//pSheet->m_cFrame.m_fMaxX = (float)(pDxfHeader->m_sW3DPEXTMAX.dX / dScale);
	//pSheet->m_cFrame.m_fMaxY = (float)(pDxfHeader->m_sW3DPEXTMAX.dY / dScale);
	//pSheet->m_cFrame.m_fMaxZ = (float)(pDxfHeader->m_sW3DPEXTMAX.dZ / dScale);
	//pSheet->m_cFrame.m_fMinX = (float)(pDxfHeader->m_sW3DPEXTMIN.dX / dScale);
	//pSheet->m_cFrame.m_fMinY = (float)(pDxfHeader->m_sW3DPEXTMIN.dY / dScale);
	//pSheet->m_cFrame.m_fMinZ = (float)(pDxfHeader->m_sW3DPEXTMIN.dZ / dScale);
	//	modify end.
}

void ImportDxfHeaderSection(CDXF_MemFile * pDxfMemFile, CCAD_Doc *pDoc)
{
	CDXF_HeaderSection	*pDxfHeader;
	pDxfHeader = pDxfMemFile->m_cpDXFHeaderSection;
	ImportDxfHeader(pDoc, pDxfHeader);
}
