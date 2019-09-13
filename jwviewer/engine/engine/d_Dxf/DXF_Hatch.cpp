#include "stdafx.h"
#include "DXF_Utils.h"
#include "DXF_Entity.h"
#include "DXF_Hatch.h"

#define		DXF_HATCH_HEIGHTPOINT		10

CDXF_Hatch::CDXF_Hatch()
{
	m_nEntityType = DXF_HATCH;
	memcpy(m_pszHatchPatternName, "", MAX_STRING_LEN);
	m_lSolidFillCode = 0;
	m_lAssociativityCode = 0;
	m_lHatchStyle = 0;
	m_lHatchPatternType = 0;
	m_dHatchPatternAngle = 0.0;
	m_dHatchPatternScaleOrOffset = 0.0;
	m_lHatchPatternDoubleCode = 0;
	m_dPixelSize = 0.0;	// option
	m_lSeedPointCnt = 0;
	m_spSeedPoints = NULL;
// boundary path data
	m_lBoundaryObjectCnt = 0;
	m_cObjectRefs.RemoveAll();

// edge data
	m_cHatchLineArray.RemoveAll();
	m_cHatchArcArray.RemoveAll();
	m_cHatchEllipseArray.RemoveAll();
	m_cHatchPolylineArray.RemoveAll();
	m_cHatchSplineArray.RemoveAll();

	m_cHatchPath.RemoveAll();

// pattern data
	m_lHatchPatternCnt = 0;
	m_spHatchPattern = NULL;
}

CDXF_Hatch::~CDXF_Hatch()
{
	DXF_LONG	i, lCnt;

	m_cHatchLineArray.RemoveAll();
	m_cHatchArcArray.RemoveAll();
	m_cHatchEllipseArray.RemoveAll();

	lCnt = m_cHatchPolylineArray.GetSize();
	if (lCnt) {
		for (i = 0; i < lCnt; i++) {
			delete [] m_cHatchPolylineArray[i].spVertices;
		}
		m_cHatchPolylineArray.RemoveAll();
	}

	lCnt = m_cHatchSplineArray.GetSize();
	if (lCnt) {
		for (i = 0; i < lCnt; i++) {
			delete [] m_cHatchSplineArray[i].spControls;
			delete [] m_cHatchSplineArray[i].dpKnots;
		}
		m_cHatchSplineArray.RemoveAll();
	}

	if (m_spSeedPoints != NULL)
		delete [] m_spSeedPoints;

	m_cObjectRefs.RemoveAll();

	for (i = 0; i < m_lHatchPatternCnt; i++) {
		if (m_spHatchPattern[i].dpDashes != NULL)
			delete [] m_spHatchPattern[i].dpDashes;
	}
	delete [] m_spHatchPattern;

	lCnt = m_cHatchPath.GetSize();
	if (lCnt) {
		for (i = 0; i < lCnt; i++) {
			if (m_cHatchPath[i].spEdges != NULL)
				delete [] m_cHatchPath[i].spEdges;
		}
		m_cHatchPath.RemoveAll();
	}
}

DXF_BOOL CDXF_Hatch::DXFReadEntity(FILE *pFile)
{
	DXF_LONG		i, lRef, lCode;
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
			case DXF_HATCH_HEIGHTPOINT:
				if (!DXFReadPoint3D(pFile, DXF_HATCH_HEIGHTPOINT, m_s3DHeightPoint)) {
					g_lDXFErrorCode = DXF_INVALID_POINT3D;
					return FALSE;
				}
				break;
			case 2:
				if (!DXFReadString(pFile, m_pszHatchPatternName)) {
					g_lDXFErrorCode = DXF_INVALID_STRING;
					return FALSE;
				}
				break;
			case 70:
				if (!DXFReadLong(pFile, m_lSolidFillCode)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case 71:
				if (!DXFReadLong(pFile, m_lAssociativityCode)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case 91:
				if (!DXFReadLong(pFile, m_lBoundaryPathCnt)) {
					g_lDXFErrorCode = DXF_INVALID_COUNT;
					return FALSE;
				}
				if (!DXFReadHatchBoundaryPath(pFile, m_lBoundaryPathCnt)) {
					return FALSE;
				}
				break;
			case 75:
				if (!DXFReadLong(pFile, m_lHatchStyle)) {
					g_lDXFErrorCode = DXF_INVALID_HATCH_STYLE;
					return FALSE;
				}
				break;
			case 76:
				if (!DXFReadLong(pFile, m_lHatchPatternType)) {
					g_lDXFErrorCode = DXF_INVALID_HATCH_PATTERNTYPE;
					return FALSE;
				}
				break;
			case 52:
				if (!DXFReadDouble(pFile, m_dHatchPatternAngle)) {
					g_lDXFErrorCode = DXF_INVALID_ANGLE;
					return FALSE;
				}
				break;
			case 41:
				if (!DXFReadDouble(pFile, m_dHatchPatternScaleOrOffset)) {
					g_lDXFErrorCode = DXF_INVALID_SCALE;
					return FALSE;
				}
				break;
			case 77:
				if (!DXFReadLong(pFile, m_lHatchPatternDoubleCode)) {
					g_lDXFErrorCode = DXF_INVALID_VALUE;
					return FALSE;
				}
				break;
			case 78:
				if (!DXFReadLong(pFile, m_lHatchPatternCnt)) {
					g_lDXFErrorCode = DXF_INVALID_COUNT;
					return FALSE;
				}
				break;
			case 47:
				if (!DXFReadDouble(pFile, m_dPixelSize)) {
					g_lDXFErrorCode = DXF_INVALID_HATCH_PIXELSIZE;
					return FALSE;
				}
				break;
			case 98:
				if (!DXFReadLong(pFile, m_lSeedPointCnt)) {
					g_lDXFErrorCode = DXF_INVALID_COUNT;
					return FALSE;
				}
				m_spSeedPoints = new SDXF_Point2D[m_lSeedPointCnt];
				if (m_spSeedPoints == NULL) {
					g_lDXFErrorCode = DXF_MEM_ERROR;
					return FALSE;
				}
				for (i = 0; i < m_lSeedPointCnt; i++) {
					if (!DXFReadLong(pFile, lCode) || lCode != 10) {
						g_lDXFErrorCode = DXF_INVALID_CODE;
						return FALSE;
					}
					if (!DXFReadPoint2D(pFile, 10, m_spSeedPoints[i])) {
						g_lDXFErrorCode = DXF_INVALID_POINT3D;
						return FALSE;
					}
				}
				break;
			case 97:
				if (!DXFReadLong(pFile, m_lBoundaryObjectCnt)) {
					g_lDXFErrorCode = DXF_INVALID_COUNT;
					return FALSE;
				}
				for (i = 0; i < m_lBoundaryObjectCnt; i++) {
					if (!DXFReadLong(pFile, lCode) || lCode != 330) {
						g_lDXFErrorCode = DXF_INVALID_CODE;
						return FALSE;
					}
					if (!DXFReadHexLong(pFile, lRef)) {
						g_lDXFErrorCode = DXF_INVALID_HANDLE;
						return FALSE;
					}
					m_cObjectRefs.Add(lRef);
				}
				break;
			case 53:	// pattern data starting
				if (!DXFReadHatchPatterns(pFile))
					return FALSE;
				break;
			default:
				if (!DXFReadString(pFile, szValue))
					return FALSE;
				break;
		}
		if (!DXFReadLong(pFile, lCode)) {
			g_lDXFErrorCode = DXF_INVALID_CODE;
			return FALSE;
		}
	} while (lCode != 0);

	return TRUE;
}

DXF_BOOL CDXF_Hatch::DXFReadHatchBoundaryPath(FILE *pFile, DXF_LONG PathCnt)
{
	DXF_LONG		i, j, k, lCode, lObjCnt;
	DXF_LONG		lEdgeType, lEdgeCnt;
	DXF_CHAR		szValue[MAX_STRING_LEN];

	for (i = 0; i < PathCnt; i++) {
		SDXF_HatchPath	sPath;

		sPath.lEdgeCnt = 0;
		sPath.spEdges = NULL;
		if (!DXFReadLong(pFile, lCode) || lCode != 92) {
			g_lDXFErrorCode = DXF_INVALID_CODE;
			return FALSE;
		}
		if (!DXFReadLong(pFile, sPath.lPathType)) {
			g_lDXFErrorCode = DXF_INVALID_HATCH_BOUNDTYPE;
			return FALSE;
		}

		if (sPath.lPathType & DXF_HATCH_BOUND_POLYLINE) {	// Polyline
			if (!DXFReadHatchBoundPolyline(pFile))
				return FALSE;
			sPath.lEdgeCnt = 1;
			sPath.spEdges = new SDXF_HatchPathEdge [1];
			if (sPath.spEdges == NULL) {
				g_lDXFErrorCode = DXF_MEM_ERROR;
				return FALSE;
			}
			sPath.spEdges[0].lType = DXF_HATCH_EDGE_POLYLINE;
			sPath.spEdges[0].lIndex = m_cHatchPolylineArray.GetSize() - 1;
		}
		else {
			if (!DXFReadLong(pFile, lCode) || lCode != 93) {
				g_lDXFErrorCode = DXF_INVALID_CODE;
				return FALSE;
			}

			// Edge Count
			if (!DXFReadLong(pFile, lEdgeCnt))
				return FALSE;
			if (lEdgeCnt < 1) {
				g_lDXFErrorCode = DXF_INVALID_COUNT;
				return FALSE;
			}
			sPath.lEdgeCnt = lEdgeCnt;
			sPath.spEdges = new SDXF_HatchPathEdge [lEdgeCnt];
			if (sPath.spEdges == NULL) {
				g_lDXFErrorCode = DXF_MEM_ERROR;
				return FALSE;
			}
			for (j = 0; j < lEdgeCnt; j++) {
				if (!DXFReadLong(pFile, lCode) || lCode != 72) {
					g_lDXFErrorCode = DXF_INVALID_CODE;
					return FALSE;
				}
				if (!DXFReadLong(pFile, lEdgeType)) {
					g_lDXFErrorCode = DXF_INVALID_HATCH_EDGETYPE;
					return FALSE;
				}
				switch (lEdgeType) {
					case 1:		// LINE
						if (!DXFReadHatchBoundLine(pFile))
							return FALSE;
						sPath.spEdges[j].lType = DXF_HATCH_EDGE_LINE;
						sPath.spEdges[j].lIndex = m_cHatchLineArray.GetSize() - 1;
						break;
					case 2:		// ARC
						if (!DXFReadHatchBoundArc(pFile))
							return FALSE;
						sPath.spEdges[j].lType = DXF_HATCH_EDGE_ARC;
						sPath.spEdges[j].lIndex = m_cHatchArcArray.GetSize() - 1;
						break;
					case 3:		// Ellipse
						if (!DXFReadHatchBoundEllipse(pFile))
							return FALSE;
						sPath.spEdges[j].lType = DXF_HATCH_EDGE_ELLIPARC;
						sPath.spEdges[j].lIndex = m_cHatchEllipseArray.GetSize() - 1;
						break;
					case 4:		// Spline
						if (!DXFReadHatchBoundSpline(pFile))
							return FALSE;
						sPath.spEdges[j].lType = DXF_HATCH_EDGE_SPLINE;
						sPath.spEdges[j].lIndex = m_cHatchSplineArray.GetSize() - 1;
						break;
				}
			}
		}
		m_cHatchPath.Add(sPath);

		if (!DXFReadLong(pFile, lCode) || lCode != 97) {
			g_lDXFErrorCode = DXF_INVALID_CODE;
			return FALSE;
		}

		lObjCnt = 0;
		if (!DXFReadLong(pFile, lObjCnt)) {
			g_lDXFErrorCode = DXF_INVALID_COUNT;
			return FALSE;
		}
		for (k = 0; k < lObjCnt; k++) {
			if (!DXFReadGroup(pFile, lCode, szValue))
				return FALSE;
		}
	}

	return TRUE;
}

DXF_BOOL CDXF_Hatch::DXFReadHatchBoundLine(FILE *pFile)
{
	DXF_LONG		lCode;
	SDXF_HatchLine	sLine;

	if (!DXFReadLong(pFile, lCode) || lCode != 10) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadPoint2D(pFile, 10, sLine.sStartPt)) {
		g_lDXFErrorCode = DXF_INVALID_POINT2D;
		return FALSE;
	}
	if (!DXFReadLong(pFile, lCode) || lCode != 11) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadPoint2D(pFile, 11, sLine.sEndPt)) {
		g_lDXFErrorCode = DXF_INVALID_POINT2D;
		return FALSE;
	}
	m_cHatchLineArray.Add(sLine);
	return TRUE;
}

DXF_BOOL CDXF_Hatch::DXFReadHatchBoundArc(FILE *pFile)
{
	DXF_LONG		lCode;
	SDXF_HatchArc	sArc;

	if (!DXFReadLong(pFile, lCode) || lCode != 10) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadPoint2D(pFile, 10, sArc.sCenter)) {
		g_lDXFErrorCode = DXF_INVALID_POINT2D;
		return FALSE;
	}
	if (!DXFReadLong(pFile, lCode) || lCode != 40) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadDouble(pFile, sArc.dRadius)) {
		g_lDXFErrorCode = DXF_INVALID_RADIUS;
		return FALSE;
	}
	if (!DXFReadLong(pFile, lCode) || lCode != 50) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadDouble(pFile, sArc.dStartAngle)) {
		g_lDXFErrorCode = DXF_INVALID_ANGLE;
		return FALSE;
	}
	if (!DXFReadLong(pFile, lCode) || lCode != 51) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadDouble(pFile, sArc.dEndAngle)) {
		g_lDXFErrorCode = DXF_INVALID_ANGLE;
		return FALSE;
	}
	if (!DXFReadLong(pFile, lCode) || lCode != 73) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadLong(pFile, sArc.lClockWise))
		return FALSE;

	m_cHatchArcArray.Add(sArc);
	return TRUE;
}

DXF_BOOL CDXF_Hatch::DXFReadHatchBoundEllipse(FILE *pFile)
{
	DXF_LONG		lCode;
	SDXF_HatchEllipse	sEllipse;

	if (!DXFReadLong(pFile, lCode) || lCode != 10) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadPoint2D(pFile, 10, sEllipse.sCenter)) {
		g_lDXFErrorCode = DXF_INVALID_POINT2D;
		return FALSE;
	}
	if (!DXFReadLong(pFile, lCode) || lCode != 11) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadPoint2D(pFile, 11, sEllipse.sEndPt)) {
		g_lDXFErrorCode = DXF_INVALID_POINT2D;
		return FALSE;
	}
	if (!DXFReadLong(pFile, lCode) || lCode != 40) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadDouble(pFile, sEllipse.dMainLen))
		return FALSE;
	if (!DXFReadLong(pFile, lCode) || lCode != 50) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadDouble(pFile, sEllipse.dStartAngle)) {
		g_lDXFErrorCode = DXF_INVALID_ANGLE;
		return FALSE;
	}
	if (!DXFReadLong(pFile, lCode) || lCode != 51) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadDouble(pFile, sEllipse.dEndAngle)) {
		g_lDXFErrorCode = DXF_INVALID_ANGLE;
		return FALSE;
	}
	if (!DXFReadLong(pFile, lCode) || lCode != 73) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadLong(pFile, sEllipse.lClockWise))
		return FALSE;
	m_cHatchEllipseArray.Add(sEllipse);
	return TRUE;
}

DXF_BOOL CDXF_Hatch::DXFReadHatchBoundSpline(FILE *pFile)
{
	DXF_LONG			lCode, i;
	SDXF_HatchSpline		sSpline;

	if (!DXFReadLong(pFile, lCode) || lCode != 94) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadLong(pFile, sSpline.lDegree)) {
		g_lDXFErrorCode = DXF_INVALID_DEGREE;
		return FALSE;
	}
	if (!DXFReadLong(pFile, lCode) || lCode != 73) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadLong(pFile, sSpline.lRationalSpline))
		return FALSE;
	if (!DXFReadLong(pFile, lCode) || lCode != 74) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadLong(pFile, sSpline.lPeriodicSpline))
		return FALSE;
	if (!DXFReadLong(pFile, lCode) || lCode != 95) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadLong(pFile, sSpline.lKnotCnt)) {
		g_lDXFErrorCode = DXF_INVALID_COUNT;
		return FALSE;
	}
	sSpline.dpKnots = new DXF_DOUBLE[sSpline.lKnotCnt];
	if (sSpline.dpKnots == NULL) {
		g_lDXFErrorCode = DXF_MEM_ERROR;
		return FALSE;
	}
	if (!DXFReadLong(pFile, lCode) || lCode != 96) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadLong(pFile, sSpline.lControlCnt)) {
		g_lDXFErrorCode = DXF_INVALID_COUNT;
		return FALSE;
	}
	sSpline.spControls = new SDXF_Point2D[sSpline.lControlCnt];
	if (sSpline.spControls == NULL) {
		g_lDXFErrorCode = DXF_MEM_ERROR;
		return FALSE;
	}
	sSpline.dpWeights = new DXF_DOUBLE[sSpline.lControlCnt];
	if (sSpline.dpWeights == NULL) {
		g_lDXFErrorCode = DXF_MEM_ERROR;
		return FALSE;
	}

	for (i = 0; i < sSpline.lKnotCnt; i++) {
		if (!DXFReadLong(pFile, lCode) || lCode != 40) {
			g_lDXFErrorCode = DXF_INVALID_CODE;
			return FALSE;
		}
		if (!DXFReadDouble(pFile, sSpline.dpKnots[i]))
			return FALSE;
	}
	
	DXF_LONG	lpos;

	for (i = 0; i < sSpline.lControlCnt; i++) {
		if (!DXFReadLong(pFile, lCode) || lCode != 10)
			return FALSE;
		if (!DXFReadPoint2D(pFile, 10, sSpline.spControls[i]))
			return FALSE;
		lpos = ftell(pFile);
		if (DXFReadLong(pFile, lCode) && lCode == 42) {
			if (!DXFReadDouble(pFile, sSpline.dpWeights[i]))
				return FALSE;
		}
		else {
			sSpline.dpWeights[i] = 1.0;
			fseek(pFile, lpos, SEEK_SET);
		}
	}
	m_cHatchSplineArray.Add(sSpline);
	return TRUE;
}

DXF_BOOL CDXF_Hatch::DXFReadHatchBoundPolyline(FILE *pFile)
{
	DXF_LONG			lCode, i;
	SDXF_HatchPolyline	sPolyline;

	if (!DXFReadLong(pFile, lCode) || lCode != 72) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadLong(pFile, sPolyline.lBulgeFlag))
		return FALSE;
	if (!DXFReadLong(pFile, lCode) || lCode != 73) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadLong(pFile, sPolyline.lIsClosedFlag))
		return FALSE;
	if (!DXFReadLong(pFile, lCode) || lCode != 93) {
		g_lDXFErrorCode = DXF_INVALID_CODE;
		return FALSE;
	}
	if (!DXFReadLong(pFile, sPolyline.lVertexCnt)) {
		g_lDXFErrorCode = DXF_INVALID_COUNT;
		return FALSE;
	}
	sPolyline.spVertices = new SDXF_PolyPoint [sPolyline.lVertexCnt];
	if (sPolyline.spVertices == NULL) {
		g_lDXFErrorCode = DXF_MEM_ERROR;
		return FALSE;
	}

	DXF_LONG	lpos;

	for (i = 0; i < sPolyline.lVertexCnt; i++) {
		if (!DXFReadLong(pFile, lCode) || lCode != 10) {
			g_lDXFErrorCode = DXF_INVALID_CODE;
			return FALSE;
		}
		if (!DXFReadPoint2D(pFile, 10, sPolyline.spVertices[i].dX, sPolyline.spVertices[i].dY)) {
			g_lDXFErrorCode = DXF_INVALID_POINT2D;
			return FALSE;
		}
		lpos = ftell(pFile);
		if (DXFReadLong(pFile, lCode) && lCode == 42) {
			if (!DXFReadDouble(pFile, sPolyline.spVertices[i].dBulge))
				return FALSE;
		}
		else {
			sPolyline.spVertices[i].dBulge = 0.0;
			fseek(pFile, lpos, SEEK_SET);
		}
	}
	m_cHatchPolylineArray.Add(sPolyline);
	return TRUE;
}

DXF_BOOL CDXF_Hatch::DXFReadHatchPatterns(FILE *pFile)
{
	DXF_LONG			i, j, lCode;
	SDXF_HatchPattern	sPattern;

	m_spHatchPattern = new SDXF_HatchPattern [m_lHatchPatternCnt];
	if (m_spHatchPattern == NULL) {
		g_lDXFErrorCode = DXF_MEM_ERROR;
		return FALSE;
	}
	for (j = 0; j < m_lHatchPatternCnt; j++) {
		if (!DXFReadDouble(pFile, sPattern.dPatternAngle)) {
			g_lDXFErrorCode = DXF_INVALID_ANGLE;
			return FALSE;
		}
		if (!DXFReadLong(pFile, lCode) || lCode != 43) {
			g_lDXFErrorCode = DXF_INVALID_CODE;
			return FALSE;
		}
		if (!DXFReadDouble(pFile, sPattern.sPatternStart.dX))
			return FALSE;
		if (!DXFReadLong(pFile, lCode) || lCode != 44) {
			g_lDXFErrorCode = DXF_INVALID_CODE;
			return FALSE;
		}
		if (!DXFReadDouble(pFile, sPattern.sPatternStart.dY))
			return FALSE;
		if (!DXFReadLong(pFile, lCode) || lCode != 45) {
			g_lDXFErrorCode = DXF_INVALID_CODE;
			return FALSE;
		}
		if (!DXFReadDouble(pFile, sPattern.sPatternOffset.dX))
			return FALSE;
		if (!DXFReadLong(pFile, lCode) || lCode != 46) {
			g_lDXFErrorCode = DXF_INVALID_CODE;
			return FALSE;
		}
		if (!DXFReadDouble(pFile, sPattern.sPatternOffset.dY))
			return FALSE;
		if (!DXFReadLong(pFile, lCode) || lCode != 79) {
			g_lDXFErrorCode = DXF_INVALID_CODE;
			return FALSE;
		}
		if (!DXFReadLong(pFile, sPattern.lDashCnt)) {
			g_lDXFErrorCode = DXF_INVALID_CODE;
			return FALSE;
		}
		if (sPattern.lDashCnt > 0) {
			sPattern.dpDashes = new DXF_DOUBLE[sPattern.lDashCnt];
			if (sPattern.dpDashes == NULL) {
				g_lDXFErrorCode = DXF_MEM_ERROR;
				return FALSE;
			}
			for (i = 0; i < sPattern.lDashCnt; i++) {
				if (!DXFReadLong(pFile, lCode) || lCode != 49) {
					g_lDXFErrorCode = DXF_INVALID_CODE;
					return FALSE;
				}
				if (!DXFReadDouble(pFile, sPattern.dpDashes[i]))
					return FALSE;
			}
		}
		else {
			sPattern.dpDashes = NULL;
		}
		m_spHatchPattern[j] = sPattern;

		if (j < m_lHatchPatternCnt-1) {
			if (!DXFReadLong(pFile, lCode) || lCode != 53) {
				g_lDXFErrorCode = DXF_INVALID_CODE;
				return FALSE;
			}
		}
	}
	return TRUE;
}

void CDXF_Hatch::DXFGenOCS()
{
	SDXF_Coord3D	sNewCoord3D;

	DXF_GenBCSFromOCS(sNewCoord3D, m_sBCS.sZAxis);

	m_s3DExtrusion = sNewCoord3D.sZAxis;

	// BCS -> WCS
	//DXF_ConvBCS2WCS(m_sBCS, );
	DXF_LONG	i, j, lEdgeIdx;

	for (i = 0; i < m_lSeedPointCnt; i++) {
		SDXF_Point3D	sPoint;
		sPoint.dX = m_spSeedPoints[i].dX;
		sPoint.dY = m_spSeedPoints[i].dY;
		sPoint.dZ = 0.0;
		DXF_ConvBCS2WCS(m_sBCS, sPoint);
		DXF_ConvWCS2BCS(sNewCoord3D, sPoint);
		m_spSeedPoints[i].dX = sPoint.dX;
		m_spSeedPoints[i].dY = sPoint.dY;
		if (i == 0)
			m_s3DHeightPoint.dZ = sPoint.dZ;
	}

	for (i = 0; i < m_lBoundaryPathCnt; i++) {
		for (j = 0; j < m_cHatchPath[i].lEdgeCnt; j++) {
			lEdgeIdx = m_cHatchPath[i].spEdges[j].lIndex;
			switch (m_cHatchPath[i].spEdges[j].lType) {
				case DXF_POLYLINE:
					DXF_ConvNewBCS_PLine(m_sBCS, sNewCoord3D, lEdgeIdx);
					break;
				case DXF_HATCH_EDGE_LINE:
					DXF_ConvNewBCS_Line(m_sBCS, sNewCoord3D, lEdgeIdx);
					break;
				case DXF_HATCH_EDGE_ARC:
					DXF_ConvNewBCS_Arc(m_sBCS, sNewCoord3D, lEdgeIdx);
					break;
				case DXF_HATCH_EDGE_ELLIPARC:
					DXF_ConvNewBCS_EllipArc(m_sBCS, sNewCoord3D, lEdgeIdx);
					break;
				case DXF_HATCH_EDGE_SPLINE:
					DXF_ConvNewBCS_SPline(m_sBCS, sNewCoord3D, lEdgeIdx);
					break;
			}
		}
	}
}

void CDXF_Hatch::DXF_ConvNewBCS_PLine(SDXF_Coord3D& sBCS, SDXF_Coord3D& sNewBCS, DXF_LONG lEdgeIdx)
{
	SDXF_Point3D	sPt;

	for (DXF_LONG i = 0; i < m_cHatchPolylineArray[lEdgeIdx].lVertexCnt; i++) {
		sPt.dX = m_cHatchPolylineArray[lEdgeIdx].spVertices[i].dX;
		sPt.dY = m_cHatchPolylineArray[lEdgeIdx].spVertices[i].dY;
		sPt.dZ = 0.0;
		DXF_ConvBCS2WCS(sBCS, sPt);
		DXF_ConvBCS2WCS(sNewBCS, sPt);
		m_cHatchPolylineArray[lEdgeIdx].spVertices[i].dX = sPt.dX;
		m_cHatchPolylineArray[lEdgeIdx].spVertices[i].dY = sPt.dY;
	}
}

void CDXF_Hatch::DXF_ConvNewBCS_Line(SDXF_Coord3D& sBCS, SDXF_Coord3D& sNewBCS, DXF_LONG lEdgeIdx)
{
	SDXF_Point3D	sPt;

	sPt.dX = m_cHatchLineArray[lEdgeIdx].sStartPt.dX;
	sPt.dY = m_cHatchLineArray[lEdgeIdx].sStartPt.dY;
	sPt.dZ = 0.0;
	DXF_ConvBCS2WCS(sBCS, sPt);
	DXF_ConvWCS2BCS(sNewBCS, sPt);
	m_cHatchLineArray[lEdgeIdx].sStartPt.dX = sPt.dX;
	m_cHatchLineArray[lEdgeIdx].sStartPt.dY = sPt.dY;

	sPt.dX = m_cHatchLineArray[lEdgeIdx].sEndPt.dX;
	sPt.dY = m_cHatchLineArray[lEdgeIdx].sEndPt.dY;
	sPt.dZ = 0.0;
	DXF_ConvBCS2WCS(sBCS, sPt);
	DXF_ConvWCS2BCS(sNewBCS, sPt);
	m_cHatchLineArray[lEdgeIdx].sEndPt.dX = sPt.dX;
	m_cHatchLineArray[lEdgeIdx].sEndPt.dY = sPt.dY;
}

void CDXF_Hatch::DXF_ConvNewBCS_Arc(SDXF_Coord3D& sBCS, SDXF_Coord3D& sNewBCS, DXF_LONG lEdgeIdx)
{
	SDXF_Point3D	sPt;

	sPt.dX = m_cHatchArcArray[lEdgeIdx].sCenter.dX;
	sPt.dY = m_cHatchArcArray[lEdgeIdx].sCenter.dY;
	sPt.dZ = 0.0;
	DXF_ConvBCS2WCS(sBCS, sPt);
	DXF_ConvWCS2BCS(sNewBCS, sPt);
	m_cHatchArcArray[lEdgeIdx].sCenter.dX = sPt.dX;
	m_cHatchArcArray[lEdgeIdx].sCenter.dY = sPt.dY;
}

void CDXF_Hatch::DXF_ConvNewBCS_EllipArc(SDXF_Coord3D& sBCS, SDXF_Coord3D& sNewBCS, DXF_LONG lEdgeIdx)
{
	SDXF_Point3D	sPt;

	sPt.dX = m_cHatchEllipseArray[lEdgeIdx].sCenter.dX;
	sPt.dY = m_cHatchEllipseArray[lEdgeIdx].sCenter.dY;
	sPt.dZ = 0.0;
	DXF_ConvBCS2WCS(sBCS, sPt);
	DXF_ConvWCS2BCS(sNewBCS, sPt);
	m_cHatchEllipseArray[lEdgeIdx].sCenter.dX = sPt.dX;
	m_cHatchEllipseArray[lEdgeIdx].sCenter.dY = sPt.dY;
}

void CDXF_Hatch::DXF_ConvNewBCS_SPline(SDXF_Coord3D& sBCS, SDXF_Coord3D& sNewBCS, DXF_LONG lEdgeIdx)
{
	SDXF_Point3D	sPt;

	for (DXF_LONG i = 0; i < m_cHatchSplineArray[lEdgeIdx].lControlCnt; i++) {
		sPt.dX = m_cHatchSplineArray[lEdgeIdx].spControls[i].dX;
		sPt.dY = m_cHatchSplineArray[lEdgeIdx].spControls[i].dY;
		sPt.dZ = 0.0;
		DXF_ConvBCS2WCS(sBCS, sPt);
		DXF_ConvWCS2BCS(sNewBCS, sPt);
		m_cHatchSplineArray[lEdgeIdx].spControls[i].dX = sPt.dX;
		m_cHatchSplineArray[lEdgeIdx].spControls[i].dY = sPt.dY;
	}
}

