
#if !defined(AFX_DXFCIRCLE_H_)
#define AFX_DXFCIRCLE_H_


class CDXF_Circle : public CDXF_Entity  
{
public:
	CDXF_Circle();
	virtual ~CDXF_Circle();

public:
	virtual DXF_BOOL	DXFReadEntity(FILE* pFile);
	virtual void	DXFGenOCS();

public:
	DXF_DOUBLE		m_dThickness;
	SDXF_Point3D	m_s3DCenter;

	DXF_DOUBLE		m_dRadius;
};

#endif // !defined(AFX_DXFCIRCLE_H_)
