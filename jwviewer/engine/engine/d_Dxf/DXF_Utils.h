#include <math.h>

#include "DXF_Define.h"
#include "CAD_PtrArray.h"
#include <stdio.h>


// DXF Global Functions
DXF_BOOL DXFReadLong(FILE* pFile, DXF_LONG& value);
DXF_BOOL DXFReadHexLong(FILE* pFile, DXF_LONG& value);
DXF_BOOL DXFReadDouble(FILE* pFile, DXF_DOUBLE& value);
DXF_BOOL DXFReadString(FILE* pFile, DXF_CHAR* value);
DXF_BOOL DXFReadPoint3D(FILE* pFile, DXF_LONG refCode, DXF_DOUBLE& x, DXF_DOUBLE& y, DXF_DOUBLE& z);
DXF_BOOL DXFReadPoint3D(FILE* pFile, DXF_LONG refCode, SDXF_Point3D& pt);
DXF_BOOL DXFReadPoint2D(FILE* pFile, DXF_LONG refCode, DXF_DOUBLE& x, DXF_DOUBLE& y);
DXF_BOOL DXFReadPoint2D(FILE* pFile, DXF_LONG refCode, SDXF_Point2D& pt);
DXF_BOOL DXFReadGroup(FILE* pFile, DXF_LONG& code, DXF_CHAR* string);

DXF_BOOL DXFReadGroupLong(FILE* pFile, DXF_LONG refcode, DXF_LONG& value);
DXF_BOOL DXFReadGroupHexLong(FILE* pFile, DXF_LONG refcode, DXF_LONG& value);
DXF_BOOL DXFReadGroupDouble(FILE* pFile, DXF_LONG refcode, DXF_DOUBLE& value);
DXF_BOOL DXFReadGroupString(FILE* pFile, DXF_LONG refcode, DXF_CHAR* value);
DXF_BOOL DXFReadGroupPoint3D(FILE* pFile, DXF_LONG refcode, SDXF_Point3D& point);
DXF_BOOL DXFReadGroupPoint2D(FILE* pFile, DXF_LONG refcode, SDXF_Point2D& point);

void DXF_GenBCSFromOCS(SDXF_Coord3D& sCoord3D, SDXF_Point3D sOcsZ);
void DXF_Normalize(SDXF_Point3D& sVec);
void DXF_ConvBCS2WCS(SDXF_Coord3D sCoord3D, SDXF_Point3D& sPoint);
void DXF_ConvWCS2BCS(SDXF_Coord3D sCoord3D, SDXF_Point3D& sPoint);

extern	DXF_LONG			g_lDXFHandleNumber;
extern	DXF_LONG			g_lDXFFileLine;

extern	FILE*				g_pDXFHeaderFile;
extern	FILE*				g_pDXFClassesFile;
extern	FILE*				g_pDXFTablesFile;
extern	FILE*				g_pDXFBlocksFile;
extern	FILE*				g_pDXFEntitiesFile;
extern	FILE*				g_pDXFObjectsFile;

extern	DXF_LONG			g_lDXFGroupHandle;
extern	DXF_LONG			g_lDXFImageDefHandle;
extern	DXF_LONG			g_lDXFMLineStyleHandle;
extern	DXF_LONG			g_lDXFLayoutHandle;

