#pragma once

#define		MAX_STRING_LEN		256
#define		PI					3.1415926535897932384626433832795F

typedef		double			DXF_DOUBLE;
typedef		float			DXF_FLOAT;
typedef		unsigned int	DXF_UINT;
typedef		int				DXF_INT;
typedef		unsigned int	DXF_ULONG;
typedef		int             DXF_LONG;
typedef		unsigned short	DXF_USHORT;
typedef		short			DXF_SHORT;
typedef		bool			DXF_BOOL;
typedef		char			DXF_CHAR;
typedef		unsigned char	DXF_BYTE;

// DXF Entity Type
enum EntityType {
		DXF_LINE = 0,
		DXF_ARC,
		DXF_CIRCLE,
		DXF_ELLIPSE,
		DXF_RAY,
		DXF_XLINE,
		DXF_LWPOLYLINE,
		DXF_POLYLINE,
		DXF_POINT,
		DXF_TRACE,
		DXF_SOLID,
		DXF_SPLINE,
		DXF_TEXT,
		DXF_MTEXT,
		DXF_INSERT,
		DXF_DIMENSION,
		DXF_LEADER,
		DXF_HATCH,
		DXF_MLINE,
		DXF_IMAGE,
		DXF_SHAPE,
		DXF_VERTEX,
		DXF_ATTRIB,
		DXF_3DFACE,
		DXF_3DSOLID,
		DXF_ACAD_PROXY_ENTITY,
		DXF_ATTDEF,
		DXF_BODY,
		DXF_OLEFRAME,
		DXF_OLE2FRAME,
		DXF_REGION,
		DXF_VIEWPORT,
		DXF_TOLERANCE,
		DXF_SEQEND
};

// DXF Table Type
enum TableType {
	DXF_APPIDTABLE = 0,
	DXF_BLOCKRECORDTABLE,
	DXF_DIMSTYLETABLE,
	DXF_LAYERTABLE,
	DXF_LTYPETABLE,
	DXF_STYLETABLE,
	DXF_UCSTABLE,
	DXF_VIEWTABLE,
	DXF_VPORTTABLE
};

// DXF Object Type
enum ObjectType {
	DXF_MLINESTYLEOBJECT = 0,
	DXF_GROUPOBJECT,
	DXF_IMAGEDEFOBJECT
};

// Leader Arrow Flag
#define		DXF_LEADER_ARROW_FALSE			0L
#define		DXF_LEADER_ARROW_TRUE			1L

///  Leader Gen Flag
#define		DXF_LEADER_GEN_TEXT				0L
#define		DXF_LEADER_GEN_TOLERANCE		1L
#define		DXF_LEADER_GEN_BLOCK			2L
#define		DXF_LEADER_GEN_NONE				3L

// Leader Path Type
#define		DXF_LEADER_PATH_LINE			0L
#define		DXF_LEADER_PATH_SPLINE			1L

// Leader HookLine Direction Flag
#define		DXF_LEADER_HOOKLINE_BACKWARD	0L
#define		DXF_LEADER_HOOKLINE_TOWARD		1L

// Leader HookLine Flag
#define		DXF_LEADER_HOOKLINE_FALSE		0L
#define		DXF_LEADER_HOOKLINE_TRUE		1L

// Image Clipping State
#define		DXF_IMAGE_CLIPPING_OFF			0L
#define		DXF_IMAGE_CLIPPING_ON			1L

// Image Clipping Boundary Type
#define		DXF_IMAGE_CLIPPING_BOUND_RECT	1L
#define		DXF_IMAGE_CLIPPING_BOUND_POLY	2L

// Image Disp Property
#define		DXF_IMAGE_DEFAULT				0x0
#define		DXF_IMAGE_DISPIMAGE				0x1
#define		DXF_IMAGE_DISPSCREEN			0x2
#define		DXF_IMAGE_USECLIPPING			0x4
#define		DXF_IMAGE_TRANS					0x8

// Polyline Prop Flag (70)
#define		DXF_POLYLINE_DEFAULT					0x00
#define		DXF_POLYLINE_MCLOSEDPOLYGONMESH			0x01
#define		DXF_POLYLINE_CURVEFIT					0x02
#define		DXF_POLYLINE_SPLINEFIT					0x04
#define		DXF_POLYLINE_3DPLINE					0x08
#define		DXF_POLYLINE_3DPOLYGONMESH				0x10
#define		DXF_POLYLINE_NCLOSEDPOLYGONMESH			0x20
#define		DXF_POLYLINE_POLYFACE					0x40
#define		DXF_POLYLINE_CONTINUELINETYPE			0x80

// Polyline Curve/Surface Type (75)
#define		DXF_POLYLINE_CAP_NOSMOOTH				0L
#define		DXF_POLYLINE_2BSPLINE					5L
#define		DXF_POLYLINE_3BSPLINE					6L
#define		DXF_POLYLINE_BEZIER						8L

// Polyline Degree
#define		DXF_POLYLINE_2D							0L
#define		DXF_POLYLINE_3D							1L
#define		DXF_POLYLINE_POLYGONMESH				2L
#define		DXF_POLYLINE_POLYMESH					3L

// Polyline Spline/Fit Cap Type
#define		DXF_POLYLINE_FITNON						0L
#define		DXF_POLYLINE_FITSPLINE					1L
#define		DXF_POLYLINE_FITCURVE					2L

// Attrib Flag
#define		DXF_ATTRIB_DEFAULT						0x0
#define		DXF_ATTRIB_INVISIBLE					0x1
#define		DXF_ATTRIB_CONSTANT						0x2
#define		DXF_ATTRIB_VERIFY						0x4
#define		DXF_ATTRIB_PRESET						0x8

// Spline Flag
#define		DXF_SPLINE_DEFAULT						0x0
#define		DXF_SPLINE_CLOSED						0x1
#define		DXF_SPLINE_PERIODIC						0x2
#define		DXF_SPLINE_RATIONAL						0x4
#define		DXF_SPLINE_PLANAR						0x8

// Vertex Type
#define		DXF_VERTEX_2D							0L
#define		DXF_VERTEX_3D							1L
#define		DXF_VERTEX_MESH							2L

// Vertex Code Values
#define		DXF_VERTEX_2DPOLYLINE					0x00
#define		DXF_VERTEX_CURVEFIT						0x01
#define		DXF_VERTEX_CURVEFIT_LINK				0x02
#define		DXF_VERTEX_RESERVE						0x04
#define		DXF_VERTEX_SPLINEFIT					0x08
#define		DXF_VERTEX_SPLINEPOINT					0x10
#define		DXF_VERTEX_3DPOLYLINE					0x20
#define		DXF_VERTEX_POLYGONMESH					0x40
#define		DXF_VERTEX_POLYMESH						0x80

// Text Generate Code
#define		DXF_TEXT_GEN_DEFAULT					0
#define		DXF_TEXT_GEN_LEFTRIGHT					2
#define		DXF_TEXT_GEN_UPDOWN						4

// Text Horizontal Justification Code
#define		DXF_TEXT_HORZJUST_LEFT					0
#define		DXF_TEXT_HORZJUST_CENTER				1
#define		DXF_TEXT_HORZJUST_RIGHT					2
#define		DXF_TEXT_HORZJUST_ALIGNED				3
#define		DXF_TEXT_HORZJUST_MIDDLE				4
#define		DXF_TEXT_HORZJUST_FIT					5

// Text Vertical Justification Code
#define		DXF_TEXT_VERTJUST_BASELINE				0
#define		DXF_TEXT_VERTJUST_BOTTOM				1
#define		DXF_TEXT_VERTJUST_MIDDLE				2
#define		DXF_TEXT_VERTJUST_TOP					3

// OLE Object Type
#define		DXF_OLE_OBJECT_LINK						1
#define		DXF_OLE_OBJECT_EMBEDDED					2
#define		DXF_OLE_OBJECT_STATIC					3

// OLE Tile Mode
#define		DXF_OLE_MODE_TILE						0
#define		DXF_OLE_MODE_NONTILE					1

// MTEXT attach Code
#define		DXF_MTEXT_ATTACH_LEFT_TOP				1
#define		DXF_MTEXT_ATTACH_CENTER_TOP				2
#define		DXF_MTEXT_ATTACH_RIGHT_TOP				3
#define		DXF_MTEXT_ATTACH_LEFT_MIDDLE			4
#define		DXF_MTEXT_ATTACH_CENTER_MIDDLE			5
#define		DXF_MTEXT_ATTACH_RIGHT_MIDDLE			6
#define		DXF_MTEXT_ATTACH_LEFT_BOTTOM			7
#define		DXF_MTEXT_ATTACH_CENTER_BOTTOM			8
#define		DXF_MTEXT_ATTACH_RIGHT_BOTTOM			9

// MTEXT DRAWING DIRECTION
#define		DXF_MTEXT_DRAWINGDIR_LEFT_RIGHT			1
#define		DXF_MTEXT_DRAWINGDIR_RIGHT_LEFT			2
#define		DXF_MTEXT_DRAWINGDIR_TOP_BOTTOM			3
#define		DXF_MTEXT_DRAWINGDIR_BOTTOM_TOP			4

// Dimension Type
#define		DXF_DIMENSION_RHVTYPE			0x00
#define		DXF_DIMENSION_PARTYPE			0x01
#define		DXF_DIMENSION_ANGTYPE			0x02
#define 	DXF_DIMENSION_DIATYPE			0x03
#define 	DXF_DIMENSION_RADTYPE			0x04
#define 	DXF_DIMENSION_3PTTYPE			0x05
#define 	DXF_DIMENSION_COORDTYPE			0x06
#define 	DXF_DIMENSION_BLOCK				0x20
#define 	DXF_DIMENSION_COORD				0x40
#define 	DXF_DIMENSION_USER				0x80

// Hatch Associative Type
#define		DXF_HATCH_NONAUTO_ASSOCIATIVE	0
#define		DXF_HATCH_AUTO_ASSOCIATIVE		1

// Hatch Edge Type
#define		DXF_HATCH_EDGE_LINE				1
#define		DXF_HATCH_EDGE_ARC				2
#define		DXF_HATCH_EDGE_ELLIPARC			3
#define		DXF_HATCH_EDGE_SPLINE			4
#define		DXF_HATCH_EDGE_POLYLINE			5

// Hatch Boundary Path Flag (bit-coded)
#define		DXF_HATCH_BOUND_DEFAULT			0x00
#define		DXF_HATCH_BOUND_EXTERNAL		0x01
#define		DXF_HATCH_BOUND_POLYLINE		0x02
#define		DXF_HATCH_BOUND_DERIVED			0x04
#define		DXF_HATCH_BOUND_TEXTBOX			0x08
#define		DXF_HATCH_BOUND_OUTMOST			0x10

// Hatching Style
#define		DXF_HATCH_STYLE_ODD				0
#define		DXF_HATCH_STYLE_OUTMOST			1
#define		DXF_HATCH_STYLE_ENTIRE			2

// Hatch Pattern Style
#define		DXF_HATCH_PATTERN_USER			0
#define		DXF_HATCH_PATTERN_PREDEFINED	1
#define		DXF_HATCH_PATTERN_CUSTOM		2

// Hatch Solid Fill Flag
#define		DXF_HATCH_FILL_PATTERN			0
#define		DXF_HATCH_FILL_SOLID			1

// MultiLine Close/Open Flag
#define		DXF_MLINE_OPEN					1
#define		DXF_MLINE_CLOSE					3

// DXF Layer Code
#define		DXF_LAYER_ONTHAW		0L
#define		DXF_LAYER_FREEZE		1L
#define		DXF_LAYER_DEFFROZEN		1L << 1
#define		DXF_LAYER_LOCK			1L << 2

// DXF Style Table Standard Code
#define		DXF_STYLE_SHAPE			0x1
#define		DXF_STYLE_VERTICAL		0x4

// DXF Style Table TextGen Code
#define		DXF_STYLE_TEXTGEN_NONE			0x0
#define		DXF_STYLE_TEXTGEN_BACKWARD		0x2
#define		DXF_STYLE_TEXTGEN_UPSIDEDOWN	0x4

// DXF LType Table Element Type
#define		DXF_LTYPE_DEFAULT		0
#define		DXF_LTYPE_TEXT			2
#define		DXF_LTYPE_SHAPE			4

// MLINESTYLE object Flag
#define		DXF_MLINESTYLE_FILLON		0x0001
#define		DXF_MLINESTYLE_JOINMITTER	0x0002
#define		DXF_MLINESTYLE_STARTLINE	0x0010
#define		DXF_MLINESTYLE_STARTINARC	0x0020
#define		DXF_MLINESTYLE_STARTOUTARC	0x0040
#define		DXF_MLINESTYLE_ENDLINE		0x0100
#define		DXF_MLINESTYLE_ENDINARC		0x0200
#define		DXF_MLINESTYLE_ENDOUTARC	0x0400

// IMAGEDEF Object Resolution Unit
#define		DXF_IMAGEDEF_UNIT_NONE			0
#define		DXF_IMAGEDEF_UNIT_CENTIMETER	2
#define		DXF_IMAGEDEF_UNIT_INCH			5

// IMAGEDEF Object Load Flag
#define		DXF_IMAGEDEF_UNLOAD				0
#define		DXF_IMAGEDEF_LOAD				1

// GROUP object Unnamed Flag
#define		DXF_GROUP_NAMED					0L
#define		DXF_GROUP_UNNAMED				1L

// GROUP object Select Flag
#define		DXF_GROUP_UNSELECTABLE			0L
#define		DXF_GROUP_SELECTABLE			1L

// ENTITY Visibility Flag
#define		DXF_VISIBILITY_ON				0L
#define		DXF_VISIBILITY_OFF				1L

// DXF Special Color Index
#define		DXF_COLOR_BYBLOCK				0L
#define		DXF_COLOR_BYLAYER				256L

// DXF Entity Space Type
#define		DXF_SPACE_MODEL					0L
#define		DXF_SPACE_PAPER					1L

// DXF MText Fraction Direction Type
#define		DXF_MTEXT_FRACTION_BOTTOM		0
#define		DXF_MTEXT_FRACTION_MIDDLE		1
#define		DXF_MTEXT_FRACTION_TOP			2

// DXF SYSTEM ERROR CODE
#define	DXF_NONE_ERROR				0L
//#define	DXF_MEM_ERROR				1L
#define	DXF_FILE_ERROR				2L

// DXF ERROR SECTION
#define	DXF_HEADER_SECTION			0L
#define	DXF_CLASSES_SECTION			1L
#define	DXF_TABLES_SECTION			2L
#define	DXF_BLOCKS_SECTION			3L
#define	DXF_ENTITIES_SECTION		4L
#define	DXF_OBJECTS_SECTION			5L

// DXF ERROR CODE
#define	DXF_SUCCESS						0L
#define	DXF_INVALID_HANDLE				1L
#define	DXF_INVALID_MPSPACE				2L
#define	DXF_INVALID_LAYER				3L
#define	DXF_INVALID_LINETYPE			4L
#define	DXF_INVALID_COLOR				5L
#define	DXF_INVALID_LINETYPESCALE		6L
#define	DXF_INVALID_EXTRUSION			7L
#define	DXF_INVALID_VISIBILITY			8L
#define	DXF_INVALID_POINT3D				9L
#define	DXF_INVALID_POINT2D				10L
#define	DXF_INVALID_ENTITYCODE			11L
#define	DXF_INVALID_APPNAME				12L
#define	DXF_INVALID_THICKNESS			13L
#define	DXF_INVALID_RADIUS				14L
#define	DXF_INVALID_STARTANGLE			15L
#define	DXF_INVALID_ENDANGLE			16L
#define	DXF_INVALID_BLOCKNAME			17L
#define	DXF_INVALID_INSERTPOINT			18L
#define	DXF_INVALID_BLOCKNAME1			19L
#define	DXF_INVALID_XREFPATH			20L
#define	DXF_INVALID_SCALE				21L
#define	DXF_INVALID_COUNT				22L
#define	DXF_INVALID_STARTWIDTH			23L
#define	DXF_INVALID_ENDWIDTH			24L
#define	DXF_INVALID_CONSTWIDTH			25L
#define	DXF_INVALID_ELEVATION			26L
#define	DXF_INVALID_BULGE				27L
#define	DXF_INVALID_DIMSTYLE			28L
#define	DXF_INVALID_HARDREFNUMBER		29L
#define	DXF_INVALID_ARROWFLAG			30L
#define	DXF_INVALID_LEADERPATHTYPE		31L
#define	DXF_INVALID_LEADERGENFLAG		32L
#define	DXF_INVALID_HOOKLINEDIRFLAG		33L
#define	DXF_INVALID_HOOKLINEFLAG		34L
#define	DXF_INVALID_TEXTHEIGHT			35L
#define	DXF_INVALID_TEXTWIDTH			36L
#define	DXF_INVALID_VERTEXCNT			37L
#define	DXF_INVALID_CLASSVER			38L
#define	DXF_INVALID_DISPPROP			39L
#define	DXF_INVALID_CLIPSTATE			40L
#define	DXF_INVALID_CLIPTYPE			41L
#define	DXF_INVALID_IMAGEFADE			42L
#define	DXF_INVALID_IMAGECONSTRAST		43L
#define	DXF_INVALID_IMAGEBRIGHT			44L
#define	DXF_INVALID_POLYLINECODE		45L
#define	DXF_INVALID_VERTEXFOLLOWCODE	46L
#define	DXF_INVALID_DENSITY				47L
#define	DXF_INVALID_POLYLINETYPE		48L
#define	DXF_INVALID_VERTEX				49L
#define	DXF_INVALID_SHAPESIZE			50L
#define	DXF_INVALID_SHAPENAME			51L
#define	DXF_INVALID_ANGLE				52L
#define	DXF_INVALID_STRING				53L
#define	DXF_INVALID_TEXTSTYLE			54L
#define	DXF_INVALID_TEXTGENCODE			55L
#define	DXF_INVALID_TEXTHORIJUST		56L
#define	DXF_INVALID_TEXTVERTJUST		57L
#define	DXF_INVALID_FIELDLEN			58L
#define	DXF_INVALID_MODELERVER			59L
#define	DXF_INVALID_TOLERANCE			60L
#define	DXF_INVALID_KNOT				61L
#define	DXF_INVALID_WEIGHT				62L
#define	DXF_INVALID_DEGREE				63L
#define	DXF_INVALID_FITTANGENT			64L
#define	DXF_INVALID_INDEX				65L
#define	DXF_INVALID_VECTOR3D			66L
#define	DXF_INVALID_SUBCLASSMAKER		67L
#define	DXF_INVALID_VERSION				68L
#define	DXF_INVALID_OLEOBJECTTYPE		69L
#define	DXF_INVALID_OLETILEMODE			70L
#define	DXF_INVALID_OLEDATASIZE			71L
#define	DXF_INVALID_TEXTDRAWINGDIR		72L
#define	DXF_INVALID_TEXTATTACHCODE		73L
#define	DXF_INVALID_MLINESTYLE			74L
#define	DXF_INVALID_LENGTH				75L
#define	DXF_INVALID_TANGENT				76L
#define	DXF_INVALID_CODE				77L
#define	DXF_INVALID_HATCH_EDGETYPE		78L
#define	DXF_INVALID_HATCH_STYLE			79L
#define	DXF_INVALID_HATCH_PATTERNTYPE	80L
#define	DXF_INVALID_HATCH_PIXELSIZE		81L
#define	DXF_INVALID_HATCH_BOUNDTYPE		82L
#define	DXF_INVALID_OFFSET				83L
#define	DXF_INVALID_MLINE_JUSTIFICATION	84L
#define	DXF_INVALID_PARAM				85L
#define	DXF_INVALID_WIDTH				86L
#define	DXF_INVALID_HEIGHT				87L
#define	DXF_INVALID_VALUE				88L
#define	DXF_INVALID_TABLECODE			89L
#define	DXF_INVALID_LAYERNAME			90L
#define	DXF_INVALID_LAYERCODE			91L
#define	DXF_INVALID_LAYERCOLOR			92L
#define	DXF_INVALID_STYLENAME			93L
#define	DXF_INVALID_STYLECODE			94L
#define	DXF_INVALID_FONTNAME			95L
#define	DXF_INVALID_UCSNAME				96L
#define	DXF_INVALID_UCSCODE				97L
#define	DXF_INVALID_VIEWMODE			98L
#define	DXF_INVALID_VIEWNAME			99L
#define	DXF_INVALID_VIEWHEIGHT			100L
#define	DXF_INVALID_VIEWWIDTH			101L
#define	DXF_INVALID_VIEWCODE			102L
#define	DXF_INVALID_LTYPENAME			103L
#define	DXF_INVALID_LTYPECODE			104L
#define	DXF_INVALID_DASHTYPE			105L
#define	DXF_INVALID_SHAPECODE			106L
#define	DXF_INVALID_MLINESTYLENAME		107L
#define	DXF_INVALID_MLINESTYLEFLAG		108L
#define	DXF_INVALID_IMAGENAME			109L
#define	DXF_INVALID_VPORTNAME			110L
#define	DXF_INVALID_VPORTCODE			111L
#define	DXF_INVALID_RESOLUTIONUNIT		112L
#define	DXF_INVALID_LOADFLAG			113L
#define	DXF_ERROR_FILEOPEN				114L
#define DXF_MEM_ERROR				115L

// DXF MAX STRING LENGTH
#define	DXF_MAX_TEXT_LEN	255

// DXF Common Code
#define	DXF_HANDLE_CODE			5
#define	DXF_SPACE_CODE			67
#define	DXF_LAYER_CODE			8
#define	DXF_LTYPE_CODE			6
#define	DXF_COLOR_CODE			62
#define	DXF_LTYPESCALE_CODE		48
#define	DXF_EXTRUSION_CODE		210
#define	DXF_VISIBILITY_CODE		60
#define	DXF_SUBCLASS_CODE		100
#define	DXF_BEGINNING_CODE		0
#define	DXF_SECTION_NAME		2
#define	DXF_THICK_CODE			39

#include "Kon_Array.h"
/////////////////// DXF Base Structure /////////////////

typedef CKon_Array < DXF_LONG, DXF_LONG >	CDXF_LongArray;

// RGB Color Structure
typedef struct _SDXF_RGBColor {
	DXF_BYTE	bRed;
	DXF_BYTE	bGreen;
	DXF_BYTE	bBlue;
	_SDXF_RGBColor() {
		bRed = 0, bGreen = 0, bBlue = 0;
	}
} SDXF_RGBColor;

// HLS Color Structure
typedef struct _SDXF_HLSColor {
	DXF_DOUBLE	dH;
	DXF_DOUBLE	dL;
	DXF_DOUBLE	dS;
} SDXF_HLSColor;

// 3D Point
typedef struct _SDXF_Point3D {
	DXF_DOUBLE		dX, dY, dZ;

	_SDXF_Point3D(DXF_DOUBLE dInX = 0.0, DXF_DOUBLE dInY = 0.0, DXF_DOUBLE dInZ = 0.0)
	{
		dX = dInX, dY = dInY, dZ = dInZ;
	}
} SDXF_Point3D;

typedef CKon_Array < DXF_DOUBLE, DXF_DOUBLE >			CDXF_DoubleArray;
typedef	CKon_Array < DXF_LONG, DXF_LONG >				CDXF_LongArray;
typedef CKon_Array < SDXF_Point3D, SDXF_Point3D >		CDXF_Point3DArray;

// 2D Point 
typedef struct _SDXF_Point2D {
	DXF_DOUBLE		dX, dY;

	_SDXF_Point2D(DXF_DOUBLE dInX = 0.0, DXF_DOUBLE dInY = 0.0)
	{
		dX = dInX, dY = dInY;
	}
} SDXF_Point2D;

typedef CKon_Array < SDXF_Point2D, SDXF_Point2D >		CDXF_Point2DArray;

// Coord3D
typedef struct _SDXF_Coord3D {
	SDXF_Point3D	sOrg;
	SDXF_Point3D	sXAxis;
	SDXF_Point3D	sYAxis;
	SDXF_Point3D	sZAxis;
	_SDXF_Coord3D() {
		sOrg.dX = sOrg.dY = sOrg.dZ = 0.0;
		sXAxis.dX = 1.0, sXAxis.dY = sXAxis.dZ = 0.0;
		sYAxis.dY = 1.0, sYAxis.dX = sYAxis.dZ = 0.0;
		sZAxis.dZ = 1.0, sZAxis.dX = sZAxis.dY = 0.0;
	}
} SDXF_Coord3D;

// Vertex
typedef struct _SDXF_Vertex {
	DXF_DOUBLE		dX, dY, dZ;
	DXF_DOUBLE		dStartWidth;
	DXF_DOUBLE		dEndWidth;
	DXF_DOUBLE		dBulge;
	DXF_DOUBLE		dCurveFitTangentDir;

	_SDXF_Vertex()
	{
		dX = 0, dY = 0, dZ = 0;
		dStartWidth = 0;
		dEndWidth = 0;
		dBulge = 0;
		dCurveFitTangentDir = 0;
	}
	_SDXF_Vertex(DXF_DOUBLE dInX, DXF_DOUBLE dInY, DXF_DOUBLE dInZ,
				 DXF_DOUBLE dInStartWidth = 0.0, DXF_DOUBLE dInEndWidth = 0.0,
				 DXF_DOUBLE dInBulge = 0.0, DXF_DOUBLE dInCurveFitTangentDir = 0.0)
	{
		dX = dInX, dY = dInY, dZ = dInZ;
		dStartWidth = dInStartWidth;
		dEndWidth = dInEndWidth;
		dBulge = dInBulge;
		dCurveFitTangentDir = dInCurveFitTangentDir;
	}
} SDXF_Vertex;

typedef CKon_Array < SDXF_Vertex, SDXF_Vertex >		CDXF_VertexArray;

// PolyFace
typedef struct _SDXF_PolyFace {
	DXF_LONG	lVertexIndex1;
	DXF_LONG	lVertexIndex2;
	DXF_LONG	lVertexIndex3;
	DXF_LONG	lVertexIndex4;

	_SDXF_PolyFace(DXF_LONG lIndex1 = 0, DXF_LONG lIndex2 = 0, DXF_LONG lIndex3 = 0, DXF_LONG lIndex4 = 0) {
		lVertexIndex1 = lIndex1;
		lVertexIndex2 = lIndex2;
		lVertexIndex3 = lIndex3;
		lVertexIndex4 = lIndex4;
	}
} SDXF_PolyFace;

typedef CKon_Array < SDXF_PolyFace, SDXF_PolyFace >		CDXF_PolyFaceArray;

// LWPolyline Element Struct
typedef struct _SDXF_PolyPoint {
	DXF_DOUBLE		dX, dY, dZ;

	DXF_DOUBLE		dStartWidth;
	DXF_DOUBLE		dEndWidth;
	DXF_DOUBLE		dBulge;
	_SDXF_PolyPoint() {
		dX = 0.0, dY = 0.0, dZ = 0.0;
		dStartWidth = 0.0;
		dEndWidth = 0.0;
		dBulge = 0.0;
	}
} SDXF_PolyPoint;

// LType Element Struct
typedef struct _SDXF_LTypeElement {
	DXF_DOUBLE		dDashLen;
	DXF_LONG		lDashType;		// 
	DXF_LONG		lShapeCode;
	DXF_LONG		lStyleObjPointer;
	DXF_DOUBLE		dScale;
	DXF_DOUBLE		dAngle;
	DXF_DOUBLE		dXOffset;
	DXF_DOUBLE		dYOffset;
	DXF_CHAR		szText[MAX_STRING_LEN];

	_SDXF_LTypeElement() {
		dDashLen = 0.0;
		lDashType = 0;
		lShapeCode = 0;
		lStyleObjPointer = 0;
		dScale = 0.0;
		dAngle = 0.0;
		dXOffset = 0.0;
		dYOffset = 0.0;
		memset(szText, 0x00, MAX_STRING_LEN);
	}
} SDXF_LTypeElement;

// MLine Style Element Struct
typedef struct _SDXF_MLineStyleElement {
	DXF_DOUBLE	dOffset;
	DXF_LONG	lDXFColor;
	DXF_CHAR	szLineType[MAX_STRING_LEN];

	_SDXF_MLineStyleElement() {
		dOffset = 0.0;
		lDXFColor = 256;
		memcpy(szLineType, "BYLAYER", MAX_STRING_LEN);
	}
} SDXF_MLineStyleElement;

// MLine Element
typedef struct _SDXF_MLineElement {
	DXF_LONG		lParam;			

	DXF_DOUBLE*		dpParam;		
	DXF_LONG		lFillParam;		

	DXF_DOUBLE*		dpFillParam;	
	_SDXF_MLineElement() {
		lParam = 0;
		dpParam = NULL;
		lFillParam = 0;
		dpFillParam = NULL;
	}
} SDXF_MLineElement;

// MLine Vertex
typedef struct _SDXF_MLineVertex {
	SDXF_Point3D		sVertex;
	SDXF_Point3D		sSegDir;
	SDXF_Point3D		sMiterDir;
	SDXF_MLineElement*	spElement;
	_SDXF_MLineVertex() {
		spElement = NULL;
	}
} SDXF_MLineVertex;

// Hatch Path Edge - Line
typedef struct _SDXF_HatchLine {
	SDXF_Point2D	sStartPt;	
	SDXF_Point2D	sEndPt;		
} SDXF_HatchLine;

typedef CKon_Array< SDXF_HatchLine, SDXF_HatchLine >	CHatchLineArray;

// Hatch Path Edge - Arc
typedef struct _SDXF_HatchArc {
	SDXF_Point2D	sCenter;		
	DXF_DOUBLE		dRadius;		
	DXF_DOUBLE		dStartAngle;	
	DXF_DOUBLE		dEndAngle;		
	DXF_LONG		lClockWise;		
} SDXF_HatchArc;

typedef CKon_Array < SDXF_HatchArc, SDXF_HatchArc >	CHatchArcArray;

// Hatch Path Edge - Ellipse
typedef struct _SDXF_HatchEllipse {
	SDXF_Point2D	sCenter;		
	SDXF_Point2D	sEndPt;			
	DXF_DOUBLE		dMainLen;		
	DXF_DOUBLE		dStartAngle;	
	DXF_DOUBLE		dEndAngle;		
	DXF_LONG		lClockWise;		
} SDXF_HatchEllipse;

typedef CKon_Array < SDXF_HatchEllipse, SDXF_HatchEllipse >	CHatchEllipseArray;

// Hatch Path - Polyline
typedef struct _SDXF_HatchPolyline {
	DXF_LONG		lBulgeFlag;			// Bulge Flag
	DXF_LONG		lIsClosedFlag;		// Open/Close Flag
	DXF_LONG		lVertexCnt;			// 

	SDXF_PolyPoint*	spVertices;		// 
} SDXF_HatchPolyline;

typedef CKon_Array < SDXF_HatchPolyline, SDXF_HatchPolyline >	CHatchPolylineArray;

// Hatch Path Edge - Spline
typedef struct _SDXF_HatchSpline {
	DXF_LONG		lDegree;			

	DXF_LONG		lRationalSpline;	
	DXF_LONG		lPeriodicSpline;	
	DXF_LONG		lKnotCnt;			

	DXF_LONG		lControlCnt;		

	DXF_DOUBLE*		dpKnots;			
	SDXF_Point2D*	spControls;			
	DXF_DOUBLE*		dpWeights;			
} SDXF_HatchSpline;

typedef CKon_Array < SDXF_HatchSpline, SDXF_HatchSpline >		CHatchSplineArray;

// Hatch Pattern
typedef struct _SDXF_HatchPattern {
	DXF_DOUBLE		dPatternAngle;		
	SDXF_Point2D	sPatternStart;		
	SDXF_Point2D	sPatternOffset;		
	DXF_LONG		lDashCnt;			

	DXF_DOUBLE*		dpDashes;			
} SDXF_HatchPattern;

typedef CKon_Array < SDXF_HatchPattern, SDXF_HatchPattern >	CHatchPatternArray;

// Hatch Path Edge
typedef struct _SDXF_HatchPathEdge {
	DXF_LONG	lType;		

	DXF_LONG	lIndex;		

} SDXF_HatchPathEdge;

// Hatch Path
typedef struct _SDXF_HatchPath {
	DXF_LONG			lPathType;	

	DXF_LONG			lEdgeCnt;	

	SDXF_HatchPathEdge*	spEdges;	
} SDXF_HatchPath;

typedef CKon_Array< SDXF_HatchPath, SDXF_HatchPath >		CHatchPathArray;

// DXF Text Style
typedef struct _SDXF_TextStyle {
	DXF_BYTE	bUnder;		
	DXF_BYTE	bOver;		
	DXF_LONG	lStart;		
	DXF_LONG	lEnd;		
	_SDXF_TextStyle() {
		bUnder = 0, bOver = 0;
		lStart = 0, lEnd = 0;
	}
} SDXF_TextStyle;


// Tolerance Toler Element Structure
typedef struct _SDXF_TolerElem {
	DXF_LONG	lHead;
	DXF_LONG	lTail;
	DXF_CHAR*	pszText;
	_SDXF_TolerElem() {
		lHead = lTail = 0L;
		pszText= NULL;
	}
} SDXF_TolerElem;

// Tolerance Datum Element Structure
typedef struct _SDXF_DatumElem {
	DXF_LONG	lTail;
	DXF_CHAR*	pszText;
	_SDXF_DatumElem() {
		lTail = 0L;
		pszText = NULL;
	}
} SDXF_DatumElem;

// Tolerance Text Structure
typedef struct _SDXF_TolerText {
	DXF_LONG		lUpPrefix;
	SDXF_TolerElem	sUpTolerElem[2];
	SDXF_DatumElem	sUpDatumElem[3];
	DXF_LONG		lDownPrefix;
	SDXF_TolerElem	sDownTolerElem[2];
	SDXF_DatumElem	sDownDatumElem[3];
	DXF_CHAR*		pszHeight;
	DXF_CHAR*		pszDatumIdentifier;
	DXF_LONG		lProjectTolerZone;
} SDXF_TolerString;

// Spline Control Point
typedef struct _SDXF_SplinePoint {
	SDXF_Point3D	sControlPt;
	DXF_DOUBLE		dWeight;
	_SDXF_SplinePoint()
	{
		sControlPt.dX = sControlPt.dY = sControlPt.dZ = 0.0F;
		dWeight = 1.0F;
	}
} SDXF_SplinePoint;

typedef CKon_Array < SDXF_SplinePoint, SDXF_SplinePoint >		CDXF_SplinePointArray;

// DXF ERROR VARIABLES
extern	DXF_LONG		g_lDXFErrorCode;
extern	DXF_LONG		g_lDXFErrorSection;
extern	DXF_LONG		g_lDXFSysErrorCode;
