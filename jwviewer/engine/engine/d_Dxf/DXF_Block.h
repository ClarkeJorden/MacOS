
#if !defined(AFX_DXFBLOCK_H_)
#define AFX_DXFBLOCK_H_

#include "DXF_Entity.h"
#include "DXF_Line.h"
#include "DXF_Arc.h"
#include "DXF_Circle.h"
#include "DXF_Ellipse.h"
#include "DXF_Point.h"
#include "DXF_Text.h"
//#include "DXF_Spline.h"
#include "DXF_DimStyleTable.h"
//#include "DXF_Dimension.h"
#include "DXF_Image.h"

class CDXF_Block  
{
public:
	CDXF_Block();
	virtual ~CDXF_Block();

public:
	DXF_BOOL	DXFReadBlock(FILE* fp);
	DXF_BOOL	DXFReadBlockEntitySkip(FILE* fp);

public:
	DXF_LONG		m_lHandleNumber;
	DXF_LONG		m_lEndHandleNumber;
	//
	DXF_CHAR		m_pszLayerName[MAX_STRING_LEN];
	//
	DXF_CHAR		m_pszBlockName[MAX_STRING_LEN];
	DXF_LONG		m_lBlockCode;
	SDXF_Point3D	m_sW3DInsertPoint;
	DXF_CHAR		m_pszBlockName1[MAX_STRING_LEN];
	DXF_CHAR		m_pszXRefPath[MAX_STRING_LEN];
	DXF_CHAR		m_pszBlockDescription[MAX_STRING_LEN];

public:
	DXF_BOOL		blAddEntity(CDXF_Entity* cpEntity);
	DXF_LONG		lGetEntityCount();
	CDXF_Entity*	cpGetEntity(DXF_LONG nIndex);

protected:
	CDXF_EntityArray	m_cEntityArray;
};

typedef CKon_Array < CDXF_Block*, CDXF_Block* >		CDXF_BlockArray;

#endif // !defined(AFX_DXFBLOCK_H_)
