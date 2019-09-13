#include "stdafx.h"
#include "CAD_TextEntity.h"
#include "CAD_Doc.h"

CCAD_TextEntity::CCAD_TextEntity(CCAD_Doc * pDoc, short nSpaceDoc, short nUds)
					: CCAD_Entity(pDoc, nSpaceDoc, nUds)
{
	m_bCode = C_TEXT;
	m_bFlag = E_DRAWING;
}

CCAD_TextEntity::~CCAD_TextEntity()
{
}

BOOL CCAD_TextEntity::IsIgno(BOOL isBlock)
{
	if ( CCAD_Entity::IsIgno(isBlock) )
		return true;

	CCAD_GemText *pGem = (CCAD_GemText *)GetGemEntity();

	if (pGem->m_fHeight <= 0 || pGem->m_fWidth <= 0 || pGem->m_nLength <= 0 ||
			pGem->m_pStr == NULL || pGem->m_nStyle < 0 || pGem->m_pTextStyle == NULL)
		return TRUE;
	else if (pGem->m_pStr[0] == 0)
		return TRUE;
	else {	//	for Space string.
		for (short i = 0; i< pGem->m_nLength; i++)
			if( pGem->m_pStr[i] != ' ' )
				return FALSE;
		return TRUE;
	}
	return FALSE;
}

void CCAD_TextEntity::MemCopyTo(CCAD_Entity * pEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	ASSERT(pEntity != NULL);
	m_cGemText.RemoveArray();
	memmove(pEntity, this, ClassSize());

	// Reset src text's m_pTextStyle for networking
	m_cGemText.m_pTextStyle = (CCAD_TextStyle *) m_pDoc->GetStyleMgr(SS_TEXT)->GetStyle(m_cGemText.m_nStyle);

	m_cGemText.MemCopyTo(pEntity->GetGemEntity(), pData1, pData2, pData3);
}

BOOL CCAD_TextEntity::Read(CCAD_File & cFile)
{
	CCAD_Entity::Read(cFile);

	m_cPen.Read(cFile);
	return m_cGemText.Read(cFile);
}

long CCAD_TextEntity::GetSize()
{
	long lSize = sizeof(CCAD_TextEntity);
	lSize += sizeof(BYTE)*m_cGemText.m_nLength;
	return lSize;
}

LPSTR CCAD_TextEntity::GetData1(LPSTR pEntity)
{
	return (pEntity + sizeof(CCAD_TextEntity));
}


BOOL CCAD_TextEntity::Draw(CCAD_DC* pCadDC, BOOL blRedraw)
{
	CCAD_GemEntity* pGemEntity = GetGemEntity();
	if (pGemEntity == NULL)
		return TRUE;

	LOGPEN	sLogPen;
	LOGPEN oldPen;
	bool blGet;
	if (blGet = GetLogPen(pCadDC, sLogPen))
		oldPen = pCadDC->SelectPen(sLogPen);
	LOGBRUSH oldBrush = pCadDC->SelectColorBrush(sLogPen.lopnColor);

	COLORREF	rgbOldColor = pCadDC->SetTextColor( sLogPen.lopnColor );

	BOOL blResult = m_cGemText.Drawing(pCadDC, E_DRAWING, blRedraw);

	pCadDC->SetTextColor( rgbOldColor );

	pCadDC->RestoreObject(oldBrush);
	
	if ( blGet )
		pCadDC->RestoreObject(oldPen);
	return blResult;
}

void CCAD_TextEntity::RemoveLocal()
{
	m_cGemText.RemoveArray();
}


