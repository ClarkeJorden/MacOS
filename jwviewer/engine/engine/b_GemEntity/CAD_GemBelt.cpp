#include "stdafx.h"
#include "CAD_Global.h"
#include "CAD_GemBelt.h"
#include "CAD_GemLine.h"
#include "CAD_ViewMain.h"

#define		IGNO_WIDTH	1

/////////////////////////////////////////////////////////////////////////////
// CCAD_GemBvertex
//
CCAD_GemBvertex::CCAD_GemBvertex()
{
	m_cBulge = 0;
	m_fStartW1 = 0;
	m_fStartW2 = 0;
	m_fEndW1 = 0;
	m_fEndW2 = 0;
}

//----------------------------------------------
CCAD_GemBvertex & CCAD_GemBvertex::operator=(const CCAD_GemBvertex & cVt)
{
	m_cPt = cVt.m_cPt;
	m_cBulge = cVt.m_cBulge;
	m_fStartW1 = cVt.m_fStartW1;
	m_fStartW2 = cVt.m_fStartW2;
	m_fEndW1 = cVt.m_fEndW1;
	m_fEndW2 = cVt.m_fEndW2;
	return *this;
}


//----------------------------------------------
void CCAD_GemBvertex::Read(CCAD_File & cFile)
{
	((CCAD_GemPvertex *)this)->Read(cFile);
	cFile.ReadCAD_Float(&m_fStartW1);
	cFile.ReadCAD_Float(&m_fStartW2);
	cFile.ReadCAD_Float(&m_fEndW1);
	cFile.ReadCAD_Float(&m_fEndW2);
	
	if (m_fStartW1 < MIN_DOUBLE)
		m_fStartW1 = MIN_DOUBLE;
	if (m_fStartW1 > MAX_DOUBLE)
		m_fStartW1 = MAX_DOUBLE;
	if (m_fStartW2 < MIN_DOUBLE)
		m_fStartW2 = MIN_DOUBLE;
	if (m_fStartW2 > MAX_DOUBLE)
		m_fStartW2 = MAX_DOUBLE;
	if (m_fEndW1 < MIN_DOUBLE)
		m_fEndW1 = MIN_DOUBLE;
	if (m_fEndW1 > MAX_DOUBLE)
		m_fEndW1 = MAX_DOUBLE;
	if (m_fEndW2 < MIN_DOUBLE)
		m_fEndW2 = MIN_DOUBLE;
	if (m_fEndW2 > MAX_DOUBLE)
		m_fEndW2 = MAX_DOUBLE;
}

/////////////////////////////////////////////////////////////////////////////
// CCAD_GemBvtxPara
//
CCAD_GemBvtxPara::CCAD_GemBvtxPara()
{
}

CCAD_GemBvtxPara::~CCAD_GemBvtxPara()
{
}
//----------------------------------------------
CCAD_GemBvtxPara & CCAD_GemBvtxPara::operator=(const CCAD_GemBvtxPara & cVt)
{
	m_cPt = cVt.m_cPt;
	m_cBulge = cVt.m_cBulge;
	m_fStartW1 = cVt.m_fStartW1;
	m_fStartW2 = cVt.m_fStartW2;
	m_fEndW1 = cVt.m_fEndW1;
	m_fEndW2 = cVt.m_fEndW2;
	int	i;
	for (i=0; i<4; i++)
		m_cRp[i] = cVt.m_cRp[i];
	m_cArc = cVt.m_cArc;
	return *this;
}

//----------------------------------------------
CCAD_GemBvtxPara & CCAD_GemBvtxPara::operator=(const CCAD_GemBvertex & cVtx)
{
	m_cPt = cVtx.m_cPt;
	m_cBulge = cVtx.m_cBulge;
	m_fStartW1 = cVtx.m_fStartW1;
	m_fStartW2 = cVtx.m_fStartW2;
	m_fEndW1 = cVtx.m_fEndW1;
	m_fEndW2 = cVtx.m_fEndW2;
	return *this;
}

//----------------------------------------------
void CCAD_GemBvtxPara::CountPara(const CCAD_Coord3D &cBcs, CCAD_GemBvtxPara * pBt, const CCAD_Vector & cNp)
{
	m_cArc.m_cBcs = cBcs;
	if (!GetArc(m_cArc, cNp)) {
		CCAD_Angle d;
		m_cPt.CountAngle2D(cNp, d);
		CAD_FLOAT fSin = d.sin();
		CAD_FLOAT fCos = d.cos();


		m_cRp[2] = cNp;
		m_cRp[3] = cNp;
		if ( m_fEndW2 != 0 || m_fEndW1 != 0 ) {
			cNp.CountVector2D(m_cRp[3], m_fEndW1, -fCos, fSin);
			cNp.CountVector2D(m_cRp[2], m_fEndW2, fCos, -fSin);
		}

		m_cRp[0] = m_cPt;
		m_cRp[1] = m_cPt;
		if ( m_fStartW1 != 0 || m_fStartW2 != 0 ) {
			m_cPt.CountVector2D(m_cRp[0], m_fStartW1, -fCos, fSin);
			m_cPt.CountVector2D(m_cRp[1], m_fStartW2, fCos, -fSin);
			if (pBt != NULL && pBt->m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy()) {
				CCAD_Angle	cRot;
				m_cPt.CountAngle2D(pBt->m_cPt, cRot);
				d.RotateAngle(cRot);
				d /= 2;
				if (d.GetAbsDegree() >= CCAD_Global::GetAngleAccy() && d.GetDegree() != DOUBLE_NINETY) {
					CCAD_Linear2D	cLinear0(m_cPt, d);
					CCAD_Linear2D	cLinear(pBt->m_cRp[0], pBt->m_cRp[3]);
					cLinear.CountCrossPt(pBt->m_cRp[3], cLinear0);
					cLinear.SetPara(pBt->m_cRp[1], pBt->m_cRp[2]);
					cLinear.CountCrossPt(pBt->m_cRp[2], cLinear0);

					cLinear.SetPara(m_cRp[0], m_cRp[3]);
					cLinear.CountCrossPt(m_cRp[0], cLinear0);
					cLinear.SetPara(m_cRp[1], m_cRp[2]);
					cLinear.CountCrossPt(m_cRp[1], cLinear0);
				}
			}
		}
	}
}

//---------------------------------------------
void CCAD_GemBvtxPara::CountVcsRange(CCAD_DC *pDC, CCAD_Rect &cRc)
{
	int		i, j;

	if (m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy()) {
		for (i=0; i<4; i++)
			pDC->CountVcsRange(m_cRp[i], cRc);
	}
	else {
		short n = 72; //(short)(m_cArc.m_cDelta.GetAbsDegree() / DOUBLE_NINETY * 18);
//		n = (short)(abs(n) + 1);

		CCAD_Angle	cAlfa = m_cArc.m_cAlfa;
		CCAD_Angle	cDelta = m_cArc.m_cDelta;
		cDelta /= n;
		CAD_FLOAT	fStartW1 = m_fStartW1;
		CAD_FLOAT	fStartW2 = m_fStartW2;
		CAD_FLOAT	fW1 = (m_fEndW1 - m_fStartW1) / n;
		CAD_FLOAT	fW2 = (m_fEndW2 - m_fStartW2) / n;
		CAD_FLOAT	fSin, fCos;

		for (i=0; i<=n; i++){
			fSin = cAlfa.sin();
			fCos = cAlfa.cos();
			if ( m_cArc.m_cDelta.GetDegree() > 0 ) {
				m_cArc.m_cCp.CountVector2D(m_cRp[0], (m_cArc.m_fR+fStartW1), fSin, fCos);
				m_cArc.m_cCp.CountVector2D(m_cRp[1], (m_cArc.m_fR-fStartW2), fSin, fCos);
			}
			else {
				m_cArc.m_cCp.CountVector2D(m_cRp[0], (m_cArc.m_fR+fStartW2), fSin, fCos);
				m_cArc.m_cCp.CountVector2D(m_cRp[1], (m_cArc.m_fR-fStartW1), fSin, fCos);
			}
			for (j=0; j<2; j++)
				pDC->CountVcsRange(m_cRp[j], cRc);
			cAlfa.RotateAngle(cDelta);
			fStartW1 += fW1;
			fStartW2 += fW2;
		}
	}
}

//---------------------------------------------
void CCAD_GemBvtxPara::CountBodyRange(CCAD_Rect &cRc)
{
	int		i, j;

	if (m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy()) {
		for (i=0; i<4; i++)
			cRc.ResetParameter(m_cRp[i]);
	}
	else {
		short n = 36;

		CCAD_Angle	cAlfa = m_cArc.m_cAlfa;
		CCAD_Angle	cDelta = m_cArc.m_cDelta;
		cDelta /= n;
		CAD_FLOAT	fStartW1 = m_fStartW1;
		CAD_FLOAT	fStartW2 = m_fStartW2;
		CAD_FLOAT	fW1 = (m_fEndW1 - m_fStartW1) / n;
		CAD_FLOAT	fW2 = (m_fEndW2 - m_fStartW2) / n;
		CAD_FLOAT	fSin, fCos;

		for (i=0; i<=n; i++){
			fSin = cAlfa.sin();
			fCos = cAlfa.cos();
			if ( m_cArc.m_cDelta.GetDegree() > 0 ) {
				m_cArc.m_cCp.CountVector2D(m_cRp[0], (m_cArc.m_fR+fStartW1), fSin, fCos);
				m_cArc.m_cCp.CountVector2D(m_cRp[1], (m_cArc.m_fR-fStartW2), fSin, fCos);
			}
			else {
				m_cArc.m_cCp.CountVector2D(m_cRp[0], (m_cArc.m_fR+fStartW2), fSin, fCos);
				m_cArc.m_cCp.CountVector2D(m_cRp[1], (m_cArc.m_fR-fStartW1), fSin, fCos);
			}
			for (j=0; j<2; j++)
				cRc.ResetParameter(m_cRp[j]);
			cAlfa.RotateAngle(cDelta);
			fStartW1 += fW1;
			fStartW2 += fW2;
		}
	}
}

//---------------------------------------------
void CCAD_GemBvtxPara::StyleDraw(CCAD_DC *pDC, const CCAD_GemBelt & cBelt, const CCAD_Vector & cPt, CAD_FLOAT & fPred)
{
	CAD_FLOAT	fR1 = 0, fR2 = 0;
	if (m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy()) {
		CAD_FLOAT	fLength = m_cPt.CountLength(cPt);
		CCAD_Angle	cAngle;
		m_cPt.CountAngle2D(cPt, cAngle);

		if ( fPred > 0 ) {
			if ( fPred > cBelt.m_fEmpty ) {
				fR2 = fPred - cBelt.m_fEmpty;
				DrawStyleLine(pDC, cBelt.m_cBcs, fLength, cAngle, fR1, fR2);
			}
		}
		fR1 = fPred;
		fPred = fLength - fPred;
		while( fPred > 0 ) {
			fR2 = fR1 + cBelt.m_fLength;
			DrawStyleLine(pDC, cBelt.m_cBcs, fLength, cAngle, fR1, fR2);
			fR1 = fR2 + cBelt.m_fEmpty;
			fPred -= (cBelt.m_fEmpty + cBelt.m_fLength);
		}
		fPred = -fPred;
	}
	else {
		if ( fPred > 0 ) {
			if ( fPred > cBelt.m_fEmpty ) {
				fR2 = (fPred - cBelt.m_fEmpty) / m_cArc.m_fR;
				DrawStyleArc(pDC, fR1, fR2);
			}
		}
		fR1 = fPred / m_cArc.m_fR;
		fPred = (CAD_FLOAT)m_cArc.m_cDelta.GetAbsRadian() * m_cArc.m_fR - fPred;
		while (fPred > 0){
			fR2 = fR1 + cBelt.m_fLength / m_cArc.m_fR;
			DrawStyleArc(pDC, fR1, fR2 );
			fR1 = fR2 + cBelt.m_fEmpty / m_cArc.m_fR;
			fPred -= (cBelt.m_fEmpty + cBelt.m_fLength);
		}
		fPred = -fPred;
	}
}

//---------------------------------------------
void CCAD_GemBvtxPara::DrawStyleLine(CCAD_DC *pDC, const CCAD_Coord3D &cBcs, CAD_FLOAT fLength, const CCAD_Angle &cAlfa, CAD_FLOAT fR1, CAD_FLOAT fR2)
{
/*	CCAD_Angle	cAngle = cAlfa;
	CAD_FLOAT	fW1 = (m_fEndW1 - m_fStartW1) / fLength;
	CAD_FLOAT	fW2 = (m_fEndW2 - m_fStartW2) / fLength;

	CCAD_GemLine	cLine;
	cLine.m_cBcs = cBcs;
	if (fR1 <= 0)
		cLine.m_cSp = m_cPt;
	else
		m_cPt.CountVector2D(cLine.m_cSp, fR1, cAngle.sin(), cAngle.cos());
	if (fR2 > fLength)
		fR2 = fLength;
	m_cPt.CountVector2D(cLine.m_cEp, fR2, cAngle.sin(), cAngle.cos());

	cAngle.RotateAngle((short)NINETY);
	long	nStartW = pDC->ConvScalarWorldToScrn(m_fStartW1 + fW1*fR1 + m_fStartW2 + fW2*fR1);
	long	nEndW = pDC->ConvScalarWorldToScrn(m_fStartW1 + fW1*fR2 + m_fStartW2 + fW2*fR2);
	if ( nStartW <= IGNO_WIDTH && nEndW <= IGNO_WIDTH ) {
		pDC->MoveTo(cLine.m_cSp);
		pDC->LineTo(cLine.m_cEp);
	}
	else {
		#define	CROSS_01	0x01
		#define	CROSS_12	0x02
		#define	CROSS_23	0x04
		#define	CROSS_30	0x08

		BYTE			bCross1 = 0;
		BYTE			bCross2 = 0;
		CCAD_Point		cCp = cLine.m_cSp;
		CCAD_GemRay		cRay1(cBcs, cCp, cAngle);
		cCp = cLine.m_cEp;
		CCAD_GemRay		cRay2(cBcs, cCp, cAngle);

		if (fR1 > 0){
			cCp = cRay1.m_cCp;
			cRay1.m_cBcs.ConvToBase(cCp);
			cLine.m_cBcs.ConvFromBase(cCp);
			cLine.m_cSp = m_cRp[0]; cLine.m_cEp = m_cRp[1];
			if (cLine.CrossTwoLine(cAngle, cCp))
				bCross1 |= CROSS_01;
			cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
			if (cLine.CrossTwoLine(cAngle, cCp))
				bCross1 |= CROSS_12;
			cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
			if (cLine.CrossTwoLine(cAngle, cCp))
				bCross1 |= CROSS_23;
			cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
			if (cLine.CrossTwoLine(cAngle, cCp))
				bCross1 |= CROSS_30;
		}
		if (fR2 < fLength){
			cCp = cRay2.m_cCp;
			cRay2.m_cBcs.ConvToBase(cCp);
			cLine.m_cBcs.ConvFromBase(cCp);
			cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
			if (cLine.CrossTwoLine(cAngle, cCp))
				bCross2 |= CROSS_01;
			cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
			if (cLine.CrossTwoLine(cAngle, cCp))
				bCross2 |= CROSS_12;
			cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
			if (cLine.CrossTwoLine(cAngle, cCp))
				bCross2 |= CROSS_23;
			cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
			if (cLine.CrossTwoLine( cAngle, cCp))
				bCross2 |= CROSS_30;
		}

		short		nNum = 0;
		CCAD_Point	cPt[6];

		if (fR1 <= 0 && fR2 >= fLength){
			nNum = 4;
			memcpy(&cPt, &m_cRp, sizeof(CCAD_Point)*nNum);
		}
		else if (fR1 <= 0){
			if ((bCross2 & CROSS_01) && (bCross2 & CROSS_12)){
				cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
				cRay2.CountCrossPtWithLine(cLine, cPt[0]);
				cBcs.ConvFromBase(cPt[0]);
				cPt[1] = m_cRp[1];
				cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
				cRay2.CountCrossPtWithLine(cLine, cPt[2]);
				cBcs.ConvFromBase(cPt[2]);
				nNum = 3;
			}
			else if ((bCross2 & CROSS_01) && (bCross2 & CROSS_23)){
				CCAD_Angle cDir;
				m_cRp[0].CountAngle2D(m_cRp[1], cDir);
				cDir.RotateAngle(cAngle);
				if (cDir.GetDegree() < 0){
					cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
					cRay2.CountCrossPtWithLine(cLine, cPt[0]);
					cBcs.ConvFromBase(cPt[0]);
					cPt[1] = m_cRp[0];
					cPt[2] = m_cRp[3];
					cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
				}
				else {
					cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
					cRay2.CountCrossPtWithLine(cLine, cPt[0]);
					cBcs.ConvFromBase(cPt[0]);
					cPt[1] = m_cRp[1];
					cPt[2] = m_cRp[2];
					cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
				}
				nNum = 4;
			}
			else if ((bCross2 & CROSS_01) && (bCross2 & CROSS_30)){
				cPt[0] = m_cRp[0];
				cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
				cRay2.CountCrossPtWithLine(cLine, cPt[1]);
				cBcs.ConvFromBase(cPt[1]);
				cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
				cRay2.CountCrossPtWithLine(cLine, cPt[2]);
				cBcs.ConvFromBase(cPt[2]);
				nNum = 3;
			}
			else if ((bCross2 & CROSS_12) && (bCross2 & CROSS_23)){
				cPt[0] = m_cRp[0];
				cPt[1] = m_cRp[1];
				cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
				cRay2.CountCrossPtWithLine(cLine, cPt[2]);
				cBcs.ConvFromBase(cPt[2]);
				cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
				cRay2.CountCrossPtWithLine(cLine, cPt[3]);
				cBcs.ConvFromBase(cPt[3]);
				cPt[4] = m_cRp[3];
				nNum = 5;
			}
			else if ((bCross2 & CROSS_12) && (bCross2 & CROSS_30)){
				cPt[0] = m_cRp[0];
				cPt[1] = m_cRp[1];
				cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
				cRay2.CountCrossPtWithLine(cLine, cPt[2]);
				cBcs.ConvFromBase(cPt[2]);
				cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
				cRay2.CountCrossPtWithLine(cLine, cPt[3]);
				cBcs.ConvFromBase(cPt[3]);
				nNum = 4;
			}
			else if ((bCross2 & CROSS_23) && (bCross2 & CROSS_30)){
				cPt[0] = m_cRp[0];
				cPt[1] = m_cRp[1];
				cPt[2] = m_cRp[2];
				cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
				cRay2.CountCrossPtWithLine(cLine, cPt[3]);
				cBcs.ConvFromBase(cPt[3]);
				cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
				cRay2.CountCrossPtWithLine(cLine, cPt[4]);
				cBcs.ConvFromBase(cPt[4]);
				nNum = 5;
			}
		}
		else if (fR2 == fLength){
			if ((bCross1 & CROSS_01) && (bCross1 & CROSS_12)){
				cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
				cRay1.CountCrossPtWithLine(cLine, cPt[0]);
				cBcs.ConvFromBase(cPt[0]);
				cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
				cRay1.CountCrossPtWithLine(cLine, cPt[1]);
				cBcs.ConvFromBase(cPt[1]);
				cPt[2] = m_cRp[2];
				cPt[3] = m_cRp[3];
				cPt[4] = m_cRp[0];
				nNum = 5;
			}
			else if ((bCross1 & CROSS_01) && (bCross1 & CROSS_23)){
				CCAD_Angle cDir;
				m_cRp[0].CountAngle2D(m_cRp[1], cDir);
				cDir.RotateAngle(cAngle);
				if (cDir.GetDegree() < 0){
					cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
					cRay1.CountCrossPtWithLine(cLine, cPt[0]);
					cBcs.ConvFromBase(cPt[0]);
					cPt[1] = m_cRp[1];
					cPt[2] = m_cRp[2];
					cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
					cRay1.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
				}
				else {
					cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
					cRay1.CountCrossPtWithLine(cLine, cPt[0]);
					cBcs.ConvFromBase(cPt[0]);
					cPt[1] = m_cRp[0];
					cPt[2] = m_cRp[3];
					cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
					cRay1.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
				}
				nNum = 4;
			}
			else if ((bCross1 & CROSS_01) && (bCross1 & CROSS_30)){
				cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
				cRay1.CountCrossPtWithLine(cLine, cPt[0]);
				cBcs.ConvFromBase(cPt[0]);
				cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
				cRay1.CountCrossPtWithLine(cLine, cPt[1]);
				cBcs.ConvFromBase(cPt[1]);
				cPt[2] = m_cRp[3];
				cPt[3] = m_cRp[2];
				cPt[4] = m_cRp[1];
				nNum = 5;
			}
			else if ((bCross1 & CROSS_12) && (bCross1 & CROSS_23)){
				cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
				cRay1.CountCrossPtWithLine(cLine, cPt[0]);
				cBcs.ConvFromBase(cPt[0]);
				cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
				cRay1.CountCrossPtWithLine(cLine, cPt[1]);
				cBcs.ConvFromBase(cPt[1]);
				cPt[2] = m_cRp[2];
				nNum = 3;
			}
			else if ((bCross1 & CROSS_12) && (bCross1 & CROSS_30)){
				cPt[0] = m_cRp[3];
				cPt[1] = m_cRp[2];
				cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
				cRay1.CountCrossPtWithLine(cLine, cPt[2]);
				cBcs.ConvFromBase(cPt[2]);
				cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
				cRay1.CountCrossPtWithLine(cLine, cPt[3]);
				cBcs.ConvFromBase(cPt[3]);
				nNum = 4;
			}
			else if ((bCross1 & CROSS_23) && (bCross1 & CROSS_30)){
				cPt[0] = m_cRp[3];
				cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
				cRay1.CountCrossPtWithLine(cLine, cPt[1]);
				cBcs.ConvFromBase(cPt[1]);
				cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
				cRay1.CountCrossPtWithLine(cLine, cPt[2]);
				cBcs.ConvFromBase(cPt[2]);
				nNum = 3;
			}
		}
		else {				// bCross1 & bCross2
			if ((bCross1 & CROSS_01) && (bCross1 & CROSS_12)){
				cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
				cRay1.CountCrossPtWithLine(cLine, cPt[0]);
				cBcs.ConvFromBase(cPt[0]);
				cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
				cRay1.CountCrossPtWithLine(cLine, cPt[1]);
				cBcs.ConvFromBase(cPt[1]);
				if ((bCross2 & CROSS_01) && (bCross2 & CROSS_12)){
					cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
					cRay2.CountCrossPtWithLine(cLine, cPt[2]);
					cBcs.ConvFromBase(cPt[2]);
					cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
					nNum = 4;
				}
				else if ((bCross2 & CROSS_12) && (bCross2 & CROSS_30)){
					cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
					cRay2.CountCrossPtWithLine(cLine, cPt[2]);
					cBcs.ConvFromBase(cPt[2]);
					cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
					cPt[4] = m_cRp[0];
					nNum = 5;
				}
				else if ((bCross2 & CROSS_23) && (bCross2 & CROSS_30)){
					cPt[2] = m_cRp[2];
					cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
					cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
					cRay2.CountCrossPtWithLine(cLine, cPt[4]);
					cBcs.ConvFromBase(cPt[4]);
					cPt[5] = m_cRp[0];
					nNum = 6;
				}
				else if ((bCross2 & CROSS_12) && (bCross2 & CROSS_23)){
					cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
					cRay2.CountCrossPtWithLine(cLine, cPt[2]);
					cBcs.ConvFromBase(cPt[2]);
					cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
					cPt[4] = m_cRp[3];
					cPt[5] = m_cRp[0];
					nNum = 6;
				}
				else if ((bCross2 & CROSS_01) && (bCross2 & CROSS_23)){
					cPt[2] = m_cRp[2];
					cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
					cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
					cRay2.CountCrossPtWithLine(cLine, cPt[4]);
					cBcs.ConvFromBase(cPt[4]);
					nNum = 5;
				}
			}
			else if ((bCross1 & CROSS_01) && (bCross1 & CROSS_23)){
				cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
				cRay1.CountCrossPtWithLine(cLine, cPt[0]);
				cBcs.ConvFromBase(cPt[0]);
				cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
				cRay1.CountCrossPtWithLine(cLine, cPt[1]);
				cBcs.ConvFromBase(cPt[1]);
				if ((bCross2 & CROSS_01) && (bCross2 & CROSS_23)){
					cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
					cRay2.CountCrossPtWithLine(cLine, cPt[2]);
					cBcs.ConvFromBase(cPt[2]);
					cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
					nNum = 4;
				}
				else if ((bCross2 & CROSS_23) && (bCross2 & CROSS_30)){
					cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
					cRay2.CountCrossPtWithLine(cLine, cPt[0]);
					cBcs.ConvFromBase(cPt[0]);
					cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
					cPt[4] = m_cRp[0];
					nNum = 5;
				}
				else if ((bCross2 & CROSS_12) && (bCross2 & CROSS_23)){
					cPt[2] = m_cRp[1];
					cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
					cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
					cRay2.CountCrossPtWithLine(cLine, cPt[4]);
					cBcs.ConvFromBase(cPt[4]);
					nNum = 5;
				}
			}
			else if ((bCross1 & CROSS_01) && (bCross1 & CROSS_30)){
				cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
				cRay1.CountCrossPtWithLine(cLine, cPt[0]);
				cBcs.ConvFromBase(cPt[0]);
				cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
				cRay1.CountCrossPtWithLine(cLine, cPt[1]);
				cBcs.ConvFromBase(cPt[1]);
				if ((bCross2 & CROSS_01) && (bCross2 & CROSS_30)){
					cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
					cRay2.CountCrossPtWithLine(cLine, cPt[2]);
					cBcs.ConvFromBase(cPt[2]);
					cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
					nNum = 4;
				}
				else if ((bCross2 & CROSS_12) && (bCross2 & CROSS_30)){
					cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
					cRay2.CountCrossPtWithLine(cLine, cPt[2]);
					cBcs.ConvFromBase(cPt[2]);
					cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
					cPt[4] = m_cRp[1];
					nNum = 5;
				}
				else if ((bCross2 & CROSS_23) && (bCross2 & CROSS_30)){
					cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
					cRay2.CountCrossPtWithLine(cLine, cPt[2]);
					cBcs.ConvFromBase(cPt[2]);
					cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
					cPt[4] = m_cRp[2];
					cPt[5] = m_cRp[1];
					nNum = 6;
				}
				else if ((bCross2 & CROSS_12) && (bCross2 & CROSS_23)){
					cPt[2] = m_cRp[3];
					cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
					cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
					cRay2.CountCrossPtWithLine(cLine, cPt[4]);
					cBcs.ConvFromBase(cPt[4]);
					cPt[5] = m_cRp[1];
					nNum = 6;
				}
				else if ((bCross2 & CROSS_01) && (bCross2 & CROSS_23)){
					cPt[2] = m_cRp[3];
					cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
					cLine.m_cSp = m_cRp[0];	cLine.m_cEp = m_cRp[1];
					cRay2.CountCrossPtWithLine(cLine, cPt[4]);
					cBcs.ConvFromBase(cPt[4]);
					nNum = 5;
				}
			}
			else if ((bCross1 & CROSS_12) && (bCross1 & CROSS_30)){
				cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
				cRay1.CountCrossPtWithLine(cLine, cPt[0]);
				cBcs.ConvFromBase(cPt[0]);
				cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
				cRay1.CountCrossPtWithLine(cLine, cPt[1]);
				cBcs.ConvFromBase(cPt[1]);
				if ((bCross2 & CROSS_12) && (bCross2 & CROSS_30)){
					cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
					cRay2.CountCrossPtWithLine(cLine, cPt[2]);
					cBcs.ConvFromBase(cPt[2]);
					cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
					nNum = 4;
				}
				else if ((bCross2 & CROSS_12) && (bCross2 & CROSS_23)){
					cPt[2] = m_cRp[3];
					cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
					cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
					cRay2.CountCrossPtWithLine(cLine, cPt[4]);
					cBcs.ConvFromBase(cPt[4]);
					nNum = 5;
				}
				else if ((bCross2 & CROSS_23) && (bCross2 & CROSS_30)){
					cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
					cRay2.CountCrossPtWithLine(cLine, cPt[2]);
					cBcs.ConvFromBase(cPt[2]);
					cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
					cRay2.CountCrossPtWithLine(cLine, cPt[3]);
					cBcs.ConvFromBase(cPt[3]);
					cPt[4] = m_cRp[2];
					nNum = 5;
				}
			}
			else if ((bCross1 & CROSS_12) && (bCross1 & CROSS_23)){
				cLine.m_cSp = m_cRp[1];	cLine.m_cEp = m_cRp[2];
				cRay1.CountCrossPtWithLine(cLine, cPt[0]);
				cBcs.ConvFromBase(cPt[0]);
				cRay2.CountCrossPtWithLine(cLine, cPt[1]);
				cBcs.ConvFromBase(cPt[1]);
				cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
				cRay2.CountCrossPtWithLine(cLine, cPt[2]);
				cBcs.ConvFromBase(cPt[2]);
				cRay1.CountCrossPtWithLine(cLine, cPt[3]);
				cBcs.ConvFromBase(cPt[3]);
				nNum = 4;
			}
			else if ((bCross1 & CROSS_23) && (bCross1 & CROSS_30)){
				cLine.m_cSp = m_cRp[2];	cLine.m_cEp = m_cRp[3];
				cRay1.CountCrossPtWithLine(cLine, cPt[0]);
				cBcs.ConvFromBase(cPt[0]);
				cRay2.CountCrossPtWithLine(cLine, cPt[1]);
				cBcs.ConvFromBase(cPt[1]);
				cLine.m_cSp = m_cRp[3];	cLine.m_cEp = m_cRp[0];
				cRay2.CountCrossPtWithLine(cLine, cPt[2]);
				cBcs.ConvFromBase(cPt[2]);
				cRay1.CountCrossPtWithLine(cLine, cPt[3]);
				cBcs.ConvFromBase(cPt[3]);
				nNum = 4;
			}
		}
		if (nNum > 0){
			LOGPEN oldPen = pDC->SelectNullPen();
			int nTriCount = 0;
			pDC->PolyPolygon(cPt, &nNum, 1, NULL, nTriCount);
			pDC->RestoreObject(oldPen);
		}
	}*/
}

//---------------------------------------------
void CCAD_GemBvtxPara::DrawStyleArc(CCAD_DC *pDC, CAD_FLOAT fR1, CAD_FLOAT fR2)
{
	if ( fR1 < 0 )
		fR1 = 0;
	if ( fR2 > m_cArc.m_cDelta.GetAbsRadian() )
		fR2 = m_cArc.m_cDelta.GetAbsRadian();

	CAD_FLOAT	fW1 = (m_fEndW1 - m_fStartW1) / m_cArc.m_cDelta.GetAbsRadian();
	CAD_FLOAT	fW2 = (m_fEndW2 - m_fStartW2) / m_cArc.m_cDelta.GetAbsRadian();
	CAD_FLOAT	fStartW1 = m_fStartW1 + fW1 * fR1;
	CAD_FLOAT	fStartW2 = m_fStartW2 + fW2 * fR1;
	CAD_FLOAT	fEndW1 = m_fStartW1 + fW1 * fR2;
	CAD_FLOAT	fEndW2 = m_fStartW2 + fW2 * fR2;

	long	nStartW = pDC->ConvScalarWorldToScrn(fStartW1 + fStartW2);
	long	nEndW = pDC->ConvScalarWorldToScrn(fEndW1 + fEndW2);

	fR2 = m_cArc.m_cDelta.GetAbsRadian() - fR2;
	CCAD_GemArc	cArc(m_cArc);
	if ( cArc.m_cDelta.GetDegree() > 0 ) {
		CAD_FLOAT	fAngle;
		fAngle = cArc.m_cAlfa.GetRadian() + fR1;
		cArc.m_cAlfa = fAngle;
		fAngle = cArc.m_cDelta.GetRadian();
		fAngle -= (fR1+fR2);
		cArc.m_cDelta = fAngle;
	}
	else {
		CAD_FLOAT	fAngle;
		fAngle = cArc.m_cAlfa.GetRadian() - fR1;
		cArc.m_cAlfa = fAngle;
		fAngle = cArc.m_cDelta.GetRadian();
		fAngle += (fR1+fR2);
		cArc.m_cDelta = fAngle;
	}

	if ( nStartW <= IGNO_WIDTH && nEndW <= IGNO_WIDTH ){
		cArc.Draw(pDC);
	}
	else {
		short n = 72;

		cArc.m_cDelta /= n;
		fW1 = (fEndW1 - fStartW1) / n;
		fW2 = (fEndW2 - fStartW2) / n;
		short	nNum = 4;

		if (cArc.m_cDelta.GetDegree() > 0){
			cArc.m_cCp.CountVector2D(m_cRp[0], (cArc.m_fR+fStartW1), cArc.m_cAlfa.sin(), cArc.m_cAlfa.cos());
			cArc.m_cCp.CountVector2D(m_cRp[1], (cArc.m_fR-fStartW2), cArc.m_cAlfa.sin(), cArc.m_cAlfa.cos());
		}
		else {
			cArc.m_cCp.CountVector2D(m_cRp[0], (cArc.m_fR+fStartW2), cArc.m_cAlfa.sin(), cArc.m_cAlfa.cos());
			cArc.m_cCp.CountVector2D(m_cRp[1], (cArc.m_fR-fStartW1), cArc.m_cAlfa.sin(), cArc.m_cAlfa.cos());
		}
		int	i;
		for (i=1; i<=n; i++) {
			cArc.m_cAlfa.RotateAngle(cArc.m_cDelta);
			fStartW1 += fW1;
			fStartW2 += fW2;
			if (cArc.m_cDelta.GetDegree() > 0 ) {
				cArc.m_cCp.CountVector2D(m_cRp[3], (cArc.m_fR+fStartW1), cArc.m_cAlfa.sin(), cArc.m_cAlfa.cos());
				cArc.m_cCp.CountVector2D(m_cRp[2], (cArc.m_fR-fStartW2), cArc.m_cAlfa.sin(), cArc.m_cAlfa.cos());
			}
			else {
				cArc.m_cCp.CountVector2D(m_cRp[3], (cArc.m_fR+fStartW2), cArc.m_cAlfa.sin(), cArc.m_cAlfa.cos());
				cArc.m_cCp.CountVector2D(m_cRp[2], (cArc.m_fR-fStartW1), cArc.m_cAlfa.sin(), cArc.m_cAlfa.cos());
			}
			nNum = 4;
			CCAD_Point	cPt[4];
			memcpy(cPt, m_cRp, sizeof(CCAD_Point)*nNum);
			LOGPEN oldPen = pDC->SelectNullPen();
			int nTriCount = 0;
			pDC->PolyPolygon(cPt, &nNum, 1, NULL, nTriCount);
			pDC->RestoreObject(oldPen);
			m_cRp[0] = m_cRp[3];
			m_cRp[1] = m_cRp[2];
		}
	}
}

//---------------------------------------------
void CCAD_GemBvtxPara::Draw(CCAD_DC *pDC)
{
	int		i;
	long	nStartW = pDC->ConvScalarWorldToScrn(m_fStartW1 + m_fStartW2);
	long	nEndW = pDC->ConvScalarWorldToScrn(m_fEndW1 + m_fEndW2);
	if (m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy()) {
		if ( nStartW <= IGNO_WIDTH && nEndW <= IGNO_WIDTH ) {
			pDC->MoveTo(m_cRp[0]);
			pDC->LineTo(m_cRp[3]);
		}
		else {
			short	nNum = 4;
			CCAD_Point	cPt[4];
			memcpy(cPt, m_cRp, sizeof(CCAD_Point)*nNum);
			LOGPEN oldPen = pDC->SelectNullPen();
			int nTriCount = 0;
			pDC->PolyPolygon(cPt, &nNum, 1, NULL, nTriCount);
			pDC->RestoreObject(oldPen);
		}
	}
	else {
		if (nStartW <= IGNO_WIDTH && nEndW <= IGNO_WIDTH){
			m_cArc.Draw(pDC);
		}
		else {
			short n = 72;	//(short)(m_cArc.m_cDelta.GetAbsDegree() / DOUBLE_NINETY * 18);

			CCAD_Angle	cAlfa = m_cArc.m_cAlfa;
			CCAD_Angle	cDelta = m_cArc.m_cDelta;
			cDelta /= n;
			CAD_FLOAT		fStartW1 = m_fStartW1;
			CAD_FLOAT		fStartW2 = m_fStartW2;
			CAD_FLOAT		fW1 = (m_fEndW1 - m_fStartW1) / n;
			CAD_FLOAT		fW2 = (m_fEndW2 - m_fStartW2) / n;

			short	nNum = 4;
			if ( m_cArc.m_cDelta.GetDegree() > 0 ) {
				m_cArc.m_cCp.CountVector2D(m_cRp[0], (m_cArc.m_fR+fStartW1), cAlfa.sin(), cAlfa.cos());
				m_cArc.m_cCp.CountVector2D(m_cRp[1], (m_cArc.m_fR-fStartW2), cAlfa.sin(), cAlfa.cos());
			}
			else {
				m_cArc.m_cCp.CountVector2D(m_cRp[0], (m_cArc.m_fR+fStartW2), cAlfa.sin(), cAlfa.cos());
				m_cArc.m_cCp.CountVector2D(m_cRp[1], (m_cArc.m_fR-fStartW1), cAlfa.sin(), cAlfa.cos());
			}
			for (i=1; i<=n; i++) {
				if (i == n){
					cAlfa = m_cArc.m_cAlfa;
					cAlfa.RotateAngle(m_cArc.m_cDelta);
				}
				else
					cAlfa.RotateAngle(cDelta);
				fStartW1 += fW1;
				fStartW2 += fW2;
				if ( m_cArc.m_cDelta.GetDegree() > 0 ) {
					m_cArc.m_cCp.CountVector2D(m_cRp[3], (m_cArc.m_fR+fStartW1), cAlfa.sin(), cAlfa.cos());
					m_cArc.m_cCp.CountVector2D(m_cRp[2], (m_cArc.m_fR-fStartW2), cAlfa.sin(), cAlfa.cos());
				}
				else {
					m_cArc.m_cCp.CountVector2D(m_cRp[3], (m_cArc.m_fR+fStartW2), cAlfa.sin(), cAlfa.cos());
					m_cArc.m_cCp.CountVector2D(m_cRp[2], (m_cArc.m_fR-fStartW1), cAlfa.sin(), cAlfa.cos());
				}
				nNum = 4;
				CCAD_Point	cPt[4];
				memcpy(cPt, m_cRp, sizeof(CCAD_Point)*nNum);
				LOGPEN oldPen = pDC->SelectNullPen();
				int nTriCount = 0;
				pDC->PolyPolygon(cPt, &nNum, 1, NULL, nTriCount);
				pDC->RestoreObject(oldPen);
				m_cRp[0] = m_cRp[3];
				m_cRp[1] = m_cRp[2];
			}
		}
	}
}

//---------------------------------------------
void CCAD_GemBvtxPara::FrameDraw(CCAD_DC *pDC, const CCAD_Coord3D &cBcs, BYTE bIndex)
{
	int		i;
	long	nStartW = pDC->ConvScalarWorldToScrn(m_fStartW1 + m_fStartW2);
	long	nEndW = pDC->ConvScalarWorldToScrn(m_fEndW1 + m_fEndW2);
	CCAD_GemLine	cLine;
	cLine.m_cBcs = cBcs;
	if (m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy()) {
		if (nStartW <= IGNO_WIDTH && nEndW <= IGNO_WIDTH){
			cLine.m_cSp = m_cRp[0];
			cLine.m_cEp = m_cRp[3];
			cLine.Draw(pDC);
		}
		else {
			if (bIndex & START_INDEX){
				cLine.m_cSp = m_cRp[0];
				cLine.m_cEp = m_cRp[1];
				cLine.Draw(pDC);
			}
			if (bIndex & END_INDEX){
				cLine.m_cSp = m_cRp[2];
				cLine.m_cEp = m_cRp[3];
				cLine.Draw(pDC);
			}
			cLine.m_cSp = m_cRp[0];
			cLine.m_cEp = m_cRp[3];
			cLine.Draw(pDC);
			cLine.m_cSp = m_cRp[1];
			cLine.m_cEp = m_cRp[2];
			cLine.Draw(pDC);
		}
	}
	else {
		if (nStartW <= IGNO_WIDTH && nEndW <= IGNO_WIDTH)
			m_cArc.Draw(pDC);
		else {
			short n =  72; //(short)(m_cArc.m_cDelta.GetAbsDegree() / DOUBLE_NINETY * 18);

			CCAD_Angle	cAlfa = m_cArc.m_cAlfa;
			CCAD_Angle	cDelta = m_cArc.m_cDelta;
			cDelta /= n;
			CAD_FLOAT	fStartW1 = m_fStartW1;
			CAD_FLOAT	fStartW2 = m_fStartW2;
			CAD_FLOAT	fW1 = (m_fEndW1 - m_fStartW1) / n;
			CAD_FLOAT	fW2 = (m_fEndW2 - m_fStartW2) / n;

			if (m_cArc.m_cDelta.GetDegree() > 0){
				m_cArc.m_cCp.CountVector2D(m_cRp[0], (m_cArc.m_fR+fStartW1), cAlfa.sin(), cAlfa.cos());
				m_cArc.m_cCp.CountVector2D(m_cRp[1], (m_cArc.m_fR-fStartW2), cAlfa.sin(), cAlfa.cos());
			}
			else {
				m_cArc.m_cCp.CountVector2D(m_cRp[0], (m_cArc.m_fR+fStartW2), cAlfa.sin(), cAlfa.cos());
				m_cArc.m_cCp.CountVector2D(m_cRp[1], (m_cArc.m_fR-fStartW1), cAlfa.sin(), cAlfa.cos());
			}
			if (bIndex & START_INDEX){
				cLine.m_cSp = m_cRp[0];
				cLine.m_cEp = m_cRp[1];
				cLine.Draw(pDC);
			}
			for (i=1; i<=n; i++) {
				cAlfa.RotateAngle(cDelta);
				fStartW1 += fW1;
				fStartW2 += fW2;
				if (m_cArc.m_cDelta.GetDegree() > 0) {
					m_cArc.m_cCp.CountVector2D(m_cRp[3], (m_cArc.m_fR+fStartW1), cAlfa.sin(), cAlfa.cos());
					m_cArc.m_cCp.CountVector2D(m_cRp[2], (m_cArc.m_fR-fStartW2), cAlfa.sin(), cAlfa.cos());
				}
				else {
					m_cArc.m_cCp.CountVector2D(m_cRp[3], (m_cArc.m_fR+fStartW2), cAlfa.sin(), cAlfa.cos());
					m_cArc.m_cCp.CountVector2D(m_cRp[2], (m_cArc.m_fR-fStartW1), cAlfa.sin(), cAlfa.cos());
				}
				cLine.m_cSp = m_cRp[0];
				cLine.m_cEp = m_cRp[3];
				cLine.Draw(pDC);
				if (m_fStartW1 != 0 || m_fStartW2 != 0 || m_fEndW1 != 0 || m_fEndW2 != 0) {
					cLine.m_cSp = m_cRp[1];
					cLine.m_cEp = m_cRp[2];
					cLine.Draw(pDC);
				}
				m_cRp[0] = m_cRp[3];
				m_cRp[1] = m_cRp[2];
			}
			if (bIndex & END_INDEX) {
				cLine.m_cSp = m_cRp[3];
				cLine.m_cEp = m_cRp[2];
				cLine.Draw(pDC);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCAD_GemBelt
//
CCAD_GemBelt::CCAD_GemBelt()
{
	Init();
}

//---------------------------------------------
CCAD_GemBelt::~CCAD_GemBelt()
{
	RemoveAll();
}

//---------------------------------------------
void CCAD_GemBelt::Init()
{
	m_bClose = NO;
	m_bDraw = DRAW_FRAME;
	m_fLength = 0;
	m_fEmpty = 0;
	m_nTot = 0;
	m_pVertex = NULL;
}

//---------------------------------------------
void CCAD_GemBelt::RemoveAll()
{
	if (m_pVertex != NULL && m_nTot > 0)
		delete [] m_pVertex;
	m_pVertex = NULL;
	m_nTot = 0;
}

void CCAD_GemBelt::Remove()
{
	RemoveAll();
}

void CCAD_GemBelt::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_Gem2DEntity::MemCopyTo(pGemEntity, pData1, pData2, pData3);

	((CCAD_GemBelt *)pGemEntity)->m_bClose = m_bClose;
	((CCAD_GemBelt *)pGemEntity)->m_bDraw = m_bDraw;
	((CCAD_GemBelt *)pGemEntity)->m_fLength = m_fLength;
	((CCAD_GemBelt *)pGemEntity)->m_fEmpty = m_fEmpty;
	((CCAD_GemBelt *)pGemEntity)->m_nTot = m_nTot;

	if ( pData1 == NULL )
		pData1 = (LPSTR)new CCAD_GemBvertex[m_nTot];

	((CCAD_GemBelt *)pGemEntity)->m_pVertex = (CCAD_GemBvertex *)pData1;
	int	i;
	for (i=0; i<m_nTot; i++)
		*(((CCAD_GemBelt *)pGemEntity)->m_pVertex + i) = *(m_pVertex + i);
}

//---------------------------------------------
BOOL CCAD_GemBelt::Read(CCAD_File & cFile)
{
	CCAD_Gem2DEntity::Read(cFile);
	cFile.ReadByte(&m_bClose);
	cFile.ReadByte(&m_bDraw);
	cFile.ReadShort(&m_nTot);
	cFile.ReadCAD_Float(&m_fLength);
	cFile.ReadCAD_Float(&m_fEmpty);
	if (m_nTot > 0) {
		try {
			m_pVertex = new CCAD_GemBvertex[m_nTot];
		}
		catch(...) {
			m_nTot = 0;
			return FALSE;
		}
		int	i;
		for (i=0; i<m_nTot; i++)
			(m_pVertex+i)->Read(cFile);
	}
	return TRUE;
}

//---------------------------------------------
BYTE CCAD_GemBelt::DrawBeltStyle(CCAD_DC *pDC) const
{
	if (m_fLength < m_fEmpty){
		if (pDC->ConvScalarWorldToScrn(m_fLength) <= 2)
			return DRAW_FRAME;
	}
	else {
		if (pDC->ConvScalarWorldToScrn(m_fEmpty) <= 2)
			return DRAW_PAINT;
	}
	return (DRAW_PAINT | DRAW_FRAME);
}

//---------------------------------------------
BOOL CCAD_GemBelt::Draw(CCAD_DC *pDC, BOOL /*blRedraw*/)
{
	if (m_pVertex == NULL || m_nTot < 2)
		return false;

	BYTE bStyle = DrawBeltStyle(pDC);
	BOOL	blPaint = FALSE;
	if ( (m_bDraw & DRAW_PAINT) && (bStyle & DRAW_PAINT) &&  !IsPanMoving()) {
		if (!pDC->m_pCamera->m_bPerspect){
			CCAD_GemBvtxPara	cBt, cVt;
			CAD_FLOAT			fPred = 0;
			short	i;
			for (i=0; i<=m_nTot; i++) {
				if (i == m_nTot && (m_bClose == NO || m_nTot < 2 || (m_nTot == 2 && m_pVertex->m_cBulge == 0 && (m_pVertex+1)->m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy())))
					break;

				if (i == 0) {
					cBt = *(m_pVertex+m_nTot-1);
					cVt = *m_pVertex;
					if (m_bClose == NO || m_nTot < 2 || (m_nTot == 2 && m_pVertex->m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy() && (m_pVertex+1)->m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy()))
						cVt.CountPara(m_cBcs, NULL, (m_pVertex+1)->m_cPt);
					else
						cVt.CountPara(m_cBcs, &cBt, (m_pVertex+1)->m_cPt);
				}
				else {
					cBt = cVt;
					cVt = (i == m_nTot) ? *m_pVertex : *(m_pVertex+i);
					if (cBt.m_cPt.m_fX == cVt.m_cPt.m_fX && cBt.m_cPt.m_fY == cVt.m_cPt.m_fY) {
						cVt = cBt;
						continue;
					}
					if (i < m_nTot-1)
						cVt.CountPara(m_cBcs, &cBt, (m_pVertex+i+1)->m_cPt);
					else if (i == m_nTot-1 && m_bClose == YES && (m_nTot > 2 || (m_nTot == 2 && (m_pVertex->m_cBulge.GetAbsDegree() >= CCAD_Global::GetAngleAccy() || (m_pVertex+1)->m_cBulge.GetAbsDegree() >= CCAD_Global::GetAngleAccy()))))
						cVt.CountPara(m_cBcs, &cBt, m_pVertex->m_cPt);
					else if (i == m_nTot)
						cVt.CountPara(m_cBcs, &cBt, (m_pVertex+1)->m_cPt);

					if ( bStyle == DRAW_PAINT)
						cBt.Draw(pDC);
					else
						cBt.StyleDraw(pDC, *this, cVt.m_cPt, fPred);
				}
				blPaint = TRUE;
			}
		}
	}
	if ((m_bDraw & DRAW_FRAME || bStyle == DRAW_FRAME) || !blPaint || IsPanMoving())
		FrameDraw(pDC);
	else {
		LOGPEN oldPen = pDC->SelectNullPen();
		FrameDraw(pDC);
		pDC->RestoreObject(oldPen);
	}
	return TRUE;
}

//---------------------------------------------
void CCAD_GemBelt::FrameDraw(CCAD_DC *pDC)
{
	if (m_pVertex == NULL || m_nTot < 2)
		return;

	CCAD_GemBvtxPara	cBt, cVt;
	BYTE		bIndex = (BYTE)(( m_bClose != YES ) ? START_INDEX : NULL);
	short		i;
	for (i=0; i<=m_nTot; i++) {
		if (i == m_nTot && (m_bClose == NO || m_nTot < 2 || (m_nTot == 2 && m_pVertex->m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy() && (m_pVertex+1)->m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy())))
			break;
		if (i == 0) {
			cBt = *(m_pVertex+m_nTot-1);
			cVt = *m_pVertex;
			if (m_bClose == NO || m_nTot < 2 || (m_nTot == 2 && m_pVertex->m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy() && (m_pVertex+1)->m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy()))
				cVt.CountPara(m_cBcs, NULL, (m_pVertex+1)->m_cPt);
			else
				cVt.CountPara(m_cBcs, &cBt, (m_pVertex+1)->m_cPt);
		}
		else {
			cBt = cVt;
			cVt = (i == m_nTot) ? *m_pVertex : *(m_pVertex+i);
			if (cBt.m_cPt.m_fX == cVt.m_cPt.m_fX && cBt.m_cPt.m_fY == cVt.m_cPt.m_fY) {
				cVt = cBt;
				continue;
			}
			if (i < m_nTot-1)
				cVt.CountPara(m_cBcs, &cBt, (m_pVertex+i+1)->m_cPt);
			else if (i == m_nTot-1 && m_bClose == YES && (m_nTot > 2 || (m_nTot == 2 && (m_pVertex->m_cBulge.GetAbsDegree() >= CCAD_Global::GetAngleAccy() || (m_pVertex+1)->m_cBulge.GetAbsDegree() >= CCAD_Global::GetAngleAccy()))))
				cVt.CountPara(m_cBcs, &cBt, m_pVertex->m_cPt);
			else if (i == m_nTot)
				cVt.CountPara(m_cBcs, &cBt, (m_pVertex+1)->m_cPt);

			if ( i == m_nTot-1 && m_bClose != YES )
				bIndex |= END_INDEX;
			cBt.FrameDraw(pDC, m_cBcs, bIndex);
			bIndex = 0;
		}
	}
}

void CCAD_GemBelt::CountBodyRange(CCAD_Rect &cRange)
{
	if (m_pVertex == NULL || m_nTot < 2)
		return;

	CCAD_GemBvtxPara	cBt, cVt;
	short	i;
	for (i=0; i<=m_nTot; i++) {
		if (i == m_nTot && (m_bClose == NO || m_nTot < 2 || (m_nTot == 2 && m_pVertex->m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy() && (m_pVertex+1)->m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy())))
			break;
		if (i == 0) {
			cBt = *(m_pVertex+m_nTot-1);
			cVt = *m_pVertex;
			if (m_bClose == NO || m_nTot < 2 || (m_nTot == 2 && m_pVertex->m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy() && (m_pVertex+1)->m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy()))
				cVt.CountPara(m_cBcs, NULL, (m_pVertex+1)->m_cPt);
			else
				cVt.CountPara(m_cBcs, &cBt, (m_pVertex+1)->m_cPt);
		}
		else {
			cBt = cVt;
			cVt = (i == m_nTot) ? *m_pVertex : *(m_pVertex+i);
			if (cBt.m_cPt.m_fX == cVt.m_cPt.m_fX && cBt.m_cPt.m_fY == cVt.m_cPt.m_fY) {
				cVt = cBt;
				continue;
			}
			if (i < m_nTot-1)
				cVt.CountPara(m_cBcs, &cBt, (m_pVertex+i+1)->m_cPt);
			else if (i == m_nTot-1 && m_bClose == YES && (m_nTot > 2 || (m_nTot == 2 && (m_pVertex->m_cBulge.GetAbsDegree() >= CCAD_Global::GetAngleAccy() || (m_pVertex+1)->m_cBulge.GetAbsDegree() >= CCAD_Global::GetAngleAccy()))))
				cVt.CountPara(m_cBcs, &cBt, m_pVertex->m_cPt);
			else if (i == m_nTot)
				cVt.CountPara(m_cBcs, &cBt, (m_pVertex+1)->m_cPt);
			cBt.CountBodyRange(cRange);
		}
	}
}

void CCAD_GemBelt::CountVcsRange(CCAD_DC *pDC, CCAD_Rect &cRange)
{
	ASSERT(pDC != NULL);

	if (m_pVertex == NULL || m_nTot < 2)
		return;

	CCAD_GemBvtxPara	cBt, cVt;
	short	i;
	for (i=0; i<=m_nTot; i++) {
		if (i == m_nTot && (m_bClose == NO || m_nTot < 2 || (m_nTot == 2 && m_pVertex->m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy() && (m_pVertex+1)->m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy())))
			break;
		if (i == 0) {
			cBt = *(m_pVertex+m_nTot-1);
			cVt = *m_pVertex;
			if (m_bClose == NO || m_nTot < 2 || (m_nTot == 2 && m_pVertex->m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy() && (m_pVertex+1)->m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy()))
				cVt.CountPara(m_cBcs, NULL, (m_pVertex+1)->m_cPt);
			else
				cVt.CountPara(m_cBcs, &cBt, (m_pVertex+1)->m_cPt);
		}
		else {
			cBt = cVt;
			cVt = (i == m_nTot) ? *m_pVertex : *(m_pVertex+i);
			if (cBt.m_cPt.m_fX == cVt.m_cPt.m_fX && cBt.m_cPt.m_fY == cVt.m_cPt.m_fY) {
				cVt = cBt;
				continue;
			}
			if (i < m_nTot-1)
				cVt.CountPara(m_cBcs, &cBt, (m_pVertex+i+1)->m_cPt);
			else if (i == m_nTot-1 && m_bClose == YES && (m_nTot > 2 || (m_nTot == 2 && (m_pVertex->m_cBulge.GetAbsDegree() >= CCAD_Global::GetAngleAccy() || (m_pVertex+1)->m_cBulge.GetAbsDegree() >= CCAD_Global::GetAngleAccy()))))
				cVt.CountPara(m_cBcs, &cBt, m_pVertex->m_cPt);
			else if (i == m_nTot)
				cVt.CountPara(m_cBcs, &cBt, (m_pVertex+1)->m_cPt);
			cBt.CountVcsRange(pDC, cRange);
		}
	}
}
