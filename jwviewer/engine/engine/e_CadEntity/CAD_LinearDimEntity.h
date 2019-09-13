#if !defined(AFX_CAD_LINEARDIMENTITY_H)
#define AFX_CAD_LINEARDIMENTITY_H

#include "CAD_DimensionEntity.h"
#include "CAD_GemLinearDim.h"

class  CCAD_LinearDimEntity : public CCAD_DimensionEntity
{
public:
	CCAD_GemLinearDim m_gemEntity;

public:
	CCAD_LinearDimEntity(CCAD_Doc* pDoc, short nSpaceDoc, short nUds);
	virtual ~CCAD_LinearDimEntity();

public:
	virtual CCAD_LinearDimEntity & operator=(const CCAD_LinearDimEntity & cEntity);
	virtual CCAD_GemDimension	*GetGemDimension() { return &m_gemEntity; }

	virtual long	ClassSize() { return sizeof(CCAD_LinearDimEntity); }
	virtual long	GetSize()  { return sizeof(CCAD_LinearDimEntity); }
};

#endif
