#if !defined(AFX_CAD_MGRBLOCK_H)
#define AFX_CAD_MGRBLOCK_H

#include	"CAD_StyleMgr.h"
#include	"CAD_Block.h"

class	CCAD_MgrEntity;
class	CCAD_Doc;

class  CCAD_MgrBlock : public CCAD_StyleMgr {
public:
	CCAD_Doc *	m_pDoc;
	HANDLE		m_hHeap;

public:
	CCAD_MgrBlock(HANDLE hHeap, CCAD_Doc * pDoc);
	virtual ~CCAD_MgrBlock();

	virtual CCAD_Style *NewStyle();
	virtual	BOOL	Add(CCAD_Style *pStyle);

};

#endif
