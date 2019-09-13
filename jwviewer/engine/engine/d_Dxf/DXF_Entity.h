#if !defined(AFX_DXFENTITY_H_)
#define AFX_DXFENTITY_H_

class CDXF_Entity
{
public:
	CDXF_Entity();
	virtual ~CDXF_Entity();

public:
	virtual DXF_BOOL	DXFReadEntity(FILE* pFile);
	virtual void		DXFGenBCS();
	virtual void		DXFGenOCS();

public:

	DXF_SHORT		m_nEntityType;
	DXF_LONG		m_lSpaceType;
	DXF_CHAR		m_pszLayerName[MAX_STRING_LEN];
	DXF_CHAR		m_pszLTypeName[MAX_STRING_LEN];
	DXF_DOUBLE		m_dLTypeScale;
	DXF_LONG		m_lVisibility;
	SDXF_Coord3D	m_sBCS;
	DXF_LONG		m_lDXFColor;

	DXF_LONG		m_lHandleNumber;

protected:

	SDXF_Point3D	m_s3DExtrusion;
};

//typedef CKon_Array < CCAD_PtrArray, CDXF_Entity* >		CDXF_EntityArray;
typedef CKon_Array < CDXF_Entity*, CDXF_Entity* >		CDXF_EntityArray;

#endif // !defined(AFX_DXFENTITY_H_)
