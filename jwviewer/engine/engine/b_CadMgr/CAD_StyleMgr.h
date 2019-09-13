//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_STYLEMGR_H)
#define AFX_CAD_STYLEMGR_H

#include "CAD_File.h"
#include "CAD_Style.h"
#include "CAD_PtrArray.h"

 

#define	MAX_STYLE_COUNT		10000

class CCAD_Style;

class   CCAD_StyleMgr {
public:
	short		m_nAct;
	CCAD_PtrArray	m_cArray;

public:
	CCAD_StyleMgr();
	virtual ~CCAD_StyleMgr();

	virtual	CCAD_Style*	NewStyle() { return NULL; }
	virtual BOOL	Read(CCAD_File & cFile);
	virtual void	RemoveAll();
	virtual BOOL	Add(CCAD_Style *pStyle);

	short		Append(CCAD_Style *pStyle);
	inline short 		GetIndex(CCAD_Style *pDstStyle) const;
	short 		GetNameIndex(CCAD_Style *pDstStyle) const;	
	short 		GetNameIndex(LPCSTR) const;	
	CCAD_Style *GetStyle(int nIndex) const;
	CCAD_Style *GetActStyle() const;
	CCAD_Style *GetStyle(LPCSTR) const;
	short		TotIndex() const { return (short)m_cArray.GetSize(); }
};


 

#endif
