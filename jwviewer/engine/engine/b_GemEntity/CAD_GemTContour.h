
#ifndef __CONTOUR__
#define __CONTOUR__

#include "CAD_GemEntity.h"
#include "Kon_Array.h"

typedef	struct	tagPATCH {
	short		nIndex[3];
	void		Read(CCAD_File &cFile)
	{
		cFile.ReadShort(&nIndex[0]);
		cFile.ReadShort(&nIndex[1]);
		cFile.ReadShort(&nIndex[2]);
	};
} SCAD_PATCH;

typedef	CKon_Array <CCAD_Point, CCAD_Point>	CPointArray;
typedef	CKon_Array <short, short>	CChildArray;

class  CCAD_GemTContour : public CCAD_GemThickEntity  
{
private:

public:
	virtual		~CCAD_GemTContour();
	CCAD_GemTContour();
	CCAD_GemTContour(const CCAD_GemTContour &cOther);
	CCAD_GemTContour &operator= (const	CCAD_GemTContour	&cOther);

	BOOL		blJudgRotDir ();
	void		AddPoint(const	CCAD_Point	&cPt);
	void		GetXMaxPoint(void);
	void		AddChild(short nID);
	void		InvertContourDir(void);
	BOOL		m_blInFlag;		// TRUE: interior, FALSE: outter
	BOOL		m_blParent;		// TRUE: there is children, FALSE: there isn`t children
	CCAD_Point	m_cXMax;
	CPointArray	m_cPointArray;
	CChildArray m_cChildArray;
};

#endif	// __CONTOUR__
