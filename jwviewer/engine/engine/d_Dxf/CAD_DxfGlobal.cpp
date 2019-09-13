#include "stdafx.h"
#include "CAD_DxfGlobal.h"
#include "CAD_PenStyle.h"
#include "CAD_Uds.h"

void CvtDxfCoordToCCAD_Vector(SDXF_Point3D *pDxfPt, CCAD_Vector &cPt)
{
	cPt.m_fX = (CAD_FLOAT)pDxfPt->dX;
	cPt.m_fY = (CAD_FLOAT)pDxfPt->dY;
	cPt.m_fZ = (CAD_FLOAT)pDxfPt->dZ;
}

void CvtDxfBcsToCCAD_Bcs(CDXF_Entity *pDxfEntity, CCAD_Coord3D &cBcs)
{
	CvtDxfCoordToCCAD_Vector(&pDxfEntity->m_sBCS.sOrg, cBcs.m_cOrg);
	CvtDxfCoordToCCAD_Vector(&pDxfEntity->m_sBCS.sXAxis, cBcs.m_cX);
	CvtDxfCoordToCCAD_Vector(&pDxfEntity->m_sBCS.sYAxis, cBcs.m_cY);
	CvtDxfCoordToCCAD_Vector(&pDxfEntity->m_sBCS.sZAxis, cBcs.m_cZ);
}

void CvtDxfEntityHeadToDrw(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity, short &nLayer, CCAD_Pen *pPen, CCAD_Brush *pBrush)
{
	nLayer = pDoc->m_pLayerMgr->GetNameIndex(pDxfEntity->m_pszLayerName);

	if (pPen != NULL){
		CCAD_Layer	*pLayer = (CCAD_Layer *)pDoc->m_pLayerMgr->GetStyle(nLayer);
		if (pDxfEntity->m_lDXFColor == 256 || pDxfEntity->m_lDXFColor == 0)
			pPen->m_rgbColor = pLayer->m_cPen.m_rgbColor;
		else{
			SDXF_RGBColor	sDxf_Rgb = sDXF_CvtDXFToRGBColor(pDxfEntity->m_lDXFColor);
			pPen->m_rgbColor = RGB(sDxf_Rgb.bRed, sDxf_Rgb.bGreen, sDxf_Rgb.bBlue);
		}

		if ((strcmp(pDxfEntity->m_pszLTypeName, "BYLAYER") == 0) || (strcmp(pDxfEntity->m_pszLTypeName, "BYBLOCK") == 0))
			pPen->m_nStyle = pLayer->m_cPen.m_nStyle;
		else
			pPen->m_nStyle = pDoc->m_pPenStyleMgr->GetNameIndex(pDxfEntity->m_pszLTypeName);
		pPen->m_fScale = (CAD_FLOAT)pDxfEntity->m_dLTypeScale;
	//	pPen->m_nThick = pDxfEntity->
	}
	if (pBrush != NULL){
		pBrush->m_rgbColor = pPen->m_rgbColor;
		pBrush->m_nPattern = BRS_SOLID;
	}
}
/*
static DWORD  dwSwap(DWORD value)
{
	WORD  hword, lword;
	BYTE  hbyte, lbyte;
     
	hword = LOWORD(value);	//read low word
	hbyte = LOBYTE(hword);  //read low byte
	lbyte = HIBYTE(hword); 	//read high byte
	hword = MAKEWORD(lbyte, hbyte);	//make high word
	
	lword = HIWORD(value);	//read high word
	hbyte = LOBYTE(lword);  //read low byte
	lbyte = HIBYTE(lword); 	//read high byte
	lword = MAKEWORD(lbyte, hbyte);	//make low word
	
	value = (DWORD)MAKELONG(lword, hword);	//make long value.
	return value;
}

static short wSwap(short value)
{
	BYTE hbyte, lbyte;
 	
 	hbyte = LOBYTE(value);
 	lbyte = HIBYTE(value);
 	value = MAKEWORD(lbyte, hbyte);
 		
 	return value;
}


BOOL SetFaceName(CString &cFullPath, char *pszFaceName)
{
	typedef struct{
					WORD	PlatformID;
					WORD	EncodingID;
					WORD	LanguageID;
					WORD	NameID;
					WORD	StrLength;
					WORD	StrOffset;
	}NameRec;

	NameRec		NameRecord;

	FILE	*spFile;
	spFile  = fopen((LPCTSTR)cFullPath, "rb" );
	if(spFile == 0) {
		return FALSE;
	}
	
	long	nSkip, nValue;
	int		i;

	for(i = 0; i < 3; i ++)
		fread(&nSkip, sizeof(long), 1, spFile);

	fread(&nValue, sizeof(long), 1, spFile);
	long	nTem=0;
	
	strncpy( (char *)&nTem, "name",4);
	while(nValue != nTem) {
		for(i = 0; i < 3; i ++)
			fread(&nSkip, sizeof(long), 1, spFile);
		fread(&nValue, sizeof(long), 1, spFile);
	}
	fread(&nSkip, sizeof(long), 1, spFile);
	
	long	nOffset = 0;
	fread(&nOffset, sizeof(long), 1, spFile);
	long	nLength = 0;
	fread(&nLength, sizeof(long), 1, spFile);

	nOffset = dwSwap( nOffset );
	nLength = dwSwap( nLength );
	fseek(spFile, nOffset, SEEK_SET);

	fseek(spFile, sizeof(short), SEEK_CUR);
	
	short	nRecordName = 0;
	fread(&nRecordName, sizeof(short), 1, spFile);
	short	nStrStartOff = 0;
	fread(&nStrStartOff, sizeof(short), 1, spFile);

	nRecordName = wSwap(nRecordName);
	nStrStartOff = wSwap(nStrStartOff);

	short	strLength = 0;
	char	chFamilyName[256];
	for( i = 0; i < nRecordName; i++ ){
		fread(&NameRecord, 1, sizeof(NameRecord),spFile);
		if((wSwap(NameRecord.PlatformID) == 3) &&( wSwap(NameRecord.LanguageID) ==0x0409)){			// Check Microsoft encoding
			fread(&NameRecord, sizeof(NameRecord), 1, spFile);
			if( wSwap(NameRecord.NameID) == 1 ){
				strLength = wSwap(NameRecord.StrLength);
				short	strOffset;
				strOffset = wSwap(NameRecord.StrOffset);
				fseek( spFile, nOffset+nStrStartOff+strOffset, SEEK_SET );
				fread(chFamilyName, strLength, 1, spFile);
				break;
			}
		}
	}
	fclose(spFile);

	char	chTem;
	int		j = 0;
	for (i = 0; i < strLength; i ++) {
		chTem = chFamilyName[i];
		if(chTem != 0) {
			pszFaceName[j] = chTem;
			j++;
		}
	}
	pszFaceName[j] = 0;
	char	chBold[5];
	strncpy(chBold, &pszFaceName[strlen(pszFaceName)-4], 4);
	if (stricmp(chBold, "BOLD") == 0)
		pszFaceName[strlen(pszFaceName)-4] = 0;
	return TRUE;
}*/
void ConvertDxfName(char* pDxfStyleName, char* pInName)
{
	char pName[MAX_STRING_LEN];
	strcpy(pName, pInName);
	for(UINT i=0; i<strlen(pName); i++)
	{
		if( pName[i]==' ' )
			pName[i] = '_';
	}
	strcpy(pDxfStyleName, pName);
}

short CountArcDivide(CAD_FLOAT fDelta)
{
	short n = (short)(fabs(fDelta) * 36 / PAI + 1);
	return n;
}
