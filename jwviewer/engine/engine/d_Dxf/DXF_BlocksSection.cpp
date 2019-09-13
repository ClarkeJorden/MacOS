#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Table.h"
#include "DXF_BlocksSection.h"

CDXF_BlocksSection::CDXF_BlocksSection()
{
	m_cDXFBlockArray.RemoveAll();
}

CDXF_BlocksSection::~CDXF_BlocksSection()
{
	DXF_LONG	lSize = m_cDXFBlockArray.GetSize();

	for (DXF_LONG i = 0; i < lSize; i++)
	{
		delete m_cDXFBlockArray[i];
	}
	m_cDXFBlockArray.RemoveAll();
}

DXF_BOOL CDXF_BlocksSection::DXFReadBlocksSection(FILE *pFile)
{
	DXF_LONG	lCode;
	DXF_CHAR	szValue[MAX_STRING_LEN];

	do
	{
		if (!DXFReadGroup(pFile, lCode, szValue))
			return FALSE;
		if (lCode == 0 && strcmp(szValue, "BLOCK") == 0)
		{
			CDXF_Block*	pBlock = new CDXF_Block;

			if (pBlock == NULL)
			{
				g_lDXFErrorCode = DXF_MEM_ERROR;
				return FALSE;
			}
			if (!pBlock->DXFReadBlock(pFile))
			{
				delete pBlock;
				return FALSE;
			}
			if (!blAddBlock(pBlock))
			{
				delete pBlock;
				return FALSE;
			}
		}
	} while (lCode != 0 || strcmp(szValue, "ENDSEC") != 0);

	return TRUE;
}

DXF_LONG CDXF_BlocksSection::lGetBlockCount()
{
	return m_cDXFBlockArray.GetSize();
}

DXF_BOOL CDXF_BlocksSection::blAddBlock(CDXF_Block* cpBlock)
{
	m_cDXFBlockArray.Add(cpBlock);

	return TRUE;
}

CDXF_Block* CDXF_BlocksSection::cpGetBlock(DXF_LONG lIndex)
{
	DXF_LONG	lSize = m_cDXFBlockArray.GetSize();

	if (lIndex >= lSize)
		return NULL;
	if (lIndex < 0)
		return NULL;
	return m_cDXFBlockArray[lIndex];
}
