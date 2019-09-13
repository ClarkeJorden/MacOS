
#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Table.h"
#include "DXF_StyleTable.h"

#define		DXF_STYLETABLE_NAME				2
#define		DXF_STYLETABLE_CODE				70
#define		DXF_STYLETABLE_FIXEDTEXTHEIGHT	40
#define		DXF_STYLETABLE_WIDTHFACTOR		41
#define		DXF_STYLETABLE_OBLIQUEANGLE		50
#define		DXF_STYLETABLE_TEXTGENCODE		71
#define		DXF_STYLETABLE_LASTTEXTHEIGHT	42
#define		DXF_STYLETABLE_FIRSTFONTNAME	3
#define		DXF_STYLETABLE_BIGFONTNAME		4
#define		DXF_STYLETABLE_FACENAME			1000
#define		DXF_STYLETABLE_ACADNAME			1001
#define		DXF_STYLETABLE_NONAME			1071


CDXF_StyleTable::CDXF_StyleTable()
{
	m_nTableType = DXF_STYLETABLE;

	strcpy(m_pszStyleName, "STANDARD");
	m_lStyleCode = 0;
	m_dFixedTextHeight = 0.0;
	m_dWidthFactor = 1.0;
	m_dObliqueAngle = 0.0;
	m_lTextGenCode = 0;
	m_dLastTextHeight = 0.2;
	strcpy(m_pszFirstFontFileName, "txt");
	strcpy(m_pszBigFontFileName, "");
	strcpy(m_pszFontFaceName, "");
}

CDXF_StyleTable::~CDXF_StyleTable()
{

}

DXF_BOOL CDXF_StyleTable::DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue)
{
	if (!DXFReadLong(pFile, lCode))
	{
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}

	while (lCode)
	{
		switch (lCode)
		{
			case DXF_HANDLE_CODE:
				if (!DXFReadHexLong(pFile, m_lHandleNumber))
				{
					g_lDXFErrorCode = DXF_INVALID_HANDLE;
					return FALSE;
				}
				break;
			case DXF_STYLETABLE_NAME:
				if (!DXFReadString(pFile, m_pszStyleName))
				{
					g_lDXFErrorCode = DXF_INVALID_STYLENAME;
					return FALSE;
				}
				break;
			case DXF_STYLETABLE_CODE:
				if (!DXFReadLong(pFile, m_lStyleCode))
				{
					g_lDXFErrorCode = DXF_INVALID_STYLECODE;
					return FALSE;
				}
				break;
			case DXF_STYLETABLE_FIXEDTEXTHEIGHT:
				if (!DXFReadDouble(pFile, m_dFixedTextHeight))
				{
					g_lDXFErrorCode = DXF_INVALID_TEXTHEIGHT;
					return FALSE;
				}
				break;
			case DXF_STYLETABLE_WIDTHFACTOR:
				if (!DXFReadDouble(pFile, m_dWidthFactor))
				{
					g_lDXFErrorCode = DXF_INVALID_SCALE;
					return FALSE;
				}
				break;
			case DXF_STYLETABLE_OBLIQUEANGLE:
				if (!DXFReadDouble(pFile, m_dObliqueAngle))
				{
					g_lDXFErrorCode = DXF_INVALID_ANGLE;
					return FALSE;
				}
				break;
			case DXF_STYLETABLE_TEXTGENCODE:
				if (!DXFReadLong(pFile, m_lTextGenCode))
				{
					g_lDXFErrorCode = DXF_INVALID_TEXTGENCODE;
					return FALSE;
				}
				break;
			case DXF_STYLETABLE_LASTTEXTHEIGHT:
				if (!DXFReadDouble(pFile, m_dLastTextHeight))
				{
					g_lDXFErrorCode = DXF_INVALID_TEXTHEIGHT;
					return FALSE;
				}
				break;
			case DXF_STYLETABLE_FIRSTFONTNAME:
				if (!DXFReadString(pFile, m_pszFirstFontFileName))
				{
					g_lDXFErrorCode = DXF_INVALID_FONTNAME;
					return FALSE;
				}
				break;
			case DXF_STYLETABLE_BIGFONTNAME:
				if (!DXFReadString(pFile, m_pszBigFontFileName))
				{
					g_lDXFErrorCode = DXF_INVALID_FONTNAME;
					return FALSE;
				}
				break;
			case DXF_STYLETABLE_FACENAME:
				if (!DXFReadString(pFile, m_pszFontFaceName))
				{
					g_lDXFErrorCode = DXF_INVALID_FONTNAME;
					return FALSE;
				}
				break;
			default:
				if (!DXFReadString(pFile, szValue))
				{
					g_lDXFErrorCode = DXF_INVALID_STRING;
					return FALSE;
				}
				break;
		}
		if (!DXFReadLong(pFile, lCode))
		{
			g_lDXFErrorCode = DXF_INVALID_CODE;
			return FALSE;
		}
	}
	
	if (!DXFReadString(pFile, szValue))
	{
		g_lDXFErrorCode = DXF_INVALID_STRING;
		return FALSE;
	}

	return TRUE;
}

