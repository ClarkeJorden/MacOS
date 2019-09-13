
#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Table.h"
#include "DXF_TablesSection.h"

#define		DXF_TABLES_COUNT		70
#define		DXF_TABLE_NAME			2

DXF_BOOL CDXF_TablesSection::blAddTable(CDXF_Table* cpDXFTable)
{
	switch (cpDXFTable->m_nTableType) {
		case DXF_APPIDTABLE:
			{
				CDXF_AppIDTable* cpAppIDTable = (CDXF_AppIDTable*)cpDXFTable;

				if (!blAddAppID(cpAppIDTable))
					return FALSE;
			}
			break;
		case DXF_BLOCKRECORDTABLE:
			{
				CDXF_BlockRecordTable* cpBlockRecordTable = (CDXF_BlockRecordTable*)cpDXFTable;

				if (!blAddBlockRecord(cpBlockRecordTable))
					return FALSE;
			}
			break;
		case DXF_DIMSTYLETABLE:
			{
				CDXF_DimStyleTable* cpDimStyleTable = (CDXF_DimStyleTable*)cpDXFTable;

				if (!blAddDimStyle(cpDimStyleTable))
					return FALSE;
			}
			break;
		case DXF_LAYERTABLE:
			{
				CDXF_LayerTable* cpLayerTable = (CDXF_LayerTable*)cpDXFTable;

				if (!blAddLayer(cpLayerTable))
					return FALSE;
			}
			break;
		case DXF_LTYPETABLE:
			{
				CDXF_LTypeTable* cpLTypeTable = (CDXF_LTypeTable*)cpDXFTable;

				if (!blAddLType(cpLTypeTable))
					return FALSE;
			}
			break;
		case DXF_STYLETABLE:
			{
				CDXF_StyleTable* cpStyleTable = (CDXF_StyleTable*)cpDXFTable;

				if (!blAddStyle(cpStyleTable))
					return FALSE;
			}
			break;
		case DXF_UCSTABLE:
			{
				CDXF_UCSTable* cpUCSTable = (CDXF_UCSTable*)cpDXFTable;

				if (!blAddUCS(cpUCSTable))
					return FALSE;
			}
			break;
		case DXF_VIEWTABLE:
			{
				CDXF_ViewTable* cpViewTable = (CDXF_ViewTable*)cpDXFTable;

				if (!blAddView(cpViewTable))
					return FALSE;
			}
			break;
		case DXF_VPORTTABLE:
			{
				CDXF_VPortTable* cpVPortTable = (CDXF_VPortTable*)cpDXFTable;

				if (!blAddVPort(cpVPortTable))
					return FALSE;
			}
			break;
		default:
			return FALSE;
			break;
	}

	return TRUE;
}

DXF_BOOL CDXF_TablesSection::blAddBlockRecord(CDXF_BlockRecordTable* cpBlockRecord)
{
	m_cDXFBlockRecordArray.Add(cpBlockRecord);

	return TRUE;
}

DXF_LONG CDXF_TablesSection::lGetBlockRecordCnt()
{
	return m_cDXFBlockRecordArray.GetSize();
}

CDXF_BlockRecordTable* CDXF_TablesSection::cpGetBlockRecord(DXF_LONG lIndex)
{
	DXF_LONG	lSize = m_cDXFBlockRecordArray.GetSize();

	if (lIndex < 0)
		return NULL;
	if (lIndex >= lSize)
		return NULL;
	return m_cDXFBlockRecordArray[lIndex];
}

DXF_BOOL CDXF_TablesSection::blRemoveBlockRecord()
{
	DXF_LONG	lSize = m_cDXFBlockRecordArray.GetSize();

	for (DXF_LONG i = 0; i < lSize; i++) {
		delete m_cDXFBlockRecordArray[i];
	}
	m_cDXFBlockRecordArray.RemoveAll();

	return TRUE;
}

DXF_BOOL CDXF_TablesSection::blAddStyle(CDXF_StyleTable* cpStyle)
{
// 	if( cpStyle->m_pszStyleName[0] == NULL )
// 		AfxMessageBox("Empty Style Name !");
// 	else
		m_cDXFStyleArray.Add(cpStyle);

	return TRUE;
}

DXF_LONG CDXF_TablesSection::lGetStyleCnt()
{
	return m_cDXFStyleArray.GetSize();
}

CDXF_StyleTable* CDXF_TablesSection::cpGetStyle(DXF_LONG lIndex)
{
	DXF_LONG	lSize = m_cDXFStyleArray.GetSize();

	if (lIndex < 0)
		return NULL;
	if (lIndex >= lSize)
		return NULL;
	return m_cDXFStyleArray[lIndex];
}

CDXF_StyleTable* CDXF_TablesSection::cpGetStyle(DXF_CHAR* pszName)
{
	DXF_LONG	i;
	DXF_LONG	lSize = m_cDXFStyleArray.GetSize();
	CDXF_StyleTable*	cpStyle = NULL;

	for (i = 0; i < lSize; i++)
	{
		cpStyle = m_cDXFStyleArray[i];
		if (strcmp(pszName, cpStyle->m_pszStyleName) == 0)
		{
			return cpStyle;
		}

	}
	return NULL;
}

DXF_LONG CDXF_TablesSection::lGetStyleIndex(DXF_LONG lHandle)
{
	DXF_LONG	i;
	DXF_LONG	lSize = m_cDXFStyleArray.GetSize();
	CDXF_StyleTable*	cpStyle = NULL;

	for (i = 0; i < lSize; i++)
	{
		cpStyle = m_cDXFStyleArray[i];
		if (lHandle == cpStyle->m_lHandleNumber)
		{
			return i;
		}

	}
	return -1;
}

DXF_BOOL CDXF_TablesSection::blRemoveStyle()
{
	DXF_LONG	lSize = m_cDXFStyleArray.GetSize();

	for (DXF_LONG i = 0; i < lSize; i++)
	{
		delete m_cDXFStyleArray[i];
	}
	m_cDXFStyleArray.RemoveAll();

	return TRUE;
}

DXF_BOOL CDXF_TablesSection::blAddVPort(CDXF_VPortTable* cpVPort)
{
	m_cDXFVPortArray.Add(cpVPort);

	return TRUE;
}

DXF_LONG CDXF_TablesSection::lGetVPortCnt()
{
	return m_cDXFVPortArray.GetSize();
}

CDXF_VPortTable* CDXF_TablesSection::cpGetVPort(DXF_LONG lIndex)
{
	DXF_LONG	lSize = m_cDXFVPortArray.GetSize();

	if (lIndex < 0)
		return NULL;
	if (lIndex >= lSize)
		return NULL;
	return m_cDXFVPortArray[lIndex];
}

DXF_BOOL CDXF_TablesSection::blRemoveVPort()
{
	DXF_LONG lSize = m_cDXFVPortArray.GetSize();

	for (DXF_LONG i = 0; i < lSize; i++)
	{
		delete m_cDXFVPortArray[i];
	}
	m_cDXFVPortArray.RemoveAll();

	return TRUE;
}

DXF_BOOL CDXF_TablesSection::blAddUCS(CDXF_UCSTable* cpUCS)
{
	m_cDXFUCSArray.Add(cpUCS);

	return TRUE;
}

DXF_LONG CDXF_TablesSection::lGetUCSCnt()
{
	return m_cDXFUCSArray.GetSize();
}

CDXF_UCSTable* CDXF_TablesSection::cpGetUCS(DXF_LONG lIndex)
{
	if (lIndex < 0 || lIndex >= m_cDXFUCSArray.GetSize())
		return NULL;

	return m_cDXFUCSArray[lIndex];
}

DXF_BOOL CDXF_TablesSection::blRemoveUCS()
{
	DXF_LONG lSize = m_cDXFUCSArray.GetSize();

	for (DXF_LONG i = 0; i < lSize; i++)
	{
		delete m_cDXFUCSArray[i];
	}
	m_cDXFUCSArray.RemoveAll();

	return TRUE;
}

DXF_BOOL CDXF_TablesSection::blAddView(CDXF_ViewTable* cpView)
{
	m_cDXFViewArray.Add(cpView);

	return TRUE;
}

DXF_LONG CDXF_TablesSection::lGetViewCnt()
{
	return m_cDXFViewArray.GetSize();
}

CDXF_ViewTable* CDXF_TablesSection::cpGetView(DXF_LONG lIndex)
{
	if (lIndex < 0 || lIndex >= m_cDXFViewArray.GetSize())
		return NULL;
	return m_cDXFViewArray[lIndex];
}

DXF_BOOL CDXF_TablesSection::blRemoveView()
{
	DXF_LONG lSize = m_cDXFViewArray.GetSize();

	for (DXF_LONG i = 0; i < lSize; i++)
	{
		delete m_cDXFViewArray[i];
	}
	m_cDXFViewArray.RemoveAll();

	return TRUE;
}

DXF_BOOL CDXF_TablesSection::blAddAppID(CDXF_AppIDTable* cpAppID)
{
	m_cDXFAppIDArray.Add(cpAppID);

	return TRUE;
}

DXF_LONG CDXF_TablesSection::lGetAppIDCnt()
{
	return m_cDXFAppIDArray.GetSize();
}

CDXF_AppIDTable* CDXF_TablesSection::cpGetAppID(DXF_LONG lIndex)
{
	DXF_LONG	lSize = m_cDXFAppIDArray.GetSize();

	if (lIndex < 0 || lIndex >= lSize)
		return NULL;

	return m_cDXFAppIDArray[lIndex];
}

DXF_BOOL CDXF_TablesSection::blRemoveAppID()
{
	DXF_LONG	lSize = m_cDXFAppIDArray.GetSize();

	for (DXF_LONG i = 0; i < lSize; i++)
	{
		delete m_cDXFAppIDArray[i];
	}
	m_cDXFAppIDArray.RemoveAll();

	return TRUE;
}

DXF_BOOL CDXF_TablesSection::blAddLType(CDXF_LTypeTable* cpLType)
{
	m_cDXFLTypeArray.Add(cpLType);

	return TRUE;
}

DXF_LONG CDXF_TablesSection::lGetLTypeCnt()
{
	return m_cDXFLTypeArray.GetSize();
}

CDXF_LTypeTable* CDXF_TablesSection::cpGetLType(DXF_LONG lIndex)
{
	if (lIndex < 0 || lIndex >= m_cDXFLTypeArray.GetSize())
		return NULL;
	return m_cDXFLTypeArray[lIndex];
}

DXF_BOOL CDXF_TablesSection::blRemoveLType()
{
	DXF_LONG lSize = m_cDXFLTypeArray.GetSize();

	for (DXF_LONG i = 0; i < lSize; i++)
	{
		delete m_cDXFLTypeArray[i];
	}
	m_cDXFLTypeArray.RemoveAll();

	return TRUE;
}
DXF_BOOL CDXF_TablesSection::blAddLayer(CDXF_LayerTable* cpLayer)
{
	m_cDXFLayerArray.Add(cpLayer);

	return TRUE;
}

DXF_LONG CDXF_TablesSection::lGetLayerCnt()
{
	return m_cDXFLayerArray.GetSize();
}

CDXF_LayerTable* CDXF_TablesSection::cpGetLayer(DXF_LONG lIndex)
{
	if (lIndex < 0 || lIndex >= m_cDXFLayerArray.GetSize())
		return NULL;
	return m_cDXFLayerArray[lIndex];
}

CDXF_LayerTable* CDXF_TablesSection::cpGetLayer(DXF_CHAR* pszLayerName)
{
	DXF_LONG	i, lSize = m_cDXFLayerArray.GetSize();
	CDXF_LayerTable* cpDXFLayer = NULL;

	for (i = 0; i < lSize; i++)
	{
		cpDXFLayer = m_cDXFLayerArray[i];
		if (strcmp(cpDXFLayer->m_pszLayerName, pszLayerName) == 0)
		{
			return cpDXFLayer;
		}
	}

	return NULL;
}

DXF_BOOL CDXF_TablesSection::blRemoveLayer()
{
	DXF_LONG lSize = m_cDXFLayerArray.GetSize();

	for (DXF_LONG i = 0; i < lSize; i++)
	{
		delete m_cDXFLayerArray[i];
	}
	m_cDXFLayerArray.RemoveAll();

	return TRUE;
}

DXF_BOOL CDXF_TablesSection::blAddDimStyle(CDXF_DimStyleTable* cpDimStyle)
{
	m_cDXFDimStyleArray.Add(cpDimStyle);

	return TRUE;
}

DXF_LONG CDXF_TablesSection::lGetDimStyleCnt()
{
	return m_cDXFDimStyleArray.GetSize();
}

CDXF_DimStyleTable* CDXF_TablesSection::cpGetDimStyle(DXF_LONG lIndex)
{
	if (lIndex < 0 || lIndex >= m_cDXFDimStyleArray.GetSize())
		return NULL;
	return m_cDXFDimStyleArray[lIndex];
}

DXF_BOOL CDXF_TablesSection::blRemoveDimStyle()
{
	DXF_LONG lSize = m_cDXFDimStyleArray.GetSize();

	for (DXF_LONG i = 0; i < lSize; i++)
	{
		delete m_cDXFDimStyleArray[i];
	}
	m_cDXFDimStyleArray.RemoveAll();

	return TRUE;
}

CDXF_TablesSection::CDXF_TablesSection()
{
	m_cDXFBlockRecordArray.RemoveAll();
	m_cDXFStyleArray.RemoveAll();
	m_cDXFVPortArray.RemoveAll();
	m_cDXFUCSArray.RemoveAll();
	m_cDXFViewArray.RemoveAll();
	m_cDXFAppIDArray.RemoveAll();
	m_cDXFLTypeArray.RemoveAll();
	m_cDXFLayerArray.RemoveAll();
	m_cDXFDimStyleArray.RemoveAll();
}

CDXF_TablesSection::~CDXF_TablesSection()
{
	blRemoveBlockRecord();
	blRemoveStyle();
	blRemoveVPort();
	blRemoveUCS();
	blRemoveView();
	blRemoveAppID();
	blRemoveLType();
	blRemoveLayer();
	blRemoveDimStyle();
}

DXF_BOOL CDXF_TablesSection::DXFReadTablesSection(FILE* pFile)
{
	DXF_LONG	lCode;
	DXF_CHAR	szValue[MAX_STRING_LEN];

	if (!DXFReadGroup(pFile, lCode, szValue))
	{
		g_lDXFErrorCode = DXF_INVALID_STRING;
		return FALSE;
	}
	do
	{
		if (lCode == DXF_BEGINNING_CODE)
		{
			/*
			if (!DXFReadString(pFile, szValue))
			{
				g_lDXFErrorCode = DXF_INVALID_STRING;
				return FALSE;
			}
			*/
			if (strcmp(szValue, "APPID") == 0)
			{
				CDXF_AppIDTable*	cpAppID;

				cpAppID = new CDXF_AppIDTable;
				if (cpAppID == NULL)
				{
					g_lDXFErrorCode = DXF_MEM_ERROR;
					return FALSE;
				}
				if (!cpAppID->DXFReadTable(pFile, lCode, szValue))
				{
					delete cpAppID;
					return FALSE;
				}
				if (!blAddAppID(cpAppID))
				{
					delete cpAppID;
					return FALSE;
				}
			}
			else if (strcmp(szValue, "BLOCK_RECORD") == 0)
			{
				CDXF_BlockRecordTable*	cpBlockRecord;

				cpBlockRecord = new CDXF_BlockRecordTable;
				if (cpBlockRecord == NULL)
				{
					g_lDXFErrorCode = DXF_MEM_ERROR;
					return FALSE;
				}
				if (!cpBlockRecord->DXFReadTable(pFile, lCode, szValue))
				{
					delete cpBlockRecord;
					return FALSE;
				}
				if (!blAddBlockRecord(cpBlockRecord))
				{
					delete cpBlockRecord;
					return FALSE;
				}
			}
			else if (strcmp(szValue, "UCS") == 0)
			{
				CDXF_UCSTable*	cpUCS;

				cpUCS = new CDXF_UCSTable;
				if (cpUCS == NULL)
				{
					g_lDXFErrorCode = DXF_MEM_ERROR;
					return FALSE;
				}
				if (!cpUCS->DXFReadTable(pFile, lCode, szValue))
				{
					delete cpUCS;
					return FALSE;
				}
				if (!blAddUCS(cpUCS))
				{
					delete cpUCS;
					return FALSE;
				}
			}
			else if (strcmp(szValue, "VPORT") == 0)
			{
				CDXF_VPortTable*	cpVPort;

				cpVPort = new CDXF_VPortTable;
				if (cpVPort == NULL)
				{
					g_lDXFErrorCode = DXF_MEM_ERROR;
					return FALSE;
				}
				if (!cpVPort->DXFReadTable(pFile, lCode, szValue))
				{
					delete cpVPort;
					return FALSE;
				}
				if (!blAddVPort(cpVPort))
				{
					delete cpVPort;
					return FALSE;
				}
			}
			else if (strcmp(szValue, "VIEW") == 0)
			{
				CDXF_ViewTable*	cpView;

				cpView = new CDXF_ViewTable;
				if (cpView == NULL)
				{
					g_lDXFErrorCode = DXF_MEM_ERROR;
					return FALSE;
				}
				if (!cpView->DXFReadTable(pFile, lCode, szValue))
				{
					delete cpView;
					return FALSE;
				}
				if (!blAddView(cpView))
				{
					delete cpView;
					return FALSE;
				}
			}
			else if (strcmp(szValue, "DIMSTYLE") == 0)
			{
				CDXF_DimStyleTable*	cpDimStyle;

				cpDimStyle = new CDXF_DimStyleTable;
				if (cpDimStyle == NULL)
				{
					g_lDXFErrorCode = DXF_MEM_ERROR;
					return FALSE;
				}
				if (!cpDimStyle->DXFReadTable(pFile, lCode, szValue))
				{
					delete cpDimStyle;
					return FALSE;
				}
				// ÃÏ´É¸ÜÂö¶¦ Indexµá °ä¾Ë
				{
					DXF_LONG	lIndex = lGetStyleIndex(cpDimStyle->m_lDIMTXSTY);

					if (lIndex < 0)
						return FALSE;
					cpDimStyle->m_lDIMTXSTY = lIndex;
				}
				if (!blAddDimStyle(cpDimStyle))
				{
					delete cpDimStyle;
					return FALSE;
				}
			}
			else if (strcmp(szValue, "STYLE") == 0)
			{
				CDXF_StyleTable*	cpStyle;

				cpStyle = new CDXF_StyleTable;
				if (cpStyle == NULL)
				{
					g_lDXFErrorCode = DXF_MEM_ERROR;
					return FALSE;
				}
				if (!cpStyle->DXFReadTable(pFile, lCode, szValue))
				{
					delete cpStyle;
					return FALSE;
				}
				if (!blAddStyle(cpStyle))
				{
					delete cpStyle;
					return FALSE;
				}
			}
			else if (strcmp(szValue, "LTYPE") == 0)
			{
				CDXF_LTypeTable*	cpLType;

				cpLType = new CDXF_LTypeTable;
				if (cpLType == NULL)
				{
					g_lDXFErrorCode = DXF_MEM_ERROR;
					return FALSE;
				}
				if (!cpLType->DXFReadTable(pFile, lCode, szValue))
				{
					delete cpLType;
					return FALSE;
				}
				if (!blAddLType(cpLType))
				{
					delete cpLType;
					return FALSE;
				}
			}
			else if (strcmp(szValue, "LAYER") == 0)
			{
				CDXF_LayerTable*	cpLayer;

				cpLayer = new CDXF_LayerTable;
				if (cpLayer == NULL)
				{
					g_lDXFErrorCode = DXF_MEM_ERROR;
					return FALSE;
				}
				if (!cpLayer->DXFReadTable(pFile, lCode, szValue))
				{
					delete cpLayer;
					return FALSE;
				}
				if (!blAddLayer(cpLayer))
				{
					delete cpLayer;
					return FALSE;
				}
			}
			else
			{
				if (!DXFReadGroup(pFile, lCode, szValue))
				{
					g_lDXFErrorCode = DXF_INVALID_STRING;
					return FALSE;
				}
			}
		}
		else
		{
			if (!DXFReadGroup(pFile, lCode, szValue))
			{
				g_lDXFErrorCode = DXF_INVALID_STRING;
				return FALSE;
			}
		}
	} while (strcmp(szValue, "ENDSEC") != 0);

	return TRUE;
}

