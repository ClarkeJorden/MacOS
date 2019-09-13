// CAD_GemEntity.h: interface for the CCAD_GemEntity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_GEMENTITY_H)
#define AFX_CAD_GEMENTITY_H

#include "CAD_Vector.h"
#include "CAD_Coord3D.h"
#include "CAD_DC.h"
#include "CAD_Linear2D.h"
#include "CAD_Curve2D.h"
#include "CAD_Parabola2D.h"
#include "CAD_Equation2D.h"

class   CCAD_GemEntity {
public:
	CCAD_GemEntity();
	virtual ~CCAD_GemEntity();

public:
	CCAD_Coord3D	m_cBcs;

public:
	//-------------------------------
	virtual BOOL	IsSprite(){return FALSE;}

	//	for read and read.
	virtual CCAD_GemEntity & operator=(const CCAD_GemEntity & cGemEntity);
	virtual void MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual void Remove(){};
	virtual BOOL Read(CCAD_File & cFile);
	
	//-------------------------------
	//	for flag.
	virtual BOOL	IsClosed(){return FALSE;}
	virtual BOOL	IsThick(){return FALSE;}

	virtual CAD_FLOAT GetThick() const {return 0.0f;}
	virtual void	SetThick(CAD_FLOAT /*fThick*/){}
	CCAD_Coord3D* 	GetBCS(){return &m_cBcs;}
	
	//-------------------------------
	//	for draw.
	virtual BOOL Drawing(CCAD_DC* pCadDC, BYTE bState, BOOL blRedraw);
	virtual void GetDspFileData(CCAD_DC *pCadDC) { Drawing(pCadDC, E_SELECTED, false); }
	virtual BOOL Draw(CCAD_DC*, BOOL/* bRedraw = FALSE*/) { return true; }
	//-------------------------------
	//	for count.
	virtual void CountBodyRange(CCAD_Rect&){}
	virtual void CountVcsRange(CCAD_DC *, CCAD_Rect &){}
	
	virtual void Mirror(CCAD_Coord3D& cCoord);
	//-------------------------------
	virtual WORD	GetCurvePoint(SCAD_POINT *spDivPnt = NULL){return 0;}


public:
	void CountVCSRange(CCAD_DC *pCadDC, CCAD_Rect& cRange);
	void ConvToBase(SCAD_POINT &sPt) const;
	void ConvFromBase(SCAD_POINT &sPt) const;
};

class   CCAD_Gem2DEntity : public CCAD_GemEntity
{
public:
	CCAD_Gem2DEntity();
	virtual ~CCAD_Gem2DEntity();
	virtual CCAD_Gem2DEntity & operator=(const CCAD_Gem2DEntity & cGem2DEntity);
};

class   CCAD_Gem3DEntity : public CCAD_GemEntity
{
public:
	short				m_nTexture;

public:
	CCAD_Gem3DEntity();
	virtual ~CCAD_Gem3DEntity();

public:
	virtual CCAD_Gem3DEntity & operator=(const CCAD_Gem3DEntity & cGem3DEntity);
	virtual void MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
    virtual BOOL Read(CCAD_File & cFile) { return 1; }
	//--------------------------------
};

class   CCAD_GemThickEntity : public CCAD_Gem3DEntity
{
public:
	CCAD_GemThickEntity();
	virtual ~CCAD_GemThickEntity();

public:
	CCAD_Vector m_cThick;

public:
	virtual CCAD_GemThickEntity & operator=(const CCAD_GemThickEntity & cGemThickEntity);
	virtual void MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual BOOL Read(CCAD_File & cFile);
	//--------------------------------
	virtual BOOL IsThick() { return true; }
	virtual CAD_FLOAT GetThick() const {return m_cThick.m_fZ;}
	virtual void SetThick(CAD_FLOAT fThick){m_cThick.m_fZ = fThick;}
	//--------------------------------
	virtual void CountBodyRange(CCAD_Rect& cRange);
	//--------------------------------
};

#endif
