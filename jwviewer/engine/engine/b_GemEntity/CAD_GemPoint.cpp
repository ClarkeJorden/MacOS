#include "stdafx.h"
#include "CAD_GemPoint.h"



#define	PTNUM	36

//-------------------------------------------
CCAD_GemPoint::CCAD_GemPoint()
{
	m_fR = 0;
	m_pTriPoints = NULL;
	m_nTriCount = 0;
}

//-------------------------------------------
CCAD_GemPoint::~CCAD_GemPoint()
{
	if(m_pTriPoints)
		delete[] m_pTriPoints;
}

//-------------------------------------------
CCAD_GemPoint::CCAD_GemPoint(const CCAD_GemPoint & cPoint)
{
	*this = cPoint;
	m_pTriPoints = NULL;
	m_nTriCount = 0;
}

//-------------------------------------------
CCAD_GemPoint::CCAD_GemPoint(const CCAD_Coord3D &cBcs, const BYTE bType, const CCAD_Vector & cPt, const CAD_FLOAT & fR)
{
	m_cBcs = cBcs;
	m_bType = bType;
	m_cPt = cPt;
	m_fR = fR;
	m_pTriPoints = NULL;
	m_nTriCount = 0;
}

//-------------------------------------------
CCAD_GemPoint & CCAD_GemPoint::operator=(const CCAD_GemPoint & cPoint)
{
	CCAD_Gem2DEntity::operator =(cPoint);
	m_bType = cPoint.m_bType;
	m_cPt = cPoint.m_cPt;
	m_fR = cPoint.m_fR;
	return *this;
}

void CCAD_GemPoint::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_Gem2DEntity::MemCopyTo(pGemEntity, pData1, pData2, pData3);
	*(CCAD_GemPoint *)pGemEntity = *this;
}
//-------------------------------------------
BOOL CCAD_GemPoint::Read(CCAD_File & cFile)
{
	CCAD_Gem2DEntity::Read(cFile);
	cFile.ReadByte(&m_bType);
	cFile.ReadCAD_Float(&m_fR);
	m_cPt.Read(cFile);
	return TRUE;
}

//-------------------------------------------
void CCAD_GemPoint::SetType(BYTE & bType) const
{
	int res, q;

	q = (int)m_bType / 5;
	res = (int)m_bType % 5;
	switch(res) {
	case 0:		bType |= POINT_PAINT;	break;
	case 1:		bType |= POINT_CROSS1;	break;
	case 2:		bType |= POINT_CROSS2;	break;
	case 3:		bType |= POINT_XCROSS;	break;
	case 4:		bType |= POINT_BAR;		break;
	}
	switch(q) {
	case 1:		bType |= POINT_CIRCLE;	break;
	case 2:		bType |= POINT_RECT;	break;
	case 3:		bType |= POINT_CIRCLE;
				bType |= POINT_RECT;	break;
	}
}

//-------------------------------------------
void CCAD_GemPoint::DrawLittle(CCAD_DC *pDC) const
{
	pDC->MoveTo(m_cPt);
	pDC->LineTo(m_cPt);
}

//-------------------------------------------
BOOL CCAD_GemPoint::Draw(CCAD_DC *pDC, BOOL blRedraw)
{
	if (m_fR <= 0)
		return FALSE;

	long	r = pDC->ConvScalarWorldToScrn(m_fR);
	if (r <= 1){
		DrawLittle(pDC);
		return TRUE;
	}

    float rad = m_fR;
    if (r > 3) { //JW
        rad = pDC->ConvScalarScrnToWorld(3);
    }
    CCAD_GemCircle	cCircle(m_cBcs, m_cPt, rad, DRAW_FRAME);
    
	switch (m_bType) {
	case 0:		//cCircle.m_fR /= 8;
//				DrawPaint(pDC, cCircle);
				cCircle.m_bDraw = DRAW_PAINT;
				cCircle.Draw(pDC, blRedraw);
				break;
	case 1:		cCircle.m_fR /= 4;
				DrawCross(pDC, cCircle);
				break;
	case 2:		cCircle.m_fR /= 2;
				DrawCross(pDC, cCircle);
				break;
	case 3:		cCircle.m_fR /= 2;
				DrawXcross(pDC, cCircle);
				break;
	case 4:		cCircle.m_fR /= 2;
				DrawBar(pDC, cCircle);
				break;
	case 5:		//cCircle.m_fR /= 2;
				cCircle.Draw(pDC, blRedraw);
				break;
	case 6:		cCircle.m_fR /= 2;
//				DrawCircle(pDC, cCircle);
				cCircle.Draw(pDC, blRedraw);
				cCircle.m_fR /= 2;
				DrawCross(pDC, cCircle);
				break;
	case 7:		cCircle.m_fR /= 2;
//				DrawCircle(pDC, cCircle);
				cCircle.Draw(pDC, blRedraw);
				DrawCross(pDC, cCircle);
				break;
	case 8:		cCircle.m_fR /= 2;
//				DrawCircle(pDC, cCircle);
				cCircle.Draw(pDC, blRedraw);
				DrawXcross(pDC, cCircle);
				break;
	case 9:		cCircle.m_fR /= 2;
//				DrawCircle(pDC, cCircle);
				cCircle.Draw(pDC, blRedraw);
				DrawBar(pDC, cCircle);
				break;
	case 10:	cCircle.m_fR /= 2;
				DrawRect(pDC, cCircle);
				cCircle.m_fR /= 4;
//				DrawPaint(pDC, cCircle);
				cCircle.m_bDraw = DRAW_PAINT;
				cCircle.Draw(pDC, blRedraw);
				break;
	case 11:	cCircle.m_fR /= 2;
				DrawRect(pDC, cCircle);
				cCircle.m_fR /= 2;
				DrawCross(pDC, cCircle);
				break;
	case 12:	cCircle.m_fR /= 2;
				DrawRect(pDC, cCircle);
				DrawCross(pDC, cCircle);
				break;
	case 13:	cCircle.m_fR /= 2;
				DrawRect(pDC, cCircle);
				DrawXcross(pDC, cCircle);
				break;
	case 14:	cCircle.m_fR /= 2;
				DrawRect(pDC, cCircle);
				DrawBar(pDC, cCircle);
				break;
	case 15:	cCircle.m_fR /= 2;
//				DrawCircle(pDC, cCircle);
				cCircle.Draw(pDC, blRedraw);
				DrawRect(pDC, cCircle);
				cCircle.m_fR /= 4;
//				DrawPaint(pDC, cCircle);
				cCircle.m_bDraw = DRAW_PAINT;
				cCircle.Draw(pDC, blRedraw);
				break;
	case 16:	cCircle.m_fR /= 2;
//				DrawCircle(pDC, cCircle);
				cCircle.Draw(pDC, blRedraw);
				DrawRect(pDC, cCircle);
				cCircle.m_fR /= 2;
				DrawCross(pDC, cCircle);
				break;
	case 17:	cCircle.m_fR /= 2;
//				DrawCircle(pDC, cCircle);
				cCircle.Draw(pDC, blRedraw);
				DrawRect(pDC, cCircle);
				DrawCross(pDC, cCircle);
				break;
	case 18:	cCircle.m_fR /= 2;
//				DrawCircle(pDC, cCircle);
				cCircle.Draw(pDC, blRedraw);
				DrawRect(pDC, cCircle);
				DrawXcross(pDC, cCircle);
				break;
	case 19:	cCircle.m_fR /= 2;
//				DrawCircle(pDC, cCircle);
				cCircle.Draw(pDC, blRedraw);
				DrawRect(pDC, cCircle);
				DrawBar(pDC, cCircle);
				break;
	}
	return TRUE;
}

//-------------------------------------------
void CCAD_GemPoint::DrawRect(CCAD_DC *pDC, const CCAD_GemCircle & cCircle) const
{
	pDC->Lines_Begin(LINETYPE_STRIP);
	CCAD_Point	cPt(cCircle.m_cCp);
	cPt.m_fX = cCircle.m_cCp.m_fX - cCircle.m_fR; cPt.m_fY = cCircle.m_cCp.m_fY + cCircle.m_fR;
	pDC->MoveTo(cPt);
	cPt.m_fX = cCircle.m_cCp.m_fX + cCircle.m_fR; cPt.m_fY = cCircle.m_cCp.m_fY + cCircle.m_fR;
	pDC->LineTo(cPt);
	cPt.m_fX = cCircle.m_cCp.m_fX + cCircle.m_fR; cPt.m_fY = cCircle.m_cCp.m_fY - cCircle.m_fR;
	pDC->LineTo(cPt);
	cPt.m_fX = cCircle.m_cCp.m_fX - cCircle.m_fR; cPt.m_fY = cCircle.m_cCp.m_fY - cCircle.m_fR;
	pDC->LineTo(cPt);
	cPt.m_fX = cCircle.m_cCp.m_fX - cCircle.m_fR; cPt.m_fY = cCircle.m_cCp.m_fY + cCircle.m_fR;
	pDC->LineTo(cPt);
	pDC->Lines_End();
}

//-------------------------------------------
void CCAD_GemPoint::CountRectVcsRange(CCAD_DC *pDC, CCAD_Rect &cRange, const CCAD_GemCircle &cCircle) const
{
	CCAD_Point	cPt(cCircle.m_cCp);
	cPt.m_fX = cCircle.m_cCp.m_fX - cCircle.m_fR; cPt.m_fY = cCircle.m_cCp.m_fY + cCircle.m_fR;
	pDC->CountVcsRange(cPt, cRange);
	cPt.m_fX = cCircle.m_cCp.m_fX + cCircle.m_fR; cPt.m_fY = cCircle.m_cCp.m_fY + cCircle.m_fR;
	pDC->CountVcsRange(cPt, cRange);
	cPt.m_fX = cCircle.m_cCp.m_fX + cCircle.m_fR; cPt.m_fY = cCircle.m_cCp.m_fY - cCircle.m_fR;
	pDC->CountVcsRange(cPt, cRange);
	cPt.m_fX = cCircle.m_cCp.m_fX - cCircle.m_fR; cPt.m_fY = cCircle.m_cCp.m_fY - cCircle.m_fR;
	pDC->CountVcsRange(cPt, cRange);
}

//-------------------------------------------
void CCAD_GemPoint::DrawCross(CCAD_DC *pDC, const CCAD_GemCircle & cCircle) const
{
	pDC->Lines_Begin(LINETYPE_LINES);
	CCAD_Point	cPt(cCircle.m_cCp);
	cPt.m_fX = cCircle.m_cCp.m_fX - cCircle.m_fR; cPt.m_fY = cCircle.m_cCp.m_fY;
	pDC->MoveTo(cPt);
	cPt.m_fX = cCircle.m_cCp.m_fX + cCircle.m_fR; cPt.m_fY = cCircle.m_cCp.m_fY;
	pDC->LineTo(cPt);
	cPt.m_fX = cCircle.m_cCp.m_fX; cPt.m_fY = cCircle.m_cCp.m_fY - cCircle.m_fR;
	pDC->MoveTo(cPt);
	cPt.m_fX = cCircle.m_cCp.m_fX; cPt.m_fY = cCircle.m_cCp.m_fY + cCircle.m_fR;
	pDC->LineTo(cPt);
	pDC->Lines_End();
}

//-------------------------------------------
void CCAD_GemPoint::DrawXcross(CCAD_DC *pDC, const CCAD_GemCircle & cCircle) const
{
	pDC->Lines_Begin(LINETYPE_LINES);
	CCAD_Point	cPt(cCircle.m_cCp);
	cPt.m_fX = cCircle.m_cCp.m_fX - cCircle.m_fR; cPt.m_fY = cCircle.m_cCp.m_fY + cCircle.m_fR;
	pDC->MoveTo(cPt);
	cPt.m_fX = cCircle.m_cCp.m_fX + cCircle.m_fR; cPt.m_fY = cCircle.m_cCp.m_fY - cCircle.m_fR;
	pDC->LineTo(cPt);
	cPt.m_fX = cCircle.m_cCp.m_fX - cCircle.m_fR; cPt.m_fY = cCircle.m_cCp.m_fY - cCircle.m_fR;
	pDC->MoveTo(cPt);
	cPt.m_fX = cCircle.m_cCp.m_fX + cCircle.m_fR; cPt.m_fY = cCircle.m_cCp.m_fY + cCircle.m_fR;
	pDC->LineTo(cPt);
	pDC->Lines_End();
}

//-------------------------------------------
void CCAD_GemPoint::DrawBar(CCAD_DC *pDC, const CCAD_GemCircle & cCircle) const
{
	CCAD_Point	cPt(cCircle.m_cCp);
	pDC->MoveTo(cPt);
	cPt.m_fY = cCircle.m_cCp.m_fY + cCircle.m_fR;
	pDC->LineTo(cPt);
}

//-------------------------------------------
void CCAD_GemPoint::CountBarVcsRange(CCAD_DC *pDC, CCAD_Rect &cRange, const CCAD_GemCircle & cCircle) const
{
	CCAD_Point	cPt(cCircle.m_cCp);
	pDC->CountVcsRange(cPt, cRange);
	cPt.m_fY = cCircle.m_cCp.m_fY + cCircle.m_fR;
	pDC->CountVcsRange(cPt, cRange);
}

//-------------------------------------------
void CCAD_GemPoint::DrawCircle(CCAD_DC *pDC, const CCAD_GemCircle & cCircle) const
{
	CCAD_Vector	cSp(cCircle.m_cCp.m_fX+cCircle.m_fR, cCircle.m_cCp.m_fY, 0);
	CCAD_Vector	cEp;
	short n = PTNUM;
	CCAD_Angle	cAlfa = (short)0;
	CCAD_Angle	cDelta = (short)(LONGDOUBLE_NINETY / n);

	pDC->Lines_Begin(LINETYPE_STRIP);
	pDC->MoveTo(cSp);
	int	i;
	for (i=0; i<n; i++){
		cAlfa.RotateAngle(cDelta);
		cCircle.CountCirclePt(cEp, cAlfa.sin(), cAlfa.cos());
		pDC->LineTo(cEp);
	}
	pDC->LineTo(cSp);
	pDC->Lines_End();
}

//-------------------------------------------
void CCAD_GemPoint::DrawPaint(CCAD_DC *pDC, const CCAD_GemCircle & cCircle, BOOL blPaint) 
{
	CCAD_Point	cPt[PTNUM];
	short		nIndex = PTNUM;
	short		n = nIndex;
	CCAD_Angle	cAlfa = (short)0;
	CCAD_Angle	cDelta = (short)(LONGDOUBLE_NINETY / n);
	int	i;
	for (i=0; i<n; i++){
		cCircle.CountCirclePt(cPt[i], cAlfa.sin(), cAlfa.cos());
		cAlfa.RotateAngle(cDelta);
	}
	if (blPaint){
		LOGPEN oldPen = pDC->SelectNullPen();
		pDC->Lines_Begin(LINETYPE_STRIP);
		pDC->MoveTo(cPt[0]);
		for (i=1; i<n; i++)
			pDC->LineTo(cPt[i]);
		pDC->LineTo(cPt[0]);
		pDC->Lines_End();

		if(m_nTriCount == 0)
			m_pTriPoints = pDC->GetTrianglesFromPolygon(cPt, &nIndex, 1, m_nTriCount);
		pDC->PolyPolygon(cPt, &nIndex, 1, m_pTriPoints, m_nTriCount);
		pDC->RestoreObject(oldPen);
	}
	else {
		pDC->Lines_Begin(LINETYPE_STRIP);
		pDC->MoveTo(cPt[0]);
		for (i=1; i<n; i++)
			pDC->LineTo(cPt[i]);
		pDC->LineTo(cPt[0]);
		pDC->Lines_End();
	}
}

void CCAD_GemPoint::CountBodyRange(CCAD_Rect& cRange)
{
	if (m_fR <= 0)
		return;

	CCAD_GemCircle	cCircle(m_cBcs, m_cPt, m_fR, DRAW_FRAME);

	if (m_bType == 0)
		cCircle.m_fR /= 8;
	else if (m_bType == 1)
		cCircle.m_fR /= 4;
	else
		cCircle.m_fR /= 2;
	cCircle.CountBodyRange(cRange);
}

void CCAD_GemPoint::CountVcsRange(CCAD_DC *pDC, CCAD_Rect& cRange)
{
	if (m_fR <= 0)
		return;
	ASSERT(pDC != NULL);

	CCAD_GemCircle	cCircle(m_cBcs, m_cPt, m_fR, DRAW_FRAME);

	if (m_bType == 0)
		cCircle.m_fR /= 8;
	else if (m_bType == 1)
		cCircle.m_fR /= 4;
	else
		cCircle.m_fR /= 2;
	if (m_bType == 4)
		CountBarVcsRange(pDC, cRange, cCircle);
	else
		CountRectVcsRange(pDC, cRange, cCircle);
}
