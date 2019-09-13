#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Table.h"
#include "DXF_ViewTable.h"

#define		DXF_VIEW_NAME				2
#define		DXF_VIEW_CODE				70
#define		DXF_VIEW_CENTER				10
#define		DXF_VIEW_HEIGHT				40
#define		DXF_VIEW_WIDTH				41
#define		DXF_VIEW_LENSLEN			42
#define		DXF_VIEW_FORECLIPLEN		43
#define		DXF_VIEW_BACKCLIPLEN		44
#define		DXF_VIEW_TWISTANGLE			50
#define		DXF_VIEW_DIR				11
#define		DXF_VIEW_TARGET				12
#define		DXF_VIEW_MODE				71

CDXF_ViewTable::CDXF_ViewTable()
{
	m_nTableType = DXF_VIEWTABLE;

	sprintf(m_pszViewName, "%s", "");
	m_lViewCode = 0;
	m_dViewHeight = 0.0;
	m_dViewWidth = 0.0;
	m_dLensLen = 0.0;
	m_dForeClipLen = 0.0;
	m_dBackClipLen = 0.0;
	m_dTwistAngle = 0.0;
	m_lViewMode = 0;
}

CDXF_ViewTable::~CDXF_ViewTable()
{

}

DXF_BOOL CDXF_ViewTable::DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue)
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
			case DXF_VIEW_NAME:
				if (!DXFReadString(pFile, m_pszViewName))
				{
					g_lDXFErrorCode = DXF_INVALID_VIEWNAME;
					return FALSE;
				}
				break;
			case DXF_VIEW_CODE:
				if (!DXFReadLong(pFile, m_lViewCode))
				{
					g_lDXFErrorCode = DXF_INVALID_VIEWMODE;
					return FALSE;
				}
				break;
			case DXF_VIEW_CENTER:
				if (!DXFReadPoint2D(pFile, DXF_VIEW_CENTER, m_sW2DViewCenter))
				{
					g_lDXFErrorCode = DXF_INVALID_POINT2D;
					return FALSE;
				}
				break;
			case DXF_VIEW_HEIGHT:
				if (!DXFReadDouble(pFile, m_dViewHeight))
				{
					g_lDXFErrorCode = DXF_INVALID_VIEWHEIGHT;
					return FALSE;
				}
				break;
			case DXF_VIEW_WIDTH:
				if (!DXFReadDouble(pFile, m_dViewWidth))
				{
					g_lDXFErrorCode = DXF_INVALID_VIEWWIDTH;
					return FALSE;
				}
				break;
			case DXF_VIEW_LENSLEN:
				if (!DXFReadDouble(pFile, m_dLensLen))
				{
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_VIEW_FORECLIPLEN:
				if (!DXFReadDouble(pFile, m_dForeClipLen))
				{
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_VIEW_BACKCLIPLEN:
				if (!DXFReadDouble(pFile, m_dBackClipLen))
				{
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case DXF_VIEW_TWISTANGLE:
				if (!DXFReadDouble(pFile, m_dTwistAngle))
				{
					g_lDXFErrorCode = DXF_INVALID_ANGLE;
					return FALSE;
				}
				break;
			case DXF_VIEW_DIR:
				if (!DXFReadPoint3D(pFile, DXF_VIEW_DIR, m_sW3DViewDir))
				{
					g_lDXFErrorCode = DXF_INVALID_POINT3D;
					return FALSE;
				}
				break;
			case DXF_VIEW_TARGET:
				if (!DXFReadPoint3D(pFile, DXF_VIEW_TARGET, m_sW3DTarget))
				{
					g_lDXFErrorCode = DXF_INVALID_POINT3D;
					return FALSE;
				}
				break;
			case DXF_VIEW_MODE:
				if (!DXFReadLong(pFile, m_lViewMode))
				{
					g_lDXFErrorCode = DXF_INVALID_VIEWMODE;
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

