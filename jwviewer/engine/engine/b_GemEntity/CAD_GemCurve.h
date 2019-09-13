#ifndef _CAD_GEM_CURVE_H_
#define _CAD_GEM_CURVE_H_

#include "CAD_GemEntity.h"

#define DEF_CURVE_DIVNUM	20

class  CCAD_GemCurve : public CCAD_GemThickEntity  
{
public:
	CCAD_GemCurve();
	virtual ~CCAD_GemCurve();

public:
	BYTE			m_bKind;
	BYTE			m_bClose;
	BYTE			m_bFirstClose;
	BYTE			m_bDraw;
	short			m_nTot;
	CCAD_Vector *	m_pPt;
	short			m_nCurveTot;
	CCAD_Vector	*	m_pCurvePt;
	CAD_FLOAT*		m_pWeight;
	short			m_nKnotTot;
	CAD_FLOAT*		m_pKnot;

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
	BOOL ThreeSplineTempVariables();
	BOOL Get3SplinePt(SCAD_POINT &cPt, long step, CAD_FLOAT fLength, long i, long j) const;
	BOOL Get2SplinePt(SCAD_POINT &cPt, long step, CAD_FLOAT fLength, long i, long j) const;
	void CountDrawPt( int & nTotPt, short nCurveDiv=DEF_CURVE_DIVNUM ) const;
	void GetDrawPt( SCAD_POINT * pPt, int & nTotPt, short nCurveDiv=DEF_CURVE_DIVNUM ) const;

	void FrameDraw(CCAD_DC *pDC, short nTotPt, CCAD_Vector *pPt);
	BOOL PtDirect(const CCAD_Vector &);
};

#endif
