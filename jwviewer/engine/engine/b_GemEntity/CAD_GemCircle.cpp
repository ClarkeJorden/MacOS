#include "stdafx.h"
#include "CAD_GemCircle.h"
#include "CAD_GemArc.h"

CCAD_GemCircle::CCAD_GemCircle()
{
	m_bDraw = DRAW_FRAME;
	m_fR = 0;
}

CCAD_GemCircle::CCAD_GemCircle(const CCAD_GemCircle & cCircle)
{
	*this = cCircle;
}

//-----------------------------------------------------
CCAD_GemCircle::CCAD_GemCircle(const CCAD_Coord3D & cBcs, const CCAD_Vector & cCp, const CAD_FLOAT & fR, BYTE bDraw)
{
	m_cBcs = cBcs;
	m_cCp = cCp;
	m_fR = fR;
	m_bDraw = bDraw;
}

CCAD_GemCircle::~CCAD_GemCircle()
{
}

//-----------------------------------------------------
CCAD_GemCircle & CCAD_GemCircle::operator=(const CCAD_GemCircle & cCircle)
{
	CCAD_GemThickEntity::operator =(cCircle);
	m_cCp = cCircle.m_cCp;
	m_fR = cCircle.m_fR;
	m_bDraw = cCircle.m_bDraw;
	return *this;
}

void CCAD_GemCircle::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_GemThickEntity::MemCopyTo(pGemEntity, pData1, pData2, pData3);
	*(CCAD_GemCircle *)pGemEntity = *this;
}

BOOL CCAD_GemCircle::Read(CCAD_File& cFile)
{
	CCAD_GemThickEntity::Read(cFile);
	m_cCp.Read(cFile);
	cFile.ReadByte(&m_bDraw);
	cFile.ReadCAD_Float(&m_fR);
	return TRUE;
}

//---------------------------------------------------
void CCAD_GemCircle::FrameDraw(CCAD_DC *pCadDC, short n, CCAD_Point *pPt)
{
	pCadDC->Lines_Begin(LINETYPE_STRIP);
	pCadDC->MoveTo(*pPt);
	short	i;
	for (i=0; i<n; i++)
		pCadDC->LineTo(*(pPt+i));
	pCadDC->LineTo(*pPt);
	pCadDC->Lines_End();
}

//---------------------------------------------------
BOOL CCAD_GemCircle::Draw(CCAD_DC *pCadDC, BOOL/* bRedraw*/)
{
	if (m_fR <= 0)
		return FALSE;
	short n = pCadDC->GetCircleDivide(m_fR);
	CCAD_Point	*pPt = new CCAD_Point[n];
	CCAD_Angle	cDelta = (short)LONGDOUBLE_NINETY;
	cDelta /= n;
	CCAD_Angle cAlfa = (short)0;
	int	i;
	for (i=0; i<n; i++){
		CountCirclePt( *(pPt+i), cAlfa.sin(), cAlfa.cos());
		cAlfa.RotateAngle(cDelta);
	}
	BOOL blPaint = FALSE;
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

void CCAD_GemCircle::CountBodyRange(CCAD_Rect& cRange)
{
	if (m_fR <= 0)
		return;

	CCAD_GemThickEntity::CountBodyRange(cRange);

	short n = 72;
	CCAD_Angle	cDelta = (short)5;
	CCAD_Angle	cAlfa = (short)0;
	CCAD_Vector	cPt;

	int	i;
	for (i=0; i<n; i++){
		CountCirclePt(cPt, cAlfa.sin(), cAlfa.cos());
		cRange.ResetParameter(cPt);
		cAlfa.RotateAngle(cDelta);
	}
}

void CCAD_GemCircle::CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange)
{
	ASSERT(pCadDC != NULL);

	if (m_fR <= 0)
		return;

	short n = pCadDC->GetCircleDivide(m_fR);
	CCAD_Angle	cDelta = (short)LONGDOUBLE_NINETY;
	cDelta /= n;
	CCAD_Angle	cAlfa = (short)0;
	CCAD_Vector	cPt;

	int	i;
	for (i=0; i<n; i++){
		CountCirclePt(cPt, cAlfa.sin(), cAlfa.cos());
		pCadDC->CountVcsRange(cPt, cRange);
		cAlfa.RotateAngle(cDelta);
	}
}


//----------------------------------------------------------
WORD CCAD_GemCircle::GetCurvePoint(SCAD_POINT *spDivPnt)
{
	short n = 36;
	
	if (spDivPnt != NULL)
	{
		CCAD_Point	cPt;
		CCAD_Angle	cDelta = (short)LONGDOUBLE_NINETY;
		cDelta /= n;
		CCAD_Angle cAlfa = (short)0;
		int	i;
		for (i = 0; i < n; i++)
		{
			CountCirclePt(cPt, cAlfa.sin(), cAlfa.cos());
			m_cBcs.ConvToBase(cPt);
			spDivPnt[i] = cPt;
			//-------------------
			cAlfa.RotateAngle(cDelta);
		}
	}
	return n;
}

void CCAD_GemCircle::CountCirclePt(CCAD_Vector & cPt, const CAD_FLOAT &fSin, const CAD_FLOAT &fCos) const
{
	cPt.m_fX = m_cCp.m_fX + m_fR * fCos;
	cPt.m_fY = m_cCp.m_fY + m_fR * fSin;
	cPt.m_fZ = m_cCp.m_fZ;
}

BYTE CCAD_GemCircle::CountTangentPt(const CCAD_Vector & cPt, CCAD_Vector & cGp1, CCAD_Vector & cGp2) const
{
	CCAD_Point	cTmpPt = cPt;
	m_cBcs.ConvFromBase(cTmpPt);
	CAD_FLOAT	fL = m_cCp.CountLength(cTmpPt);
	if (fL < m_fR)
		return ROOT_NO;
	CCAD_Angle	cAlfa;
	m_cCp.CountAngle2D(cTmpPt, cAlfa);
	CCAD_Angle	cBeta;
	cBeta.SetFromAsin(fL, m_fR);
	CCAD_Angle	cAngle = cAlfa;
	cAngle.RotateAngle(cBeta);
	cGp1.m_fX = m_cCp.m_fX;
	cGp1.m_fY = m_cCp.m_fY - m_fR;
	cGp1.Rotate(m_cCp, cAngle.sin(), cAngle.cos());
	CCAD_Vector	cTmp;
	cTmp.m_fX = m_cCp.m_fX;
	cTmp.m_fY = m_cCp.m_fY + m_fR;
	cTmp.Rotate(m_cCp, cAngle.sin(), cAngle.cos());
	fL = cGp1.CountLength(cTmpPt);
	if (cTmp.CountLength(cTmpPt) < fL)
		cGp1 = cTmp;

	cAlfa.InvertRotateAngle(cBeta);
	cGp2.m_fX = m_cCp.m_fX;
	cGp2.m_fY = m_cCp.m_fY - m_fR;
	cGp2.Rotate(m_cCp, cAlfa.sin(), cAlfa.cos());
	cTmp.m_fX = m_cCp.m_fX;
	cTmp.m_fY = m_cCp.m_fY + m_fR;
	cTmp.Rotate(m_cCp, cAlfa.sin(), cAlfa.cos());
	fL = cGp2.CountLength(cTmpPt);
	if (cTmp.CountLength(cTmpPt) < fL)
		cGp2 = cTmp;
	cGp1.m_fZ = m_cCp.m_fZ;
	cGp2.m_fZ = m_cCp.m_fZ;
	return ROOT_TWO;
}

//////////////////////////////////////////////////////////////////////////
//			Counting CIRCLE from center & radius's entity				//
//////////////////////////////////////////////////////////////////////////
BOOL CCAD_GemCircle::CountFromRadius(const CAD_FLOAT &fR)
{
	m_fR = fR;
	return (m_fR > 0);
}

BOOL CCAD_GemCircle::CountFrom2Entity(const CCAD_Vector &cPt1, const CCAD_Vector &cPt2)
{
	return CountPP(cPt2, cPt1);
}

BOOL CCAD_GemCircle::Count2P(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2)
{
	m_cCp.m_fX = (cPt1.m_fX + cPt2.m_fX) / 2;
	m_cCp.m_fY = (cPt1.m_fY + cPt2.m_fY) / 2;
	m_cCp.m_fZ = cPt1.m_fZ;
	m_fR = cPt1.CountLength(m_cCp);
	return (m_fR > 0);
}

BOOL CCAD_GemCircle::CountPP(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2)
{
	return Count2P(cPt1, cPt2);
}

BYTE CCAD_GemCircle::CountRootFromLinear(const CCAD_Equation2D & cEquation, const CCAD_Linear2D & cLinear)
{
	CAD_FLOAT	fV1, fV2;
	BYTE bRoot = cEquation.CountRoot(fV1, fV2);
	if (bRoot == ROOT_NO)
		return ROOT_NO;
	else if (bRoot == ROOT_ONE) {
		if (cEquation.m_bBase == X_BASE)
			cLinear.CountVerticalPt(m_cCp, fV1);
		else 
			cLinear.CountHorizontalPt(m_cCp, fV1);
		return ROOT_ONE;
	}
	if (cEquation.m_bBase == X_BASE)
		cLinear.CountVerticalPt(m_cCp, fV1);
	else 
		cLinear.CountHorizontalPt(m_cCp, fV1);
	return ROOT_TWO;
}

//----------------------------------------------------
BYTE CCAD_GemCircle::CountRootFromParabola(const CCAD_Equation2D & cEquation, const CCAD_Parabola2D & cPara)
{
	CAD_FLOAT	fV1, fV2;
	BYTE bRoot = cEquation.CountRoot(fV1, fV2);
	if (bRoot == ROOT_NO)
		return ROOT_NO;
	else if (bRoot == ROOT_ONE) {
		cPara.CountPt(m_cCp, fV1);
		return ROOT_ONE;
	}
	cPara.CountPt(m_cCp, fV1);
	return ROOT_TWO;
}

//----------------------------------------------------
BOOL CCAD_GemCircle::CountFrom3Entity(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2, const CCAD_Vector & cPt3)
{
	return CountPPP(cPt1, cPt2, cPt3);
}

BOOL CCAD_GemCircle::CountPPP(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2, const CCAD_Vector & cPt3)
{
	CCAD_Vector cTmpPt;
	CCAD_Angle	cAlfa;
	cPt1.CountAngle2D(cPt2, cAlfa);
	cTmpPt.m_fX = (cPt1.m_fX + cPt2.m_fX) / 2;
	cTmpPt.m_fY = (cPt1.m_fY + cPt2.m_fY) / 2;
	cTmpPt.m_fZ = cPt1.m_fZ;
	cAlfa.RotateAngle(NINETY);
	CCAD_Linear2D	cLinear1(cTmpPt, cAlfa);

	cPt1.CountAngle2D(cPt3, cAlfa);
	cTmpPt.m_fX = (cPt1.m_fX + cPt3.m_fX) / 2;
	cTmpPt.m_fY = (cPt1.m_fY + cPt3.m_fY) / 2;
	cTmpPt.m_fZ = cPt1.m_fZ;
	cAlfa.RotateAngle(NINETY);
	CCAD_Linear2D	cLinear2(cTmpPt, cAlfa);

	if (!cLinear1.CountCrossPt(m_cCp, cLinear2))
		return FALSE;
	m_fR = m_cCp.CountLength(cPt1);
	return (m_fR > 0);
}

BOOL CCAD_GemCircle::CountWithCenterFrom2Entity(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2, const CCAD_Vector & cCp)
{
	return CountWithCenterPP(cPt2, cPt1, cCp);
}

//----------------------------------------------------
BOOL CCAD_GemCircle::CountWithCenterPP(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2, const CCAD_Vector & cCp)
{
	CCAD_Angle cAlfa;
	cPt1.CountAngle2D(cPt2, cAlfa);
	CCAD_Linear2D	cLinear1(cCp, cAlfa);
	CCAD_Vector	cMp;
	cMp.m_fX = (cPt1.m_fX + cPt2.m_fX) / 2;
	cMp.m_fY = (cPt1.m_fY + cPt2.m_fY) / 2;
	cMp.m_fZ = cPt1.m_fZ;
	cAlfa.RotateAngle(NINETY);
	CCAD_Linear2D	cLinear2(cMp, cAlfa);
	if (!cLinear1.CountCrossPt(m_cCp, cLinear2))
		return FALSE;
	m_fR = m_cCp.CountLength(cPt1);
	return (m_fR > 0);
}

BOOL CCAD_GemCircle::CountWithRadiusFrom2Entity(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2,
											const CAD_FLOAT & fR)
{
	return CountWithRadiusPP(cPt2, cPt1, fR);
}

//----------------------------------------------------
BOOL CCAD_GemCircle::CountWithRadiusPP(const CCAD_Vector & cPt1, const CCAD_Vector & cPt2, const CAD_FLOAT & fR)
{
	CAD_FLOAT	d = cPt1.CountLength(cPt2);
	if (d > fR * 2)
		return FALSE;
	d = (CAD_FLOAT)sqrt(fR*fR - d*d/4);
	CCAD_Vector	cMp;
	cMp.m_fX = (cPt1.m_fX + cPt2.m_fX) / 2;
	cMp.m_fY = (cPt1.m_fY + cPt2.m_fY) / 2;
	cMp.m_fZ = cPt1.m_fZ;
	CCAD_Angle cAlfa;
	cPt1.CountAngle2D(cPt2, cAlfa);
	cAlfa.RotateAngle(NINETY);
	CAD_FLOAT	fSin = cAlfa.sin();
	CAD_FLOAT	fCos = cAlfa.cos();
	cMp.CountVector2D(m_cCp, d, fSin, fCos);
	m_fR = fR;
	return (m_fR > 0);
}


BYTE CCAD_GemCircle::CountCrossPtWithLine(const CCAD_GemLine & cLine, CCAD_Vector & cPt1, CCAD_Vector & cPt2) const
{
	CCAD_Point		cSp = cLine.m_cSp;
	CCAD_Point		cEp = cLine.m_cEp;

	cLine.m_cBcs.ConvToBase(cSp);
	cLine.m_cBcs.ConvToBase(cEp);
	m_cBcs.ConvFromBase(cSp);
	m_cBcs.ConvFromBase(cEp);

	if ((m_cCp.m_fX+m_fR) < cSp.m_fX && (m_cCp.m_fX+m_fR) < cEp.m_fX)
		return ROOT_NO;
	if ((m_cCp.m_fX-m_fR) > cSp.m_fX && (m_cCp.m_fX-m_fR) > cEp.m_fX)
		return ROOT_NO;
	if ((m_cCp.m_fY+m_fR) < cSp.m_fY && (m_cCp.m_fY+m_fR) < cEp.m_fY)
		return ROOT_NO;
	if ((m_cCp.m_fY-m_fR) > cSp.m_fY && (m_cCp.m_fY-m_fR) > cEp.m_fY)
		return ROOT_NO;

	CCAD_Linear2D	cLinear(cSp, cEp);
	CCAD_Curve2D	c2dCurve;
	CCAD_Equation2D	cEquation;
	
	c2dCurve.SetPara(m_cCp, m_fR);
	if (cEquation.SetPara(c2dCurve, cLinear)) {
		CAD_FLOAT	fV1, fV2;
		BYTE bRoot = cEquation.CountRoot(fV1, fV2);
		if (bRoot == ROOT_ONE) {
			if (cEquation.m_bBase == X_BASE)
				cLinear.CountVerticalPt(cPt1, fV1);
			else 
				cLinear.CountHorizontalPt(cPt1, fV1);
			m_cBcs.ConvToBase(cPt1);
			cLine.m_cBcs.ConvFromBase(cPt1);
			if (cLine.IsPtInterval(cPt1)){
				cLine.m_cBcs.ConvToBase(cPt1);
				return ROOT_ONE;
			}
		}
		if (bRoot == ROOT_TWO) {
			if (cEquation.m_bBase == X_BASE) {
				cLinear.CountVerticalPt(cPt1, fV1);
				cLinear.CountVerticalPt(cPt2, fV2);
			}
			else {
				cLinear.CountHorizontalPt(cPt1, fV1);
				cLinear.CountHorizontalPt(cPt2, fV2);
			}
			m_cBcs.ConvToBase(cPt1);
			m_cBcs.ConvToBase(cPt2);
			cLine.m_cBcs.ConvFromBase(cPt1);
			cLine.m_cBcs.ConvFromBase(cPt2);
			BOOL	bInFlag1 = cLine.IsPtInterval(cPt1);
			BOOL	bInFlag2 = cLine.IsPtInterval(cPt2);
			cLine.m_cBcs.ConvToBase(cPt1);
			cLine.m_cBcs.ConvToBase(cPt2);
			if (bInFlag1 && bInFlag2)
				return ROOT_TWO;
			else if (bInFlag1 && !bInFlag2)
				return ROOT_ONE;
			else if (!bInFlag1 && bInFlag2) {
				cPt1 = cPt2;
				return ROOT_ONE;
			}
		}
	}
	return ROOT_NO;
}

BYTE CCAD_GemCircle::CountCrossPtWithLineRay(const CCAD_GemLine & cLine, CCAD_Vector & cPt1, CCAD_Vector & cPt2) const
{
	CCAD_Point		cSp = cLine.m_cSp;
	CCAD_Point		cEp = cLine.m_cEp;

	cLine.m_cBcs.ConvToBase(cSp);
	cLine.m_cBcs.ConvToBase(cEp);
	m_cBcs.ConvFromBase(cSp);
	m_cBcs.ConvFromBase(cEp);
	
	CCAD_Linear2D	cLinear(cSp, cEp);
	CCAD_Curve2D	c2dCurve;
	CCAD_Equation2D	cEquation;
	
	c2dCurve.SetPara(m_cCp, m_fR);
	if (cEquation.SetPara(c2dCurve, cLinear)) {
		CAD_FLOAT	fV1, fV2;
		BYTE bRoot = cEquation.CountRoot(fV1, fV2);
		if (bRoot == ROOT_ONE) {
			if (cEquation.m_bBase == X_BASE)
				cLinear.CountVerticalPt(cPt1, fV1);
			else 
				cLinear.CountHorizontalPt(cPt1, fV1);
			m_cBcs.ConvToBase(cPt1);
			cPt2 = cPt1;
		}
		if (bRoot == ROOT_TWO) {
			if (cEquation.m_bBase == X_BASE) {
				cLinear.CountVerticalPt(cPt1, fV1);
				cLinear.CountVerticalPt(cPt2, fV2);
			}
			else {
				cLinear.CountHorizontalPt(cPt1, fV1);
				cLinear.CountHorizontalPt(cPt2, fV2);
			}
			m_cBcs.ConvToBase(cPt1);
			m_cBcs.ConvToBase(cPt2);
		}
		return bRoot;
	}
	return ROOT_NO;
}

BYTE CCAD_GemCircle::CountCrossPtWithCircle(const CCAD_GemCircle & cCircle, CCAD_Vector & cPt1, CCAD_Vector & cPt2) const
{
	CCAD_Point	cCp = cCircle.m_cCp;
	CAD_FLOAT	fR = cCircle.m_fR;
	cCircle.m_cBcs.ConvToBase(cCp);
	return CountCrossPtWithCircle(cCp, fR, cPt1, cPt2);
}

BYTE CCAD_GemCircle::CountCrossPtWithCircle(const CCAD_Vector & cCp, const CAD_FLOAT & fR, CCAD_Vector & cPt1, CCAD_Vector & cPt2) const
{
	CCAD_Point		cTmp = cCp;

	m_cBcs.ConvFromBase(cTmp);
	if ((m_cCp.m_fX+m_fR) < (cTmp.m_fX-fR))
		return ROOT_NO;
	if ((m_cCp.m_fX-m_fR) > (cTmp.m_fX+fR))
		return ROOT_NO;
	if ((m_cCp.m_fY+m_fR) < (cTmp.m_fY-fR))
		return ROOT_NO;
	if ((m_cCp.m_fY-m_fR) > (cTmp.m_fY+fR))
		return ROOT_NO;

	CCAD_Linear2D	cLinear;
	CCAD_Curve2D	c2dCurve;
	CCAD_Equation2D	cEquation;

	cLinear.SetParaTwoCircle(m_cCp, m_fR, cTmp, fR);
	c2dCurve.SetPara(m_cCp, m_fR);
	if (cEquation.SetPara(c2dCurve, cLinear)) {
		CAD_FLOAT	fV1, fV2;
		BYTE bRoot = cEquation.CountRoot(fV1, fV2);
		if (bRoot == ROOT_ONE) {
			if (cEquation.m_bBase == X_BASE)
				cLinear.CountVerticalPt(cPt1, fV1);
			else 
				cLinear.CountHorizontalPt(cPt1, fV1);
			m_cBcs.ConvToBase(cPt1);
			cPt2 = cPt1;
		}
		if (bRoot == ROOT_TWO) {
			if (cEquation.m_bBase == X_BASE) {
				cLinear.CountVerticalPt(cPt1, fV1);
				cLinear.CountVerticalPt(cPt2, fV2);
			}
			else {
				cLinear.CountHorizontalPt(cPt1, fV1);
				cLinear.CountHorizontalPt(cPt2, fV2);
			}
			m_cBcs.ConvToBase(cPt1);
			m_cBcs.ConvToBase(cPt2);
		}
		return bRoot;
	}
	return ROOT_NO;
}
