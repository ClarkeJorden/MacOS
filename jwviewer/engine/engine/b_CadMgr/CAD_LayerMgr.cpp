#include "stdafx.h"
#include "CAD_LayerMgr.h"

 

CCAD_LayerMgr::CCAD_LayerMgr()
{
	m_bOnlyActLayer = NO;
}

CCAD_LayerMgr::~CCAD_LayerMgr()
{
}


CCAD_Style *CCAD_LayerMgr::NewStyle()
{
	if (m_cArray.GetSize() == MAX_STYLE_COUNT) {
		return NULL;
	}

	try {
		CCAD_Layer * pNewStyle = new CCAD_Layer;
		return pNewStyle;
	}
	catch(...) {
	}
	return NULL;
}


BOOL CCAD_LayerMgr::IsVisible(short n) const
{
	if (n < 0 || n >= m_cArray.GetSize())
		return FALSE;
	if (m_bOnlyActLayer == YES && m_nAct != n)
		return FALSE;
	
	CCAD_Layer * pLayer = (CCAD_Layer *)m_cArray[n];
	return pLayer->IsVisible();
}


 
