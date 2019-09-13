#if !defined(AFX_DXFHATCH_H_)
#define AFX_DXFHATCH_H_

class CDXF_Hatch : public CDXF_Entity  
{
public:
	CDXF_Hatch();
	virtual ~CDXF_Hatch();

public:
	virtual DXF_BOOL	DXFReadEntity(FILE* pFile);
	virtual void		DXFGenOCS();

			DXF_BOOL	DXFReadHatchPatterns(FILE* pFile);

public:
	SDXF_Point3D	m_s3DHeightPoint;
	DXF_CHAR		m_pszHatchPatternName[MAX_STRING_LEN];
	DXF_LONG		m_lSolidFillCode;
	DXF_LONG		m_lAssociativityCode;
	DXF_LONG		m_lHatchStyle;
	DXF_LONG		m_lHatchPatternType;
	// Hatch Pattern Angle
	DXF_DOUBLE		m_dHatchPatternAngle;
	// Hatch pattern
	DXF_DOUBLE		m_dHatchPatternScaleOrOffset;
	// Hatch pattern
	// 1 - double
	// 0 - non double
	DXF_LONG		m_lHatchPatternDoubleCode;

	DXF_LONG		m_lBoundaryPathCnt;
	// Pixel
	DXF_DOUBLE		m_dPixelSize;

	DXF_LONG		m_lSeedPointCnt;
	SDXF_Point2D*	m_spSeedPoints;
// boundary path data

	DXF_LONG		m_lBoundaryObjectCnt;
	CDXF_LongArray		m_cObjectRefs;
// edge data
	// 
	CHatchLineArray		m_cHatchLineArray;
	CHatchArcArray		m_cHatchArcArray;
	CHatchPolylineArray	m_cHatchPolylineArray;
	CHatchEllipseArray	m_cHatchEllipseArray;
	CHatchSplineArray	m_cHatchSplineArray;

	CHatchPathArray		m_cHatchPath;

// pattern data

	DXF_LONG			m_lHatchPatternCnt;
	SDXF_HatchPattern*	m_spHatchPattern;

protected:

protected:
	DXF_BOOL	DXFReadHatchBoundPolyline(FILE* pFile);
	DXF_BOOL	DXFReadHatchBoundSpline(FILE* pFile);
	DXF_BOOL	DXFReadHatchBoundEllipse(FILE* pFile);
	DXF_BOOL	DXFReadHatchBoundArc(FILE* pFile);
	DXF_BOOL	DXFReadHatchBoundLine(FILE* pFile);
	DXF_BOOL	DXFReadHatchBoundaryPath(FILE* pFile, DXF_LONG lPathCnt);

	void		DXF_ConvNewBCS_PLine(SDXF_Coord3D& sBCS, SDXF_Coord3D& sNewBCS, DXF_LONG lEdgeIdx);
	void		DXF_ConvNewBCS_Line(SDXF_Coord3D& sBCS, SDXF_Coord3D& sNewBCS, DXF_LONG lEdgeIdx);
	void		DXF_ConvNewBCS_Arc(SDXF_Coord3D& sBCS, SDXF_Coord3D& sNewBCS, DXF_LONG lEdgeIdx);
	void		DXF_ConvNewBCS_EllipArc(SDXF_Coord3D& sBCS, SDXF_Coord3D& sNewBCS, DXF_LONG lEdgeIdx);
	void		DXF_ConvNewBCS_SPline(SDXF_Coord3D& sBCS, SDXF_Coord3D& sNewBCS, DXF_LONG lEdgeIdx);
};

#endif // AFX_DXFHATCH_H_
