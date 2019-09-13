
#if !defined(AFX_DXF_OBJECT_H_)
#define AFX_DXF_OBJECT_H_


class CDXF_Object  
{
public:
	CDXF_Object();
	virtual ~CDXF_Object();

public:
	DXF_SHORT	m_nObjectType;
	DXF_LONG	m_lHandleNumber;

	virtual DXF_BOOL DXFReadObject(FILE* pFile);
};

#endif // !defined(AFX_DXF_OBJECT_H_)
