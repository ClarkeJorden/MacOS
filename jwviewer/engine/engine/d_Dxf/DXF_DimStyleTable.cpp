#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Table.h"
#include "DXF_DimStyleTable.h"

#define		DXF_DIMSTYLE_NAME		2
#define		DXF_DIMSTYLE_FLAG		70
#define		DXF_DIMSTYLE_DIMPOST	3
#define		DXF_DIMSTYLE_DIMAPOST	4
#define		DXF_DIMSTYLE_ARROWTYPE1		5
#define		DXF_DIMSTYLE_ARROWTYPE2		6
#define		DXF_DIMSTYLE_ARROWTYPE3		7
#define		DXF_DIMSTYLE_DIMSCALE	40
#define		DXF_DIMSTYLE_DIMASZ		41
#define		DXF_DIMSTYLE_DIMEXO		42
#define		DXF_DIMSTYLE_DIMDLI		43
#define		DXF_DIMSTYLE_DIMEXE		44
#define		DXF_DIMSTYLE_DIMRND		45
#define		DXF_DIMSTYLE_DIMDLE		46
#define		DXF_DIMSTYLE_DIMTP		47
#define		DXF_DIMSTYLE_DIMTM		48
#define		DXF_DIMSTYLE_DIMTXT		140
#define		DXF_DIMSTYLE_DIMCEN		141
#define		DXF_DIMSTYLE_DIMTSZ		142
#define		DXF_DIMSTYLE_DIMALTF	143
#define		DXF_DIMSTYLE_DIMLFAC	144
#define		DXF_DIMSTYLE_DIMTVP		145
#define		DXF_DIMSTYLE_DIMTFAC	146
#define		DXF_DIMSTYLE_DIMGAP		147
#define		DXF_DIMSTYLE_DIMTOL		71
#define		DXF_DIMSTYLE_DIMLIM		72
#define		DXF_DIMSTYLE_DIMTIH		73
#define		DXF_DIMSTYLE_DIMTOH		74
#define		DXF_DIMSTYLE_DIMSE1		75
#define		DXF_DIMSTYLE_DIMSE2		76
#define		DXF_DIMSTYLE_DIMTAD		77
#define		DXF_DIMSTYLE_DIMZIN		78
#define		DXF_DIMSTYLE_DIMALT		170
#define		DXF_DIMSTYLE_DIMALTD	171
#define		DXF_DIMSTYLE_DIMTOFL	172
#define		DXF_DIMSTYLE_DIMSAH		173
#define		DXF_DIMSTYLE_DIMTIX		174
#define		DXF_DIMSTYLE_DIMSOXD	175
#define		DXF_DIMSTYLE_DIMCLRD	176
#define		DXF_DIMSTYLE_DIMCLRE	177
#define		DXF_DIMSTYLE_DIMCLRT	178
#define		DXF_DIMSTYLE_DIMUNIT	270
#define		DXF_DIMSTYLE_DIMDEC		271
#define		DXF_DIMSTYLE_DIMTDEC	272
#define		DXF_DIMSTYLE_DIMALTU	273
#define		DXF_DIMSTYLE_DIMALTTD	274
#define		DXF_DIMSTYLE_DIMTXSTY	340
#define		DXF_DIMSTYLE_DIMAUNIT	275
#define		DXF_DIMSTYLE_DIMJUST	280
#define		DXF_DIMSTYLE_DIMSD1		281
#define		DXF_DIMSTYLE_DIMSD2		282
#define		DXF_DIMSTYLE_DIMTOLJ	283
#define		DXF_DIMSTYLE_DIMTZIN	284
#define		DXF_DIMSTYLE_DIMALTZ	285
#define		DXF_DIMSTYLE_DIMALTTZ	286
#define		DXF_DIMSTYLE_DIMFIT		287
#define		DXF_DIMSTYLE_DIMUPT		288

#define		DXF_ARROWTYPE_NUM		20

char	* g_szDXFArrowName[DXF_ARROWTYPE_NUM] = 
											{	"CLOSEDFILLED",
												"CLOSEDBLANK",
												"CLOSED",
												"DOT",
												"ARCHTICK",
												"OBLIQUE",
												"OPEN",
												"ORIGIN",
												"ORIGIN2",
												"OPEN90",
												"OPEN30",
												"DOTSMALL",
												"DOTBLANK",
												"SMALL",
												"BOXBLANK",
												"BOXFILLED",
												"DATUMBLANK",
												"DATUMFILLED",
												"INTEGRAL",
												"NONE" };

CDXF_DimStyleTable::CDXF_DimStyleTable()
{
	m_nTableType = DXF_DIMSTYLETABLE;

	memcpy(m_pszDimStyleName, "STANDARD", MAX_STRING_LEN);
	m_lDimStyleCode = 0;
	memset(m_pszDIMPOST, 0x00, MAX_STRING_LEN);
	memset(m_pszDIMAPOST, 0x00, MAX_STRING_LEN);

	m_lARROWTYPE1 = 0;
	m_lARROWTYPE2 = 0;
	m_lARROWTYPE3 = 0;

	m_dDIMSCALE = 1.0;
	m_dDIMASZ = 0.18;
	m_dDIMEXO = 0.0625;
	m_dDIMDLI = 0.38;
	m_dDIMEXE = 0.18;
	m_dDIMRND = 0.0;
	m_dDIMDLE = 0.0;
	m_dDIMTP = 0.0;
	m_dDIMTM = 0.0;
	m_dDIMTXT = 0.18;
	m_dDIMCEN = 0.09;
	m_dDIMTSZ = 0.0;
	m_dDIMALTF = 25.4f;
	m_dDIMLFAC = 1.0;
	m_dDIMTVP = 0.0;
	m_dDIMTFAC = 1.0;
	m_dDIMGAP = 0.09;
	m_lDIMTOL = 0;
	m_lDIMLIM = 0;
	m_lDIMTIH = 0;
	m_lDIMTOH = 0;
	m_lDIMSE1 = 0;
	m_lDIMSE2 = 0;
	m_lDIMTAD = 1;
	m_lDIMZIN = 8;
	m_lDIMALT = 0;
	m_lDIMALTD = 3;
	m_lDIMTOFL = 1;
	m_lDIMSAH = 0;
	m_lDIMTIX = 0;
	m_lDIMSOXD = 0;
	m_lDIMCLRD = 0;
	m_lDIMCLRE = 0;
	m_lDIMCLRT = 0;
	m_lDIMUNIT = 2;
	m_lDIMDEC = 2;
	m_lDIMTDEC = 2;
	m_lDIMALTU = 2;
	m_lDIMALTTD = 3;
	m_lDIMTXSTY = 0;
	m_lDIMAUNIT = 0;
	m_lDIMJUST = 0;
	m_lDIMSD1 = 0;
	m_lDIMSD2 = 0;
	m_lDIMTOLJ = 0;
	m_lDIMTZIN = 8;
	m_lDIMALTZ = 0;
	m_lDIMALTTZ = 0;
	m_lDIMFIT = 3;
	m_lDIMUPT = 0;
}

CDXF_DimStyleTable::~CDXF_DimStyleTable()
{

}

DXF_BOOL CDXF_DimStyleTable::DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue)
{
	if (!DXFReadLong(pFile, lCode)) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}

	char	arrowName[MAX_STRING_LEN];

	while (lCode) {
		switch (lCode) {
			case DXF_DIMSTYLE_NAME:
				if (!DXFReadString(pFile, m_pszDimStyleName)) {
					g_lDXFErrorCode = DXF_INVALID_DIMSTYLE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_FLAG:
				if (!DXFReadLong(pFile, m_lDimStyleCode)) {
					g_lDXFErrorCode = DXF_INVALID_TABLECODE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMPOST:
				if (!DXFReadString(pFile, m_pszDIMPOST)) {
					g_lDXFErrorCode = DXF_INVALID_STRING;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMAPOST:
				if (!DXFReadString(pFile, m_pszDIMAPOST)) {
					g_lDXFErrorCode = DXF_INVALID_STRING;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_ARROWTYPE1:
				if (!DXFReadString(pFile, arrowName)) {
					g_lDXFErrorCode = DXF_INVALID_BLOCKNAME;
					return FALSE;
				}
				m_lARROWTYPE1 = GetArrowType(arrowName);
				break;
			case DXF_DIMSTYLE_ARROWTYPE2:
				if (!DXFReadString(pFile, arrowName)) {
					g_lDXFErrorCode = DXF_INVALID_BLOCKNAME;
					return FALSE;
				}
				m_lARROWTYPE2 = GetArrowType(arrowName);
				break;
			case DXF_DIMSTYLE_ARROWTYPE3:
				if (!DXFReadString(pFile, arrowName)) {
					g_lDXFErrorCode = DXF_INVALID_BLOCKNAME;
					return FALSE;
				}
				m_lARROWTYPE3 = GetArrowType(arrowName);
				break;
			case DXF_DIMSTYLE_DIMSCALE:
				if (!DXFReadDouble(pFile, m_dDIMSCALE)) {
					g_lDXFErrorCode = DXF_INVALID_SCALE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMASZ:
				if (!DXFReadDouble(pFile, m_dDIMASZ)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMEXO:
				if (!DXFReadDouble(pFile, m_dDIMEXO)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMDLI:
				if (!DXFReadDouble(pFile, m_dDIMDLI)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMEXE:
				if (!DXFReadDouble(pFile, m_dDIMEXE)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMRND:
				if (!DXFReadDouble(pFile, m_dDIMRND)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMDLE:
				if (!DXFReadDouble(pFile, m_dDIMDLE)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMTP:
				if (!DXFReadDouble(pFile, m_dDIMTP)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMTM:
				if (!DXFReadDouble(pFile, m_dDIMTM)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMTXT:
				if (!DXFReadDouble(pFile, m_dDIMTXT)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMCEN:
				if (!DXFReadDouble(pFile, m_dDIMCEN)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMTSZ:
				if (!DXFReadDouble(pFile, m_dDIMTSZ)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMALTF:
				if (!DXFReadDouble(pFile, m_dDIMALTF)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMLFAC:
				if (!DXFReadDouble(pFile, m_dDIMLFAC)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMTVP:
				if (!DXFReadDouble(pFile, m_dDIMTVP)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMTFAC:
				if (!DXFReadDouble(pFile, m_dDIMTFAC)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMGAP:
				if (!DXFReadDouble(pFile, m_dDIMGAP)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMTOL:
				if (!DXFReadLong(pFile, m_lDIMTOL)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMLIM:
				if (!DXFReadLong(pFile, m_lDIMLIM)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMTIH:
				if (!DXFReadLong(pFile, m_lDIMTIH)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMTOH:
				if (!DXFReadLong(pFile, m_lDIMTOH)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMSE1:
				if (!DXFReadLong(pFile, m_lDIMSE1)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMSE2:
				if (!DXFReadLong(pFile, m_lDIMSE2)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMTAD:
				if (!DXFReadLong(pFile, m_lDIMTAD)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMZIN:
				if (!DXFReadLong(pFile, m_lDIMZIN)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMALT:
				if (!DXFReadLong(pFile, m_lDIMALT)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMALTD:
				if (!DXFReadLong(pFile, m_lDIMALTD)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMTOFL:
				if (!DXFReadLong(pFile, m_lDIMTOFL)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMSAH:
				if (!DXFReadLong(pFile, m_lDIMSAH)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMTIX:
				if (!DXFReadLong(pFile, m_lDIMTIX)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMSOXD:
				if (!DXFReadLong(pFile, m_lDIMSOXD)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMCLRD:
				if (!DXFReadLong(pFile, m_lDIMCLRD)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMCLRE:
				if (!DXFReadLong(pFile, m_lDIMCLRE)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMCLRT:
				if (!DXFReadLong(pFile, m_lDIMCLRT)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMUNIT:
				if (!DXFReadLong(pFile, m_lDIMUNIT)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMDEC:
				if (!DXFReadLong(pFile, m_lDIMDEC)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMTDEC:
				if (!DXFReadLong(pFile, m_lDIMTDEC)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMALTU:
				if (!DXFReadLong(pFile, m_lDIMALTU)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMALTTD:
				if (!DXFReadLong(pFile, m_lDIMALTTD)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMTXSTY:
				if (!DXFReadHexLong(pFile, m_lDIMTXSTY)) {
					g_lDXFErrorCode = DXF_INVALID_HANDLE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMAUNIT:
				if (!DXFReadLong(pFile, m_lDIMAUNIT)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMJUST:
				if (!DXFReadLong(pFile, m_lDIMJUST)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMSD1:
				if (!DXFReadLong(pFile, m_lDIMSD1)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMSD2:
				if (!DXFReadLong(pFile, m_lDIMSD2)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMTOLJ:
				if (!DXFReadLong(pFile, m_lDIMTOLJ)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMTZIN:
				if (!DXFReadLong(pFile, m_lDIMTZIN)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMALTZ:
				if (!DXFReadLong(pFile, m_lDIMALTZ)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMALTTZ:
				if (!DXFReadLong(pFile, m_lDIMALTTZ)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMFIT:
				if (!DXFReadLong(pFile, m_lDIMFIT)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_DIMSTYLE_DIMUPT:
				if (!DXFReadLong(pFile, m_lDIMUPT)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			default:
				if (!DXFReadString(pFile, szValue)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
		}
		if (!DXFReadLong(pFile, lCode)) {
			g_lDXFErrorCode = DXF_INVALID_CODE;
			return FALSE;
		}
	}

	if (!DXFReadString(pFile, szValue)) {
		g_lDXFErrorCode = DXF_INVALID_VALUE;
		return FALSE;
	}

	return TRUE;
}

long	CDXF_DimStyleTable::GetArrowType( char * arrowName)
{
	static	long	prevType = 0;

	if ( strlen(arrowName) != 0 ) {
		for ( long i=0; i<DXF_ARROWTYPE_NUM; i++ ) {
			if ( strcmp( arrowName, g_szDXFArrowName[i] ) == 0 ) {
				prevType = i;
				break;
			}
		}
	}
	return prevType;
}

void	CDXF_DimStyleTable::GetArrowName( long type, char * arrowName)
{
	if ( type >= 1 && type <= DXF_ARROWTYPE_NUM )
		::strcpy(arrowName, g_szDXFArrowName[type]);
	else
		arrowName[0] = '\0';
}

