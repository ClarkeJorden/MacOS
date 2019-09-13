#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Table.h"
#include "DXF_BlockRecordTable.h"
#include "DXF_TablesSection.h"
#include "DXF_Block.h"
#include "DXF_ObjectsSection.h"

#define		DXF_BLOCK_NAME			2
#define		DXF_BLOCK_FLAG			70
#define		DXF_BLOCK_INSERTPOINT	10
#define		DXF_BLOCK_NAME1			3
#define		DXF_BLOCK_XREFPATH		1

CDXF_Block::CDXF_Block()
{
	strcpy(m_pszLayerName, "0");
	strcpy(m_pszBlockName, "*MODEL_SPACE");
	m_lBlockCode = 0;
	strcpy(m_pszBlockName1, "");
	strcpy(m_pszXRefPath, "");
	strcpy(m_pszBlockDescription, "");

	m_cEntityArray.RemoveAll();
}

CDXF_Block::~CDXF_Block()
{
	DXF_LONG lSize = m_cEntityArray.GetSize();

	for (DXF_LONG i = 0; i < lSize; i++)
	{
		delete m_cEntityArray[i];
	}
	m_cEntityArray.RemoveAll();
}

DXF_BOOL CDXF_Block::DXFReadBlock(FILE *pFile)
{
	DXF_LONG	lCode;
	DXF_CHAR		szValue[MAX_STRING_LEN];

	if (!DXFReadLong(pFile, lCode)) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	do {
		switch (lCode) {	
			case DXF_HANDLE_CODE:		// handle number
				if (!DXFReadHexLong(pFile, m_lHandleNumber)) {
					g_lDXFErrorCode = DXF_INVALID_HANDLE;
					return FALSE;
				}
				break;
			case DXF_LAYER_CODE:		// layer name
				if (!DXFReadString(pFile, m_pszLayerName)) {
					g_lDXFErrorCode= DXF_INVALID_LAYER;
					return FALSE;
				}
				break;
			case DXF_BLOCK_NAME:		// block name
				if (!DXFReadString(pFile, m_pszBlockName)) {
					g_lDXFErrorCode = DXF_INVALID_BLOCKNAME;
					return FALSE;
				}
				break;
			case DXF_BLOCK_FLAG:	// flag
				if (!DXFReadLong(pFile, m_lBlockCode)) {
					g_lDXFErrorCode = DXF_INVALID_ENTITYCODE;
					return FALSE;
				}
				break;
			case DXF_BLOCK_INSERTPOINT:	// insert_x
				if (!DXFReadPoint3D(pFile, DXF_BLOCK_INSERTPOINT, m_sW3DInsertPoint)) {
					g_lDXFErrorCode = DXF_INVALID_INSERTPOINT;
					return FALSE;
				}
				break;
			case DXF_BLOCK_NAME1:		// block name
				if (!DXFReadString(pFile, m_pszBlockName1)) {
					g_lDXFErrorCode = DXF_INVALID_BLOCKNAME1;
					return FALSE;
				}
				break;
			case DXF_BLOCK_XREFPATH:		// Xref path - ignore
				if (!DXFReadString(pFile, m_pszXRefPath)) {
					g_lDXFErrorCode = DXF_INVALID_XREFPATH;
					return FALSE;
				}
				break;
			default:	// ignore
				if (!DXFReadString(pFile, szValue)) {
					g_lDXFErrorCode = DXF_INVALID_STRING;
					return FALSE;
				}
				break;
		}
		if (!DXFReadLong(pFile, lCode)) {
			g_lDXFErrorCode = DXF_INVALID_CODE;
			return FALSE;
		}
	} while (lCode != 0);

	// Read the entities associated with the block
	if (!DXFReadString(pFile, szValue)) {
		g_lDXFErrorCode = DXF_INVALID_STRING;
		return FALSE;
	}

	while (strcmp(szValue, "ENDBLK") != 0) {
		if (strcmp(szValue, "LINE") == 0) {
			CDXF_Line*	cpDXFLine = NULL;

			cpDXFLine = new CDXF_Line;
			if (cpDXFLine == NULL) {
				g_lDXFErrorCode = DXF_MEM_ERROR;
				return FALSE;
			}
			if (!cpDXFLine->DXFReadEntity(pFile)) {
				delete cpDXFLine;
				return FALSE;
			}
			if (!blAddEntity(cpDXFLine)) {
				delete cpDXFLine;
				return FALSE;
			}
		}
		else if (strcmp(szValue, "ARC") == 0) {
			CDXF_Arc*	cpDXFArc = NULL;

			cpDXFArc = new CDXF_Arc;
			if (cpDXFArc == NULL) {
				g_lDXFErrorCode = DXF_MEM_ERROR;
				return FALSE;
			}
			if (!cpDXFArc->DXFReadEntity(pFile)) {
				delete cpDXFArc;
				return FALSE;
			}
			if (!blAddEntity(cpDXFArc)) {
				delete cpDXFArc;
				return FALSE;
			}
		}
		else if (strcmp(szValue, "CIRCLE") == 0) {
			CDXF_Circle*	cpDXFCircle = NULL;

			cpDXFCircle = new CDXF_Circle;
			if (cpDXFCircle == NULL) {
				g_lDXFErrorCode = DXF_MEM_ERROR;
				return FALSE;
			}
			if (!cpDXFCircle->DXFReadEntity(pFile)) {
				delete cpDXFCircle;
				return FALSE;
			}
			if (!blAddEntity(cpDXFCircle)) {
				delete cpDXFCircle;
				return FALSE;
			}
		}
		else if (strcmp(szValue, "ELLIPSE") == 0) {
			CDXF_Ellipse*	cpDXFEllipse = NULL;

			cpDXFEllipse = new CDXF_Ellipse;
			if (cpDXFEllipse == NULL) {
				g_lDXFErrorCode = DXF_MEM_ERROR;
				return FALSE;
			}
			if (!cpDXFEllipse->DXFReadEntity(pFile)) {
				delete cpDXFEllipse;
				return FALSE;
			}
			if (!blAddEntity(cpDXFEllipse)) {
				delete cpDXFEllipse;
				return FALSE;
			}
		}
		else if (strcmp(szValue, "POINT") == 0) {
			CDXF_Point*	cpDXFPoint = NULL;

			cpDXFPoint = new CDXF_Point;
			if (cpDXFPoint == NULL) {
				g_lDXFErrorCode = DXF_MEM_ERROR;
				return FALSE;
			}
			if (!cpDXFPoint->DXFReadEntity(pFile)) {
				delete cpDXFPoint;
				return FALSE;
			}
			if (!blAddEntity(cpDXFPoint)) {
				delete cpDXFPoint;
				return FALSE;
			}
		}
		else if (strcmp(szValue, "TEXT") == 0) {
			CDXF_Text*	cpDXFText = NULL;

			cpDXFText = new CDXF_Text;
			if (cpDXFText == NULL) {
				g_lDXFErrorCode = DXF_MEM_ERROR;
				return FALSE;
			}
			if (!cpDXFText->DXFReadEntity(pFile)) {
				delete cpDXFText;
				return FALSE;
			}
			if (!blAddEntity(cpDXFText)) {
				delete cpDXFText;
				return FALSE;
			}
		}
/*
		else if (strcmp(szValue, "SPLINE") == 0) {
			CDXF_Spline*	cpDXFSpline = NULL;

			cpDXFSpline = new CDXF_Spline;
			if (cpDXFSpline == NULL) {
				g_lDXFErrorCode = DXF_MEM_ERROR;
				return FALSE;
			}
			if (!cpDXFSpline->DXFReadEntity(pFile)) {
				delete cpDXFSpline;
				return FALSE;
			}
			if (!blAddEntity(cpDXFSpline)) {
				delete cpDXFSpline;
				return FALSE;
			}
		}*/
		else if (strcmp(szValue, "IMAGE") == 0) {
			CDXF_Image*	cpDXFImage = NULL;

			cpDXFImage = new CDXF_Image;
			if (cpDXFImage == NULL) {
				g_lDXFErrorCode = DXF_MEM_ERROR;
				return FALSE;
			}
			if (!cpDXFImage->DXFReadEntity(pFile)) {
				delete cpDXFImage;
				return FALSE;
			}
			if (!blAddEntity(cpDXFImage)) {
				delete cpDXFImage;
				return FALSE;
			}
		}
		else {
			if (!DXFReadLong(pFile, lCode)) {
				g_lDXFErrorCode = DXF_INVALID_CODE;
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

DXF_BOOL CDXF_Block::blAddEntity(CDXF_Entity* cpEntity)
{
	m_cEntityArray.Add(cpEntity);

	return TRUE;
}

CDXF_Entity*	CDXF_Block::cpGetEntity(DXF_LONG nIndex)
{
	if (nIndex >= m_cEntityArray.GetSize())
		return NULL;
	if (nIndex < 0)
		return NULL;
	return m_cEntityArray[nIndex];
}

DXF_LONG CDXF_Block::lGetEntityCount()
{
	return m_cEntityArray.GetSize();
}

DXF_BOOL CDXF_Block::DXFReadBlockEntitySkip(FILE* pFile)
{
	DXF_LONG	lCode;
	DXF_CHAR	szValue[MAX_STRING_LEN];

	do {
		if (!DXFReadString(pFile, szValue)) {
			g_lDXFErrorCode = DXF_INVALID_STRING;
			return FALSE;
		}
		if (!DXFReadLong(pFile, lCode)) {
			g_lDXFErrorCode = DXF_INVALID_CODE;
			return FALSE;
		}
	} while (lCode != 0);

	return TRUE;
}
