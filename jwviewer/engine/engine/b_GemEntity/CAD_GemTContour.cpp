#include "stdafx.h"
#include "math.h"
#include "CAD_GemTContour.h"

CCAD_GemTContour::CCAD_GemTContour()
{
	m_blInFlag = FALSE;
	m_blParent = TRUE;

	if(m_cChildArray.GetSize() != 0) {
		m_cChildArray.RemoveAll();
	}

	if(m_cPointArray.GetSize() != 0) {
		m_cPointArray.RemoveAll();
	}
}

CCAD_GemTContour::CCAD_GemTContour(const CCAD_GemTContour &cOther)
{
	m_blInFlag = cOther.m_blInFlag;
	m_blParent = cOther.m_blParent;
	
	m_cXMax = cOther.m_cXMax;
		
	int nSizeArray = cOther.m_cChildArray.GetSize();
	int	i;
	for(i = 0; i < nSizeArray; i ++) {
		m_cChildArray.Add(cOther.m_cChildArray[i]);
	}
	
	nSizeArray = cOther.m_cPointArray.GetSize();
	for(i = 0; i < nSizeArray; i ++) {
		m_cPointArray.Add(cOther.m_cPointArray[i]);
	}
}

CCAD_GemTContour::~CCAD_GemTContour()
{
	if(m_cChildArray.GetSize() != 0) {
		m_cChildArray.RemoveAll();
	}

	if(m_cPointArray.GetSize() != 0) {
		m_cPointArray.RemoveAll();
	}
}

void
CCAD_GemTContour::AddPoint(const	CCAD_Point	&cPt)
{
	int nSizeArray = m_cPointArray.GetSize();
	if (nSizeArray > 0) {
		if(	m_cPointArray[nSizeArray - 1].Equal(cPt) ||
			m_cPointArray[0].Equal(cPt)) {
			return;
		}
	}

	m_cPointArray.Add(cPt);
}

CCAD_GemTContour
&CCAD_GemTContour::operator= (const	CCAD_GemTContour	&cOther)
{
	m_blInFlag = cOther.m_blInFlag;
	m_blParent = cOther.m_blParent;

	m_cXMax = cOther.m_cXMax;
		
	int nSizeArray = cOther.m_cChildArray.GetSize();
	int	i;
	for(i = 0; i < nSizeArray; i ++) {
		m_cChildArray.Add(cOther.m_cChildArray[i]);
	}
	
	nSizeArray = cOther.m_cPointArray.GetSize();
	for(i = 0; i < nSizeArray; i ++) {
		m_cPointArray.Add(cOther.m_cPointArray[i]);
	}

	return *this;
}

void
CCAD_GemTContour::GetXMaxPoint(void)
{
	int nSizeArray = m_cPointArray.GetSize();
	
	m_cXMax = m_cPointArray[0];
	float fXMax = m_cXMax.m_fX;
	int	i;
	for(i = 1; i < nSizeArray; i ++) {
		if(m_cPointArray[i].m_fX > fXMax) {
			m_cXMax = m_cPointArray[i];
			fXMax = m_cXMax.m_fX;
		}
	}
}

void
CCAD_GemTContour::AddChild(short nID)
{
	m_cChildArray.Add(nID);
}

void
CCAD_GemTContour::InvertContourDir(void)
{
	int nSize = m_cPointArray.GetSize();
	int nCount = nSize / 2;
	for(int i = 0; i < nCount; i ++)
	{
		CCAD_Point cPt = m_cPointArray[i];
		m_cPointArray[i] = m_cPointArray[nSize-i-1];
		m_cPointArray[nSize-i-1] = cPt;
	}
}

#define		DIR_LINE	0
#define		DIR_PLUS	1
#define		DIR_MINUS	2

static short nGetDirOfTwoVect(const CCAD_Point &cVect1, const CCAD_Point &cVect2)
{
	double dValue = cVect1.m_fX * cVect2.m_fY -	cVect2.m_fX * cVect1.m_fY;
	
	if (IS_ZERO(dValue))
		return  DIR_LINE;
	if (dValue > 0)
		return DIR_PLUS;
	return DIR_MINUS;
}

BOOL
CCAD_GemTContour::blJudgRotDir ()

{
	/*
	WORD		wIndex, wNumPoint, wMin, wTem,
				wIdxTem[6];
	float		fValue[6];
		
	BOOL blRet = FALSE;
	wIdxTem[1] = wIdxTem[2] = wIdxTem[3] = wIdxTem[4] = 0;
	wNumPoint = m_cPointArray.GetSize();
	
	CCAD_Point	cTemPt = m_cPointArray[0];
	fValue[1] = fValue[3] = cTemPt.m_fX;
	fValue[2] = fValue[4] = cTemPt.m_fY;
	
	for ( wIndex = 1; wIndex < wNumPoint; wIndex ++ ) {
		cTemPt = m_cPointArray[wIndex];
		if ( fValue[1] > cTemPt.m_fX )
			fValue[1] = cTemPt.m_fX, wIdxTem[1] = wIndex;	// X min
		else	if ( fValue[3] < cTemPt.m_fX )
				fValue[3] = cTemPt.m_fX, wIdxTem[3] = wIndex;	// X max
		
		if ( fValue[2] > cTemPt.m_fY )
			fValue[2] = cTemPt.m_fY, wIdxTem[2] = wIndex;	// Y min
		else	if ( fValue[4] < cTemPt.m_fY )
				fValue[4] = cTemPt.m_fY, wIdxTem[4] = wIndex;	// Y max
	}
	
	wIdxTem[0] = wIdxTem[4], wIdxTem[5] = wIdxTem[1];
	wTem = 1, wMin = wIdxTem[1];
	for ( wIndex = 2; wIndex < 5; wIndex ++ ) {
		if ( wMin > wIdxTem[wIndex] )
			wMin = wIdxTem[wIndex], wTem = wIndex;
	}
	
	if ( wIdxTem[wTem - 1] < wIdxTem[wTem + 1] ) blRet = TRUE;
	else	blRet = FALSE;
	
	return blRet;
	*/
	int			nCount = m_cPointArray.GetSize();
	double		dAngleTot = 0.0, dAngle[2], dAngleFirst;
	CCAD_Point	cVect[2], cVectFirst;
	CCAD_Point	cPrev = m_cPointArray[nCount - 1];
	CCAD_Point	cNext = m_cPointArray[0];

	cVectFirst = CCAD_Vector(cPrev, cNext);
	cVect[1] = cVectFirst;
	dAngleFirst = dAngle[1] = ::atan2(cVect[1].m_fY, cVect[1].m_fX);
	
	cPrev = cNext;
	for (int i = 0; i < nCount; i++)
	{
		cVect[0] = cVect[1];
		dAngle[0] = dAngle[1];
		
		if (i == nCount - 1)
			cNext = m_cPointArray[0];
		else
			cNext = m_cPointArray[i+1];
		cVect[1] = CCAD_Vector(cPrev, cNext);
		dAngle[1] = ::atan2(cVect[1].m_fY, cVect[1].m_fX);
		
		short nDir = nGetDirOfTwoVect(cVect[0], cVect[1]);
		if (nDir == DIR_LINE)
			continue;
		
		dAngleTot += (dAngle[1] - dAngle[0]);
		if (nDir == DIR_PLUS && dAngle[1] < dAngle[0])
			dAngleTot += 2 * PAI;
		else if (nDir == DIR_MINUS && dAngle[1] > dAngle[0])
			dAngleTot -= 2 * PAI;
		
		cVect[0] = cVect[1];
		dAngle[0] = dAngle[1];
		cPrev = cNext;
	}
	return (dAngleTot > 0);
}


