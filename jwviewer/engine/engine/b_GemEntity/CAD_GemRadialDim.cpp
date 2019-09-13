#include "stdafx.h"
#include "CAD_GemRadialDim.h"
#include "CAD_Doc.h"

CCAD_GemRadialDim::CCAD_GemRadialDim(CCAD_Doc *pDoc) : CCAD_GemDimension(pDoc)
{
}

CCAD_GemRadialDim::~CCAD_GemRadialDim()
{
}

void CCAD_GemRadialDim::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_GemDimension::MemCopyTo(pGemEntity, pData1, pData2, pData3);

	CCAD_GemRadialDim	*pDim = (CCAD_GemRadialDim *) pGemEntity;
	pDim->m_ptEnd1 = m_ptEnd1;
	pDim->m_ptEnd2 = m_ptEnd2;
}

BOOL CCAD_GemRadialDim::Read(CCAD_File& cFile)
{
	CCAD_GemDimension::Read(cFile);

	m_ptEnd1.Read(cFile);
	m_ptEnd2.Read(cFile);

	return TRUE;
}

void CCAD_GemRadialDim::DrawDimension(BOOL blTmp)
{
	short nSpaceDoc = m_pDoc->GetCurrentDoc()->m_nID;
	short nUds = m_pDoc->GetUds()->m_nID;

	unsigned short i;

	CCAD_GemLine baseDim;		// default dimension line
	GetBaseDimLine(baseDim);

	CAD_FLOAT fDimLength = m_ptEnd1.CountLength(m_ptEnd2);
	BYTE bTypeCode;
	if((m_nOption & RADIALDIM_RADIUS) != 0)
		bTypeCode = DIM_RADIUS;
	else
		bTypeCode = DIM_DIAMETER;
	CCAD_TextEntity txt(m_pDoc, nSpaceDoc, nUds);
	CCAD_TextEntity upper(m_pDoc, nSpaceDoc, nUds);
	CCAD_TextEntity lower(m_pDoc, nSpaceDoc, nUds);
	i = 0;
	// get text string & local locations
	GetDefaultTextParameter(bTypeCode, fDimLength, txt, upper, lower, i);
	CCAD_TextEntity* pTxt[3];
	short nTxtCount = 0;
	if((i & 0x01) != 0)
	{
		pTxt[nTxtCount] = &txt;
		nTxtCount++;
	}
	if((i & 0x02) != 0)
	{
		pTxt[nTxtCount] = &upper;
		nTxtCount++;
	}
	if((i & 0x04) != 0)
	{
		pTxt[nTxtCount] = &lower;
		nTxtCount++;
	}
	ASSERT(nTxtCount > 0);
	CCAD_Coord3D* pTxtCoord = &pTxt[0]->GetGemEntity()->m_cBcs;
	CCAD_Rect boundTxt;
	// get local bounding of text array
	GetTextArrayBounds(nTxtCount, pTxt, boundTxt);

	// text alignment
	if(m_anglText.IsZero())
	{
		if(m_pStyle->m_sDimText.sState.align == DIM_TEXT_ALIGN_HORZ)
			pTxtCoord->SetCoord(CCAD_Vector(0, 0, 0), CCAD_Vector(1, 0, 0));
		else
			*pTxtCoord = baseDim.m_cBcs;
	}
	else
	{
		pTxtCoord->m_cX = CCAD_Vector(m_anglText.cos(), m_anglText.sin(), (CAD_FLOAT) 0);
		pTxtCoord->SetCoord(CCAD_Point(0, 0, 0), pTxtCoord->m_cX);
	}

	// get text bounding on dimension line coord
	m_nOption |= DIMTEXTPLACE_INSIDE;
	CAD_FLOAT fTxtWidth, fTxtHeight;
	GetTextArrayBoundsUnderAlignment(baseDim, pTxtCoord, boundTxt, fTxtWidth, fTxtHeight);

	// get in/out state of arrowhead & dimension text
	CheckInOutState(baseDim, fTxtWidth, fTxtHeight);

	if(((m_nOption & DIMTEXTPLACE_INSIDE) == 0) && (m_pStyle->m_sDimText.sState.align == DIM_TEXT_ALIGN_ISO))
	{	// if text alignment == ISO && outside place a dimension text, then horizontal alignment
		if(m_anglText.IsZero())
			pTxtCoord->SetCoord(CCAD_Vector(0, 0, 0), CCAD_Vector(1, 0, 0));
	}

	DrawDimensionLines(baseDim, pTxtCoord, boundTxt);
	DrawDimensionText(nTxtCount, pTxt);
	if (blTmp==FALSE)
		DrawArrowHeaders(baseDim);
}

void CCAD_GemRadialDim::GetBaseDimLine(CCAD_GemLine& baseDim)
{
	baseDim.m_cSp = m_ptEnd1;
	baseDim.m_cEp = m_ptEnd2;
	baseDim.m_cBcs.SetCoord(baseDim.m_cSp, baseDim.m_cEp - baseDim.m_cSp);
	baseDim.m_cBcs.ConvFromBase(baseDim.m_cSp);
	baseDim.m_cBcs.ConvFromBase(baseDim.m_cEp);
}

void CCAD_GemRadialDim::GetTextArrayBoundsUnderAlignment(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt,
														 CAD_FLOAT& fTxtWidth, CAD_FLOAT& fTxtHeight)
{
	CCAD_Coord3D coord(*pTxtCoord);
	if(((m_nOption & DIMTEXTPLACE_INSIDE) != 0)
		|| (((m_nOption & DIMTEXTPLACE_INSIDE) == 0) && (m_pStyle->m_sDimText.sState.align == DIM_TEXT_ALIGN_WITH)))
		baseDim.m_cBcs.ThisToChild(&coord);

	CCAD_Rect boundTxtParallel(boundTxt);
	coord.ConvToBase(boundTxtParallel);
	fTxtWidth = boundTxtParallel.Width() + 2 * m_pStyle->m_sDimText.fGap;
	fTxtHeight = boundTxtParallel.Height() + 2 * m_pStyle->m_sDimText.fGap;
}

void CCAD_GemRadialDim::CheckInOutState(CCAD_GemLine& baseDim, CAD_FLOAT fTxtWidth, CAD_FLOAT fTxtHeight)
{
	CAD_FLOAT fDimLength = baseDim.m_cSp.CountLength(baseDim.m_cEp);
	CAD_FLOAT fArrowWidth = 2 * m_pStyle->m_sDimArrow.fSize;

	// reset all in/out state flags
	m_nOption &= ~DIMTEXTORARROW_INOUTSTATEFIELD;

	CCAD_Vector ptText = m_ptText;
	baseDim.m_cBcs.ConvFromBase(ptText);

	if((m_nOption & DIMTEXTPLACE_NODEFAULT) != 0)
	{
		if(fDimLength > fArrowWidth)
			m_nOption |= DIMARROWPLACE_INSIDE;
		return;
	}

	if(m_pStyle->m_sDimText.sState.place == DIM_TEXT_PLACE_BESIDE)
	{
		if((fTxtWidth + fArrowWidth) > fDimLength)
		{
			switch(m_pStyle->m_sDimText.sState.fit)
			{
			case DIM_TEXT_FIT_BEST:
				if(m_pStyle->m_sDimText.sState.vert == DIM_TEXT_VERT_CENTER)
				{
					if(fTxtWidth > fArrowWidth)
					{
						if(fTxtWidth < fDimLength)
							m_nOption |= DIMTEXTPLACE_INSIDE;
						else
						if(fArrowWidth < fDimLength)
							m_nOption |= DIMARROWPLACE_INSIDE;
					}
					else
					{
						if(fArrowWidth < fDimLength)
							m_nOption |= DIMARROWPLACE_INSIDE;
						else
						if(fTxtWidth < fDimLength)
							m_nOption |= DIMTEXTPLACE_INSIDE;
					}
				}
				else
				{
					if(fTxtWidth < fDimLength)
						m_nOption |= DIMTEXTPLACE_INSIDE;
					if(fArrowWidth < fDimLength)
						m_nOption |= DIMARROWPLACE_INSIDE;
				}
				break;

			case DIM_TEXT_FIT_ARROW:
				if(fArrowWidth < fDimLength)
					m_nOption |= DIMARROWPLACE_INSIDE;
				break;

			case DIM_TEXT_FIT_TEXT:
				if(fTxtWidth < fDimLength)
					m_nOption |= DIMTEXTPLACE_INSIDE;
				break;

			case DIM_TEXT_FIT_BOTH:
				break;

			case DIM_TEXT_FIT_KEEPTEXT:
				m_nOption |= DIMTEXTPLACE_INSIDE;
				break;

			default:
				ASSERT(FALSE);
				break;
			}
		}
		else
			m_nOption |= DIMTEXTORARROW_INOUTSTATEFIELD;
	}
	else
	{
		if(ptText.m_fX < baseDim.m_cEp.m_fX)
		{
			m_nOption |= DIMARROWPLACE_INSIDE;
			if((fDimLength > fArrowWidth) && (ptText.m_fX > baseDim.m_cSp.m_fX))
				m_nOption |= DIMTEXTPLACE_INSIDE;
		}
		else
		{
			if(fDimLength > fArrowWidth)
				m_nOption |= DIMARROWPLACE_INSIDE;
		}
	}
}

void CCAD_GemRadialDim::DrawDimensionLines(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt)
{
	CAD_FLOAT fDimLength = baseDim.m_cSp.CountLength(baseDim.m_cEp);

	CAD_FLOAT fTxtWidth, fTxtHeight;
	GetTextArrayBoundsUnderAlignment(baseDim, pTxtCoord, boundTxt, fTxtWidth, fTxtHeight);

	if((m_nOption & DIMTEXTPLACE_NODEFAULT) != 0)
	{
		pTxtCoord->m_cOrg = m_ptText;

		CCAD_LineEntity line(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
		CCAD_GemLine* pLine = (CCAD_GemLine*) line.GetGemEntity();
		baseDim.MemCopyTo(pLine);

		if((m_nOption & DIMARROWPLACE_INSIDE) != 0)
		{
			if(m_pStyle->m_sDimLine.wShow != 0)
			{
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
				{
					if(IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
						pLine->m_cSp.m_fX -= m_pStyle->m_sDimLine.fExt;
					else
						pLine->m_cSp.m_fX += m_pStyle->m_sDimArrow.fSize;
				}
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
				{
					if(IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
						pLine->m_cEp.m_fX += m_pStyle->m_sDimLine.fExt;
					else
						pLine->m_cEp.m_fX -= m_pStyle->m_sDimArrow.fSize;
				}
				DrawElement(DIMELEMENT_DIMENSION, &line);
			}
		}
		else
		{
			if(m_pStyle->m_sDimText.sState.always != 0)
			{
				pLine->m_cSp.m_fX = baseDim.m_cSp.m_fX;
				pLine->m_cEp.m_fX = baseDim.m_cEp.m_fX;
				DrawElement(DIMELEMENT_DIMENSION, &line);
			}

			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
			{
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
					pLine->m_cSp.m_fX -= m_pStyle->m_sDimArrow.fSize;
				pLine->m_cEp.m_fX = pLine->m_cSp.m_fX - m_pStyle->m_sDimArrow.fSize;
				DrawElement(DIMELEMENT_DIMENSION, &line);
			}
			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
			{
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
					pLine->m_cSp.m_fX += m_pStyle->m_sDimArrow.fSize;
				pLine->m_cEp.m_fX = pLine->m_cSp.m_fX + m_pStyle->m_sDimArrow.fSize;
				DrawElement(DIMELEMENT_DIMENSION, &line);
			}
		}

		return;
	}

	if(m_pStyle->m_sDimText.sState.place == DIM_TEXT_PLACE_BESIDE)
	{
		if((m_nOption & DIMTEXTPLACE_INSIDE) != 0)
		{
			CAD_FLOAT fXOff = fTxtWidth / 2;
			if(m_pStyle->m_sDimText.sState.vert == DIM_TEXT_VERT_CENTER)
				fXOff += m_pStyle->m_sDimArrow.fSize;
			switch(m_pStyle->m_sDimText.sState.horz)
			{
			case DIM_TEXT_HORZ_EXT1:
				m_ptText.m_fX = baseDim.m_cSp.m_fX + fXOff;
				break;

			case DIM_TEXT_HORZ_EXT2:
				m_ptText.m_fX = baseDim.m_cEp.m_fX - fXOff;
				break;

			default:		// center
				m_ptText.m_fX = (baseDim.m_cSp.m_fX + baseDim.m_cEp.m_fX) / 2;
				break;
			}
			m_ptText.m_fY = 0;
			baseDim.m_cBcs.ConvToBase(m_ptText);

			DrawTextInsidePlaceBeside(baseDim, pTxtCoord, boundTxt);
		}
		else
		{
			CAD_FLOAT fXOff = m_pStyle->m_sDimArrow.fSize;
			if(m_pStyle->m_sDimText.sState.vert == DIM_TEXT_VERT_CENTER)
				fXOff += m_pStyle->m_sDimArrow.fSize;
			m_ptText.m_fX = baseDim.m_cEp.m_fX + fXOff;
			m_ptText.m_fY = 0;
			baseDim.m_cBcs.ConvToBase(m_ptText);

			DrawTextOutsidePlaceBeside(baseDim, pTxtCoord, boundTxt);
		}
	}
	else
	{
		CCAD_Vector ptText(m_ptText);
		baseDim.m_cBcs.ConvFromBase(ptText);

//		DrawTextPlaceNoBeside(baseDim, pTxtCoord, boundTxt);
		DrawTextOutsidePlaceBeside(baseDim, pTxtCoord, boundTxt);
	}

	CAD_FLOAT fYOff = fTxtHeight / 2;
	if(m_pStyle->m_sDimText.sState.vert != DIM_TEXT_VERT_CENTER)
	{
		BOOL bHorzTxtDir = FALSE;
		if(m_pStyle->m_sDimText.sState.align == DIM_TEXT_ALIGN_HORZ)
			bHorzTxtDir = TRUE;
		else
		if((m_pStyle->m_sDimText.sState.align == DIM_TEXT_ALIGN_ISO) && ((m_nOption & DIMTEXTPLACE_INSIDE) == 0))
			bHorzTxtDir = TRUE;

		if(bHorzTxtDir)
		{
			pTxtCoord->m_cOrg.m_fY += fYOff;
		}
		else
		{
			baseDim.m_cBcs.ConvFromBase(pTxtCoord->m_cOrg);
			if(baseDim.m_cBcs.m_cY.m_fY < 0)
				pTxtCoord->m_cOrg.m_fY -= fYOff;
			else
				pTxtCoord->m_cOrg.m_fY += fYOff;
			baseDim.m_cBcs.ConvToBase(pTxtCoord->m_cOrg);
		}
	}
}

void CCAD_GemRadialDim::DrawTextInsidePlaceBeside(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt)
{
	CAD_FLOAT fDimLength = baseDim.m_cSp.CountLength(baseDim.m_cEp);

	CAD_FLOAT fTxtWidth, fTxtHeight;
	GetTextArrayBoundsUnderAlignment(baseDim, pTxtCoord, boundTxt, fTxtWidth, fTxtHeight);

	pTxtCoord->m_cOrg = m_ptText;
	CCAD_LineEntity line(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
	CCAD_GemLine* pLine = (CCAD_GemLine*) line.GetGemEntity();
	baseDim.MemCopyTo(pLine);

	if(m_pStyle->m_sDimText.sState.vert == DIM_TEXT_VERT_CENTER)
	{
		CCAD_Vector ptText(m_ptText);
		baseDim.m_cBcs.ConvFromBase(ptText);

		if((m_nOption & DIMARROWPLACE_INSIDE) != 0)
		{
			if(fDimLength > fTxtWidth)
			{	// dim line break
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
				{
					pLine->m_cSp.m_fX = 0;
					if(IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
						pLine->m_cSp.m_fX -= m_pStyle->m_sDimLine.fExt;
					else
						pLine->m_cSp.m_fX += m_pStyle->m_sDimArrow.fSize;
					pLine->m_cEp.m_fX = ptText.m_fX - fTxtWidth / 2;
					DrawElement(DIMELEMENT_DIMENSION, &line);
				}
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
				{
					pLine->m_cSp.m_fX = fDimLength;
					if(IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
						pLine->m_cSp.m_fX += m_pStyle->m_sDimLine.fExt;
					else
						pLine->m_cSp.m_fX -= m_pStyle->m_sDimArrow.fSize;
					pLine->m_cEp.m_fX = ptText.m_fX + fTxtWidth / 2;
					DrawElement(DIMELEMENT_DIMENSION, &line);
				}
			}
		}
		else
		{
			if(m_pStyle->m_sDimText.sState.always != 0)
			{
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
				{
					pLine->m_cSp.m_fX = 0;
					pLine->m_cEp.m_fX = ptText.m_fX - fTxtWidth / 2;
					DrawElement(DIMELEMENT_DIMENSION, &line);
				}
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
				{
					pLine->m_cSp.m_fX = fDimLength;
					pLine->m_cEp.m_fX = ptText.m_fX + fTxtWidth / 2;
					DrawElement(DIMELEMENT_DIMENSION, &line);
				}
			}

			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
			{
				pLine->m_cSp.m_fX = 0;
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
					pLine->m_cSp.m_fX -= m_pStyle->m_sDimArrow.fSize;
				pLine->m_cEp.m_fX = pLine->m_cSp.m_fX - m_pStyle->m_sDimArrow.fSize;
				DrawElement(DIMELEMENT_DIMENSION, &line);
			}
			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
			{
				pLine->m_cSp.m_fX = fDimLength;
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
					pLine->m_cSp.m_fX += m_pStyle->m_sDimArrow.fSize;
				pLine->m_cEp.m_fX = pLine->m_cSp.m_fX + m_pStyle->m_sDimArrow.fSize;
				DrawElement(DIMELEMENT_DIMENSION, &line);
			}
		}
	}
	else
	{	// vertical place is no-center
		if((m_nOption & DIMARROWPLACE_INSIDE) != 0)
		{
			if(m_pStyle->m_sDimLine.wShow != 0)
			{
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
				{
					if(IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
						pLine->m_cSp.m_fX -= m_pStyle->m_sDimLine.fExt;
					else
						pLine->m_cSp.m_fX += m_pStyle->m_sDimArrow.fSize;
				}
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
				{
					if(IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
						pLine->m_cEp.m_fX += m_pStyle->m_sDimLine.fExt;
					else
						pLine->m_cEp.m_fX -= m_pStyle->m_sDimArrow.fSize;
				}
				DrawElement(DIMELEMENT_DIMENSION, &line);
			}
		}
		else
		{
			if(m_pStyle->m_sDimText.sState.always != 0)
			{
				pLine->m_cSp.m_fX = baseDim.m_cSp.m_fX;
				pLine->m_cEp.m_fX = baseDim.m_cEp.m_fX;
				DrawElement(DIMELEMENT_DIMENSION, &line);
			}

			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
			{
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
					pLine->m_cSp.m_fX -= m_pStyle->m_sDimArrow.fSize;
				pLine->m_cEp.m_fX = pLine->m_cSp.m_fX - m_pStyle->m_sDimArrow.fSize;
				DrawElement(DIMELEMENT_DIMENSION, &line);
			}
			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
			{
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
					pLine->m_cSp.m_fX += m_pStyle->m_sDimArrow.fSize;
				pLine->m_cEp.m_fX = pLine->m_cSp.m_fX + m_pStyle->m_sDimArrow.fSize;
				DrawElement(DIMELEMENT_DIMENSION, &line);
			}
		}
	}
}

void CCAD_GemRadialDim::DrawTextOutsidePlaceBeside(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt)
{
	CAD_FLOAT fTxtWidth, fTxtHeight;
	GetTextArrayBoundsUnderAlignment(baseDim, pTxtCoord, boundTxt, fTxtWidth, fTxtHeight);

	pTxtCoord->m_cOrg = m_ptText;
	CCAD_LineEntity line(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
	CCAD_GemLine* pLine = (CCAD_GemLine*) line.GetGemEntity();
	baseDim.MemCopyTo(pLine);

	CCAD_Vector ptText(m_ptText);
	baseDim.m_cBcs.ConvFromBase(ptText);

	if(m_pStyle->m_sDimText.sState.align == DIM_TEXT_ALIGN_WITH)
	{
		CCAD_Coord3D coord(*pTxtCoord);
		baseDim.m_cBcs.ThisToChild(&coord);

		CCAD_Rect boundTxtUnderBaseLine(boundTxt);
		coord.ConvToBase(boundTxtUnderBaseLine);

		baseDim.m_cBcs.ConvFromBase(pTxtCoord->m_cOrg);
		if(ptText.m_fX < 0)
			pTxtCoord->m_cOrg.m_fX -= boundTxtUnderBaseLine.Width() / 2;
		else
			pTxtCoord->m_cOrg.m_fX += boundTxtUnderBaseLine.Width() / 2;
		baseDim.m_cBcs.ConvToBase(pTxtCoord->m_cOrg);
	}
	else
	{
		if(ptText.m_fX < (baseDim.m_cSp.m_fX + baseDim.m_cEp.m_fX) / 2)
			pTxtCoord->m_cOrg.m_fX -= fTxtWidth / 2 - m_pStyle->m_sDimText.fGap;
		else
			pTxtCoord->m_cOrg.m_fX += fTxtWidth / 2 - m_pStyle->m_sDimText.fGap;
	}

	if((m_nOption & DIMARROWPLACE_INSIDE) != 0)
	{
		pLine->m_cSp.m_fX = baseDim.m_cSp.m_fX;
		pLine->m_cEp.m_fX = baseDim.m_cEp.m_fX;
		if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
		{
			if(IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
				pLine->m_cSp.m_fX -= m_pStyle->m_sDimLine.fExt;
			else
				pLine->m_cSp.m_fX += m_pStyle->m_sDimArrow.fSize;
		}
		if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
		{
			if(IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
				pLine->m_cEp.m_fX += m_pStyle->m_sDimLine.fExt;
			else
				pLine->m_cEp.m_fX -= m_pStyle->m_sDimArrow.fSize;
		}
		DrawElement(DIMELEMENT_DIMENSION, &line);
	}
	else
	{
		if(m_pStyle->m_sDimText.sState.always != 0)
		{
			pLine->m_cSp.m_fX = baseDim.m_cSp.m_fX;
			pLine->m_cEp.m_fX = baseDim.m_cEp.m_fX;
			DrawElement(DIMELEMENT_DIMENSION, &line);
		}

		if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
		{
			pLine->m_cSp.m_fX = baseDim.m_cSp.m_fX;
			if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst) && (m_pStyle->m_sDimText.sState.arrow == 0))
				pLine->m_cSp.m_fX -= m_pStyle->m_sDimArrow.fSize;
			pLine->m_cEp.m_fX = pLine->m_cSp.m_fX - m_pStyle->m_sDimArrow.fSize;
			DrawElement(DIMELEMENT_DIMENSION, &line);
		}
		if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
		{
			pLine->m_cSp.m_fX = baseDim.m_cEp.m_fX;
			if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond) && (m_pStyle->m_sDimText.sState.arrow == 0))
				pLine->m_cSp.m_fX += m_pStyle->m_sDimArrow.fSize;
			pLine->m_cEp.m_fX = pLine->m_cSp.m_fX + m_pStyle->m_sDimArrow.fSize;
			DrawElement(DIMELEMENT_DIMENSION, &line);
		}
	}

	if(m_pStyle->m_sDimText.sState.align == DIM_TEXT_ALIGN_WITH)
	{
		if(ptText.m_fX < baseDim.m_cSp.m_fX)
		{
			pLine->m_cSp.m_fX = baseDim.m_cSp.m_fX;
			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
			{
				if((m_nOption & DIMARROWPLACE_INSIDE) == 0)
				{
					if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst) && (m_pStyle->m_sDimText.sState.arrow == 0))
						pLine->m_cSp.m_fX -= m_pStyle->m_sDimArrow.fSize;
					pLine->m_cSp.m_fX -= m_pStyle->m_sDimArrow.fSize;
				}
			}
			pLine->m_cEp.m_fX = ptText.m_fX;
			if(m_pStyle->m_sDimText.sState.vert != DIM_TEXT_VERT_CENTER)
				pLine->m_cEp.m_fX -= fTxtWidth - m_pStyle->m_sDimText.fGap;		// + m_pStyle->m_sDimText.fGap;
			// move text coord
			baseDim.m_cBcs.ConvFromBase(pTxtCoord->m_cOrg);
			pTxtCoord->m_cOrg.m_fX -= m_pStyle->m_sDimText.fGap;
			baseDim.m_cBcs.ConvToBase(pTxtCoord->m_cOrg);
		}
		else						// at ext2
		{
			pLine->m_cSp.m_fX = baseDim.m_cEp.m_fX;
			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
			{
				if((m_nOption & DIMARROWPLACE_INSIDE) == 0)
				{
					if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond) && (m_pStyle->m_sDimText.sState.arrow == 0))
						pLine->m_cSp.m_fX += m_pStyle->m_sDimArrow.fSize;
					pLine->m_cSp.m_fX += m_pStyle->m_sDimArrow.fSize;
				}
			}
			pLine->m_cEp.m_fX = ptText.m_fX;
			if(m_pStyle->m_sDimText.sState.vert != DIM_TEXT_VERT_CENTER)
				pLine->m_cEp.m_fX += fTxtWidth - m_pStyle->m_sDimText.fGap;		// + m_pStyle->m_sDimText.fGap
			// move text coord
			baseDim.m_cBcs.ConvFromBase(pTxtCoord->m_cOrg);
			pTxtCoord->m_cOrg.m_fX += m_pStyle->m_sDimText.fGap;
			baseDim.m_cBcs.ConvToBase(pTxtCoord->m_cOrg);
		}
		DrawElement(DIMELEMENT_DIMENSION, &line);
	}
	else
	{
		CCAD_Angle angle;
		angle.SetFromXY(baseDim.m_cBcs.m_cX.m_fX, baseDim.m_cBcs.m_cX.m_fY);

		if(ptText.m_fX < baseDim.m_cSp.m_fX)
		{
			pLine->m_cSp.m_fX = baseDim.m_cSp.m_fX;
			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
			{
				if((m_nOption & DIMARROWPLACE_INSIDE) == 0)
				{
					if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst) && (m_pStyle->m_sDimText.sState.arrow == 0))
						pLine->m_cSp.m_fX -= m_pStyle->m_sDimArrow.fSize;
					pLine->m_cSp.m_fX -= m_pStyle->m_sDimArrow.fSize;
				}
			}
			pLine->m_cEp.m_fX = ptText.m_fX;
			if(angle.Horizontal())
			{
				if(m_pStyle->m_sDimText.sState.vert != DIM_TEXT_VERT_CENTER)
					pLine->m_cEp.m_fX -= fTxtWidth - m_pStyle->m_sDimText.fGap;	// + m_pStyle->m_sDimText.fGap
				baseDim.m_cBcs.ConvFromBase(pTxtCoord->m_cOrg);
				pTxtCoord->m_cOrg.m_fX -= m_pStyle->m_sDimText.fGap;
				baseDim.m_cBcs.ConvToBase(pTxtCoord->m_cOrg);
			}
		}
		else						// at ext2
		{
			pLine->m_cSp.m_fX = baseDim.m_cEp.m_fX;
			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
			{
				if((m_nOption & DIMARROWPLACE_INSIDE) == 0)
				{
					if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond) && (m_pStyle->m_sDimText.sState.arrow == 0))
						pLine->m_cSp.m_fX += m_pStyle->m_sDimArrow.fSize;
					pLine->m_cSp.m_fX += m_pStyle->m_sDimArrow.fSize;
				}
			}
			pLine->m_cEp.m_fX = ptText.m_fX;
			if(angle.Horizontal())
			{
				if(m_pStyle->m_sDimText.sState.vert != DIM_TEXT_VERT_CENTER)
					pLine->m_cEp.m_fX += fTxtWidth - m_pStyle->m_sDimText.fGap;	// + m_pStyle->m_sDimText.fGap
				baseDim.m_cBcs.ConvFromBase(pTxtCoord->m_cOrg);
				pTxtCoord->m_cOrg.m_fX += m_pStyle->m_sDimText.fGap;
				baseDim.m_cBcs.ConvToBase(pTxtCoord->m_cOrg);
			}
		}
		DrawElement(DIMELEMENT_DIMENSION, &line);

		if(!angle.Horizontal())
		{
			pLine->m_cBcs.SetCoord(CCAD_Vector(0, 0, 0), CCAD_Vector(1, 0, 0));
			pLine->m_cBcs.m_cOrg = m_ptText;

			if(!angle.IsAcuteAngle())
				pLine->m_cBcs.ZRotate(CCAD_Angle((short) 180));

			pLine->m_cSp = CCAD_Point(0, 0, 0);
			pLine->m_cEp = CCAD_Point(0, 0, 0);
			if(m_pStyle->m_sDimText.sState.vert == DIM_TEXT_VERT_CENTER)
			{
				pLine->m_cEp.m_fX += m_pStyle->m_sDimArrow.fSize;

				if(!angle.IsAcuteAngle())
					pTxtCoord->m_cOrg.m_fX -= m_pStyle->m_sDimArrow.fSize + m_pStyle->m_sDimText.fGap;
				else
					pTxtCoord->m_cOrg.m_fX += m_pStyle->m_sDimArrow.fSize + m_pStyle->m_sDimText.fGap;
			}
			else
			{
				pLine->m_cEp.m_fX += fTxtWidth - m_pStyle->m_sDimText.fGap;		// + m_pStyle->m_sDimText.fGap

				if(!angle.IsAcuteAngle())
					pTxtCoord->m_cOrg.m_fX -= m_pStyle->m_sDimText.fGap;
				else
					pTxtCoord->m_cOrg.m_fX += m_pStyle->m_sDimText.fGap;
			}
			DrawElement(DIMELEMENT_DIMENSION, &line);
		}
	}
}

void CCAD_GemRadialDim::DrawTextPlaceNoBeside(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt)
{
}

void CCAD_GemRadialDim::DrawArrowHeaders(CCAD_GemLine& baseDim)
{
	if(((m_nOption & DIMARROWPLACE_INSIDE) == 0) && (m_pStyle->m_sDimText.sState.arrow != 0))
		return;

	CCAD_BlockEntity arrow(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
	arrow.GetCadPen()->m_rgbColor = m_pStyle->m_sDimLine.rgbColor;
	arrow.GetCadPen()->m_nThick = m_pStyle->m_sDimLine.nThick;
	arrow.GetCadPen()->m_nStyle = m_pDoc->GetPenStyle();
	arrow.GetCadBrush()->m_rgbColor = m_pStyle->m_sDimLine.rgbColor;
	arrow.GetCadBrush()->m_nPattern = BRS_SOLID;

	arrow.m_wPriority |= PRI_BRUSH_COLOR;
	arrow.m_wPriority |= PRI_BRUSH_PATTERN;
	arrow.m_wPriority |= PRI_PEN_COLOR;
	arrow.m_wPriority |= PRI_PEN_STYLE;
	arrow.m_wPriority |= PRI_PEN_THICK;
	arrow.m_wPriority |= PRI_PEN_SCALE;
	CCAD_Coord3D* pArrowCoord = &arrow.m_cGemBlock.m_cBcs;
	if ( m_pStyle->m_sDimArrow.fSize > CAD_ZERO )
		pArrowCoord->m_fScale = m_pStyle->m_sDimArrow.fSize;
	if(m_pStyle->m_sDimLine.wShow != 0)
	{
		CCAD_Block* pArrowBlock;
		if((m_nOption & RADIALDIM_RADIUS) == 0)
		{
			pArrowBlock = GetArrowHeadBlock(m_pStyle->m_sDimArrow.nFirst);
			if(pArrowBlock != NULL)
			{
				*pArrowCoord = baseDim.m_cBcs;
				if((m_nOption & DIMARROWPLACE_INSIDE) != 0)
					pArrowCoord->ZRotate(CCAD_Angle((short) 180));
				if ( m_pStyle->m_sDimArrow.fSize > CAD_ZERO )
					pArrowCoord->m_fScale = m_pStyle->m_sDimArrow.fSize;
				strcpy(arrow.m_cGemBlock.m_szName, pArrowBlock->m_szName);
				DrawElement(DIMELEMENT_DIMENSION, &arrow);
			}
		}

		pArrowBlock = GetArrowHeadBlock(m_pStyle->m_sDimArrow.nSecond);
		if(pArrowBlock != NULL)
		{
			*pArrowCoord = baseDim.m_cBcs;
			pArrowCoord->m_cOrg = baseDim.m_cEp;
			baseDim.m_cBcs.ConvToBase(pArrowCoord->m_cOrg);
			if((m_nOption & DIMARROWPLACE_INSIDE) == 0)
				pArrowCoord->ZRotate(CCAD_Angle((short) 180));
			if ( m_pStyle->m_sDimArrow.fSize > CAD_ZERO )
				pArrowCoord->m_fScale = m_pStyle->m_sDimArrow.fSize;
			strcpy(arrow.m_cGemBlock.m_szName, pArrowBlock->m_szName);
			DrawElement(DIMELEMENT_DIMENSION, &arrow);
		}
	}
}

void CCAD_GemRadialDim::ReDefineCtrlPts()
{
	m_cBcs.ConvToBase(m_ptEnd1);
	m_cBcs.ConvToBase(m_ptEnd2);
	m_cBcs.ConvToBase(m_ptText);

	if((m_nOption & RADIALDIM_RADIUS) != 0)
		m_cBcs.m_cOrg = m_ptEnd1;
	else
	{
		m_cBcs.m_cOrg.m_fX = (m_ptEnd1.m_fX + m_ptEnd2.m_fX) / 2;
		m_cBcs.m_cOrg.m_fY = (m_ptEnd1.m_fY + m_ptEnd2.m_fY) / 2;
		m_cBcs.m_cOrg.m_fZ = (m_ptEnd1.m_fZ + m_ptEnd2.m_fZ) / 2;
	}

	m_cBcs.ConvFromBase(m_ptEnd1);
	m_cBcs.ConvFromBase(m_ptEnd2);
	m_cBcs.ConvFromBase(m_ptText);
}
