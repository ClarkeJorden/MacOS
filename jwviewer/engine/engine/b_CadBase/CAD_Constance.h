#if !defined(AFX_CAD_CONSTANCE_H_)
#define AFX_CAD_CONSTANCE_H_

//////////////////////////////////////////////////////////////////////
// global value
//
#ifdef _WIN32
	#define uint64	unsigned __int64
#else
	typedef unsigned long long uint64;
#endif


#define 	CAD_FLOAT			float

#define		CAD_ZERO			((CAD_FLOAT)1.0e-8)
#define		IS_ZERO(d)			((d > -CAD_ZERO) && (d < CAD_ZERO))

#define		SCALA_ACCY			(CAD_FLOAT)1.0e-4	


//////////////////////////////////////////////////////////////////////
// flag of status
//
#define 	YES				0x01
#define 	NO				0x00
#define		DISABLE			0xFF

#define		DISABLE_INDEX	(short)-1

//////////////////////////////////////////////////////////////////////
// value of vector
//
#define 	MAX_DOUBLE		3.4E+9F
#define 	MIN_DOUBLE		-3.4E+9F
#define 	ZERO_DOUBLE		3.4E-9F
#define		MAX_LONG		0x7FFFFFFF
#define		MIN_LONG		-0x7FFFFFFF

#define	DEFAULT_ACCURANCE	0.01F


//////////////////////////////////////////////////////////////////////
// value of angle
//
#define		PAI					3.1415926535897932384626433832795F
#define 	NINETY				90
#define 	DOUBLE_NINETY		180
#define 	LONGDOUBLE_NINETY	360

//////////////////////////////////////////////////////////////////////
//
#define		INCH_UNIT			25.4f

//////////////////////////////////////////////////////////////////////
// length of strings
//
#define		LENGTH_VALUE		16
#define		LENGTH_NAME			32			//	old = 16
#define		EXT_LENGTH_NAME		256			//	Modify212
#define		LENGTH_DSCRIPT		64			//	old = 32
#define		LENGTH_MESSAGE		80			//	= old
#define		MAX_STR				256
#define		MAX_DIMSTR			1024

//////////////////////////////////////////////////////////////////////
// pen, brush priority
//
#define		PRI_PEN_COLOR		0x0100
#define		PRI_PEN_STYLE		0x0200
#define		PRI_PEN_THICK		0x0400
#define		PRI_PEN_SCALE		0x0800
#define		PRI_BRUSH_COLOR		0x1000
#define		PRI_BRUSH_PATTERN	0x2000


//////////////////////////////////////////////////////////////////////
// Entity Flag
//
#define		E_DELETE			0
#define		E_DRAWING			1
#define		E_SELECTED			2


//////////////////////////////////////////////////////////////////////
// Entity Code
//
#define		C_NOENTITY			0
#define		C_POINT				1
#define		C_LINE				2
#define		C_RAY				3
#define		C_ARC				4
#define		C_CIRCLE			5
#define		C_ELLPARC			6
#define		C_ELLIPSE			7
#define		C_PLINE	 			8
#define		C_BELT	 			9
#define		C_CURVE	 			12
#define		C_PAINT				13			// = region
#define		C_HATCH	 			14
#define		C_BLOCK				15
#define		C_TEXT				16
#define		C_DOC				17			// = multi text
#define		C_IMAGE				18
#define		C_DIMLINEAR			19
#define		C_DIMANGULAR		20
#define		C_DIMRADIAL			21
#define		C_DIMLEADER			22

//////////////////////////////////////////////////////////////////////
// brush property
//
#define		BRS_HORIZONTAL		0
#define		BRS_VERTICAL		1
#define		BRS_FDIAGONAL		2			//	HS_BDIAGONAL	/* ///// */
#define		BRS_BDIAGONAL		3			//	HS_FDIAGONAL	/* \\\\\ */
#define		BRS_CROSS			4
#define		BRS_DIAGCROSS		5
#define		BRS_SOLID			6			// Solid Brush Number
#define 	PATTERN_STYLE_NUM	7			// Brush Tot Number


//////////////////////////////////////////////////////////////////////
// environment parameters
//
#define		DRAW_FRAME			0x01
#define		DRAW_PAINT			0x02

#define		ARC_NORMAL			1		// arc type !
#define		ARC_PIE				2		// pie
#define		ARC_CHORD			3		// chord

#define		DRAW_NONE			0		// drawing mode
#define		DRAW_NORMAL			1
#define		DRAW_APPROX			2

#define		MEASURE_LENGTH		0			// point drawing
#define		MEASURE_ANGLE		1
#define		DIVIDE_ENTITY		0
#define		DIVIDE_DRAW			1

#define		CURVE_3SPLINE		0			// curve type
#define		CURVE_2SPLINE		1

#define		POS_LT				0			// Insert Position Flags
#define		POS_LC				1
#define		POS_LB				2
#define		POS_CT				3
#define		POS_CENTER			4
#define		POS_CB				5
#define		POS_RT				6
#define		POS_RC				7
#define		POS_RB				8
#define		POS_BASE			9

//------- Camera define --------------------
#define	CAMERA_WIDTH		60000.0F
#define CAMERA_HEIGHT		CAMERA_WIDTH

#define	CAMERA_GRID_WIDTH	3000.0F
#define	CAMERA_SNAP_WIDTH	1500.0F

//---------------------------------------------
#define		INTERVAL_NO			0
#define		INTERVAL_IN			1
#define		INTERVAL_START		2
#define		INTERVAL_END		3

#define		LINES_NOCROSS		0
#define		TWO_ONLINE			1
#define		FIRST_ONLINE		2
#define		SECOND_ONLINE		3
#define		LINES_CROSS			4

#define		NO_DIRECT			0		// on line
#define		CLOCK_DIRECT		1		// clock side of line
#define		NOCLOCK_DIRECT		2		// no clock side of line

#define		DIM_PREFIX			16		// for symleader
#define		DIM_TAG				16		// for symleader

#define		X_BASE		0x00
#define		Y_BASE		0x01

#define		ROOT_NO		0x00
#define		ROOT_ONE	0x01
#define		ROOT_TWO	0x02
#define		ROOT_THREE	0x03
#define		ROOT_FOUR	0x04

//////////////////////////////////////////////////////////////////////
// file	parameters
//
#define		VERSION_10		0x01


//--------------------------------
#define FONT_DEFAULTNAME	"Arial Unicode MS"

#ifdef _WIN32
#define RGBA(a,b,c)			(RGB(a,b,c) | 0xFF000000)
#endif


#include "CAD_File.h"

#endif // !defined(AFX_CAD_CONSTANCE_H_)
