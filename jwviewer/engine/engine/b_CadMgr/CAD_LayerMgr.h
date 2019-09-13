// CAD_LayerMgr.h: interface for the CCAD_LayerMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_LAYERMGR_H)
#define AFX_CAD_LAYERMGR_H


#include "CAD_StyleMgr.h"
#include "CAD_Layer.h"

 

class   CCAD_LayerMgr : public CCAD_StyleMgr {
public:
	BYTE		m_bOnlyActLayer;	// not serialized
public:
	CCAD_LayerMgr();
	virtual ~CCAD_LayerMgr();

	virtual CCAD_Style *NewStyle();

	BOOL	IsVisible(short n) const;
};

 

#endif
