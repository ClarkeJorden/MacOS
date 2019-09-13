#if !defined(AFX_CDXF_TablesSection_H_)
#define AFX_CDXF_TablesSection_H_

#include "DXF_AppIDTable.h"
#include "DXF_BlockRecordTable.h"
#include "DXF_DimStyleTable.h"
#include "DXF_LTypeTable.h"
#include "DXF_LayerTable.h"
#include "DXF_StyleTable.h"
#include "DXF_UCSTable.h"
#include "DXF_ViewTable.h"
#include "DXF_VPortTable.h"

class CDXF_TablesSection  
{
public:
	CDXF_TablesSection();
	virtual ~CDXF_TablesSection();

public:
	DXF_BOOL DXFReadTablesSection(FILE* pFile);

	DXF_BOOL	blAddTable(CDXF_Table* cpDXFTable);

public:
	DXF_BOOL	blAddAppID(CDXF_AppIDTable* cpAppID);
	DXF_BOOL	blAddBlockRecord(CDXF_BlockRecordTable* cpBlockRecord);
	DXF_BOOL	blAddDimStyle(CDXF_DimStyleTable* cpDimStyle);
	DXF_BOOL	blAddLayer(CDXF_LayerTable* cpLayer);
	DXF_BOOL	blAddLType(CDXF_LTypeTable* cpLType);
	DXF_BOOL	blAddStyle(CDXF_StyleTable* cpStyle);
	DXF_BOOL	blAddVPort(CDXF_VPortTable* cpVPort);
	DXF_BOOL	blAddUCS(CDXF_UCSTable* cpUCS);
	DXF_BOOL	blAddView(CDXF_ViewTable* cpView);

public:
	// BlockRecord Table
	DXF_LONG	lGetBlockRecordCnt();
	CDXF_BlockRecordTable*	cpGetBlockRecord(DXF_LONG nIndex);

	// Style Table
	DXF_LONG	lGetStyleCnt();
	CDXF_StyleTable*	cpGetStyle(DXF_LONG nIndex);
	CDXF_StyleTable*	cpGetStyle(DXF_CHAR* pszName);
	DXF_LONG			lGetStyleIndex(DXF_LONG lHandle);

	// VPort Table
	DXF_LONG	lGetVPortCnt();
	CDXF_VPortTable*		cpGetVPort(DXF_LONG nIndex);

	// UCS Table
	DXF_LONG	lGetUCSCnt();
	CDXF_UCSTable*		cpGetUCS(DXF_LONG nIndex);

	// View Table
	DXF_LONG	lGetViewCnt();
	CDXF_ViewTable*		cpGetView(DXF_LONG nIndex);

	// AppID Table
	DXF_LONG	lGetAppIDCnt();
	CDXF_AppIDTable*		cpGetAppID(DXF_LONG nIndex);

	// LType Table
	DXF_LONG	lGetLTypeCnt();
	CDXF_LTypeTable*		cpGetLType(DXF_LONG nIndex);

	// Layer Table
	DXF_LONG	lGetLayerCnt();
	CDXF_LayerTable*		cpGetLayer(DXF_LONG nLayerIndex);
	CDXF_LayerTable*		cpGetLayer(DXF_CHAR* pszLayerName);

	// DimStyle Table
	DXF_LONG	lGetDimStyleCnt();
	CDXF_DimStyleTable*		cpGetDimStyle(DXF_LONG nIndex);

protected:
	CDXF_AppIDArray		m_cDXFAppIDArray;
	DXF_BOOL	blRemoveAppID();

	CDXF_BlockRecordArray	m_cDXFBlockRecordArray;
	DXF_BOOL	blRemoveBlockRecord();

	CDXF_StyleArray		m_cDXFStyleArray;
	DXF_BOOL	blRemoveStyle();

	CDXF_VPortArray		m_cDXFVPortArray;
	DXF_BOOL	blRemoveVPort();

	CDXF_UCSArray		m_cDXFUCSArray;
	DXF_BOOL	blRemoveUCS();

	CDXF_ViewArray		m_cDXFViewArray;
	DXF_BOOL	blRemoveView();

	CDXF_LTypeArray		m_cDXFLTypeArray;
	DXF_BOOL	blRemoveLType();

	CDXF_LayerArray		m_cDXFLayerArray;
	DXF_BOOL	blRemoveLayer();

	CDXF_DimStyleArray	m_cDXFDimStyleArray;
	DXF_BOOL	blRemoveDimStyle();
};

#endif // !defined(AFX_CDXF_TablesSection_H_)
