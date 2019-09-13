#pragma once

#include "CAD_File.h"
#include <stdlib.h>


#define		LAYER_COUNT			16
#define		MARK_JUMP_COUNT		8
#define		DBL_LINE_DIST_COUNT	10
#define		PALETTE_COUNT		10
#define		LINE_TYPE_COUNT		9
#define		RAND_LINE_TYPE_COUNT	5
#define		DBL_LINE_TYPE_COUNT		4
#define		SXF_LINE_COLOR_COUNT	257
#define		SXF_LINE_TYPE_COUNT		33
#define		TEXT_TYPE_COUNT			10

/****************************************************
 S_Point
 ******************************************************/
struct S_Point{
    double	X;
    double	Y;
    
    S_Point()
    {
        X = 0;
        Y = 0;
    }
    
    S_Point(double x, double y)	{
        X = x;
        Y = y;
    }
    
    void operator = (const S_Point & other)
    {
        X = other.X;
        Y = other.Y;
    };
    
    void Read(CCAD_File* pStream)
    {
        pStream->ReadDouble(&X);
        pStream->ReadDouble(&Y);
    };
    
};

/****************************************************
 SLayer
 ******************************************************/
enum	LayerState {
	eLayerState_NoDisp		= 0,
	eLayerState_DispOnly,
	eLayerState_Editable,
	eLayerState_Writable
};

enum	LayerProtect {
	eLayerProtect_None		= 0,
	eLayerProtect_Modifiable,
	eLayerProtect_Fixed
};

struct SLayer {
	DWORD	m_aanLay;			// ( 0 : off, 1 : show, 2 : edit, 3 : write )
	DWORD	m_aanLayProtect;
	char	m_aStrLayName[MAX_STR];

	SLayer()
	{
		m_aanLay = eLayerState_NoDisp;
		m_aanLayProtect = eLayerProtect_None;
		memset(m_aStrLayName, 0, MAX_STR);
	}

	void operator = (const SLayer & other)
	{
		m_aanLay = other.m_aanLay;
		m_aanLayProtect = other.m_aanLayProtect;
		memcpy(m_aStrLayName, other.m_aStrLayName, MAX_STR);
	};

	void ReadParameter(CCAD_File* pStream)
	{
		pStream->ReadDWord(&m_aanLay);
		pStream->ReadDWord(&m_aanLayProtect);
	};

	void ReadName(CCAD_File* pStream)
	{
		BYTE nLength = 0;
		pStream->ReadByte(&nLength);
		pStream->ReadString(m_aStrLayName, nLength);
	};
};

/****************************************************
			SLayerGroup
******************************************************/
struct SLayerGroup {
	DWORD	m_anGLay;			//( 0 : off, 1 : show, 2 : edit, 3 : write )
	DWORD	m_anWriteLay;
	double	m_adScale;
	DWORD	m_anGLayProtect;
	SLayer	m_LayerList[LAYER_COUNT];
	char	m_aStrGLayName[MAX_STR];

	SLayerGroup()
	{
		m_anGLay = eLayerState_NoDisp;
		m_anWriteLay = -1;
		m_adScale = 1;
		m_anGLayProtect = eLayerProtect_None;
		// layer list
		memset(m_aStrGLayName, 0, MAX_STR);
	}

	void operator = (const SLayerGroup & other)
	{
		m_anGLay = other.m_anGLay;
		m_anWriteLay = other.m_anWriteLay;
		m_adScale = other.m_adScale;
		m_anGLayProtect = other.m_anGLayProtect;
		for (int i = 0; i < LAYER_COUNT; i++) {
			m_LayerList[i] = other.m_LayerList[i];
		}
		memcpy(m_aStrGLayName, other.m_aStrGLayName, MAX_STR);
	};

	void ReadParameter(CCAD_File* pStream)
	{
		pStream->ReadDWord(&m_anGLay);
		pStream->ReadDWord(&m_anWriteLay);
		pStream->ReadDouble(&m_adScale);
		pStream->ReadDWord(&m_anGLayProtect);
		for(int i = 0; i < LAYER_COUNT; i++) {
			m_LayerList[i].ReadParameter(pStream);
		}
	};

	void ReadName(CCAD_File* pStream)
	{
		BYTE nLength = 0;
		pStream->ReadByte(&nLength);
		pStream->ReadString(m_aStrGLayName, nLength);
	};
};

/****************************************************
			SMarkJump
******************************************************/
struct SMarkJump {
	double	m_dZoomJumpBairitsu;
	S_Point	m_DPZoomJumpGenten;
	DWORD	m_nZoomJumpGLay;

	SMarkJump()
	{
		m_dZoomJumpBairitsu = 0;
		//m_DPZoomJumpGenten
		m_nZoomJumpGLay = -1;
	}

	void operator = (const SMarkJump & other)
	{
		m_dZoomJumpBairitsu = other.m_dZoomJumpBairitsu;
		m_DPZoomJumpGenten = other.m_DPZoomJumpGenten;
		m_nZoomJumpGLay = other.m_nZoomJumpGLay;
	}

	void Read(CCAD_File* pStream)
	{
		pStream->ReadDouble(&m_dZoomJumpBairitsu);
		m_DPZoomJumpGenten.Read(pStream);
		if (pStream->m_nJWVersion >= 300)
			pStream->ReadDWord(&m_nZoomJumpGLay);
	}
};

/****************************************************
			SPalette
******************************************************/
struct SPalette {
	DWORD	m_aPenColor;
	DWORD	m_anPenWidth;

	DWORD	m_aPrtPenColor;
	DWORD	m_anPrtPenWidth;
	double	m_adPrtTenHankei;

	SPalette() {
		m_aPenColor = 0;
		m_anPenWidth = 0;

		m_aPrtPenColor = 0;
		m_anPrtPenWidth = 0;
		m_adPrtTenHankei = 0;
	}

	void operator = (const SPalette & other) {
		m_aPenColor = other.m_aPenColor;
		m_anPenWidth = other.m_anPenWidth;

		m_aPrtPenColor = other.m_aPrtPenColor;
		m_anPrtPenWidth = other.m_anPrtPenWidth;
		m_adPrtTenHankei = other.m_adPrtTenHankei;
	}

	void ReadDispParam(CCAD_File* pStream) {
		pStream->ReadDWord(&m_aPenColor);
		pStream->ReadDWord(&m_anPenWidth);
	}

	void ReadPrintParam(CCAD_File* pStream) {
		pStream->ReadDWord(&m_aPrtPenColor);
		pStream->ReadDWord(&m_anPrtPenWidth);
		pStream->ReadDouble(&m_adPrtTenHankei);
	}
};

/****************************************************
			SLineType
******************************************************/
struct SLineType {
	DWORD	m_alLType;
	DWORD	m_anTokushuSenUintDot;
	DWORD	m_anTokushuSenPich;
	DWORD	m_anPrtTokushuSenPich;

	SLineType()
	{
		m_alLType = 0;
		m_anTokushuSenUintDot = 32;
		m_anTokushuSenPich = 1;
		m_anPrtTokushuSenPich = 10;
	}

	void operator = (const SLineType & other)
	{
		m_alLType = other.m_alLType;
		m_anTokushuSenUintDot = other.m_anTokushuSenUintDot;
		m_anTokushuSenPich = other.m_anTokushuSenPich;
		m_anPrtTokushuSenPich = other.m_anPrtTokushuSenPich;
	}

	void Read(CCAD_File* pStream)
	{
		pStream->ReadDWord(&m_alLType);
		pStream->ReadDWord(&m_anTokushuSenUintDot);
		pStream->ReadDWord(&m_anTokushuSenPich);
		pStream->ReadDWord(&m_anPrtTokushuSenPich);
	}
};

/****************************************************
			SRandomLineType
******************************************************/
struct SRandomLineType {
	DWORD	m_alLType;
	DWORD	m_anRandSenWide;
	DWORD	m_anTokushuSenPich;
	DWORD	m_anPrtRandSenWide;
	DWORD	m_anPrtTokushuSenPich;

	SRandomLineType()
	{
		m_alLType = 0;
		m_anRandSenWide = 0;
		m_anTokushuSenPich = 0;
		m_anPrtRandSenWide = 0;
		m_anPrtTokushuSenPich = 0;
	}

	void operator = (const SRandomLineType & other)
	{
		m_alLType = other.m_alLType;
		m_anRandSenWide = other.m_anRandSenWide;
		m_anTokushuSenPich = other.m_anTokushuSenPich;
		m_anPrtRandSenWide = other.m_anPrtRandSenWide;
		m_anPrtTokushuSenPich = other.m_anPrtTokushuSenPich;
	}

	void Read(CCAD_File* pStream)
	{
		pStream->ReadDWord(&m_alLType);
		pStream->ReadDWord(&m_anRandSenWide);
		pStream->ReadDWord(&m_anTokushuSenPich);
		pStream->ReadDWord(&m_anPrtRandSenWide);
		pStream->ReadDWord(&m_anPrtTokushuSenPich);
	}
};

/****************************************************
			SSXFLineColor
******************************************************/
struct SSXFLineColor {
	DWORD	m_aPenColor;
	DWORD	m_anPenWidth;

	char	m_astrUDColorName[MAX_STR];
	DWORD	m_aPrtPenColor;
	DWORD	m_anPrtPenWidth;
	double	m_adPrtTenHankei;

	SSXFLineColor() {
		m_aPenColor = 0;
		m_anPenWidth = 0;

		memset(m_astrUDColorName, 0, MAX_STR);
		m_aPrtPenColor = 0;
		m_anPrtPenWidth = 0;
		m_adPrtTenHankei = 0;
	}

	void operator = (const SSXFLineColor & other) {
		m_aPenColor = other.m_aPenColor;
		m_anPenWidth = other.m_anPenWidth;

		memcpy(m_astrUDColorName, other.m_astrUDColorName, MAX_STR);
		m_aPrtPenColor = other.m_aPrtPenColor;
		m_anPrtPenWidth = other.m_anPrtPenWidth;
		m_adPrtTenHankei = other.m_adPrtTenHankei;
	}

	void ReadDispParam(CCAD_File* pStream) {
		pStream->ReadDWord(&m_aPenColor);
		pStream->ReadDWord(&m_anPenWidth);
	}

	void ReadPrintParam(CCAD_File* pStream) {
		BYTE nLength = 0;
		pStream->ReadByte(&nLength);
		pStream->ReadString(m_astrUDColorName, nLength);
		pStream->ReadDWord(&m_aPrtPenColor);
		pStream->ReadDWord(&m_anPrtPenWidth);
		pStream->ReadDouble(&m_adPrtTenHankei);
	}
};

/****************************************************
			SSXFLineType
******************************************************/
struct SSXFLineType {
	DWORD	m_alLType;
	DWORD	m_anTokushuSenUintDot;
	DWORD	m_anTokushuSenPich;
	DWORD	m_anPrtTokushuSenPich;

	char	m_astrUDLTypeName[MAX_STR];
	DWORD	m_anUDLTypeSegment;
	double	m_aadUDLTypePitch[10];

	SSXFLineType()
	{
		m_alLType = 0;
		m_anTokushuSenUintDot = 32;
		m_anTokushuSenPich = 1;
		m_anPrtTokushuSenPich = 10;

		memset(m_astrUDLTypeName, 0, MAX_STR);
		m_anUDLTypeSegment = 0;
		for (int i = 0; i < 10; i++) {
			m_aadUDLTypePitch[i] = 0;
		}
	}

	void operator = (const SSXFLineType & other)
	{
		m_alLType = other.m_alLType;
		m_anTokushuSenUintDot = other.m_anTokushuSenUintDot;
		m_anTokushuSenPich = other.m_anTokushuSenPich;
		m_anPrtTokushuSenPich = other.m_anPrtTokushuSenPich;

		memcpy(m_astrUDLTypeName, other.m_astrUDLTypeName, MAX_STR);
		m_anUDLTypeSegment = other.m_anUDLTypeSegment;
		for (int i = 0; i < 10; i++) {
			m_aadUDLTypePitch[i] = other.m_aadUDLTypePitch[i];
		}
	}

	void ReadPattern(CCAD_File* pStream)
	{
		pStream->ReadDWord(&m_alLType);
		pStream->ReadDWord(&m_anTokushuSenUintDot);
		pStream->ReadDWord(&m_anTokushuSenPich);
		pStream->ReadDWord(&m_anPrtTokushuSenPich);
	}

	void ReadParameter(CCAD_File* pStream)
	{
		BYTE nLength = 0;
		pStream->ReadByte(&nLength);
		pStream->ReadString(m_astrUDLTypeName, nLength);
		pStream->ReadDWord(&m_anUDLTypeSegment);
		for (int i = 0; i < 10; i++) {
			pStream->ReadDouble(&m_aadUDLTypePitch[i]);
		}
	}
};

/****************************************************
			STextType
******************************************************/
struct STextType {
	double	m_adMojiX;
	double	m_adMojiY;
	double	m_adMojiD;
	DWORD	m_anMojiCol;

	STextType() {
		m_adMojiX = 0;
		m_adMojiY = 0;
		m_adMojiD = 0;
		m_anMojiCol = 0;
	}

	void operator = (const STextType & other) {
		m_adMojiX = other.m_adMojiX;
		m_adMojiY = other.m_adMojiY;
		m_adMojiD = other.m_adMojiD;
		m_anMojiCol = other.m_anMojiCol;
	}

	void Read(CCAD_File* pStream) {
		pStream->ReadDouble(&m_adMojiX);
		pStream->ReadDouble(&m_adMojiY);
		pStream->ReadDouble(&m_adMojiD);
		pStream->ReadDWord(&m_anMojiCol);
	}
};

/****************************************************
            SData
 ******************************************************/
struct SData {
    DWORD		m_lGroup;
    BYTE		m_nPenStyle;
    WORD		m_nPenColor;
    WORD		m_nPenWidth;
    
    WORD		m_nLayer;
    WORD		m_nGLayer;
    WORD		m_sFlg;
    
    SData() {
        m_lGroup = 0;
        m_nPenStyle = 0;
        m_nPenColor = 0;
        m_nPenWidth = 0;
        
        m_nLayer = 0;
        m_nGLayer = 0;
        m_sFlg = 0;
    }
    
    void Read(CCAD_File* pStream) {
        pStream->ReadDWord(&m_lGroup);
        pStream->ReadByte(&m_nPenStyle);
        pStream->ReadWord(&m_nPenColor);
        if (pStream->m_nJWVersion >=351) {
            pStream->ReadWord(&m_nPenWidth);
        }
        pStream->ReadWord(&m_nLayer);
        pStream->ReadWord(&m_nGLayer);
        pStream->ReadWord(&m_sFlg);
    }
};

/****************************************************
            SDataSen
 ******************************************************/
struct SDataSen {
    S_Point		m_sStartPt;
    S_Point		m_sEndPt;
    
    SDataSen() {
        
    }
    
    void Read(CCAD_File* pStream) {
        m_sStartPt.Read(pStream);
        m_sEndPt.Read(pStream);
    }
};

/****************************************************
            SDataEnko
 ******************************************************/
struct SDataEnko {
    S_Point		m_sCenterPt;
    double		m_dHankei;
    double		m_radKaishiKaku;
    double		m_radEnkoKaku;
    double		m_radKatamukiKaku;
    double		m_dHenpeiRitsu;
    DWORD		m_bZenEnFlg;
    
    SDataEnko() {
        
    }
    
    void Read(CCAD_File* pStream) {
        m_sCenterPt.Read(pStream);
        pStream->ReadDouble(&m_dHankei);
        pStream->ReadDouble(&m_radKaishiKaku);
        pStream->ReadDouble(&m_radEnkoKaku);
        pStream->ReadDouble(&m_radKatamukiKaku);
        pStream->ReadDouble(&m_dHenpeiRitsu);
        pStream->ReadDWord(&m_bZenEnFlg);
    }
};

/****************************************************
            SDataTen
 ******************************************************/
struct SDataTen {
    S_Point		m_Point;
    DWORD		m_bKariten;
    DWORD		m_nCode;
    double		m_radKaitenKaku;
    double		m_dBairitsu;
    
    SDataTen() {
        
    }
    
    void Read(CCAD_File* pStream, BYTE nPenStyle) {
        m_Point.Read(pStream);
        pStream->ReadDWord(&m_bKariten);
        if (100 == nPenStyle) {
            pStream->ReadDWord(&m_nCode);
            pStream->ReadDouble(&m_radKaitenKaku);
            pStream->ReadDouble(&m_dBairitsu);
        }
    }
};

/****************************************************
            SDataMoji
 ******************************************************/
struct SDataMoji {
    S_Point		m_sStartPt;
    S_Point		m_sEndPt;
    DWORD		m_nMojiShu;
    S_Point		m_sSize;
    double		m_dKankaku;
    double		m_degKakudo;
    char		m_szFontName[MAX_STR];
    char		m_szString[MAX_STR];
    
    char		m_szBmpFileName[MAX_STR];
    BOOL		m_bIsEmbedded;
    double		m_dBmpWidth;
    double		m_dBmpHeight;
    double		m_dBmpTrim1;
    double		m_dBmpTrim2;
    double		m_dBmpScale;
    double		m_dBmpRotAngle;
    
    SDataMoji() {
        memset(m_szFontName, 0, MAX_STR);
        memset(m_szString, 0, MAX_STR);
        
        memset(m_szBmpFileName, 0, MAX_STR);
        m_bIsEmbedded = FALSE;
        m_dBmpWidth = 0;
        m_dBmpHeight = 0;
        m_dBmpTrim1 = 0;
        m_dBmpTrim2 = 0;
        m_dBmpScale = 1;
        m_dBmpRotAngle = 0;
    }
    
    void Read(CCAD_File* pStream) {
        m_sStartPt.Read(pStream);
        m_sEndPt.Read(pStream);
        pStream->ReadDWord(&m_nMojiShu);
        m_sSize.Read(pStream);
        pStream->ReadDouble(&m_dKankaku);
        pStream->ReadDouble(&m_degKakudo);
        BYTE nLength = 0;
        pStream->ReadByte(&nLength);
        pStream->ReadString(m_szFontName, nLength);
        pStream->ReadByte(&nLength);
        pStream->ReadString(m_szString, nLength);
        
        if ( strstr(m_szString, "^@BM") != NULL ) {
            if ( strstr(m_szString, "%temp%") != NULL ) {
                char seps[]  = "%,";
                char * token = NULL;
                char * array[9];
                for (int i = 0; i < 9; i++)
                    array[i] = NULL;
                
                int k = 0;
                token = strtok( m_szString, seps );
                while( token != NULL )
                {
                    array[k++] = token;
                    // Get next token:
                    token = strtok( NULL, seps ); // C4996
                }
                
                if (array[2] != NULL)
                    strcpy(m_szBmpFileName, array[2]);
                if (array[3] != NULL)
                    m_dBmpWidth = atof(array[3]);
                if (array[4] != NULL)
                    m_dBmpHeight = atof(array[4]);
                if (array[5] != NULL)
                    m_dBmpTrim1 = atof(array[5]);
                if (array[6] != NULL)
                    m_dBmpTrim2 = atof(array[6]);
                if (array[7] != NULL)
                    m_dBmpScale = atof(array[7]);
                if (array[8] != NULL)
                    m_dBmpRotAngle = atof(array[8]);
                m_bIsEmbedded = TRUE;
                
            } else {
                char seps[]  = ",";
                char * token = NULL;
                char * array[7];
                for (int i = 0; i < 7; i++)
                    array[i] = NULL;
                
                int k = 0;
                token = strtok( m_szString, seps );
                while( token != NULL )
                {
                    array[k++] = token;
                    // Get next token:
                    token = strtok( NULL, seps ); // C4996
                }
                
                if (array[0] != NULL) {
                    strcpy(m_szBmpFileName, array[0] + 4);
                }
                if (array[1] != NULL)
                    m_dBmpWidth = atof(array[1]);
                if (array[2] != NULL)
                    m_dBmpHeight = atof(array[2]);
                if (array[3] != NULL)
                    m_dBmpTrim1 = atof(array[3]);
                if (array[4] != NULL)
                    m_dBmpTrim2 = atof(array[4]);
                if (array[5] != NULL)
                    m_dBmpScale = atof(array[5]);
                if (array[6] != NULL)
                    m_dBmpRotAngle = atof(array[6]);
                m_bIsEmbedded = FALSE;
            }
        }
    }
};

/****************************************************
            SDataSunpou
 ******************************************************/
struct SDataSunpou {
    SDataSen	m_Sen;
    SDataMoji	m_Moji;
    WORD		m_bSxfMode;
    SDataSen	m_SenHo1;
    SDataSen	m_SenHo2;
    SDataTen	m_Ten1;
    SDataTen	m_Ten2;
    SDataTen	m_TenHo1;
    SDataTen	m_TenHo2;
    
    SDataSunpou() {
        
    }
    
    void Read(CCAD_File* pStream, BYTE nPenStyle) {
        m_Sen.Read(pStream);
        m_Moji.Read(pStream);
        if( pStream->m_nJWVersion >= 420 ) {		//Ver.4.20
            pStream->ReadWord(&m_bSxfMode);
            m_SenHo1.Read(pStream);
            m_SenHo2.Read(pStream);
            m_Ten1.Read(pStream, nPenStyle);
            m_Ten2.Read(pStream, nPenStyle);
            m_TenHo1.Read(pStream, nPenStyle);
            m_TenHo2.Read(pStream, nPenStyle);
        }
    }
};

/****************************************************
            SDataSolid
 ******************************************************/
struct SDataSolid {
    S_Point		m_sStartPt;
    S_Point		m_sEndPt;
    S_Point		m_sPoint2;
    S_Point		m_sPoint3;
    DWORD		m_Color;
    
    SDataSolid() {
        
    }
    
    void Read(CCAD_File* pStream, WORD nPenColor) {
        m_sStartPt.Read(pStream);
        m_sEndPt.Read(pStream);
        m_sPoint2.Read(pStream);
        m_sPoint3.Read(pStream);
        if( 10 == nPenColor ) {
            pStream->ReadDWord(&m_Color);
        }
    }
};

/****************************************************
            SDataBlock
 ******************************************************/
struct SDataBlock {
    S_Point		m_DPKijunTen;
    double		m_dBairitsuX;
    double		m_dBairitsuY;
    double		m_radKaitenKaku;
    DWORD		m_nNumber;
    
    SDataBlock() {
        
    }
    
    void Read(CCAD_File* pStream) {
        m_DPKijunTen.Read(pStream);
        pStream->ReadDouble(&m_dBairitsuX);
        pStream->ReadDouble(&m_dBairitsuY);
        pStream->ReadDouble(&m_radKaitenKaku);
        pStream->ReadDWord(&m_nNumber);
    }
};

/****************************************************
            SDataList
 ******************************************************/
struct SDataList {
    DWORD		m_nNumber;
    DWORD		m_bReffered;
    DWORD		m_time;
    char		m_szName[MAX_STR];
    //CCAD_PtrArray	m_DataList;
    
    SDataList() {
        memset(m_szName, 0, MAX_STR);
    }
    
    void Read(CCAD_File* pStream) {
        pStream->ReadDWord(&m_nNumber);
        pStream->ReadDWord(&m_bReffered);
        pStream->ReadDWord(&m_time);
        BYTE nLength = 0;
        pStream->ReadByte(&nLength);
        pStream->ReadString(m_szName, nLength);
        
        ////
        //short nDataCnt = 0;
        //pStream->ReadShort(&nDataCnt);
        
        //short nRepeatNo;
        //short nVersion;
        //short nLetterCnt;
        
        //for (int i = 0; i < nDataCnt; i++) {
        //	pStream->ReadShort(&nRepeatNo);
        //	nRepeatNo = nRepeatNo & 0x7FFF;
        
        //	if (nRepeatNo == 0x7FFF) {
        //		pStream->ReadShort(&nVersion);
        //		pStream->ReadShort(&nLetterCnt);
        //		SClassInfo * pClassInfo = new SClassInfo();
        //		pStream->ReadString(pClassInfo->szName, nLetterCnt);
        
        //		CData * pNewData = NewDataClass(pClassInfo->szName);
        //		if (pNewData != NULL) {
        //			pClassInfo->nOrder = g_ClassInfoList.GetCount() + g_ClassCnt;
        //			g_ClassInfoList.Add(pClassInfo);
        
        //			pNewData->Read(pStream);
        //			m_DataList.Add(pNewData);
        
        //		} else {
        //			delete pClassInfo;
        //		}
        
        //	} else {
        //		char * szClassName = GetDataClassName(nRepeatNo);
        //		if (szClassName != NULL) {
        //			CData * pNewData = NewDataClass(szClassName);
        //			if (pNewData != NULL) {
        //				pNewData->Read(pStream);
        //				m_DataList.Add(pNewData);
        //			}
        //		}
        //	}
        //}
    }
};
