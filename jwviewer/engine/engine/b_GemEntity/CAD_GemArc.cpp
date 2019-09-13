#include "stdafx.h"
#include "CAD_GemArc.h"

CCAD_GemArc::CCAD_GemArc()
{
	m_bType = ARC_NORMAL;
	m_bDraw = DRAW_FRAME;
	m_fR = 0;
	m_cAlfa = 0;
	m_cDelta = LONGDOUBLE_NINETY;
	m_pTriPoints = NULL;
	m_nTriCount = 0;
}

CCAD_GemArc::~CCAD_GemArc()
{
	if(m_pTriPoints)
		delete[] m_pTriPoints;
}

CCAD_GemArc::CCAD_GemArc(const CCAD_Coord3D & cBcs, const CCAD_Point & cCp, const CAD_FLOAT &fR)
{
	m_bDraw = DRAW_FRAME;
	m_bType = ARC_NORMAL;
	m_cBcs = cBcs;
	m_cCp = cCp;
	m_fR = fR;
	m_cAlfa = 0;
	m_cDelta = LONGDOUBLE_NINETY;
	m_pTriPoints = NULL;
	m_nTriCount = 0;
}

//-----------------------------------------------------
CCAD_GemArc::CCAD_GemArc(CCAD_GemCircle *pCircle)
{
	m_bDraw = pCircle->m_bDraw;
	m_cBcs = pCircle->m_cBcs;
	m_cThick = pCircle->m_cThick;
	m_cCp = pCircle->m_cCp;
	m_fR = pCircle->m_fR;
	m_cAlfa = 0;
	m_cDelta = LONGDOUBLE_NINETY;
	if (pCircle->m_bDraw & DRAW_PAINT)
		m_bType = ARC_PIE;
	else
		m_bType = ARC_NORMAL;
	m_pTriPoints = NULL;
	m_nTriCount = 0;
}

//-----------------------------------------------------
CCAD_GemArc::CCAD_GemArc(const CCAD_GemCircle & cCircle, const CCAD_Angle & cAlfa, const CCAD_Angle & cDelta, BYTE bType)
{
	m_bDraw = cCircle.m_bDraw;
	m_bType = bType;
	m_cBcs = cCircle.m_cBcs;
	m_cThick = cCircle.m_cThick;
	m_cCp = cCircle.m_cCp;
	m_fR = cCircle.m_fR;
	m_cAlfa = cAlfa;
	m_cDelta = cDelta;
	m_pTriPoints = NULL;
	m_nTriCount = 0;
}

//-----------------------------------------------------
CCAD_GemArc::CCAD_GemArc(const CCAD_GemArc& cArc)
{
	*this = cArc;
}

//-----------------------------------------------------
CCAD_GemArc & CCAD_GemArc::operator=(const CCAD_GemArc & cArc)
{
	CCAD_GemThickEntity::operator =(cArc);
	m_bDraw = cArc.m_bDraw;
	m_bType = cArc.m_bType;
	m_cCp = cArc.m_cCp;
	m_fR = cArc.m_fR;
	m_cAlfa = cArc.m_cAlfa;
	m_cDelta = cArc.m_cDelta;
	return *this;
}

void CCAD_GemArc::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_GemThickEntity::MemCopyTo(pGemEntity, pData1, pData2, pData3);
	*(CCAD_GemArc *)pGemEntity  = *this;
}

BOOL CCAD_GemArc::IsClosed()
{
	if (m_bType == ARC_PIE || m_bType == ARC_CHORD)
		return TRUE;
	return FALSE;
}

BOOL CCAD_GemArc::Read(CCAD_File& cFile)
{
	CCAD_GemCircle::Read(cFile);
	cFile.ReadByte(&m_bType);
	m_cAlfa.Read(cFile);
	m_cDelta.Read(cFile);
	return TRUE;
}

//-----------------------------------------------------
void CCAD_GemArc::FrameDraw(CCAD_DC *pDC, short n, const CCAD_Angle &cDelta)
{
	CCAD_Vector	cPt, cSp, cEp;
	CCAD_Angle	cAlfa = m_cAlfa;
	CountCirclePt(cSp, cAlfa.sin(), cAlfa.cos());
	cAlfa.RotateAngle(m_cDelta);
	CountCirclePt(cEp, cAlfa.sin(), cAlfa.cos());
	cAlfa = m_cAlfa;
	pDC->Lines_Begin(LINETYPE_STRIP);
	pDC->MoveTo(cSp);
	int i;
	for (i=0; i<n-1; i++){
		cAlfa.RotateAngle(cDelta);
		CountCirclePt(cPt, cAlfa.sin(), cAlfa.cos());
		pDC->LineTo(cPt);
	}
	pDC->LineTo(cEp);
	if (m_bType == ARC_PIE) {
		cPt = m_cCp;
		pDC->LineTo(cPt);
		pDC->LineTo(cSp);
	}
	else if(m_bType == ARC_CHORD)
		pDC->LineTo(cSp);
	pDC->Lines_End();
}

//-----------------------------------------------------
BOOL CCAD_GemArc::Draw(CCAD_DC *pDC, BOOL bRedraw)
{
	if (m_fR == 0)
		return false;

	short n = pDC->GetArcDivide(m_cDelta, m_fR);
	CCAD_Angle	cAlfa;
	CCAD_Angle	cDelta = m_cDelta;
	cDelta /= n;

	BOOL	blPaint = FALSE;
	if ((m_bDraw & DRAW_PAINT) && (m_bType == ARC_PIE || m_bType == ARC_CHORD)) {
		if (!pDC->m_pCamera->m_bPerspect){
			short	nNum = (short)((m_bType == ARC_PIE) ? (n+2) : (n+1));
			CCAD_Point	*pPt = new CCAD_Point[nNum];
			cAlfa = m_cAlfa;
			CCAD_Point	cEp;
			cAlfa.RotateAngle(m_cDelta);
			CountCirclePt( cEp, cAlfa.sin(), cAlfa.cos());
			cAlfa = m_cAlfa;
			int i;
			for (i=0; i<n; i++){
				CountCirclePt( *(pPt+i), cAlfa.sin(), cAlfa.cos());
				cAlfa.RotateAngle(cDelta);
			}
			*(pPt+i) = cEp;
			if (m_bType == ARC_PIE)
				*(pPt+n+1) = m_cCp;

			LOGPEN oldPen = pDC->SelectNullPen();
			if(m_nTriCount == 0)
				m_pTriPoints = pDC->GetTrianglesFromPolygon(pPt, &nNum, 1, m_nTriCount);
			pDC->PolyPolygon(pPt, &nNum, 1, m_pTriPoints, m_nTriCount);
			pDC->RestoreObject(oldPen);

			delete []pPt;
			blPaint = TRUE;
		}
	}

	if ((m_bDraw & DRAW_FRAME) || !blPaint)
		FrameDraw(pDC, n, cDelta);
	else {
		LOGPEN oldPen = pDC->SelectNullPen();
		FrameDraw(pDC, n, cDelta);
		pDC->RestoreObject(oldPen);
	}
	return TRUE;
}


void CCAD_GemArc::CountBodyRange(CCAD_Rect& cRange)
{
	if (m_fR <= 0 || IS_ZERO(m_cDelta.GetDegree()))
		return;
	CCAD_GemThickEntity::CountBodyRange(cRange);

	short	n = (short)(12 * (short)(m_cDelta.GetAbsDegree() / DOUBLE_NINETY + 1));
	CCAD_Angle	cAlfa = m_cAlfa;
	CCAD_Angle	cDelta = m_cDelta;
	cDelta /= n;

	CCAD_Vector	cPt;
	CountCirclePt(cPt, cAlfa.sin(), cAlfa.cos());
	cRange.ResetParameter(cPt);
	cAlfa.RotateAngle(m_cDelta);
	CountCirclePt(cPt, cAlfa.sin(), cAlfa.cos());
	cRange.ResetParameter(cPt);
	cAlfa = m_cAlfa;
	int	i;
	for (i=0; i<n-1; i++){
		cAlfa.RotateAngle(cDelta);
		CountCirclePt(cPt, cAlfa.sin(), cAlfa.cos());
		cRange.ResetParameter(cPt);
	}
	if (m_bType == ARC_PIE)
		cRange.ResetParameter(m_cCp);
}

void CCAD_GemArc::CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange)
{
	ASSERT(pCadDC != NULL);
	
	if (m_fR <= 0 || IS_ZERO(m_cDelta.GetDegree()))
		return;

	short n = pCadDC->GetArcDivide(m_cDelta, m_fR);
	CCAD_Angle	cDelta = m_cDelta;
	cDelta /= n;
	CCAD_Vector	cPt;
	CCAD_Angle	cAlfa = m_cAlfa;

	CountCirclePt(cPt, cAlfa.sin(), cAlfa.cos());
	pCadDC->CountVcsRange(cPt, cRange);
	cAlfa.RotateAngle(m_cDelta);
	CountCirclePt(cPt, cAlfa.sin(), cAlfa.cos());
	pCadDC->CountVcsRange(cPt, cRange);
	cAlfa = m_cAlfa;
	int	i;
	for (i=0; i<n-1; i++){
		cAlfa.RotateAngle(cDelta);
		CountCirclePt(cPt, cAlfa.sin(), cAlfa.cos());
		pCadDC->CountVcsRange(cPt, cRange);
	}
	if (m_bType == ARC_PIE){
		cPt = m_cCp;
		pCadDC->CountVcsRange(cPt, cRange);
	}
}

//----------------------------------------------------------
WORD CCAD_GemArc::GetCurvePoint(SCAD_POINT *spDivPnt)
{
	short n = 36;
	if (spDivPnt != NULL)
	{
		CCAD_Angle	cAlfa = m_cAlfa;
		CCAD_Angle	cDelta = m_cDelta;
		cDelta /= n;
		
		CCAD_Point	cPt;
		int	i;
		for (i = 0; i <= n; i++)
		{
			CountCirclePt(cPt, cAlfa.sin(), cAlfa.cos());
			m_cBcs.ConvToBase(cPt);
			spDivPnt[i] = cPt;
			
			cAlfa.RotateAngle(cDelta);
		}
		if (m_bType == ARC_PIE)
		{
			cPt = m_cCp;
			m_cBcs.ConvToBase(cPt);
			spDivPnt[n+1] = cPt;
		}
	}
	if (m_bType == ARC_PIE)
		n++;
	return (WORD)(n + 1);
}
