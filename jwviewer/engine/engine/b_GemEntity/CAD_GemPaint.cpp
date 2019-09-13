#include "stdafx.h"
#include "CAD_GemPaint.h"

CCAD_GemPaint::CCAD_GemPaint()
{
	m_pTriPoints = NULL;
	m_nTriCount = 0;
}

CCAD_GemPaint::~CCAD_GemPaint()
{
	if(m_pTriPoints)
		delete[] m_pTriPoints;
}

void CCAD_GemPaint::Remove()
{
	RemoveFrameAll();
}

BOOL CCAD_GemPaint::Draw(CCAD_DC *pDC, BOOL blRedraw)
{
	if (m_nTotGrp == 0 || m_pNum == NULL || m_pPt == NULL)
		return FALSE;

	BOOL	blPaint = FALSE;
	if (m_bDraw & DRAW_PAINT) {
		if (!pDC->m_pCamera->m_bPerspect) {
			CCAD_Point	*pPt = new CCAD_Point[m_nTotPt];
			memcpy(pPt, m_pPt, sizeof(CCAD_Point)*m_nTotPt);
			
			LOGPEN oldPen = pDC->SelectNullPen();
			if(m_nTriCount == 0)
				m_pTriPoints = pDC->GetTrianglesFromPolygon(pPt, m_pNum, m_nTotGrp, m_nTriCount);
			pDC->PolyPolygon(pPt, m_pNum, m_nTotGrp, m_pTriPoints, m_nTriCount);
			pDC->RestoreObject(oldPen);
			
			delete []pPt;
			blPaint = TRUE;
		}
	}
	if ((m_bDraw & DRAW_FRAME) || !blPaint)
		return CCAD_GemFrame::Draw(pDC, blRedraw);

	LOGPEN oldPen = pDC->SelectNullPen();
	CCAD_GemFrame::Draw(pDC, blRedraw);
	pDC->RestoreObject(oldPen);

	return TRUE;
}

//-----------------------------------------------------
void CCAD_GemPaint::CountBodyRange(CCAD_Rect& cRange)
{
	if (m_nTotGrp == 0 || m_nTotPt < 3 || m_pNum == NULL || m_pPt == NULL)
		return;

	CCAD_GemThickEntity::CountBodyRange(cRange);
	CCAD_GemFrame::CountBodyRange(cRange);
}

//-----------------------------------------------------
void CCAD_GemPaint::CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange)
{
	ASSERT(pCadDC != NULL);

	if (m_nTotGrp == 0 || m_nTotPt < 3 || m_pNum == NULL || m_pPt == NULL)
		return;

	CCAD_GemFrame::CountVcsRange(pCadDC, cRange);
}
