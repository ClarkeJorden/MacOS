//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CAD_DimStyle.h"
#include "resource.h"

 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCAD_DimStyle::CCAD_DimStyle()
{
	Init();
}

CCAD_DimStyle::CCAD_DimStyle(const CCAD_DimStyle *pStyle)
{
	CopyFrom(pStyle);
}

CCAD_DimStyle::~CCAD_DimStyle()
{

}

CCAD_DimStyle & CCAD_DimStyle::operator =(const CCAD_DimStyle & cStyle)
{
	CopyFrom(&cStyle);

	return *this;
}

void CCAD_DimStyle::Init()
{
	CCAD_Style::Init();

	m_sDimLine.wShow = DIM_LINE_SHOW_FIRST | DIM_LINE_SHOW_SECOND;
	m_sDimLine.rgbColor = RGB(0x00, 0x00, 0x00);
	m_sDimLine.nThick = 0;
	m_sDimLine.fExt = 0;
	m_sDimLine.fBase = 1500;
	m_sDimExt.wShow = DIM_LINE_SHOW_FIRST | DIM_LINE_SHOW_SECOND;
	m_sDimExt.rgbColor = RGB(0x00, 0x00, 0x00);
	m_sDimExt.nThick = 0;
	m_sDimExt.fExt = 500;
	m_sDimExt.fBase = 250;

	m_sDimArrow.nFirst = 0;
	m_sDimArrow.nSecond = 0;
	m_sDimArrow.nLeader = 0;
	m_sDimArrow.fSize = 1000;
	m_sDimArrow.nMark = DIM_ARROW_CENTER_MARK;
	m_sDimArrow.fMarkSize = 100;

	m_sDimText.nStyle = 0;
	m_sDimText.rgbColor = RGB(0x00, 0x00, 0x00);
	m_sDimText.fHeight = 1000;
	m_sDimText.fWidth = 1000;
	m_sDimText.fGap = 250;
	m_sDimText.sState.horz = DIM_TEXT_HORZ_CENTER;
	m_sDimText.sState.vert = DIM_TEXT_VERT_ABOVE;
	m_sDimText.sState.align = DIM_TEXT_ALIGN_WITH;
	m_sDimText.sState.rect = 0;
	m_sDimText.sState.fit = DIM_TEXT_FIT_BEST;
	m_sDimText.sState.arrow = 0;
	m_sDimText.sState.place = DIM_TEXT_PLACE_BESIDE;
	m_sDimText.sState.manual = 0;
	m_sDimText.sState.always = 1;

	m_sDimUnit.nLengthUnit = 2;
	m_sDimUnit.nAngleUnit = 0;
	m_sDimUnit.fRound = 0;
	m_sDimUnit.fScale = 0;
	m_sDimUnit.szPrefix[0] = '\0';
	m_sDimUnit.szSuffix[0] = '\0';
	m_sDimUnit.sState.lengthhead = 0;
	m_sDimUnit.sState.lengthtail = 0;
	m_sDimUnit.sState.anglehead = 0;
	m_sDimUnit.sState.angletail = 0;
	m_sDimUnit.sState.decimal = 0;
	m_sDimUnit.sState.scalelayout = 0;

	m_sDimToler.nUnit = m_sDimUnit.nLengthUnit;
	m_sDimToler.fUpper = 0;
	m_sDimToler.fLower = 0;
	m_sDimToler.fScale = 1;
	m_sDimToler.sState.head = 0;
	m_sDimToler.sState.tail = 0;
	m_sDimToler.sState.format = 0;
	m_sDimToler.sState.vert = DIM_TOLER_VERT_TOP;
}


void CCAD_DimStyle::CopyFrom(const CCAD_Style *pStyle)
{
	CCAD_Style::CopyFrom(pStyle);

	CCAD_DimStyle	*pDim = (CCAD_DimStyle *) pStyle;

	m_sDimLine = pDim->m_sDimLine;
	m_sDimExt = pDim->m_sDimExt;
	m_sDimArrow = pDim->m_sDimArrow;
	m_sDimText = pDim->m_sDimText;
	m_sDimUnit = pDim->m_sDimUnit;
	m_sDimToler = pDim->m_sDimToler;
}


BOOL CCAD_DimStyle::IsEqual(const CCAD_Style *pStyle)
{
	if ( strlen(pStyle->m_szName) > 0 ) {
		if (strcmp(m_szName, pStyle->m_szName) == 0)
			return true;
	}

	if (m_sDimLine.wShow != ((CCAD_DimStyle*)pStyle)->m_sDimLine.wShow)
		return false;
	if (m_sDimLine.rgbColor != ((CCAD_DimStyle*)pStyle)->m_sDimLine.rgbColor)
		return false;
	if (m_sDimLine.nThick != ((CCAD_DimStyle*)pStyle)->m_sDimLine.nThick)
		return false;
	if (m_sDimLine.fExt != ((CCAD_DimStyle*)pStyle)->m_sDimLine.fExt)
		return false;
	if (m_sDimLine.fBase != ((CCAD_DimStyle*)pStyle)->m_sDimLine.fBase)
		return false;

	if (m_sDimExt.wShow != ((CCAD_DimStyle*)pStyle)->m_sDimExt.wShow)
		return false;
	if (m_sDimExt.rgbColor != ((CCAD_DimStyle*)pStyle)->m_sDimExt.rgbColor)
		return false;
	if (m_sDimExt.nThick != ((CCAD_DimStyle*)pStyle)->m_sDimExt.nThick)
		return false;
	if (m_sDimExt.fExt != ((CCAD_DimStyle*)pStyle)->m_sDimExt.fExt)
		return false;
	if (m_sDimExt.fBase != ((CCAD_DimStyle*)pStyle)->m_sDimExt.fBase)
		return false;

	if (m_sDimArrow.fMarkSize != ((CCAD_DimStyle*)pStyle)->m_sDimArrow.fMarkSize)
		return false;
	if (m_sDimArrow.nFirst != ((CCAD_DimStyle*)pStyle)->m_sDimArrow.nFirst)
		return false;
	if (m_sDimArrow.nSecond != ((CCAD_DimStyle*)pStyle)->m_sDimArrow.nSecond)
		return false;
	if (m_sDimArrow.nLeader != ((CCAD_DimStyle*)pStyle)->m_sDimArrow.nLeader)
		return false;
	if (m_sDimArrow.fSize != ((CCAD_DimStyle*)pStyle)->m_sDimArrow.fSize)
		return false;
	if (m_sDimArrow.nMark != ((CCAD_DimStyle*)pStyle)->m_sDimArrow.nMark)
		return false;

	if (m_sDimText.fGap != ((CCAD_DimStyle*)pStyle)->m_sDimText.fGap)
		return false;
	if (m_sDimText.nStyle != ((CCAD_DimStyle*)pStyle)->m_sDimText.nStyle)
		return false;
	if (m_sDimText.rgbColor != ((CCAD_DimStyle*)pStyle)->m_sDimText.rgbColor)
		return false;
	if (m_sDimText.fHeight != ((CCAD_DimStyle*)pStyle)->m_sDimText.fHeight)
		return false;
	if (m_sDimText.fWidth != ((CCAD_DimStyle*)pStyle)->m_sDimText.fWidth)
		return false;
	if (m_sDimText.sState.align != ((CCAD_DimStyle*)pStyle)->m_sDimText.sState.align)
		return false;
	if (m_sDimText.sState.horz != ((CCAD_DimStyle*)pStyle)->m_sDimText.sState.horz)
		return false;
	if (m_sDimText.sState.vert != ((CCAD_DimStyle*)pStyle)->m_sDimText.sState.vert)
		return false;
	if (m_sDimText.sState.rect != ((CCAD_DimStyle*)pStyle)->m_sDimText.sState.rect)
		return false;
	if (m_sDimText.sState.fit != ((CCAD_DimStyle*)pStyle)->m_sDimText.sState.fit)
		return false;
	if (m_sDimText.sState.arrow != ((CCAD_DimStyle*)pStyle)->m_sDimText.sState.arrow)
		return false;
	if (m_sDimText.sState.place != ((CCAD_DimStyle*)pStyle)->m_sDimText.sState.place)
		return false;
	if (m_sDimText.sState.manual != ((CCAD_DimStyle*)pStyle)->m_sDimText.sState.manual)
		return false;
	if (m_sDimText.sState.always != ((CCAD_DimStyle*)pStyle)->m_sDimText.sState.always)
		return false;

	if (m_sDimUnit.fRound != ((CCAD_DimStyle*)pStyle)->m_sDimUnit.fRound)
		return false;
	if (m_sDimUnit.nLengthUnit != ((CCAD_DimStyle*)pStyle)->m_sDimUnit.nLengthUnit)
		return false;
	if (m_sDimUnit.nAngleUnit != ((CCAD_DimStyle*)pStyle)->m_sDimUnit.nAngleUnit)
		return false;
	if (m_sDimUnit.fScale != ((CCAD_DimStyle*)pStyle)->m_sDimUnit.fScale)
		return false;
	if (strcmp(m_sDimUnit.szPrefix, ((CCAD_DimStyle*)pStyle)->m_sDimUnit.szPrefix) != 0)
		return false;
	if (strcmp(m_sDimUnit.szSuffix, ((CCAD_DimStyle*)pStyle)->m_sDimUnit.szSuffix) != 0)
		return false;
	if (m_sDimUnit.sState.anglehead != ((CCAD_DimStyle*)pStyle)->m_sDimUnit.sState.anglehead)
		return false;
	if (m_sDimUnit.sState.lengthhead != ((CCAD_DimStyle*)pStyle)->m_sDimUnit.sState.lengthhead)
		return false;
	if (m_sDimUnit.sState.lengthtail != ((CCAD_DimStyle*)pStyle)->m_sDimUnit.sState.lengthtail)
		return false;
	if (m_sDimUnit.sState.angletail != ((CCAD_DimStyle*)pStyle)->m_sDimUnit.sState.angletail)
		return false;
	if (m_sDimUnit.sState.decimal != ((CCAD_DimStyle*)pStyle)->m_sDimUnit.sState.decimal)
		return false;
	if (m_sDimUnit.sState.scalelayout != ((CCAD_DimStyle*)pStyle)->m_sDimUnit.sState.scalelayout)
		return false;

	if (m_sDimToler.fLower != ((CCAD_DimStyle*)pStyle)->m_sDimToler.fLower)
		return false;
	if (m_sDimToler.fUpper != ((CCAD_DimStyle*)pStyle)->m_sDimToler.fUpper)
		return false;
	if (m_sDimToler.nUnit != ((CCAD_DimStyle*)pStyle)->m_sDimToler.nUnit)
		return false;
	if (m_sDimToler.fScale != ((CCAD_DimStyle*)pStyle)->m_sDimToler.fScale)
		return false;
	if (m_sDimToler.sState.format != ((CCAD_DimStyle*)pStyle)->m_sDimToler.sState.format)
		return false;
	if (m_sDimToler.sState.head != ((CCAD_DimStyle*)pStyle)->m_sDimToler.sState.head)
		return false;
	if (m_sDimToler.sState.tail != ((CCAD_DimStyle*)pStyle)->m_sDimToler.sState.tail)
		return false;
	if (m_sDimToler.sState.vert != ((CCAD_DimStyle*)pStyle)->m_sDimToler.sState.vert)
		return false;
	return true;
}


BOOL CCAD_DimStyle::Read(CCAD_File & cFile)
{
	CCAD_Style::Read(cFile);
	m_sDimLine.Read(cFile);
	m_sDimExt.Read(cFile);
	m_sDimArrow.Read(cFile);
	m_sDimText.Read(cFile);
	m_sDimUnit.Read(cFile);
	m_sDimToler.Read(cFile);
	return TRUE;
}


void	ReverseBits(void* vByte, int nSize/*=1*/)
{
	BYTE* pByte = (BYTE*)vByte;
	for( int nByte=0; nByte<nSize; nByte++) {
		BYTE temp = pByte[nByte];
		pByte[nByte] = 0;
		for (int i=0; i<8; i++)
			pByte[nByte] += ((temp>>i)&0x01) << (7-i);
	}
}
void	SwapBits(void* vByte, int n1, int n2/*=-1*/)
{
	if( n2<0 )	n2 = n1+1 ;

	BYTE* pByte = (BYTE*)vByte;
	if( n1>7 ) {
		pByte++;
		n1 -= 8;
		n2 -= 8;
	}
	BYTE b1 = 1 << (7-n1);
	BYTE b2 = 1 << (7-n2);
	BYTE c1 = 0xFF - b1;
	BYTE c2 = 0xFF - b2;
	BYTE temp = *pByte;
	if( (temp&b1) == b1 )
		*pByte |= b2;	// bit Set.
	else
		*pByte &= c2;	// bit Clean

	if( (temp&b2) == b2 )
		*pByte |= b1;	// bit Set
	else
		*pByte &= c1;	// bit Clean
		
}
//-------------------------------------------

 
