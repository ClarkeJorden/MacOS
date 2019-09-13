#include "stdafx.h"
#include "CAD_LineEntity.h"
#include "CAD_Doc.h"

CCAD_LineEntity::CCAD_LineEntity(CCAD_Doc * pDoc, short nSpaceDoc, short nUds)
					: CCAD_Entity(pDoc, nSpaceDoc, nUds)
{
	m_bCode = C_LINE;
	m_bFlag = E_DRAWING;
}

CCAD_LineEntity::~CCAD_LineEntity()
{
}

CCAD_LineEntity::CCAD_LineEntity(const CCAD_LineEntity & cLine)
{
	*this = cLine;
}

CCAD_LineEntity & CCAD_LineEntity::operator=(const CCAD_LineEntity & cLine)
{
	CCAD_Entity::operator =(cLine);
	m_cPen = cLine.m_cPen;
	m_cGemLine = cLine.m_cGemLine;
	return *this;
}

long CCAD_LineEntity::GetSize()
{
	return (long)sizeof(CCAD_LineEntity);
}

BOOL CCAD_LineEntity::Read(CCAD_File& cFile)
{
	CCAD_Entity::Read(cFile);
	m_cPen.Read(cFile);
	return m_cGemLine.Read(cFile);
}

BOOL CCAD_LineEntity::IsIgno(BOOL isBlock)
{
	if ( CCAD_Entity::IsIgno(isBlock) )
		return true;

	CCAD_GemLine *pGem = (CCAD_GemLine *)GetGemEntity();

	if( pGem->m_cSp == pGem->m_cEp )
		return TRUE;
	return FALSE;
} 
