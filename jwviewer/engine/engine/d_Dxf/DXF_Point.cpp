#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Entity.h"
#include "DXF_Point.h"

#define		DXF_POINT_COORDINATE		10
#define		DXF_POINT_ANGLE				50

CDXF_Point::CDXF_Point()
{
	m_nEntityType = DXF_POINT;
	m_dThickness = 0.0;	// default = 0.0
	m_dAngle = 0.0;
}

CDXF_Point::~CDXF_Point()
{

}

DXF_BOOL CDXF_Point::DXFReadEntity(FILE *pFile)
{
	DXF_LONG		lCode;
	DXF_CHAR		szValue[MAX_STRING_LEN];

	g_lDXFErrorCode = DXF_SUCCESS;

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
			case DXF_SPACE_CODE:	// modelspace ? paperspace?
				if (!DXFReadLong(pFile, m_lSpaceType)) {
					g_lDXFErrorCode = DXF_INVALID_MPSPACE;
					return FALSE;
				}
				break;
			case DXF_LAYER_CODE:		// layer name
				if (!DXFReadString(pFile, m_pszLayerName)) {
					g_lDXFErrorCode = DXF_INVALID_LAYER;
					return FALSE;
				}
				break;
			case DXF_LTYPE_CODE:		// line type name
				if (!DXFReadString(pFile, m_pszLTypeName)) {
					g_lDXFErrorCode = DXF_INVALID_LINETYPE;
					return FALSE;
				}
				break;
			case DXF_COLOR_CODE:	// color index
				if (!DXFReadLong(pFile, m_lDXFColor)) {
					g_lDXFErrorCode = DXF_INVALID_COLOR;
					return FALSE;
				}
				break;
			case DXF_LTYPESCALE_CODE:	// line style scale
				if (!DXFReadDouble(pFile, m_dLTypeScale)) {
					g_lDXFErrorCode = DXF_INVALID_LINETYPESCALE;
					return FALSE;
				}
				break;
			case DXF_EXTRUSION_CODE:	// Extrusion direction
				if (!DXFReadPoint3D(pFile, DXF_EXTRUSION_CODE, m_s3DExtrusion)) {
					g_lDXFErrorCode = DXF_INVALID_EXTRUSION;
					return FALSE;
				}
				break;
			case DXF_VISIBILITY_CODE:	// visibility of object
				if (!DXFReadLong(pFile, m_lVisibility)) {
					g_lDXFErrorCode = DXF_INVALID_VISIBILITY;
					return FALSE;
				}
				break;
			case DXF_THICK_CODE:
				if (!DXFReadDouble(pFile, m_dThickness)) {
					g_lDXFErrorCode = DXF_INVALID_THICKNESS;
					return FALSE;
				}
				break;
			case DXF_POINT_COORDINATE:	// coordinate
				if (!DXFReadPoint3D(pFile, DXF_POINT_COORDINATE, m_s3DCoord)) {
					g_lDXFErrorCode = DXF_INVALID_POINT3D;
					return FALSE;
				}
				break;
			case DXF_POINT_ANGLE:
				if (!DXFReadDouble(pFile, m_dAngle)) {
					g_lDXFErrorCode = DXF_INVALID_ANGLE;
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
	} while (lCode != 0);

	return TRUE;
}

void CDXF_Point::DXFGenOCS()
{
	SDXF_Coord3D	sNewCoord3D;

	DXF_ConvBCS2WCS(m_sBCS, m_s3DCoord);

	DXF_GenBCSFromOCS(sNewCoord3D, m_sBCS.sZAxis);

	m_s3DExtrusion = sNewCoord3D.sZAxis;
}

void CDXF_Point::DXFGenBCS()
{
	DXF_GenBCSFromOCS(m_sBCS, m_s3DExtrusion);

	DXF_ConvWCS2BCS(m_sBCS, m_s3DCoord);
}
