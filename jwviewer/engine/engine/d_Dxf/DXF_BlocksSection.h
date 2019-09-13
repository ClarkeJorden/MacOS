
#if !defined(AFX_IMPORTBLOCKSSECTION_H_)
#define AFX_IMPORTBLOCKSSECTION_H_


#include "DXF_Block.h"
#include "DXF_TablesSection.h"
#include "DXF_ObjectsSection.h"

class CDXF_BlocksSection  
{
public:
	CDXF_BlocksSection();
	virtual ~CDXF_BlocksSection();

	DXF_BOOL	DXFReadBlocksSection(FILE* fp);

public:
	DXF_LONG	lGetBlockCount();
	DXF_BOOL	blAddBlock(CDXF_Block* cpBlock);
	CDXF_Block*	cpGetBlock(DXF_LONG lIndex);

protected:
	CDXF_BlockArray		m_cDXFBlockArray;
};

#endif // !defined(AFX_IMPORTBLOCKSSECTION_H_)
