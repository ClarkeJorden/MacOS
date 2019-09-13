// CAD_UdsMgr.h: interface for the CCAD_UdsMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_UDSMGR_H)
#define AFX_CAD_UDSMGR_H

#include "CAD_StyleMgr.h"
#include "CAD_Uds.h"


class   CCAD_UdsMgr : public CCAD_StyleMgr {
public:
	HANDLE			m_hHeap;
	CCAD_Doc	*	m_pDoc;
	short			m_nSpaceDoc;

public:
	CCAD_UdsMgr(CCAD_Doc *, short, HANDLE);
	virtual ~CCAD_UdsMgr();

	virtual CCAD_Style *NewStyle();
	virtual BOOL	Add(CCAD_Style *pStyle);

	BOOL		SetDefault();
};

#endif
