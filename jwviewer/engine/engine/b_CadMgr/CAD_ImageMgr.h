//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_IMAGEMGR_H)
#define AFX_CAD_IMAGEMGR_H


#include "CAD_StyleMgr.h"
#include "CAD_Image.h"

 


class   CCAD_ImageMgr : public CCAD_StyleMgr {
public:
	HANDLE		m_hHeap;

public:
	CCAD_ImageMgr(HANDLE hHeap);
	virtual ~CCAD_ImageMgr();

public:
	virtual	CCAD_Style*	NewStyle();
};

 

#endif
