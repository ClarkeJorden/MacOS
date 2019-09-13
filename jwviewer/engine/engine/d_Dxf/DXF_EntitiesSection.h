#if !defined(AFX_IMPORTENTITIESSECTION_H_)
#define AFX_IMPORTENTITIESSECTION_H_


#include "DXF_Entity.h"
#include "DXF_Line.h"
#include "DXF_Arc.h"
#include "DXF_Circle.h"
#include "DXF_Ellipse.h"
#include "DXF_Point.h"
#include "DXF_Text.h"
//#include "DXF_Spline.h"
#include "DXF_Table.h"
#include "DXF_DimStyleTable.h"
//#include "DXF_Dimension.h"
#include "DXF_Image.h"

class CDXF_EntitiesSection  
{
public:
	CDXF_EntitiesSection();
	virtual ~CDXF_EntitiesSection();

public:
	DXF_BOOL DXFReadEntitySkip(FILE* pFile);
	DXF_BOOL DXFReadEntitiesSection(FILE* pFile);
	DXF_LONG DXFCreateEntity(DXF_CHAR* pszType, CDXF_Entity** cpDXFEntity);

public:
	DXF_BOOL blAddEntity(CDXF_Entity* cpEntity);

	DXF_LONG		lGetEntityCount();
	CDXF_Entity*	cpGetEntity(DXF_LONG lIndex);
	DXF_LONG		lGetEntityIdxWithHandle(DXF_LONG lHandle);
	CDXF_Entity*	cpGetEntityWithHandle(DXF_LONG lHandle);

protected:
	CDXF_EntityArray	m_cDXFEntityArray;
};

#endif // !defined(AFX_IMPORTENTITIESSECTION_H_)
