
#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Entity.h"
#include "DXF_Text.h"
#include "stdlib.h"

#define		DXF_TEXT_FIRSTALIGNPOINT		10
#define		DXF_TEXT_TEXTHEIGHT				40
#define		DXF_TEXT_TEXT					1
#define		DXF_TEXT_ROTATEANGLE			50
#define		DXF_TEXT_RELATIVEXSCALE			41
#define		DXF_TEXT_OBLIQUEANGLE			51
#define		DXF_TEXT_STYLENAME				7
#define		DXF_TEXT_TEXTGENCODE			71
#define		DXF_TEXT_HORIJUSTCODE			72
#define		DXF_TEXT_SECONDALIGNPOINT		11
#define		DXF_TEXT_VERTJUSTCODE			73

CDXF_Text::CDXF_Text()
{
	m_nEntityType = DXF_TEXT;

	m_dThickness = 0.0;				// default = 0.0
	m_dTextHeight = 0.0;
	memset(m_pszText, 0x00, MAX_STRING_LEN);
	m_dRotateAngle = 0.0;			// default = 0.0
	m_dRelativeXScale = 1.0;		// default = 1.0
	m_dObliqueAngle = 0.0;			// default = 0.0
	memcpy(m_pszTextStyleName, "STANDARD", MAX_STRING_LEN);	// default = "STANDARD"
	m_lTextGenCode = 0;				// default = 0
	m_lHoriJustCode = 0;			// default = 0
	m_lVertJustCode = 0;			// default = 0

	m_lDXFTextStyle = 0L;
	m_spDXFTextStyle = NULL;
}

CDXF_Text::~CDXF_Text()
{
	if (m_spDXFTextStyle != NULL)
		delete [] m_spDXFTextStyle;
}

DXF_BOOL CDXF_Text::DXFReadEntity(FILE *pFile)
{
	DXF_LONG		lCode;
	DXF_CHAR		szValue[MAX_STRING_LEN];

	g_lDXFErrorCode = DXF_SUCCESS;

	if (!DXFReadLong(pFile, lCode)) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	do {
		switch (lCode) {	
			case DXF_HANDLE_CODE:		// handle number
				if (!DXFReadHexLong(pFile, m_lHandleNumber)) {
					g_lDXFErrorCode = DXF_INVALID_HANDLE;
					return FALSE;
				}
				break;
			case DXF_SPACE_CODE:	// modelspace ? paperspace?
				if (!DXFReadLong(pFile, m_lSpaceType)) {
					g_lDXFErrorCode = DXF_INVALID_MPSPACE;
					return FALSE;
				}
				break;
			case DXF_LAYER_CODE:		// layer name
				if (!DXFReadString(pFile, m_pszLayerName)) {
					g_lDXFErrorCode = DXF_INVALID_LAYER;
					return FALSE;
				}
				break;
			case DXF_LTYPE_CODE:		// line type name
				if (!DXFReadString(pFile, m_pszLTypeName)) {
					g_lDXFErrorCode = DXF_INVALID_LINETYPE;
					return FALSE;
				}
				break;
			case DXF_COLOR_CODE:	// color index
				if (!DXFReadLong(pFile, m_lDXFColor)) {
					g_lDXFErrorCode = DXF_INVALID_COLOR;
					return FALSE;
				}
				break;
			case DXF_LTYPESCALE_CODE:	// line style scale
				if (!DXFReadDouble(pFile, m_dLTypeScale)) {
					g_lDXFErrorCode = DXF_INVALID_LINETYPESCALE;
					return FALSE;
				}
				break;
			case DXF_EXTRUSION_CODE:	// Extrusion direction
				if (!DXFReadPoint3D(pFile, DXF_EXTRUSION_CODE, m_s3DExtrusion)) {
					g_lDXFErrorCode = DXF_INVALID_EXTRUSION;
					return FALSE;
				}
				break;
			case DXF_VISIBILITY_CODE:	// visibility of object
				if (!DXFReadLong(pFile, m_lVisibility)) {
					g_lDXFErrorCode = DXF_INVALID_VISIBILITY;
					return FALSE;
				}
				break;
			case DXF_THICK_CODE:
				if (!DXFReadDouble(pFile, m_dThickness)) {
					g_lDXFErrorCode = DXF_INVALID_THICKNESS;
					return FALSE;
				}
				break;
			case DXF_TEXT_FIRSTALIGNPOINT:
				if (!DXFReadPoint3D(pFile, DXF_TEXT_FIRSTALIGNPOINT, m_s3DFirstAlignPoint)) {
					g_lDXFErrorCode = DXF_INVALID_POINT3D;
					return FALSE;
				}
				break;
			case DXF_TEXT_TEXTHEIGHT:
				if (!DXFReadDouble(pFile, m_dTextHeight)) {
					g_lDXFErrorCode = DXF_INVALID_TEXTHEIGHT;
					return FALSE;
				}
				break;
			case DXF_TEXT_TEXT:
				if (!DXFReadString(pFile, m_pszText)) {
					g_lDXFErrorCode = DXF_INVALID_STRING;
					return FALSE;
				}
				break;
			case DXF_TEXT_ROTATEANGLE:
				if (!DXFReadDouble(pFile, m_dRotateAngle)) {
					g_lDXFErrorCode = DXF_INVALID_ANGLE;
					return FALSE;
				}
				break;
			case DXF_TEXT_RELATIVEXSCALE:
				if (!DXFReadDouble(pFile, m_dRelativeXScale)) {
					g_lDXFErrorCode = DXF_INVALID_SCALE;
					return FALSE;
				}
				break;
			case DXF_TEXT_OBLIQUEANGLE:
				if (!DXFReadDouble(pFile, m_dObliqueAngle)) {
					g_lDXFErrorCode = DXF_INVALID_ANGLE;
					return FALSE;
				}
				break;
			case DXF_TEXT_STYLENAME:
				if (!DXFReadString(pFile, m_pszTextStyleName)) {
					g_lDXFErrorCode = DXF_INVALID_TEXTSTYLE;
					return FALSE;
				}
				break;
			case DXF_TEXT_TEXTGENCODE:
				if (!DXFReadLong(pFile, m_lTextGenCode)) {
					g_lDXFErrorCode = DXF_INVALID_TEXTGENCODE;
					return FALSE;
				}
				break;
			case DXF_TEXT_HORIJUSTCODE:
				if (!DXFReadLong(pFile, m_lHoriJustCode)) {
					g_lDXFErrorCode = DXF_INVALID_TEXTHORIJUST;
					return FALSE;
				}
				break;
			case DXF_TEXT_SECONDALIGNPOINT:
				if (!DXFReadPoint3D(pFile, DXF_TEXT_SECONDALIGNPOINT, m_s3DSecondAlignPoint)) {
					g_lDXFErrorCode = DXF_INVALID_POINT3D;
					return FALSE;
				}
				break;
			case DXF_TEXT_VERTJUSTCODE:
				if (!DXFReadLong(pFile, m_lVertJustCode)) {
					g_lDXFErrorCode = DXF_INVALID_TEXTVERTJUST;
					return FALSE;
				}
				break;
			default:
				if (!DXFReadString(pFile, szValue)) {
					g_lDXFErrorCode = DXF_INVALID_STRING;
					return FALSE;
				}
				break;
		}
		if (!DXFReadLong(pFile, lCode)) {
			g_lDXFErrorCode = DXF_INVALID_CODE;
			return FALSE;
		}
	} while (lCode != 0);

	DXFInterpretText();

	return TRUE;
}


void CDXF_Text::DXFExchangeCode(DXF_CHAR* pszCode, DXF_CHAR szCode)
{
	DXF_CHAR*	pszDest = NULL;
	DXF_LONG	lResult, lSize;

	lResult = 0;
	do {
		lSize = strlen(m_pszText);
		pszDest = strstr(m_pszText + lResult, pszCode);
		if (pszDest != NULL) {
			lResult = pszDest - m_pszText;
			strcpy(m_pszText + lResult, pszDest + 3);
			lResult++;
		}
	} while (pszDest != NULL);
}

#define		DXF_TEXT_UNDERSCORE		1L
#define		DXF_TEXT_OVERSCORE		2L

void CDXF_Text::DXFInterpretText()
{
	DXF_CHAR*	pszDest = NULL;
	DXF_LONG	i, lResult;

	DXFExchangeCode("%%c", (DXF_CHAR)0xED);

	DXFExchangeCode("%%d", (DXF_CHAR)0xB0);

	DXFExchangeCode("%%p", (DXF_CHAR)0xB1);

	// %%xxx and %%%
	lResult = 0;
	do {
		pszDest = strstr(m_pszText + lResult, "%%");
		if (pszDest != NULL) {
			lResult = pszDest - m_pszText;
			if (isdigit(pszDest[2]) && isdigit(pszDest[3]) && isdigit(pszDest[4])) {
				DXF_CHAR	digits[4];

				strncpy(digits, pszDest + 2, 3);
				m_pszText[lResult] = (DXF_CHAR)atol(digits);
				strcpy(m_pszText + lResult, pszDest + 5);
				lResult++;
			}
			else if (pszDest[2] == '%') {
				m_pszText[lResult] = '%';
				lResult++;
				strcpy(m_pszText + lResult, pszDest + 3);
			}
			else {
				lResult += 2;
			}
		}
	} while (pszDest != NULL);

	// UniCode
	lResult = 0;
	pszDest = m_pszText;
	while (pszDest != NULL) {
		pszDest = strstr(m_pszText + lResult, "\\U");
		if (pszDest != NULL) {
			lResult = pszDest - m_pszText;
			strcpy(m_pszText + lResult, pszDest + 7);
			lResult;
		}
	}

	DXF_BYTE	bpStatus[MAX_STRING_LEN];

	lResult = 0;
	pszDest = m_pszText;
	memset(bpStatus, 0x00, MAX_STRING_LEN);
	while (pszDest != NULL) {
		pszDest = strstr(m_pszText + lResult, "%%");
		if (pszDest != NULL) {
			if (pszDest[2] == 'u' || pszDest[2] == 'U') {
				lResult = pszDest - m_pszText;
				strcpy(m_pszText + lResult, pszDest + 3);
				bpStatus[lResult] += DXF_TEXT_UNDERSCORE;
			//	lResult = 0;
			}
			else if (pszDest[2] == 'o' || pszDest[2] == 'O') {
				lResult = pszDest - m_pszText;
				strcpy(m_pszText + lResult, pszDest + 3);
				bpStatus[lResult] += DXF_TEXT_OVERSCORE;
			//	lResult = 0;
			}
			else {
				lResult++;
			}
		}
	}

	lResult = strlen(m_pszText);
	bpStatus[lResult] = 0xFF;

	SDXF_TextStyle	sDXFTextStyle;

	for (i = 0; i < lResult; i++) {
		if (bpStatus[i] == DXF_TEXT_UNDERSCORE) {
			sDXFTextStyle.lEnd = i - 1;
			DXFAppendTextStyle(sDXFTextStyle);
			sDXFTextStyle.bUnder = 1 - sDXFTextStyle.bUnder;
			sDXFTextStyle.lStart = i;
		}
		else if (bpStatus[i] == DXF_TEXT_OVERSCORE) {
			sDXFTextStyle.lEnd = i - 1;
			DXFAppendTextStyle(sDXFTextStyle);
			sDXFTextStyle.bOver = 1 - sDXFTextStyle.bOver;
			sDXFTextStyle.lStart = i;
		}
		else if (bpStatus[i] == (DXF_TEXT_OVERSCORE + DXF_TEXT_UNDERSCORE)) {
			sDXFTextStyle.lEnd = i - 1;
			DXFAppendTextStyle(sDXFTextStyle);
			sDXFTextStyle.bOver = 1 - sDXFTextStyle.bOver;
			sDXFTextStyle.bUnder = 1 - sDXFTextStyle.bUnder;
			sDXFTextStyle.lStart = i;
		}
	}
	sDXFTextStyle.lEnd = i - 1;
	DXFAppendTextStyle(sDXFTextStyle);
}

void CDXF_Text::DXFAppendTextStyle(SDXF_TextStyle sDXFTextStyle)
{
	if (m_lDXFTextStyle == 0) {
		m_spDXFTextStyle = new SDXF_TextStyle;
	}
	else {
		SDXF_TextStyle*		spTempStyle = new SDXF_TextStyle [m_lDXFTextStyle + 1];

		memcpy(spTempStyle, m_spDXFTextStyle, sizeof(SDXF_TextStyle) * m_lDXFTextStyle);
		delete [] m_spDXFTextStyle;
		m_spDXFTextStyle = spTempStyle;
	}
	m_spDXFTextStyle[m_lDXFTextStyle] = sDXFTextStyle;
	m_lDXFTextStyle++;
}
