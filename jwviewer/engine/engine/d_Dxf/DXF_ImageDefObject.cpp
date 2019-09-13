#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Object.h"
#include "DXF_ImageDefObject.h"

#define		DXF_IMAGEDEF_CLASSVER			90
#define		DXF_IMAGEDEF_IMAGENAME			1
#define		DXF_IMAGEDEF_IMAGESIZE			10
#define		DXF_IMAGEDEF_PIXELSIZE			11
#define		DXF_IMAGEDEF_LOADFLAG			280
#define		DXF_IMAGEDEF_RESOLUTIONUNIT		281

CDXF_ImageDefObject::CDXF_ImageDefObject()
{
	m_nObjectType = DXF_IMAGEDEFOBJECT;
	m_lClassVersion = 0;
	memset(m_pszImageFileName, 0x00, MAX_STRING_LEN);
	m_lLoadFlag = DXF_IMAGEDEF_LOAD;
	m_lResolutionUnit = DXF_IMAGEDEF_UNIT_CENTIMETER;
}

CDXF_ImageDefObject::CDXF_ImageDefObject(DXF_CHAR* szImageName, SDXF_Point2D& sImageSize, SDXF_Point2D& sPixelSize)
{
	m_nObjectType = DXF_IMAGEDEFOBJECT;

	m_lClassVersion = 0;
	memcpy(m_pszImageFileName, szImageName, MAX_STRING_LEN);
	m_lLoadFlag = DXF_IMAGEDEF_LOAD;
	m_lResolutionUnit = DXF_IMAGEDEF_UNIT_CENTIMETER;
	m_sW2DImageSize = sImageSize;
	m_sW2DOnePixelSize = sPixelSize;
}

CDXF_ImageDefObject::~CDXF_ImageDefObject()
{

}

DXF_BOOL CDXF_ImageDefObject::DXFReadObject(FILE *pFile)
{
	DXF_LONG	lCode;
	DXF_CHAR		szValue[MAX_STRING_LEN];

	if (!DXFReadLong(pFile, lCode)) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	do {
		switch (lCode) {	
			case DXF_HANDLE_CODE:
				if (!DXFReadHexLong(pFile, m_lHandleNumber)) {
					g_lDXFErrorCode = DXF_INVALID_HANDLE;
					return FALSE;
				}
				break;
			case DXF_IMAGEDEF_CLASSVER:
				if (!DXFReadLong(pFile, m_lClassVersion)) {
					g_lDXFErrorCode = DXF_INVALID_CLASSVER;
					return FALSE;
				}
				if (m_lClassVersion != 0) {
					g_lDXFErrorCode = DXF_INVALID_CLASSVER;
					return FALSE;
				}
				break;
			case DXF_IMAGEDEF_IMAGENAME:
				if (!DXFReadString(pFile, m_pszImageFileName)) {
					g_lDXFErrorCode = DXF_INVALID_IMAGENAME;
					return FALSE;
				}
				break;
			case DXF_IMAGEDEF_IMAGESIZE:
				if (!DXFReadPoint2D(pFile, DXF_IMAGEDEF_IMAGESIZE, m_sW2DImageSize)) {
					g_lDXFErrorCode = DXF_INVALID_POINT2D;
					return FALSE;
				}
				break;
			case DXF_IMAGEDEF_PIXELSIZE:
				if (!DXFReadPoint2D(pFile, DXF_IMAGEDEF_PIXELSIZE, m_sW2DOnePixelSize)) {
					g_lDXFErrorCode = DXF_INVALID_POINT2D;
					return FALSE;
				}
				break;
			case DXF_IMAGEDEF_LOADFLAG:
				if (!DXFReadLong(pFile, m_lLoadFlag)) {
					g_lDXFErrorCode = DXF_INVALID_LOADFLAG;
					return FALSE;
				}
				break;
			case DXF_IMAGEDEF_RESOLUTIONUNIT:
				if (!DXFReadLong(pFile, m_lResolutionUnit)) {
					g_lDXFErrorCode = DXF_INVALID_RESOLUTIONUNIT;
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

