
#if !defined(AFX_DXFTEXT_H_)
#define AFX_DXFTEXT_H_

class CDXF_Text : public CDXF_Entity  
{
public:
	CDXF_Text();
	virtual ~CDXF_Text();

public:
	virtual DXF_BOOL	DXFReadEntity(FILE* pFile);
			void		DXFAppendTextStyle(SDXF_TextStyle sDXFTextStyle);

protected:
			void		DXFInterpretText();
			void		DXFExchangeCode(DXF_CHAR* pszCode, DXF_CHAR szCode);

public:
	DXF_DOUBLE			m_dThickness;			
	SDXF_Point3D		m_s3DFirstAlignPoint;	
	DXF_DOUBLE			m_dTextHeight;			
	DXF_CHAR			m_pszText[MAX_STRING_LEN];
	DXF_DOUBLE			m_dRotateAngle;			
	DXF_DOUBLE			m_dRelativeXScale;		
	DXF_DOUBLE			m_dObliqueAngle;		
	DXF_CHAR			m_pszTextStyleName[MAX_STRING_LEN];
	DXF_LONG			m_lTextGenCode;			
												
												
	DXF_LONG			m_lHoriJustCode;		
												
	SDXF_Point3D		m_s3DSecondAlignPoint;	
	DXF_LONG			m_lVertJustCode;		

	DXF_LONG			m_lDXFTextStyle;		

	SDXF_TextStyle*		m_spDXFTextStyle;		
};

#endif // !defined(AFX_DXFTEXT_H_)
