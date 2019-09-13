#if !defined(AFX_DXFDIMSTYLETABLE_H_)
#define AFX_DXFDIMSTYLETABLE_H_

class CDXF_DimStyleTable : public CDXF_Table
{
public:
	CDXF_DimStyleTable();
	virtual ~CDXF_DimStyleTable();

public:
	virtual DXF_BOOL DXFReadTable(FILE* pFile, DXF_LONG& lCode, DXF_CHAR* szValue);

public:
	DXF_CHAR		m_pszDimStyleName[MAX_STRING_LEN];	
	DXF_LONG		m_lDimStyleCode;					
	DXF_CHAR		m_pszDIMPOST[MAX_STRING_LEN];		
	DXF_CHAR		m_pszDIMAPOST[MAX_STRING_LEN];		
	DXF_DOUBLE		m_dDIMSCALE;				
	DXF_DOUBLE		m_dDIMASZ;					

	DXF_DOUBLE		m_dDIMEXO;					
	DXF_DOUBLE		m_dDIMDLI;					
	DXF_DOUBLE		m_dDIMEXE;					
	DXF_DOUBLE		m_dDIMRND;					
	DXF_DOUBLE		m_dDIMDLE;					
	DXF_DOUBLE		m_dDIMTP;					
	DXF_DOUBLE		m_dDIMTM;					
	DXF_DOUBLE		m_dDIMTXT;					
	DXF_DOUBLE		m_dDIMCEN;					
	DXF_DOUBLE		m_dDIMTSZ;					
	DXF_DOUBLE		m_dDIMALTF;					
	DXF_DOUBLE		m_dDIMLFAC;					

	DXF_DOUBLE		m_dDIMTVP;					
	DXF_DOUBLE		m_dDIMTFAC;					
	DXF_DOUBLE		m_dDIMGAP;					

	DXF_LONG		m_lARROWTYPE1;				
	DXF_LONG		m_lARROWTYPE2;				
	DXF_LONG		m_lARROWTYPE3;				

	DXF_LONG		m_lDIMTOL;					
	DXF_LONG		m_lDIMLIM;					
	DXF_LONG		m_lDIMTIH;					

	DXF_LONG		m_lDIMTOH;					

	DXF_LONG		m_lDIMSE1;					
	DXF_LONG		m_lDIMSE2;					
	DXF_LONG		m_lDIMTAD;					
	DXF_LONG		m_lDIMZIN;					
	DXF_LONG		m_lDIMALT;					
	DXF_LONG		m_lDIMALTD;					
	DXF_LONG		m_lDIMTOFL;					
	DXF_LONG		m_lDIMSAH;					
	DXF_LONG		m_lDIMTIX;					
	DXF_LONG		m_lDIMSOXD;					
	DXF_LONG		m_lDIMUNIT;					
	DXF_LONG		m_lDIMDEC;					
	DXF_LONG		m_lDIMTDEC;					
	DXF_LONG		m_lDIMALTU;					
	DXF_LONG		m_lDIMALTTD;				
	DXF_LONG		m_lDIMAUNIT;				
	DXF_LONG		m_lDIMJUST;					
	DXF_LONG		m_lDIMSD1;					
	DXF_LONG		m_lDIMSD2;					
	DXF_LONG		m_lDIMTOLJ;					
	DXF_LONG		m_lDIMTZIN;					
	DXF_LONG		m_lDIMALTZ;					
	DXF_LONG		m_lDIMALTTZ;				
	DXF_LONG		m_lDIMFIT;					
	DXF_LONG		m_lDIMUPT;					
	DXF_LONG		m_lDIMTXSTY;				
	DXF_LONG		m_lDIMCLRD;					
	DXF_LONG		m_lDIMCLRE;					
	DXF_LONG		m_lDIMCLRT;					

protected:
	long				GetArrowType( char * );
	void				GetArrowName( long, char * );
};

typedef CKon_Array < CDXF_DimStyleTable*, CDXF_DimStyleTable* >	CDXF_DimStyleArray;

#endif // !defined(AFX_DXFDIMSTYLETABLE_H_)
