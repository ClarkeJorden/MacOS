﻿#include "stdafx.h"
#include "CAD_Global.h"
#include "CAD_GemLinearDim.h"
#include "CAD_Doc.h"

CCAD_GemLinearDim::CCAD_GemLinearDim(CCAD_Doc* pDoc) : CCAD_GemDimension(pDoc)
{
	Init();
}

CCAD_GemLinearDim::~CCAD_GemLinearDim()
{
}

void CCAD_GemLinearDim::Init()
{
	CCAD_GemDimension::Init();

	m_anglRotate = 0;
	m_anglOblique = 0;
}

void CCAD_GemLinearDim::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_GemDimension::MemCopyTo(pGemEntity, pData1, pData2, pData3);

	CCAD_GemLinearDim	*pDim = (CCAD_GemLinearDim *) pGemEntity;
	pDim->m_ptExt1 = m_ptExt1;
	pDim->m_ptExt2 = m_ptExt2;
	pDim->m_ptDimLine = m_ptDimLine;
	pDim->m_anglRotate = m_anglRotate;
	pDim->m_anglOblique = m_anglOblique;
}

BOOL CCAD_GemLinearDim::Read(CCAD_File& cFile)
{
	CCAD_GemDimension::Read(cFile);

	// original position parameter
	m_ptExt1.Read(cFile);
	m_ptExt2.Read(cFile);
	m_ptDimLine.Read(cFile);
	m_anglRotate.Read(cFile);
	m_anglOblique.Read(cFile);

	return TRUE;
}

void CCAD_GemLinearDim::DrawDimension(BOOL blTmp)
{
	short nSpaceDoc = m_pDoc->GetCurrentDoc()->m_nID;
	short nUds = m_pDoc->GetUds()->m_nID;

	unsigned short i;

	CCAD_GemLine baseDim;		// default dimension line
	GetBaseDimLine(baseDim);

	CCAD_Vector ptExt1(m_ptExt1);
	baseDim.m_cBcs.ConvFromBase(ptExt1);
	CCAD_Vector ptExt2(m_ptExt2);
	baseDim.m_cBcs.ConvFromBase(ptExt2);
	CAD_FLOAT fDimLength = (CAD_FLOAT) fabs((double) (ptExt2.m_fX - ptExt1.m_fX));

	CCAD_TextEntity txt(m_pDoc, nSpaceDoc, nUds);
	CCAD_TextEntity upper(m_pDoc, nSpaceDoc, nUds);
	CCAD_TextEntity lower(m_pDoc, nSpaceDoc, nUds);
	i = 0;
	// get text string & local locations
	GetDefaultTextParameter(DIM_LINEAR, fDimLength, txt, upper, lower, i);
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

	DrawExtensionLines(baseDim);
	DrawDimensionLines(baseDim, pTxtCoord, boundTxt);
	DrawDimensionText(nTxtCount, pTxt);
	if (blTmp==FALSE)
		DrawArrowHeaders(baseDim);
}

void CCAD_GemLinearDim::GetTextArrayBoundsUnderAlignment(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt,
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

void CCAD_GemLinearDim::GetBaseDimLine(CCAD_GemLine& baseDim)
{
	if((m_nOption & LINEARDIM_ALIGNED) != 0)
	{
		CCAD_Angle angle;
		angle.SetFromXY(m_ptExt2.m_fX - m_ptExt1.m_fX, m_ptExt2.m_fY - m_ptExt1.m_fY);
		baseDim.m_cBcs.ZRotate(angle);
	}
	else
	{
		baseDim.m_cBcs.ZRotate(m_anglRotate);
		if((m_nOption & LINEARDIM_HORZ) == 0)		// vertical
			baseDim.m_cBcs.ZRotate(CCAD_Angle((short) 90));
	}

	CCAD_Vector ptDimLine(m_ptDimLine);
	baseDim.m_cBcs.ConvFromBase(ptDimLine);
	if(ptDimLine.m_fY < 0)
		baseDim.m_cBcs.XRotate(CCAD_Angle((short) 180));
	ptDimLine = m_ptDimLine;
	baseDim.m_cBcs.ConvFromBase(ptDimLine);

	baseDim.m_cSp = m_ptExt1;
	baseDim.m_cBcs.ConvFromBase(baseDim.m_cSp);
	baseDim.m_cEp = m_ptExt2;
	baseDim.m_cBcs.ConvFromBase(baseDim.m_cEp);

	if((m_nOption & LINEARDIM_OBLIQUE) != 0)
	{
		CCAD_Vector ptOrg(0, 0, 0);
		baseDim.m_cBcs.ConvFromBase(ptOrg);
		CCAD_Vector ptOblique(m_anglOblique.cos(), m_anglOblique.sin(), 0);
		baseDim.m_cBcs.ConvFromBase(ptOblique);
		CCAD_Angle anglOblique;
		anglOblique.SetFromXY(ptOblique.m_fX - ptOrg.m_fX, ptOblique.m_fY - ptOrg.m_fY);

		if(!anglOblique.Horizontal())
		{
			baseDim.m_cSp.m_fX += (ptDimLine.m_fY - baseDim.m_cSp.m_fY) / anglOblique.tan();
			baseDim.m_cEp.m_fX += (ptDimLine.m_fY - baseDim.m_cEp.m_fY) / anglOblique.tan();
		}
	}

	baseDim.m_cSp.m_fY = ptDimLine.m_fY;
	baseDim.m_cEp.m_fY = ptDimLine.m_fY;

	BOOL bYAxisMirror = FALSE;
	if(baseDim.m_cEp.m_fX < baseDim.m_cSp.m_fX)
		bYAxisMirror = TRUE;

	baseDim.m_cBcs.ConvToBase(baseDim.m_cSp);
	baseDim.m_cBcs.ConvToBase(baseDim.m_cEp);

	baseDim.m_cBcs.m_cOrg.m_fX = (baseDim.m_cSp.m_fX + baseDim.m_cEp.m_fX) / 2;
	baseDim.m_cBcs.m_cOrg.m_fY = (baseDim.m_cSp.m_fY + baseDim.m_cEp.m_fY) / 2;
	if(bYAxisMirror)
		baseDim.m_cBcs.YRotate(CCAD_Angle((short) 180));

	baseDim.m_cBcs.ConvFromBase(baseDim.m_cSp);
	baseDim.m_cBcs.ConvFromBase(baseDim.m_cEp);
}

void CCAD_GemLinearDim::CheckInOutState(CCAD_GemLine& baseDim, CAD_FLOAT fTxtWidth, CAD_FLOAT fTxtHeight)
{
	CAD_FLOAT fDimLength = baseDim.m_cSp.CountLength(baseDim.m_cEp);
	CAD_FLOAT fArrowWidth = 2 * m_pStyle->m_sDimArrow.fSize;

	// reset all in/out state flags
	m_nOption &= ~DIMTEXTORARROW_INOUTSTATEFIELD;

	if((m_nOption & DIMTEXTPLACE_NODEFAULT) == 0)
	{	// text place default location
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
		if(fDimLength > fArrowWidth)
			m_nOption |= DIMARROWPLACE_INSIDE;
	}
}

void CCAD_GemLinearDim::DrawExtensionLines(CCAD_GemLine& baseDim)
{
	CAD_FLOAT fBase = m_pStyle->m_sDimExt.fBase;
	CAD_FLOAT fExt = m_pStyle->m_sDimExt.fExt;

	if((m_nOption & LINEARDIM_OBLIQUE) != 0)
	{
		CCAD_Vector ptOrg(0, 0, 0);
		baseDim.m_cBcs.ConvFromBase(ptOrg);
		CCAD_Vector ptOblique(m_anglOblique.cos(), m_anglOblique.sin(), 0);
		baseDim.m_cBcs.ConvFromBase(ptOblique);
		CCAD_Angle anglOblique;
//  	anglOblique.SetFromXY(ptOblique.m_fX - ptOrg.m_fX, ptOblique.m_fY - ptOrg.m_fY);
		anglOblique.SetFromXY(ptOblique.m_fX + ptOrg.m_fX, ptOblique.m_fY + ptOrg.m_fY);

		if(!anglOblique.Horizontal())
		{
			fBase /= (CAD_FLOAT) fabs((double) anglOblique.sin());
			fExt /= (CAD_FLOAT) fabs((double) anglOblique.sin());
		}
	}

	CCAD_LineEntity ext(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
	
	CCAD_GemLine* pGemLine = (CCAD_GemLine*) ext.GetGemEntity();
	pGemLine->m_cSp = m_ptExt1;
	pGemLine->m_cEp = baseDim.m_cSp;
	baseDim.m_cBcs.ConvToBase(pGemLine->m_cEp);
	pGemLine->m_cBcs.SetCoord(pGemLine->m_cSp, pGemLine->m_cEp - pGemLine->m_cSp);
	pGemLine->m_cBcs.ConvFromBase(pGemLine->m_cSp);
	pGemLine->m_cBcs.ConvFromBase(pGemLine->m_cEp);
	pGemLine->m_cSp.m_fX += fBase;
	pGemLine->m_cEp.m_fX += fExt;
	if((m_pStyle->m_sDimExt.wShow & DIM_LINE_SHOW_FIRST) != 0)
		DrawElement(DIMELEMENT_EXTENSION, &ext);

	pGemLine->m_cSp = m_ptExt2;
	pGemLine->m_cEp = baseDim.m_cEp;
	baseDim.m_cBcs.ConvToBase(pGemLine->m_cEp);
	pGemLine->m_cBcs.SetCoord(pGemLine->m_cSp, pGemLine->m_cEp - pGemLine->m_cSp);
	pGemLine->m_cBcs.ConvFromBase(pGemLine->m_cSp);
	pGemLine->m_cBcs.ConvFromBase(pGemLine->m_cEp);
	pGemLine->m_cSp.m_fX += fBase;
	pGemLine->m_cEp.m_fX += fExt;
	if((m_pStyle->m_sDimExt.wShow & DIM_LINE_SHOW_SECOND) != 0)
		DrawElement(DIMELEMENT_EXTENSION, &ext);
}

void CCAD_GemLinearDim::DrawDimensionLines(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt)
{
	CAD_FLOAT fDimLength = baseDim.m_cEp.m_fX;
	CAD_FLOAT fTxtWidth, fTxtHeight;
	GetTextArrayBoundsUnderAlignment(baseDim, pTxtCoord, boundTxt, fTxtWidth, fTxtHeight);

	if ((m_nOption & DIMTEXTPLACE_NODEFAULT) != 0)
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
						pLine->m_cSp.m_fX = baseDim.m_cSp.m_fX - m_pStyle->m_sDimLine.fExt;
					else
						pLine->m_cSp.m_fX = baseDim.m_cSp.m_fX + m_pStyle->m_sDimArrow.fSize;
				}
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
				{
					if(IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
						pLine->m_cEp.m_fX = baseDim.m_cEp.m_fX + m_pStyle->m_sDimLine.fExt;
					else
						pLine->m_cEp.m_fX = baseDim.m_cEp.m_fX - m_pStyle->m_sDimArrow.fSize;
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

		return;
	}

	if((m_nOption & DIMTEXTPLACE_INSIDE) != 0)
	{
		m_ptText = CCAD_Vector(0, 0, 0);			// center point under base line
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
			break;
		}
		baseDim.m_cBcs.ConvToBase(m_ptText);
		DrawTextInsidePlace(baseDim, pTxtCoord, boundTxt);
	}
	else
	{
		if(m_pStyle->m_sDimText.sState.place != DIM_TEXT_PLACE_BESIDE)
		{
			m_ptText = CCAD_Vector(0, 0, 0);			// center point under base line
			m_ptText.m_fY = (3 * m_pStyle->m_sDimExt.fExt + fTxtHeight) / 2 + m_pStyle->m_sDimText.fGap;
			CCAD_Angle angle((short) TANGENT_LEADERLINE);
			if(baseDim.m_cBcs.m_cX.m_fX * baseDim.m_cBcs.m_cY.m_fY < 0)
				m_ptText.m_fX = -m_ptText.m_fY * angle.tan();
			else
				m_ptText.m_fX = m_ptText.m_fY * angle.tan();
			baseDim.m_cBcs.ConvToBase(m_ptText);
			DrawTextOutsidePlaceNoBeside(baseDim, pTxtCoord, boundTxt);
		}
		else
		{
			m_ptText = CCAD_Vector(0, 0, 0);			// center point under base line
			if(m_pStyle->m_sDimText.sState.horz == DIM_TEXT_HORZ_EXT1)
			{
				m_ptText.m_fX = baseDim.m_cSp.m_fX - m_pStyle->m_sDimArrow.fSize;
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
				{
					if((m_nOption & DIMARROWPLACE_INSIDE) == 0)
					{
						if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst) && (m_pStyle->m_sDimText.sState.arrow == 0))
							m_ptText.m_fX -= m_pStyle->m_sDimArrow.fSize;
					}
				}
			}
			else
			{
				m_ptText.m_fX = baseDim.m_cEp.m_fX + m_pStyle->m_sDimArrow.fSize;
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
				{
					if((m_nOption & DIMARROWPLACE_INSIDE) == 0)
					{
						if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond) && (m_pStyle->m_sDimText.sState.arrow == 0))
							m_ptText.m_fX += m_pStyle->m_sDimArrow.fSize;
					}
				}
			}
			baseDim.m_cBcs.ConvToBase(m_ptText);
			DrawTextOutsidePlaceBeside(baseDim, pTxtCoord, boundTxt);
		}
	}

	m_ptDimLine = baseDim.m_cEp;
	baseDim.m_cBcs.ConvToBase(m_ptDimLine);

	CAD_FLOAT fYOff = fTxtHeight / 2;
	if(m_pStyle->m_sDimText.sState.vert != DIM_TEXT_VERT_CENTER)
	{
		if((m_pStyle->m_sDimText.sState.align != DIM_TEXT_ALIGN_WITH) && ((m_nOption & DIMTEXTPLACE_INSIDE) == 0))
		{
			if(m_pStyle->m_sDimText.sState.vert == DIM_TEXT_VERT_ABOVE)
				pTxtCoord->m_cOrg.m_fY += fYOff;
			else
			{
				if(baseDim.m_cBcs.m_cY.m_fY < 0)
					pTxtCoord->m_cOrg.m_fY -= fYOff;
				else
					pTxtCoord->m_cOrg.m_fY += fYOff;
			}
		}
		else
		{
			baseDim.m_cBcs.ThisToChild(pTxtCoord);
			if(m_pStyle->m_sDimText.sState.vert == DIM_TEXT_VERT_OUTSIDE)
				pTxtCoord->m_cOrg.m_fY += fYOff;
			else
			{
				CCAD_Angle angle;
				angle.SetFromXY(baseDim.m_cBcs.m_cY.m_fX, baseDim.m_cBcs.m_cY.m_fY);
				if(!angle.Horizontal())
				{
					if(baseDim.m_cBcs.m_cY.m_fY < 0)
						pTxtCoord->m_cOrg.m_fY -= fYOff;
					else
						pTxtCoord->m_cOrg.m_fY += fYOff;
				}
				else
				{
					if(angle.GetAbsDegree() < CCAD_Global::GetAngleAccy())
						pTxtCoord->m_cOrg.m_fY -= fYOff;
					else
						pTxtCoord->m_cOrg.m_fY += fYOff;
				}
			}
			baseDim.m_cBcs.ChildToThis(pTxtCoord);
		}
	}
}

void CCAD_GemLinearDim::DrawArrowHeaders(CCAD_GemLine& baseDim)
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
	if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
	{
		CCAD_Block* pArrowBlock = GetArrowHeadBlock(m_pStyle->m_sDimArrow.nFirst);
		if(pArrowBlock != NULL)
		{
			*pArrowCoord = baseDim.m_cBcs;
			pArrowCoord->m_cOrg = baseDim.m_cSp;
			baseDim.m_cBcs.ConvToBase(pArrowCoord->m_cOrg);
			if((m_nOption & DIMARROWPLACE_INSIDE) != 0)
				pArrowCoord->ZRotate(CCAD_Angle((short) 180));
			if ( m_pStyle->m_sDimArrow.fSize > CAD_ZERO )
				pArrowCoord->m_fScale = m_pStyle->m_sDimArrow.fSize;
			strcpy(arrow.m_cGemBlock.m_szName, pArrowBlock->m_szName);
			DrawElement(DIMELEMENT_DIMENSION, &arrow);
		}
	}
	if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
	{
		CCAD_Block* pArrowBlock = GetArrowHeadBlock(m_pStyle->m_sDimArrow.nSecond);
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

void CCAD_GemLinearDim::DrawTextInsidePlace(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt)
{
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
			if(baseDim.m_cEp.m_fX > fTxtWidth)
			{	// dim line break
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
				{
					pLine->m_cSp.m_fX = baseDim.m_cSp.m_fX;
					if(IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
						pLine->m_cSp.m_fX -= m_pStyle->m_sDimLine.fExt;
					else
						pLine->m_cSp.m_fX += m_pStyle->m_sDimArrow.fSize;
					pLine->m_cEp.m_fX = ptText.m_fX - fTxtWidth / 2;
					DrawElement(DIMELEMENT_DIMENSION, &line);
				}
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
				{
					pLine->m_cSp.m_fX = baseDim.m_cEp.m_fX;
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
					pLine->m_cSp.m_fX = baseDim.m_cSp.m_fX;
					pLine->m_cEp.m_fX = ptText.m_fX - fTxtWidth / 2;
					DrawElement(DIMELEMENT_DIMENSION, &line);
				}
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
				{
					pLine->m_cSp.m_fX = baseDim.m_cEp.m_fX;
					pLine->m_cEp.m_fX = ptText.m_fX + fTxtWidth / 2;
					DrawElement(DIMELEMENT_DIMENSION, &line);
				}
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
	}
	else
	{
		if((m_nOption & DIMARROWPLACE_INSIDE) != 0)
		{
			if(m_pStyle->m_sDimLine.wShow != 0)
			{
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
				{
					if(IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
						pLine->m_cSp.m_fX = baseDim.m_cSp.m_fX - m_pStyle->m_sDimLine.fExt;
					else
						pLine->m_cSp.m_fX = baseDim.m_cSp.m_fX + m_pStyle->m_sDimArrow.fSize;
				}
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
				{
					if(IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
						pLine->m_cEp.m_fX = baseDim.m_cEp.m_fX + m_pStyle->m_sDimLine.fExt;
					else
						pLine->m_cEp.m_fX = baseDim.m_cEp.m_fX - m_pStyle->m_sDimArrow.fSize;
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
	}
}

void CCAD_GemLinearDim::DrawTextOutsidePlaceBeside(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt)
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
		if(ptText.m_fX < 0)
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

void CCAD_GemLinearDim::DrawTextOutsidePlaceNoBeside(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt)
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
			pTxtCoord->m_cOrg.m_fX += boundTxtUnderBaseLine.Width() / 2;
		else
			pTxtCoord->m_cOrg.m_fX -= boundTxtUnderBaseLine.Width() / 2;
		baseDim.m_cBcs.ConvToBase(pTxtCoord->m_cOrg);
	}
	else
	{
		if(ptText.m_fX < 0)
			pTxtCoord->m_cOrg.m_fX += fTxtWidth / 2 - m_pStyle->m_sDimText.fGap;
		else
			pTxtCoord->m_cOrg.m_fX -= fTxtWidth / 2 - m_pStyle->m_sDimText.fGap;
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

	if(m_pStyle->m_sDimText.sState.place == DIM_TEXT_PLACE_LEADER)
	{
		pLine->m_cSp = CCAD_Vector(0, 0, 0);
		pLine->m_cEp = m_ptText;
		pLine->m_cBcs.ConvFromBase(pLine->m_cEp);
		DrawElement(DIMELEMENT_DIMENSION, &line);

		if(m_pStyle->m_sDimText.sState.align == DIM_TEXT_ALIGN_WITH)
			pLine->m_cBcs = baseDim.m_cBcs;
		else
			pLine->m_cBcs = CCAD_Coord3D(CCAD_Vector(0, 0, 0), CCAD_Vector(1, 0, 0));
		pLine->m_cBcs.m_cOrg = m_ptText;
		pLine->m_cSp = CCAD_Point(0, 0, 0);
		pLine->m_cEp = CCAD_Point(0, 0, 0);
		if(m_pStyle->m_sDimText.sState.vert == DIM_TEXT_VERT_CENTER)
		{
			pLine->m_cEp.m_fX += m_pStyle->m_sDimArrow.fSize;

			;
		}
		else
		{
			pLine->m_cEp.m_fX += fTxtWidth - m_pStyle->m_sDimText.fGap;		// + m_pStyle->m_sDimText.fGap

			;
		}
		DrawElement(DIMELEMENT_DIMENSION, &line);
	}
}

void CCAD_GemLinearDim::ReDefineCtrlPts()
{
	m_cBcs.ConvToBase(m_ptExt1);
	m_cBcs.ConvToBase(m_ptExt2);
	m_cBcs.ConvToBase(m_ptText);
	m_cBcs.ConvToBase(m_ptDimLine);

	m_cBcs.m_cOrg.m_fX = (m_ptExt1.m_fX + m_ptExt2.m_fX) / 2;
	m_cBcs.m_cOrg.m_fY = (m_ptExt1.m_fY + m_ptExt2.m_fY) / 2;
	m_cBcs.m_cOrg.m_fZ = (m_ptExt1.m_fZ + m_ptExt2.m_fZ) / 2;

	m_cBcs.ConvFromBase(m_ptExt1);
	m_cBcs.ConvFromBase(m_ptExt2);
	m_cBcs.ConvFromBase(m_ptText);
	m_cBcs.ConvFromBase(m_ptDimLine);
}

void CCAD_GemLinearDim::GetBaseCouninueDim(BOOL bContinue, CCAD_GemDimension* pGetDim, const CCAD_Vector& ptExt2)
	// ptExt2 : define on the world
{
	CCAD_GemLinearDim::MemCopyTo(pGetDim);

	CCAD_GemLinearDim* pGetLinearDim = (CCAD_GemLinearDim*) pGetDim;

	pGetLinearDim->m_szName[0] = 0x00;
	pGetLinearDim->m_szText[0] = 0x00;

	pGetLinearDim->m_nOption &= ~DIMTEXTPLACE_NODEFAULT;
	if(bContinue)
		pGetLinearDim->m_ptExt1 = m_ptExt2;
	else
	{
		CCAD_GemLine baseDim;
		GetBaseDimLine(baseDim);

		baseDim.m_cBcs.ConvFromBase(pGetLinearDim->m_ptText);
		pGetLinearDim->m_ptText.m_fY += m_pStyle->m_sDimLine.fBase;
		baseDim.m_cBcs.ConvToBase(pGetLinearDim->m_ptText);
	}
	pGetLinearDim->m_ptExt2 = ptExt2;
	pGetLinearDim->m_cBcs.ConvFromBase(pGetLinearDim->m_ptExt2);
	if((pGetLinearDim->m_nOption & LINEARDIM_ALIGNED) != 0)
	{
		pGetLinearDim->m_nOption &= ~LINEARDIM_ALIGNED;
		pGetLinearDim->m_anglRotate.SetFromXY(m_ptExt2.m_fX - m_ptExt1.m_fX, m_ptExt2.m_fY - m_ptExt1.m_fY);
	}
	pGetLinearDim->m_ptExt2.m_fZ = 0;
	pGetLinearDim->m_ptDimLine = pGetLinearDim->m_ptText;

	pGetLinearDim->ReDefineCtrlPts();
}


