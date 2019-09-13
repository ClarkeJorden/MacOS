
#if !defined(AFX_DXFARC_H_)
#define AFX_DXFARC_H_

class CDXF_Arc : public CDXF_Entity
{
public:
	CDXF_Arc();
	virtual ~CDXF_Arc();

public:
	virtual DXF_BOOL	DXFReadEntity(FILE* pFile);
	virtual void	DXFGenOCS();

public:
	DXF_DOUBLE		m_dThickness;
	SDXF_Point3D	m_s3DCenter;
	DXF_DOUBLE		m_dRadius;
	DXF_DOUBLE		m_dStartAngle;
	DXF_DOUBLE		m_dEndAngle;
};

#endif // !defined(AFX_DXFARC_H_)
