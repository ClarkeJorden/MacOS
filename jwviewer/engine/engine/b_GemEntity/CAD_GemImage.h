#ifndef _CAD_GEM_IMAGE_H_
#define _CAD_GEM_IMAGE_H_

#include "CAD_GemFrame.h"
#include "Kon_Array.h"
#include "CAD_Size2D.h"

typedef	CKon_Array <POINT, POINT>	CScrnPtArray;

class  CCAD_GemImage : public CCAD_Gem2DEntity 
{
public:
	CCAD_GemImage();
	virtual ~CCAD_GemImage();

public:
	BYTE			m_bDraw;
	short			m_nPtNum;
	CCAD_Vector *	m_pPt;
	POINT		*   m_pScrnPt;
	CCAD_Point	*   m_pTriPoints;
	int				m_nTriCount;

	short			m_nIndex;
	CCAD_Image *	m_pImage;
    char			m_szName[EXT_LENGTH_NAME];  // for JW read
    
    double			m_dRotAngle;	// degree
    BOOL			m_bIsTrim;
    double			m_dTrimLeft;
    double			m_dTrimRight;
    double			m_dTrimTop;
    double			m_dTrimBottom;
    
    // for JW read of trim
    double			m_dBmpWidth;
    double			m_dBmpHeight;
    double			m_dBmpTrim1;
    double			m_dBmpTrim2;
    double			m_dBmpScale;
    // ----------------

public:
	virtual void	MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual void	Remove();
	virtual BOOL	Read(CCAD_File& cFile);
	virtual BOOL	IsClosed(){return TRUE;}

	virtual BOOL	Draw(CCAD_DC *pCadDC, BOOL bRedraw = FALSE);
	virtual void	CountBodyRange(CCAD_Rect& cRange);
	virtual void	CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange);
		
	BOOL	DrawImage(CCAD_DC *pDC);
	BOOL	DrawImageInPixelMode(CCAD_DC *pDC);
	BOOL	DrawImageInTextureMode(CCAD_DC *pDC);

private:
	CCAD_Size		m_sizeDIB;
	BYTE		*m_pBits;
	SCAD_POINT	m_sLptForVCS[4];
	SCAD_POINT	m_sLptForBCS[4];

	short		ConvBCSToScrn(CCAD_DC	*pDC);
	void		ClipPoint(CCAD_DC *pCadDC, SCAD_POINT *spPt, const RECT &);

	BOOL		CreateImage(CCAD_DC	*pDC, short, const RECT &);
	BOOL		CreateDib(int cx, int cy, int ibitcount );
};

#endif
