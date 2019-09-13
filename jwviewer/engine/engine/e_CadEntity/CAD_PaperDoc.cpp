#include "stdafx.h"
#include "CAD_PaperDoc.h"
#include "CAD_MgrEntity.h"

CCAD_PaperDoc::CCAD_PaperDoc(CCAD_Doc *pDoc, HANDLE hHeap, short nID) : CCAD_SpaceDoc(hHeap, nID)
{
	m_pUdsMgr = new CCAD_UdsMgr(pDoc, m_nID, hHeap);
}

CCAD_PaperDoc::~CCAD_PaperDoc()
{
	if (m_pUdsMgr != NULL)
		delete m_pUdsMgr;
}

BOOL	CCAD_PaperDoc::Read( CCAD_File & cFile )
{
   return TRUE;
}

void	CCAD_PaperDoc::CountWorldRange(CCAD_DC * pDC, CCAD_Rect & cOutRect, BYTE bFlag) const
{
	CCAD_Rect	cRc;
	short nTot = m_pUdsMgr->TotIndex();
	int	i;
	for (i=0; i<nTot; i++ ) {
		CCAD_Uds *	pUds = (CCAD_Uds *)m_pUdsMgr->GetStyle(i);

		cRc.Init();
		pUds->m_pMgrEntity->CountWorldRange(pDC, cRc, bFlag);
		if ( !cRc.IsInited() ) {
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
			int	j;
			for (j = 0; j < 8; j++)	{
				ptTemp = pt[j];
				pUds->m_cCoord3D.ConvToBase(ptTemp);
				cOutRect.ResetParameter(ptTemp);
			}
		}
	}

	if ( !cOutRect.IsInited() ) {
		GetDefaultWorldRect(pDC, cOutRect);
	}
}

void	CCAD_PaperDoc::GetDefaultWorldRect(CCAD_DC * pDC, CCAD_Rect & cOutRect) const
{
	cOutRect.m_fMaxZ = cOutRect.m_fMinZ = 0;
	cOutRect.m_fMaxX = m_cSheet.m_cSize.m_fW;
	cOutRect.m_fMinX = 0;
	cOutRect.m_fMaxY = m_cSheet.m_cSize.m_fH;
	cOutRect.m_fMinY = 0;
}

void	CCAD_PaperDoc::CountVcsRange(CCAD_DC * pDC, CCAD_Rect & cOutRect) const
{
	short nTot = m_pUdsMgr->TotIndex();
	int	i;
	for (i=0; i<nTot; i++ ) {
		CCAD_Uds *	pUds = (CCAD_Uds *)m_pUdsMgr->GetStyle(i);
		CCAD_Coord3D cECS = pDC->PushCamera(&pUds->m_cCoord3D);
		pUds->m_pMgrEntity->CountVcsRange(pDC, cOutRect);
		pDC->RestorCamera(cECS);
	}

	CCAD_Camera* pCamera = pDC->m_pCamera;
    if (pCamera == NULL)
		return;
    
	if(0 < cOutRect.m_fMinZ)
		cOutRect.m_fMinZ = 0;
	if(0 > cOutRect.m_fMaxZ)
		cOutRect.m_fMaxZ = 0;

	if(cOutRect.IsInited())
		GetDefaultVcsRect(pDC, cOutRect);
}

void	CCAD_PaperDoc::GetDefaultVcsRect(CCAD_DC * pDC, CCAD_Rect & cOutRect) const
{
	CCAD_Point	pt;
	pt.m_fX = 0;
	pt.m_fY = 0;
	pt.m_fZ = 0;
	pDC->WCStoVCS(pt);
	cOutRect.ResetParameter(pt);
	pt.m_fX = m_cSheet.m_cSize.m_fW;
	pt.m_fY = m_cSheet.m_cSize.m_fH;
	pt.m_fZ = 0;
	pDC->WCStoVCS(pt);
	cOutRect.ResetParameter(pt);
}

long	CCAD_PaperDoc::CountEntity() const
{
	long lCount = 0;
	short nTot = m_pUdsMgr->TotIndex();
	int	i;
	for (i=0; i<nTot; i++ ) {
		CCAD_Uds *	pUds = (CCAD_Uds *)m_pUdsMgr->GetStyle(i);
		lCount += pUds->m_pMgrEntity->CountEntity();
	}
	return lCount;
}

BOOL		CCAD_PaperDoc::IsEntities()
{
	short nTot = m_pUdsMgr->TotIndex();
	int	i;
	for (i=0; i<nTot; i++ ) {
		CCAD_Uds *	pUds = (CCAD_Uds *)m_pUdsMgr->GetStyle(i);
		if ( pUds->m_pMgrEntity->IsEntities() )
			return true;
	}
	return false;
}

 
