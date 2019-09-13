#include "stdafx.h"
#include "CAD_GemCurve.h"
#include "CAD_GemLine.h"

/////////////////////////////////////////////////////////////////////////////
// CCAD_GemCurve
//
CCAD_GemCurve::CCAD_GemCurve()
{
	Init();
}

//----------------------------------------------
CCAD_GemCurve::~CCAD_GemCurve()
{
	RemoveAll();
}

//----------------------------------------------
void CCAD_GemCurve::Init()
{
	m_bKind = CURVE_3SPLINE;
	m_bClose = NO;
	m_bFirstClose = NO;
	m_bDraw = DRAW_FRAME;
	m_nTot = 0;
	m_pPt = NULL;
	m_nCurveTot = 0;
	m_pCurvePt = NULL;
	m_pWeight = NULL;
	m_nKnotTot = 0;
	m_pKnot = NULL;
}

//----------------------------------------------
void CCAD_GemCurve::RemoveAll()
{
	if (m_pPt != NULL)
		delete [] m_pPt;
	m_pPt = NULL;
	if (m_pCurvePt != NULL)
		delete [] m_pCurvePt;
	m_pCurvePt = NULL;
	if (m_pWeight != NULL)
		delete [] m_pWeight;
	m_pWeight = NULL;
	if (m_pKnot != NULL)
		delete [] m_pKnot;
	m_pKnot = NULL;

	m_nTot = 0;
	m_nCurveTot = 0;
	m_nKnotTot = 0;
}

void CCAD_GemCurve::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_GemThickEntity::MemCopyTo(pGemEntity, pData1, pData2, pData3);
	
	((CCAD_GemCurve *)pGemEntity)->m_bKind = m_bKind;
	((CCAD_GemCurve *)pGemEntity)->m_bClose = m_bClose;
	((CCAD_GemCurve *)pGemEntity)->m_bFirstClose = m_bFirstClose;
	((CCAD_GemCurve *)pGemEntity)->m_bDraw = m_bDraw;
	((CCAD_GemCurve *)pGemEntity)->m_nCurveTot = m_nCurveTot;
	((CCAD_GemCurve *)pGemEntity)->m_nKnotTot = m_nKnotTot;

	if (pData1 == NULL){
		pData1 = (LPSTR)new CCAD_Vector[m_nCurveTot];
		pData2 = (LPSTR)new CAD_FLOAT[m_nCurveTot];
		pData3 = (LPSTR)new CAD_FLOAT[m_nKnotTot];
	}

	int	i;
	((CCAD_GemCurve *)pGemEntity)->m_pCurvePt = (CCAD_Point *)pData1;
	for (i=0; i<m_nCurveTot; i++)
		*(((CCAD_GemCurve *)pGemEntity)->m_pCurvePt + i) = *(m_pCurvePt + i);

	((CCAD_GemCurve *)pGemEntity)->m_pWeight = (CAD_FLOAT *)pData2;
	for (i=0; i<m_nCurveTot; i++)
		*(((CCAD_GemCurve *)pGemEntity)->m_pWeight + i) = *(m_pWeight + i);

	((CCAD_GemCurve *)pGemEntity)->m_pKnot = (CAD_FLOAT *)pData3;
	for (i=0; i<m_nKnotTot; i++)
		*(((CCAD_GemCurve *)pGemEntity)->m_pKnot + i) = *(m_pKnot + i);
}

void CCAD_GemCurve::Remove()
{
	RemoveAll();
}

//----------------------------------------------
BOOL CCAD_GemCurve::ThreeSplineTempVariables()
{
	short	L, N;
	short	M = 4;
	L = (short)(m_nTot - 1);
	N = (short)(m_nTot + 1);
	m_nCurveTot = (short)(m_nTot+2);
	m_nKnotTot = (short)(m_nCurveTot + M);
	
	if (m_pCurvePt != NULL){
		delete []m_pCurvePt;
		m_pCurvePt = NULL;
	}
	if (m_pWeight != NULL){
		delete []m_pWeight;
		m_pWeight = NULL;
	}
	if (m_pKnot != NULL){
		delete []m_pKnot;
		m_pKnot = NULL;
	}

	try {
		m_pCurvePt = new CCAD_Vector[m_nCurveTot];
		m_pWeight = new CAD_FLOAT[m_nCurveTot];
		m_pKnot = (CAD_FLOAT *)new CAD_FLOAT[m_nKnotTot];
	}
	catch(...) {
		if (m_pCurvePt != NULL){
			delete []m_pCurvePt;
			m_pCurvePt = NULL;
		}
		if (m_pWeight != NULL){
			delete []m_pWeight;
			m_pWeight = NULL;
		}
		if (m_pKnot != NULL){
			delete []m_pKnot;
			m_pKnot = NULL;
		}
		return FALSE;
	}

	int	i;
	for ( i=0; i<m_nCurveTot; i++ )
		m_pWeight[i] = 1.0;

	m_pKnot[0] = 0.0;
	m_pKnot[1] = m_pPt->CountLength(m_pPt[1]);
	for (i=2; i<m_nTot; i++)
	{
		m_pKnot[i] = m_pKnot[i-1] + m_pPt[i-1].CountLength(m_pPt[i]);
	}

// Knot Delta
	CAD_FLOAT	*pDeltaU = NULL;
	CAD_FLOAT	*pCoeffA = NULL;
	CAD_FLOAT	*pCoeffB = NULL;
	CAD_FLOAT	*pCoeffC = NULL;
	CAD_FLOAT	*pCoeffP = NULL;
	CAD_FLOAT	*pCoeffQ = NULL;
	CAD_FLOAT	*pCoeffR = NULL;
	CAD_FLOAT	*pCoeffZ = NULL;
	CAD_FLOAT	*pCoeffr = NULL;
	try {
		pDeltaU = (CAD_FLOAT *)new CAD_FLOAT[m_nTot+1];
		pCoeffA = (CAD_FLOAT *)new CAD_FLOAT[m_nTot];
		pCoeffB = (CAD_FLOAT *)new CAD_FLOAT[m_nTot];
		pCoeffC = (CAD_FLOAT *)new CAD_FLOAT[m_nTot];
		pCoeffP = (CAD_FLOAT *)new CAD_FLOAT[m_nTot];
		pCoeffQ = (CAD_FLOAT *)new CAD_FLOAT[m_nTot];
		pCoeffR = (CAD_FLOAT *)new CAD_FLOAT[m_nTot];
		pCoeffZ = (CAD_FLOAT *)new CAD_FLOAT[m_nTot];
		pCoeffr = (CAD_FLOAT *)new CAD_FLOAT[m_nTot];
	}
	catch(...) {
		if (pDeltaU != NULL)
			delete []pDeltaU;
		if (pCoeffA != NULL)
			delete []pCoeffA;
		if (pCoeffB != NULL)
			delete []pCoeffB;
		if (pCoeffC != NULL)
			delete []pCoeffC;
		if (pCoeffP != NULL)
			delete []pCoeffP;
		if (pCoeffQ != NULL)
			delete []pCoeffQ;
		if (pCoeffR != NULL)
			delete []pCoeffR;
		if (pCoeffZ != NULL)
			delete []pCoeffZ;
		if (pCoeffr != NULL)
			delete []pCoeffr;
		return FALSE;
	}
		
	pDeltaU[0] = 0;
	for ( i=1; i<m_nTot; i++ )
	{
		pDeltaU[i] = m_pKnot[i] - m_pKnot[i-1];
	}
	pDeltaU[m_nTot] = 0;

// CalcuCoeff()
	pCoeffA[1] = pDeltaU[2]*pDeltaU[2]/(pDeltaU[1]+pDeltaU[2]);
	pCoeffB[0] = pCoeffB[L] = 1;
	pCoeffB[1] = pDeltaU[2]*pDeltaU[1]/(pDeltaU[1]+pDeltaU[2]) +
		   pDeltaU[1]*(pDeltaU[2]+pDeltaU[3])/(pDeltaU[1]+pDeltaU[2]+pDeltaU[3]);
	pCoeffC[0] = 0;
	pCoeffC[1] = pDeltaU[1]*pDeltaU[1]/(pDeltaU[1]+pDeltaU[2]+pDeltaU[3]);
	for ( i=2; i<=L-1; i++ ) {
		pCoeffA[i] = pDeltaU[i+1]*pDeltaU[i+1]/(pDeltaU[i-1]+pDeltaU[i]+pDeltaU[i+1]);
		pCoeffB[i] = pDeltaU[i+1]*(pDeltaU[i-1]+pDeltaU[i])/(pDeltaU[i-1]+pDeltaU[i]+pDeltaU[i+1]) +
			   pDeltaU[i]*(pDeltaU[i+1]+pDeltaU[i+2])/(pDeltaU[i]+pDeltaU[i+1]+pDeltaU[i+2]);
		pCoeffC[i] = pDeltaU[i]*pDeltaU[i]/(pDeltaU[i]+pDeltaU[i+1]+pDeltaU[i+2]);
	}
	pCoeffA[L] = 0;

//	LUDivision

	pCoeffQ[0] = pCoeffB[0];
	for ( i=0; i<L; i++ ) {
		pCoeffR[i] = pCoeffC[i];
	}
	for ( i=1; i<=L; i++ ) {
		pCoeffP[i] = pCoeffA[i]/pCoeffQ[i-1];
		pCoeffQ[i] = pCoeffB[i] - pCoeffP[i]*pCoeffC[i-1];
	}

//	AutoCalcuTangent
	CAD_FLOAT	beta;
	CCAD_Vector	StartVec, EndVec;

	beta = pDeltaU[1] + pDeltaU[2];
	StartVec.m_fX = -(2*pDeltaU[1] + pDeltaU[2])*m_pPt[0].m_fX/pDeltaU[1]/beta + 
		beta*m_pPt[1].m_fX/pDeltaU[1]/pDeltaU[2] - pDeltaU[1]*m_pPt[2].m_fX/pDeltaU[2]/beta;
	StartVec.m_fY = -(2*pDeltaU[1] + pDeltaU[2])*m_pPt[0].m_fY/pDeltaU[1]/beta + 
		beta*m_pPt[1].m_fY/pDeltaU[1]/pDeltaU[2] - pDeltaU[1]*m_pPt[2].m_fY/pDeltaU[2]/beta;

	beta = pDeltaU[L-1] + pDeltaU[L];
	EndVec.m_fX = pDeltaU[L]*m_pPt[L-2].m_fX/pDeltaU[L-1]/beta - beta*m_pPt[L-1].m_fX/pDeltaU[L-1]/pDeltaU[L] + 
		(2*pDeltaU[L] + pDeltaU[L-1])*m_pPt[L].m_fX/beta/pDeltaU[L];
	EndVec.m_fY = pDeltaU[L]*m_pPt[L-2].m_fY/pDeltaU[L-1]/beta - beta*m_pPt[L-1].m_fY/pDeltaU[L-1]/pDeltaU[L] + 
		(2*pDeltaU[L] + pDeltaU[L-1])*m_pPt[L].m_fY/beta/pDeltaU[L];

	CAD_FLOAT	fLength;
	fLength = (CAD_FLOAT)sqrt(StartVec.m_fX*StartVec.m_fX + StartVec.m_fY*StartVec.m_fY);
	StartVec.m_fX /= fLength;	
	StartVec.m_fY /= fLength;

	if ( m_bClose ) {
		EndVec = StartVec;
		m_bFirstClose = YES;
	}
	else {
		fLength = (CAD_FLOAT)sqrt(EndVec.m_fX*EndVec.m_fX + EndVec.m_fY*EndVec.m_fY);
		EndVec.m_fX /= fLength;
		EndVec.m_fY /= fLength;
	}

//	CalcuSomeSplinePoly(long k)
	m_pCurvePt[0] = m_pPt[0];

	m_pCurvePt[1].m_fX      = m_pPt[0].m_fX + m_pPt[0].CountLength(m_pPt[1])*StartVec.m_fX/3;
	m_pCurvePt[1].m_fY      = m_pPt[0].m_fY + m_pPt[0].CountLength(m_pPt[1])*StartVec.m_fY/3;
	m_pCurvePt[m_nCurveTot-2].m_fX = m_pPt[L].m_fX - m_pPt[L-1].CountLength(m_pPt[L])*EndVec.m_fX/3;
	m_pCurvePt[m_nCurveTot-2].m_fY = m_pPt[L].m_fY - m_pPt[L-1].CountLength(m_pPt[L])*EndVec.m_fY/3;

	m_pCurvePt[N] = m_pPt[L];

//	CalcuConst(long k)
	pCoeffr[0] = m_pCurvePt[1].m_fX;
	for (i=1; i<L; i++ )
		pCoeffr[i] = (pDeltaU[i]+pDeltaU[i+1])*m_pPt[i].m_fX;
	pCoeffr[L] = m_pCurvePt[L+1].m_fX;

//	CalcuSplinePoly(long k)
	pCoeffZ[0] = pCoeffr[0];
	for (i=1; i<=L; i++)
		pCoeffZ[i] = pCoeffr[i] - pCoeffP[i]*pCoeffZ[i-1];
	m_pCurvePt[L+1].m_fX = pCoeffZ[L]/pCoeffQ[L];
	for ( i=L-1; i>=0; i-- )
		m_pCurvePt[i+1].m_fX = (pCoeffZ[i]-pCoeffR[i]*m_pCurvePt[i+2].m_fX)/pCoeffQ[i];

//	CalcuConst(long k)
	pCoeffr[0] = m_pCurvePt[1].m_fY;
	for (i=1; i<L; i++)
		pCoeffr[i] = (pDeltaU[i]+pDeltaU[i+1])*m_pPt[i].m_fY;
	pCoeffr[L] = m_pCurvePt[L+1].m_fY;

//	CalcuSplinePoly(long k)
	pCoeffZ[0] = pCoeffr[0];
	for ( i=1; i<=L; i++ )
		pCoeffZ[i] = pCoeffr[i] - pCoeffP[i]*pCoeffZ[i-1];
	m_pCurvePt[L+1].m_fY = pCoeffZ[L]/pCoeffQ[L];
	for ( i=L-1; i>=0; i-- )
		m_pCurvePt[i+1].m_fY = (pCoeffZ[i]-pCoeffR[i]*m_pCurvePt[i+2].m_fY)/pCoeffQ[i];
	
// Reparameterize
	m_pKnot[N+M] = m_pKnot[N+M-1] = m_pKnot[N+M-2] = m_pKnot[N+M-3] = m_pKnot[L];
	for (i=L-1; i>0; i-- )
		m_pKnot[i+3] = m_pKnot[i];
	m_pKnot[3] = m_pKnot[2] = m_pKnot[1] = m_pKnot[0] = 0.0;

	delete []pDeltaU;
	delete []pCoeffA;
	delete []pCoeffB;
	delete []pCoeffC;
	delete []pCoeffP;
	delete []pCoeffQ;
	delete []pCoeffR;
	delete []pCoeffZ;
	delete []pCoeffr;
	return TRUE;
}

//----------------------------------------------
BOOL CCAD_GemCurve::Read(CCAD_File & cFile)
{
	CCAD_GemThickEntity::Read(cFile);
	cFile.ReadByte(&m_bKind);
	cFile.ReadByte(&m_bClose);
	cFile.ReadByte(&m_bDraw);
	cFile.ReadShort(&m_nCurveTot);
	if (m_nCurveTot > 0) {
		try {
			m_pCurvePt = new CCAD_Vector[m_nCurveTot];
			m_pWeight = new CAD_FLOAT[m_nCurveTot];
		}
		catch(...) {
			return FALSE;
		}

		int	i;
		for (i=0; i<m_nCurveTot; i++)
			(m_pCurvePt+i)->Read(cFile);
		for (i=0; i<m_nCurveTot; i++)
			cFile.ReadCAD_Float(m_pWeight+i);
	}
	cFile.ReadShort(&m_nKnotTot);
	if (m_nKnotTot > 0) {
		try {
			m_pKnot = new CAD_FLOAT[m_nKnotTot];
		}
		catch(...) {
			return FALSE;
		}
		for (int i=0; i<m_nKnotTot; i++)
			cFile.ReadCAD_Float(m_pKnot+i);
	}
	return TRUE;
}

BOOL CCAD_GemCurve::IsClosed()
{
	if (m_bClose == YES)
		return TRUE;
	return FALSE;
}

//----------------------------------------------
void CCAD_GemCurve::FrameDraw(CCAD_DC *pDC, short nTotPt, CCAD_Vector *pPt)
{
	pDC->Lines_Begin(LINETYPE_STRIP);
	pDC->MoveTo(pPt[0]);
	int	i;
	for (i=1; i<nTotPt; i++)
		pDC->LineTo(pPt[i]);
	pDC->Lines_End();
}

//----------------------------------------------
BOOL CCAD_GemCurve::Draw(CCAD_DC *pCadDC, BOOL blRedraw)
{
	short nBorder;
	if (m_bKind == CURVE_3SPLINE)
		nBorder = 4;
	else
		nBorder = 3;

	if (m_nCurveTot >= nBorder && m_pCurvePt != NULL) {
		int		nTotPt;

		CCAD_Vector	* pPt = NULL;
		CountDrawPt( nTotPt, pCadDC->m_pPlanInfo->nCurveDivide );
		try {
			pPt = new CCAD_Vector[nTotPt];
		}
		catch(...) {
			return FALSE;
		}
		GetDrawPt(pPt, nTotPt, pCadDC->m_pPlanInfo->nCurveDivide);
		BOOL	blPaint = FALSE;
		if (m_bDraw & DRAW_PAINT) {
			if (!pCadDC->m_pCamera->m_bPerspect){
				short	nIndex = (short)nTotPt;
				CCAD_Point	*pPaint = new CCAD_Point[nTotPt];
				memcpy(pPaint, pPt, sizeof(CCAD_Point)*nTotPt);
				LOGPEN oldPen = pCadDC->SelectNullPen();
				int nTriCount = 0;
				pCadDC->PolyPolygon(pPaint, &nIndex, 1, NULL, nTriCount);
				pCadDC->RestoreObject(oldPen);
				delete []pPaint;
				blPaint = TRUE;
			}
		}
		if ((m_bDraw & DRAW_FRAME) || !blPaint)
			FrameDraw(pCadDC, (short)nTotPt, pPt);
		else {
			LOGPEN oldPen = pCadDC->SelectNullPen();
			FrameDraw(pCadDC, (short)nTotPt, pPt);
			pCadDC->RestoreObject(oldPen);
		}
		delete [] pPt;
	}
	return TRUE;
}
//----------------------------------------------
BOOL CCAD_GemCurve::Get3SplinePt(SCAD_POINT &cPt, long step, CAD_FLOAT fLength, long i, long j) const
{
	if (fLength == 0)
		return FALSE;

	CAD_FLOAT	t;
	CAD_FLOAT	N22, N32;
	CAD_FLOAT	N13, N23, N33;
	CAD_FLOAT	N04, N14, N24, N34;
	CAD_FLOAT	numerator, denominator;

	t = m_pKnot[i] + j*fLength/step;

	N22 = (m_pKnot[i+1]-t) / (m_pKnot[i+1]-m_pKnot[i]);
	N32 = (t-m_pKnot[i] )  / (m_pKnot[i+1]-m_pKnot[i]);

	N13 = (m_pKnot[i+1]-t)*N22 / (m_pKnot[i+1]-m_pKnot[i-1]);
	N23 = (t-m_pKnot[i-1])*N22 / (m_pKnot[i+1]-m_pKnot[i-1]) +
		  (m_pKnot[i+2]-t)*N32 / (m_pKnot[i+2]-m_pKnot[i]);
	N33 = (t-m_pKnot[i])*N32   / (m_pKnot[i+2]-m_pKnot[i]);

	N04 = (m_pKnot[i+1]-t)*N13 / (m_pKnot[i+1]-m_pKnot[i-2]);
	N14 = (t-m_pKnot[i-2])*N13 / (m_pKnot[i+1]-m_pKnot[i-2]) +
		  (m_pKnot[i+2]-t)*N23 / (m_pKnot[i+2]-m_pKnot[i-1]);
	N24 = (t-m_pKnot[i-1])*N23 / (m_pKnot[i+2]-m_pKnot[i-1]) + 
		  (m_pKnot[i+3]-t)*N33 / (m_pKnot[i+3]-m_pKnot[i]);
	N34 = (t-m_pKnot[i])*N33   / (m_pKnot[i+3]-m_pKnot[i]);

	numerator = N04*m_pCurvePt[i-3].m_fX*m_pWeight[i-3] + N14*m_pCurvePt[i-2].m_fX*m_pWeight[i-2] +
				N24*m_pCurvePt[i-1].m_fX*m_pWeight[i-1] + N34*m_pCurvePt[i].m_fX*m_pWeight[i];
	denominator = N04*m_pWeight[i-3] + N14*m_pWeight[i-2] + N24*m_pWeight[i-1] + N34*m_pWeight[i];
	if (denominator == 0)
		return FALSE;
	cPt.m_fX = numerator/denominator;
	numerator = N04*m_pCurvePt[i-3].m_fY*m_pWeight[i-3] + N14*m_pCurvePt[i-2].m_fY*m_pWeight[i-2] +
				N24*m_pCurvePt[i-1].m_fY*m_pWeight[i-1] + N34*m_pCurvePt[i].m_fY*m_pWeight[i];
	cPt.m_fY = numerator/denominator;
	cPt.m_fZ = 0;
	return TRUE;
}

//----------------------------------------------
BOOL CCAD_GemCurve::Get2SplinePt(SCAD_POINT& cPt, long step, CAD_FLOAT fLength, long i, long j) const
{
	if (fLength == 0)
		return FALSE;

	CAD_FLOAT		t;
	CAD_FLOAT		N12, N22;
	CAD_FLOAT		N03, N13, N23;
	CAD_FLOAT		numerator, denominator;

	t = m_pKnot[i] + j*fLength/step;

	N12 = (m_pKnot[i+1]-t) / (m_pKnot[i+1]-m_pKnot[i]);
	N22 = (t-m_pKnot[i] )  / (m_pKnot[i+1]-m_pKnot[i]);

	N03 = (m_pKnot[i+1]-t)*N12 / (m_pKnot[i+1]-m_pKnot[i-1]);
	N13 = (t-m_pKnot[i-1])*N12 / (m_pKnot[i+1]-m_pKnot[i-1]) +
		  (m_pKnot[i+2]-t)*N22 / (m_pKnot[i+2]-m_pKnot[i]);
	N23 = (t-m_pKnot[i])*N22   / (m_pKnot[i+2]-m_pKnot[i]);

	denominator = N03*m_pWeight[i-2] + N13*m_pWeight[i-1] + N23*m_pWeight[i];
	if (denominator == 0)
		return FALSE;

	numerator   = N03*m_pWeight[i-2]*m_pCurvePt[i-2].m_fX + N13*m_pWeight[i-1]*m_pCurvePt[i-1].m_fX + N23*m_pWeight[i]*m_pCurvePt[i].m_fX;
	cPt.m_fX = numerator/denominator;
	numerator   = N03*m_pWeight[i-2]*m_pCurvePt[i-2].m_fY + N13*m_pWeight[i-1]*m_pCurvePt[i-1].m_fY + N23*m_pWeight[i]*m_pCurvePt[i].m_fY;
	cPt.m_fY = numerator/denominator;
	cPt.m_fZ = 0;
	return TRUE;
}

//----------------------------------------------
void CCAD_GemCurve::CountDrawPt( int & nTotPt, short nCurveDiv ) const
{
	if (m_bKind == CURVE_3SPLINE)
		nTotPt = (m_nCurveTot - 3)*nCurveDiv + 1;
	else
		nTotPt = (m_nCurveTot - 2)*nCurveDiv + 1;
}

//----------------------------------------------
void CCAD_GemCurve::GetDrawPt( SCAD_POINT * pPt, int & nTotPt, short nCurveDiv ) const
{
	long		step = nCurveDiv;
	CAD_FLOAT	fLength;
	long		i, j;
	short		nStart, nEnd;

	if (m_bKind == CURVE_3SPLINE)
		nStart = 3;
	else
		nStart = 2;
	nEnd = (short)(m_nCurveTot-1);
	
	nTotPt = 0;
	for ( i=nStart; i<=nEnd; i++ ) {
		fLength = m_pKnot[i+1] - m_pKnot[i];
		for ( j=0; j<step; j++ ) {
			if (m_bKind == CURVE_3SPLINE) {
				if( Get3SplinePt(pPt[nTotPt], step, fLength, i, j) )
					nTotPt++;
			}
			else {
				if ( Get2SplinePt(pPt[nTotPt], step, fLength, i, j) )
					nTotPt++;
			}
		}
	}
	pPt[nTotPt] = m_pCurvePt[m_nCurveTot-1];
	nTotPt++;
}


//----------------------------------------------------------
WORD CCAD_GemCurve::GetCurvePoint(SCAD_POINT *spDivPnt)
{
	int nTotPt = 0;
	CountDrawPt(nTotPt);
	
	if (spDivPnt != NULL)
	{
		GetDrawPt(spDivPnt, nTotPt);
		
		int	i;
		for (i = 0; i < nTotPt; i++)
			m_cBcs.ConvToBase(spDivPnt[i]);
	}
	return nTotPt;
}

void CCAD_GemCurve::CountBodyRange(CCAD_Rect& cRange)
{
	short nBorder;
	if (m_bKind == CURVE_3SPLINE)
		nBorder = 4;
	else
		nBorder = 3;

	if (m_nCurveTot < nBorder || m_pCurvePt == NULL)
		return;

	int	nTotPt;
	CCAD_Vector	* pPt = NULL;

	CountDrawPt( nTotPt );
	try {
		pPt = new CCAD_Vector[nTotPt];
	}
	catch(...) {
		return;
	}

	CCAD_GemThickEntity::CountBodyRange(cRange);

	GetDrawPt(pPt, nTotPt);
	int	k;
	for (k=0; k<nTotPt; k++)
		cRange.ResetParameter(pPt[k]);

	delete []pPt;
}

void CCAD_GemCurve::CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange)
{
	short nBorder;
	if (m_bKind == CURVE_3SPLINE)
		nBorder = 4;
	else
		nBorder = 3;

	if (m_nCurveTot < nBorder || m_pCurvePt == NULL)
		return;

	ASSERT(pCadDC != NULL);

	int	nTotPt;
	CCAD_Vector	* pPt = NULL;

	CountDrawPt( nTotPt, pCadDC->m_pPlanInfo->nCurveDivide );
	try {
		pPt = new CCAD_Vector[nTotPt];
	}
	catch(...) {
		return;
	}

	GetDrawPt(pPt, nTotPt, pCadDC->m_pPlanInfo->nCurveDivide);
	int	k;
	for (k=0; k<nTotPt; k++)
		pCadDC->CountVcsRange(pPt[k], cRange);

	delete []pPt;
}

//-----------------------------------------------------
BOOL	CCAD_GemCurve::PtDirect(const CCAD_Vector & cPt0)
{
	long			step  = 5;
	CAD_FLOAT		lenth;
	long			i, j;
	BOOL			rtn;
	CCAD_Vector		cPt;
	CCAD_GemLine	cLine, cLine0;
	CAD_FLOAT		dTmp, dL = MAX_DOUBLE;
	short			nStart, nEnd;

	if (m_bKind == CURVE_3SPLINE){
		nStart = 3;
		nEnd = (short)(m_nCurveTot+1);
	}
	else{
		nStart = 2;
		nEnd = (short)(m_nCurveTot-1);
	}
	for ( i=nStart; i<=nEnd; i++ ) {
		lenth = m_pKnot[i+1] - m_pKnot[i];
		for ( j=0; j<=step; j++ ) {
			if (m_bKind == CURVE_3SPLINE)
				rtn = Get3SplinePt(cPt, step, lenth, i, j);
			else
				rtn = Get2SplinePt(cPt, step, lenth, i, j);
			if ( rtn ) {
				if ( j == 0 && i == nStart)
					cLine.m_cSp = cPt;
				else {
					cLine.m_cEp = cPt;
					CCAD_Angle	cAngle;
					cLine.m_cSp.CountAngle2D(cLine.m_cEp, cAngle);
					cAngle.RotateAngle(NINETY);
					if ( cLine.CrossTwoLine(cAngle, cPt0) == LINES_CROSS ) {
						dTmp = cLine.CountDistanceToPoint(cPt0);
						if ( dL > dTmp ) {
							cLine0 = cLine;
							dL = dTmp;
						}
					}
					cLine.m_cSp = cLine.m_cEp;
				}
			}
		}
	}
	return cLine0.PtDirect(cPt0);
}
