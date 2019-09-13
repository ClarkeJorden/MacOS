
#if !defined(AFX_IMPORTCLASSESSECTION_H_)
#define AFX_IMPORTCLASSESSECTION_H_

class CDXF_ClassesSection  
{
public:
	CDXF_ClassesSection();
	virtual ~CDXF_ClassesSection();

public:
	DXF_BOOL DXFReadClassesSection(FILE* fp);

};

#endif // !defined(AFX_IMPORTCLASSESSECTION_H_)
