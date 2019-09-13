#if !defined(AFX_DXFELLIPSE_H_)
#define AFX_DXFELLIPSE_H_

class CDXF_Ellipse : public CDXF_Entity  
{
public:
	CDXF_Ellipse();
	virtual ~CDXF_Ellipse();

public:
	virtual DXF_BOOL	DXFReadEntity(FILE* pFile);
	virtual void	DXFGenOCS();

public:
	SDXF_Point3D	m_s3DCenter;			
	SDXF_Point3D	m_s3DMajorAxisEndPoint;	
	DXF_DOUBLE		m_dThickness;			
	DXF_DOUBLE		m_dScale;				

	DXF_DOUBLE		m_dStartAngle;			
	DXF_DOUBLE		m_dEndAngle;			
};

#endif // !defined(AFX_DXFELLIPSE_H_)
