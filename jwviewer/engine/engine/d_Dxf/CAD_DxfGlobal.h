#if !defined(_CAD_DXF_GLOBAL_)
#define _CAD_DXF_GLOBAL_

#include "DXF_Utils.h"
#include "DXF_MemFile.h"
#include "DXF_Entity.h"
#include "CAD_Doc.h"

///////////////////  For Import /////////////////
void			CvtDxfEntityHeadToDrw(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity, short &nLayer, CCAD_Pen *cPen=NULL, CCAD_Brush *cBrush=NULL);
void			CvtDxfBcsToCCAD_Bcs(CDXF_Entity *pDxfEntity, CCAD_Coord3D &cBcs);
void			CvtDxfCoordToCCAD_Vector(SDXF_Point3D *pDxfPt, CCAD_Vector &cPt);
SDXF_RGBColor	sDXF_CvtDXFToRGBColor(DXF_LONG& lDXF);

BOOL ImportDxfAddEntity(CCAD_Doc *pDoc, CDXF_Entity *pDxfEntity, short nBlockID = -1, BOOL blDimBlock = FALSE);

void ImportDxfHeaderSection(CDXF_MemFile * pDxfMemFile, CCAD_Doc *pDoc);
BOOL ImportDxfTableSection(CDXF_MemFile * pDxfMemFile, CCAD_Doc *pDoc);
BOOL ImportDxfBlockSection(CDXF_MemFile * pDxfMemFile, CCAD_Doc *pDoc);
BOOL ImportDxfEntitySection(CDXF_MemFile * pDxfMemFile, CCAD_Doc *pDoc);
BOOL ImportDxfObjectSection(CDXF_MemFile * pDxfMemFile, CCAD_Doc *pDoc);

//BOOL SetFaceName(CString &cFullPath, char *pszFaceName);

void ConvertDxfName(char* pDxfStyleName, char* pInName);
short CountArcDivide(CAD_FLOAT fDelta);

#endif //_CAD_DXF_GLOBAL_