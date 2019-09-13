#include "stdafx.h"
#include "CAD_GemEllipse.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCAD_GemEllipse::CCAD_GemEllipse(const CCAD_GemEllipse & cEllp)
{
	*this = cEllp;
}

//-----------------------------------------------------
CCAD_GemEllipse::CCAD_GemEllipse(const CCAD_Coord3D & cBcs, const CCAD_Vector & cCp, const CAD_FLOAT & fRx, const CAD_FLOAT & fRy, const CCAD_Angle & cDirect, BYTE bDraw)
{
	m_bDraw = bDraw;
	m_cBcs = cBcs;
	m_cCp = cCp;
	m_fRx = fRx;
	m_fRy = fRy;
	m_cDirect = cDirect;
}

//-----------------------------------------------------
CCAD_GemEllipse & CCAD_GemEllipse::operator=(const CCAD_GemEllipse & cEllp)
{
	CCAD_GemThickEntity::operator =(cEllp);
	m_bDraw = cEllp.m_bDraw;
	m_cCp =  cEllp.m_cCp;
	m_fRx = cEllp.m_fRx;
	m_fRy = cEllp.m_fRy;
	m_cDirect = cEllp.m_cDirect;
	return *this;
}

//-----------------------------------------------------
CCAD_GemEllipse & CCAD_GemEllipse::operator=(const CCAD_GemCircle & cCircle)
{
	CCAD_GemThickEntity::operator =(cCircle);
	m_bDraw = cCircle.m_bDraw;
	m_cCp =  cCircle.m_cCp;
	m_fRx = cCircle.m_fR;
	m_fRy = cCircle.m_fR;
	m_cDirect = 0;
	return *this;
}

void CCAD_GemEllipse::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_GemThickEntity::MemCopyTo(pGemEntity, pData1, pData2, pData3);
	*(CCAD_GemEllipse *)pGemEntity = *this;
}

//---------------------------------------------
BOOL CCAD_GemEllipse::Read(CCAD_File & cFile)
{
	CCAD_GemThickEntity::Read(cFile);
	cFile.ReadByte(&m_bDraw);
	cFile.ReadCAD_Float(&m_fRx);
	cFile.ReadCAD_Float(&m_fRy);
	m_cDirect.Read(cFile);
	m_cCp.Read(cFile);
	return TRUE;
}

//-----------------------------------------------------
void CCAD_GemEllipse::FrameDraw(CCAD_DC *pDC, short n, CCAD_Point *pPt)
{
	pDC->Lines_Begin(LINETYPE_STRIP);
	pDC->MoveTo(*pPt);
	int	i;
	for (i=1; i<n; i++)
		pDC->LineTo(*(pPt+i));
	pDC->LineTo(*pPt);
	pDC->Lines_End();
}

//-----------------------------------------------------
BOOL CCAD_GemEllipse::Draw(CCAD_DC *pCadDC, BOOL blRedraw)
{
	if (m_fRx == 0 || m_fRy == 0)
		return FALSE;
	CCAD_Angle		cAlfa = (short)0, cDelta;
	short		n = pCadDC->GetEllipseDivide(m_fRx, m_fRy);
	cDelta = (short)LONGDOUBLE_NINETY;
	cDelta /= n;
	CCAD_Point	*pPt = new CCAD_Point[n];
	if (pPt == NULL)
		return TRUE;
	int	i;
	for (i=0; i<n; i++) {
		CountEllpPt(*(pPt+i), cAlfa, m_cDirect.sin(), m_cDirect.cos());
		cAlfa.RotateAngle(cDelta);
	}
	
	BOOL	blPaint = FALSE;
	if (m_bDraw & DRAW_PAINT) {
		if (!pCadDC->m_pCamera->m_bPerspect){
			CCAD_Point	*pPaint = new CCAD_Point[n];
			memcpy(pPaint, pPt, sizeof(CCAD_Point)*n);
			LOGPEN oldPen = pCadDC->SelectNullPen();
			int nTriCount = 0;
			pCadDC->PolyPolygon(pPaint, &n, 1, NULL, nTriCount);
			pCadDC->RestoreObject(oldPen);
			delete []pPaint;
			blPaint = TRUE;
		}
	}
	if ((m_bDraw & DRAW_FRAME) || !blPaint)
		FrameDraw(pCadDC, n, pPt);
	else {
		LOGPEN oldPen = pCadDC->SelectNullPen();
		FrameDraw(pCadDC, n, pPt);
		pCadDC->RestoreObject(oldPen);
	}
	delete []pPt;
	return TRUE;
}

//-----------------------------------------------------
void CCAD_GemEllipse::CountBodyRange(CCAD_Rect& cRange)
{
	if (m_fRx == 0 || m_fRy == 0)
		return;

	CCAD_GemThickEntity::CountBodyRange(cRange);

	CCAD_Angle	cAlfa = (short)0, cDelta;
	short		n = 72;
	cDelta = (short)LONGDOUBLE_NINETY;
	cDelta /= n;

	CCAD_Point	cPt;
	int	i;
	for (i=0; i<n; i++) {
		CountEllpPt(cPt, cAlfa, m_cDirect.sin(), m_cDirect.cos());
		cRange.ResetParameter(cPt);
		cAlfa.RotateAngle(cDelta);
	}
}

//-----------------------------------------------------
void CCAD_GemEllipse::CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange)
{
	if (m_fRx == 0 || m_fRy == 0)
		return;

	ASSERT(pCadDC != NULL);

	CCAD_Angle	cAlfa = (short)0, cDelta;
	short		n = 72;
	cDelta = (short)LONGDOUBLE_NINETY;
	cDelta /= n;

	CCAD_Point	cPt;
	int	i;
	for (i=0; i<n; i++) {
		CountEllpPt(cPt, cAlfa, m_cDirect.sin(), m_cDirect.cos());
		pCadDC->CountVcsRange(cPt, cRange);
		cAlfa.RotateAngle(cDelta);
	}
}

//----------------------------------------------------------
WORD CCAD_GemEllipse::GetCurvePoint(SCAD_POINT *spDivPnt)
{
	short n = 36;
	if (spDivPnt != NULL)
	{
		CCAD_Angle cAlfa = (short)0;
		CCAD_Angle	cDelta = (short)LONGDOUBLE_NINETY;
		cDelta /= n;
		
		CCAD_Point	cPt;
		int	i;
		for (i = 0; i < n; i++)
		{
			CountEllpPt(cPt, cAlfa, m_cDirect.sin(), m_cDirect.cos());
			m_cBcs.ConvToBase(cPt);
			spDivPnt[i] = cPt;
			//-------------------
			cAlfa.RotateAngle(cDelta);
		}
	}
	return n;
}


//-----------------------------------------------------
void CCAD_GemEllipse::CountEllpPt(CCAD_Vector & cPt, const CCAD_Angle & cAlfa, const CAD_FLOAT & fSin, const CAD_FLOAT & fCos) const
{
	CAD_FLOAT fx = cAlfa.cos() * m_fRx;
	CAD_FLOAT fy = cAlfa.sin() * m_fRy;
	cPt.m_fX = m_cCp.m_fX + fx*fCos - fy*fSin;
	cPt.m_fY = m_cCp.m_fY + fx*fSin + fy*fCos;
	cPt.m_fZ = m_cCp.m_fZ;
}

