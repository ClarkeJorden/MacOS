// CAD_PenStyleMgr.h: interface for the CAD_PenStyleMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_PENSTYLEMGR_H)
#define AFX_CAD_PENSTYLEMGR_H


#include "CAD_StyleMgr.h"
#include "CAD_PenStyle.h"

 


class   CCAD_PenStyleMgr : public CCAD_StyleMgr {
public:
	CCAD_PenStyleMgr();
	virtual ~CCAD_PenStyleMgr();

	virtual CCAD_Style *NewStyle();

	bool SetDefault();
};

 

#endif
