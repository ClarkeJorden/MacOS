//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DXF_TABLE_H_)
#define AFX_DXF_TABLE_H_

class CDXF_Table
{
public:
	CDXF_Table();
	virtual ~CDXF_Table();

public:
	DXF_SHORT	m_nTableType;
	DXF_LONG	m_lHandleNumber;

public:
	virtual DXF_BOOL DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue);
};

#endif // !defined(AFX_DXF_TABLE_H_)
