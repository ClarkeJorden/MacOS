#pragma once

#include "CAD_File.h"
#include "jwcad_layer.h"
#include "CAD_PtrArray.h"

class	CCAD_Doc;
class	CCAD_MgrEntity;
class	CCAD_Entity;
class	CCAD_Block;
class	CCAD_Image;
class	CCAD_LayerMgr;


extern int g_ClassCnt;
extern CCAD_PtrArray g_ClassInfoList;
struct SClassInfo {
    char szName[MAX_STR];
    short nOrder;
    
    SClassInfo() {
        memset(szName, 0, MAX_STR);
        nOrder = 0;
    }
};

inline char* GetDataClassName(short nOrder)
{
    for (int i = 0; i < g_ClassInfoList.GetCount(); i++) {
        SClassInfo * pClassInfo = (SClassInfo *)g_ClassInfoList.GetAt(i);
        if (pClassInfo->nOrder == nOrder)
            return pClassInfo->szName;
    }
    
    return NULL;
};

inline void ClearClassInfoList()
{
    for (int i = 0; i < g_ClassInfoList.GetCount(); i++) {
        SClassInfo * pClassInfo = (SClassInfo *) g_ClassInfoList.GetAt(i);
        if (pClassInfo != NULL)
            delete pClassInfo;
    }
    g_ClassInfoList.RemoveAll();
    g_ClassCnt = 0;
}


/****************************************************

******************************************************/
class CJWCAD_Doc {
public:
	CJWCAD_Doc();
	virtual ~CJWCAD_Doc();
    
public:
    CCAD_Doc * m_pDoc;

public:

	DWORD			m_nFileVersion;


	char			m_strMemo[MAX_STR];


	DWORD			m_nZumen;


	DWORD			m_nWriteGLay;
	SLayerGroup		m_GLayerList[LAYER_COUNT];


	DWORD			m_lnSunpou1;
	DWORD			m_lnSunpou2;
	DWORD			m_lnSunpou3;
	DWORD			m_lnSunpou4;
	DWORD			m_lnSunpou5;


	DWORD			m_nMaxDrawWid;
	DWORD			m_nMaxDrawWidBak;
	DWORD			m_bSenHaba100;


	S_Point			m_DPPrtGenten;
	double			m_dPrtBairitsu;
	DWORD			m_nPrtSet;


	DWORD			m_nMemoriMode;
	double			m_dMemoriHyoujiMin;
	double			m_dMemoriX;
	double			m_dMemoriY;
	S_Point			m_DpMemoriKijunTen;


	double			m_dKageLevel;
	double			m_dKageIdo;
	DWORD			m_nKage9_15JiFlg;
	double			m_dKabeKageLevel;


	double			m_dTenkuuZuLevel;
	double			m_dTenkuuZuEnkoR;


	DWORD			m_nMMTani3D;


	double			m_dBairitsu;
	S_Point			m_DPGenten;


	double			m_dHanniBairitsu;
	S_Point			m_DPHanniGenten;


	SMarkJump		m_MarkJumpList[MARK_JUMP_COUNT];


	double			m_dMojiBG;
	DWORD			m_nMojiBG;


	double			m_adFukusenSuuchi[DBL_LINE_DIST_COUNT];


	double			m_dRyoygawaFukusenTomeDe;


	SPalette		m_PaletteList[PALETTE_COUNT];


	SLineType		m_LineTypeList[LINE_TYPE_COUNT];


	SRandomLineType	m_RandomLineTypeList[RAND_LINE_TYPE_COUNT];


	SLineType		m_DoubleLineTypeList[DBL_LINE_TYPE_COUNT];


	DWORD			m_nDrawGamenTen;
	DWORD			m_nDrawPrtTen;


	DWORD			m_nBitMapFirstDraw;


	DWORD			m_nGyakuDraw;


	DWORD			m_nGyakuSearch;


	DWORD			m_nColorPrint;


	DWORD			m_nLayJunPrint;


	DWORD			m_nColJunPrint;


	DWORD			m_nPrtRenzoku;


	DWORD			m_nPrtKyoutsuuGray;


	DWORD			m_nPrtDispOnlyNonDraw;


	DWORD			m_lnDrawTime;


	DWORD			nEyeInit;


	DWORD			m_dEye_H_Ichi_1;
	DWORD			m_dEye_H_Ichi_2;
	DWORD			m_dEye_H_Ichi_3;


	double			m_dEye_Z_Ichi_1;
	double			m_dEye_Y_Ichi_1;


	double			m_dEye_Z_Ichi_2;
	double			m_dEye_Y_Ichi_2;


	double			m_dEye_V_Ichi_3;


	double			m_dSenNagasaSnpou;


	double			m_dBoxSunpouX;
	double			m_dBoxSunpouY;


	double			m_dEnHankeiSnpou ;


	DWORD			m_nSolidNinniColor; 
	DWORD			m_SolidColor;   //RGB


	SSXFLineColor	m_SXFLineColorList[SXF_LINE_COLOR_COUNT];


	SSXFLineType	m_SXFLineTypeList[SXF_LINE_TYPE_COUNT];


	STextType		m_TextTypeList[TEXT_TYPE_COUNT];


	double	m_dMojiSizeX;
	double	m_dMojiSizeY;
	double	m_dMojiKankaku;
	DWORD	m_nMojiColor;
	DWORD	m_nMojiShu;


	double	m_dMojiSeiriGyouKan;
	double	m_dMojiSeiriSuu;


	DWORD	m_nMojiKijunZureOn;


	double	m_adMojiKijunZureX[3];


	double	m_adMojiKijunZureY[3];


//	CCAD_PtrArray	m_DataList;
//	CCAD_PtrArray	m_DataListList;
//	CCAD_PtrArray	m_ImageList;

public:
	void		Read(CCAD_File & cFile);
    
    void			SetEnvironment();
    void			ConvertLayer(CCAD_LayerMgr * pLayerMgr);
    
    CCAD_Entity *	ReadEntity(CCAD_File * pFile, CCAD_MgrEntity * pMgrEntity, BOOL isBlock, char * szName);
    CCAD_Entity *	ReadLine(CCAD_File * pFile, CCAD_MgrEntity * pMgrEntity, BOOL isBlock);
    CCAD_Entity *	ReadPoint(CCAD_File * pFile, CCAD_MgrEntity * pMgrEntity, BOOL isBlock, BYTE nPenStyle);
    CCAD_Entity *	ReadCircle(CCAD_File * pFile, CCAD_MgrEntity * pMgrEntity, BOOL isBlock);
    CCAD_Entity *	ReadText(CCAD_File * pFile, CCAD_MgrEntity * pMgrEntity, BOOL isBlock);
    CCAD_Entity *	ReadDimension(CCAD_File * pFile, CCAD_MgrEntity * pMgrEntity, BOOL isBlock, BYTE nPenStyle);
    CCAD_Entity *	ReadBlock(CCAD_File * pFile, CCAD_MgrEntity * pMgrEntity, BOOL isBlock);
    CCAD_Entity *	ReadSolid(CCAD_File * pFile, CCAD_MgrEntity * pMgrEntity, BOOL isBlock, WORD nPenColor);
    
    CCAD_Block *	ReadBlockStyle(CCAD_File * pFile);
    CCAD_Image *	ReadImageStyle(CCAD_File * pFile);
    BYTE *			gz_decompress_buf(BYTE * buf, DWORD nSize);
    void			ResetEntities();
};
