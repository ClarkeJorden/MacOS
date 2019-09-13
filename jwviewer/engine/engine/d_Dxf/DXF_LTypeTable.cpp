
#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Table.h"
#include "DXF_LTypeTable.h"

#define		DXF_LTYPE_NAME				2
#define		DXF_LTYPE_FLAG				70
#define		DXF_LTYPE_DESCRIPTION		3
#define		DXF_LTYPE_ALIGNCODE			72
#define		DXF_LTYPE_ELEMCOUNT			73
#define		DXF_LTYPE_TOTALPATLEN		40
#define		DXF_LTYPE_DASHLEN			49
#define		DXF_LTYPE_DASHTYPE			74
#define		DXF_LTYPE_SHAPECODE			75
#define		DXF_LTYPE_STYLEOBJPOINTER	340
#define		DXF_LTYPE_DASHSCALE			46
#define		DXF_LTYPE_DASHANGLE			50
#define		DXF_LTYPE_DASHXOFFSET		44
#define		DXF_LTYPE_DASHYOFFSET		45
#define		DXF_LTYPE_STRING				9

CDXF_LTypeTable::CDXF_LTypeTable()
{
	m_nTableType = DXF_LTYPETABLE;

	sprintf(m_pszLineTypeName, "%s", "BYLAYER");
	m_lLTypeCode = 0;
	memset(m_pszLTypeComment, 0x00, MAX_STRING_LEN);
	m_lAlignmentCode = 65;
	m_lLTypeElemNum = 0;
	m_dTotPatLen = 0.0;
	m_spLTypeElement = NULL;
}

CDXF_LTypeTable::~CDXF_LTypeTable()
{
	if (m_spLTypeElement)
		delete [] m_spLTypeElement;
}

DXF_BOOL CDXF_LTypeTable::DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue)
{
	DXF_LONG	lDashCnt = -1;

	if (!DXFReadLong(pFile, lCode))
	{
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}

	while (lCode != 0)
	{
		switch (lCode)
		{
			case DXF_LTYPE_NAME:		// Line Type Name
				if (!DXFReadString(pFile, m_pszLineTypeName))
				{
					g_lDXFErrorCode = DXF_INVALID_LTYPENAME;
					return FALSE;
				}
				break;
			case DXF_LTYPE_FLAG:	// Standard flag value (bit-coded values):
				if (!DXFReadLong(pFile, m_lLTypeCode))
				{
					g_lDXFErrorCode = DXF_INVALID_LTYPECODE;
					return FALSE;
				}
				break;
			case DXF_LTYPE_DESCRIPTION:		// Descriptive text for linetype
				if (!DXFReadString(pFile, m_pszLTypeComment))
				{
					g_lDXFErrorCode = DXF_INVALID_STRING;
					return FALSE;
				}
				break;
			case DXF_LTYPE_ALIGNCODE:	// Alignment lCode; value is always 65
				if (!DXFReadLong(pFile, m_lAlignmentCode))
				{
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_LTYPE_ELEMCOUNT:	// The number of linetype elements
				if (!DXFReadLong(pFile, m_lLTypeElemNum))
				{
					g_lDXFErrorCode = DXF_INVALID_COUNT;
					return FALSE;
				}
				if (m_lLTypeElemNum < 0)
				{
					g_lDXFErrorCode = DXF_INVALID_COUNT;
					return FALSE;
				}
				if (m_lLTypeElemNum)
				{
					m_spLTypeElement = new SDXF_LTypeElement [m_lLTypeElemNum];
					if (m_spLTypeElement == NULL)
					{
						g_lDXFErrorCode = DXF_MEM_ERROR;
						return FALSE;
					}
					memset(m_spLTypeElement, 0x00, sizeof(SDXF_LTypeElement) * m_lLTypeElemNum);
				}
				break;
			case DXF_LTYPE_TOTALPATLEN:	// Total pattern length
				if (!DXFReadDouble(pFile, m_dTotPatLen))
				{
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_LTYPE_DASHLEN:	// Dash,dot or space length
				if (!DXFReadDouble(pFile, m_spLTypeElement[++lDashCnt].dDashLen))
				{
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_LTYPE_DASHTYPE:
				if (!DXFReadLong(pFile, m_spLTypeElement[lDashCnt].lDashType))
				{
					g_lDXFErrorCode = DXF_INVALID_DASHTYPE;
					return FALSE;
				}
				break;
			case DXF_LTYPE_SHAPECODE:
				if (!DXFReadLong(pFile, m_spLTypeElement[lDashCnt].lShapeCode))
				{
					g_lDXFErrorCode = DXF_INVALID_SHAPECODE;
					return FALSE;
				}
				break;
			case DXF_LTYPE_STYLEOBJPOINTER:
				if (!DXFReadHexLong(pFile, m_spLTypeElement[lDashCnt].lStyleObjPointer))
				{
					g_lDXFErrorCode = DXF_INVALID_HANDLE;
					return FALSE;
				}
				break;
			case DXF_LTYPE_DASHSCALE:
				if (!DXFReadDouble(pFile, m_spLTypeElement[lDashCnt].dScale))
				{
					g_lDXFErrorCode = DXF_INVALID_SCALE;
					return FALSE;
				}
				break;
			case DXF_LTYPE_DASHANGLE:
				if (!DXFReadDouble(pFile, m_spLTypeElement[lDashCnt].dAngle))
				{
					g_lDXFErrorCode = DXF_INVALID_ANGLE;
					return FALSE;
				}
				break;
			case DXF_LTYPE_DASHXOFFSET:
				if (!DXFReadDouble(pFile, m_spLTypeElement[lDashCnt].dXOffset))
				{
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_LTYPE_DASHYOFFSET:
				if (!DXFReadDouble(pFile, m_spLTypeElement[lDashCnt].dYOffset))
				{
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_LTYPE_STRING:
				if (!DXFReadString(pFile, m_spLTypeElement[lDashCnt].szText))
				{
					g_lDXFErrorCode = DXF_INVALID_VALUE;
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

