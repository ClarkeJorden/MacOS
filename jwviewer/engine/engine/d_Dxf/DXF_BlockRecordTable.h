#if !defined(AFX_DXFBLOCKRECORDTABLE_H_)
#define AFX_DXFBLOCKRECORDTABLE_H_

class CDXF_BlockRecordTable : public CDXF_Table
{
public:
	CDXF_BlockRecordTable();
	CDXF_BlockRecordTable(DXF_CHAR* szBlockName);
	virtual ~CDXF_BlockRecordTable();

	virtual DXF_BOOL DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue);

public:
	DXF_CHAR	m_pszBlockName[MAX_STRING_LEN];
};

typedef CKon_Array < CDXF_BlockRecordTable*, CDXF_BlockRecordTable* >	CDXF_BlockRecordArray;

#endif // !defined(AFX_DXFBLOCKRECORDTABLE_H_)
