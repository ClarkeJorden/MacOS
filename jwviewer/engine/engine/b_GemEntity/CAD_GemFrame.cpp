// CAD_GemFrame.cpp: implementation of the CCAD_GemFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CAD_GemFrame.h"
#include "CAD_GemLine.h"

/////////////////////////////////////////////////////////////////////////////
// CCAD_GemFrame
//
CCAD_GemFrame::CCAD_GemFrame()
{
	Init();
}

//----------------------------------------------
CCAD_GemFrame::~CCAD_GemFrame()
{
	RemoveFrameAll();
}

//----------------------------------------------
void CCAD_GemFrame::Init()
{
	m_nTotGrp = 0;
	m_nTotPt = 0;
	m_pNum = NULL;
	m_pPt = NULL;
	m_bDraw = DRAW_FRAME;
}

//----------------------------------------------
void CCAD_GemFrame::RemoveFrameAll()
{
	if (m_pNum != NULL)
		delete [] m_pNum;
	if (m_pPt != NULL)
		delete [] m_pPt;
	m_nTotGrp = 0;
	m_nTotPt = 0;
	m_pNum = NULL;
	m_pPt = NULL;
}

void CCAD_GemFrame::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_GemThickEntity::MemCopyTo(pGemEntity, pData1, pData2, pData3);

	((CCAD_GemFrame *)pGemEntity)->m_nTotGrp = m_nTotGrp;
	((CCAD_GemFrame *)pGemEntity)->m_nTotPt = m_nTotPt;
	((CCAD_GemFrame *)pGemEntity)->m_bDraw = m_bDraw;

	if (pData1 == NULL){
		pData1 = (LPSTR)new short[m_nTotGrp];
		pData2 = (LPSTR)new CCAD_Vector[m_nTotPt];
	}

	short i;
	((CCAD_GemFrame *)pGemEntity)->m_pNum = (short *)pData1;
	for (i=0; i<m_nTotGrp; i++)
		*(((CCAD_GemFrame *)pGemEntity)->m_pNum + i) = *(m_pNum + i);

	((CCAD_GemFrame *)pGemEntity)->m_pPt = (CCAD_Point *)pData2;
	for (i=0; i<m_nTotPt; i++)
		*(((CCAD_GemFrame *)pGemEntity)->m_pPt + i) = *(m_pPt + i);
}

void CCAD_GemFrame::Remove()
{
	RemoveFrameAll();
}

//-------------------------------------------
BOOL CCAD_GemFrame::Read(CCAD_File & cFile)
{
	CCAD_GemThickEntity::Read(cFile);
	cFile.ReadByte(&m_bDraw);
	cFile.ReadShort(&m_nTotGrp);
	cFile.ReadShort(&m_nTotPt);
	try {
		if (m_nTotGrp > 0) {
			m_pNum = new short[m_nTotGrp];
			for (int i=0; i<m_nTotGrp; i++)
				cFile.ReadShort(m_pNum+i);
		}
		if (m_nTotPt > 0) {
			m_pPt = new CCAD_Vector[m_nTotPt];
			short	i;
			for (i=0; i<m_nTotPt; i++)
				(m_pPt+i)->Read(cFile);
		}
	}
	catch(...) {
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------
BOOL CCAD_GemFrame::Draw(CCAD_DC *pDC, BOOL /*bRedraw*/)
{
	if (m_nTotGrp <= 0 || m_pNum == NULL || m_pPt == NULL) 
		return FALSE;

	CCAD_Vector		cSp, cPt;
	short i, j, k = 0;
	pDC->Lines_Begin(LINETYPE_LINES);
	for (i=0; i<m_nTotGrp; i++) {
		cPt = *(m_pPt+k);
		cSp = cPt;
		k++;
		pDC->MoveTo(cSp);
		for (j=1; j<*(m_pNum+i); j++) {
			cPt = *(m_pPt + k);
			k++;
			pDC->LineTo(cPt);
		}
		if ( *(m_pNum+i) > 2 )
			pDC->LineTo(cSp);
	}
	pDC->Lines_End();
	return TRUE;
}

//-----------------------------------------------------
void CCAD_GemFrame::CountBodyRange(CCAD_Rect& cRange)
{
	short i;
	for (i=0; i<m_nTotPt; i++) {
		cRange.ResetParameter(*(m_pPt+i));
	}
}

//-----------------------------------------------------
void CCAD_GemFrame::CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange)
{
	ASSERT(pCadDC != NULL);

	if (m_nTotGrp > 0 && m_nTotPt >= 3 && m_pNum != NULL && m_pPt != NULL) {
		short i;
		for (i=0; i<m_nTotPt; i++) {
			pCadDC->CountVcsRange(*(m_pPt+i), cRange);
		}
	}
}

//----------------------------------------------------------
WORD CCAD_GemFrame::GetCurvePoint(SCAD_POINT *spDivPnt)
{
	if (spDivPnt != NULL)
	{
	}
	return 0;
}

