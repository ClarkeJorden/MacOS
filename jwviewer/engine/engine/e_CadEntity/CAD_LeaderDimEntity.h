#if !defined(AFX_CAD_LEADERDIMENTITY_H)
#define AFX_CAD_LEADERDIMENTITY_H

#include "CAD_DimensionEntity.h"
#include "CAD_GemLeaderDim.h"

class  CCAD_LeaderDimEntity : public CCAD_DimensionEntity
{
public:
	CCAD_GemLeaderDim m_gemEntity;

public:
	CCAD_LeaderDimEntity(CCAD_Doc* pDoc, short nSpaceDoc, short nUds);
	virtual ~CCAD_LeaderDimEntity();

public:
	virtual CCAD_GemDimension	*GetGemDimension() { return &m_gemEntity; }

	virtual long	ClassSize() { return sizeof(CCAD_LeaderDimEntity); }
	virtual long	GetSize();

	virtual LPSTR	GetData1(LPSTR pData);
};

#endif
