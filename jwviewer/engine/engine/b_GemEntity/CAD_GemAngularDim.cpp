#include "stdafx.h"
#include "CAD_GemAngularDim.h"

CCAD_GemAngularDim::CCAD_GemAngularDim(CCAD_Doc * pDoc) : CCAD_GemDimension(pDoc)
{
}

CCAD_GemAngularDim::~CCAD_GemAngularDim()
{
}

void CCAD_GemAngularDim::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_GemDimension::MemCopyTo(pGemEntity, pData1, pData2, pData3);

	CCAD_GemAngularDim	*pDim = (CCAD_GemAngularDim *) pGemEntity;
	pDim->m_ptVertex = m_ptVertex;
	pDim->m_ptFirst = m_ptFirst;
	pDim->m_ptSecond = m_ptSecond;
	pDim->m_ptDimArc = m_ptDimArc;
}

BOOL CCAD_GemAngularDim::Read(CCAD_File& cFile)
{
	CCAD_GemDimension::Read(cFile);

	m_ptVertex.Read(cFile);
	m_ptFirst.Read(cFile);
	m_ptSecond.Read(cFile);
	m_ptDimArc.Read(cFile);
	return TRUE;
}

void CCAD_GemAngularDim::DrawDimension(BOOL blTmp)
{
	short nSpaceDoc = m_pDoc->GetCurrentDoc()->m_nID;
	short nUds = m_pDoc->GetUds()->m_nID;

	unsigned short i;

	CCAD_GemArc baseDim;
	GetBaseDimArc(baseDim);

	CCAD_TextEntity txt(m_pDoc, nSpaceDoc, nUds);
	CCAD_TextEntity upper(m_pDoc, nSpaceDoc, nUds);
	CCAD_TextEntity lower(m_pDoc, nSpaceDoc, nUds);
	i = 0;
	// get text string & local locations
	GetDefaultTextParameter(DIM_ANGULAR, baseDim.m_cDelta.GetDegree(), txt, upper, lower, i);
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

	// get in/out state of arrowhead & dimension text
	CheckInOutState(baseDim, boundTxt);

	DrawExtensionLines(baseDim);
	DrawDimensionLines(baseDim, pTxtCoord, boundTxt);
	DrawDimensionText(nTxtCount, pTxt);
	if (blTmp == FALSE)
		DrawArrowHeaders(baseDim);
}

void CCAD_GemAngularDim::GetBaseDimArc(CCAD_GemArc& baseDim)
{
	baseDim.m_fR = m_ptVertex.CountLength(m_ptDimArc);
	if((m_nOption & ANGULARDIM_QUADDIVIDE) != 0)
	{
		baseDim.m_cAlfa.SetFromXY(m_ptFirst.m_fX - m_ptVertex.m_fX, m_ptFirst.m_fY - m_ptVertex.m_fY);
		baseDim.m_cDelta.SetFromXY(m_ptSecond.m_fX - m_ptVertex.m_fX, m_ptSecond.m_fY - m_ptVertex.m_fY);
		baseDim.m_cDelta.SetFromDegree(baseDim.m_cDelta.GetDegree() - baseDim.m_cAlfa.GetDegree());
		if(baseDim.m_cDelta.GetDegree() < 0)
			baseDim.m_cDelta.SetFromDegree((CAD_FLOAT) 360 + baseDim.m_cDelta.GetDegree());
		if(baseDim.m_cDelta.GetDegree() > 180)
			baseDim.m_cDelta.SetFromDegree(baseDim.m_cDelta.GetDegree() - (CAD_FLOAT) 180);

		CCAD_Angle delta;
		delta.SetFromXY(m_ptDimArc.m_fX - m_ptVertex.m_fX, m_ptDimArc.m_fY - m_ptVertex.m_fY);
		delta.SetFromDegree(delta.GetDegree() - baseDim.m_cAlfa.GetDegree());
		if(delta.GetDegree() < 0)
			delta.SetFromDegree((CAD_FLOAT) 360 + delta.GetDegree());

		if(delta.GetDegree() < baseDim.m_cDelta.GetDegree())
		{
			;		// empty code
		}
		else
		if(delta.GetDegree() < 180)
		{
			baseDim.m_cAlfa.SetFromDegree((CAD_FLOAT) 180 + baseDim.m_cAlfa.GetDegree());
			baseDim.m_cDelta.SetFromDegree(baseDim.m_cDelta.GetDegree() - (CAD_FLOAT) 180);
		}
		else
		if(delta.GetDegree() < 180 + baseDim.m_cDelta.GetDegree())
		{
			baseDim.m_cAlfa.SetFromDegree((CAD_FLOAT) 180 + baseDim.m_cAlfa.GetDegree());
		}
		else
		{
			baseDim.m_cDelta.SetFromDegree(baseDim.m_cDelta.GetDegree() - (CAD_FLOAT) 180);
		}
	}
	else
	{
		baseDim.m_cAlfa.SetFromXY(m_ptFirst.m_fX - m_ptVertex.m_fX, m_ptFirst.m_fY - m_ptVertex.m_fY);
		baseDim.m_cDelta.SetFromXY(m_ptSecond.m_fX - m_ptVertex.m_fX, m_ptSecond.m_fY - m_ptVertex.m_fY);
		baseDim.m_cDelta.SetFromDegree(baseDim.m_cDelta.GetDegree() - baseDim.m_cAlfa.GetDegree());

		CCAD_Angle delta;
		delta.SetFromXY(m_ptDimArc.m_fX - m_ptVertex.m_fX, m_ptDimArc.m_fY - m_ptVertex.m_fY);
		delta.SetFromDegree(delta.GetDegree() - baseDim.m_cAlfa.GetDegree());
		if(baseDim.m_cDelta.GetDegree() * delta.GetDegree() < 0)
		{
			delta.SetFromDegree((CAD_FLOAT) 360 - delta.GetAbsDegree());
			if(baseDim.m_cDelta.GetDegree() < 0)
				delta.Invert();
		}

		if(delta.GetAbsDegree() > baseDim.m_cDelta.GetAbsDegree())
		{
			baseDim.m_cDelta.SetFromDegree((CAD_FLOAT) 360 - baseDim.m_cDelta.GetAbsDegree());
			if(delta.GetDegree() > 0)
				baseDim.m_cDelta.Invert();
		}
	}
}

void CCAD_GemAngularDim::CheckInOutState(CCAD_GemArc& baseDim, const CCAD_Rect& boundTxt)
{
	CCAD_Angle angle;
	CCAD_Coord3D coord;
	angle = baseDim.m_cAlfa;
	baseDim.CountCirclePt(coord.m_cOrg, angle.sin(), angle.cos());
	baseDim.m_cBcs.ConvToBase(coord.m_cOrg);
	angle.RotateAngle(baseDim.m_cDelta);
	baseDim.CountCirclePt(coord.m_cX, angle.sin(), angle.cos());
	baseDim.m_cBcs.ConvToBase(coord.m_cX);

	CAD_FLOAT fDimLength = coord.m_cOrg.CountLength(coord.m_cX);
	coord.SetCoord(coord.m_cOrg, coord.m_cX - coord.m_cOrg);

	CCAD_Coord3D coordTxt;
	if(m_anglText.IsZero())
	{
		if(m_pStyle->m_sDimText.sState.align == DIM_TEXT_ALIGN_HORZ)
			coord.ThisToChild(&coordTxt);
	}
	else
	{
		coordTxt.ZRotate(m_anglText);
		coord.ThisToChild(&coordTxt);
	}

	CAD_FLOAT fTxtWidth, fTxtHeight;
	CCAD_Rect boundTxtParallel(boundTxt);
	coordTxt.ConvToBase(boundTxtParallel);
	fTxtWidth = boundTxtParallel.Width() + 2 * m_pStyle->m_sDimText.fGap;
	fTxtHeight = boundTxtParallel.Height() + 2 * m_pStyle->m_sDimText.fGap;

	CAD_FLOAT fArrowWidth = 2 * m_pStyle->m_sDimArrow.fSize;

	// reset all in/out state flags
	m_nOption &= ~DIMTEXTORARROW_INOUTSTATEFIELD;

	if((m_nOption & DIMTEXTPLACE_NODEFAULT) == 0)
	{	// text place default location
		if(baseDim.m_cDelta.GetAbsDegree() < 180)
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
			m_nOption |= DIMTEXTORARROW_INOUTSTATEFIELD;
	}
	else
	{
		if(baseDim.m_cDelta.GetAbsDegree() < 180)
		{
			if(fDimLength > fArrowWidth)
				m_nOption |= DIMARROWPLACE_INSIDE;
		}
		else
			m_nOption |= DIMARROWPLACE_INSIDE;
	}
}

BOOL CCAD_GemAngularDim::PtInBaseDimArc(CCAD_Vector pt, CCAD_GemArc& baseDim)
{
	CCAD_Angle delta;
	delta.SetFromXY(pt.m_fX, pt.m_fY);
	delta.SetFromDegree(delta.GetDegree() - baseDim.m_cAlfa.GetDegree());
	if(baseDim.m_cDelta.GetDegree() * delta.GetDegree() < 0)
	{
		delta.SetFromDegree((CAD_FLOAT) 360 - delta.GetAbsDegree());
		if(baseDim.m_cDelta.GetDegree() < 0)
			delta.Invert();
	}

	if(delta.GetAbsDegree() > baseDim.m_cDelta.GetAbsDegree())
		return FALSE;
	else
		return TRUE;
}

void CCAD_GemAngularDim::DrawExtensionLines(CCAD_GemArc& baseDim)
{
	CCAD_LineEntity ext(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
	CCAD_GemLine* pLine = (CCAD_GemLine*) ext.GetGemEntity();

	CCAD_Angle angle = baseDim.m_cAlfa;
	pLine->m_cSp = m_ptFirst;
	baseDim.CountCirclePt(pLine->m_cEp, angle.sin(), angle.cos());
	baseDim.m_cBcs.ConvToBase(pLine->m_cEp);
	pLine->m_cBcs.SetCoord(pLine->m_cSp, pLine->m_cEp - pLine->m_cSp);
	pLine->m_cBcs.ConvFromBase(pLine->m_cSp);
	pLine->m_cBcs.ConvFromBase(pLine->m_cEp);
	pLine->m_cSp.m_fX += m_pStyle->m_sDimExt.fBase;
	pLine->m_cEp.m_fX += m_pStyle->m_sDimExt.fExt;
	if((m_pStyle->m_sDimExt.wShow & DIM_LINE_SHOW_FIRST) != 0)
		DrawElement(DIMELEMENT_EXTENSION, &ext);

	angle.RotateAngle(baseDim.m_cDelta);
	pLine->m_cSp = m_ptSecond;
	baseDim.CountCirclePt(pLine->m_cEp, angle.sin(), angle.cos());
	baseDim.m_cBcs.ConvToBase(pLine->m_cEp);
	pLine->m_cBcs.SetCoord(pLine->m_cSp, pLine->m_cEp - pLine->m_cSp);
	pLine->m_cBcs.ConvFromBase(pLine->m_cSp);
	pLine->m_cBcs.ConvFromBase(pLine->m_cEp);
	pLine->m_cSp.m_fX += m_pStyle->m_sDimExt.fBase;
	pLine->m_cEp.m_fX += m_pStyle->m_sDimExt.fExt;
	if((m_pStyle->m_sDimExt.wShow & DIM_LINE_SHOW_SECOND) != 0)
		DrawElement(DIMELEMENT_EXTENSION, &ext);
}

void CCAD_GemAngularDim::DrawDimensionLines(CCAD_GemArc& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt)
{
	if((m_nOption & DIMTEXTPLACE_NODEFAULT) != 0)
	{
		pTxtCoord->m_cOrg = m_ptText;

		CCAD_ArcEntity arc(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
		CCAD_GemArc* pArc = (CCAD_GemArc*) arc.GetGemEntity();
		baseDim.MemCopyTo(pArc);

		if((m_nOption & DIMARROWPLACE_INSIDE) != 0)
		{
			if(m_pStyle->m_sDimLine.wShow != 0)
			{
				CAD_FLOAT fExt1st = 0;
				CAD_FLOAT fExt2nd = 0;
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
				{
					if(IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
						fExt1st = -m_pStyle->m_sDimLine.fExt;
					else
						fExt1st = m_pStyle->m_sDimArrow.fSize;
				}
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
				{
					if(IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
						fExt2nd = -m_pStyle->m_sDimLine.fExt;
					else
						fExt2nd = m_pStyle->m_sDimArrow.fSize;
				}
				CCAD_Angle angle;
				angle.SetFromAsin(2 * baseDim.m_fR, (CAD_FLOAT) fabs((double) fExt1st));
				angle *= 2;
				if(fExt1st < 0)
					angle.Invert();
				if(baseDim.m_cDelta.GetDegree() < 0)
					angle.Invert();
				pArc->m_cAlfa.RotateAngle(angle);
				angle.SetFromAsin(2 * baseDim.m_fR, (CAD_FLOAT) fabs((double) (fExt1st + fExt2nd)));
				angle *= 2;
				if((fExt1st + fExt2nd) < 0)
					angle.Invert();
				if(baseDim.m_cDelta.GetDegree() < 0)
					angle.Invert();
				pArc->m_cDelta.SetFromDegree(pArc->m_cDelta.GetDegree() - angle.GetDegree());
				DrawElement(DIMELEMENT_DIMENSION, &arc);
			}
		}
		else
		{
			if(m_pStyle->m_sDimText.sState.always != 0)
				DrawElement(DIMELEMENT_DIMENSION, &arc);

			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
			{
				pArc->m_cAlfa = baseDim.m_cAlfa;
				pArc->m_cDelta.SetFromAsin(2 * baseDim.m_fR, m_pStyle->m_sDimArrow.fSize);
				pArc->m_cDelta *= 2;
				if(baseDim.m_cDelta.GetDegree() > 0)
					pArc->m_cDelta.Invert();
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
					pArc->m_cAlfa.RotateAngle(pArc->m_cDelta);
				DrawElement(DIMELEMENT_DIMENSION, &arc);
			}
			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
			{
				pArc->m_cAlfa = baseDim.m_cAlfa;
				pArc->m_cAlfa.RotateAngle(baseDim.m_cDelta);
				pArc->m_cDelta.SetFromAsin(2 * baseDim.m_fR, m_pStyle->m_sDimArrow.fSize);
				pArc->m_cDelta *= 2;
				if(baseDim.m_cDelta.GetDegree() < 0)
					pArc->m_cDelta.Invert();
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
					pArc->m_cAlfa.RotateAngle(pArc->m_cDelta);
				DrawElement(DIMELEMENT_DIMENSION, &arc);
			}
		}

		return;
	}

	if((m_nOption & DIMTEXTPLACE_INSIDE) != 0)
	{
		CCAD_Angle angle = baseDim.m_cAlfa;
		CCAD_Angle delta = baseDim.m_cDelta;
		delta /= 2;
		angle.RotateAngle(delta);
		baseDim.CountCirclePt(m_ptText, angle.sin(), angle.cos());

		if(m_pStyle->m_sDimText.sState.horz != DIM_TEXT_HORZ_CENTER)
		{
			CAD_FLOAT fXOff = 0;
			if(m_pStyle->m_sDimText.sState.vert == DIM_TEXT_VERT_CENTER)
				fXOff = m_pStyle->m_sDimArrow.fSize;
			CCAD_Angle dltArrow;
			dltArrow.SetFromAsin(baseDim.m_fR, fXOff / 2);
			dltArrow *= 2;

			CAD_FLOAT fTxtWidth;
			CCAD_Angle dltTxt;
			fTxtWidth = boundTxt.Width() + 2 * m_pStyle->m_sDimText.fGap;
			dltTxt.SetFromAsin(baseDim.m_fR, fTxtWidth / 2);

			angle.SetFromDegree(dltArrow.GetDegree() + dltTxt.GetDegree());

			CCAD_Angle anglTxtPt = baseDim.m_cAlfa;
			if(m_pStyle->m_sDimText.sState.horz == DIM_TEXT_HORZ_EXT1)
			{
				if(baseDim.m_cDelta.GetDegree() < 0)
					angle.Invert();
			}
			else
			{
				anglTxtPt.RotateAngle(baseDim.m_cDelta);
				if(baseDim.m_cDelta.GetDegree() > 0)
					angle.Invert();
			}
			anglTxtPt.RotateAngle(angle);
			baseDim.CountCirclePt(m_ptText, anglTxtPt.sin(), anglTxtPt.cos());
		}

		DrawTextInsidePlace(baseDim, pTxtCoord, boundTxt);
	}
	else
	{
		if(m_pStyle->m_sDimText.sState.place != DIM_TEXT_PLACE_BESIDE)
		{
			CCAD_Angle angle = baseDim.m_cAlfa;
			CCAD_Angle delta = baseDim.m_cDelta;
			delta /= 2;
			angle.RotateAngle(delta);
			CCAD_Coord3D coord;
			baseDim.CountCirclePt(coord.m_cOrg, angle.sin(), angle.cos());
			coord.SetCoord(coord.m_cOrg, coord.m_cOrg);

			m_ptText.m_fX = m_ptVertex.CountLength(m_ptSecond) + m_pStyle->m_sDimArrow.fSize - baseDim.m_fR;
			m_ptText.m_fY = 0;
			coord.ConvToBase(m_ptText);

			DrawTextOutsidePlaceNoBeside(baseDim, pTxtCoord, boundTxt);
		}
		else
		{
			CCAD_Angle anglTxtPt = baseDim.m_cAlfa;
			CCAD_Angle delta;
			delta.SetFromAsin(baseDim.m_fR, m_pStyle->m_sDimArrow.fSize / 2);
			delta *= 2;
			if(m_pStyle->m_sDimText.sState.horz == DIM_TEXT_HORZ_EXT1)
			{
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
				{
					if((m_nOption & DIMARROWPLACE_INSIDE) == 0)
					{
						if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst) && (m_pStyle->m_sDimText.sState.arrow == 0))
						{
							CCAD_Angle angle;
							angle.SetFromAsin(baseDim.m_fR, m_pStyle->m_sDimArrow.fSize / 2);
							angle *= 2;

							delta.SetFromDegree(delta.GetDegree() + angle.GetDegree());
						}
					}
				}
				if(baseDim.m_cDelta.GetDegree() > 0)
					delta.Invert();
			}
			else
			{
				anglTxtPt.RotateAngle(baseDim.m_cDelta);
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
				{
					if((m_nOption & DIMARROWPLACE_INSIDE) == 0)
					{
						if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond) && (m_pStyle->m_sDimText.sState.arrow == 0))
						{
							CCAD_Angle angle;
							angle.SetFromAsin(baseDim.m_fR, m_pStyle->m_sDimArrow.fSize / 2);
							angle *= 2;

							delta.SetFromDegree(delta.GetDegree() + angle.GetDegree());
						}
					}
				}
				if(baseDim.m_cDelta.GetDegree() < 0)
					delta.Invert();
			}
			anglTxtPt.RotateAngle(delta);
			baseDim.CountCirclePt(m_ptText, anglTxtPt.sin(), anglTxtPt.cos());

			DrawTextOutsidePlaceBeside(baseDim, pTxtCoord, boundTxt);
		}
	}

	if(m_pStyle->m_sDimText.sState.vert != DIM_TEXT_VERT_CENTER)
	{
		CAD_FLOAT fTxtWidth, fTxtHeight;
		GetTextArrayBoundsUnderAlignment(pTxtCoord, boundTxt, fTxtWidth, fTxtHeight);

		BOOL bHorz = FALSE;
		if(m_pStyle->m_sDimText.sState.align == DIM_TEXT_ALIGN_HORZ)
			bHorz = TRUE;
		else
			if((m_pStyle->m_sDimText.sState.align == DIM_TEXT_ALIGN_ISO) && ((m_nOption & DIMTEXTPLACE_INSIDE) != 0))
				bHorz = TRUE;

		if(!bHorz)
		{
			CCAD_Vector ptTxt(m_ptText);
			CCAD_Coord3D coord;
			coord.SetCoord(CCAD_Vector(0, 0, 0), m_ptText);
			coord.ConvFromBase(ptTxt);
			if((m_ptText.m_fY < 0) && (m_pStyle->m_sDimText.sState.vert == DIM_TEXT_VERT_ABOVE))
				ptTxt.m_fX -= fTxtHeight / 2;
			else
				ptTxt.m_fX += fTxtHeight / 2;
			coord.ConvToBase(ptTxt);
			CCAD_Vector off;
			off = ptTxt - m_ptText;
			pTxtCoord->m_cOrg += off;
		}
		else
		{
			if((m_ptText.m_fY < 0) && (m_pStyle->m_sDimText.sState.vert == DIM_TEXT_VERT_OUTSIDE))
				pTxtCoord->m_cOrg.m_fY -= fTxtHeight / 2;
			else
				pTxtCoord->m_cOrg.m_fY += fTxtHeight / 2;
		}
	}
}

void CCAD_GemAngularDim::DrawArrowHeaders(CCAD_GemArc& baseDim)
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
			if(IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
			{
				CCAD_Angle angle(baseDim.m_cAlfa);
				pArrowCoord->SetCoord(CCAD_Vector(0, 0, 0), CCAD_Vector(1, 0, 0));
				pArrowCoord->ZRotate(angle);
				if(baseDim.m_cDelta.GetDegree() < 0)
					pArrowCoord->ZRotate(CCAD_Angle((short) -90));
				else
					pArrowCoord->ZRotate(CCAD_Angle((short) 90));
				baseDim.CountCirclePt(pArrowCoord->m_cOrg, angle.sin(), angle.cos());
			}
			else
			{
				CCAD_Angle delta;
				delta.SetFromAsin(2 * baseDim.m_fR, m_pStyle->m_sDimArrow.fSize);
				delta *= 2;
				if((m_nOption & DIMARROWPLACE_INSIDE) != 0)
				{
					if(baseDim.m_cDelta.GetDegree() < 0)
						delta.Invert();
				}
				else
				{
					if(baseDim.m_cDelta.GetDegree() > 0)
						delta.Invert();
				}
				CCAD_Angle angle = baseDim.m_cAlfa;
				baseDim.CountCirclePt(pArrowCoord->m_cOrg, angle.sin(), angle.cos());
				angle.RotateAngle(delta);
				baseDim.CountCirclePt(pArrowCoord->m_cX, angle.sin(), angle.cos());
				pArrowCoord->SetCoord(pArrowCoord->m_cOrg, pArrowCoord->m_cX - pArrowCoord->m_cOrg);
			}
			pArrowCoord->ZRotate(CCAD_Angle((short) 180));
			baseDim.m_cBcs.ChildToThis(pArrowCoord);
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
			if(IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
			{
				CCAD_Angle angle(baseDim.m_cAlfa);
				angle.RotateAngle(baseDim.m_cDelta);
				pArrowCoord->SetCoord(CCAD_Vector(0, 0, 0), CCAD_Vector(1, 0, 0));
				pArrowCoord->ZRotate(angle);
				if(baseDim.m_cDelta.GetDegree() < 0)
					pArrowCoord->ZRotate(CCAD_Angle((short) 90));
				else
					pArrowCoord->ZRotate(CCAD_Angle((short) +90));
				baseDim.CountCirclePt(pArrowCoord->m_cOrg, angle.sin(), angle.cos());
			}
			else
			{
				CCAD_Angle delta;
				delta.SetFromAsin(2 * baseDim.m_fR, m_pStyle->m_sDimArrow.fSize);
				delta *= 2;
				if((m_nOption & DIMARROWPLACE_INSIDE) != 0)
				{
					if(baseDim.m_cDelta.GetDegree() > 0)
						delta.Invert();
				}
				else
				{
					if(baseDim.m_cDelta.GetDegree() < 0)
						delta.Invert();
				}
				CCAD_Angle angle = baseDim.m_cAlfa;
				angle.RotateAngle(baseDim.m_cDelta);
				baseDim.CountCirclePt(pArrowCoord->m_cOrg, angle.sin(), angle.cos());
				angle.RotateAngle(delta);
				baseDim.CountCirclePt(pArrowCoord->m_cX, angle.sin(), angle.cos());
				pArrowCoord->SetCoord(pArrowCoord->m_cOrg, pArrowCoord->m_cX - pArrowCoord->m_cOrg);
			}
			pArrowCoord->ZRotate(CCAD_Angle((short) 180));
			baseDim.m_cBcs.ChildToThis(pArrowCoord);
			if ( m_pStyle->m_sDimArrow.fSize > CAD_ZERO )
				pArrowCoord->m_fScale = m_pStyle->m_sDimArrow.fSize;
			strcpy(arrow.m_cGemBlock.m_szName, pArrowBlock->m_szName);
			DrawElement(DIMELEMENT_DIMENSION, &arrow);
		}
	}
}

void CCAD_GemAngularDim::DrawTextInsidePlace(CCAD_GemArc& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt)
{
	CAD_FLOAT fTxtWidth, fTxtHeight;
	GetTextArrayBoundsUnderAlignment(pTxtCoord, boundTxt, fTxtWidth, fTxtHeight);

	pTxtCoord->m_cOrg = m_ptText;
	CCAD_ArcEntity arc(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
	CCAD_GemArc* pArc = (CCAD_GemArc*) arc.GetGemEntity();
	baseDim.MemCopyTo(pArc);

	if(m_pStyle->m_sDimText.sState.vert == DIM_TEXT_VERT_CENTER)
	{
		CCAD_Angle anglTxtPt;
		anglTxtPt.SetFromXY(m_ptText.m_fX, m_ptText.m_fY);
		CCAD_Angle anglTxtWidth;
		anglTxtWidth.SetFromXY(baseDim.m_fR, fTxtWidth / 2);
		CCAD_Angle anglTxtStart(anglTxtPt);
		CCAD_Angle anglTxtEnd(anglTxtPt);
		if(baseDim.m_cDelta.GetDegree() < 0)
		{
			anglTxtStart.RotateAngle(anglTxtWidth);
			anglTxtWidth.Invert();
			anglTxtEnd.RotateAngle(anglTxtWidth);
		}
		else
		{
			anglTxtEnd.RotateAngle(anglTxtWidth);
			anglTxtWidth.Invert();
			anglTxtStart.RotateAngle(anglTxtWidth);
		}

		if((m_nOption & DIMARROWPLACE_INSIDE) != 0)
		{
			CCAD_Angle angle;
			angle.SetFromDegree(baseDim.m_cDelta.GetAbsDegree());
			angle /= 2;
			CAD_FLOAT fDimLength = 2 * baseDim.m_fR * angle.tan();
			if(fDimLength > fTxtWidth)
			{	// dim line break
				CAD_FLOAT fExt = 0;
				CCAD_Angle angle;
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
				{
					if(IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
						fExt = -m_pStyle->m_sDimLine.fExt;
					else
						fExt = m_pStyle->m_sDimArrow.fSize;
					angle.SetFromAsin(2 * baseDim.m_fR, (CAD_FLOAT) fabs((double) fExt));
					angle *= 2;
					if(fExt < 0)
						angle.Invert();
					if(baseDim.m_cDelta.GetDegree() < 0)
						angle.Invert();
					pArc->m_cAlfa = baseDim.m_cAlfa;
					pArc->m_cAlfa.RotateAngle(angle);
					anglTxtStart.Delta(pArc->m_cAlfa, pArc->m_cDelta);
					if(baseDim.m_cDelta.GetDegree() * pArc->m_cDelta.GetDegree() < 0)
						pArc->m_cDelta.Invert();
					DrawElement(DIMELEMENT_DIMENSION, &arc);
				}
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
				{
					if(IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
						fExt = m_pStyle->m_sDimLine.fExt;
					else
						fExt = -m_pStyle->m_sDimArrow.fSize;
					angle.SetFromAsin(2 * baseDim.m_fR, (CAD_FLOAT) fabs((double) fExt));
					angle *= 2;
					if(fExt < 0)
						angle.Invert();
					if(baseDim.m_cDelta.GetDegree() < 0)
						angle.Invert();
					pArc->m_cAlfa = anglTxtEnd;
					CCAD_Angle anglEnd = baseDim.m_cAlfa;
					anglEnd.RotateAngle(baseDim.m_cDelta);
					anglEnd.RotateAngle(angle);
					anglEnd.Delta(pArc->m_cAlfa, pArc->m_cDelta);
					if(baseDim.m_cDelta.GetDegree() * pArc->m_cDelta.GetDegree() < 0)
						pArc->m_cDelta.Invert();
					DrawElement(DIMELEMENT_DIMENSION, &arc);
				}
			}
		}
		else
		{
			if(m_pStyle->m_sDimText.sState.always != 0)
			{
				pArc->m_cAlfa = baseDim.m_cAlfa;
				anglTxtStart.Delta(pArc->m_cAlfa, pArc->m_cDelta);
				if(baseDim.m_cDelta.GetDegree() * pArc->m_cDelta.GetDegree() < 0)
					pArc->m_cDelta.Invert();
				DrawElement(DIMELEMENT_DIMENSION, &arc);
				pArc->m_cAlfa = anglTxtEnd;
				CCAD_Angle anglEnd = baseDim.m_cAlfa;
				anglEnd.RotateAngle(baseDim.m_cDelta);
				anglEnd.Delta(pArc->m_cAlfa, pArc->m_cDelta);
				if(baseDim.m_cDelta.GetDegree() * pArc->m_cDelta.GetDegree() < 0)
					pArc->m_cDelta.Invert();
				DrawElement(DIMELEMENT_DIMENSION, &arc);
			}

			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
			{
				pArc->m_cAlfa = baseDim.m_cAlfa;
				pArc->m_cDelta.SetFromAsin(2 * baseDim.m_fR, m_pStyle->m_sDimArrow.fSize);
				pArc->m_cDelta *= 2;
				if(baseDim.m_cDelta.GetDegree() > 0)
					pArc->m_cDelta.Invert();
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
					pArc->m_cAlfa.RotateAngle(pArc->m_cDelta);
				DrawElement(DIMELEMENT_DIMENSION, &arc);
			}
			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
			{
				pArc->m_cAlfa = baseDim.m_cAlfa;
				pArc->m_cAlfa.RotateAngle(baseDim.m_cDelta);
				pArc->m_cDelta.SetFromAsin(2 * baseDim.m_fR, m_pStyle->m_sDimArrow.fSize);
				pArc->m_cDelta *= 2;
				if(baseDim.m_cDelta.GetDegree() < 0)
					pArc->m_cDelta.Invert();
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
					pArc->m_cAlfa.RotateAngle(pArc->m_cDelta);
				DrawElement(DIMELEMENT_DIMENSION, &arc);
			}
		}
	}
	else
	{
		if((m_nOption & DIMARROWPLACE_INSIDE) != 0)
		{
			if(m_pStyle->m_sDimLine.wShow != 0)
			{
				CAD_FLOAT fExt1st = 0;
				CAD_FLOAT fExt2nd = 0;
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
				{
					if(IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
						fExt1st = -m_pStyle->m_sDimLine.fExt;
					else
						fExt1st = m_pStyle->m_sDimArrow.fSize;
				}
				if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
				{
					if(IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
						fExt2nd = -m_pStyle->m_sDimLine.fExt;
					else
						fExt2nd = m_pStyle->m_sDimArrow.fSize;
				}
				CCAD_Angle angle;
				angle.SetFromAsin(2 * baseDim.m_fR, (CAD_FLOAT) fabs((double) fExt1st));
				angle *= 2;
				if(fExt1st < 0)
					angle.Invert();
				if(baseDim.m_cDelta.GetDegree() < 0)
					angle.Invert();
				pArc->m_cAlfa.RotateAngle(angle);
				angle.SetFromAsin(2 * baseDim.m_fR, (CAD_FLOAT) fabs((double) (fExt1st + fExt2nd)));
				angle *= 2;
				if((fExt1st + fExt2nd) < 0)
					angle.Invert();
				if(baseDim.m_cDelta.GetDegree() < 0)
					angle.Invert();
				pArc->m_cDelta.SetFromDegree(pArc->m_cDelta.GetDegree() - angle.GetDegree());
				DrawElement(DIMELEMENT_DIMENSION, &arc);
			}
		}
		else
		{
			if(m_pStyle->m_sDimText.sState.always != 0)
				DrawElement(DIMELEMENT_DIMENSION, &arc);

			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
			{
				pArc->m_cAlfa = baseDim.m_cAlfa;
				pArc->m_cDelta.SetFromAsin(2 * baseDim.m_fR, m_pStyle->m_sDimArrow.fSize);
				pArc->m_cDelta *= 2;
				if(baseDim.m_cDelta.GetDegree() > 0)
					pArc->m_cDelta.Invert();
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
					pArc->m_cAlfa.RotateAngle(pArc->m_cDelta);
				DrawElement(DIMELEMENT_DIMENSION, &arc);
			}
			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
			{
				pArc->m_cAlfa = baseDim.m_cAlfa;
				pArc->m_cAlfa.RotateAngle(baseDim.m_cDelta);
				pArc->m_cDelta.SetFromAsin(2 * baseDim.m_fR, m_pStyle->m_sDimArrow.fSize);
				pArc->m_cDelta *= 2;
				if(baseDim.m_cDelta.GetDegree() < 0)
					pArc->m_cDelta.Invert();
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
					pArc->m_cAlfa.RotateAngle(pArc->m_cDelta);
				DrawElement(DIMELEMENT_DIMENSION, &arc);
			}
		}
	}
}

void CCAD_GemAngularDim::DrawTextOutsidePlaceBeside(CCAD_GemArc& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt)
{
	CAD_FLOAT fTxtWidth, fTxtHeight;
	GetTextArrayBoundsUnderAlignment(pTxtCoord, boundTxt, fTxtWidth, fTxtHeight);

	pTxtCoord->m_cOrg = m_ptText;
	CCAD_ArcEntity arc(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
	CCAD_GemArc* pArc = (CCAD_GemArc*) arc.GetGemEntity();
	baseDim.MemCopyTo(pArc);

	BOOL bExt2ndDir = TRUE;
	CCAD_Angle anglTxtPt;
	anglTxtPt.SetFromXY(m_ptText.m_fX, m_ptText.m_fY);
	CCAD_Angle angle;
	angle = baseDim.m_cAlfa;
	CCAD_Angle anglFrom1st;
	angle.Delta(anglTxtPt, anglFrom1st);
	angle.RotateAngle(baseDim.m_cDelta);
	CCAD_Angle anglFrom2nd;
	angle.Delta(anglTxtPt, anglFrom2nd);
	if(anglFrom1st.GetAbsDegree() < anglFrom2nd.GetAbsDegree())
		bExt2ndDir = FALSE;

	if((m_nOption & DIMARROWPLACE_INSIDE) != 0)
	{
		if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
		{
			CAD_FLOAT fOff = 0;
			if(IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
				fOff = -m_pStyle->m_sDimLine.fExt;
			else
				fOff = m_pStyle->m_sDimArrow.fSize;

			angle.SetFromAsin(2 * baseDim.m_fR, fOff);
			angle *= 2;
			if(fOff < 0)
				angle.Invert();
			if(baseDim.m_cDelta.GetDegree() < 0)
				angle.Invert();

			pArc->m_cAlfa = baseDim.m_cAlfa;
			pArc->m_cAlfa.RotateAngle(angle);
		}
		if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
		{
			CAD_FLOAT fOff = 0;
			if(IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
				fOff = m_pStyle->m_sDimLine.fExt;
			else
				fOff = -m_pStyle->m_sDimArrow.fSize;

			angle.SetFromAsin(2 * baseDim.m_fR, fOff);
			angle *= 2;
			if(fOff < 0)
				angle.Invert();
			if(baseDim.m_cDelta.GetDegree())
				angle.Invert();

			pArc->m_cDelta = baseDim.m_cAlfa;
			pArc->m_cDelta.RotateAngle(baseDim.m_cDelta);
			pArc->m_cDelta.RotateAngle(angle);
			angle = pArc->m_cDelta;
			angle.Delta(pArc->m_cAlfa, pArc->m_cDelta);
			if(baseDim.m_cDelta.GetDegree() * pArc->m_cDelta.GetDegree() < 0)
				pArc->m_cDelta.Invert();
		}
		DrawElement(DIMELEMENT_DIMENSION, &arc);
	}
	else
	{
		if(m_pStyle->m_sDimText.sState.always != 0)
			DrawElement(DIMELEMENT_DIMENSION, &arc);

		if(!bExt2ndDir)		// at ext1
		{
			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
			{
				pArc->m_cAlfa = baseDim.m_cAlfa;
				pArc->m_cAlfa.RotateAngle(baseDim.m_cDelta);
				pArc->m_cDelta.SetFromAsin(2 * baseDim.m_fR, m_pStyle->m_sDimArrow.fSize);
				if(baseDim.m_cDelta.GetDegree() < 0)
					pArc->m_cDelta.Invert();
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
					pArc->m_cAlfa.RotateAngle(pArc->m_cDelta);
				DrawElement(DIMELEMENT_DIMENSION, &arc);
			}
		}
		else						// at ext2
		{
			if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
			{
				pArc->m_cAlfa = baseDim.m_cAlfa;
				pArc->m_cDelta.SetFromAsin(2 * baseDim.m_fR, m_pStyle->m_sDimArrow.fSize);
				if(baseDim.m_cDelta.GetDegree() > 0)
					pArc->m_cDelta.Invert();
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
					pArc->m_cAlfa.RotateAngle(pArc->m_cDelta);
				DrawElement(DIMELEMENT_DIMENSION, &arc);
			}
		}
	}

	if(!bExt2ndDir)		// at ext1
	{
		pArc->m_cAlfa = baseDim.m_cAlfa;
		if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
		{
			if((m_nOption & DIMARROWPLACE_INSIDE) == 0)
			{
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
				{
					angle.SetFromAsin(2 * baseDim.m_fR, m_pStyle->m_sDimArrow.fSize);
					angle *= 2;
					if(baseDim.m_cDelta.GetDegree() > 0)
						angle.Invert();
					pArc->m_cAlfa.RotateAngle(angle);
				}
			}
		}
		anglTxtPt.Delta(pArc->m_cAlfa, pArc->m_cDelta);
		if(baseDim.m_cDelta.GetDegree() > 0)
			pArc->m_cDelta.Invert();
	}
	else						// at ext2
	{
		pArc->m_cAlfa = baseDim.m_cAlfa;
		pArc->m_cAlfa.RotateAngle(baseDim.m_cDelta);
		if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
		{
			if((m_nOption & DIMARROWPLACE_INSIDE) == 0)
			{
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
				{
					angle.SetFromAsin(2 * baseDim.m_fR, m_pStyle->m_sDimArrow.fSize);
					angle *= 2;
					if(baseDim.m_cDelta.GetDegree() < 0)
						angle.Invert();
					pArc->m_cAlfa.RotateAngle(angle);
				}
			}
		}
		anglTxtPt.Delta(pArc->m_cAlfa, pArc->m_cDelta);
		if(baseDim.m_cDelta.GetDegree() < 0)
			pArc->m_cDelta.Invert();
	}
	DrawElement(DIMELEMENT_DIMENSION, &arc);

	CCAD_LineEntity line(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
	CCAD_GemLine* pLine = (CCAD_GemLine*) line.GetGemEntity();

	pLine->m_cBcs = *pTxtCoord;
	pLine->m_cBcs.m_cOrg = m_ptText;
	pLine->m_cSp = CCAD_Point(0, 0, 0);
	pLine->m_cEp = CCAD_Point(0, 0, 0);
	if(m_pStyle->m_sDimText.sState.vert == DIM_TEXT_VERT_CENTER)
	{
		pLine->m_cEp.m_fX = m_pStyle->m_sDimArrow.fSize;

		pTxtCoord->m_cOrg = pLine->m_cEp;
		pLine->m_cBcs.ConvToBase(pTxtCoord->m_cOrg);
		CCAD_Point org(0, -boundTxt.Height() / 2, 0);
		pTxtCoord->ConvToBase(org);
		pTxtCoord->m_cOrg = org;
	}
	else
		pLine->m_cEp.m_fX = fTxtWidth;
	DrawElement(DIMELEMENT_DIMENSION, &line);
}

void CCAD_GemAngularDim::DrawTextOutsidePlaceNoBeside(CCAD_GemArc& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt)
{
	CAD_FLOAT fTxtWidth, fTxtHeight;
	GetTextArrayBoundsUnderAlignment(pTxtCoord, boundTxt, fTxtWidth, fTxtHeight);

	pTxtCoord->m_cOrg = m_ptText;
	CCAD_ArcEntity arc(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
	CCAD_GemArc* pArc = (CCAD_GemArc*) arc.GetGemEntity();
	baseDim.MemCopyTo(pArc);

	if((m_nOption & DIMARROWPLACE_INSIDE) != 0)
	{
		CCAD_Angle angle;
		if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
		{
			CAD_FLOAT fOff = 0;
			if(IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
				fOff = -m_pStyle->m_sDimLine.fExt;
			else
				fOff = m_pStyle->m_sDimArrow.fSize;

			angle.SetFromAsin(2 * baseDim.m_fR, fOff);
			angle *= 2;
			if(fOff < 0)
				angle.Invert();
			if(baseDim.m_cDelta.GetDegree() < 0)
				angle.Invert();

			pArc->m_cAlfa = baseDim.m_cAlfa;
			pArc->m_cAlfa.RotateAngle(angle);
		}
		if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
		{
			CAD_FLOAT fOff = 0;
			if(IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
				fOff = m_pStyle->m_sDimLine.fExt;
			else
				fOff = -m_pStyle->m_sDimArrow.fSize;

			angle.SetFromAsin(2 * baseDim.m_fR, fOff);
			angle *= 2;
			if(fOff < 0)
				angle.Invert();
			if(baseDim.m_cDelta.GetDegree())
				angle.Invert();

			pArc->m_cDelta = baseDim.m_cAlfa;
			pArc->m_cDelta.RotateAngle(baseDim.m_cDelta);
			pArc->m_cDelta.RotateAngle(angle);
			angle = pArc->m_cDelta;
			angle.Delta(pArc->m_cAlfa, pArc->m_cDelta);
			if(baseDim.m_cDelta.GetDegree() * pArc->m_cDelta.GetDegree() < 0)
				pArc->m_cDelta.Invert();
		}
		DrawElement(DIMELEMENT_DIMENSION, &arc);
	}
	else
	{
		if(m_pStyle->m_sDimText.sState.always != 0)
			DrawElement(DIMELEMENT_DIMENSION, &arc);

		if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_SECOND) != 0)
		{
			pArc->m_cAlfa = baseDim.m_cAlfa;
			pArc->m_cAlfa.RotateAngle(baseDim.m_cDelta);
			pArc->m_cDelta.SetFromAsin(2 * baseDim.m_fR, m_pStyle->m_sDimArrow.fSize);
			if(baseDim.m_cDelta.GetDegree() < 0)
				pArc->m_cDelta.Invert();
			if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nSecond))
				pArc->m_cAlfa.RotateAngle(pArc->m_cDelta);
			DrawElement(DIMELEMENT_DIMENSION, &arc);
		}
		if((m_pStyle->m_sDimLine.wShow & DIM_LINE_SHOW_FIRST) != 0)
		{
			pArc->m_cAlfa = baseDim.m_cAlfa;
			pArc->m_cDelta.SetFromAsin(2 * baseDim.m_fR, m_pStyle->m_sDimArrow.fSize);
			if(baseDim.m_cDelta.GetDegree() > 0)
				pArc->m_cDelta.Invert();
			if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nFirst))
				pArc->m_cAlfa.RotateAngle(pArc->m_cDelta);
			DrawElement(DIMELEMENT_DIMENSION, &arc);
		}
	}

	if(m_pStyle->m_sDimText.sState.place == DIM_TEXT_PLACE_LEADER)
	{
		CCAD_LineEntity line(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
		CCAD_GemLine* pLine = (CCAD_GemLine*) line.GetGemEntity();

		CCAD_Angle angle = baseDim.m_cAlfa;
		CCAD_Angle delta = baseDim.m_cDelta;
		delta /= 2;
		angle.RotateAngle(delta);
		baseDim.CountCirclePt(pLine->m_cBcs.m_cOrg, angle.sin(), angle.cos());

		pLine->m_cBcs.SetCoord(pLine->m_cBcs.m_cOrg, m_ptText - pLine->m_cBcs.m_cOrg);
		pLine->m_cSp = pLine->m_cBcs.m_cOrg;
		pLine->m_cEp = m_ptText;
		pLine->m_cBcs.ConvFromBase(pLine->m_cSp);
		pLine->m_cBcs.ConvFromBase(pLine->m_cEp);
		DrawElement(DIMELEMENT_DIMENSION, &line);

		pLine->m_cBcs = *pTxtCoord;
		pLine->m_cBcs.m_cOrg = m_ptText;
		pLine->m_cSp = CCAD_Point(0, 0, 0);
		pLine->m_cEp = CCAD_Point(0, 0, 0);
		if(m_pStyle->m_sDimText.sState.vert == DIM_TEXT_VERT_CENTER)
		{
			pLine->m_cEp.m_fX += m_pStyle->m_sDimArrow.fSize;

			pTxtCoord->m_cOrg = pLine->m_cEp;
			pLine->m_cBcs.ConvToBase(pTxtCoord->m_cOrg);
			CCAD_Point org(0, -boundTxt.Height() / 2, 0);
			pTxtCoord->ConvToBase(org);
			pTxtCoord->m_cOrg = org;
		}
		else
			pLine->m_cEp.m_fX += fTxtWidth;
		DrawElement(DIMELEMENT_DIMENSION, &line);
	}
}

void CCAD_GemAngularDim::GetTextArrayBoundsUnderAlignment(CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt,
														  CAD_FLOAT& fTxtWidth, CAD_FLOAT& fTxtHeight)
{
	GetTextDirect(pTxtCoord);

	CCAD_Rect boundTxtParallel(boundTxt);
	pTxtCoord->ConvToBase(boundTxtParallel);
	fTxtWidth = boundTxtParallel.Width() + 2 * m_pStyle->m_sDimText.fGap;
	if((m_pStyle->m_sDimText.sState.horz == DIM_TEXT_HORZ_OVEREXT1) ||
		(m_pStyle->m_sDimText.sState.horz == DIM_TEXT_HORZ_OVEREXT2))
	{
		fTxtWidth = 0;
	}
	fTxtHeight = boundTxtParallel.Height() + 2 * m_pStyle->m_sDimText.fGap;
}

void CCAD_GemAngularDim::GetTextDirect(CCAD_Coord3D* pTxtCoord)
{
	CCAD_Vector ptOrgTxt = pTxtCoord->m_cOrg;
	if(!m_anglText.IsZero())
	{
		pTxtCoord->SetCoord(CCAD_Vector(0, 0, 0), CCAD_Vector(1, 0, 0));
		pTxtCoord->ZRotate(m_anglText);
	}
	else
	{
		BOOL bHorz = FALSE;
		if(m_pStyle->m_sDimText.sState.align == DIM_TEXT_ALIGN_HORZ)
			bHorz = TRUE;
		else
			if((m_pStyle->m_sDimText.sState.align == DIM_TEXT_ALIGN_ISO) && ((m_nOption & DIMTEXTPLACE_INSIDE) != 0))
				bHorz = TRUE;

		if(!bHorz)
		{
			pTxtCoord->SetCoord(CCAD_Vector(0, 0, 0), m_ptText);
			pTxtCoord->ZRotate(CCAD_Angle((short) 90));
		}
		else
			pTxtCoord->SetCoord(CCAD_Vector(0, 0, 0), CCAD_Vector(1, 0, 0));
	}
	pTxtCoord->m_cOrg = ptOrgTxt;
}

void CCAD_GemAngularDim::ReDefineCtrlPts()
{
	m_cBcs.ConvToBase(m_ptVertex);
	m_cBcs.ConvToBase(m_ptFirst);
	m_cBcs.ConvToBase(m_ptSecond);
	m_cBcs.ConvToBase(m_ptText);
	m_cBcs.ConvToBase(m_ptDimArc);

	m_cBcs.m_cOrg = m_ptVertex;

	m_cBcs.ConvFromBase(m_ptVertex);
	m_cBcs.ConvFromBase(m_ptFirst);
	m_cBcs.ConvFromBase(m_ptSecond);
	m_cBcs.ConvFromBase(m_ptText);
	m_cBcs.ConvFromBase(m_ptDimArc);
}

void CCAD_GemAngularDim::GetBaseCouninueDim(BOOL bContinue, CCAD_GemDimension* pGetDim, const CCAD_Vector& ptExt2)
	// ptExt2 : define on the world
{
	CCAD_GemAngularDim::MemCopyTo(pGetDim);

	CCAD_GemAngularDim* pGemAngularDim = (CCAD_GemAngularDim*) pGetDim;

	pGemAngularDim->m_szName[0] = 0x00;
	pGemAngularDim->m_szText[0] = 0x00;

	CAD_FLOAT radius = m_ptVertex.CountLength(m_ptDimArc);

	pGemAngularDim->m_nOption &= ~DIMTEXTPLACE_NODEFAULT;
	if(bContinue)
		pGemAngularDim->m_ptFirst = m_ptSecond;
	else
		radius += m_pStyle->m_sDimLine.fBase;
	pGemAngularDim->m_ptSecond = ptExt2;
	pGemAngularDim->m_cBcs.ConvFromBase(pGemAngularDim->m_ptSecond);

	CCAD_Coord3D coord;
	coord.m_cOrg = pGemAngularDim->m_ptVertex;
	coord.m_cX.m_fX = (pGemAngularDim->m_ptFirst.m_fX + pGemAngularDim->m_ptSecond.m_fX) / 2;
	coord.m_cX.m_fY = (pGemAngularDim->m_ptFirst.m_fY + pGemAngularDim->m_ptSecond.m_fY) / 2;
	coord.m_cX.m_fZ = 0;
	coord.SetCoord(coord.m_cOrg, coord.m_cX - coord.m_cOrg);

	pGemAngularDim->m_ptText = CCAD_Vector(radius, 0, 0);
	coord.ConvToBase(pGemAngularDim->m_ptText);
	pGemAngularDim->m_ptDimArc = pGemAngularDim->m_ptText;
}
