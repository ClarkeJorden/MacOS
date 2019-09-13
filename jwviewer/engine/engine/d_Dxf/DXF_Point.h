#if !defined(AFX_DXFPOINT_H_)
#define AFX_DXFPOINT_H_

class CDXF_Point : public CDXF_Entity  
{
public:
	CDXF_Point();
	virtual ~CDXF_Point();

public:
	virtual DXF_BOOL	DXFReadEntity(FILE* pFile);
	virtual void		DXFGenOCS();
	virtual void		DXFGenBCS();

public:
	SDXF_Point3D	m_s3DCoord;

	DXF_DOUBLE		m_dThickness;	
	DXF_DOUBLE		m_dAngle;
};

#endif // !defined(AFX_DXFPOINT_H_)
