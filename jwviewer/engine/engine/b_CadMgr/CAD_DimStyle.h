//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_DIMSTYLE_H)
#define AFX_CAD_DIMSTYLE_H


#include "CAD_Style.h"

 

#define		DIM_LINE_SHOW_FIRST		0x1
#define		DIM_LINE_SHOW_SECOND	0x2

enum SANAK_DIM_ARROW_STYLE {
	SDAS_CLOSEDFILLED,
	SDAS_CLOSEDBLANK,
	SDAS_CLOSED,
	SDAS_DOT,
	SDAS_ARCHTICK,
	SDAS_OBLIQUE,
	SDAS_OPEN,
	SDAS_ORIGIN,
	SDAS_ORIGIN2,
	SDAS_OPEN90,
	SDAS_OPEN30,
	SDAS_DOTSMALL,
	SDAS_DOTBLANK,
	SDAS_SMALL,
	SDAS_BOXBLANK,
	SDAS_BOXFILLED,
	SDAS_DATUMBLANK,
	SDAS_DATUMFILLED,
	SDAS_INTEGRAL,
	SDAS_NONE,
	SDAS_END
};

#define		DIM_ARROW_CENTER_NONE	0
#define		DIM_ARROW_CENTER_MARK	1
#define		DIM_ARROW_CENTER_LINE	2

#define		DIM_TEXT_HORZ_CENTER	0
#define		DIM_TEXT_HORZ_EXT1		1
#define		DIM_TEXT_HORZ_EXT2		2
#define		DIM_TEXT_HORZ_OVEREXT1	3
#define		DIM_TEXT_HORZ_OVEREXT2	4
#define		DIM_TEXT_VERT_CENTER	0
#define		DIM_TEXT_VERT_ABOVE		1
#define		DIM_TEXT_VERT_OUTSIDE	2
#define		DIM_TEXT_ALIGN_HORZ		0
#define		DIM_TEXT_ALIGN_WITH		1
#define		DIM_TEXT_ALIGN_ISO		2
#define		DIM_TEXT_FIT_BEST		0
#define		DIM_TEXT_FIT_ARROW		1
#define		DIM_TEXT_FIT_TEXT		2
#define		DIM_TEXT_FIT_BOTH		3
#define		DIM_TEXT_FIT_KEEPTEXT	4
#define		DIM_TEXT_PLACE_BESIDE	0
#define		DIM_TEXT_PLACE_LEADER	1
#define		DIM_TEXT_PLACE_NOLEADER	2

#define		DIM_UNIT_PERIOD			0
#define		DIM_UNIT_COMMA			1
#define		DIM_UNIT_SPACE			2
#define		DIM_TOLER_NONE			0
#define		DIM_TOLER_FORMAT_SYM	1
#define		DIM_TOLER_FORMAT_NOSYM	2
#define		DIM_TOLER_FORMAT_LIMIT	3
#define		DIM_TOLER_VERT_MIDDLE	0
#define		DIM_TOLER_VERT_TOP		1
#define		DIM_TOLER_VERT_BOTTOM	2


void	ReverseBits	(void* vByte, int nSize=1);
void	SwapBits	(void* vByte, int n1, int n2=-1);

typedef struct tagSCAD_DimLine {
	WORD		wShow;
	COLORREF	rgbColor;
	short		nThick;
	CAD_FLOAT	fExt;
	CAD_FLOAT	fBase;
	void	Read(CCAD_File &cFile)
	{
		cFile.ReadWord(&wShow);
		cFile.ReadColorRef(&rgbColor);
		cFile.ReadShort(&nThick);
		cFile.ReadCAD_Float(&fExt);
		cFile.ReadCAD_Float(&fBase);
	};
} SCAD_DimLine;

typedef struct tagSCAD_DimArrow {
	short		nFirst;
	short		nSecond;
	short		nLeader;
	CAD_FLOAT	fSize;
	short		nMark;
	CAD_FLOAT	fMarkSize;
	void	Read(CCAD_File &cFile)
	{
		cFile.ReadShort(&nFirst);
		cFile.ReadShort(&nSecond);
		cFile.ReadShort(&nLeader);
		cFile.ReadCAD_Float(&fSize);
		cFile.ReadShort(&nMark);
		cFile.ReadCAD_Float(&fMarkSize);
	};
} SCAD_DimArrow;

typedef struct tagDimTextPos {
	unsigned	horz	:3;
	unsigned	vert	:2;
	unsigned	align	:2;	
	unsigned	rect	:1;
	unsigned	fit		:3;
	unsigned	arrow	:1;
	unsigned	place	:2;
	unsigned	manual	:1;
	unsigned	always	:1;
} DimTextPos;

typedef struct tagSCAD_DimText {
	short		nStyle;
	COLORREF	rgbColor;
	CAD_FLOAT	fHeight;
	CAD_FLOAT	fWidth;
	CAD_FLOAT	fGap;
	DimTextPos	sState;
	void	Read(CCAD_File &cFile)
	{
		cFile.ReadShort(&nStyle);
		cFile.ReadColorRef(&rgbColor);
		cFile.ReadCAD_Float(&fHeight);
		cFile.ReadCAD_Float(&fWidth);
		cFile.ReadCAD_Float(&fGap);
		cFile.Read((BYTE *)&sState,2);
		if(cFile.m_blMacVersion) {
			SwapBits(&sState, 0, 2);
			SwapBits(&sState, 3);
			SwapBits(&sState, 5);
			SwapBits(&sState, 8, 10);
			SwapBits(&sState, 12);
			ReverseBits(&sState,2);
		}
	};
} SCAD_DimText;

typedef struct tagDimUnit {
	unsigned	lengthhead	:1;
	unsigned	lengthtail	:1;
	unsigned	anglehead	:1;
	unsigned	angletail	:1;
	unsigned	decimal		:2;
	unsigned	scalelayout	:1;
	unsigned	reserved	:1;
} DimUnit;

typedef struct tagDimToler {
	unsigned	head		:1;
	unsigned	tail		:1;
	unsigned	format		:2;
	unsigned	vert		:2;
	unsigned	reserved	:2;
} DimToler;

typedef struct tagSCAD_DimUnit {
	short		nLengthUnit;
	short		nAngleUnit;
	CAD_FLOAT	fRound;
	CAD_FLOAT	fScale;
	DimUnit		sState;
	char		szPrefix[LENGTH_NAME];
	char		szSuffix[LENGTH_NAME];
	void	Read(CCAD_File &cFile)
	{
		cFile.ReadShort(&nLengthUnit);
		cFile.ReadShort(&nAngleUnit);
		cFile.ReadCAD_Float(&fRound);
		cFile.ReadCAD_Float(&fScale);
		cFile.ReadByte((BYTE*)&sState);
		if(cFile.m_blMacVersion) {
			SwapBits(&sState, 4);
			ReverseBits(&sState);
		}
		//-------------------------------
		cFile.ReadString((char*)szPrefix, LENGTH_NAME);
		cFile.ReadString((char*)szSuffix, LENGTH_NAME);
	};
} SCAD_DimUnit;

typedef struct tagSCAD_DimToler {
	short		nUnit;
	CAD_FLOAT	fUpper;
	CAD_FLOAT	fLower;
	CAD_FLOAT	fScale;
	DimToler	sState;
	void	Read(CCAD_File &cFile)
	{
		cFile.ReadShort(&nUnit);
		cFile.ReadCAD_Float(&fUpper);
		cFile.ReadCAD_Float(&fLower);
		cFile.ReadCAD_Float(&fScale);
		cFile.ReadByte((BYTE*)&sState);
		if(cFile.m_blMacVersion) {
			SwapBits(&sState, 2);
			SwapBits(&sState, 4);
			SwapBits(&sState, 6);
			ReverseBits(&sState);
		}
	};
} SCAD_DimToler;


class   CCAD_DimStyle : public CCAD_Style {
public:
	SCAD_DimLine	m_sDimLine;
	SCAD_DimLine	m_sDimExt;
	SCAD_DimArrow	m_sDimArrow;
	SCAD_DimText	m_sDimText;
	SCAD_DimUnit	m_sDimUnit;
	SCAD_DimToler	m_sDimToler;

public:
	CCAD_DimStyle();
	CCAD_DimStyle(const CCAD_DimStyle *pStyle);
	virtual ~CCAD_DimStyle();

	CCAD_DimStyle & operator =(const CCAD_DimStyle &cStyle);

	virtual void	Init();
	virtual	BOOL	IsEqual(const CCAD_Style *pStyle);
	virtual void	CopyFrom(const CCAD_Style *pStyle);
	virtual BOOL	Read(CCAD_File &cFile);
};


enum _ANGLECONSTRAINTS
{
	ANGLECONSTRAINTS_ANY = 0,
	ANGLECONSTRAINTS_HORZ,
	ANGLECONSTRAINTS_90,
	ANGLECONSTRAINTS_45,
	ANGLECONSTRAINTS_30,
	ANGLECONSTRAINTS_15
};

#define LEADERPOINT_MINNUMBER		2
#define LEADERPOINT_MAXNUMBER		255

typedef struct _tagSCAD_LEADERSETTING
{
	unsigned bSpline	:1;
	unsigned bNoLimit	:1;
	unsigned nFrame		:6;				// option
	short nMaxNumber;
	short n1stSegment;
	short n2ndSegment;
} SCAD_LEADERSETTING;

 

#endif
