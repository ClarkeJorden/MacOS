
#include "stdafx.h"
#include "CAD_TextStyleMgr.h"
#include "resource.h"



CCAD_TextStyleMgr::CCAD_TextStyleMgr()
{
}

CCAD_TextStyleMgr::~CCAD_TextStyleMgr()
{
}


BOOL CCAD_TextStyleMgr::SetDefault()
{
	RemoveAll();

	CCAD_TextStyle	*pStyle = NULL;
	try {
		pStyle = new CCAD_TextStyle;
		sprintf(pStyle->m_szName, "%s", DEFAULTSTYLE);
		m_cArray.Add(pStyle);
		m_nAct = 0;
	}
	catch(...) {
		return FALSE;
	}
	return TRUE;
}


CCAD_Style *CCAD_TextStyleMgr::NewStyle()
{
	if (m_cArray.GetSize() == MAX_STYLE_COUNT) {
		return NULL;
	}

	try {
		CCAD_TextStyle * pNewStyle = new CCAD_TextStyle;
		return pNewStyle;
	}
	catch(...) {
	}
	return NULL;
}

 
