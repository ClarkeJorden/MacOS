#if !defined(AFX_CAD_RADIALDIMENTITY_H)
#define AFX_CAD_RADIALDIMENTITY_H

#include "CAD_DimensionEntity.h"
#include "CAD_GemRadialDim.h"

class  CCAD_RadialDimEntity : public CCAD_DimensionEntity  
{
public:
	CCAD_GemRadialDim m_gemEntity;

public:
	CCAD_RadialDimEntity(CCAD_Doc* pDoc, short nSpaceDoc, short nUds);
	virtual ~CCAD_RadialDimEntity();

public:
	virtual CCAD_RadialDimEntity & operator=(const CCAD_RadialDimEntity & cEntity);
	virtual CCAD_GemDimension	*GetGemDimension() { return &m_gemEntity; }

	virtual long	ClassSize() { return sizeof(CCAD_RadialDimEntity); }
	virtual long	GetSize() { return sizeof(CCAD_RadialDimEntity); }
};

#endif
