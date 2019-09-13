#ifndef	_DEFINE_TYPE_H_
#define	_DEFINE_TYPE_H_

#include <wchar.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <MacTypes.h>
#include <objc/objc.h>

typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned int    DWORD;
typedef char CHAR;

typedef unsigned char   UCHAR;
typedef unsigned int    UINT;
typedef unsigned short  USHORT;

typedef int             LONG;
typedef unsigned int    ULONG;

typedef unsigned char   boolean;

typedef int             INT32;
typedef unsigned int    UINT32;
// #ifndef __int64
// typedef long long       __int64;
// #endif

typedef unsigned int    UINT_PTR;
typedef long            LONG_PTR;
typedef int             INT_PTR;
// #ifndef LONGLONG
// typedef __int64         LONGLONG;
// #endif
typedef unsigned long long    ULONGLONG;

typedef BYTE *          LPBYTE;
typedef BYTE *          PBYTE;

// passing and returning type
typedef UINT_PTR        WPARAM;
typedef LONG_PTR        LPARAM;
typedef LONG_PTR        LRESULT;

typedef void*	LPVOID;
typedef int*	LPINT;

#define LOBYTE(w)           ((BYTE)(((DWORD)(w)) & 0xff))

//string
typedef char*	LPSTR;
typedef const char*	LPCSTR;

typedef unsigned short  TCHAR;
typedef TCHAR*          LPTSTR;
typedef const TCHAR*    LPCTSTR;
typedef DWORD   COLORREF;
#define LF_FACESIZE         32
typedef struct tagLOGFONTA
{
	LONG      lfHeight;
	LONG      lfWidth;
	LONG      lfEscapement;
	LONG      lfOrientation;
	LONG      lfWeight;
	LONG      lfAscent;

	BYTE      lfBold;
	BYTE      lfItalic;
	BYTE      lfUnderline;
	BYTE      lfStrikeOut;
	BYTE      lfCharSet;
	BYTE      lfOutPrecision;
	BYTE      lfClipPrecision;
	BYTE      lfQuality;
	BYTE      lfPitchAndFamily;
	CHAR     lfFaceName[LF_FACESIZE];
} LOGFONTA, *PLOGFONTA, /*FAR*/*LPLOGFONTA;

typedef struct tagTEXTMETRIC
{
	LONG        tmHeight;
	LONG        tmAscent;
	LONG        tmDescent;
	LONG        tmInternalLeading;
	LONG        tmExternalLeading;
} TEXTMETRIC, *PTEXTMETRIC, *LPTEXTMETRIC;

#ifndef SEEK_SET	
#define SEEK_SET  0	
#endif

typedef void*           HANDLE;

#ifndef _SYSTEMTIME_
#define _SYSTEMTIME_
typedef struct _SYSTEMTIME
{
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
} 	SYSTEMTIME;
#endif // _SYSTEMTIME_

#define RGBA(r,g,b,a) ((COLORREF)((((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))|(((DWORD)(BYTE)(a))<<24)))
#define RGB(r,g,b) RGBA(r,g,b,255)

#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#define FW_DONTCARE         0
#define FW_THIN             100
#define FW_EXTRALIGHT       200
#define FW_LIGHT            300
#define FW_NORMAL           400
#define FW_MEDIUM           500
#define FW_SEMIBOLD         600
#define FW_BOLD             700
#define FW_EXTRABOLD        800
#define FW_HEAVY            900

typedef struct tagPOINT
{
	LONG x;
	LONG y;
} 	POINT, *PPOINT, *LPPOINT;

typedef struct tagRECT
{
	LONG    left;
	LONG    top;
	LONG    right;
	LONG    bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;

typedef const LPRECT    LPCRECT;

typedef struct tagLOGPEN
{
	UINT        lopnStyle;
	POINT       lopnWidth;
	COLORREF    lopnColor;
} LOGPEN, *PLOGPEN;

typedef struct _FIXED {
	short   value;
	WORD    fract;
} FIXED;

typedef struct tagPOINTFX
{
	FIXED x;
	FIXED y;
} POINTFX;

typedef struct tagTTPOLYCURVE
{
	WORD    wType;
	WORD    cpfx;
	POINTFX apfx[1];
} TTPOLYCURVE;

typedef struct tagTTPOLYGONHEADER
{
	DWORD   cb;
	DWORD   dwType;
	POINTFX pfxStart;
} TTPOLYGONHEADER;

typedef struct tagLOGBRUSH
{
	UINT        lbStyle;
	COLORREF    lbColor;
	ULONG    lbHatch;    // Sundown: lbHatch could hold a HANDLE
} LOGBRUSH, *PLOGBRUSH;

typedef struct tagSIZE
{
	LONG        cx;
	LONG        cy;
} SIZE, *PSIZE, *LPSIZE;

#define SHIFTJIS_CHARSET        128

#define TT_PRIM_LINE       1
#define TT_PRIM_QSPLINE    2
#define TT_PRIM_CSPLINE    3

#define		HEAP_ZERO_MEMORY	0x00000008
#define		HEAP_NO_SERIALIZE	0x00000001

#define PS_SOLID            0
#define PS_DASH             1       /* -------  */
#define PS_DOT              2       /* .......  */
#define PS_DASHDOT          3       /* _._._._  */
#define PS_DASHDOTDOT       4       /* _.._.._  */
#define PS_NULL             5


#define BS_SOLID            0
#define BS_NULL             1
#define BS_HOLLOW           BS_NULL
#define BS_HATCHED          2
#define BS_PATTERN          3
#define BS_INDEXED          4
#define BS_DIBPATTERN       5
#define BS_DIBPATTERNPT     6
#define BS_PATTERN8X8       7
#define BS_DIBPATTERN8X8    8
#define BS_MONOPATTERN      9

#define HS_HORIZONTAL       0       /* ----- */
#define HS_VERTICAL         1       /* ||||| */
#define HS_FDIAGONAL        2       /* \\\\\ */
#define HS_BDIAGONAL        3       /* ///// */
#define HS_CROSS            4       /* +++++ */
#define HS_DIAGCROSS        5       /* xxxxx */

#endif

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif 

typedef struct _tagRGBColor
{
	unsigned short	red;
	unsigned short	green;
	unsigned short	blue;
} RGBColor;

enum UPDATE_MESSAGE	{
	UM_SPACEDOC_CHANGE = 1,
	UM_VIEW_UPDATE,				// Change camera
	UM_RENDER_VIEW,

	UM_UNDO_BEGIN_FLAG = 0x100,	// Start undo in multi step command
	UM_UNDO_END_FLAG = 0x200,	// End undo in multi step command
	UM_UNDO_MSG_FLAG = 0x400,	// UpdateDoc called from OnUndo
};

enum VIEW_UPDATE_CONDITIONS	{
	UPDATE_ENTIRE = 1,

	UPDATE_SETCAMERA,
	UPDATE_GETVIEWINFO,
	UPDATE_SETVIEWINFO,
	UPDATE_3D_VIEW,
	UPDATE_RENDER,
	UPDATE_EXTENT,
	UPDATE_VIEWINIT,
	UPDATE_ALL_VIEW,
	UPDATE_PRESTATE_VIEW,
	UPDATE_RENDER_DISP,

	UPDATE_GET_PREVIEWINFO,
};

enum	ViewType {MODELVIEW, LAYOUTVIEW};

enum SANAK_STYLE{
	SS_LINE, SS_HPATTERN, SS_MLINE, SS_MATERIAL, SS_TEXT, SS_DIMENSION, 
	SS_LAYER, SS_UDS, SS_IMAGE, SS_TEXTURE,	SS_VIEWSTATE, SS_CAMERA, SS_BLOCK,
	SS_DIMBLOCK, SS_GROUP
};

#define ASSERT(a) ((void)(a)) 
#define ASSERT_VALID(a) ((void)(a)) 
#define ASSERT_KINDOF(a, b)
#define AfxThrowInvalidArgException()	{throw 1;}

#define	_max(a, b) ( (a > b) ? a : b )
#define _min(a, b) ( (a < b) ? a : b )

#define _LOBYTE(w)	((BYTE)(w))
#define _HIBYTE(w)	((BYTE)(((WORD)(w) >> 8) & 0xFF))
#define	_SWAP( code ) ( ( _LOBYTE( code ) << 8 ) | _HIBYTE( code ) )
#define _MAKEWORD(a,b) ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))

#define	 	DEVICEDPI				72
#define		INCH_PER_MM				25.4f

typedef struct tagGLPOINT
{
	int x;
	int y;
} 	GLPOINT, *PGLPOINT, *LPGLPOINT;

#define 	CAD_YES			0x01
#define 	CAD_NO			0x00
#define		DISABLE			0xFF

#define ID_VIEW_3D_TOP                  44487
#define ID_VIEW_3D_BOTTOM               44488
#define ID_VIEW_3D_LEFT                 44489
#define ID_VIEW_3D_RIGHT                44490
#define ID_VIEW_3D_FRONT                44491
#define ID_VIEW_3D_BACK                 44492
#define ID_VIEW_3D_NE                   44493
#define ID_VIEW_3D_NW                   44494
#define ID_VIEW_3D_SE                   44495
#define ID_VIEW_3D_SW                   44496

#define SUPPORT_ORTHO_MODE_WRIREFRAME	1 

#endif
