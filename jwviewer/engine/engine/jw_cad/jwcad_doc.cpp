#include "jwcad_doc.h"
#include "CAD_Doc.h"
#include "CAD_MgrEntity.h"
#include "CAD_GemLine.h"
#include "CAD_GemPoint.h"
#include "CAD_GemCircle.h"
#include "CAD_GemEllipse.h"
#include "CAD_GemArc.h"
#include "CAD_GemEllipseArc.h"
#include "CAD_GemText.h"
#include "CAD_TextStyleMgr.h"
#include "CAD_GemImage.h"
#include "CAD_ImageMgr.h"
#include "CAD_Image.h"
#include "CAD_GemBlock.h"
#include "CAD_PaintEntity.h"
#include "CAD_GemPaint.h"
#include "CAD_Uds.h"
#include "CAD_PaperDoc.h"
#include "CAD_LayerMgr.h"


int g_ClassCnt = 0;
CCAD_PtrArray g_ClassInfoList;


CJWCAD_Doc::CJWCAD_Doc()
{
    m_pDoc = NULL;
	memset(m_strMemo, 0, MAX_STR);
}

CJWCAD_Doc::~CJWCAD_Doc()
{

}

void CJWCAD_Doc::Read(CCAD_File & cFile)
{
	DWORD dummy_DWORD;
	double dummy_double;


	char szTemp[8];
	cFile.ReadString(szTemp, 8);

    
	cFile.ReadDWord(&m_nFileVersion);	// file version
	cFile.m_nJWVersion = m_nFileVersion;
	if (m_nFileVersion > 700)
		return;

    
	BYTE nLength = 0;
	cFile.ReadByte(&nLength);
	cFile.ReadString(m_strMemo, nLength);	// memo

    
	cFile.ReadDWord(&m_nZumen);	// size of plan

    
	cFile.ReadDWord(&m_nWriteGLay);
	for (int i = 0; i < LAYER_COUNT; i++) {
		m_GLayerList[i].ReadParameter(&cFile);
	}

    
	// Dummy
	for (int i = 0; i < 14; i++) {
		cFile.ReadDWord(&dummy_DWORD);
	}

    
	cFile.ReadDWord(&m_lnSunpou1);
	cFile.ReadDWord(&m_lnSunpou2);
	cFile.ReadDWord(&m_lnSunpou3);
	cFile.ReadDWord(&m_lnSunpou4);
	cFile.ReadDWord(&m_lnSunpou5);

    
	// Dummy
	cFile.ReadDWord(&dummy_DWORD);

    
	cFile.ReadDWord(&m_nMaxDrawWid);

    
	m_DPPrtGenten.Read(&cFile);
	cFile.ReadDouble(&m_dPrtBairitsu);
	cFile.ReadDWord(&m_nPrtSet);

    
	cFile.ReadDWord(&m_nMemoriMode);
	cFile.ReadDouble(&m_dMemoriHyoujiMin);
	cFile.ReadDouble(&m_dMemoriX);
	cFile.ReadDouble(&m_dMemoriY);
	m_DpMemoriKijunTen.Read(&cFile);

    
	// Layer
	for (int i = 0; i < LAYER_COUNT; i++) {
		for (int j = 0; j < LAYER_COUNT; j++) {
			m_GLayerList[i].m_LayerList[j].ReadName(&cFile);
		}
	}
	for (int i = 0; i < LAYER_COUNT; i++) {
		m_GLayerList[i].ReadName(&cFile);
	}

    
	cFile.ReadDouble(&m_dKageLevel);
	cFile.ReadDouble(&m_dKageIdo);
	cFile.ReadDWord(&m_nKage9_15JiFlg);
	cFile.ReadDouble(&m_dKabeKageLevel);

    
	cFile.ReadDouble(&m_dTenkuuZuLevel);
	cFile.ReadDouble(&m_dTenkuuZuEnkoR);
	m_dTenkuuZuEnkoR /= 2;

    
	cFile.ReadDWord(&m_nMMTani3D);

    
	cFile.ReadDouble(&m_dBairitsu);
	m_DPGenten.Read(&cFile);

    
	cFile.ReadDouble(&m_dHanniBairitsu);
	m_DPHanniGenten.Read(&cFile);

    
	if (cFile.m_nJWVersion >= 300) {
		for (int i = 0; i < MARK_JUMP_COUNT; i++) {
			m_MarkJumpList[i].Read(&cFile);
		}
	} else {
		for (int i = 0; i < 4; i++) {
			m_MarkJumpList[i].Read(&cFile);
		}
	}

    
	if (cFile.m_nJWVersion >= 300) {
		cFile.ReadDouble(&dummy_double);
		cFile.ReadDouble(&dummy_double);
		cFile.ReadDouble(&dummy_double);
		cFile.ReadDWord(&dummy_DWORD);
		cFile.ReadDouble(&dummy_double);
		cFile.ReadDouble(&dummy_double);
		cFile.ReadDouble(&m_dMojiBG);
		cFile.ReadDWord(&m_nMojiBG);
	}

    
	for (int i = 0; i < DBL_LINE_DIST_COUNT; i++) {
		cFile.ReadDouble(&m_adFukusenSuuchi[i]);
	}

    
	cFile.ReadDouble(&m_dRyoygawaFukusenTomeDe);

    
	for (int i = 0; i < PALETTE_COUNT; i++) {
		m_PaletteList[i].ReadDispParam(&cFile);
	}
	for (int i = 0; i < PALETTE_COUNT; i++) {
		m_PaletteList[i].ReadPrintParam(&cFile);
	}
    

	for (int i = 1; i < LINE_TYPE_COUNT; i++) {
		m_LineTypeList[i].Read(&cFile);
	}


	for (int i = 0; i < RAND_LINE_TYPE_COUNT; i++) {
		m_RandomLineTypeList[i].Read(&cFile);
	}


	for (int i = 0; i < DBL_LINE_TYPE_COUNT; i++) {
		m_DoubleLineTypeList[i].Read(&cFile);
	}


	cFile.ReadDWord(&m_nDrawGamenTen);
	cFile.ReadDWord(&m_nDrawPrtTen);


	cFile.ReadDWord(&m_nBitMapFirstDraw);


	cFile.ReadDWord(&m_nGyakuDraw);


	cFile.ReadDWord(&m_nGyakuSearch);


	cFile.ReadDWord(&m_nColorPrint);


	cFile.ReadDWord(&m_nLayJunPrint);


	cFile.ReadDWord(&m_nColJunPrint);


	cFile.ReadDWord(&m_nPrtRenzoku);


	cFile.ReadDWord(&m_nPrtKyoutsuuGray);


	cFile.ReadDWord(&m_nPrtDispOnlyNonDraw);


	cFile.ReadDWord(&m_lnDrawTime);


	cFile.ReadDWord(&nEyeInit);


	cFile.ReadDWord(&m_dEye_H_Ichi_1);
	cFile.ReadDWord(&m_dEye_H_Ichi_2);
	cFile.ReadDWord(&m_dEye_H_Ichi_3);


	cFile.ReadDouble(&m_dEye_Z_Ichi_1);
	cFile.ReadDouble(&m_dEye_Y_Ichi_1);


	cFile.ReadDouble(&m_dEye_Z_Ichi_2);
	cFile.ReadDouble(&m_dEye_Y_Ichi_2);


	cFile.ReadDouble(&m_dEye_V_Ichi_3);


	cFile.ReadDouble(&m_dSenNagasaSnpou);


	cFile.ReadDouble(&m_dBoxSunpouX);
	cFile.ReadDouble(&m_dBoxSunpouY);


	cFile.ReadDouble(&m_dEnHankeiSnpou);


	cFile.ReadDWord(&m_nSolidNinniColor); 
	cFile.ReadDWord(&m_SolidColor);   //RGB


	for (int i = 0; i < SXF_LINE_COLOR_COUNT; i++) {
		m_SXFLineColorList[i].ReadDispParam(&cFile);
	}
	for (int i = 0; i < SXF_LINE_COLOR_COUNT; i++) {
		m_SXFLineColorList[i].ReadPrintParam(&cFile);
	}


	for (int i = 0; i < SXF_LINE_TYPE_COUNT; i++) {
		m_SXFLineTypeList[i].ReadPattern(&cFile);
	}
	for (int i = 0; i < SXF_LINE_TYPE_COUNT; i++) {
		m_SXFLineTypeList[i].ReadParameter(&cFile);
	}


	for (int i = 0; i < TEXT_TYPE_COUNT; i++) {
		m_TextTypeList[i].Read(&cFile);
	}


	cFile.ReadDouble(&m_dMojiSizeX);
	cFile.ReadDouble(&m_dMojiSizeY);
	cFile.ReadDouble(&m_dMojiKankaku);
	cFile.ReadDWord(&m_nMojiColor);
	cFile.ReadDWord(&m_nMojiShu);


	cFile.ReadDouble(&m_dMojiSeiriGyouKan);
	cFile.ReadDouble(&m_dMojiSeiriSuu);


	cFile.ReadDWord(&m_nMojiKijunZureOn);


	for (int i = 0; i < 3; i++) {
		cFile.ReadDouble(&m_adMojiKijunZureX[i]);
	}


	for (int i = 0; i < 3; i++) {
		cFile.ReadDouble(&m_adMojiKijunZureY[i]);
	}

    SetEnvironment();
    ConvertLayer(m_pDoc->m_pLayerMgr);
    
	ClearClassInfoList();

    short nDataCnt1 = 0;
    DWORD nDataCnt2 = 0;
    cFile.ReadShort(&nDataCnt1);
    if (nDataCnt1 < 0)
        cFile.ReadDWord(&nDataCnt2);
    else
        nDataCnt2 = nDataCnt1;

	short nRepeatNo;
	short nVersion;
	short nLetterCnt;

    CCAD_MgrEntity * pMgrEntity = m_pDoc->GetSpaceDoc(0)->GetUds()->m_pMgrEntity;
	for (int i = 0; i < nDataCnt2; i++) {
		cFile.ReadShort(&nRepeatNo);
		nRepeatNo = nRepeatNo & 0x7FFF;

		if (nRepeatNo == 0x7FFF) {
			cFile.ReadShort(&nVersion);
			cFile.ReadShort(&nLetterCnt);
			SClassInfo * pClassInfo = new SClassInfo();
			cFile.ReadString(pClassInfo->szName, nLetterCnt);

            g_ClassCnt++;
            pClassInfo->nOrder = g_ClassInfoList.GetCount() + g_ClassCnt;
            g_ClassInfoList.Add(pClassInfo);
            ReadEntity(&cFile, pMgrEntity, FALSE, pClassInfo->szName);

		} else {
			char * szClassName = GetDataClassName(nRepeatNo);
			if (szClassName != NULL) {
                g_ClassCnt++;
                ReadEntity(&cFile, pMgrEntity, FALSE, szClassName);
			}
		}
	}


	short nBlockStyleCount = 0;
	cFile.ReadShort(&nBlockStyleCount);

	for (int i = 0; i < nBlockStyleCount; i++) {
		cFile.ReadShort(&nRepeatNo);
		nRepeatNo = nRepeatNo & 0x7FFF;

		if (nRepeatNo == 0x7FFF) {
			cFile.ReadShort(&nVersion);
			cFile.ReadShort(&nLetterCnt);
			SClassInfo * pClassInfo = new SClassInfo();
			cFile.ReadString(pClassInfo->szName, nLetterCnt);

            g_ClassCnt++;
            pClassInfo->nOrder = g_ClassInfoList.GetCount() + g_ClassCnt;
            g_ClassInfoList.Add(pClassInfo);
            ReadBlockStyle(&cFile);

		} else {
			char * szClassName = GetDataClassName(nRepeatNo);
			if (szClassName != NULL) {
                g_ClassCnt++;
                ReadBlockStyle(&cFile);
			}
		}
	}


	short nImageCount = 0;
	cFile.ReadShort(&nImageCount);

    WORD nDummy = 0;
    cFile.ReadWord(&nDummy);
	for (int i = 0; i < nImageCount; i++) {
        ReadImageStyle(&cFile);
	}
    
    ResetEntities();
}

void CJWCAD_Doc::SetEnvironment()
{
    CCAD_PaperDoc * pPaperDoc = (CCAD_PaperDoc*)m_pDoc->GetSpaceDoc(0);
    //background color
    if (m_PaletteList[0].m_aPenColor==0x00ffffff)
        pPaperDoc->m_cSheet.m_bBackColor = SHEET_BACK_WHITE;
    else
        pPaperDoc->m_cSheet.m_bBackColor = SHEET_BACK_BLACK;
    //
    CCAD_Size size(1000,1000);
    switch(m_nZumen)
    {
        case 0://A0
            size.m_fW = 1189; size.m_fH = 841;
            break;
        case 1://A1
            size.m_fW = 841; size.m_fH = 594;
            break;
        case 2://A2
            size.m_fW = 594; size.m_fH = 420;
            break;
        case 3://A3
            size.m_fW = 420; size.m_fH = 297;
            break;
        case 4://A4
            size.m_fW = 297; size.m_fH = 210;
            break;
        case 5:
        case 6:
        case 7:
            break;
        case 8://-tempp
            size.m_fW = 1682; size.m_fH = 1189;
            break;
        case 9://2A
            size.m_fW = 2378; size.m_fH = 1682;
            break;
        case 10://3A
            size.m_fW = 3364; size.m_fH = 2378;
            break;
        case 11://4A
            size.m_fW = 5756; size.m_fH = 3364;
            break;
        case 12:
            size.m_fW = 10*1000; size.m_fH = 10*1000;
            break;
        case 13:
            size.m_fW = 50*1000; size.m_fH = 50*1000;
            break;
        case 14:
            size.m_fW = 100*1000; size.m_fH = 100*1000;
            break;
        default:
            size.m_fW = 1189; size.m_fH = 841;
            break;
    }
    pPaperDoc->m_cSheet.m_cSize = size;
    pPaperDoc->m_cSheet.m_fScale = (CAD_FLOAT)1.0;
}

void CJWCAD_Doc::ConvertLayer(CCAD_LayerMgr * pLayerMgr)
{
    for (int i=0; i<LAYER_COUNT; i++) {
        SLayerGroup group = m_GLayerList[i];
        for (int j=0; j<LAYER_COUNT; j++) {
            CCAD_Layer * pLayer = new CCAD_Layer;
            if (pLayer == NULL)
                break;
            SLayer OldLayer = group.m_LayerList[j];
            
            sprintf(pLayer->m_szName, "%s_%s", group.m_aStrGLayName, OldLayer.m_aStrLayName);
            pLayer->m_wState = LAYER_WORKABLE;
            if (group.m_anGLay >= eLayerState_DispOnly && OldLayer.m_aanLay >= eLayerState_DispOnly)
                pLayer->m_wState |= LAYER_ON;
            if (group.m_anGLay >= eLayerState_Editable && OldLayer.m_aanLay >= eLayerState_Editable)
                pLayer->m_wState |= LAYER_EDITABLE;
            pLayerMgr->m_cArray.Add(pLayer);
        }
    }
    pLayerMgr->m_nAct = 0;
}

void ConvertToCadPen(CCAD_Entity *pEntity, const CJWCAD_Doc & inJWDoc, SData *pData)
{
    if (pEntity == NULL) return;
    CCAD_Pen *pen = pEntity->GetCadPen();
    if (pen == NULL) return;
    pen->m_rgbColor = inJWDoc.m_PaletteList[pData->m_nPenColor].m_aPenColor;
    
    pen->m_nStyle = pData->m_nPenStyle - 1;
    
    pen->m_fScale = 1.0;
}

CCAD_Entity * CJWCAD_Doc::ReadEntity(CCAD_File * pFile, CCAD_MgrEntity * pMgrEntity, BOOL isBlock, char * szName)
{
    SData sData;
    sData.Read(pFile);
    
    CCAD_Entity * pEntity = NULL;
    
    try {
        if (strcmp(szName, "CDataSen") == 0) {
            pEntity = ReadLine(pFile, pMgrEntity, isBlock);
            
        } else if (strcmp(szName, "CDataTen") == 0) {
            pEntity = ReadPoint(pFile, pMgrEntity, isBlock, sData.m_nPenStyle);
            
        } else if (strcmp(szName, "CDataEnko") == 0) {
            pEntity = ReadCircle(pFile, pMgrEntity, isBlock);
            
        } else if (strcmp(szName, "CDataMoji") == 0) {
            pEntity = ReadText(pFile, pMgrEntity, isBlock);
            
        } else if (strcmp(szName, "CDataBlock") == 0) {
            pEntity = ReadBlock(pFile, pMgrEntity, isBlock);
            
        } else if (strcmp(szName, "CDataSunpou") == 0) {
            pEntity = ReadDimension(pFile, pMgrEntity, isBlock, sData.m_nPenStyle);
            
        } else if (strcmp(szName, "CDataSolid") == 0) {
            pEntity = ReadSolid(pFile, pMgrEntity, isBlock, sData.m_nPenColor);
            
        }
    }
    catch(...)
    {
    }
    
    // JW Common Properties
    ConvertToCadPen(pEntity, *this, &sData);
    pEntity->m_nLayer = sData.m_nGLayer * LAYER_COUNT + sData.m_nLayer;
    
    // Coordinate adjust
    if (!isBlock && pEntity && pEntity->GetGemEntity()) {
        CCAD_Vector *org = &pEntity->GetGemEntity()->m_cBcs.m_cOrg;
        CCAD_PaperDoc *paperDoc = (CCAD_PaperDoc *)m_pDoc->GetSpaceDoc(0);
        org->m_fX += paperDoc->m_cSheet.m_cSize.m_fW/2;
        org->m_fY += paperDoc->m_cSheet.m_cSize.m_fH/2;
    }
    //----------------
    
    if (isBlock) {
        pEntity->m_pDoc = m_pDoc;
        pEntity->m_nLayer = DISABLE_INDEX;
    }
    
    if (pEntity != NULL)
        pMgrEntity->AddEntity(pEntity);
    
    return pEntity;
}

CCAD_Entity * CJWCAD_Doc::ReadLine(CCAD_File * pFile, CCAD_MgrEntity * pMgrEntity, BOOL isBlock)
{
    SDataSen sDataSen;
    sDataSen.Read(pFile);
    
    CCAD_Entity * pEntity = pMgrEntity->NewEntity(C_LINE, isBlock);
    CCAD_GemLine *pGemLine = (CCAD_GemLine *)pEntity->GetGemEntity();
    pGemLine->m_cSp.Init(sDataSen.m_sStartPt.X, sDataSen.m_sStartPt.Y);
    pGemLine->m_cEp.Init(sDataSen.m_sEndPt.X, sDataSen.m_sEndPt.Y);
    
    return pEntity;
}

CCAD_Entity * CJWCAD_Doc::ReadPoint(CCAD_File * pFile, CCAD_MgrEntity * pMgrEntity, BOOL isBlock, BYTE nPenStyle)
{
    SDataTen sDataTen;
    sDataTen.Read(pFile, nPenStyle);
    
    CCAD_Entity * pEntity = pMgrEntity->NewEntity(C_POINT, isBlock);
    CCAD_GemPoint *pGemPoint = (CCAD_GemPoint *)pEntity->GetGemEntity();
    pGemPoint->m_cPt.Init(sDataTen.m_Point.X, sDataTen.m_Point.Y);
    if (sDataTen.m_bKariten == 1) {
        pGemPoint->m_bType = 5;
    } else {
        pGemPoint->m_bType = 0;
    }
    
    //    CCAD_PaperDoc * pPaperDoc = (CCAD_PaperDoc *)m_pDoc->GetSpaceDoc(0);
    //    pGemPoint->m_fR = 10 * (pPaperDoc->m_cSheet.m_cSize.m_fW / 594);
    pGemPoint->m_fR = 1;
    
    return pEntity;
}

CCAD_Entity * CJWCAD_Doc::ReadCircle(CCAD_File * pFile, CCAD_MgrEntity * pMgrEntity, BOOL isBlock)
{
    SDataEnko sDataEnko;
    sDataEnko.Read(pFile);
    
    CCAD_Entity * pEntity = NULL;
    if (sDataEnko.m_bZenEnFlg == 1) {
        if (sDataEnko.m_dHenpeiRitsu == 1) {
            pEntity = pMgrEntity->NewEntity(C_CIRCLE, isBlock);
            CCAD_GemCircle *pGemCircle = (CCAD_GemCircle *)pEntity->GetGemEntity();
            pGemCircle->m_cCp.Init(sDataEnko.m_sCenterPt.X, sDataEnko.m_sCenterPt.Y);
            pGemCircle->m_fR = sDataEnko.m_dHankei;
        } else {
            pEntity = pMgrEntity->NewEntity(C_ELLIPSE, isBlock);
            CCAD_GemEllipse *pGemEllipse = (CCAD_GemEllipse *)pEntity->GetGemEntity();
            pGemEllipse->m_cCp.Init(sDataEnko.m_sCenterPt.X, sDataEnko.m_sCenterPt.Y);
            pGemEllipse->m_fRx = sDataEnko.m_dHankei;
            pGemEllipse->m_fRy = sDataEnko.m_dHankei * sDataEnko.m_dHenpeiRitsu;
            pGemEllipse->m_cDirect = (CAD_FLOAT)sDataEnko.m_radKatamukiKaku;
        }
    } else {
        if (sDataEnko.m_dHenpeiRitsu == 1) {
            pEntity = pMgrEntity->NewEntity(C_ARC, isBlock);
            CCAD_GemArc *pGemArc = (CCAD_GemArc *)pEntity->GetGemEntity();
            pGemArc->m_cCp.Init(sDataEnko.m_sCenterPt.X, sDataEnko.m_sCenterPt.Y);
            pGemArc->m_fR = sDataEnko.m_dHankei;
            pGemArc->m_cAlfa = (CAD_FLOAT)sDataEnko.m_radKaishiKaku;
            pGemArc->m_cDelta = (CAD_FLOAT)sDataEnko.m_radEnkoKaku;
        } else {
            pEntity = pMgrEntity->NewEntity(C_ELLPARC, isBlock);
            CCAD_GemEllipseArc *pGemEllipseArc = (CCAD_GemEllipseArc *)pEntity->GetGemEntity();
            pGemEllipseArc->m_cCp.Init(sDataEnko.m_sCenterPt.X, sDataEnko.m_sCenterPt.Y);
            pGemEllipseArc->m_fRx = sDataEnko.m_dHankei;
            pGemEllipseArc->m_fRy = sDataEnko.m_dHankei * sDataEnko.m_dHenpeiRitsu;
            pGemEllipseArc->m_cDirect = (CAD_FLOAT)sDataEnko.m_radKatamukiKaku;
            pGemEllipseArc->m_cAlfa = (CAD_FLOAT)sDataEnko.m_radKaishiKaku;
            pGemEllipseArc->m_cDelta = (CAD_FLOAT)sDataEnko.m_radEnkoKaku;
        }
    }
    
    return pEntity;
}

CCAD_Entity * CJWCAD_Doc::ReadText(CCAD_File * pFile, CCAD_MgrEntity * pMgrEntity, BOOL isBlock)
{
    SDataMoji sDataMoji;
    sDataMoji.Read(pFile);
    
    CCAD_Entity * pEntity = NULL;
    if ( strstr(sDataMoji.m_szString, "^@BM") == NULL ) {	// Text data
        pEntity = pMgrEntity->NewEntity(C_TEXT, isBlock);
        CCAD_GemText *pGemText = (CCAD_GemText *)pEntity->GetGemEntity();
        pGemText->m_bPaint = YES;
        CCAD_Angle directAngle;
        directAngle.SetFromXY((sDataMoji.m_sEndPt.X - sDataMoji.m_sStartPt.X), (sDataMoji.m_sEndPt.Y - sDataMoji.m_sStartPt.Y));
        pGemText->m_cDirect = directAngle;
        pGemText->m_cOblique = 0;
        pGemText->m_fHeight = sDataMoji.m_sSize.X;
        pGemText->m_fWidth = sDataMoji.m_sSize.Y;
        pGemText->m_nLength = strlen(sDataMoji.m_szString);
        pGemText->m_cPt.Init(sDataMoji.m_sStartPt.X, sDataMoji.m_sStartPt.Y);
        pGemText->m_fEmpty = 0;
        CCAD_TextStyleMgr * pStyleMgr = (CCAD_TextStyleMgr *)m_pDoc->GetStyleMgr(SS_TEXT);
        pGemText->m_nStyle = 0;
        pGemText->m_pTextStyle = (CCAD_TextStyle *) pStyleMgr->GetStyle(0);
        
        pGemText->m_pStr = new char[pGemText->m_nLength];
        ::memcpy( (void*)pGemText->m_pStr, (void*)sDataMoji.m_szString, pGemText->m_nLength);
        
    } else {	// Image data
        pEntity = pMgrEntity->NewEntity(C_IMAGE, isBlock);
        CCAD_GemImage *pGemImage = (CCAD_GemImage *)pEntity->GetGemEntity();
        //CCAD_ImageMgr * pImageMgr = (CCAD_ImageMgr *)m_pDoc->GetStyleMgr(SS_IMAGE);
        //pGemImage->m_nIndex = pImageMgr->GetNameIndex(sDataMoji.m_szBmpFileName);
        //pGemImage->m_pImage = (CCAD_Image *)pImageMgr->GetStyle(pGemImage->m_nIndex);
        //if (pGemImage->m_nIndex < 0)
        //	pGemImage->m_bDraw = DRAW_FRAME;
        //else
        //	pGemImage->m_bDraw = DRAW_PAINT;
        strcpy(pGemImage->m_szName, sDataMoji.m_szBmpFileName);
        
        pGemImage->m_nPtNum = 4;
        pGemImage->m_pPt = new CCAD_Vector[pGemImage->m_nPtNum];
        CCAD_Vector * pVector = &pGemImage->m_pPt[0];
        pVector->Init(sDataMoji.m_sStartPt.X, sDataMoji.m_sStartPt.Y);
        pVector = &pGemImage->m_pPt[1];
        pVector->Init(sDataMoji.m_sStartPt.X, sDataMoji.m_sStartPt.Y + sDataMoji.m_dBmpHeight);
        pVector = &pGemImage->m_pPt[2];
        pVector->Init(sDataMoji.m_sStartPt.X + sDataMoji.m_dBmpWidth, sDataMoji.m_sStartPt.Y + sDataMoji.m_dBmpHeight);
        pVector = &pGemImage->m_pPt[3];
        pVector->Init(sDataMoji.m_sStartPt.X + sDataMoji.m_dBmpWidth, sDataMoji.m_sStartPt.Y);
        
        // Rotate
        pGemImage->m_dRotAngle = - sDataMoji.m_dBmpRotAngle;
        CCAD_Angle rotAngle;
        rotAngle.SetFromDegree(sDataMoji.m_dBmpRotAngle);
        CCAD_Coord3D cBcs;
        cBcs.m_cOrg.Init(sDataMoji.m_sStartPt.X, sDataMoji.m_sStartPt.Y);
        cBcs.ThisToChild(&pGemImage->m_cBcs);
        cBcs.ZRotateChildCoord(rotAngle, &pGemImage->m_cBcs);
        cBcs.ChildToThis(&pGemImage->m_cBcs);
        
        // for Trim
        pGemImage->m_dBmpWidth = sDataMoji.m_dBmpWidth;
        pGemImage->m_dBmpHeight = sDataMoji.m_dBmpHeight;
        pGemImage->m_dBmpTrim1 = sDataMoji.m_dBmpTrim1;
        pGemImage->m_dBmpTrim2 = sDataMoji.m_dBmpTrim2;
        pGemImage->m_dBmpScale = sDataMoji.m_dBmpScale;
    }
    
    return pEntity;
}

CCAD_Entity * CJWCAD_Doc::ReadDimension(CCAD_File * pFile, CCAD_MgrEntity * pMgrEntity, BOOL isBlock, BYTE nPenStyle)
{
    SDataSunpou sDataSunpou;
    sDataSunpou.Read(pFile, nPenStyle);
    
    CCAD_Entity * pEntity = pMgrEntity->NewEntity(C_DIMLINEAR, isBlock);
    
    return pEntity;
}

CCAD_Entity * CJWCAD_Doc::ReadBlock(CCAD_File * pFile, CCAD_MgrEntity * pMgrEntity, BOOL isBlock)
{
    SDataBlock sDataBlock;
    sDataBlock.Read(pFile);
    
    CCAD_Entity * pEntity = pMgrEntity->NewEntity(C_BLOCK, isBlock);
    CCAD_GemBlock *pGemBlock = (CCAD_GemBlock *)pEntity->GetGemEntity();
    //CDataList *pDataList = (CDataList *)m_DataListList.GetAt(sDataBlock.m_nNumber);
    //strcpy(pGemBlock->m_szName, pDataList->m_szName);
    pGemBlock->m_nNumber = sDataBlock.m_nNumber;
    pGemBlock->m_cBcs.m_cOrg.Init(sDataBlock.m_DPKijunTen.X, sDataBlock.m_DPKijunTen.Y);
    pGemBlock->m_cBcs.m_cX.m_fX = (CAD_FLOAT)cos(sDataBlock.m_radKaitenKaku);
    pGemBlock->m_cBcs.m_cX.m_fY = (CAD_FLOAT)sin(sDataBlock.m_radKaitenKaku);
    pGemBlock->m_cBcs.m_cY.m_fX = -pGemBlock->m_cBcs.m_cX.m_fY;
    pGemBlock->m_cBcs.m_cY.m_fY = pGemBlock->m_cBcs.m_cX.m_fX;
    pGemBlock->m_cBcs.m_fScale = sDataBlock.m_dBairitsuX;
    //pGemBlock->m_cBcs.m_fScale = sDataBlock.m_dBairitsuY;
    
    return pEntity;
}

CCAD_Entity * CJWCAD_Doc::ReadSolid(CCAD_File * pFile, CCAD_MgrEntity * pMgrEntity, BOOL isBlock, WORD nPenColor)
{
    SDataSolid sDataSolid;
    sDataSolid.Read(pFile, nPenColor);
    
    CCAD_Entity * pEntity = pMgrEntity->NewEntity(C_PAINT, isBlock);
    CCAD_GemPaint *pGemPaint = (CCAD_GemPaint *)pEntity->GetGemEntity();
    
    ((CCAD_PaintEntity*)pEntity)->m_cBrush.m_rgbColor = m_PaletteList[nPenColor].m_aPenColor;
    
    pGemPaint->m_bDraw = DRAW_PAINT;
    pGemPaint->m_nTotGrp = 1;
    pGemPaint->m_nTotPt = 4;
    pGemPaint->m_pNum = new short[1];
    pGemPaint->m_pPt = new CCAD_Vector[pGemPaint->m_nTotPt];
    *pGemPaint->m_pNum = pGemPaint->m_nTotPt;
    pGemPaint->m_pPt = new CCAD_Point[*pGemPaint->m_pNum];
    pGemPaint->m_pPt[0] = CCAD_Vector(sDataSolid.m_sStartPt.X, sDataSolid.m_sStartPt.Y, (CAD_FLOAT) 0.0);
    pGemPaint->m_pPt[1] = CCAD_Vector(sDataSolid.m_sEndPt.X, sDataSolid.m_sEndPt.Y, (CAD_FLOAT) 0.0);
    pGemPaint->m_pPt[2] = CCAD_Vector(sDataSolid.m_sPoint2.X, sDataSolid.m_sPoint2.Y, (CAD_FLOAT) 0.0);
    pGemPaint->m_pPt[3] = CCAD_Vector(sDataSolid.m_sPoint3.X, sDataSolid.m_sPoint3.Y, (CAD_FLOAT) 0.0);
    
    return pEntity;
}

CCAD_Block * CJWCAD_Doc::ReadBlockStyle(CCAD_File * pFile)
{
    SData sData;
    sData.Read(pFile);
    SDataList sDataList;
    sDataList.Read(pFile);
    
    CCAD_Block * pBlock = new CCAD_Block(m_pDoc->m_hHeap, m_pDoc);
    if (pBlock == NULL)
        return NULL;
    pBlock->m_cMgrEntity.m_pDoc = m_pDoc;
    
    // 		pBlock->m_cRc.m_fMinX = (CAD_FLOAT)pOldBlock->m_pObj->cRc.m_dLeft;
    // 		pBlock->m_cRc.m_fMaxX = (CAD_FLOAT)pOldBlock->m_pObj->cRc.m_dRight;
    // 		pBlock->m_cRc.m_fMinY = (CAD_FLOAT)pOldBlock->m_pObj->cRc.m_dTop;
    // 		pBlock->m_cRc.m_fMaxY = (CAD_FLOAT)pOldBlock->m_pObj->cRc.m_dBottom;
    pBlock->m_cRc.m_fMaxZ = pBlock->m_cRc.m_fMinZ = 0;
    // 		sprintf(pBlock->m_szDescript, "%s", pOldBlock->m_pObj->szDescript);
    sprintf(pBlock->m_szName, "%s", sDataList.m_szName);
    //		sprintf(pBlock->m_szPart, "%s", pOldBlock->m_pObj->szPart);
    
    pBlock->m_lSize = 0;
    long	lSize = 0;
    
    // Included entities
    short nDataCnt = 0;
    pFile->ReadShort(&nDataCnt);
    
    short nRepeatNo;	// FFFF : 새이름, (nRepeatNo - 1) / 2
    short nVersion;	// 버젼(무시)
    short nLetterCnt;
    
    CCAD_Entity * pEntity = NULL;
    for (int i = 0; i < nDataCnt; i++) {
        pFile->ReadShort(&nRepeatNo);
        nRepeatNo = nRepeatNo & 0x7FFF;
        
        pEntity = NULL;
        if (nRepeatNo == 0x7FFF) {
            pFile->ReadShort(&nVersion);
            pFile->ReadShort(&nLetterCnt);
            SClassInfo * pClassInfo = new SClassInfo();
            pFile->ReadString(pClassInfo->szName, nLetterCnt);
            
            g_ClassCnt++;
            pClassInfo->nOrder = g_ClassInfoList.GetCount() + g_ClassCnt;
            g_ClassInfoList.Add(pClassInfo);
            pEntity = ReadEntity(pFile, &(pBlock->m_cMgrEntity), TRUE, pClassInfo->szName);
            
        } else {
            char * szClassName = GetDataClassName(nRepeatNo);
            if (szClassName != NULL) {
                g_ClassCnt++;
                pEntity = ReadEntity(pFile, &(pBlock->m_cMgrEntity), TRUE, szClassName);
            }
        }
        
        if (pEntity != NULL)
            pBlock->m_lSize += pEntity->GetSize();
    }
    
    if (pBlock != NULL)
        m_pDoc->m_pMgrBlock->Add(pBlock);
    
    return pBlock;
}

CCAD_Image * CJWCAD_Doc::ReadImageStyle(CCAD_File * pFile)
{
    char	aStrTempGazouName[MAX_STR];
    DWORD	nSize;
    BYTE *	buf;
    
    memset(aStrTempGazouName, 0, MAX_STR);
    nSize = 0;
    buf = NULL;
    
    BYTE nLength = 0;
    pFile->ReadByte(&nLength);
    pFile->ReadString(aStrTempGazouName, nLength);
    pFile->ReadDWord(&nSize);
    if (nSize > 0) {
        char * pFind = strstr(aStrTempGazouName, ".gz");
        if ( pFind == NULL ) {
            buf = (BYTE *)malloc(nSize);
            memset(buf, 0, nSize);
            pFile->ReadBlock(buf, nSize);
        } else {
            *pFind = '\0';
            BYTE * readBuf = (BYTE *)malloc(nSize);
            memset(readBuf, 0, nSize);
            pFile->ReadBlock(readBuf, nSize);
            buf = gz_decompress_buf(readBuf, nSize);
            free(readBuf);
        }
    }
    
    CCAD_Image * pImage = NULL;
    if (buf != NULL) {
        pImage = new CCAD_Image(m_pDoc->m_hHeap);
        pImage->SetBmpData(buf);
        pImage->m_bImageMgr = YES;
        
        memcpy(pImage->m_szName, aStrTempGazouName, MAX_STR);
        
        m_pDoc->m_pImageMgr->m_cArray.Add(pImage);
        free(buf);
    }
    
    return pImage;
}

void CJWCAD_Doc::ResetEntities()
{
    CCAD_MgrEntity * pMgrEntity = m_pDoc->GetSpaceDoc(0)->GetUds()->m_pMgrEntity;
    long pos;
    CCAD_Entity	* pEntity = pMgrEntity->GetFirstEntity(pos);
    
    while (pEntity != NULL) {
        if (pEntity->m_bCode == C_BLOCK) {
            CCAD_GemBlock * pGemBlock = (CCAD_GemBlock *)pEntity->GetGemEntity();
            CCAD_Block * pBlock = (CCAD_Block *)m_pDoc->m_pMgrBlock->GetStyle(pGemBlock->m_nNumber);
            if (pBlock != NULL)
                strcpy(pGemBlock->m_szName, pBlock->m_szName);
            
        } else if (pEntity->m_bCode == C_IMAGE) {
            CCAD_GemImage * pGemImage = (CCAD_GemImage *)pEntity->GetGemEntity();
            CCAD_ImageMgr * pImageMgr = (CCAD_ImageMgr *)m_pDoc->GetStyleMgr(SS_IMAGE);
            pGemImage->m_nIndex = pImageMgr->GetNameIndex(pGemImage->m_szName);
            pGemImage->m_pImage = (CCAD_Image *)pImageMgr->GetStyle(pGemImage->m_nIndex);
            if (pGemImage->m_nIndex < 0)
                pGemImage->m_bDraw = DRAW_FRAME;
            else
                pGemImage->m_bDraw = DRAW_PAINT;
            
            // Trim
            if (pGemImage->m_dBmpScale < 1) {
                pGemImage->m_bIsTrim = TRUE;
                pGemImage->m_dTrimLeft = pGemImage->m_dBmpTrim1;
                pGemImage->m_dTrimRight = pGemImage->m_dTrimLeft + pGemImage->m_dBmpScale;
                if (pGemImage->m_pImage != NULL) {
                    double dOrgBmpWidth = pGemImage->m_dBmpWidth / pGemImage->m_dBmpScale;
                    double dOrgBmpHeight = dOrgBmpWidth * ((double)pGemImage->m_pImage->m_sImage.iHeight / (double)pGemImage->m_pImage->m_sImage.iWidth);
                    if (dOrgBmpHeight != 0) {
                        pGemImage->m_dTrimBottom = pGemImage->m_dBmpTrim2 * (dOrgBmpWidth / dOrgBmpHeight);
                        pGemImage->m_dTrimTop = pGemImage->m_dTrimBottom + pGemImage->m_dBmpHeight / dOrgBmpHeight;
                    }
                }
            }
        }
        
        pEntity = pMgrEntity->GetNextEntity(pos);
    }
}


#include <stdio.h>
#include "zlib.h"
BYTE * CJWCAD_Doc::gz_decompress_buf(BYTE * buf, DWORD nSize)
{
    z_stream infstream;
    
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    
    infstream.avail_in = nSize; // size of input
    infstream.next_in = (Bytef *)buf; // input char array
    
    DWORD nOutSize = 0;
    BYTE * outbuf = NULL;
    
    DWORD bufferLength = 128 * 1024;
    BYTE * buffer = (BYTE *)malloc(bufferLength);
    
    int result = inflateInit2(&infstream, (15 + 16));
    if (result != Z_OK) {
        free(buffer);
        return NULL;
    }
    
    while(TRUE) {
        infstream.avail_out = bufferLength; // size of output
        infstream.next_out = (Bytef *)buffer; // output char array
        
        result = inflate(&infstream, Z_SYNC_FLUSH);
        
        if (result != Z_OK && result != Z_STREAM_END) {
            inflateEnd(&infstream);
            free(buffer);
            return NULL;
        }
        
        int nAddSize = bufferLength - infstream.avail_out;
        int nTotalSize = nOutSize + nAddSize;
        if (nTotalSize > 0) {
            if (outbuf == NULL)
                outbuf = (BYTE *)malloc(nTotalSize);
            else
                outbuf = (BYTE *)realloc(outbuf, nTotalSize);
        }
        memcpy(outbuf + nOutSize, buffer, nAddSize);
        nOutSize = nTotalSize;
        
        if (result == Z_STREAM_END) {
            break;
        }
    }
    
    //FILE *fp = fopen("c:\\1.bmp","wb");
    //fwrite(outbuf, infstream.total_out,1,fp);
    //fclose(fp);
    
    inflateEnd(&infstream);
    free(buffer);
    return outbuf;
}
