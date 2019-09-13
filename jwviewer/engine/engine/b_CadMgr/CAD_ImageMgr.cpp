//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CAD_ImageMgr.h"

 

CCAD_ImageMgr::CCAD_ImageMgr(HANDLE hHeap)
{
	m_hHeap = hHeap;
}


CCAD_ImageMgr::~CCAD_ImageMgr()
{
}

CCAD_Style *CCAD_ImageMgr::NewStyle()
{
	if (m_cArray.GetSize() == MAX_STYLE_COUNT) {
		return NULL;
	}

	try {
		CCAD_Image * pNewStyle = new CCAD_Image(m_hHeap);
		pNewStyle->m_bImageMgr = YES;
		return pNewStyle;
	}
	catch(...) {
	}
	return NULL;
}

 
