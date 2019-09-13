#include "stdafx.h"
#include "CAD_Global.h"
#include "CAD_GemDimension.h"
#include "CAD_Doc.h"


CCAD_GemDimension::CCAD_GemDimension(CCAD_Doc * pDoc) : CCAD_GemBlock(pDoc)
{
	Init();
}

CCAD_GemDimension::~CCAD_GemDimension()
{
}

void CCAD_GemDimension::Init()
{
	CCAD_GemBlock::Init();

	m_nStyle = -1;
	m_pStyle = NULL;
	m_nOption = 0;
	m_pTxtStyle = NULL;
	m_szText[0] = '\0';
	m_anglText = 0;

	m_szDefaultText[0] = '\0';

	m_pDrawDC = NULL;
	m_pDimBlock = NULL;
}

//---------------------------------------------
void CCAD_GemDimension::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_GemBlock::MemCopyTo(pGemEntity, pData1, pData2, pData3);

	CCAD_GemDimension	*pDim = (CCAD_GemDimension*) pGemEntity;
	pDim->m_nStyle = m_nStyle;
	pDim->m_nOption = m_nOption;
	strcpy(pDim->m_szText, m_szText);
	pDim->m_anglText = m_anglText;
	pDim->m_ptText = m_ptText;

	// Reset dst dimension's m_pStyle for networking
	pDim->m_pStyle = (CCAD_DimStyle *) m_pDoc->GetStyleMgr(SS_DIMENSION)->GetStyle(pDim->m_nStyle);
	// Reset dst text's m_pTextStyle for networking
	pDim->m_pTxtStyle = (CCAD_TextStyle *) m_pDoc->GetStyleMgr(SS_TEXT)->GetStyle(pDim->m_pStyle->m_sDimText.nStyle);
}

//---------------------------------------------
BOOL CCAD_GemDimension::Read(CCAD_File& cFile)
{
	CCAD_GemBlock::Read(cFile);

	cFile.ReadShort(&m_nStyle);
	cFile.ReadUShort(&m_nOption);
	
	cFile.ReadString(m_szText, MAX_DIMSTR);

	m_anglText.Read(cFile);
	m_ptText.Read(cFile);

	return TRUE;
}

//---------------------------------------------
CCAD_Block * CCAD_GemDimension::GetBlock()
{
	if ( m_pDoc == NULL )
		return NULL;
	return (CCAD_Block*) m_pDoc->m_pMgrDimBlock->GetStyle(m_szName);
}

void CCAD_GemDimension::TmpDraw(CCAD_DC* pCadDC)
{
	ASSERT(pCadDC != NULL);

	m_pDrawDC = pCadDC;
	DrawDimension(true);
	m_pDrawDC = NULL;
}

void CCAD_GemDimension::AttachElementToDimBlock(CCAD_Block* pBlock)
{
	ASSERT(pBlock != NULL);

	m_pDimBlock = pBlock;
	DrawDimension();
	m_pDimBlock = NULL;
}

//---------------------------------------------
void CCAD_GemDimension::DrawElement(short nCode, CCAD_Entity* pElement)
{
	ASSERT((m_pDrawDC != NULL) || (m_pDimBlock != NULL));

	COLORREF clr;
	short nWeight = 0;

	switch(nCode)
	{
	case DIMELEMENT_EXTENSION:
		clr = m_pStyle->m_sDimExt.rgbColor;
		nWeight = m_pStyle->m_sDimExt.nThick;
		break;

	case DIMELEMENT_DIMENSION:
		clr = m_pStyle->m_sDimLine.rgbColor;
		nWeight = m_pStyle->m_sDimLine.nThick;
		break;

	case DIMELEMENT_TEXT:
		clr = m_pStyle->m_sDimText.rgbColor;
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	CCAD_Pen* pPen = pElement->GetCadPen();
	if(pPen != NULL)
	{
		pPen->m_nStyle = DISABLE_INDEX;
		pPen->m_rgbColor = clr;
		pPen->m_nThick = nWeight;
	}
	CCAD_Brush* pBrush = pElement->GetCadBrush();
	if(pBrush != NULL)
	{
		pBrush->m_nPattern = BRS_SOLID;
		pBrush->m_rgbColor = clr;
	}

	if(m_pDrawDC != NULL)
	{
		if((nCode == DIMELEMENT_TEXT) && (pElement->m_bCode == C_TEXT))
		{
			CCAD_LineEntity line(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
			line.m_nLayer = DISABLE_INDEX;
			line.GetCadPen()->m_rgbColor = clr;

			CCAD_GemLine* pLine = (CCAD_GemLine*) line.GetGemEntity();
			pLine->m_cBcs = pElement->GetGemEntity()->m_cBcs;

			CAD_FLOAT fWidth, fHeight;
			((CCAD_GemText*) pElement->GetGemEntity())->GetWidth(fHeight, fWidth);

			CCAD_Vector	cPt0(0, 0, 0), cPt1(fWidth, fHeight, 0);
			switch( ((CCAD_GemText*)pElement->GetGemEntity())->m_bPosFlag ) {
			case POS_LC:	cPt1.m_fY /= 2;
									cPt0.m_fY = -cPt1.m_fY;
									break;
			case POS_LT:	cPt0.m_fY = -cPt1.m_fY;
									cPt1.m_fY = 0;
									break;

			case POS_CB :	cPt1.m_fX /= 2;
									cPt0.m_fX = -cPt1.m_fX;
									break;
			case POS_CENTER:
									cPt1.m_fX /= 2;
									cPt0.m_fX = -cPt1.m_fX;
									cPt1.m_fY /= 2;
									cPt0.m_fY = -cPt1.m_fY;
									break;
			case POS_CT:	cPt1.m_fX /= 2;
									cPt0.m_fX = -cPt1.m_fX;
									cPt0.m_fY = -cPt1.m_fY;
									cPt1.m_fY = 0;
									break;

			case POS_RB :	cPt0.m_fX = -cPt1.m_fX;
									cPt1.m_fX = 0;
									break;
			case POS_RC:	cPt0.m_fX = -cPt1.m_fX;
									cPt1.m_fX = 0;
									cPt1.m_fY /= 2;
									cPt0.m_fY = -cPt1.m_fY;
									break;
			case POS_RT:	cPt0.m_fX = -cPt1.m_fX;
									cPt1.m_fX = 0;
									cPt0.m_fY = -cPt1.m_fY;
									cPt1.m_fY = 0;
									break;
			}

			pLine->m_cSp = cPt0;
			pLine->m_cEp.m_fX = cPt1.m_fX;
			pLine->m_cEp.m_fY = cPt0.m_fY;
			line.Draw(m_pDrawDC);
			pLine->m_cSp = pLine->m_cEp;
			pLine->m_cEp.m_fY = cPt1.m_fY;
			line.Draw(m_pDrawDC);
			pLine->m_cSp = pLine->m_cEp;
			pLine->m_cEp.m_fX = cPt0.m_fX;
			line.Draw(m_pDrawDC);
			pLine->m_cSp = pLine->m_cEp;
			pLine->m_cEp.m_fY = cPt0.m_fY;
			line.Draw(m_pDrawDC);
		}
		else {
			pElement->m_nLayer = DISABLE_INDEX;
			pElement->Draw(m_pDrawDC);
		}
	}
	else
	if(m_pDimBlock != NULL)
		m_pDimBlock->m_cMgrEntity.AddEntity(pElement);
	else
		ASSERT(FALSE);
}

//---------------------------------------------
CCAD_Block* CCAD_GemDimension::GetArrowHeadBlock(short nID)
{
	ASSERT(m_pDoc != NULL);

	char *szBlockArray[32] = { "_Closedfilled", "_Closedblank", "_Closed", "_Dot", "_ArchTick", 
							"_Oblique",	"_Open", "_Original", "_Original2", "_Open90", 
							"_Open30", "_DotSmall", "_DotBlank", "_Small", "_BoxBlank", 
							"_BoxFilled", "_DatumBlank", "_DatumFilled", "_Integral", "_None" };
	char szBlockName[32];
	strcpy(szBlockName, szBlockArray[nID]);
	CCAD_Block* pArrow = (CCAD_Block*) m_pDoc->m_pMgrBlock->GetStyle(szBlockName);
	if(pArrow != NULL)
		return pArrow;

	if(nID == SDAS_NONE)
		return NULL;

		// create arrow header block
	try
	{
		CCAD_Rect bound(CCAD_Point(-1, -1, 0), CCAD_Point(1, 1, 0));
		pArrow = (CCAD_Block*) m_pDoc->m_pMgrBlock->NewStyle();
		strcpy(pArrow->m_szName, szBlockName);

		if(nID == SDAS_CLOSEDFILLED)
		{	// _CLOSEDFILLED
			CCAD_PaintEntity paint(NULL, -1, -1);
			CCAD_GemPaint* pGemPaint = (CCAD_GemPaint*) paint.GetGemEntity();
			pGemPaint->m_bDraw = DRAW_PAINT;
			pGemPaint->m_nTotGrp = 1;
			pGemPaint->m_nTotPt = 3;
			pGemPaint->m_pNum = new short[1];
			*pGemPaint->m_pNum = pGemPaint->m_nTotPt;
			pGemPaint->m_pPt = new CCAD_Point[*pGemPaint->m_pNum];
			pGemPaint->m_pPt[0] = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.167, (CAD_FLOAT) 0.0);
			pGemPaint->m_pPt[1] = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemPaint->m_pPt[2] = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) -0.167, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&paint);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += paint.GetSize();
		}
		else
		if(nID == SDAS_CLOSEDBLANK)
		{	// _CLOSEDBLANK
			CCAD_LineEntity line1(NULL, -1, -1);
			CCAD_GemLine* pGemLine1 = (CCAD_GemLine*) line1.GetGemEntity();
			pGemLine1->m_cSp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.167, (CAD_FLOAT) 0.0);
			pGemLine1->m_cEp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line2(NULL, -1, -1);
			CCAD_GemLine* pGemLine2 = (CCAD_GemLine*) line2.GetGemEntity();
			pGemLine2->m_cSp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine2->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) -0.167, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line3(NULL, -1, -1);
			CCAD_GemLine* pGemLine3 = (CCAD_GemLine*) line3.GetGemEntity();
			pGemLine3->m_cSp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.167, (CAD_FLOAT) 0.0);
			pGemLine3->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) -0.167, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&line1);
			pArrow->m_cMgrEntity.AddEntity(&line2);
			pArrow->m_cMgrEntity.AddEntity(&line3);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += line1.GetSize();
			pArrow->m_lSize += line2.GetSize();
			pArrow->m_lSize += line3.GetSize();
		}
		else
		if(nID == SDAS_CLOSED)
		{	// _CLOSED
			CCAD_LineEntity line1(NULL, -1, -1);
			CCAD_GemLine* pGemLine1 = (CCAD_GemLine*) line1.GetGemEntity();
			pGemLine1->m_cSp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.167, (CAD_FLOAT) 0.0);
			pGemLine1->m_cEp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line2(NULL, -1, -1);
			CCAD_GemLine* pGemLine2 = (CCAD_GemLine*) line2.GetGemEntity();
			pGemLine2->m_cSp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine2->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) -0.167, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line3(NULL, -1, -1);
			CCAD_GemLine* pGemLine3 = (CCAD_GemLine*) line3.GetGemEntity();
			pGemLine3->m_cSp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.167, (CAD_FLOAT) 0.0);
			pGemLine3->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) -0.167, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line4(NULL, -1, -1);
			CCAD_GemLine* pGemLine4 = (CCAD_GemLine*) line4.GetGemEntity();
			pGemLine4->m_cSp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine4->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&line1);
			pArrow->m_cMgrEntity.AddEntity(&line2);
			pArrow->m_cMgrEntity.AddEntity(&line3);
			pArrow->m_cMgrEntity.AddEntity(&line4);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += line1.GetSize();
			pArrow->m_lSize += line2.GetSize();
			pArrow->m_lSize += line3.GetSize();
			pArrow->m_lSize += line4.GetSize();
		}
		else
		if(nID == SDAS_DOT)
		{	// _DOT
			CCAD_CircleEntity circle(NULL, -1, -1);
			CCAD_GemCircle* pGemCircle = (CCAD_GemCircle*) circle.GetGemEntity();
			pGemCircle->m_bDraw = DRAW_PAINT;
			pGemCircle->m_fR = (CAD_FLOAT) 0.5;
			CCAD_LineEntity line(NULL, -1, -1);
			CCAD_GemLine* pGemLine = (CCAD_GemLine*) line.GetGemEntity();
			pGemLine->m_cSp = CCAD_Vector((CAD_FLOAT) -0.5, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
		//	pGemLine->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine->m_cEp = CCAD_Vector((CAD_FLOAT) -2.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);	

			pArrow->m_cMgrEntity.AddEntity(&circle);
			pArrow->m_cMgrEntity.AddEntity(&line);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += circle.GetSize();
			pArrow->m_lSize += line.GetSize();
		}
		else
		if(nID == SDAS_ARCHTICK)
		{	// _ARCHTICK
			CCAD_LineEntity line(NULL, -1, -1);
			CCAD_GemLine* pGemLine = (CCAD_GemLine*) line.GetGemEntity();
			pGemLine->m_cSp = CCAD_Vector((CAD_FLOAT) -0.5, (CAD_FLOAT) -0.5, (CAD_FLOAT) 0.0);
			pGemLine->m_cEp = CCAD_Vector((CAD_FLOAT) 0.5, (CAD_FLOAT) 0.5, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&line);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += line.GetSize();
		}
		else
		if(nID == SDAS_OBLIQUE)
		{	// _OBLIQUE
			CCAD_LineEntity line(NULL, -1, -1);
			CCAD_GemLine* pGemLine = (CCAD_GemLine*) line.GetGemEntity();
			pGemLine->m_cSp = CCAD_Vector((CAD_FLOAT) -0.5, (CAD_FLOAT) -0.5, (CAD_FLOAT) 0.0);
			pGemLine->m_cEp = CCAD_Vector((CAD_FLOAT) 0.5, (CAD_FLOAT) 0.5, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&line);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += line.GetSize();
		}
		else
		if(nID == SDAS_OPEN)
		{	// _OPEN
			CCAD_LineEntity line1(NULL, -1, -1);
			CCAD_GemLine* pGemLine1 = (CCAD_GemLine*) line1.GetGemEntity();
			pGemLine1->m_cSp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.167, (CAD_FLOAT) 0.0);
			pGemLine1->m_cEp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line2(NULL, -1, -1);
			CCAD_GemLine* pGemLine2 = (CCAD_GemLine*) line2.GetGemEntity();
			pGemLine2->m_cSp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine2->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) -0.167, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line3(NULL, -1, -1);
			CCAD_GemLine* pGemLine3 = (CCAD_GemLine*) line3.GetGemEntity();
			pGemLine3->m_cSp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine3->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&line1);
			pArrow->m_cMgrEntity.AddEntity(&line2);
			pArrow->m_cMgrEntity.AddEntity(&line3);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += line1.GetSize();
			pArrow->m_lSize += line2.GetSize();
			pArrow->m_lSize += line3.GetSize();
		}
		else
		if(nID == SDAS_ORIGIN)
		{	// _ORIGIN
			CCAD_CircleEntity circle(NULL, -1, -1);
			CCAD_GemCircle* pGemCircle = (CCAD_GemCircle*) circle.GetGemEntity();
			pGemCircle->m_fR = (CAD_FLOAT) 0.5;
			CCAD_LineEntity line(NULL, -1, -1);
			CCAD_GemLine* pGemLine = (CCAD_GemLine*) line.GetGemEntity();
			pGemLine->m_cSp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&circle);
			pArrow->m_cMgrEntity.AddEntity(&line);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += circle.GetSize();
			pArrow->m_lSize += line.GetSize();
		}
		else
		if(nID == SDAS_ORIGIN2)
		{	// _ORIGIN2
			CCAD_CircleEntity circle1(NULL, -1, -1);
			CCAD_GemCircle* pGemCircle1 = (CCAD_GemCircle*) circle1.GetGemEntity();
			pGemCircle1->m_fR = (CAD_FLOAT) 0.5;
			CCAD_CircleEntity circle2(NULL, -1, -1);
			CCAD_GemCircle* pGemCircle2 = (CCAD_GemCircle*) circle2.GetGemEntity();
			pGemCircle2->m_fR = (CAD_FLOAT) 0.25;
			CCAD_LineEntity line(NULL, -1, -1);
			CCAD_GemLine* pGemLine = (CCAD_GemLine*) line.GetGemEntity();
			pGemLine->m_cSp = CCAD_Vector((CAD_FLOAT) -0.5, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&circle1);
			pArrow->m_cMgrEntity.AddEntity(&circle2);
			pArrow->m_cMgrEntity.AddEntity(&line);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += circle1.GetSize();
			pArrow->m_lSize += circle2.GetSize();
			pArrow->m_lSize += line.GetSize();
		}
		else
		if(nID == SDAS_OPEN90)
		{	// _OPEN90
			CCAD_LineEntity line1(NULL, -1, -1);
			CCAD_GemLine* pGemLine1 = (CCAD_GemLine*) line1.GetGemEntity();
			pGemLine1->m_cSp = CCAD_Vector((CAD_FLOAT) -0.5, (CAD_FLOAT) 0.5, (CAD_FLOAT) 0.0);
			pGemLine1->m_cEp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line2(NULL, -1, -1);
			CCAD_GemLine* pGemLine2 = (CCAD_GemLine*) line2.GetGemEntity();
			pGemLine2->m_cSp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine2->m_cEp = CCAD_Vector((CAD_FLOAT) -0.5, (CAD_FLOAT) -0.5, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line3(NULL, -1, -1);
			CCAD_GemLine* pGemLine3 = (CCAD_GemLine*) line3.GetGemEntity();
			pGemLine3->m_cSp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine3->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&line1);
			pArrow->m_cMgrEntity.AddEntity(&line2);
			pArrow->m_cMgrEntity.AddEntity(&line3);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += line1.GetSize();
			pArrow->m_lSize += line2.GetSize();
			pArrow->m_lSize += line3.GetSize();
		}
		else
		if(nID == SDAS_OPEN30)
		{	// _OPEN30
			CCAD_LineEntity line1(NULL, -1, -1);
			CCAD_GemLine* pGemLine1 = (CCAD_GemLine*) line1.GetGemEntity();
			pGemLine1->m_cSp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.268, (CAD_FLOAT) 0.0);
			pGemLine1->m_cEp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line2(NULL, -1, -1);
			CCAD_GemLine* pGemLine2 = (CCAD_GemLine*) line2.GetGemEntity();
			pGemLine2->m_cSp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine2->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) -0.268, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line3(NULL, -1, -1);
			CCAD_GemLine* pGemLine3 = (CCAD_GemLine*) line3.GetGemEntity();
			pGemLine3->m_cSp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine3->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&line1);
			pArrow->m_cMgrEntity.AddEntity(&line2);
			pArrow->m_cMgrEntity.AddEntity(&line3);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += line1.GetSize();
			pArrow->m_lSize += line2.GetSize();
			pArrow->m_lSize += line3.GetSize();
		}
		else
		if(nID == SDAS_DOTSMALL)
		{	// _DOTSMALL
			CCAD_CircleEntity circle(NULL, -1, -1);
			CCAD_GemCircle* pGemCircle = (CCAD_GemCircle*) circle.GetGemEntity();
			pGemCircle->m_bDraw = DRAW_PAINT;
			pGemCircle->m_fR = (CAD_FLOAT) 0.25;
			CCAD_LineEntity line(NULL, -1, -1);
			CCAD_GemLine* pGemLine = (CCAD_GemLine*) line.GetGemEntity();
			pGemLine->m_cSp = CCAD_Vector((CAD_FLOAT) -0.25, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&circle);
			pArrow->m_cMgrEntity.AddEntity(&line);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += circle.GetSize();
			pArrow->m_lSize += line.GetSize();
		}
		else
		if(nID == SDAS_DOTBLANK)
		{	// _DOTBLANK
			CCAD_CircleEntity circle(NULL, -1, -1);
			CCAD_GemCircle* pGemCircle = (CCAD_GemCircle*) circle.GetGemEntity();
			pGemCircle->m_fR = (CAD_FLOAT) 0.5;
			CCAD_LineEntity line(NULL, -1, -1);
			CCAD_GemLine* pGemLine = (CCAD_GemLine*) line.GetGemEntity();
			pGemLine->m_cSp = CCAD_Vector((CAD_FLOAT) -0.5, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&circle);
			pArrow->m_cMgrEntity.AddEntity(&line);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += circle.GetSize();
			pArrow->m_lSize += line.GetSize();
		}
		else
		if(nID == SDAS_SMALL)
		{	// _SMALL
			CCAD_CircleEntity circle(NULL, -1, -1);
			CCAD_GemCircle* pGemCircle = (CCAD_GemCircle*) circle.GetGemEntity();
			pGemCircle->m_fR = (CAD_FLOAT) 0.25;
			CCAD_LineEntity line(NULL, -1, -1);
			CCAD_GemLine* pGemLine = (CCAD_GemLine*) line.GetGemEntity();
			pGemLine->m_cSp = CCAD_Vector((CAD_FLOAT) -0.25, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&circle);
			pArrow->m_cMgrEntity.AddEntity(&line);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += circle.GetSize();
			pArrow->m_lSize += line.GetSize();
		}
		else
		if(nID == SDAS_BOXBLANK)
		{	// _BOXBLANK
			CCAD_LineEntity line1(NULL, -1, -1);
			CCAD_GemLine* pGemLine1 = (CCAD_GemLine*) line1.GetGemEntity();
			pGemLine1->m_cSp = CCAD_Vector((CAD_FLOAT) -0.5, (CAD_FLOAT) -0.5, (CAD_FLOAT) 0.0);
			pGemLine1->m_cEp = CCAD_Vector((CAD_FLOAT) 0.5, (CAD_FLOAT) -0.5, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line2(NULL, -1, -1);
			CCAD_GemLine* pGemLine2 = (CCAD_GemLine*) line2.GetGemEntity();
			pGemLine2->m_cSp = CCAD_Vector((CAD_FLOAT) 0.5, (CAD_FLOAT) -0.5, (CAD_FLOAT) 0.0);
			pGemLine2->m_cEp = CCAD_Vector((CAD_FLOAT) 0.5, (CAD_FLOAT) 0.5, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line3(NULL, -1, -1);
			CCAD_GemLine* pGemLine3 = (CCAD_GemLine*) line3.GetGemEntity();
			pGemLine3->m_cSp = CCAD_Vector((CAD_FLOAT) 0.5, (CAD_FLOAT) 0.5, (CAD_FLOAT) 0.0);
			pGemLine3->m_cEp = CCAD_Vector((CAD_FLOAT) -0.5, (CAD_FLOAT) 0.5, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line4(NULL, -1, -1);
			CCAD_GemLine* pGemLine4 = (CCAD_GemLine*) line4.GetGemEntity();
			pGemLine4->m_cSp = CCAD_Vector((CAD_FLOAT) -0.5, (CAD_FLOAT) 0.5, (CAD_FLOAT) 0.0);
			pGemLine4->m_cEp = CCAD_Vector((CAD_FLOAT) -0.5, (CAD_FLOAT) -0.5, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line5(NULL, -1, -1);
			CCAD_GemLine* pGemLine5 = (CCAD_GemLine*) line5.GetGemEntity();
			pGemLine5->m_cSp = CCAD_Vector((CAD_FLOAT) -0.5, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine5->m_cEp = CCAD_Vector((CAD_FLOAT) -5.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&line1);
			pArrow->m_cMgrEntity.AddEntity(&line2);
			pArrow->m_cMgrEntity.AddEntity(&line3);
			pArrow->m_cMgrEntity.AddEntity(&line4);
			pArrow->m_cMgrEntity.AddEntity(&line5);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += line1.GetSize();
			pArrow->m_lSize += line2.GetSize();
			pArrow->m_lSize += line3.GetSize();
			pArrow->m_lSize += line4.GetSize();
			pArrow->m_lSize += line5.GetSize();
		}
		else
		if(nID == SDAS_BOXFILLED)
		{	// _BOXFILLED
			CCAD_PaintEntity paint(NULL, -1, -1);
			CCAD_GemPaint* pGemPaint = (CCAD_GemPaint*) paint.GetGemEntity();
			pGemPaint->m_bDraw = DRAW_PAINT;
			pGemPaint->m_nTotGrp = 1;
			pGemPaint->m_nTotPt = 4;
			pGemPaint->m_pNum = new short[1];
			*pGemPaint->m_pNum = pGemPaint->m_nTotPt;
			pGemPaint->m_pPt = new CCAD_Point[*pGemPaint->m_pNum];
			pGemPaint->m_pPt[0] = CCAD_Vector((CAD_FLOAT) -0.5, (CAD_FLOAT) -0.5, (CAD_FLOAT) 0.0);
			pGemPaint->m_pPt[1] = CCAD_Vector((CAD_FLOAT) 0.5, (CAD_FLOAT) -0.5, (CAD_FLOAT) 0.0);
			pGemPaint->m_pPt[2] = CCAD_Vector((CAD_FLOAT) 0.5, (CAD_FLOAT) 0.5, (CAD_FLOAT) 0.0);
			pGemPaint->m_pPt[3] = CCAD_Vector((CAD_FLOAT) -0.5, (CAD_FLOAT) 0.5, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line(NULL, -1, -1);
			CCAD_GemLine* pGemLine = (CCAD_GemLine*) line.GetGemEntity();
			pGemLine->m_cSp = CCAD_Vector((CAD_FLOAT) -0.5, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&paint);
			pArrow->m_cMgrEntity.AddEntity(&line);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += paint.GetSize();
			pArrow->m_lSize += line.GetSize();
		}
		else
		if(nID == SDAS_DATUMBLANK)
		{	// _DATUMBLANK
			CCAD_LineEntity line1(NULL, -1, -1);
			CCAD_GemLine* pGemLine1 = (CCAD_GemLine*) line1.GetGemEntity();
			pGemLine1->m_cSp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.577, (CAD_FLOAT) 0.0);
			pGemLine1->m_cEp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line2(NULL, -1, -1);
			CCAD_GemLine* pGemLine2 = (CCAD_GemLine*) line2.GetGemEntity();
			pGemLine2->m_cSp = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemLine2->m_cEp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) -0.577, (CAD_FLOAT) 0.0);
			CCAD_LineEntity line3(NULL, -1, -1);
			CCAD_GemLine* pGemLine3 = (CCAD_GemLine*) line3.GetGemEntity();
			pGemLine3->m_cSp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) -0.577, (CAD_FLOAT) 0.0);
			pGemLine3->m_cEp = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.577, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&line1);
			pArrow->m_cMgrEntity.AddEntity(&line2);
			pArrow->m_cMgrEntity.AddEntity(&line3);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += line1.GetSize();
			pArrow->m_lSize += line2.GetSize();
			pArrow->m_lSize += line3.GetSize();
		}
		else
		if(nID == SDAS_DATUMFILLED)
		{	// _DATUMFILLED
			CCAD_PaintEntity paint(NULL, -1, -1);
			CCAD_GemPaint* pGemPaint = (CCAD_GemPaint*) paint.GetGemEntity();
			pGemPaint->m_bDraw = DRAW_PAINT;
			pGemPaint->m_nTotGrp = 1;
			pGemPaint->m_nTotPt = 3;
			pGemPaint->m_pNum = new short[1];
			*pGemPaint->m_pNum = pGemPaint->m_nTotPt;
			pGemPaint->m_pPt = new CCAD_Point[*pGemPaint->m_pNum];
			pGemPaint->m_pPt[0] = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) 0.577, (CAD_FLOAT) 0.0);
			pGemPaint->m_pPt[1] = CCAD_Vector((CAD_FLOAT) -1.0, (CAD_FLOAT) 0.0, (CAD_FLOAT) 0.0);
			pGemPaint->m_pPt[2] = CCAD_Vector((CAD_FLOAT) 0.0, (CAD_FLOAT) -0.577, (CAD_FLOAT) 0.0);

			pArrow->m_cMgrEntity.AddEntity(&paint);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += paint.GetSize();
		}
		else
		if(nID == SDAS_INTEGRAL)
		{	// _INTEGRAL
			CCAD_ArcEntity arc1(NULL, -1, -1);
			CCAD_GemArc* pGemArc1 = (CCAD_GemArc*) arc1.GetGemEntity();
			pGemArc1->m_cCp = CCAD_Vector((CAD_FLOAT) 0.445, (CAD_FLOAT) -0.091, (CAD_FLOAT) 0.0);
			pGemArc1->m_fR = (CAD_FLOAT) 0.454;
			pGemArc1->m_cAlfa = (short) 102;
			pGemArc1->m_cDelta = (short) (168 - 102);
			CCAD_ArcEntity arc2(NULL, -1, -1);
			CCAD_GemArc* pGemArc2 = (CCAD_GemArc*) arc2.GetGemEntity();
			pGemArc2->m_cCp = CCAD_Vector((CAD_FLOAT) -0.445, (CAD_FLOAT) 0.091, (CAD_FLOAT) 0.0);
			pGemArc2->m_fR = (CAD_FLOAT) 0.454;
			pGemArc2->m_cAlfa = (short) 282;
			pGemArc2->m_cDelta = (short) (348 - 282);

			pArrow->m_cMgrEntity.AddEntity(&arc1);
			pArrow->m_cMgrEntity.AddEntity(&arc2);
			pArrow->m_cRc = bound;
			pArrow->m_lSize += arc1.GetSize();
			pArrow->m_lSize += arc2.GetSize();
		}
		else
			ASSERT(FALSE);

		short i;
		for(i = 0; i < pArrow->m_cMgrEntity.m_cArray.GetSize(); i++)
		{
			CCAD_Entity* pEntity = (CCAD_Entity*) pArrow->m_cMgrEntity.m_cArray[i];
			pEntity->m_pDoc = m_pDoc;
			pEntity->m_nSpaceDoc = m_pDoc->GetCurrentDoc()->m_nID;
			pEntity->m_nUds = m_pDoc->GetUds()->m_nID;
			pEntity->InitEntity();
		}

		if(!m_pDoc->m_pMgrBlock->Add(pArrow))
			throw -1;
	}
	catch(...)
	{
		if(pArrow != NULL)
			delete pArrow;
		pArrow = NULL;
	}

	return pArrow;
}

//---------------------------------------------
void CCAD_GemDimension::GetDefaultTextParameter(BYTE bCode, CAD_FLOAT fValue, CCAD_TextEntity& txt, 
												CCAD_TextEntity& upper, CCAD_TextEntity& lower, unsigned short& nShowText)
{
	fValue = (CAD_FLOAT) fabs((double) fValue);
	m_szDefaultText[0] = '\0';

	txt.m_cGemText.m_nStyle = m_pStyle->m_sDimText.nStyle;
	txt.m_cGemText.m_pTextStyle = m_pTxtStyle;
	txt.m_cGemText.m_bPosFlag = POS_CENTER;
	txt.m_cGemText.m_fWidth = m_pStyle->m_sDimText.fWidth;
	txt.m_cGemText.m_fHeight = m_pStyle->m_sDimText.fHeight;
	txt.m_cGemText.m_cDirect = (short) 0;
	txt.m_cGemText.m_fEmpty = 0;
	if(m_szText[0] != '\0')
	{
		txt.m_cGemText.m_nLength = strlen(m_szText);
		txt.m_cGemText.m_pStr = new char[txt.m_cGemText.m_nLength + 1];
		strcpy(txt.m_cGemText.m_pStr, m_szText);
		nShowText = 0x01;
	}
	else
	{
		char *strUnits[] = { " ", " ", "R", "F", "'", ""/*reserv*/ };
		char strUnit[5];
		strcpy(strUnit, strUnits[bCode]);
		char szFormat[MAX_STR];
		if((bCode == DIM_RADIUS) || (bCode == DIM_DIAMETER))
			sprintf(szFormat, "%s%%.%df", strUnit, m_pStyle->m_sDimUnit.nLengthUnit);
		else
		if(bCode == DIM_ANGULAR)
			sprintf(szFormat, "%%.%df%s", m_pStyle->m_sDimUnit.nAngleUnit, strUnit);
		else		// DIM_LINEAR | DIM_ORDINATE
			sprintf(szFormat, "%%.%df", m_pStyle->m_sDimUnit.nLengthUnit);

		char szTemp[MAX_STR];
		sprintf(szTemp, szFormat, fabs((double) fValue));
		txt.m_cGemText.m_nLength = strlen(m_pStyle->m_sDimUnit.szPrefix) + strlen(szTemp) + strlen(m_pStyle->m_sDimUnit.szSuffix);
		txt.m_cGemText.m_pStr = new char[txt.m_cGemText.m_nLength + 1];
		sprintf(txt.m_cGemText.m_pStr, "%s%s%s", m_pStyle->m_sDimUnit.szPrefix, szTemp, m_pStyle->m_sDimUnit.szSuffix);
		nShowText = 0x01;

		strcpy(m_szDefaultText, txt.m_cGemText.m_pStr);

		txt.m_cGemText.MemCopyTo(&upper.m_cGemText);
		upper.m_cGemText.m_fWidth *= m_pStyle->m_sDimToler.fScale;
		upper.m_cGemText.m_fHeight *= m_pStyle->m_sDimToler.fScale;
		upper.m_cGemText.Remove();
		txt.m_cGemText.MemCopyTo(&lower.m_cGemText);
		lower.m_cGemText.m_fWidth *= m_pStyle->m_sDimToler.fScale;
		lower.m_cGemText.m_fHeight *= m_pStyle->m_sDimToler.fScale;
		lower.m_cGemText.Remove();

		CAD_FLOAT fTxtWidth, fTxtHeight;
		txt.m_cGemText.GetWidth(fTxtHeight, fTxtWidth);

		sprintf(szFormat, "%%.%df", m_pStyle->m_sDimToler.nUnit);

		if(m_pStyle->m_sDimToler.sState.format == DIM_TOLER_FORMAT_SYM)
		{
			char *strPlusMinusSymbol = "+";

			sprintf(szTemp, szFormat, m_pStyle->m_sDimToler.fUpper);
			upper.m_cGemText.m_nLength = strlen(szTemp) + strlen(strPlusMinusSymbol);
			upper.m_cGemText.m_pStr = new char[upper.m_cGemText.m_nLength + 1];
			sprintf(upper.m_cGemText.m_pStr, "%s%s", strPlusMinusSymbol, szTemp);

			CAD_FLOAT fUpperWidth, fUpperHeight;
			upper.m_cGemText.GetWidth(fUpperHeight, fUpperWidth);

			upper.m_cGemText.m_cBcs.m_cOrg.m_fX = fTxtWidth + m_pStyle->m_sDimText.fGap;
			switch(m_pStyle->m_sDimToler.sState.vert)
			{
			case DIM_TOLER_VERT_TOP:
				upper.m_cGemText.m_cBcs.m_cOrg.m_fY = fTxtHeight - fUpperHeight;
				break;

			case DIM_TOLER_VERT_MIDDLE:
				upper.m_cGemText.m_cBcs.m_cOrg.m_fY = (fTxtHeight - fUpperHeight) / 2;
				break;

			case DIM_TOLER_VERT_BOTTOM:
				break;
			}

			nShowText |= 0x02;
		}
		else
		if(m_pStyle->m_sDimToler.sState.format == DIM_TOLER_FORMAT_NOSYM)
		{
			char *strPlusSymbol = "+";
			char *strMinusSymbol = "-";

			sprintf(szTemp, szFormat, m_pStyle->m_sDimToler.fUpper);
			upper.m_cGemText.m_nLength = strlen(szTemp) + strlen(strPlusSymbol);
			upper.m_cGemText.m_pStr = new char[upper.m_cGemText.m_nLength + 1];
			sprintf(upper.m_cGemText.m_pStr, "%s%s", strPlusSymbol, szTemp);

			sprintf(szTemp, szFormat, m_pStyle->m_sDimToler.fLower);
			lower.m_cGemText.m_nLength = strlen(szTemp) + strlen(strMinusSymbol);
			lower.m_cGemText.m_pStr = new char[lower.m_cGemText.m_nLength + 1];
			sprintf(lower.m_cGemText.m_pStr, "%s%s", strMinusSymbol, szTemp);

			CAD_FLOAT fUpperWidth, fUpperHeight;
			upper.m_cGemText.GetWidth(fUpperHeight, fUpperWidth);
			CAD_FLOAT fLowerWidth, fLowerHeight;
			lower.m_cGemText.GetWidth(fLowerHeight, fLowerWidth);

			upper.m_cGemText.m_cBcs.m_cOrg.m_fX = fTxtWidth + m_pStyle->m_sDimText.fGap;
			lower.m_cGemText.m_cBcs.m_cOrg.m_fX = fTxtWidth + m_pStyle->m_sDimText.fGap;
			switch(m_pStyle->m_sDimToler.sState.vert)
			{
			case DIM_TOLER_VERT_TOP:
				lower.m_cGemText.m_cBcs.m_cOrg.m_fY = fTxtHeight - (fUpperHeight + m_pStyle->m_sDimText.fGap + fLowerHeight);
				break;

			case DIM_TOLER_VERT_MIDDLE:
				lower.m_cGemText.m_cBcs.m_cOrg.m_fY = (fTxtHeight - m_pStyle->m_sDimText.fGap) / 2 - fLowerHeight;
				break;

			case DIM_TOLER_VERT_BOTTOM:
				break;
			}
			upper.m_cGemText.m_cBcs.m_cOrg.m_fY = lower.m_cGemText.m_cBcs.m_cOrg.m_fY + fLowerHeight + m_pStyle->m_sDimText.fGap;

			nShowText |= (0x02 | 0x04);
		}
		else
		if(m_pStyle->m_sDimToler.sState.format == DIM_TOLER_FORMAT_LIMIT)
		{
			sprintf(szTemp, szFormat, fValue + m_pStyle->m_sDimToler.fUpper);
			upper.m_cGemText.m_nLength = strlen(szTemp);
			upper.m_cGemText.m_pStr = new char[upper.m_cGemText.m_nLength + 1];
			strcpy(upper.m_cGemText.m_pStr, szTemp);

			sprintf(szTemp, szFormat, fValue - m_pStyle->m_sDimToler.fLower);
			lower.m_cGemText.m_nLength = strlen(szTemp);
			lower.m_cGemText.m_pStr = new char[lower.m_cGemText.m_nLength + 1];
			strcpy(lower.m_cGemText.m_pStr, szTemp);

			CAD_FLOAT fUpperWidth, fUpperHeight;
			upper.m_cGemText.GetWidth(fUpperHeight, fUpperWidth);
			CAD_FLOAT fLowerWidth, fLowerHeight;
			lower.m_cGemText.GetWidth(fLowerHeight, fLowerWidth);

			lower.m_cGemText.m_cBcs.m_cOrg.m_fY = m_pStyle->m_sDimText.fGap + fLowerHeight;

			nShowText = (0x02 | 0x04);
		}
	}
}

//---------------------------------------------
void CCAD_GemDimension::GetTextArrayBounds(short nCount, CCAD_TextEntity** ppText, CCAD_Rect & boundTxt)
{
	boundTxt.m_fMinX = 0;
	boundTxt.m_fMinY = 0;
	ppText[0]->m_cGemText.GetWidth(boundTxt.m_fMaxY, boundTxt.m_fMaxX);

	CCAD_Rect bound;
	int	i;
	for(i = 1; i < nCount; i++)
	{
		ppText[i]->m_cGemText.GetWidth(bound.m_fMaxY, bound.m_fMaxX);
		bound.m_fMinX = ppText[i]->m_cGemText.m_cBcs.m_cOrg.m_fX;
		bound.m_fMinY = ppText[i]->m_cGemText.m_cBcs.m_cOrg.m_fY;
		bound.m_fMaxX += bound.m_fMinX;
		bound.m_fMaxY += bound.m_fMinY;

		if(boundTxt.m_fMinX > bound.m_fMinX)
			boundTxt.m_fMinX = bound.m_fMinX;
		if(boundTxt.m_fMinY > bound.m_fMinY)
			boundTxt.m_fMinY = bound.m_fMinY;
		if(boundTxt.m_fMaxX < bound.m_fMaxX)
			boundTxt.m_fMaxX = bound.m_fMaxX;
		if(boundTxt.m_fMaxY < bound.m_fMaxY)
			boundTxt.m_fMaxY = bound.m_fMaxY;
	}

	boundTxt.m_fMaxX = boundTxt.Width();
	boundTxt.m_fMaxY = boundTxt.Height();
	boundTxt.m_fMinX = 0;
	boundTxt.m_fMinY = 0;

	if(m_pStyle->m_sDimText.sState.rect != 0)
	{
		boundTxt.m_fMaxX += 2 * m_pStyle->m_sDimText.fGap;
		boundTxt.m_fMaxY += 2 * m_pStyle->m_sDimText.fGap;
	}
}

//----------------------------------------------------------
void CCAD_GemDimension::DrawDimensionText(short nTxtCount, CCAD_TextEntity** ppText)
{
	CCAD_Coord3D* pTxtCoord = &ppText[0]->GetGemEntity()->m_cBcs;

	CCAD_Rect boundTxt;
	GetTextArrayBounds(nTxtCount, ppText, boundTxt);

	if(m_anglText.IsZero())
	{
		CCAD_Vector orgTxt(0, 0, 0);
		CCAD_Vector vctTxt(1, 0, 0);
		pTxtCoord->ConvToBase(orgTxt);
		pTxtCoord->ConvToBase(vctTxt);
		CCAD_Angle angle;
		angle.SetFromXY(vctTxt.m_fX - orgTxt.m_fX, vctTxt.m_fY - orgTxt.m_fY);

		if(!angle.Vertical()) {
			if(vctTxt.m_fX - orgTxt.m_fX < 0)
				pTxtCoord->YRotate(CCAD_Angle((short) 180));
		}
		else {
			if(angle.GetDegree() < 0)					// angle == - PAI / 2;
				pTxtCoord->YRotate(CCAD_Angle((short) 180));
		}

		orgTxt = CCAD_Vector(0, 0, 0);
		vctTxt = CCAD_Vector(0, 1, 0);
		pTxtCoord->ConvToBase(orgTxt);
		pTxtCoord->ConvToBase(vctTxt);
		angle.SetFromXY(vctTxt.m_fX - orgTxt.m_fX, vctTxt.m_fY - orgTxt.m_fY);

		if(!angle.Horizontal()) {
			if(vctTxt.m_fY - orgTxt.m_fY < 0)
				pTxtCoord->XRotate(CCAD_Angle((short) 180));
		}
		else {
			if(angle.GetAbsDegree() < CCAD_Global::GetAngleAccy())	// angle == - PAI / 2;
				pTxtCoord->XRotate(CCAD_Angle((short) 180));
		}
	}

	if(m_pStyle->m_sDimText.sState.rect != 0)
	{
		CCAD_LineEntity line(m_pDoc, m_pDoc->GetCurrentDoc()->m_nID, m_pDoc->GetUds()->m_nID);
		line.GetCadPen()->m_rgbColor = m_pStyle->m_sDimText.rgbColor;
		CCAD_GemLine* pLine = (CCAD_GemLine*) line.GetGemEntity();
		pLine->m_cBcs = *pTxtCoord;

		CCAD_Vector	cPt0(0, 0, 0), cPt1(boundTxt.Width(), boundTxt.Height(), 0);
		switch( ((CCAD_GemText*)ppText[0]->GetGemEntity())->m_bPosFlag ) {
		case POS_LC:	cPt1.m_fY /= 2;					cPt0.m_fY = -cPt1.m_fY;
								break;
		case POS_LT:	cPt0.m_fY = -cPt1.m_fY;		cPt1.m_fY = 0;
								break;
		case POS_CB :	cPt1.m_fX /= 2;					cPt0.m_fX = -cPt1.m_fX;
								break;
		case POS_CENTER:
								cPt1.m_fX /= 2;					cPt0.m_fX = -cPt1.m_fX;
								cPt1.m_fY /= 2;					cPt0.m_fY = -cPt1.m_fY;
								break;
		case POS_CT:	cPt1.m_fX /= 2;					cPt0.m_fX = -cPt1.m_fX;
								cPt0.m_fY = -cPt1.m_fY;		cPt1.m_fY = 0;
								break;
		case POS_RB :	cPt0.m_fX = -cPt1.m_fX;		cPt1.m_fX = 0;
								break;
		case POS_RC:	cPt0.m_fX = -cPt1.m_fX;		cPt1.m_fX = 0;
								cPt1.m_fY /= 2;						cPt0.m_fY = -cPt1.m_fY;
								break;
		case POS_RT:	cPt0.m_fX = -cPt1.m_fX;		cPt1.m_fX = 0;
								cPt0.m_fY = -cPt1.m_fY;		cPt1.m_fY = 0;
								break;
		}

		pLine->m_cSp = cPt0;
		pLine->m_cEp.m_fX = cPt1.m_fX;
		pLine->m_cEp.m_fY = cPt0.m_fY;
		DrawElement(DIMELEMENT_TEXT,&line);
		pLine->m_cSp = pLine->m_cEp;
		pLine->m_cEp.m_fY = cPt1.m_fY;
		DrawElement(DIMELEMENT_TEXT,&line);
		pLine->m_cSp = pLine->m_cEp;
		pLine->m_cEp.m_fX = cPt0.m_fX;
		DrawElement(DIMELEMENT_TEXT,&line);
		pLine->m_cSp = pLine->m_cEp;
		pLine->m_cEp.m_fY = cPt0.m_fY;
		DrawElement(DIMELEMENT_TEXT,&line);

		CCAD_Point org(m_pStyle->m_sDimText.fGap, m_pStyle->m_sDimText.fGap, 0);
		pTxtCoord->ConvToBase(org);
		pTxtCoord->m_cOrg = org;
	}

	short i;
	for(i = 1; i < nTxtCount; i++)
	{
		if(ppText[i]->m_cGemText.m_cBcs.m_cOrg.m_fY < 0)
		{
			CCAD_Point org(0, -ppText[i]->m_cGemText.m_cBcs.m_cOrg.m_fY, 0);
			pTxtCoord->ConvToBase(org);
			pTxtCoord->m_cOrg = org;
		}
	}

	for(i = 1; i < nTxtCount; i++)
		ppText[0]->m_cGemText.m_cBcs.ChildToThis(&ppText[i]->m_cGemText.m_cBcs);
	for(i = 0; i < nTxtCount; i++)
		DrawElement(DIMELEMENT_TEXT, ppText[i]);
		
}

//----------------------------------------------------------
void CCAD_GemDimension::CountBodyRange(CCAD_Rect &cRange)
{
	CCAD_Block * pBlock = GetBlock();
	if ( pBlock == NULL )
		return;

	long pos;
	CCAD_Entity	* pEntity = pBlock->m_cMgrEntity.GetFirstEntity(pos);
	while (pEntity != NULL) {
		CCAD_Rect	cRc;
		cRc.Init();
		pEntity->CountBodyRange(cRc);
		if(!cRc.IsInited())
		{
			CCAD_Coord3D *pBCS = pEntity->GetGemEntity()->GetBCS();
			ASSERT(pBCS != NULL);
			SCAD_POINT	pt[8] = {
				{cRc.m_fMinX, cRc.m_fMinY, cRc.m_fMinZ}, 
				{cRc.m_fMaxX, cRc.m_fMinY, cRc.m_fMinZ},
				{cRc.m_fMaxX, cRc.m_fMaxY, cRc.m_fMinZ},
				{cRc.m_fMinX, cRc.m_fMaxY, cRc.m_fMinZ},
				{cRc.m_fMinX, cRc.m_fMinY, cRc.m_fMaxZ}, 
				{cRc.m_fMaxX, cRc.m_fMinY, cRc.m_fMaxZ},
				{cRc.m_fMaxX, cRc.m_fMaxY, cRc.m_fMaxZ},
				{cRc.m_fMinX, cRc.m_fMaxY, cRc.m_fMaxZ}};

			CCAD_Point	ptTemp;
			int	i;
			for (i = 0; i < 8; i++)	{
				ptTemp = pt[i];
				pBCS->ConvToBase(ptTemp);
				cRange.ResetParameter(ptTemp);
			}
		}
		pEntity = pBlock->m_cMgrEntity.GetNextEntity(pos);
	}
}

//----------------------------------------------------------
void CCAD_GemDimension::CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect &cRange)
{
	CCAD_Block * pBlock = GetBlock();
	if ( pBlock == NULL )
		return;

	long pos;
	CCAD_Entity	* pEntity = pBlock->m_cMgrEntity.GetFirstEntity(pos);
	while (pEntity != NULL) {
		pEntity->CountVcsRange(pCadDC, cRange);
		pEntity = pBlock->m_cMgrEntity.GetNextEntity(pos);
	}
}
