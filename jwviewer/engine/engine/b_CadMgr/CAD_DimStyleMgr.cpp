//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CAD_DimStyleMgr.h"
#include "resource.h"

 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCAD_DimStyleMgr::CCAD_DimStyleMgr()
{
	m_sLeaderSetting.bSpline = 0;
	m_sLeaderSetting.bNoLimit = 0;
	m_sLeaderSetting.nFrame = 0;
	m_sLeaderSetting.nMaxNumber = 3;
	m_sLeaderSetting.n1stSegment = ANGLECONSTRAINTS_ANY;
	m_sLeaderSetting.n2ndSegment = ANGLECONSTRAINTS_ANY;
	// end update
}

CCAD_DimStyleMgr::~CCAD_DimStyleMgr()
{

}


BOOL CCAD_DimStyleMgr::SetDefault()
{
	RemoveAll();

	CCAD_DimStyle	*pStyle = NULL;
	try {
		pStyle = new CCAD_DimStyle;
		sprintf(pStyle->m_szName, "%s", DEFAULTSTYLE);
		m_cArray.Add(pStyle);
		m_nAct = 0;
	}
	catch(...) {
		return FALSE;
	}
	return TRUE;
}


CCAD_Style *CCAD_DimStyleMgr::NewStyle()
{
	if (m_cArray.GetSize() == MAX_STYLE_COUNT) {
		return NULL;
	}

	CCAD_DimStyle * pNewStyle = new CCAD_DimStyle;
	return pNewStyle;
	return NULL;
}


 
