#if !defined(AFX_CAD_ANGULARDIMENTITY_H)
#define AFX_CAD_ANGULARDIMENTITY_H

#include "CAD_DimensionEntity.h"
#include "CAD_GemAngularDim.h"

class  CCAD_AngularDimEntity : public CCAD_DimensionEntity
{
public:
	CCAD_GemAngularDim m_gemEntity;

public:
	CCAD_AngularDimEntity(CCAD_Doc* pDoc, short nSpaceDoc, short nUds);
	virtual ~CCAD_AngularDimEntity();

public:
	virtual CCAD_AngularDimEntity & operator=(const CCAD_AngularDimEntity & cEntity);
	virtual CCAD_GemDimension	*GetGemDimension() { return &m_gemEntity; }

	virtual long ClassSize() { return sizeof(CCAD_AngularDimEntity); }
	virtual long GetSize() { return sizeof(CCAD_AngularDimEntity); }
};

#endif
