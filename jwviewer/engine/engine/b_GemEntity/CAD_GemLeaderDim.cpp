#include "stdafx.h"
#include "CAD_GemLeaderDim.h"
#include "CAD_Doc.h"

CCAD_GemLeaderDim::CCAD_GemLeaderDim(CCAD_Doc * pDoc) : CCAD_GemDimension(pDoc)
{
	m_nPtCount = 0;
	m_pVertex = NULL;
}

CCAD_GemLeaderDim::~CCAD_GemLeaderDim()
{
	if(m_pVertex != NULL)
		delete []m_pVertex;
}

//---------------------------------------------
void CCAD_GemLeaderDim::Init()
{
	CCAD_GemDimension::Init();
	m_nPtCount = 0;
	m_pVertex = NULL;
}

//---------------------------------------------
void CCAD_GemLeaderDim::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_GemDimension::MemCopyTo(pGemEntity, pData1, pData2, pData3);

	CCAD_GemLeaderDim* pCopyTo = (CCAD_GemLeaderDim*) pGemEntity;
	pCopyTo->m_nPtCount = m_nPtCount;
	pCopyTo->m_pVertex = (CCAD_Vector*) pData1;
	if(pCopyTo->m_pVertex == NULL)
		pCopyTo->m_pVertex = new CCAD_Vector[m_nPtCount];
	int	i;
	for(i = 0; i < m_nPtCount; i++)
		pCopyTo->m_pVertex[i] = m_pVertex[i];
}

void CCAD_GemLeaderDim::Remove()
{
	m_nPtCount = 0;
	if(m_pVertex != NULL)
		delete []m_pVertex;
	m_pVertex = NULL;
}

BOOL CCAD_GemLeaderDim::Read(CCAD_File& cFile)
{
	CCAD_GemDimension::Read(cFile);

	cFile.ReadShort(&m_nPtCount);
	m_sSetting.bSpline = NO;
	if (m_nPtCount > 0){
		m_pVertex = new CCAD_Vector[m_nPtCount];
		if(m_pVertex == NULL)
		{
			m_nPtCount = 0;
			return FALSE;
		}
		int	i;
		for(i = 0; i < m_nPtCount; i++)
			m_pVertex[i].Read(cFile);
	}
	return TRUE;
}
void CCAD_GemLeaderDim::DrawDimension(BOOL blTmp)
{
	if(m_nPtCount < 2)
		return;
	ASSERT(m_pVertex != NULL);

	CCAD_Vector ptFirst(m_pVertex[0]);
	if(m_pStyle->m_sDimLine.wShow != 0)
	{
		BOOL bShow = TRUE;
		if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nLeader))
		{
			if(m_pVertex[0].CountLength(m_pVertex[1]) > 2 * m_pStyle->m_sDimArrow.fSize)
			{
				CCAD_Coord3D coord;
				coord.SetCoord(m_pVertex[0], m_pVertex[1] - m_pVertex[0]);

				ptFirst = m_pVertex[0];
				coord.ConvFromBase(ptFirst);
				if(!IsObliqueArrow(m_pStyle->m_sDimArrow.nLeader))
					ptFirst.m_fX += m_pStyle->m_sDimArrow.fSize;
				coord.ConvToBase(ptFirst);
			}
			else
				bShow = FALSE;
		}
		if (blTmp==TRUE)
			bShow = FALSE;
		if(bShow)
		{
			CCAD_BlockEntity arrow(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
			arrow.m_wPriority |= PRI_BRUSH_COLOR;
			arrow.m_wPriority |= PRI_BRUSH_PATTERN;
			arrow.m_wPriority |= PRI_PEN_COLOR;
			arrow.m_wPriority |= PRI_PEN_STYLE;
			arrow.m_wPriority |= PRI_PEN_THICK;
			arrow.m_wPriority |= PRI_PEN_SCALE;
			CCAD_Coord3D* pArrowCoord = &arrow.m_cGemBlock.m_cBcs;
			CCAD_Block* pArrowBlock = GetArrowHeadBlock(m_pStyle->m_sDimArrow.nLeader);
			if(pArrowBlock != NULL)
			{
				pArrowCoord->SetCoord(m_pVertex[0], m_pVertex[1] - m_pVertex[0]);
				pArrowCoord->ZRotate(CCAD_Angle((short) 180));
				if ( m_pStyle->m_sDimArrow.fSize > CAD_ZERO )
					pArrowCoord->m_fScale = m_pStyle->m_sDimArrow.fSize;
				strcpy(arrow.m_cGemBlock.m_szName, pArrowBlock->m_szName);
				DrawElement(DIMELEMENT_DIMENSION, &arrow);
			}
		}
	}

	BOOL bSpline = FALSE;
	if((m_sSetting.bSpline == YES) && (m_nPtCount > 2))
		bSpline = TRUE;

	short i;

	if(!bSpline)
	{
		CCAD_Pline2DEntity line(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
		CCAD_GemPline2D* pLine = (CCAD_GemPline2D*) line.GetGemEntity();
		pLine->m_pVertex = new CCAD_GemPvertex[m_nPtCount];
		if (pLine->m_pVertex)
		{
			pLine->m_nTot = m_nPtCount;
			pLine->m_pVertex[0].m_cPt = ptFirst;
			for(i = 1; i < m_nPtCount; i++)
				pLine->m_pVertex[i].m_cPt = m_pVertex[i];
			DrawElement(DIMELEMENT_DIMENSION, &line);
		}
	}
	else
	{
		CCAD_CurveEntity line(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
		CCAD_GemCurve* pLine = (CCAD_GemCurve*) line.GetGemEntity();
		pLine->m_bKind = CURVE_3SPLINE;
		pLine->m_pPt = new CCAD_Point[m_nPtCount];
		if (pLine->m_pPt)
		{
			pLine->m_nTot = m_nPtCount;
			pLine->m_pPt[0] = ptFirst;
			for(i = 1; i < m_nPtCount; i++)
				pLine->m_pPt[i] = m_pVertex[i];
			pLine->ThreeSplineTempVariables();
			DrawElement(DIMELEMENT_DIMENSION, &line);
		}
	}

	if((m_nOption & DIMTEXTPLACE_NODEFAULT) == 0)
	{
		m_ptText = m_pVertex[m_nPtCount - 1];
		if(m_pVertex[m_nPtCount - 1].m_fX - m_pVertex[m_nPtCount - 2].m_fX < 0)
			m_ptText.m_fX -= m_pStyle->m_sDimArrow.fSize;
		else
			m_ptText.m_fX += m_pStyle->m_sDimArrow.fSize;
	}

	if(m_szText[0] == '\0')
		return;

	CCAD_TextEntity txt(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
	CCAD_GemText* pText = (CCAD_GemText*) txt.GetGemEntity();
	pText->m_nStyle = m_pStyle->m_sDimText.nStyle;
	pText->m_pTextStyle = m_pTxtStyle;
	pText->m_bPosFlag = POS_LB;
	pText->m_fWidth = m_pStyle->m_sDimText.fWidth;
	pText->m_fHeight = m_pStyle->m_sDimText.fHeight;
	pText->m_cDirect = (short) 0;
	pText->m_fEmpty = 0;
	pText->m_nLength = strlen(m_szText);
	pText->m_pStr = new char[strlen(m_szText) + 1];
	strcpy(pText->m_pStr, m_szText);

	CCAD_TextEntity* ppText[1];
	ppText[0] = &txt;
	CCAD_Rect boundTxt;
	GetTextArrayBounds(1, ppText, boundTxt);

	pText->m_cBcs.m_cOrg = m_ptText;

	if(m_pVertex[m_nPtCount - 1].m_fX - m_pVertex[m_nPtCount - 2].m_fX < 0)
	{
		CCAD_Vector orgTxt(-(m_pStyle->m_sDimText.fGap + boundTxt.Width()), -boundTxt.Height() / 2, 0);
		pText->m_cBcs.ConvToBase(orgTxt);
		pText->m_cBcs.m_cOrg = orgTxt;
	}
	else
	{
		CCAD_Vector orgTxt(m_pStyle->m_sDimText.fGap, -boundTxt.Height() / 2, 0);
		pText->m_cBcs.ConvToBase(orgTxt);
		pText->m_cBcs.m_cOrg = orgTxt;
	}

	DrawDimensionText(1, ppText);
}
