#ifndef _CAD_STYLE_H_
#define _CAD_STYLE_H_

#include "CAD_Constance.h"
#include "CAD_PtrArray.h"
#include <stdio.h>

#define		DEFAULTSTYLE	"Standard"


class CCAD_Style
{
public:
	char	m_szName[EXT_LENGTH_NAME];

public:
	CCAD_Style();
	CCAD_Style(const CCAD_Style *pStyle);
	virtual ~CCAD_Style();

	CCAD_Style & operator =(const CCAD_Style &cStyle);

	inline BOOL	IsDeleted() { return false; }

	virtual	void	Init();
	BOOL	IsEqualName(const CCAD_Style *pStyle);
	virtual	BOOL	IsEqual(const CCAD_Style *pStyle);
	virtual void	CopyFrom(const CCAD_Style *pStyle);
	virtual BOOL	Read(CCAD_File &cFile);

};

#endif
