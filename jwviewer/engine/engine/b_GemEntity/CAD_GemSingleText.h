#ifndef __SINGLE_TEXT__
#define __SINGLE_TEXT__

#include "CAD_GemEntity.h"
#include "CAD_GemTContour.h"

const unsigned short	LINE_DIV_NUM = 8;
const unsigned short	QSPLINE_DIV_NUM	= 4;
const unsigned short	MAX_CURVE_POINT	= 200;
const unsigned short	SCREEN_TEXT_SIZE = 32;
const unsigned short	CURVE_DIV_MAX = 100;

#define		LOGFONT_SIZE	100

typedef struct {
	WORD	wDivNum;	/* 0 <  wDivNum < CURVE_DIVMAX */
	float	fA[CURVE_DIV_MAX],
			fB[CURVE_DIV_MAX],
			fC[CURVE_DIV_MAX];
} SCAD_QSpline;

typedef	CKon_Array <CCAD_GemTContour, CCAD_GemTContour>	CContourArray;
typedef CKon_Array <SCAD_POINT, SCAD_POINT>		CSCAD_PointArray;
typedef CKon_Array <SCAD_PATCH, SCAD_PATCH>		CSCAD_PatchArray;
typedef CKon_Array <DWORD, DWORD>				CDWORDArray;

class  CCAD_GemSingleText : public CCAD_GemEntity  
{
private:
	CContourArray	m_cContourArray;
	CAD_FLOAT		m_fWidth, m_fHeight;
	CAD_FLOAT		m_fRateX, m_fRateY;
	CAD_FLOAT		m_fSkew;
	WORD			m_wChar;
	PFONT			m_spFont;
	float			m_fW_HScale;
	CCAD_Vector		m_cBndRect[4];
	CCAD_Point		m_cPos;
	
	//---------------------------
	CDWORDArray *		m_pOffsets;
	//---------------------------
	
	BYTE *  GetTTPOLYGONHEADER(TTPOLYGONHEADER &sTTPolygonHeader, BYTE *pFontData);
	void	GetPointOfGlyph(		CCAD_DC		*pDC,
									//POINTFX		sPntFX,
							const	CCAD_Point	cSrcPnt,
									CCAD_Point	&cPt,
							const	CCAD_Point	&cPtBase,
							const	CAD_FLOAT	fRateX,
							const	CAD_FLOAT	fRateY);
#ifdef  _WIN32
	void	PntFXtoVector(	POINTFX&	sPntFX,	CCAD_Point	&sPnt);
#endif
	void	SetQSpline(SCAD_QSpline &sQSpline, WORD wDivNum);
	void	SetInOutContour(void);
	BOOL	JudgInOut(	const	CCAD_Point	sMax,
						const	CCAD_GemTContour	&cContour);
#ifdef  _WIN32
	void	AddPoint(			CCAD_GemTContour	&cContour,
						const	CCAD_Point			cPt1,
						const	CCAD_Point			cPt2);
#endif
	BOOL	blJudgRotDir (const CCAD_GemTContour	&cContour);

public:
	CCAD_GemSingleText();
	CCAD_GemSingleText(	const	CCAD_Coord3D	&cBCS,
								const	CCAD_Vector		*rect,
								const	CAD_FLOAT		fW_HRatio,
								const	CAD_FLOAT		fSkew,
								WORD	wChar,
								PFONT	font);

	CCAD_GemSingleText(	const CCAD_GemSingleText &cOther);
	CCAD_GemSingleText &operator= (	const CCAD_GemSingleText &cOther);
	virtual ~CCAD_GemSingleText();
#ifdef  _WIN32
	void	ConvGlyphToPoly(		CCAD_DC	*pDC,
									BYTE	*bpFontData,
							const	BYTE	*bpGlyphEnd);
#else
// 	void	ConvGlyphToPoly(		CCAD_DC	*pDC,
// 									FT_Outline * outline);
#endif
	void	QSpline(		CCAD_GemTContour	&cContour,
					const	CCAD_Point			sPt[3],
							SCAD_QSpline		*spQSpl);
#ifdef  _WIN32
	BYTE	*GetTTPOLYCURVE(TTPOLYCURVE	&sTTPolyCurve,
							POINTFX		*sPntFX,
							BYTE			*pFontData);
#endif
	void	Draw(			CCAD_DC		*pDC,
					const	CCAD_Vector	&cThick,
					const	BYTE	bPaint);
	void	CountBodyRange(CCAD_Rect& cRange);
	void	CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange);

	DWORD	MakeTriPatch(			SCAD_MESH_4 *spMesh,
							const	CCAD_Vector	&cThick);
	void	ConvGlyphToBody(CCAD_DC	*pDC);
	void	AdjustPoints(CCAD_Angle cAng, CAD_FLOAT fRatio);
};

#endif		// __SINGLE_TEXT__
