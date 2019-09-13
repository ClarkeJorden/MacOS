#ifndef _CAD_GEM_BELT_H_
#define _CAD_GEM_BELT_H_

#include "CAD_GemEntity.h"
#include "CAD_GemArc.h"
#include "CAD_GemPline2D.h"

#define		START_INDEX		0x01
#define		END_INDEX		0x02

class  CCAD_GemBvertex : public CCAD_GemPvertex
{
public:
	CCAD_GemBvertex();
	CCAD_GemBvertex & operator=( const CCAD_GemBvertex & );

public:
	CAD_FLOAT	m_fStartW1;		// right
	CAD_FLOAT	m_fStartW2;		// left
	CAD_FLOAT	m_fEndW1;		// right
	CAD_FLOAT	m_fEndW2;		// left

public:
	void		Read(CCAD_File & cFile);

};

class  CCAD_GemBelt : public CCAD_Gem2DEntity
{
public:
	CCAD_GemBelt();
	~CCAD_GemBelt();

public:
	BYTE				m_bClose;
	BYTE				m_bDraw;
	short				m_nTot;
	CAD_FLOAT			m_fLength;
	CAD_FLOAT			m_fEmpty;
	CCAD_GemBvertex*	m_pVertex;

public:
	virtual void	MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual void	Remove();
	virtual BOOL	Read(CCAD_File& cFile);
	virtual BOOL	Draw(CCAD_DC *pCadDC, BOOL bRedraw = FALSE);
	virtual void	CountBodyRange(CCAD_Rect& cRange);
	virtual void	CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange);

	void	Init();
	void	RemoveAll(void);

protected:
	void	FrameDraw(CCAD_DC *pDC);
	BYTE	DrawBeltStyle(CCAD_DC *pDC) const;
};

class  CCAD_GemBvtxPara : public CCAD_GemBvertex {
public:
	CCAD_GemBvtxPara();
	~CCAD_GemBvtxPara();
	CCAD_GemBvtxPara & operator=(const CCAD_GemBvtxPara &);
	CCAD_GemBvtxPara & operator=(const CCAD_GemBvertex &);

public:
	CCAD_Vector		m_cRp[4];
	CCAD_GemArc		m_cArc;
	
public:
	void	CountPara(const CCAD_Coord3D &cBcs, CCAD_GemBvtxPara *, const CCAD_Vector &);
	void	StyleDraw(CCAD_DC *pDC, const CCAD_GemBelt &, const CCAD_Vector &, CAD_FLOAT &);
	void	Draw(CCAD_DC *pDC);
	void	FrameDraw(CCAD_DC *pDC, const CCAD_Coord3D &cBcs, BYTE);
	void	DrawStyleLine(CCAD_DC *pDC, const CCAD_Coord3D &cBcs, CAD_FLOAT, const CCAD_Angle &, CAD_FLOAT, CAD_FLOAT);
	void	DrawStyleArc(CCAD_DC *pDC, CAD_FLOAT, CAD_FLOAT);
	void	CountBodyRange(CCAD_Rect& cRange);
	void	CountVcsRange(CCAD_DC *pDC, CCAD_Rect &cRc);
};

#endif
