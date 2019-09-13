#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Table.h"
#include "DXF_BlockRecordTable.h"

#define		DXF_BLOCKRECORD_BLOCKNAME		2

CDXF_BlockRecordTable::CDXF_BlockRecordTable()
{
	m_nTableType = DXF_BLOCKRECORDTABLE;
	sprintf(m_pszBlockName, "%s", "");
}

CDXF_BlockRecordTable::CDXF_BlockRecordTable(DXF_CHAR* pszBlockName)
{
	m_nTableType = DXF_BLOCKRECORDTABLE;
	memcpy(m_pszBlockName, pszBlockName, MAX_STRING_LEN);
}

CDXF_BlockRecordTable::~CDXF_BlockRecordTable()
{

}

DXF_BOOL CDXF_BlockRecordTable::DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue)
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
			case DXF_BLOCKRECORD_BLOCKNAME:
				if (!DXFReadString(pFile, m_pszBlockName))
				{
					g_lDXFErrorCode = DXF_INVALID_BLOCKNAME;
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
