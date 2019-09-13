#if !defined(AFX_DXFVIEWTABLE_H_)
#define AFX_DXFVIEWTABLE_H_

class CDXF_ViewTable : public CDXF_Table
{
public:
	CDXF_ViewTable();
	virtual ~CDXF_ViewTable();

	virtual DXF_BOOL DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue);

public:
	DXF_CHAR		m_pszViewName[MAX_STRING_LEN];	
	DXF_LONG		m_lViewCode;		
										
	SDXF_Point2D	m_sW2DViewCenter;	
	DXF_DOUBLE		m_dViewHeight;		
	DXF_DOUBLE		m_dViewWidth;		

	SDXF_Point3D	m_sW3DViewDir;		

	SDXF_Point3D	m_sW3DTarget;		
	DXF_DOUBLE		m_dLensLen;			
	DXF_DOUBLE		m_dForeClipLen;		
	DXF_DOUBLE		m_dBackClipLen;		
	DXF_DOUBLE		m_dTwistAngle;		
	DXF_LONG		m_lViewMode;		
										
										
										
										
										
};

typedef CKon_Array < CDXF_ViewTable*, CDXF_ViewTable* >	CDXF_ViewArray;

#endif // !defined(AFX_DXFVIEWTABLE_H_)
