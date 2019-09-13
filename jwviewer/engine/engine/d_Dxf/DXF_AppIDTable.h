#if !defined(AFX_DXFAPPIDTABLE_H_)
#define AFX_DXFAPPIDTABLE_H_

class CDXF_AppIDTable : public CDXF_Table
{
public:
	CDXF_AppIDTable();
	virtual ~CDXF_AppIDTable();

	virtual DXF_BOOL DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue);

	DXF_CHAR	m_pszAppName[MAX_STRING_LEN];	// Application Name
	DXF_LONG	m_lStdCode;			// Standard Code
};

typedef CKon_Array < CDXF_AppIDTable*, CDXF_AppIDTable* >		CDXF_AppIDArray;

#endif // !defined(AFX_DXFAPPIDTABLE_H_)
