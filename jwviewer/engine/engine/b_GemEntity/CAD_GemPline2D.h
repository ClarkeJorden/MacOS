#ifndef _CAD_GEM_PLINE2D_H_
#define _CAD_GEM_PLINE2D_H_

#include "CAD_GemEntity.h"
#include "CAD_GemLine.h"
#include "CAD_GemArc.h"
#include "CAD_GemEllipse.h"
#include "CAD_GemEllipseArc.h"

class  CCAD_GemPvertex  {
public:
	CCAD_Vector		m_cPt;
	CCAD_Angle		m_cBulge;

public:
	CCAD_GemPvertex();
	CCAD_GemPvertex & operator=(const CCAD_GemPvertex &);
	void	SetBulge(const CCAD_Vector &, const CCAD_Vector &);
	void	SetBulge(const CCAD_Vector &, const CCAD_GemArc &);
	BOOL	GetArc(CCAD_GemArc &, const CCAD_Vector &) const;
	BOOL	Draw(CCAD_GemThickEntity *pThickEntity, const CCAD_Vector & cPt, CCAD_DC *pDC, BOOL blRedraw);

	void	Read(CCAD_File & cFile);

	// For OSnap
	WORD	GetCurvePoint(CCAD_GemThickEntity *pThickEntity, const CCAD_Vector & cPt, SCAD_POINT *spDivPnt = NULL);
	void	CountBodyRange(CCAD_GemThickEntity *pThickEntity, const CCAD_Vector & cPt, CCAD_Rect &cRange);
	void	CountVcsRange(CCAD_DC *pCadDC, CCAD_GemThickEntity *pThickEntity, const CCAD_Vector & cPt, CCAD_Rect &cRange);
};


class  CCAD_GemPline2D : public CCAD_GemThickEntity  
{
public:
	CCAD_GemPline2D();
	virtual ~CCAD_GemPline2D();

public:
	BYTE				m_bClose;
	short				m_nTot;
	CCAD_GemPvertex  *	m_pVertex;

public:
	virtual void	MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual void	Remove();
	virtual BOOL	Read(CCAD_File& cFile);
	virtual BOOL	IsClosed();
	virtual BOOL	Draw(CCAD_DC *pCadDC, BOOL bRedraw = FALSE);
	virtual void	CountBodyRange(CCAD_Rect& cRange);
	virtual void	CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange);
	virtual WORD	GetCurvePoint(SCAD_POINT *spDivPnt = NULL);

	void Init();
	void RemoveAll();

};

#endif
