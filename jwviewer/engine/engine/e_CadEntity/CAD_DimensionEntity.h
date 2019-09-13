#if !defined(AFX_CAD_DIMENSIONENTITY_H)
#define AFX_CAD_DIMENSIONENTITY_H

#include "CAD_Entity.h"
#include "CAD_GemDimension.h"

class  CCAD_DimensionEntity : public CCAD_Entity
{
public:
	CCAD_DimensionEntity(CCAD_Doc* pDoc, short nSpaceDoc, short nUds);
	virtual ~CCAD_DimensionEntity();

public:
	virtual BOOL Read(CCAD_File& cFile);

	virtual	CCAD_GemEntity		*GetGemEntity();
	virtual CCAD_GemDimension	*GetGemDimension() { return NULL; }

	CCAD_Block* CreateDimensionBlock();
};

#endif
