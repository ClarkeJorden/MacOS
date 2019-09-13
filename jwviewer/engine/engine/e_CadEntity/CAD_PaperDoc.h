#if !defined(AFX_CAD_PAPERDOC_H)
#define AFX_CAD_PAPERDOC_H

#include "CAD_Doc.h"
#include "CAD_UdsMgr.h"

class   CCAD_PaperDoc : public CCAD_SpaceDoc {
public:
	CCAD_Sheet			m_cSheet;
	CCAD_UdsMgr	*		m_pUdsMgr;

public:
	CCAD_PaperDoc(CCAD_Doc*, HANDLE hHeap, short);
	virtual ~CCAD_PaperDoc();

	BOOL	Read(CCAD_File & file);

	CCAD_UdsMgr		*GetUdsMgr() { return m_pUdsMgr; }
	CCAD_Uds		*GetUds() {return (CCAD_Uds*)(m_pUdsMgr->GetActStyle());}
	CCAD_Sheet		*GetSheet() { return &m_cSheet; }

	void		CountWorldRange(CCAD_DC * pDC, CCAD_Rect &, BYTE bFlag = E_DRAWING) const;
	void		GetDefaultWorldRect(CCAD_DC * pDC, CCAD_Rect & cOutRect) const;
	void		CountVcsRange(CCAD_DC * pDC, CCAD_Rect &) const;
	void		GetDefaultVcsRect(CCAD_DC * pDC, CCAD_Rect & cOutRect) const;
	long		CountEntity() const;

	BOOL		IsEntities();
};

#endif
