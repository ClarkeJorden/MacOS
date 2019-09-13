#if !defined(AFX_DXFIMAGEDEFOBJECT_H_)
#define AFX_DXFIMAGEDEFOBJECT_H_

class CDXF_ImageDefObject : public CDXF_Object
{
public:
	CDXF_ImageDefObject();
	CDXF_ImageDefObject(DXF_CHAR* szImageName, SDXF_Point2D& sImageSize, SDXF_Point2D& sPixelSize);

	virtual ~CDXF_ImageDefObject();

public:
	virtual DXF_BOOL DXFReadObject(FILE* pFile);

public:
	DXF_LONG		m_lClassVersion;
	DXF_CHAR		m_pszImageFileName[MAX_STRING_LEN];
	SDXF_Point2D	m_sW2DImageSize;
	SDXF_Point2D	m_sW2DOnePixelSize;
	DXF_LONG		m_lLoadFlag;
	DXF_LONG		m_lResolutionUnit;
};

typedef CKon_Array < CDXF_ImageDefObject*, CDXF_ImageDefObject* >	CDXF_ImageDefArray;

#endif // !defined(AFX_DXFIMAGEDEFOBJECT_H_)
