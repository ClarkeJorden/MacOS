#include "stdafx.h"
#include "CAD_Constance.h"
#include "CAD_Doc.h"
#include "CAD_PaperDoc.h"
#include "CAD_Global.h"
#include "Common.h"
#include "CAD_ViewMain.h"

CCAD_SpaceDoc::CCAD_SpaceDoc(HANDLE hHeap, short nID)
{
	m_nID = nID;
}

CCAD_SpaceDoc::~CCAD_SpaceDoc()
{
}

/////////////////////////////////////////////////////////////////////////////
// CCAD_MgrDoc

CCAD_MgrDoc::CCAD_MgrDoc()
{
	m_hHeap = HeapCreate(0, 1024, 0);

	m_pPenStyleMgr = new CCAD_PenStyleMgr;
	m_pImageMgr = new CCAD_ImageMgr(m_hHeap);
	m_pTextStyleMgr = new CCAD_TextStyleMgr;
	m_pDimStyleMgr = new CCAD_DimStyleMgr;

	m_pMgrBlock = new CCAD_MgrBlock(m_hHeap, (CCAD_Doc *) this);
	m_pMgrDimBlock = new CCAD_MgrBlock(m_hHeap, (CCAD_Doc *) this);
}

//--------------------------------------------------------------------
CCAD_MgrDoc::~CCAD_MgrDoc()
{
	if (m_pPenStyleMgr != NULL)
		delete m_pPenStyleMgr;
	if (m_pImageMgr != NULL)
		delete m_pImageMgr;
	if (m_pTextStyleMgr != NULL)
		delete m_pTextStyleMgr;
	if (m_pDimStyleMgr != NULL)
		delete m_pDimStyleMgr;

	if (m_pMgrBlock != NULL)
		delete m_pMgrBlock;
	if (m_pMgrDimBlock != NULL)
		delete m_pMgrDimBlock;

	HeapDestroy(m_hHeap);
}

//--------------------------------------------------------------------
void CCAD_MgrDoc::RemoveAll()
{
	m_pPenStyleMgr->RemoveAll();
	m_pImageMgr->RemoveAll();
	m_pTextStyleMgr->RemoveAll();
	m_pDimStyleMgr->RemoveAll();
	m_pMgrBlock->RemoveAll();
	m_pMgrDimBlock->RemoveAll();
}

CCAD_StyleMgr *CCAD_MgrDoc::GetStyleMgr(LPARAM lParam)
{
	BYTE bStyle = (BYTE) lParam;
	switch (bStyle) {
		case SS_LINE:
			return m_pPenStyleMgr;
		case SS_TEXT:
			return m_pTextStyleMgr;
		case SS_DIMENSION:
			return m_pDimStyleMgr;
		case SS_IMAGE:
			return m_pImageMgr;
		case SS_BLOCK:
			return m_pMgrBlock;
		case SS_DIMBLOCK:
			return m_pMgrDimBlock;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CCAD_Doc

CCAD_Doc::CCAD_Doc()
{
	m_cPaperArray.RemoveAll();
	m_pSpaceDoc = 0;

	m_pLayerMgr = new CCAD_LayerMgr;
	memset(&m_sInform, 0, sizeof (m_sInform));
	//----------------------------
	m_cInitViewInfo.m_fScale = -1;
}

CCAD_Doc::~CCAD_Doc()
{
	EmptyDoc();
	
	int	i;
	for ( i=0; i < m_cPaperArray.GetSize(); i++ ) {
		CCAD_PaperDoc * pDoc = (CCAD_PaperDoc *)m_cPaperArray[i];
		delete pDoc;
		pDoc = NULL;
	}
	m_cPaperArray.RemoveAll();

	if (m_pLayerMgr != NULL)
		delete m_pLayerMgr;

}

void	CCAD_Doc::InitUds(short n)
{
	CCAD_Sheet cSheet;
	for (int i = n; i < m_cPaperArray.GetSize(); i ++) {
		CCAD_PaperDoc	*pDoc = (CCAD_PaperDoc *) m_cPaperArray.GetAt(i);
		pDoc->GetUdsMgr()->SetDefault();
		pDoc->m_cSheet = cSheet;
	}
}

void CCAD_Doc::RemoveAll()
{
	CCAD_MgrDoc::RemoveAll();

	m_pLayerMgr->RemoveAll();

	for (int i = 0; i < m_cPaperArray.GetSize(); i ++) {
		CCAD_PaperDoc	*pDoc = (CCAD_PaperDoc *) m_cPaperArray.GetAt(i);
		pDoc->GetUdsMgr()->RemoveAll();
	}
}

short CCAD_Doc::Read(CCAD_File & cFile)
{
    m_cEnvironment.Init();
    m_pTextStyleMgr->SetDefault();
    m_pDimStyleMgr->SetDefault();
    m_pPenStyleMgr->SetDefault();
    InitUds();
    
    CreatePaperDoc();
    CCAD_PaperDoc *pPaperDoc = (CCAD_PaperDoc *)GetSpaceDoc(0);
    CCAD_Style *pUds = pPaperDoc->m_pUdsMgr->NewStyle();
    pPaperDoc->m_pUdsMgr->Add(pUds);
    
    SetSpaceDoc(GetSpaceDoc(0));

    CJWCAD_Doc cJWCadDoc;
    cJWCadDoc.m_pDoc = this;
    try {
        cJWCadDoc.Read(cFile);
        CreateSceneGraph(pPaperDoc);
    } 
    catch(...)
    {
        return DISABLE_INDEX;
    }

    return cFile.m_bVersion;
}

CCAD_SpaceDoc *	CCAD_Doc::GetSpaceDoc(short nID)
{
	int	nTot = m_cPaperArray.GetSize();
	int	i;
	for ( i=0; i<nTot; i++ ) {
		CCAD_SpaceDoc * pDoc = (CCAD_SpaceDoc *)m_cPaperArray[i];
		if ( pDoc->m_nID == nID)
			return pDoc;
	}
	return NULL;
}

short	CCAD_Doc::GetCurrentDocID()
{
	int	nTot = m_cPaperArray.GetSize();
	int	i;
	for ( i=0; i<nTot; i++ ) {
		CCAD_SpaceDoc * pDoc = (CCAD_SpaceDoc *)m_cPaperArray[i];
		if ( pDoc == m_pSpaceDoc)
			return i;
	}
	return -2;
}

CCAD_SpaceDoc *	CCAD_Doc::CreatePaperDoc()
{
	short nTot = (short)m_cPaperArray.GetSize();

	CCAD_PaperDoc * pDoc = NULL;
	try {
		pDoc = new CCAD_PaperDoc(this, m_hHeap, nTot);
		m_cPaperArray.Add(pDoc);
	}
	catch(...) {
		return NULL;
	}
//	pDoc->SetInitDoc();
	return	pDoc;
}

CCAD_Sheet * CCAD_Doc::GetSheet()
{
	ASSERT(m_pSpaceDoc != NULL);
	return m_pSpaceDoc->GetSheet();
}

CCAD_UdsMgr* CCAD_Doc::GetUdsMgr()
{
	ASSERT(m_pSpaceDoc != NULL);
	return m_pSpaceDoc->GetUdsMgr();
}

CCAD_Uds* CCAD_Doc::GetUds()
{
	ASSERT(m_pSpaceDoc != NULL);
	return m_pSpaceDoc->GetUds();
}

CCAD_Uds *CCAD_Doc::GetUds(short nSpace, short nUds)
{
	return (CCAD_Uds *) GetSpaceDoc(nSpace)->GetUdsMgr()->GetStyle(nUds);
}

short CCAD_Doc::GetActiveUdsID()
{
	CCAD_UdsMgr	*pUdsMgr = GetUdsMgr();
	if (pUdsMgr == NULL)
		return -1;
	return pUdsMgr->m_nAct;
}


CCAD_Grid* CCAD_Doc::GetGrid()
{
	CCAD_Uds *pUds = GetUds();
	ASSERT(pUds != NULL);
	return &pUds->m_cGrid;
}

void CCAD_Doc::CountWorldRange(CCAD_DC * pDC, CCAD_Rect & cRect, BYTE bFlag) const
{
	if (m_pSpaceDoc != NULL)
		m_pSpaceDoc->CountWorldRange(pDC, cRect);
}

void CCAD_Doc::CountVcsRange(CCAD_DC * pDC, CCAD_Rect & cRect) const
{
	if (m_pSpaceDoc != NULL)
		m_pSpaceDoc->CountVcsRange(pDC, cRect);
}

long CCAD_Doc::CountEntity() const
{
	return m_pSpaceDoc->CountEntity();
}

CCAD_StyleMgr *CCAD_Doc::GetStyleMgr(LPARAM lParam)
{
	CCAD_StyleMgr	*pMgr = CCAD_MgrDoc::GetStyleMgr(lParam);
	if (pMgr != NULL)
		return pMgr;

	BYTE bStyle = (BYTE) lParam;
	switch (bStyle) {
		case SS_LAYER:
			return m_pLayerMgr;
		case SS_UDS:
			return GetUdsMgr();
	}
	return NULL;
}

CCAD_StyleMgr *CCAD_Doc::GetSpaceStyleMgr(LPARAM lParam)
{
	SCAD_EntityInfo	*pInfo = (SCAD_EntityInfo *) lParam;
//	CCAD_StyleMgr	*pMgr = GetSpaceDoc(pInfo->nSpace)->GetUdsMgr();

	switch (pInfo->lParam) {
		case SS_UDS:
			return GetSpaceDoc(pInfo->nSpace)->GetUdsMgr();
	}
	return NULL;
}

BOOL CCAD_Doc::UpdateDoc(short msg, long lParam1, long lParam2)
{
    return true;
}
///////////////////////////////
//
void CCAD_Doc::CreateSceneGraph(CCAD_PaperDoc *pPaperDoc)
{
	for(int i = 0; i < pPaperDoc->m_pUdsMgr->TotIndex(); i++)
	{
		CCAD_Uds *pUds = (CCAD_Uds *)pPaperDoc->m_pUdsMgr->GetStyle(i);
		CCAD_Rect cRect;
		pUds->m_pMgrEntity->m_pDrawState = new BYTE[pUds->m_pMgrEntity->m_cArray.GetSize()];
		memset(pUds->m_pMgrEntity->m_pDrawState, 0, sizeof(BYTE) * pUds->m_pMgrEntity->m_cArray.GetSize());
		pUds->m_pMgrEntity->CountWorldRange(NULL, cRect);
		CCAD_PtrArray cArray;
		cArray.Copy(pUds->m_pMgrEntity->m_cArray);
		pUds->m_pMgrEntity->QuickSort_BoundSize(cArray, 0, cArray.GetCount());
		pUds->m_pSceneGraph = new CSceneGraph((CCAD_Style*)pUds, cRect, NULL);
		pUds->m_pSceneGraph->InitSceneGraph(cArray);
		cArray.RemoveAll();
	}
}

void CCAD_Doc::SetSpaceDoc( CCAD_SpaceDoc * pDoc )
{
	if ( pDoc != NULL )
		m_pSpaceDoc = pDoc;
}

void	CCAD_Doc::EmptyDoc()
{
	m_pPenStyleMgr->RemoveAll();
	m_pImageMgr->RemoveAll();
	m_pTextStyleMgr->RemoveAll();
	m_pDimStyleMgr->RemoveAll();

	m_pMgrBlock->RemoveAll();
	m_pMgrDimBlock->RemoveAll();

	memset(&m_sInform, 0, sizeof (m_sInform));

	m_pLayerMgr->RemoveAll();
	m_pLayerMgr->m_bOnlyActLayer = NO;

	CCAD_PaperDoc * pPaperDoc;
	for (short i=0; i<3; i++ ) {
		pPaperDoc = (CCAD_PaperDoc *)GetSpaceDoc(i);
		if ( pPaperDoc == NULL )
			continue;
		pPaperDoc->GetUdsMgr()->RemoveAll();
	}
	
	m_cInitViewInfo.m_fScale = -1;	//	for InitSatate update.
}

