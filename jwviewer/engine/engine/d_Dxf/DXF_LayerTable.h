
#if !defined(AFX_DXFLAYERTABLE_H_)
#define AFX_DXFLAYERTABLE_H_


class CDXF_LayerTable : public CDXF_Table
{
public:
	CDXF_LayerTable();
	virtual ~CDXF_LayerTable();

public:
	virtual DXF_BOOL DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue);

public:
	DXF_CHAR		m_pszLayerName[MAX_STRING_LEN];
	DXF_LONG		m_lLayerCode;
	DXF_LONG		m_lLayerColor;
    DXF_CHAR		m_pszLineTypeName[MAX_STRING_LEN];
};

typedef CKon_Array < CDXF_LayerTable*, CDXF_LayerTable* >	CDXF_LayerArray;

#endif // !defined(AFX_DXFLAYERTABLE_H_)
