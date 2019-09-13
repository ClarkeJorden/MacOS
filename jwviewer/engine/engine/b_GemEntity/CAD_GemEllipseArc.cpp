#include "stdafx.h"
#include "CAD_GemEllipseArc.h"

////////////////////////////////////////////////////////////////////
// CCAD_GemEllipseArc
//
CCAD_GemEllipseArc::CCAD_GemEllipseArc(const CCAD_GemEllipseArc & cEllpArc)
{
	*this = cEllpArc;
}

//-----------------------------------------------------
CCAD_GemEllipseArc::CCAD_GemEllipseArc(const CCAD_GemEllipse & cEllp)
{
	m_bDraw = cEllp.m_bDraw;
	m_cBcs = cEllp.m_cBcs;
	m_cCp = cEllp.m_cCp;
	m_fRx = cEllp.m_fRx;
	m_fRy = cEllp.m_fRy;
	m_cDirect = cEllp.m_cDirect;
	m_cAlfa = 0;
	m_cDelta = LONGDOUBLE_NINETY;
	if (cEllp.m_bDraw & DRAW_PAINT)
		m_bType = ARC_PIE;
	else
		m_bType = ARC_NORMAL;
}

//-----------------------------------------------------
CCAD_GemEllipseArc::CCAD_GemEllipseArc(const CCAD_GemEllipse & cEllp, const CCAD_Angle & cAlfa, const CCAD_Angle & cDelta)
{
	m_bDraw = cEllp.m_bDraw;
	m_cBcs = cEllp.m_cBcs;
	m_cCp = cEllp.m_cCp;
	m_fRx = cEllp.m_fRx;
	m_fRy = cEllp.m_fRy;
	m_cDirect = cEllp.m_cDirect;
	m_cAlfa = cAlfa;
	m_cDelta = cDelta;
	if (cEllp.m_bDraw & DRAW_PAINT)
		m_bType = ARC_PIE;
	else
		m_bType = ARC_NORMAL;
}

//-----------------------------------------------------
CCAD_GemEllipseArc & CCAD_GemEllipseArc::operator=(const CCAD_GemEllipseArc & cEllpArc)
{
	CCAD_GemThickEntity::operator =(cEllpArc);
	m_bDraw = cEllpArc.m_bDraw;
	m_bType = cEllpArc.m_bType;
	m_cCp = cEllpArc.m_cCp;
	m_fRx = cEllpArc.m_fRx;
	m_fRy = cEllpArc.m_fRy;
	m_cDirect = cEllpArc.m_cDirect;
	m_cAlfa = cEllpArc.m_cAlfa;
	m_cDelta = cEllpArc.m_cDelta;
	return *this;
}

//-----------------------------------------------------
CCAD_GemEllipseArc & CCAD_GemEllipseArc::operator=(const CCAD_GemArc & cArc)
{
	m_bDraw = cArc.m_bDraw;
	m_bType = cArc.m_bType;
	m_cBcs = cArc.m_cBcs;
	m_cCp = cArc.m_cCp;
	m_fRx = cArc.m_fR;
	m_fRy = cArc.m_fR;
	m_cDirect = 0;
	m_cAlfa = cArc.m_cAlfa;
	m_cDelta = cArc.m_cDelta;
	return *this;
}

void CCAD_GemEllipseArc::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_GemThickEntity::MemCopyTo(pGemEntity, pData1, pData2, pData3);
	*(CCAD_GemEllipseArc *)pGemEntity = *this;
}

//---------------------------------------------
BOOL CCAD_GemEllipseArc::Read(CCAD_File & cFile)
{
	CCAD_GemEllipse::Read(cFile);
	cFile.ReadByte(&m_bType);
	m_cAlfa.Read(cFile);
	m_cDelta.Read(cFile);
	return TRUE;
}

//---------------------------------------------
BOOL CCAD_GemEllipseArc::IsClosed()
{
	if (m_bType == ARC_PIE || m_bType == ARC_CHORD)
		return TRUE;
	return FALSE;
}

void CCAD_GemEllipseArc::FrameDraw(CCAD_DC *pDC, short n, const CCAD_Angle &cDelta)
{
	CCAD_Point	cPt, cSp, cEp;
	CCAD_Angle cAlfa = m_cAlfa;
	CountEllpPt(cPt, cAlfa, m_cDirect.sin(), m_cDirect.cos());
	cSp = cPt;
	cAlfa.RotateAngle(m_cDelta);
	CountEllpPt(cEp, cAlfa, m_cDirect.sin(), m_cDirect.cos());
	pDC->Lines_Begin(LINETYPE_STRIP);
	pDC->MoveTo(cPt);
	cAlfa = m_cAlfa;
	int	i;
	for (i=0; i<n-1; i++) {
		cAlfa.RotateAngle(cDelta);
		CountEllpPt(cPt, cAlfa, m_cDirect.sin(), m_cDirect.cos());
		pDC->LineTo(cPt);
	}
	pDC->LineTo(cEp);
	if (m_bType == ARC_PIE || m_bType == ARC_CHORD) {
		if (m_bType == ARC_PIE)
			pDC->LineTo(m_cCp);
		pDC->LineTo(cSp);
	}
	pDC->Lines_End();
}

//-----------------------------------------------------
BOOL CCAD_GemEllipseArc::Draw(CCAD_DC *pDC, BOOL /*bRedraw*/)
{
	if (m_fRx == 0 || m_fRy == 0 || m_cDelta.GetAbsDegree() < DIV_ANGLE_VALUE)
		return FALSE;
	CCAD_Angle		cAlfa = m_cAlfa;
	CCAD_Angle		cDelta = m_cDelta;
	short		n = pDC->GetEllipseArcDivide(m_cDelta, m_fRx, m_fRy);
	cDelta /= n;

	BOOL	blPaint = FALSE;
	if ((m_bDraw & DRAW_PAINT) && (m_bType == ARC_PIE || m_bType == ARC_CHORD)) {
		if (!pDC->m_pCamera->m_bPerspect){
			short	nNum = (int)((m_bType == ARC_PIE) ? (n+2) : (n+1));
			CCAD_Point	*pPt = new CCAD_Point[nNum];
			CCAD_Point	cEp;
			cAlfa.RotateAngle(m_cDelta);
			CountEllpPt(cEp, cAlfa, m_cDirect.sin(), m_cDirect.cos());
			cAlfa = m_cAlfa;
			int	i;
			for (i=0; i<n; i++){
				CountEllpPt(*(pPt+i), cAlfa, m_cDirect.sin(), m_cDirect.cos());
				cAlfa.RotateAngle(cDelta);
			}
			*(pPt+i) = cEp;
			if (m_bType == ARC_PIE)
				*(pPt+n+1) = m_cCp;
			LOGPEN oldPen = pDC->SelectNullPen();
			int nTriCount = 0;
			pDC->PolyPolygon(pPt, &nNum, 1, NULL, nTriCount);
			pDC->RestoreObject(oldPen);
			delete pPt;
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

//-----------------------------------------------------
void CCAD_GemEllipseArc::CountBodyRange(CCAD_Rect& cRange)
{
	if (m_fRx == 0 || m_fRy == 0 || m_cDelta.GetAbsDegree() < DIV_ANGLE_VALUE)
		return;

	CCAD_GemThickEntity::CountBodyRange(cRange);

	CCAD_Angle	cAlfa = m_cAlfa;
	CCAD_Angle	cDelta = m_cDelta;
	short		n = 36;
	cDelta /= n;

	CCAD_Point	cPt;
	int	i;
	for (i=0; i<=n; i++){
		CountEllpPt(cPt, cAlfa, m_cDirect.sin(), m_cDirect.cos());
		cRange.ResetParameter(cPt);
		cAlfa.RotateAngle(cDelta);
	}
	if (m_bType == ARC_PIE)
		cRange.ResetParameter(m_cCp);
}

//-----------------------------------------------------
void CCAD_GemEllipseArc::CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange)
{
	if (m_fRx == 0 || m_fRy == 0 || m_cDelta.GetAbsDegree() < DIV_ANGLE_VALUE)
		return;

	ASSERT(pCadDC != NULL);

	CCAD_Angle	cAlfa = m_cAlfa;
	CCAD_Angle	cDelta = m_cDelta;
	short		n = 36;
	cDelta /= n;

	CCAD_Point	cPt;
	int	i;
	for (i=0; i<=n; i++){
		CountEllpPt(cPt, cAlfa, m_cDirect.sin(), m_cDirect.cos());
		pCadDC->CountVcsRange(cPt, cRange);
		cAlfa.RotateAngle(cDelta);
	}
	if (m_bType == ARC_PIE)
		pCadDC->CountVcsRange(m_cCp, cRange);
}

//----------------------------------------------------------
WORD CCAD_GemEllipseArc::GetCurvePoint(SCAD_POINT *spDivPnt)
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
			CountEllpPt(cPt, cAlfa, m_cDirect.sin(), m_cDirect.cos());
			m_cBcs.ConvToBase(cPt);
			spDivPnt[i] = cPt;
			//-------------------
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
