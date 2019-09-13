#include "stdafx.h"
#include "CAD_Global.h"
#include "CAD_GemPline2D.h"

CCAD_GemPvertex::CCAD_GemPvertex()
{
	m_cBulge = (short)0;
}

//----------------------------------------------
CCAD_GemPvertex & CCAD_GemPvertex::operator=(const CCAD_GemPvertex & cVt)
{
	m_cPt = cVt.m_cPt;
	m_cBulge = cVt.m_cBulge;
	return *this;
}

//---------------------------------------------
void CCAD_GemPvertex::SetBulge(const CCAD_Vector & cSp, const CCAD_Vector & /*cEp*/)
{
	m_cPt = cSp;
	m_cBulge = (short)0;
}

//---------------------------------------------
void CCAD_GemPvertex::SetBulge(const CCAD_Vector & cSp, const CCAD_GemArc & cArc)
{
	m_cPt = cSp;
	m_cBulge =	cArc.m_cDelta;			//tan(cArc.m_dDelta/4);
}

//---------------------------------------------
BOOL CCAD_GemPvertex::GetArc(CCAD_GemArc & cArc, const CCAD_Vector & cNp) const
{
	if (m_cBulge.GetAbsDegree() < CCAD_Global::GetAngleAccy())
		return FALSE;
	cArc.m_cDelta =	m_cBulge;				//atan(m_dBulge) * 4;
	CAD_FLOAT fLength = m_cPt.CountLength(cNp) / 2;
	CCAD_Angle	cAngle = cArc.m_cDelta;
	cAngle /= 2;
	cArc.m_fR = fLength / (CAD_FLOAT)fabs(cAngle.sin());
	m_cPt.CountAngle2D(cNp, cAngle);
	cArc.m_cCp.m_fX = m_cPt.m_fX + cArc.m_fR * cAngle.cos();
	cArc.m_cCp.m_fY = m_cPt.m_fY + cArc.m_fR * cAngle.sin();

	CAD_FLOAT	fDelta = cArc.m_cDelta.GetDegree();
	CAD_FLOAT	fAlfa;
	if (fDelta< 0)
		fAlfa = -(DOUBLE_NINETY+fDelta)/2;
	else
		fAlfa = (DOUBLE_NINETY-fDelta)/2;
	cArc.m_cAlfa.SetFromDegree(fAlfa);
	cArc.m_cCp.Rotate(m_cPt, cArc.m_cAlfa.sin(), cArc.m_cAlfa.cos());
	cArc.m_cCp.CountAngle2D(m_cPt, cArc.m_cAlfa);
	return TRUE;
}

//---------------------------------------------
BOOL CCAD_GemPvertex::Draw(CCAD_GemThickEntity *pThickEntity, const CCAD_Vector & cPt, CCAD_DC *pDC, BOOL blRedraw)
{
	CCAD_GemArc	cArc;
	if (!GetArc(cArc, cPt)) {
		CCAD_GemLine	cLine;
		cLine.m_cBcs = pThickEntity->m_cBcs;
		cLine.m_cThick = pThickEntity->m_cThick;
		cLine.m_cSp = m_cPt;
		cLine.m_cEp = cPt;
		return cLine.Draw(pDC, blRedraw);
	}
	cArc.m_cBcs = pThickEntity->m_cBcs;
	cArc.m_cThick = pThickEntity->m_cThick;
	return cArc.Draw(pDC, blRedraw);
}

void CCAD_GemPvertex::CountBodyRange(CCAD_GemThickEntity *pThickEntity, const CCAD_Vector & cPt, CCAD_Rect &cRange)
{
	CCAD_GemArc	cArc;
	if (!GetArc(cArc, cPt)) {
		CCAD_GemLine	cLine;
		cLine.m_cBcs = pThickEntity->m_cBcs;
		cLine.m_cThick = pThickEntity->m_cThick;
		cLine.m_cSp = m_cPt;
		cLine.m_cEp = cPt;
		cLine.CountBodyRange(cRange);
	}
	else {
		cArc.m_cBcs = pThickEntity->m_cBcs;
		cArc.m_cThick = pThickEntity->m_cThick;
		cArc.CountBodyRange(cRange);
	}
}

void CCAD_GemPvertex::CountVcsRange(CCAD_DC *pCadDC, CCAD_GemThickEntity *pThickEntity, const CCAD_Vector & cPt, CCAD_Rect &cRange)
{
	CCAD_GemArc	cArc;
	if (!GetArc(cArc, cPt)) {
		CCAD_GemLine	cLine;
		cLine.m_cBcs = pThickEntity->m_cBcs;
		cLine.m_cThick = pThickEntity->m_cThick;
		cLine.m_cSp = m_cPt;
		cLine.m_cEp = cPt;
		cLine.CountVcsRange(pCadDC, cRange);
	}
	else {
		cArc.m_cBcs = pThickEntity->m_cBcs;
		cArc.m_cThick = pThickEntity->m_cThick;
		cArc.CountVcsRange(pCadDC, cRange);
	}
}

//---------------------------------------------
WORD CCAD_GemPvertex::GetCurvePoint(CCAD_GemThickEntity *pThickEntity, const CCAD_Vector & cPt, SCAD_POINT *spDivPnt)
{
	CCAD_GemArc	cArc;
	if (!GetArc(cArc, cPt))
	{
		if (spDivPnt != NULL)
		{
			spDivPnt[0] = m_cPt;
			pThickEntity->m_cBcs.ConvToBase(spDivPnt[0]);
		}
		return 1;
	}
	cArc.m_cBcs = pThickEntity->m_cBcs;
	cArc.m_cThick = pThickEntity->m_cThick;
	return cArc.GetCurvePoint(spDivPnt) - 1;
}

//---------------------------------------------
void CCAD_GemPvertex::Read(CCAD_File & cFile)
{
}



/////////////////////////////////////////////////////////////////////////////
// CCAD_GemPline2D
//
CCAD_GemPline2D::CCAD_GemPline2D()
{
	Init();
}

//---------------------------------------------
CCAD_GemPline2D::~CCAD_GemPline2D()
{
	RemoveAll();
}

void CCAD_GemPline2D::Init()
{
	m_bClose = NO;
	m_nTot = 0;
	m_pVertex = NULL;
}

//---------------------------------------------
void CCAD_GemPline2D::RemoveAll()
{
	if (m_pVertex != NULL)
		delete [] m_pVertex;
	m_pVertex = NULL;
	m_nTot = 0;
}

void CCAD_GemPline2D::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_GemThickEntity::MemCopyTo(pGemEntity, pData1, pData2, pData3);

	((CCAD_GemPline2D *)pGemEntity)->m_bClose = m_bClose;
	((CCAD_GemPline2D *)pGemEntity)->m_nTot = m_nTot;
	if (pData1 == NULL)
		pData1 = (LPSTR)new CCAD_GemPvertex[m_nTot];
	((CCAD_GemPline2D *)pGemEntity)->m_pVertex = (CCAD_GemPvertex *)pData1;
	int	i;
	for (i=0; i<m_nTot; i++)
		*(((CCAD_GemPline2D *)pGemEntity)->m_pVertex + i) = *(m_pVertex + i);
}

void CCAD_GemPline2D::Remove()
{
	RemoveAll();
}

//---------------------------------------------
BOOL CCAD_GemPline2D::Read(CCAD_File & cFile)
{
	CCAD_GemThickEntity::Read(cFile);
	cFile.ReadByte(&m_bClose);
	cFile.ReadShort(&m_nTot);
	if (m_nTot > 0) {
		try {
			m_pVertex = new CCAD_GemPvertex[m_nTot];
		}
		catch(...) {
			return FALSE;
		}
		int	i;
		for (i=0; i<m_nTot; i++)
			(m_pVertex+i)->Read(cFile);
	}
	return TRUE;
}

//--------------------------------------
BOOL CCAD_GemPline2D::IsClosed()
{
	if (m_bClose == YES)
		return TRUE;
	return FALSE;
}

//---------------------------------------------
BOOL CCAD_GemPline2D::Draw(CCAD_DC *pDC, BOOL blRedraw)
{
	if (m_pVertex == NULL || m_nTot < 2)
		return FALSE;
	short	i, j;
	for (i=0; i<m_nTot; i++) {
		j = (short)(i+1);
		if (j == m_nTot) {
			if (m_bClose == NO)
				break;
			j = 0;
		}
		(m_pVertex+i)->Draw((CCAD_GemThickEntity *)this, (m_pVertex+j)->m_cPt, pDC, blRedraw);
	}
	return TRUE;
}

void CCAD_GemPline2D::CountBodyRange(CCAD_Rect &cRange)
{
	if (m_pVertex == NULL || m_nTot < 2)
		return;

	CCAD_GemThickEntity::CountBodyRange(cRange);
	short	i, j;
	for (i=0; i<m_nTot; i++) {
		j = (short)(i+1);
		if (j == m_nTot) {
			if (m_bClose == NO)
				break;
			j = 0;
		}
		(m_pVertex+i)->CountBodyRange((CCAD_GemThickEntity *)this, (m_pVertex+j)->m_cPt, cRange);
	}
}

void CCAD_GemPline2D::CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect &cRange)
{
	if (m_pVertex == NULL || m_nTot < 2)
		return;

	short	i, j;
	for (i=0; i<m_nTot; i++) {
		j = (short)(i+1);
		if (j == m_nTot) {
			if (m_bClose == NO)
				break;
			j = 0;
		}
		(m_pVertex+i)->CountVcsRange(pCadDC, (CCAD_GemThickEntity *)this, (m_pVertex+j)->m_cPt, cRange);
	}
}
//----------------------------------------------------------
//----------------------------------------------------------
WORD CCAD_GemPline2D::GetCurvePoint(SCAD_POINT *spDivPnt)
{
	WORD wTot = 0;
	if (spDivPnt == NULL)
	{
		int	i;
		for (i=0; i<m_nTot; i++)
		{
			short j = i + 1;
			if (j == m_nTot)
			{
				if (m_bClose == NO)
					wTot += 1;
				else
				{
					j = 0;
					wTot += (m_pVertex+i)->GetCurvePoint(this, (m_pVertex+j)->m_cPt);
				}
			}
			else
				wTot += (m_pVertex+i)->GetCurvePoint(this, (m_pVertex+j)->m_cPt);
		}
		return wTot;
	}
	
	int	i;
	for (i=0; i<m_nTot; i++)
	{
		short j = i + 1;
		WORD wDivNum = 0;
		if (j == m_nTot)
		{
			if (m_bClose == NO)
			{
				wDivNum = 1;
				spDivPnt[wTot] = (m_pVertex+i)->m_cPt;
				m_cBcs.ConvToBase(spDivPnt[wTot]);
			}
			else
			{
				j = 0;
				wDivNum = (m_pVertex+i)->GetCurvePoint(this, (m_pVertex+j)->m_cPt, &spDivPnt[wTot]);
			}
		}
		else
			wDivNum = (m_pVertex+i)->GetCurvePoint(this, (m_pVertex+j)->m_cPt, &spDivPnt[wTot]);
		wTot += wDivNum;
	}
	return wTot;
}
