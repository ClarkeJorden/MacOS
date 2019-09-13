#include "stdafx.h"
#include "CAD_UdsMgr.h"

CCAD_UdsMgr::CCAD_UdsMgr(CCAD_Doc * pDoc, short nSpaceDoc, HANDLE hHeap)
{
	m_pDoc = pDoc;
	m_hHeap = hHeap;
	m_nSpaceDoc = nSpaceDoc;
}

CCAD_UdsMgr::~CCAD_UdsMgr()
{
}

BOOL CCAD_UdsMgr::SetDefault()
{
	RemoveAll();
	try {
		CCAD_Uds * pUds = new CCAD_Uds(m_pDoc, m_hHeap, m_nSpaceDoc, TotIndex());
		strcpy(pUds->m_szName, DEFAULTSTYLE);
		m_cArray.Add(pUds);
		m_nAct = 0;
	}
	catch(...) {
		return FALSE;
	}
	return TRUE;
}

CCAD_Style *CCAD_UdsMgr::NewStyle()
{
	if (m_cArray.GetSize() == MAX_STYLE_COUNT) {
		return NULL;
	}

	try {
		CCAD_Uds * pNewStyle = new CCAD_Uds(m_pDoc, m_hHeap, m_nSpaceDoc, TotIndex());
		return pNewStyle;
	}
	catch(...) {
	}
	return NULL;
}

BOOL CCAD_UdsMgr::Add(CCAD_Style *pStyle)
{
	((CCAD_Uds *)pStyle)->m_nID = TotIndex();
	return CCAD_StyleMgr::Add(pStyle);
}

 
