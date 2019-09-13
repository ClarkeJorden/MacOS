#if !defined(AFX_DXFVPORTTABLE_H_)
#define AFX_DXFVPORTTABLE_H_

class CDXF_VPortTable : public CDXF_Table
{
public:
	CDXF_VPortTable();
	virtual ~CDXF_VPortTable();

	virtual DXF_BOOL DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue);

	//100	Subclass marker (AcDbViewportTableRecord)
	DXF_CHAR		m_pszVPortName[MAX_STRING_LEN];	
	DXF_LONG		m_lStdCode;						
	SDXF_Point2D	m_sW2DLowerLeftCorner;			
	SDXF_Point2D	m_sW2DUpperRightCorner;			
	SDXF_Point2D	m_sW2DViewCenter;				
	SDXF_Point2D	m_sW2DSnapBasePoint;			
	SDXF_Point2D	m_sW2DSnapSpacing;				
	SDXF_Point2D	m_sW2DGridSpacing;				
	SDXF_Point2D	m_sW2DViewDirFromTargetPoint;	
	SDXF_Point3D	m_sW3DViewTargetPoint;			
	DXF_DOUBLE		m_dViewHeight;					
	DXF_DOUBLE		m_dVPortAspectRatio;			
	DXF_DOUBLE		m_dLensLen;						
	DXF_DOUBLE		m_dFrontClipPlane;				
	DXF_DOUBLE		m_dBackClipPlane;				
	DXF_DOUBLE		m_dSnapRotateAngle;				
	DXF_DOUBLE		m_dViewTwistAngle;				
	DXF_LONG		m_lViewMode;					
													
	DXF_LONG		m_lCircleZoomPercent;			
	DXF_LONG		m_lFastZoomSetting;				
	DXF_LONG		m_lUCSICONSetting;				
	DXF_LONG		m_lSnapOnOff;					
	DXF_LONG		m_lGridOnOff;					
	DXF_LONG		m_lSnapStyle;					
	DXF_LONG		m_lIsopair;						
};

typedef CKon_Array < CDXF_VPortTable*, CDXF_VPortTable* >	CDXF_VPortArray;

#endif // !defined(AFX_DXFVPORTTABLE_H_)
