#include "stdafx.h"
#include "CAD_DxfGlobal.h"
#include "DXF_Table.h"
#include "DXF_LayerTable.h"
#include "DXF_LTypeTable.h"
#include "DXF_StyleTable.h"
#include "DXF_DimStyleTable.h"
#include "DXF_UCSTable.h"
#include "DXF_VPortTable.h"
#include "DXF_ViewTable.h"
#include "CAD_DimStyle.h"
#include "CAD_TextStyle.h"

///////////////////////// Table Section ////////////////////////
void ImportDxfLayer(CCAD_Doc *pDoc, CDXF_LayerTable *cpLayer);
void ImportDxfLType(CCAD_Doc *pDoc, CDXF_LTypeTable *cpLType);
void ImportDxfStyle(CCAD_Doc *pDoc, CDXF_StyleTable *cpStyle);
void ImportDxfDimStyle(CCAD_Doc *pDoc, CDXF_DimStyleTable *cpDimStyle);
void ImportDxfUCS(CCAD_Doc *pDoc, CDXF_UCSTable *cpUCS);
void ImportDxfVPort(CCAD_Doc *pDoc, CDXF_VPortTable *cpVPort);
void ImportDxfView(CCAD_Doc *pDoc, CDXF_ViewTable *cpView);

void ImportDxfLayer(CCAD_Doc *pDoc, CDXF_LayerTable *cpLayer)
{
	CCAD_Layer	cLayer;

	sprintf(cLayer.m_szName, "%s", cpLayer->m_pszLayerName);
	if ((cpLayer->m_lLayerCode & DXF_LAYER_FREEZE) || (cpLayer->m_lLayerCode & DXF_LAYER_DEFFROZEN))
		cLayer.SetState( LAYER_EDITABLE | LAYER_ON, FALSE );
	if (cpLayer->m_lLayerCode & DXF_LAYER_LOCK)
		cLayer.SetState( LAYER_EDITABLE, FALSE );
	SDXF_RGBColor	sDxf_Rgb = sDXF_CvtDXFToRGBColor(cpLayer->m_lLayerColor);
	cLayer.m_cPen.m_rgbColor = RGB(sDxf_Rgb.bRed, sDxf_Rgb.bGreen, sDxf_Rgb.bBlue);
	cLayer.m_cPen.m_nStyle = pDoc->m_pPenStyleMgr->GetNameIndex(cpLayer->m_pszLineTypeName);
	cLayer.m_cPen.m_fScale = 1;
	cLayer.m_cBrush.m_rgbColor = cLayer.m_cPen.m_rgbColor;
	cLayer.m_cBrush.m_nPattern = BRS_SOLID;
	pDoc->m_pLayerMgr->Add(&cLayer);
}

void ImportDxfLType(CCAD_Doc *pDoc, CDXF_LTypeTable *cpLType)
{
	CCAD_PenStyle	cPenStyle;

	//if (cpLType->m_lLTypeCode == 0)//JW
    {
		cPenStyle.m_fLength = (CAD_FLOAT)cpLType->m_dTotPatLen;
		cPenStyle.m_nTotPart = (short)cpLType->m_lLTypeElemNum;
		sprintf(cPenStyle.m_szName, "%s", cpLType->m_pszLineTypeName);
		for (short i=0; i<cPenStyle.m_nTotPart; i++)
			cPenStyle.m_fPart[i] = (CAD_FLOAT)(cpLType->m_spLTypeElement+i)->dDashLen *2;

		pDoc->m_pPenStyleMgr->Add(&cPenStyle);
	}
}

void ImportDxfStyle(CCAD_Doc *pDoc, CDXF_StyleTable *cpStyle)
{
	CCAD_TextStyle	cTextStyle;

	sprintf(cTextStyle.m_szName, "%s", cpStyle->m_pszStyleName);
	if (cpStyle->m_dFixedTextHeight == 0.0)
		cTextStyle.m_fHeight = (CAD_FLOAT)cpStyle->m_dLastTextHeight;
	else
		cTextStyle.m_fHeight = (CAD_FLOAT)cpStyle->m_dFixedTextHeight;
	cTextStyle.m_fWidth = cTextStyle.m_fHeight * (CAD_FLOAT)cpStyle->m_dWidthFactor;


/*	char	szBuffer[MAX_PATH];
	GetSystemDirectory(szBuffer, MAX_PATH);
	char* szTemp = strrchr(szBuffer, '\\');
	if (szTemp != NULL)
		*szTemp = 0x00;
	CString		cFullPath;
	cFullPath.Empty();
	cFullPath = szBuffer;
	cFullPath += "\\FONTS\\";
	cFullPath += cpStyle->m_pszFirstFontFileName;

	if( ! SetFaceName(cFullPath, cTextStyle.m_szFaceName) )
*/		sprintf(cTextStyle.m_szFaceName, "%s", cpStyle->m_pszFontFaceName);

	if (cpStyle->m_lStyleCode & DXF_STYLE_VERTICAL)
		cTextStyle.m_sEffect.vert = 1;
	if (cpStyle->m_lTextGenCode & DXF_STYLE_TEXTGEN_UPSIDEDOWN)
		cTextStyle.m_sEffect.upside = 1;
	if (cpStyle->m_lTextGenCode & DXF_STYLE_TEXTGEN_BACKWARD)
		cTextStyle.m_sEffect.back = 1;
	cTextStyle.m_fAngle = (CAD_FLOAT)cpStyle->m_dObliqueAngle;
	pDoc->m_pTextStyleMgr->Add(&cTextStyle);
}

void ImportDxfDimStyle(CCAD_Doc *pDoc, CDXF_DimStyleTable *cpStyle)
{
	CCAD_DimStyle	cDimStyle;

	sprintf(cDimStyle.m_szName, "%s", cpStyle->m_pszDimStyleName);

	cDimStyle.m_sDimArrow.fMarkSize = (CAD_FLOAT)(cpStyle->m_dDIMCEN * cpStyle->m_dDIMSCALE);
	cDimStyle.m_sDimArrow.fSize = (CAD_FLOAT)(cpStyle->m_dDIMASZ * cpStyle->m_dDIMSCALE);
//	cDimStyle.m_sDimArrow.nMark = (CAD_FLOAT)cpStyle-
	cDimStyle.m_sDimArrow.nFirst = (short)cpStyle->m_lARROWTYPE1;
	cDimStyle.m_sDimArrow.nSecond = (short)cpStyle->m_lARROWTYPE2;
	cDimStyle.m_sDimArrow.nLeader = (short)cpStyle->m_lARROWTYPE3;
	cDimStyle.m_sDimExt.fBase = (CAD_FLOAT)(cpStyle->m_dDIMEXE * cpStyle->m_dDIMSCALE);
	cDimStyle.m_sDimExt.fExt = (CAD_FLOAT)(cpStyle->m_dDIMEXO * cpStyle->m_dDIMSCALE);
//	cDimStyle.m_sDimExt.nThick = (CAD_FLOAT)cpStyle->m_dDIMLWE;		//2000
	SDXF_RGBColor	sDxf_Rgb = sDXF_CvtDXFToRGBColor(cpStyle->m_lDIMCLRE);
	cDimStyle.m_sDimExt.rgbColor = RGB(sDxf_Rgb.bRed, sDxf_Rgb.bGreen, sDxf_Rgb.bBlue);
	if (cpStyle->m_lDIMSE1 != 0)
		cDimStyle.m_sDimExt.wShow ^= DIM_LINE_SHOW_FIRST;
	if (cpStyle->m_lDIMSE2 != 0)
		cDimStyle.m_sDimExt.wShow ^= DIM_LINE_SHOW_SECOND;
	cDimStyle.m_sDimLine.fBase = (CAD_FLOAT)(cpStyle->m_dDIMDLI * cpStyle->m_dDIMSCALE);
	cDimStyle.m_sDimLine.fExt = (CAD_FLOAT)(cpStyle->m_dDIMDLE * cpStyle->m_dDIMSCALE);
//	cDimStyle.m_sDimLine.nThick = (CAD_FLOAT)cpStyle->m_dDIMLWD; // 2000
	sDxf_Rgb = sDXF_CvtDXFToRGBColor(cpStyle->m_lDIMCLRD);
	cDimStyle.m_sDimLine.rgbColor = RGB(sDxf_Rgb.bRed, sDxf_Rgb.bGreen, sDxf_Rgb.bBlue);
	if (cpStyle->m_lDIMSD1 != 0)
		cDimStyle.m_sDimLine.wShow ^= DIM_LINE_SHOW_FIRST;
	if (cpStyle->m_lDIMSD2 != 0)
		cDimStyle.m_sDimLine.wShow ^= DIM_LINE_SHOW_SECOND;
	cDimStyle.m_sDimText.fGap = (CAD_FLOAT)(cpStyle->m_dDIMGAP * cpStyle->m_dDIMSCALE);
	cDimStyle.m_sDimText.fHeight = (CAD_FLOAT)(cpStyle->m_dDIMTXT * cpStyle->m_dDIMSCALE);
	cDimStyle.m_sDimText.fWidth = (CAD_FLOAT)(cpStyle->m_dDIMTXT * cpStyle->m_dDIMTFAC * cpStyle->m_dDIMSCALE);
	cDimStyle.m_sDimText.nStyle = (short)cpStyle->m_lDIMTXSTY;
	sDxf_Rgb = sDXF_CvtDXFToRGBColor(cpStyle->m_lDIMCLRT);
	cDimStyle.m_sDimText.rgbColor = RGB(sDxf_Rgb.bRed, sDxf_Rgb.bGreen, sDxf_Rgb.bBlue);
//	cDimStyle.m_sDimText.sState.align = (CAD_FLOAT)cpStyle-
//	cDimStyle.m_sDimText.sState.always = (CAD_FLOAT)cpStyle-
//	cDimStyle.m_sDimText.sState.arrow = (CAD_FLOAT)cpStyle-
//	cDimStyle.m_sDimText.sState.fit = (CAD_FLOAT)cpStyle-
//	cDimStyle.m_sDimText.sState.horz = (CAD_FLOAT)cpStyle-
//	cDimStyle.m_sDimText.sState.manual = (CAD_FLOAT)cpStyle-
//	cDimStyle.m_sDimText.sState.place = (CAD_FLOAT)cpStyle-
//	cDimStyle.m_sDimText.sState.rect = (CAD_FLOAT)cpStyle-
//	cDimStyle.m_sDimText.sState.vert = (CAD_FLOAT)cpStyle-
	cDimStyle.m_sDimToler.fLower = (CAD_FLOAT)cpStyle->m_dDIMTM;
	cDimStyle.m_sDimToler.fScale = (CAD_FLOAT)cpStyle->m_dDIMTFAC;
	cDimStyle.m_sDimToler.fUpper = (CAD_FLOAT)cpStyle->m_dDIMTP;
	cDimStyle.m_sDimToler.nUnit = (short)cpStyle->m_lDIMTDEC;
//	cDimStyle.m_sDimToler.sState.format = (CAD_FLOAT)cpStyle-
//	cDimStyle.m_sDimToler.sState.head = (CAD_FLOAT)cpStyle-
//	cDimStyle.m_sDimToler.sState.reserved = (CAD_FLOAT)cpStyle-
//	cDimStyle.m_sDimToler.sState.tail = (CAD_FLOAT)cpStyle-
//	cDimStyle.m_sDimToler.sState.vert = (CAD_FLOAT)cpStyle-
	cDimStyle.m_sDimUnit.fRound = (CAD_FLOAT)cpStyle->m_dDIMRND;
	cDimStyle.m_sDimUnit.fScale = (CAD_FLOAT)cpStyle->m_dDIMLFAC;
//	cDimStyle.m_sDimUnit.nAngleUnit = (CAD_FLOAT)cpStyle-
//	cDimStyle.m_sDimUnit.nLengthUnit = (CAD_FLOAT)cpStyle-
	cDimStyle.m_sDimUnit.nLengthUnit = 0;
//	if (cpStyle->m_lDIMAZIN & 4)
//		cDimStyle.m_sDimUnit.sState.anglehead = 1;
//	if (cpStyle->m_lDIMAZIN & 8)
//		cDimStyle.m_sDimUnit.sState.angletail = 1;
//	cDimStyle.m_sDimUnit.sState.decimal = (CAD_FLOAT)cpStyle->m_lDIMDSEP; // 2000
	if (cpStyle->m_lDIMZIN & 4)
		cDimStyle.m_sDimUnit.sState.lengthhead = 1;
	if (cpStyle->m_lDIMZIN & 8)
		cDimStyle.m_sDimUnit.sState.lengthtail = 1;
//	cDimStyle.m_sDimUnit.sState.scalelayout = (CAD_FLOAT)cpStyle-
	sprintf(cDimStyle.m_sDimUnit.szPrefix, "%s", cpStyle->m_pszDIMAPOST);
	sprintf(cDimStyle.m_sDimUnit.szSuffix, "%s", cpStyle->m_pszDIMAPOST);
	pDoc->m_pDimStyleMgr->Add(&cDimStyle);
}

void ImportDxfUCS(CCAD_Doc *pDoc, CDXF_UCSTable *cpUCS)
{
}

void ImportDxfVPort(CCAD_Doc *pDoc, CDXF_VPortTable *cpVPort)
{
}

void ImportDxfView(CCAD_Doc *pDoc, CDXF_ViewTable *cpView)
{
}

BOOL ImportDxfTableSection(CDXF_MemFile * pDxfMemFile, CCAD_Doc *pDoc)
{
	long	lCount, i;

	lCount = pDxfMemFile->DXFGetLTypeCnt();
	for (i=0; i<lCount; i++){
		CDXF_LTypeTable *cpLType = NULL;
		cpLType = pDxfMemFile->DXFGetLType(i);
		ImportDxfLType(pDoc, cpLType);
	}

	lCount = pDxfMemFile->DXFGetLayerCnt();
	for (i=0; i<lCount; i++){
		CDXF_LayerTable *cpLayer = NULL;
		cpLayer = pDxfMemFile->DXFGetLayer(i);
		ImportDxfLayer(pDoc, cpLayer);
	}

	lCount =  pDxfMemFile->DXFGetStyleCnt();
	for (i=0; i<lCount; i++){
		CDXF_StyleTable *cpStyle = NULL;
		cpStyle = pDxfMemFile->DXFGetStyle(i);
		ImportDxfStyle(pDoc, cpStyle);
	}
	
	lCount = pDxfMemFile->DXFGetDimStyleCnt();
	for (i=0; i<lCount; i++){
		CDXF_DimStyleTable *cpDimStyle = NULL;
		cpDimStyle = pDxfMemFile->DXFGetDimStyle(i);
		ImportDxfDimStyle(pDoc, cpDimStyle);
	}

	lCount = pDxfMemFile->DXFGetUCSCnt();
	for (i=0; i<lCount; i++){
		CDXF_UCSTable *cpUCS = NULL;
		cpUCS = pDxfMemFile->DXFGetUCS(i);
		ImportDxfUCS(pDoc, cpUCS);
	}

	lCount = pDxfMemFile->DXFGetVPortCnt();
	for (i=0; i<lCount; i++){
		CDXF_VPortTable *cpVPort = NULL;
		cpVPort = pDxfMemFile->DXFGetVPort(i);
		ImportDxfVPort(pDoc, cpVPort);
	}

	lCount = pDxfMemFile->DXFGetViewCnt();
	for (i=0; i<lCount; i++){
		CDXF_ViewTable *cpView = NULL;
		cpView = pDxfMemFile->DXFGetView(i);
		ImportDxfView(pDoc, cpView);
	}
	return TRUE;
}
