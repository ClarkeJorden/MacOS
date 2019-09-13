#if !defined(AFX_DXFSTYLETABLE_H_)
#define AFX_DXFSTYLETABLE_H_

class CDXF_StyleTable : public CDXF_Table
{
public:
	CDXF_StyleTable();
	virtual ~CDXF_StyleTable();

public:
	virtual DXF_BOOL DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue);

public:
	DXF_CHAR	m_pszStyleName[MAX_STRING_LEN];	
	DXF_LONG	m_lStyleCode;				
											
											

	DXF_DOUBLE	m_dFixedTextHeight;			
	DXF_DOUBLE	m_dWidthFactor;				

	DXF_DOUBLE	m_dObliqueAngle;			
	DXF_LONG	m_lTextGenCode;				
											
											
	DXF_DOUBLE	m_dLastTextHeight;			
	DXF_CHAR	m_pszFirstFontFileName[MAX_STRING_LEN];	
	DXF_CHAR	m_pszBigFontFileName[MAX_STRING_LEN];	
	DXF_CHAR	m_pszFontFaceName[MAX_STRING_LEN];		
};

typedef CKon_Array < CDXF_StyleTable*, CDXF_StyleTable* >	CDXF_StyleArray;

#endif // !defined(AFX_DXFSTYLETABLE_H_)
