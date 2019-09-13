#if !defined(AFX_DXFUCSTABLE_H_)
#define AFX_DXFUCSTABLE_H_

class CDXF_UCSTable : public CDXF_Table
{
public:
	CDXF_UCSTable();
	virtual ~CDXF_UCSTable();

	virtual DXF_BOOL DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue);

public:
	DXF_CHAR		m_pszUCSName[MAX_STRING_LEN];	
	DXF_LONG		m_lUCSCode;			
	SDXF_Point3D	m_sW3DOrgPoint;		
	SDXF_Point3D	m_sW3DXAxis;		

	SDXF_Point3D	m_sW3DYAxis;		

};

typedef CKon_Array < CDXF_UCSTable*, CDXF_UCSTable* >	CDXF_UCSArray;

#endif // !defined(AFX_DXFUCSTABLE_H_)
