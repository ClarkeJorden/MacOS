#include "stdafx.h"
#include "CAD_Entity.h"
#include "CAD_Block.h"
#include "CAD_BlockEntity.h"
#include "CAD_Doc.h"
#include "CAD_Uds.h"

CCAD_Block::CCAD_Block(HANDLE hHeap, CCAD_Doc * pDoc) : m_cMgrEntity(hHeap, pDoc, -1, -1, false)
{
	Init();
}

CCAD_Block::~CCAD_Block()
{
}

void CCAD_Block::Init()
{
	CCAD_Style::Init();

	m_szPart[0] = '\0';
	m_szDescript[0] = '\0';
	m_lSize = 0;
}

void CCAD_Block::CopyFrom(const CCAD_Style *pStyle)
{
	CCAD_Style::CopyFrom(pStyle);

	strcpy( m_szPart, ((CCAD_Block *)pStyle)->m_szPart );
	strcpy( m_szDescript, ((CCAD_Block *)pStyle)->m_szDescript );
	m_cRc = ((CCAD_Block *)pStyle)->m_cRc;
	m_lSize = ((CCAD_Block *)pStyle)->m_lSize;

	m_cMgrEntity.RemoveAll();
	m_cMgrEntity.CopyFromOther( ((CCAD_Block *)pStyle)->m_cMgrEntity );
}

BOOL CCAD_Block::Read(CCAD_File & cFile)
{
	CCAD_Style::Read(cFile);
	cFile.ReadString(m_szPart, LENGTH_DSCRIPT );
	cFile.ReadString(m_szDescript, LENGTH_DSCRIPT );
	m_cRc.Read(cFile);
	cFile.ReadLong(&m_lSize);
	return m_cMgrEntity.Read(cFile, true);
}

BOOL CCAD_Block::IsIncludeEqualBlock(LPCSTR name)
{
	long pos;
	CCAD_Entity	* pEntity = m_cMgrEntity.GetFirstEntity(pos);
	while (pEntity != NULL) {
		if (pEntity->m_bCode == C_BLOCK) {
			if( strcmp( ((CCAD_BlockEntity*)pEntity)->m_cGemBlock.m_szName, name) == 0) {
				return true;
			}

			CCAD_Block	*pBlock = ((CCAD_BlockEntity*)pEntity)->m_cGemBlock.GetBlock();
			if (pBlock->IsIncludeEqualBlock(name))
				return true;
		}
		pEntity = m_cMgrEntity.GetNextEntity(pos);
	}
	return false;
}


