
#if !defined(AFX_DXFLINE_H_)
#define AFX_DXFLINE_H_


class CDXF_Line : public CDXF_Entity  
{
public:
	CDXF_Line();
	virtual ~CDXF_Line();

public:
	virtual DXF_BOOL	DXFReadEntity(FILE* pFile);
	virtual void		DXFGenOCS();
	virtual void		DXFGenBCS();

public:
	DXF_DOUBLE		m_dThickness;		
	SDXF_Point3D	m_s3DStartPoint;	
	SDXF_Point3D	m_s3DEndPoint;		
};

#endif // !defined(AFX_DXFLINE_H_)
