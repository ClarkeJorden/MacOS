#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Table.h"
#include "DXF_LayerTable.h"

#define		DXF_LAYER_NAME		2
#define		DXF_LAYER_FLAG		70
#define		DXF_LAYER_LTYPE		6

CDXF_LayerTable::CDXF_LayerTable()
{
	m_nTableType = DXF_LAYERTABLE;
	memcpy(m_pszLayerName, "0", MAX_STRING_LEN);
	m_lLayerCode = 0;
	m_lLayerColor = 7;
	memcpy(m_pszLineTypeName, "CONTINUOUS", MAX_STRING_LEN);
}

CDXF_LayerTable::~CDXF_LayerTable()
{

}

DXF_BOOL CDXF_LayerTable::DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue)
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
			case DXF_LAYER_NAME:
				if (!DXFReadString(pFile, m_pszLayerName))
				{
					g_lDXFErrorCode = DXF_INVALID_LAYERNAME;
					return FALSE;
				}
				break;
			case DXF_LAYER_FLAG:
				if (!DXFReadLong(pFile, m_lLayerCode))
				{
					g_lDXFErrorCode = DXF_INVALID_LAYERCODE;
					return FALSE;
				}
				break;
			case DXF_COLOR_CODE:
				if (!DXFReadLong(pFile, m_lLayerColor))
				{
					g_lDXFErrorCode = DXF_INVALID_LAYERCOLOR;
					return FALSE;
				}
				break;
			case DXF_LAYER_LTYPE:
				if (!DXFReadString(pFile, m_pszLineTypeName))
				{
					g_lDXFErrorCode = DXF_INVALID_LINETYPE;
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

