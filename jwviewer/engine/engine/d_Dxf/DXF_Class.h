#if !defined(AFX_DXF_CLASS_H_)
#define AFX_DXF_CLASS_H_

class CDXF_Class  
{
public:
	CDXF_Class();
	CDXF_Class(DXF_CHAR* pszClassName, DXF_CHAR* pszPlusClassName, DXF_CHAR* pszAppName, DXF_LONG lClassVer, DXF_LONG lProxyFlag, DXF_LONG lEntityFlag);
	virtual ~CDXF_Class();

	DXF_CHAR	m_pszClassName[MAX_STRING_LEN];
	DXF_CHAR	m_pszPlusClassName[MAX_STRING_LEN];
	DXF_CHAR	m_pszAppName[MAX_STRING_LEN];
	DXF_LONG	m_lClassVer;
	DXF_LONG	m_lProxyFlag;
	DXF_LONG	m_lEntityFlag;

public:
	BOOL	DXFReadClass(FILE* pFile);
};

typedef CKon_Array < CDXF_Class, CDXF_Class* >		CDXFClassArray;

#endif // !defined(AFX_DXF_CLASS_H_)
