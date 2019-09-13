#if !defined(AFX_IMPORTOBJECTSSECTION_H_)
#define AFX_IMPORTOBJECTSSECTION_H_

#include "DXF_Object.h"
//#include "DXF_GroupObject.h"
#include "DXF_ImageDefObject.h"
#include "DXF_EntitiesSection.h"

class CDXF_ObjectsSection  
{
public:
	CDXF_ObjectsSection();
	virtual ~CDXF_ObjectsSection();

public:
	DXF_BOOL DXFReadObjectsSection(FILE* pFile);

public:
	DXF_BOOL				blAddObject(CDXF_Object* cpDXFObject);
	DXF_LONG				lGetMLineStyleCnt();
	DXF_LONG				lGetGroupCnt();
	DXF_LONG				lGetImageDefCnt();

	CDXF_ImageDefObject*	cpGetImageDef(DXF_LONG nIndex);
	//CDXF_GroupObject*		cpGetGroup(DXF_LONG nIndex);

	//CDXF_GroupObject*		cpFindGroup(DXF_LONG nHandle);
	CDXF_ImageDefObject*	cpFindImageDef(DXF_LONG nHandle);
	CDXF_ImageDefObject*	cpFindImageDef(DXF_CHAR* pszFileName);
	DXF_LONG				FindMLineStyleHandle(DXF_CHAR* pszName);

public:
	DXF_BOOL blAddImageDef(CDXF_ImageDefObject* cpImageDef);
	//DXF_BOOL blAddGroup(CDXF_GroupObject* cpGroup);

protected:
	// ImageDef

	CDXF_ImageDefArray		m_cDXFImageDefArray;

// 	CDXF_MLineStyleArray	m_cDXFMLineStyleArray;
// 	CDXF_GroupArray		m_cDXFGroupArray;

protected:
	DXF_BOOL DXFReadObjectsCommonDic(FILE* pFile, DXF_LONG& nCode, DXF_CHAR* szString);
	DXF_BOOL DXFReadObjectsDic(FILE* pFile, DXF_LONG& nCode, DXF_CHAR* szString);

	DXF_BOOL blRemoveAllObject();
};

#endif // !defined(AFX_IMPORTOBJECTSSECTION_H_)
