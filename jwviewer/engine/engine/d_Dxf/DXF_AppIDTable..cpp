#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Table.h"
#include "DXF_AppIDTable.h"

#define		DXF_APPID_APPNAME		2
#define		DXF_APPID_STANDCODE		70

CDXF_AppIDTable::CDXF_AppIDTable()
{
	m_nTableType = DXF_APPIDTABLE;
	memcpy(m_pszAppName, "ACAD", MAX_STRING_LEN);
	m_lStdCode = 0;
}

CDXF_AppIDTable::~CDXF_AppIDTable()
{

}

DXF_BOOL CDXF_AppIDTable::DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue)
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
			case DXF_APPID_APPNAME:
				if (!DXFReadString(pFile, m_pszAppName))
				{
					g_lDXFErrorCode = DXF_INVALID_APPNAME;
					return FALSE;
				}
				break;
			case DXF_APPID_STANDCODE:
				if (!DXFReadLong(pFile, m_lStdCode))
				{
					g_lDXFErrorCode = DXF_INVALID_ENTITYCODE;
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

