#include "stdafx.h"

#include "DXF_MemFile.h"
#include "resource.h"

DXF_LONG		g_lDXFErrorSection = -1;
DXF_LONG		g_lDXFErrorCode = DXF_SUCCESS;
DXF_LONG		g_lDXFSysErrorCode = DXF_NONE_ERROR;
DXF_LONG		g_lDXFHandleNumber = 1;
DXF_LONG		g_lDXFFileLine = 0;
DXF_BOOL		g_blDXFImageExist = FALSE;

CDXF_HeaderSection*		g_cpDXFHeaderSection = NULL;
CDXF_ClassesSection*	g_cpDXFClassesSection = NULL;
CDXF_TablesSection*		g_cpDXFTablesSection = NULL;
CDXF_BlocksSection*		g_cpDXFBlocksSection = NULL;
CDXF_EntitiesSection*	g_cpDXFEntitiesSection = NULL;
CDXF_ObjectsSection*	g_cpDXFObjectsSection = NULL;

CDXF_MemFile::CDXF_MemFile()
{
	g_lDXFHandleNumber = 1L;
	g_lDXFFileLine = 0;
	g_blDXFImageExist = FALSE;

	m_cpDXFHeaderSection = new CDXF_HeaderSection;
	g_cpDXFHeaderSection = m_cpDXFHeaderSection;

	m_cpDXFClassesSection = new CDXF_ClassesSection;
	g_cpDXFClassesSection = m_cpDXFClassesSection;

	m_cpDXFTablesSection = new CDXF_TablesSection;
	g_cpDXFTablesSection = m_cpDXFTablesSection;

	m_cpDXFBlocksSection = new CDXF_BlocksSection;
	g_cpDXFBlocksSection = m_cpDXFBlocksSection;

	m_cpDXFEntitiesSection = new CDXF_EntitiesSection;
	g_cpDXFEntitiesSection = m_cpDXFEntitiesSection;

	m_cpDXFObjectsSection = new CDXF_ObjectsSection;
	g_cpDXFObjectsSection = m_cpDXFObjectsSection;
}

CDXF_MemFile::~CDXF_MemFile()
{
	if (m_cpDXFHeaderSection == NULL)
		delete m_cpDXFHeaderSection;
	if (m_cpDXFClassesSection == NULL)
		delete m_cpDXFClassesSection;
	if (m_cpDXFTablesSection == NULL)
		delete m_cpDXFTablesSection;
	if (m_cpDXFBlocksSection == NULL)
		delete m_cpDXFBlocksSection;
	if (m_cpDXFEntitiesSection == NULL)
		delete m_cpDXFEntitiesSection;
	if (m_cpDXFObjectsSection == NULL)
		delete m_cpDXFObjectsSection;
}

DXF_BOOL CDXF_MemFile::DXFImport(char* szFileName)
{
	FILE*	pFile = NULL;

	g_lDXFHandleNumber = 0L;
	g_lDXFErrorCode = 0L;

	DXF_BOOL blRet = TRUE;
	pFile = fopen(szFileName, "r");
	if (pFile == NULL)
	{
		g_lDXFErrorCode = DXF_ERROR_FILEOPEN;
		blRet = FALSE;
	}
	else
	{
		DXF_LONG	lCode;
		DXF_CHAR	szValue[MAX_STRING_LEN];

		while (strcmp(szValue, "EOF") != 0) {
			if (!DXFReadGroup(pFile, lCode, szValue)) {
				blRet = FALSE;
				break;
			}
			if (lCode == 2 && strcmp(szValue, "HEADER") == 0) {
				if (m_cpDXFHeaderSection == NULL) {
					g_lDXFErrorCode = DXF_MEM_ERROR;
					blRet = FALSE;
					break;
				}
				if (!m_cpDXFHeaderSection->DXFReadHeaderSection(pFile)) {
					g_lDXFErrorSection = DXF_HEADER_SECTION;
					blRet = FALSE;
					break;
				}
			}
			else if (lCode == 2 && strcmp(szValue, "CLASSES") == 0) {
				if (m_cpDXFClassesSection == NULL)
				{
					g_lDXFErrorCode = DXF_MEM_ERROR;
					blRet = FALSE;
					break;
				}
				if (!m_cpDXFClassesSection->DXFReadClassesSection(pFile)) {
					g_lDXFErrorSection = DXF_CLASSES_SECTION;
					blRet = FALSE;
					break;
				}
			}
			else if (lCode == 2 && strcmp(szValue, "TABLES") == 0) {
				if (m_cpDXFTablesSection == NULL) {
					g_lDXFErrorCode = DXF_MEM_ERROR;
					blRet = FALSE;
					break;
				}
				if (!m_cpDXFTablesSection->DXFReadTablesSection(pFile)) {
					g_lDXFErrorSection = DXF_TABLES_SECTION;
					blRet = FALSE;
					break;
				}
			}
			else if (lCode == 2 && strcmp(szValue, "BLOCKS") == 0) {
				if (m_cpDXFBlocksSection == NULL) {
					g_lDXFErrorCode = DXF_MEM_ERROR;
					blRet = FALSE;
					break;
				}
				if (!m_cpDXFBlocksSection->DXFReadBlocksSection(pFile)) {
					g_lDXFErrorSection = DXF_BLOCKS_SECTION;
					blRet = FALSE;
					break;
				}
			}
			else if (lCode == 2 && strcmp(szValue, "ENTITIES") == 0) {
				if (m_cpDXFEntitiesSection == NULL) {
					g_lDXFErrorCode = DXF_MEM_ERROR;
					blRet = FALSE;
					break;
				}
				if (!m_cpDXFEntitiesSection->DXFReadEntitiesSection(pFile)) {
					g_lDXFErrorSection = DXF_ENTITIES_SECTION;
					blRet = FALSE;
					break;
				}
			}
			else if (lCode == 2 && strcmp(szValue, "OBJECTS") == 0) {
				if (m_cpDXFObjectsSection == NULL) {
					g_lDXFErrorCode = DXF_MEM_ERROR;
					blRet = FALSE;
					break;
				}
				if (!m_cpDXFObjectsSection->DXFReadObjectsSection(pFile)) {
					g_lDXFErrorSection = DXF_OBJECTS_SECTION;
					blRet = FALSE;
					break;
				}
			}
		}
		fclose(pFile);
	}

	return blRet;
}

#define	DXF_FILE_CNT		6
#define	DXF_HEADER_FILE		0
#define	DXF_CLASSES_FILE	DXF_HEADER_FILE + 1
#define	DXF_TABLES_FILE		DXF_CLASSES_FILE + 1
#define	DXF_BLOCKS_FILE		DXF_TABLES_FILE + 1
#define	DXF_ENTITIES_FILE	DXF_BLOCKS_FILE + 1
#define	DXF_OBJECTS_FILE	DXF_ENTITIES_FILE + 1

void RemoveTmpFile(FILE **fpTempFile)
{
	DXF_CHAR	szName[256];

	for (int i = 0; i < DXF_FILE_CNT; i++){
		sprintf(szName, "$$$00%d", i);
		if (fpTempFile[i] != NULL)
			fclose(fpTempFile[i]);
		remove(szName);
	}
}
DXF_LONG CDXF_MemFile::DXFGetEntityCnt()
{
	return m_cpDXFEntitiesSection->lGetEntityCount();
}

CDXF_Entity* CDXF_MemFile::DXFGetEntity(DXF_LONG lIndex)
{
	CDXF_Entity* cpDXFEntity = m_cpDXFEntitiesSection->cpGetEntity(lIndex);

	if (cpDXFEntity == NULL)
		return NULL;

	if (cpDXFEntity->m_nEntityType == DXF_IMAGE) {
		CDXF_Image* DXFImage = (CDXF_Image*)cpDXFEntity;
		CDXF_ImageDefObject* DXFImageDef = m_cpDXFObjectsSection->cpFindImageDef(DXFImage->m_lImageDefHandle);
		if (DXFImageDef == NULL)
			return NULL;
		memcpy(DXFImage->m_pszImageName, DXFImageDef->m_pszImageFileName, MAX_STRING_LEN);
	}
	cpDXFEntity->DXFGenBCS();

	return cpDXFEntity;
}

DXF_LONG CDXF_MemFile::DXFGetBlockCnt()
{
	return m_cpDXFBlocksSection->lGetBlockCount();
}

CDXF_Block* CDXF_MemFile::DXFGetBlock(DXF_LONG lIndex)
{
	CDXF_Block*	DXFBlock = m_cpDXFBlocksSection->cpGetBlock(lIndex);

	if (DXFBlock == NULL)
		return NULL;

	DXF_LONG	i, lCount = DXFBlock->lGetEntityCount();

	for (i = 0; i < lCount; i++) {
		CDXF_Entity* cpDXFEntity = DXFBlock->cpGetEntity(i);

		if (cpDXFEntity == NULL)
			return NULL;
		if (cpDXFEntity->m_nEntityType == DXF_IMAGE) {
			CDXF_Image* DXFImage = (CDXF_Image*)cpDXFEntity;
			CDXF_ImageDefObject* DXFImageDef = m_cpDXFObjectsSection->cpFindImageDef(DXFImage->m_lImageDefHandle);
			if (DXFImageDef == NULL)
				return NULL;
			memcpy(DXFImage->m_pszImageName, DXFImageDef->m_pszImageFileName, MAX_STRING_LEN);
		}

		cpDXFEntity->DXFGenBCS();
	}

	return DXFBlock;
}

DXF_LONG CDXF_MemFile::DXFGetLayerCnt()
{
	return m_cpDXFTablesSection->lGetLayerCnt();
}

CDXF_LayerTable* CDXF_MemFile::DXFGetLayer(DXF_LONG lIndex)
{
	return m_cpDXFTablesSection->cpGetLayer(lIndex);
}

DXF_LONG CDXF_MemFile::DXFGetLTypeCnt()
{
	return m_cpDXFTablesSection->lGetLTypeCnt();
}

CDXF_LTypeTable* CDXF_MemFile::DXFGetLType(DXF_LONG lIndex)
{
	return m_cpDXFTablesSection->cpGetLType(lIndex);
}

DXF_LONG CDXF_MemFile::DXFGetDimStyleCnt()
{
	return m_cpDXFTablesSection->lGetDimStyleCnt();
}

CDXF_DimStyleTable* CDXF_MemFile::DXFGetDimStyle(DXF_LONG lIndex)
{
	return m_cpDXFTablesSection->cpGetDimStyle(lIndex);
}

DXF_LONG CDXF_MemFile::DXFGetStyleCnt()
{
	return m_cpDXFTablesSection->lGetStyleCnt();
}

CDXF_StyleTable* CDXF_MemFile::DXFGetStyle(DXF_LONG lIndex)
{
	return m_cpDXFTablesSection->cpGetStyle(lIndex);
}

DXF_LONG CDXF_MemFile::DXFGetVPortCnt()
{
	return m_cpDXFTablesSection->lGetVPortCnt();
}

CDXF_VPortTable* CDXF_MemFile::DXFGetVPort(DXF_LONG lIndex)
{
	return m_cpDXFTablesSection->cpGetVPort(lIndex);
}

DXF_LONG CDXF_MemFile::DXFGetUCSCnt()
{
	return m_cpDXFTablesSection->lGetUCSCnt();
}

CDXF_UCSTable* CDXF_MemFile::DXFGetUCS(DXF_LONG lIndex)
{
	return m_cpDXFTablesSection->cpGetUCS(lIndex);
}

DXF_LONG CDXF_MemFile::DXFGetViewCnt()
{
	return m_cpDXFTablesSection->lGetViewCnt();
}

CDXF_ViewTable* CDXF_MemFile::DXFGetView(DXF_LONG lIndex)
{
	return m_cpDXFTablesSection->cpGetView(lIndex);
}

DXF_LONG CDXF_MemFile::DXFGetAppIDCnt()
{
	return m_cpDXFTablesSection->lGetAppIDCnt();
}

CDXF_AppIDTable* CDXF_MemFile::DXFGetAppID(DXF_LONG lIndex)
{
	return m_cpDXFTablesSection->cpGetAppID(lIndex);
}

DXF_LONG CDXF_MemFile::DXFGetGroupCnt()
{
	return m_cpDXFObjectsSection->lGetGroupCnt();
}

