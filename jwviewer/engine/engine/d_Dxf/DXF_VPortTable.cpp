#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Table.h"
#include "DXF_VPortTable.h"

#define		DXF_VPORT_NAME					2
#define		DXF_VPORT_CODE					70
#define		DXF_VPORT_LOWERLEFTCORNER		10
#define		DXF_VPORT_UPRIGHTCORNER			11
#define		DXF_VPORT_VIEWCENTER			12
#define		DXF_VPORT_SNAPBASEPOINT			13
#define		DXF_VPORT_SNAPSPACING			14
#define		DXF_VPORT_GRIDSPACING			15
#define		DXF_VPORT_VIEWDIR				16
#define		DXF_VPORT_VIEWTARGET			17
#define		DXF_VPORT_VIEWHEIGHT			40
#define		DXF_VPORT_ASPECTRATIO			41
#define		DXF_VPORT_LENSLEN				42
#define		DXF_VPORT_FRONTCLIPPLANE		43
#define		DXF_VPORT_BACKCLIPPLANE			44
#define		DXF_VPORT_SNAPROTATEANGLE		50
#define		DXF_VPORT_VIEWTWISTANGLE		51
#define		DXF_VPORT_VIEWMODE				71
#define		DXF_VPORT_CIRCLEZOOMPERCENT		72
#define		DXF_VPORT_FASTZOOMSETTING		73
#define		DXF_VPORT_UCSICONSETTING		74
#define		DXF_VPORT_SNAPONOFF				75
#define		DXF_VPORT_GRIDONOFF				76
#define		DXF_VPORT_SNAPSTYLE				77
#define		DXF_VPORT_ISOPAIR				78

CDXF_VPortTable::CDXF_VPortTable()
{
	m_nTableType = DXF_VPORTTABLE;

	memset(m_pszVPortName, 0x00, MAX_STRING_LEN);
	m_lStdCode = 0;				// 70
	m_dViewHeight = 0.0;			// 40
	m_dVPortAspectRatio = 0.0;	// 41
	m_dLensLen = 0.0;				// 42
	m_dFrontClipPlane = 0.0;
	m_dBackClipPlane = 0.0;
	m_dSnapRotateAngle = 0.0;
	m_dViewTwistAngle = 0.0;
	m_lViewMode = 0;
	m_lCircleZoomPercent = 0;
	m_lFastZoomSetting = 0;
	m_lUCSICONSetting = 0;
	m_lSnapOnOff = 0;
	m_lGridOnOff = 0;
	m_lSnapStyle = 0;
	m_lIsopair = 0;
}

CDXF_VPortTable::~CDXF_VPortTable()
{

}

DXF_BOOL CDXF_VPortTable::DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue)
{
	if (!DXFReadLong(pFile, lCode)) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}

	while (lCode) {
		switch (lCode) {
			case DXF_VPORT_NAME:
				if (!DXFReadString(pFile, m_pszVPortName)) {
					g_lDXFErrorCode = DXF_INVALID_VPORTNAME;
					return FALSE;
				}
				break;
			case DXF_VPORT_CODE:
				if (!DXFReadLong(pFile, m_lStdCode)) {
					g_lDXFErrorCode = DXF_INVALID_VPORTCODE;
					return FALSE;
				}
				break;
			case DXF_VPORT_LOWERLEFTCORNER:
				if (!DXFReadPoint2D(pFile, DXF_VPORT_LOWERLEFTCORNER, m_sW2DLowerLeftCorner)) {
					g_lDXFErrorCode = DXF_INVALID_POINT2D;
					return FALSE;
				}
				break;
			case DXF_VPORT_UPRIGHTCORNER:
				if (!DXFReadPoint2D(pFile, DXF_VPORT_UPRIGHTCORNER, m_sW2DUpperRightCorner)) {
					g_lDXFErrorCode = DXF_INVALID_POINT2D;
					return FALSE;
				}
				break;
			case DXF_VPORT_VIEWCENTER:
				if (!DXFReadPoint2D(pFile, DXF_VPORT_VIEWCENTER, m_sW2DViewCenter)) {
					g_lDXFErrorCode = DXF_INVALID_POINT2D;
					return FALSE;
				}
				break;
			case DXF_VPORT_SNAPBASEPOINT:
				if (!DXFReadPoint2D(pFile, DXF_VPORT_SNAPBASEPOINT, m_sW2DSnapBasePoint)) {
					g_lDXFErrorCode = DXF_INVALID_POINT2D;
					return FALSE;
				}
				break;
			case DXF_VPORT_SNAPSPACING:
				if (!DXFReadPoint2D(pFile, DXF_VPORT_SNAPSPACING, m_sW2DSnapSpacing)) {
					g_lDXFErrorCode = DXF_INVALID_POINT2D;
					return FALSE;
				}
				break;
			case DXF_VPORT_GRIDSPACING:
				if (!DXFReadPoint2D(pFile, DXF_VPORT_GRIDSPACING, m_sW2DGridSpacing)) {
					g_lDXFErrorCode = DXF_INVALID_POINT2D;
					return FALSE;
				}
				break;
			case DXF_VPORT_VIEWDIR:
				if (!DXFReadPoint2D(pFile, DXF_VPORT_VIEWDIR, m_sW2DViewDirFromTargetPoint)) {
					g_lDXFErrorCode = DXF_INVALID_POINT2D;
					return FALSE;
				}
				break;
			case DXF_VPORT_VIEWTARGET:
				if (!DXFReadPoint3D(pFile, DXF_VPORT_VIEWTARGET, m_sW3DViewTargetPoint)) {
					g_lDXFErrorCode = DXF_INVALID_POINT3D;
					return FALSE;
				}
				break;
			case DXF_VPORT_VIEWHEIGHT:
				if (!DXFReadDouble(pFile, m_dViewHeight)) {
					return FALSE;
				}
				break;
			case DXF_VPORT_ASPECTRATIO:
				if (!DXFReadDouble(pFile, m_dVPortAspectRatio)) {
					return FALSE;
				}
				break;
			case DXF_VPORT_LENSLEN:
				if (!DXFReadDouble(pFile, m_dLensLen)) {
					return FALSE;
				}
				break;
			case DXF_VPORT_FRONTCLIPPLANE:
				if (!DXFReadDouble(pFile, m_dFrontClipPlane)) {
					return FALSE;
				}
				break;
			case DXF_VPORT_BACKCLIPPLANE:
				if (!DXFReadDouble(pFile, m_dBackClipPlane)) {
					return FALSE;
				}
				break;
			case DXF_VPORT_SNAPROTATEANGLE:
				if (!DXFReadDouble(pFile, m_dSnapRotateAngle)) {
					g_lDXFErrorCode = DXF_INVALID_ANGLE;
					return FALSE;
				}
				break;
			case DXF_VPORT_VIEWTWISTANGLE:
				if (!DXFReadDouble(pFile, m_dViewTwistAngle)) {
					g_lDXFErrorCode = DXF_INVALID_ANGLE;
					return FALSE;
				}
				break;
			case DXF_VPORT_VIEWMODE:
				if (!DXFReadLong(pFile, m_lViewMode)) {
					return FALSE;
				}
				break;
			case DXF_VPORT_CIRCLEZOOMPERCENT:
				if (!DXFReadLong(pFile, m_lCircleZoomPercent)) {
					return FALSE;
				}
				break;
			case DXF_VPORT_FASTZOOMSETTING:
				if (!DXFReadLong(pFile, m_lFastZoomSetting)) {
					return FALSE;
				}
				break;
			case DXF_VPORT_UCSICONSETTING:
				if (!DXFReadLong(pFile, m_lUCSICONSetting)) {
					return FALSE;
				}
				break;
			case DXF_VPORT_SNAPONOFF:
				if (!DXFReadLong(pFile, m_lSnapOnOff)) {
					return FALSE;
				}
				break;
			case DXF_VPORT_GRIDONOFF:
				if (!DXFReadLong(pFile, m_lGridOnOff)) {
					return FALSE;
				}
				break;
			case DXF_VPORT_SNAPSTYLE:
				if (!DXFReadLong(pFile, m_lSnapStyle)) {
					return FALSE;
				}
				break;
			case DXF_VPORT_ISOPAIR:
				if (!DXFReadLong(pFile, m_lIsopair)) {
					return FALSE;
				}
				break;
			default:
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
	}
	
	if (!DXFReadString(pFile, szValue))
	{
		g_lDXFErrorCode = DXF_INVALID_STRING;
		return FALSE;
	}

	return TRUE;
}

