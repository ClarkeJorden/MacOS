//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_DIMSTYLEMGR_H)
#define AFX_CAD_DIMSTYLEMGR_H


#include "CAD_StyleMgr.h"
#include "CAD_DimStyle.h"

 

class   CCAD_DimStyleMgr : public CCAD_StyleMgr {
public:
	CCAD_DimStyleMgr();
	virtual ~CCAD_DimStyleMgr();

	virtual CCAD_Style *NewStyle();

	BOOL	SetDefault();

public:
	SCAD_LEADERSETTING m_sLeaderSetting;
};

 

#endif
