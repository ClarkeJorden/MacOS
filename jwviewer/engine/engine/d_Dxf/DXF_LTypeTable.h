#if !defined(AFX_DXFLTYPETABLE_H_)
#define AFX_DXFLTYPETABLE_H_

class CDXF_LTypeTable : public CDXF_Table
{
public:
	CDXF_LTypeTable();
	virtual ~CDXF_LTypeTable();

public:
	DXF_CHAR		m_pszLineTypeName[MAX_STRING_LEN];	
	DXF_LONG		m_lLTypeCode;			
	DXF_CHAR		m_pszLTypeComment[MAX_STRING_LEN];	
	DXF_LONG		m_lAlignmentCode;		
	DXF_LONG		m_lLTypeElemNum;		

	DXF_DOUBLE		m_dTotPatLen;			
	SDXF_LTypeElement*	m_spLTypeElement;	

public:
	virtual DXF_BOOL DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue);

};

typedef CKon_Array < CDXF_LTypeTable*, CDXF_LTypeTable* >	CDXF_LTypeArray;

#endif // !defined(AFX_DXFLTYPETABLE_H_)
