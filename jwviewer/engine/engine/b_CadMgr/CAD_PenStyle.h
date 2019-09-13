// CAD_PenStyle.h: interface for the CCAD_PenStyle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_PENSTYLE_H)
#define AFX_CAD_PENSTYLE_H


#include "CAD_Style.h"

 

class   CCAD_PenStyle : public CCAD_Style {

#define 	MAX_PENSTYLE_PART	16

public:
	short		m_nTotPart;
	CAD_FLOAT	m_fLength;
	CAD_FLOAT	m_fPart[MAX_PENSTYLE_PART];

public:
	CCAD_PenStyle();
	CCAD_PenStyle(const CCAD_PenStyle *pStyle);
	CCAD_PenStyle(const CCAD_PenStyle *, CAD_FLOAT);
	CCAD_PenStyle & operator =(const CCAD_PenStyle &);

public:
	virtual	void	Init();
	virtual void	CopyFrom(const CCAD_Style *pStyle);
	virtual BOOL	Read(CCAD_File &cFile);
};

 

#endif
