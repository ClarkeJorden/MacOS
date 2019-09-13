
#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Table.h"
#include "DXF_UCSTable.h"

#define		DXF_UCS_NAME			2
#define		DXF_UCS_CODE			70
#define		DXF_UCS_ORGPOINT		10
#define		DXF_UCS_XAXIS			11
#define		DXF_UCS_YAXIS			12

CDXF_UCSTable::CDXF_UCSTable()
{
	m_nTableType = DXF_UCSTABLE;

	sprintf(m_pszUCSName, "%s", "");
	m_lUCSCode = 0;
}

CDXF_UCSTable::~CDXF_UCSTable()
{
}

DXF_BOOL CDXF_UCSTable::DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue)
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
			case DXF_UCS_NAME:
				if (!DXFReadString(pFile, m_pszUCSName))
				{
					g_lDXFErrorCode = DXF_INVALID_UCSNAME;
					return FALSE;
				}
				break;
			case DXF_UCS_CODE:
				if (!DXFReadLong(pFile, m_lUCSCode))
				{
					g_lDXFErrorCode = DXF_INVALID_UCSCODE;
					return FALSE;
				}
				break;
			case DXF_UCS_ORGPOINT:
				if (!DXFReadPoint3D(pFile, DXF_UCS_ORGPOINT, m_sW3DOrgPoint))
				{
					g_lDXFErrorCode = DXF_INVALID_POINT3D;
					return FALSE;
				}
				break;
			case DXF_UCS_XAXIS:
				if (!DXFReadPoint3D(pFile, DXF_UCS_XAXIS, m_sW3DXAxis))
				{
					g_lDXFErrorCode = DXF_INVALID_POINT3D;
					return FALSE;
				}
				break;
			case DXF_UCS_YAXIS:
				if (!DXFReadPoint3D(pFile, DXF_UCS_YAXIS, m_sW3DYAxis))
				{
					g_lDXFErrorCode = DXF_INVALID_POINT3D;
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

