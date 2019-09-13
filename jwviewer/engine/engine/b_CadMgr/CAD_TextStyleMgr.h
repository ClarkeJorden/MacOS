#if !defined(AFX_CAD_TEXTSTYLEMGR_H)
#define AFX_CAD_TEXTSTYLEMGR_H


#include "CAD_StyleMgr.h"
#include "CAD_TextStyle.h"

 
class   CCAD_TextStyleMgr : public CCAD_StyleMgr {
public:
	CCAD_TextStyleMgr();
	virtual ~CCAD_TextStyleMgr();

	virtual CCAD_Style *NewStyle();

	BOOL	SetDefault();
};

 

#endif
