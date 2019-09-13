#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_EntitiesSection.h"

CDXF_EntitiesSection::CDXF_EntitiesSection()
{
	m_cDXFEntityArray.RemoveAll();
}

CDXF_EntitiesSection::~CDXF_EntitiesSection()
{
	DXF_LONG	lSize = m_cDXFEntityArray.GetSize();

	for (DXF_LONG i = 0; i < lSize; i++) {
		delete m_cDXFEntityArray[i];
	}
	m_cDXFEntityArray.RemoveAll();
}

DXF_BOOL CDXF_EntitiesSection::DXFReadEntitiesSection(FILE* pFile)
{
	DXF_LONG		lCode;
	DXF_CHAR		szValue[MAX_STRING_LEN];

	if (!DXFReadGroup(pFile, lCode, szValue))
		return FALSE;
	while (strcmp(szValue, "ENDSEC") != 0)
	{
		CDXF_Entity* cpDXFEntity = NULL;
		DXF_LONG lRetVal = DXFCreateEntity(szValue, &cpDXFEntity);
		if (lRetVal < 0)
		{
			return FALSE;
		}
		else if (lRetVal == 0)
		{
			if (!DXFReadEntitySkip(pFile))
				return FALSE;
		}
		else
		{
			if (!cpDXFEntity->DXFReadEntity(pFile))
			{
				delete cpDXFEntity;
				return FALSE;
			}
			if (!blAddEntity(cpDXFEntity))
			{
				delete cpDXFEntity;
				return FALSE;
			}
		}
		if (!DXFReadString(pFile, szValue)) {
			g_lDXFErrorCode = DXF_INVALID_STRING;
			return FALSE;
		}
	}

	return TRUE;
}

DXF_LONG CDXF_EntitiesSection::DXFCreateEntity(DXF_CHAR* pszType, CDXF_Entity** cpDXFEntity)
{
	if (strcmp(pszType, "LINE") == 0)
	{
		CDXF_Line* cpDXFLine = new CDXF_Line;
		if (cpDXFLine == NULL)
		{
			g_lDXFErrorCode = DXF_MEM_ERROR;
			return -1;
		}
		*cpDXFEntity = (CDXF_Entity*)cpDXFLine;
	}
	else if (strcmp(pszType, "ARC") == 0)
	{
		CDXF_Arc* cpDXFArc = new CDXF_Arc;
		if (cpDXFArc == NULL)
		{
			g_lDXFErrorCode = DXF_MEM_ERROR;
			return -1;
		}
		*cpDXFEntity = (CDXF_Entity*)cpDXFArc;
	}
	else if (strcmp(pszType, "CIRCLE") == 0)
	{
		CDXF_Circle* cpDXFCircle = new CDXF_Circle;
		if (cpDXFCircle == NULL)
		{
			g_lDXFErrorCode = DXF_MEM_ERROR;
			return -1;
		}
		*cpDXFEntity = (CDXF_Entity*)cpDXFCircle;
	}
	else if (strcmp(pszType, "ELLIPSE") == 0)
	{
		CDXF_Ellipse* cpDXFEllipse = new CDXF_Ellipse;
		if (cpDXFEllipse == NULL)
		{
			g_lDXFErrorCode = DXF_MEM_ERROR;
			return -1;
		}
		*cpDXFEntity = (CDXF_Entity*)cpDXFEllipse;
	}
	else if (strcmp(pszType, "POINT") == 0)
	{
		CDXF_Point*	cpDXFPoint = new CDXF_Point;
		if (cpDXFPoint == NULL)
		{
			g_lDXFErrorCode = DXF_MEM_ERROR;
			return -1;
		}
		*cpDXFEntity = (CDXF_Entity*)cpDXFPoint;
	}
	else if (strcmp(pszType, "TEXT") == 0)
	{
		CDXF_Text* cpDXFText = new CDXF_Text;
		if (cpDXFText == NULL)
		{
			g_lDXFErrorCode = DXF_MEM_ERROR;
			return -1;
		}
		*cpDXFEntity = (CDXF_Entity*)cpDXFText;
	}
// 	else if (strcmp(pszType, "SPLINE") == 0)
// 	{
// 		CDXF_Spline* cpDXFSpline = new CDXF_Spline;
// 		if (cpDXFSpline == NULL)
// 		{
// 			g_lDXFErrorCode = DXF_MEM_ERROR;
// 			return -1;
// 		}
// 		*cpDXFEntity = (CDXF_Entity*)cpDXFSpline;
// 	}
// 	else if (strcmp(pszType, "DIMENSION") == 0)
// 	{
// 		CDXF_Dimension*	cpDXFDimension = new CDXF_Dimension;
// 		if (cpDXFDimension == NULL)
// 		{
// 			g_lDXFErrorCode = DXF_MEM_ERROR;
// 			return -1;
// 		}
// 		*cpDXFEntity = (CDXF_Entity*)cpDXFDimension;
// 	}
	else if (strcmp(pszType, "IMAGE") == 0)
	{
		CDXF_Image*	cpDXFImage = new CDXF_Image;
		if (cpDXFImage == NULL)
		{
			g_lDXFErrorCode = DXF_MEM_ERROR;
			return -1;
		}
		*cpDXFEntity = (CDXF_Entity*)cpDXFImage;
	}
	else
	{
		*cpDXFEntity = NULL;
		return 0;
	}

	return 1;
}

DXF_BOOL CDXF_EntitiesSection::DXFReadEntitySkip(FILE *pFile)
{
	DXF_LONG		lCode;
	DXF_CHAR		szValue[MAX_STRING_LEN];

	do {
		if (!DXFReadString(pFile, szValue))
			return FALSE;
		if (!DXFReadLong(pFile, lCode))
			return FALSE;
	} while (lCode != 0);

	return TRUE;
}

DXF_BOOL CDXF_EntitiesSection::blAddEntity(CDXF_Entity* cpEntity)
{
	m_cDXFEntityArray.Add(cpEntity);

	return TRUE;
}

CDXF_Entity* CDXF_EntitiesSection::cpGetEntity(DXF_LONG lIndex)
{
	DXF_LONG	lSize = m_cDXFEntityArray.GetSize();

	if (lIndex >= lSize)
		return NULL;
	if (lIndex < 0)
		return NULL;

	return m_cDXFEntityArray[lIndex];
}

CDXF_Entity* CDXF_EntitiesSection::cpGetEntityWithHandle(DXF_LONG lHandle)
{
	DXF_LONG	lSize = m_cDXFEntityArray.GetSize();

	for (DXF_LONG i = 0; i < lSize; i++) {
		CDXF_Entity* cpDXFEntity = m_cDXFEntityArray[i];

		if (cpDXFEntity != NULL && cpDXFEntity->m_lHandleNumber == lHandle)
			return cpDXFEntity;;
	}

	return NULL;
}

DXF_LONG CDXF_EntitiesSection::lGetEntityIdxWithHandle(DXF_LONG lHandle)
{
	DXF_LONG	lSize = m_cDXFEntityArray.GetSize();

	for (DXF_LONG i = 0; i < lSize; i++) {
		CDXF_Entity* cpDXFEntity = m_cDXFEntityArray[i];

		if (cpDXFEntity != NULL && cpDXFEntity->m_lHandleNumber == lHandle)
			return i;
	}

	return -1;
}

DXF_LONG CDXF_EntitiesSection::lGetEntityCount()
{
	return m_cDXFEntityArray.GetSize();
}
