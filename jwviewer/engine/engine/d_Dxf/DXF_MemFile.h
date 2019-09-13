
#if !defined(AFX_DXFMEMFILE_H_)
#define AFX_DXFMEMFILE_H_


#include "DXF_Utils.h"
// Header Section
#include "DXF_HeaderSection.h"	// Added by ClassView

// Classes Section
#include "DXF_ClassesSection.h"	// Added by ClassView

// Tables Section
#include "DXF_Table.h"
#include "DXF_TablesSection.h"

// Entities Section
#include "DXF_EntitiesSection.h"

// Blocks Section
#include "DXF_BlocksSection.h"

// Object Section
#include "DXF_Object.h"
#include "DXF_ObjectsSection.h"

class CDXF_MemFile  
{
public:
	CDXF_MemFile();
	virtual ~CDXF_MemFile();

public:
	DXF_BOOL	DXFImport(char* szFileName);

	// Entities Section
	DXF_LONG		DXFGetEntityCnt();
	CDXF_Entity*	DXFGetEntity(DXF_LONG lIndex);

	// Blocks Section
	DXF_LONG		DXFGetBlockCnt();
	CDXF_Block*		DXFGetBlock(DXF_LONG lIndex);

	// Tables Section
	DXF_LONG		DXFGetLayerCnt();
	CDXF_LayerTable*	DXFGetLayer(DXF_LONG lIndex);
	DXF_LONG		DXFGetLTypeCnt();
	CDXF_LTypeTable*	DXFGetLType(DXF_LONG lIndex);
	DXF_LONG		DXFGetDimStyleCnt();
	CDXF_DimStyleTable*	DXFGetDimStyle(DXF_LONG lIndex);
	DXF_LONG		DXFGetStyleCnt();
	CDXF_StyleTable*	DXFGetStyle(DXF_LONG lIndex);
	DXF_LONG		DXFGetVPortCnt();
	CDXF_VPortTable*	DXFGetVPort(DXF_LONG lIndex);
	DXF_LONG		DXFGetUCSCnt();
	CDXF_UCSTable*		DXFGetUCS(DXF_LONG lIndex);
	DXF_LONG		DXFGetViewCnt();
	CDXF_ViewTable*		DXFGetView(DXF_LONG lIndex);
	DXF_LONG		DXFGetAppIDCnt();
	CDXF_AppIDTable*	DXFGetAppID(DXF_LONG lIndex);

	// Objects Section
	DXF_LONG		DXFGetGroupCnt();
//	CDXF_GroupObject*	DXFGetGroup(DXF_LONG lIndex);
	DXF_LONG		DXFGetMLineStyleCnt();

public:
	CDXF_HeaderSection*		m_cpDXFHeaderSection;
	CDXF_ClassesSection*	m_cpDXFClassesSection;
	CDXF_TablesSection*		m_cpDXFTablesSection;
	CDXF_EntitiesSection*	m_cpDXFEntitiesSection;
	CDXF_BlocksSection*		m_cpDXFBlocksSection;
	CDXF_ObjectsSection*	m_cpDXFObjectsSection;
};

#endif // !defined(AFX_DXFMEMFILE_H_)
