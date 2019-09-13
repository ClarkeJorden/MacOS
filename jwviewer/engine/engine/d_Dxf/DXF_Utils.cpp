#include "stdafx.h"

#include <math.h>
#include "DXF_Utils.h"

DXF_BOOL DXFReadLong(FILE* pFile, DXF_LONG& lValue)
{
	fscanf(pFile, "%d", &lValue);
	g_lDXFFileLine++;
	return TRUE;
}

DXF_BOOL DXFReadHexLong(FILE* pFile, DXF_LONG& lValue)
{
	fscanf(pFile, "%x", &lValue);
	g_lDXFFileLine++;
	return TRUE;
}

DXF_BOOL DXFReadDouble(FILE* pFile, DXF_DOUBLE& dValue)
{
	fscanf(pFile, "%lf", &dValue);
	g_lDXFFileLine++;
	return TRUE;
}

DXF_BOOL DXFReadString(FILE* pFile, DXF_CHAR* szValue)
{
	DXF_CHAR	tmpString[MAX_STRING_LEN];
	DXF_LONG	lLen;

	fgets(tmpString, MAX_STRING_LEN, pFile);
	
	if (NULL == fgets(szValue, MAX_STRING_LEN, pFile)) {
		if ( strcmp(szValue, "END") == 0 )
			return TRUE;
		return FALSE;
	}

	lLen = strlen(szValue);
	::memcpy(tmpString, szValue, lLen);

	int nCount = 0;
	for ( int i=0; i<lLen; i++ ) {
		if ( strncmp( &tmpString[i], "\\M+2", 4 ) == 0 ) {
			i += 4;
			char nCode[2];
			for ( int k=0; k<2; k++ ) {
				short num;
				nCode[k] = 0;
				for ( int j=0; j<2; j++, i++ ) {
					nCode[k] *= 16;
					switch (tmpString[i]) {
					case 'A' :	case 'a' :			num = 10;	break;
					case 'B' :	case 'b' :			num = 11;	break;
					case 'C' :	case 'c' :			num = 12;	break;
					case 'D' :	case 'd' :			num = 13;	break;
					case 'E' :	case 'e' :			num = 14;	break;
					case 'F' :	case 'f' :			num = 15;	break;
					case '0' :			num = 0;	break;
					case '1' :			num = 1;	break;
					case '2' :			num = 2;	break;
					case '3' :			num = 3;	break;
					case '4' :			num = 4;	break;
					case '5' :			num = 5;	break;
					case '6' :			num = 6;	break;
					case '7' :			num = 7;	break;
					case '8' :			num = 8;	break;
					case '9' :			num = 9;	break;
					}
					nCode[k] += num;
				}
			}
			szValue[nCount] = nCode[0];
			szValue[nCount+1] = nCode[1];
			i--;
			nCount += 2;
		}
		else {
			szValue[nCount] = tmpString[i];
			nCount++;
		}
	}

	if (nCount > 0)
		szValue[nCount-2] = 0x00;
	g_lDXFFileLine++;
	return TRUE;
}

DXF_BOOL DXFReadPoint3D(FILE* pFile, DXF_LONG lRefCode, DXF_DOUBLE& dX, DXF_DOUBLE& dY, DXF_DOUBLE& dZ)
{
	DXF_LONG	lCode;

	fscanf(pFile, "%lf", &dX);
	g_lDXFFileLine++;
	if ((DXFReadLong(pFile, lCode) == FALSE) || (lCode != lRefCode + 10)) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	fscanf(pFile, "%lf", &dY);
	g_lDXFFileLine++;

	DXF_LONG	lpos = ftell(pFile);
	if ((DXFReadLong(pFile, lCode) == FALSE)) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if ( lCode != lRefCode + 20) {
		dZ = 0;
		fseek(pFile, lpos, SEEK_SET);
		g_lDXFFileLine--;
		return TRUE;
	}
	fscanf(pFile, "%lf", &dZ);
	g_lDXFFileLine++;

	return TRUE;
}

DXF_BOOL DXFReadPoint3D(FILE* pFile, DXF_LONG lRefCode, SDXF_Point3D& s3DPoint)
{
	DXF_LONG	lCode;

	fscanf(pFile, "%lf", &s3DPoint.dX);
	g_lDXFFileLine++;
	if ((DXFReadLong(pFile, lCode) == FALSE) || (lCode != lRefCode + 10)) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	fscanf(pFile, "%lf", &s3DPoint.dY);
	g_lDXFFileLine++;

	DXF_LONG	lpos = ftell(pFile);
	if ((DXFReadLong(pFile, lCode) == FALSE)) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if ( lCode != lRefCode + 20) {
		s3DPoint.dZ = 0;
		fseek(pFile, lpos, SEEK_SET);
		g_lDXFFileLine--;
		return TRUE;
	}
	fscanf(pFile, "%lf", &s3DPoint.dZ);
	g_lDXFFileLine++;

	return TRUE;
}

DXF_BOOL DXFReadPoint2D(FILE* pFile, DXF_LONG lRefCode, DXF_DOUBLE& dX, DXF_DOUBLE& dY)
{
	DXF_LONG	lCode;

	fscanf(pFile, "%lf", &dX);
	g_lDXFFileLine++;
	if ((DXFReadLong(pFile, lCode) == FALSE) || (lCode != lRefCode + 10)) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	fscanf(pFile, "%lf", &dY);
	g_lDXFFileLine++;

	return TRUE;
}

DXF_BOOL DXFReadPoint2D(FILE* pFile, DXF_LONG lRefCode, SDXF_Point2D& s2DPoint)
{
	DXF_LONG	lCode;

	fscanf(pFile, "%lf", &s2DPoint.dX);
	g_lDXFFileLine++;
	if ((DXFReadLong(pFile, lCode) == FALSE) || (lCode != lRefCode + 10)) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	fscanf(pFile, "%lf", &s2DPoint.dY);
	g_lDXFFileLine++;

	return TRUE;
}

DXF_BOOL DXFReadGroup(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue)
{
	DXF_CHAR	tmpString[MAX_STRING_LEN];
	DXF_LONG	lLen;

	fscanf(pFile, "%d", &lCode);
	g_lDXFFileLine++;
	if (lCode < 0) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	fgets(tmpString, MAX_STRING_LEN, pFile);
	if (NULL == fgets(szValue, MAX_STRING_LEN, pFile))
		return FALSE;
	lLen = strlen(szValue);
	if (lLen > 0)
		szValue[lLen-2] = 0x00;
	g_lDXFFileLine++;

	return TRUE;
}

DXF_BOOL DXFReadGroupLong(FILE* pFile, DXF_LONG lRefCode, DXF_LONG& lValue)
{
	DXF_LONG	lCode;

	if (FALSE == DXFReadLong(pFile, lCode) || lCode != lRefCode) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}

	return DXFReadLong(pFile, lValue);
}

DXF_BOOL DXFReadGroupHexLong(FILE* pFile, DXF_LONG lRefCode, DXF_LONG& lValue)
{
	DXF_LONG	lCode;

	if (FALSE == DXFReadLong(pFile, lCode) || lCode != lRefCode) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}

	return DXFReadHexLong(pFile, lValue);
}

DXF_BOOL DXFReadGroupDouble(FILE* pFile, DXF_LONG lRefCode, DXF_DOUBLE& dValue)
{
	DXF_LONG	lCode;

	if (FALSE == DXFReadLong(pFile, lCode) || lCode != lRefCode) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}

	return DXFReadDouble(pFile, dValue);
}

DXF_BOOL DXFReadGroupString(FILE* pFile, DXF_LONG lRefCode, DXF_CHAR* szValue)
{
	DXF_LONG	lCode;

	if (FALSE == DXFReadLong(pFile, lCode) || lCode != lRefCode) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}

	return DXFReadString(pFile, szValue);
}

DXF_BOOL DXFReadGroupPoint3D(FILE* pFile, DXF_LONG lRefCode, SDXF_Point3D& s3DPoint)
{
	DXF_LONG	lCode;

	if ((DXFReadLong(pFile, lCode) == FALSE) || (lCode != lRefCode)) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	fscanf(pFile, "%lf", &s3DPoint.dX);
	g_lDXFFileLine++;
	if ((DXFReadLong(pFile, lCode) == FALSE) || (lCode != lRefCode + 10)) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	fscanf(pFile, "%lf", &s3DPoint.dY);
	g_lDXFFileLine++;
	if ((DXFReadLong(pFile, lCode) == FALSE) || (lCode != lRefCode + 20)) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	fscanf(pFile, "%lf", &s3DPoint.dZ);
	g_lDXFFileLine++;

	return TRUE;
}
DXF_BOOL DXFReadGroupPoint2D(FILE* pFile, DXF_LONG lRefCode, SDXF_Point2D& s2DPoint)
{
	DXF_LONG	lCode;

	if ((DXFReadLong(pFile, lCode) == FALSE) || (lCode != lRefCode)) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	fscanf(pFile, "%lf", &s2DPoint.dX);
	g_lDXFFileLine++;
	if ((DXFReadLong(pFile, lCode) == FALSE) || (lCode != lRefCode + 10)) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	fscanf(pFile, "%lf", &s2DPoint.dY);
	g_lDXFFileLine++;
	return TRUE;
}

DXF_DOUBLE dGetDistance(SDXF_Point3D &s3DPoint1, SDXF_Point3D &s3DPoint2)
{
	DXF_DOUBLE	dX = s3DPoint1.dX - s3DPoint2.dX;
	DXF_DOUBLE	dY = s3DPoint1.dY - s3DPoint2.dY;

	return sqrt(dX*dX + dY*dY);
}

DXF_DOUBLE dGetAngle(SDXF_Point3D &s3DPoint1, SDXF_Point3D &s3DPoint2)
{
	DXF_DOUBLE	dX = s3DPoint1.dX - s3DPoint2.dX;
	DXF_DOUBLE	dY = s3DPoint1.dY - s3DPoint2.dY;

	return atan2(dY, dX);
}

void DXF_GenBCSFromOCS(SDXF_Coord3D& sCoord3D, SDXF_Point3D sOcsZ)
{
	DXF_DOUBLE	dRange = 1.0 / 64.0;

	sCoord3D.sZAxis = sOcsZ;
	if ((fabs(sOcsZ.dX) < dRange) && (fabs(sOcsZ.dY) < dRange)) {
		sCoord3D.sXAxis.dX = sOcsZ.dZ;
		sCoord3D.sXAxis.dY = 0.0;
		sCoord3D.sXAxis.dZ = -sOcsZ.dX;
	}
	else {
		sCoord3D.sXAxis.dX = -sOcsZ.dY;
		sCoord3D.sXAxis.dY = sOcsZ.dX;
		sCoord3D.sXAxis.dZ = 0.0;
	}

	sCoord3D.sYAxis.dX = sOcsZ.dY * sCoord3D.sXAxis.dZ - sOcsZ.dZ * sCoord3D.sXAxis.dY;
	sCoord3D.sYAxis.dY = sOcsZ.dZ * sCoord3D.sXAxis.dX - sOcsZ.dX * sCoord3D.sXAxis.dZ;
	sCoord3D.sYAxis.dZ = sOcsZ.dX * sCoord3D.sXAxis.dY - sOcsZ.dY * sCoord3D.sXAxis.dX;

	DXF_Normalize(sCoord3D.sXAxis);
	DXF_Normalize(sCoord3D.sYAxis);
	DXF_Normalize(sCoord3D.sZAxis);
}

void DXF_Normalize(SDXF_Point3D& sVec)
{
	DXF_DOUBLE	dLen = sqrt(sVec.dX * sVec.dX + sVec.dY * sVec.dY + sVec.dZ * sVec.dZ);

	sVec.dX /= dLen;
	sVec.dY /= dLen;
	sVec.dZ /= dLen;
}

void DXF_ConvBCS2WCS(SDXF_Coord3D sCoord3D, SDXF_Point3D& sPoint)
{
	DXF_DOUBLE dOldX = sPoint.dX;
	DXF_DOUBLE dOldY = sPoint.dY;
	DXF_DOUBLE dOldZ = sPoint.dZ;

	sPoint.dX = sCoord3D.sOrg.dX +
				(dOldX * sCoord3D.sXAxis.dX +
				 dOldY * sCoord3D.sYAxis.dX +
				 dOldZ * sCoord3D.sZAxis.dX);
	sPoint.dY = sCoord3D.sOrg.dY +
				(dOldX * sCoord3D.sXAxis.dY +
				 dOldY * sCoord3D.sYAxis.dY +
				 dOldZ * sCoord3D.sZAxis.dY);
	sPoint.dZ = sCoord3D.sOrg.dZ +
				(dOldX * sCoord3D.sXAxis.dZ +
				 dOldY * sCoord3D.sYAxis.dZ +
				 dOldZ * sCoord3D.sZAxis.dZ);
}

void DXF_ConvWCS2BCS(SDXF_Coord3D sCoord3D, SDXF_Point3D& sPoint)
{
	DXF_DOUBLE	dDirX = sPoint.dX - sCoord3D.sOrg.dX;
	DXF_DOUBLE	dDirY = sPoint.dY - sCoord3D.sOrg.dY;
	DXF_DOUBLE	dDirZ = sPoint.dZ - sCoord3D.sOrg.dZ;

	sPoint.dX = dDirX * sCoord3D.sXAxis.dX +
				dDirY * sCoord3D.sXAxis.dY +
				dDirZ * sCoord3D.sXAxis.dZ;
	sPoint.dY = dDirX * sCoord3D.sYAxis.dX +
				dDirY * sCoord3D.sYAxis.dY +
				dDirZ * sCoord3D.sYAxis.dZ;
	sPoint.dZ = dDirX * sCoord3D.sZAxis.dX +
				dDirY * sCoord3D.sZAxis.dY +
				dDirZ * sCoord3D.sZAxis.dZ;
}

