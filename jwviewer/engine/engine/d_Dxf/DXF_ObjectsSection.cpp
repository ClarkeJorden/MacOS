
#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Object.h"
#include "DXF_ObjectsSection.h"

DXF_LONG	g_lDXFGroupHandle;
DXF_LONG	g_lDXFImageDefHandle;
DXF_LONG	g_lDXFMLineStyleHandle;
DXF_LONG	g_lDXFLayoutHandle;

#define		DXF_DICTIONARY_NAME			3
#define		DXF_OBJECT_HANDLECODE		350

CDXF_ObjectsSection::CDXF_ObjectsSection()
{
	m_cDXFImageDefArray.RemoveAll();
// 	m_cDXFMLineStyleArray.RemoveAll();
// 	m_cDXFGroupArray.RemoveAll();
}

CDXF_ObjectsSection::~CDXF_ObjectsSection()
{
	DXF_LONG	i, lSize;

/*
	// Remove Group Objects
	lSize = m_cDXFGroupArray.GetSize();
	for (i = 0; i < lSize; i++)
	{
		delete m_cDXFGroupArray[i];
	}
	m_cDXFGroupArray.RemoveAll();
*/

	// Remove ImageDef Objects
	lSize = m_cDXFImageDefArray.GetSize();
	for (i = 0; i < lSize; i++)
	{
		delete m_cDXFImageDefArray[i];
	}
	m_cDXFImageDefArray.RemoveAll();

/*
	// Remove MLineStyle Objects
	lSize = m_cDXFMLineStyleArray.GetSize();
	for (i = 0; i < lSize; i++)
	{
		delete m_cDXFMLineStyleArray[i];
	}
	m_cDXFMLineStyleArray.RemoveAll();
*/
}
DXF_BOOL CDXF_ObjectsSection::blAddObject(CDXF_Object* cpDXFObject)
{
	switch (cpDXFObject->m_nObjectType) {
/*		case DXF_MLINESTYLEOBJECT:
			{
				CDXF_MLineStyleObject* cpMLineStyle = (CDXF_MLineStyleObject*)cpDXFObject;

				if (!blAddMLineStyle(cpMLineStyle))
					return FALSE;
			}
			break;

		case DXF_GROUPOBJECT:
			{
				CDXF_GroupObject* cpGroupObject = (CDXF_GroupObject*)cpDXFObject;

				if (!blAddGroup(cpGroupObject))
					return FALSE;
			}
			break;
*/
		case DXF_IMAGEDEFOBJECT:
			{
				CDXF_ImageDefObject* cpImageDef = (CDXF_ImageDefObject*)cpDXFObject;

				if (!blAddImageDef(cpImageDef))
					return FALSE;
			}
			break;
		default:
			return FALSE;
			break;
	}

	return TRUE;
}


DXF_BOOL CDXF_ObjectsSection::blAddImageDef(CDXF_ImageDefObject* cpImageDef)
{
	m_cDXFImageDefArray.Add(cpImageDef);

	return TRUE;
}


CDXF_ImageDefObject* CDXF_ObjectsSection::cpGetImageDef(DXF_LONG lIndex)
{
	DXF_LONG	lSize = m_cDXFImageDefArray.GetSize();

	if (lIndex < 0)
		return NULL;
	if (lIndex >= lSize)
		return NULL;
	return m_cDXFImageDefArray[lIndex];
}

DXF_BOOL CDXF_ObjectsSection::DXFReadObjectsCommonDic(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szString)
{
	if (!DXFReadGroupString(pFile, DXF_BEGINNING_CODE, szString))
	{
		return FALSE;
	}
	if (strcmp(szString, "DICTIONARY"))
	{
		g_lDXFErrorCode = DXF_INVALID_STRING;
		return FALSE;
	}
	while (lCode != 0)
	{
		if (!DXFReadGroup(pFile, lCode, szString))
		{
			return FALSE;
		}
		if (lCode == DXF_DICTIONARY_NAME && strcmp(szString, "ACAD_GROUP") == 0)
		{
			if (!DXFReadGroupHexLong(pFile, DXF_OBJECT_HANDLECODE, g_lDXFGroupHandle))
			{
				return FALSE;
			}
		}
		else if (lCode == DXF_DICTIONARY_NAME && strcmp(szString, "ACAD_MLINESTYLE") == 0)
		{
			if (!DXFReadGroupHexLong(pFile, DXF_OBJECT_HANDLECODE, g_lDXFMLineStyleHandle))
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

DXF_BOOL CDXF_ObjectsSection::DXFReadObjectsDic(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szString)
{
	DXF_LONG	lHandle;

	if (!DXFReadGroupHexLong(pFile, DXF_HANDLE_CODE, lHandle))
	{
		return FALSE;
	}
	lCode = DXF_HANDLE_CODE;

	if (lHandle == g_lDXFGroupHandle)
	{
		// Read Group Objects
		while (lCode != 0)
		{
			if (!DXFReadGroup(pFile, lCode, szString))
			{
				return FALSE;
			}
/*
			if (lCode == DXF_DICTIONARY_NAME)
			{
				CDXF_GroupObject* cpGroup = new CDXF_GroupObject;

				if (cpGroup == NULL)
				{
					g_lDXFErrorCode = DXF_MEM_ERROR;
					return FALSE;
				}
				strcpy(cpGroup->m_pszGroupName, szString);
				if (!DXFReadGroupHexLong(pFile, DXF_OBJECT_HANDLECODE, cpGroup->m_lHandleNumber))
				{
					delete cpGroup;
					return FALSE;
				}
				if (!blAddGroup(cpGroup))
				{
					delete cpGroup;
					return FALSE;
				}
			}
*/
		}
	}
	else if (lHandle == g_lDXFMLineStyleHandle)
	{
		// Read MLineStyle Objects
		while (lCode != 0)
		{
			if (!DXFReadGroup(pFile, lCode, szString))
			{
				return FALSE;
			}
/*
			if (lCode == DXF_DICTIONARY_NAME)
			{
				CDXF_MLineStyleObject* cpMLineStyle = new CDXF_MLineStyleObject;

				if (cpMLineStyle == NULL)
				{
					g_lDXFErrorCode = DXF_MEM_ERROR;
					return FALSE;
				}
				strcpy(cpMLineStyle->m_pszMLineStyleName, szString);
				if (!DXFReadGroupHexLong(pFile, DXF_OBJECT_HANDLECODE, cpMLineStyle->m_lHandleNumber))
				{
					delete cpMLineStyle;
					return FALSE;
				}
				if (!blAddMLineStyle(cpMLineStyle))
				{
					delete cpMLineStyle;
					return FALSE;
				}
			}
*/
		}
	}
	else
	{
		while (lCode)
		{
			if (!DXFReadGroup(pFile, lCode, szString))
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

DXF_BOOL CDXF_ObjectsSection::DXFReadObjectsSection(FILE *pFile)
{
	DXF_LONG	lCode = -1, lHandle;
	DXF_CHAR	szString[MAX_STRING_LEN];

	if (!DXFReadObjectsCommonDic(pFile, lCode, szString))
	{
		return FALSE;
	}
	while (lCode != DXF_BEGINNING_CODE || strcmp(szString, "ENDSEC") != 0)
	{
		if (lCode == DXF_BEGINNING_CODE && strcmp(szString, "DICTIONARY") == 0)
		{
			if (!DXFReadObjectsDic(pFile, lCode, szString))
			{
				return FALSE;
			}
		}
		else if (lCode == DXF_BEGINNING_CODE && strcmp(szString, "MLINESTYLE") == 0)
		{
			if (!DXFReadGroupHexLong(pFile, DXF_HANDLE_CODE, lHandle))
			{
				return FALSE;
			}

		}
		else if (lCode == DXF_BEGINNING_CODE && strcmp(szString, "IMAGEDEF") == 0)
		{
			CDXF_ImageDefObject*		cpImageDef;

			cpImageDef = new CDXF_ImageDefObject;
			if (cpImageDef == NULL)
			{
				g_lDXFErrorCode = DXF_MEM_ERROR;
				return FALSE;
			}
			if (!cpImageDef->DXFReadObject(pFile))
			{
				delete cpImageDef;
				return FALSE;
			}
			if (!blAddImageDef(cpImageDef))
			{
				delete cpImageDef;
				return FALSE;
			}
			if (!DXFReadString(pFile, szString))
			{
				g_lDXFErrorCode = DXF_INVALID_STRING;
				return FALSE;
			}
		}
/*
		else if (lCode == DXF_BEGINNING_CODE && strcmp(szString, "GROUP") == 0)
		{
			if (!DXFReadGroupHexLong(pFile, DXF_HANDLE_CODE, lHandle))
			{
				return FALSE;
			}

			CDXF_GroupObject* cpGroup = cpFindGroup(lHandle);
			if (cpGroup == NULL)
			{
				return FALSE;
			}
			if (!cpGroup->DXFReadObject(pFile))
			{
				delete cpGroup;
				return FALSE;
			}
			if (!DXFReadString(pFile, szString))
			{
				g_lDXFErrorCode = DXF_INVALID_STRING;
				return FALSE;
			}
		}
	*/	else
		{
			if (!DXFReadGroup(pFile, lCode, szString))
				return FALSE;
		}
	}

	return TRUE;
}
CDXF_ImageDefObject* CDXF_ObjectsSection::cpFindImageDef(DXF_LONG lHandle)
{
	DXF_LONG	lSize = m_cDXFImageDefArray.GetSize();

	for (DXF_LONG i = 0; i < lSize; i++)
	{
		CDXF_ImageDefObject*	cpImageDef = m_cDXFImageDefArray[i];
		if (cpImageDef != NULL && cpImageDef->m_lHandleNumber == lHandle)
		{
			return cpImageDef;
		}
	}
	return NULL;
}

CDXF_ImageDefObject* CDXF_ObjectsSection::cpFindImageDef(DXF_CHAR* pszFileName)
{
	DXF_LONG	lSize = m_cDXFImageDefArray.GetSize();

	for (DXF_LONG i = 0; i < lSize; i++)
	{
		CDXF_ImageDefObject* cpImageDef = m_cDXFImageDefArray[i];
		if (cpImageDef != NULL && strcmp(cpImageDef->m_pszImageFileName, pszFileName) == 0)
		{
			return cpImageDef;
		}
	}
	return NULL;
}


DXF_LONG  CDXF_ObjectsSection::lGetMLineStyleCnt()
{
	return 0;//m_cDXFMLineStyleArray.GetSize();
}

DXF_LONG  CDXF_ObjectsSection::lGetGroupCnt()
{
	return 0;//m_cDXFGroupArray.GetSize();
}

DXF_LONG  CDXF_ObjectsSection::lGetImageDefCnt()
{
	return m_cDXFImageDefArray.GetSize();
}
