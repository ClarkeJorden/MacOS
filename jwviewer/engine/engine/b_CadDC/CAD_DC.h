// CAD_DC.h: interface for the CCAD_DC class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CAD_DC_H_
#define _CAD_DC_H_

#include "CAD_Camera.h"
#include "CAD_Brush.h"
#include "CAD_Pen.h"
#include "CAD_Rect.h"
#include "CAD_DspFile.h"
#include "CAD_Environment.h"
#include "CAD_PenStyle.h"
#include "CAD_Grid.h"
#include "InstBaseStruct.h"	//	for FLOATINGPOINT

#ifdef _WIN32
#include <glew/include/GL/glew.h>
#include <gl/GL.h>
#include "ft2build.h"
#include "freetype.h"
#include "ftglyph.h"
#else
#include "../freetype/include/ft2build.h"
#include "../freetype/include/freetype/freetype.h"
#include "../freetype/include/freetype/ftglyph.h"
#endif

#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
#include FT_SYNTHESIS_H
#include FT_SIZES_H

typedef struct _tagFONT
{
	FT_Matrix  fntMat;

	LONG      fntHeight;
	LONG      fntWidth;
	LONG      fntAscent;

	BYTE      fntBold;
	BYTE      fntUnderline;
	BYTE      fntStrikeOut;
} FONT, *PFONT;

enum	DRAW_LINETYPE{ LINETYPE_NULL = 0x0, LINETYPE_STRIP = 0x1, LINETYPE_LINES = 0x2 };

class	CCAD_ClipPoly;

// pen style information made by screen scalar
typedef struct{
	CAD_FLOAT 	fElements[MAX_PENSTYLE_PART];
	int	nElementNum;
	int nCrntIdx;
}PEN_STYLE, *LP_PEN_STYLE;

#define CCAD_PI    ((CAD_FLOAT)  3.141592654f)
#define CCADToRadian( degree ) ((CAD_FLOAT)(degree) * (CCAD_PI / (CAD_FLOAT)180.0f))
#define CCADToDegree( radian ) ((CAD_FLOAT)(radian) * ((CAD_FLOAT)180.0f / CCAD_PI))

#define SUPPORT_VIEWPORT_CLIPPING
#define TEMP_FONT_ZIG_BUG_FIX

class   CCAD_PreViewInfo
{
public:
	CCAD_PreViewInfo(){ m_blSave = false; 

		m_fScale = 1.0f;	
		m_wUdsCrntPlan = 0;

	}
	virtual ~CCAD_PreViewInfo(){}

	bool			m_blSave;
	CAD_FLOAT		m_fScale;
	CCAD_Camera		m_cCamera;

	CCAD_Coord3D	m_cUdsCoord3D;
	WORD			m_wUdsCrntPlan;
	CCAD_Grid		m_cUdsGrid;
	// ------------------------

	void operator = (const CCAD_PreViewInfo& other)
	{
		m_blSave = other.m_blSave;
		m_fScale = other.m_fScale;
		m_cCamera = other.m_cCamera;

		m_cUdsCoord3D = other.m_cUdsCoord3D;		// Coordinate System
		m_wUdsCrntPlan = other.m_wUdsCrntPlan;		// active plan
		m_cUdsGrid = other.m_cUdsGrid;
	};
};

extern  FT_Library	m_Lib;
#define FT_FIXED_ONE 0x10000L
static FT_Matrix italicMat = { FT_FIXED_ONE, (FT_Fixed)(FT_FIXED_ONE*0.325f), 0, FT_FIXED_ONE };

class   CCAD_DC

{
public:
	CCAD_DC();
	virtual ~CCAD_DC();

// Attribute 
public:
	CCAD_Camera*	m_pCamera;		// Camera
	SCAD_PlanInfo *	m_pPlanInfo;	// Drawing Environment Information (doc)

	CCAD_Rect		m_cClipRect;	// Projection(Clipping)Region

	CAD_FLOAT		m_fBakScale;
	CAD_FLOAT		m_fScale;		// Projection Ratio

	CAD_FLOAT		m_fThick;		// 

	BYTE			m_bIsClose;		// close mode of MoveTo and LineTo
	BYTE			m_bIsThick;		// thick available

	CCAD_DspFile*	m_pDspFile;		

	void *			m_pLayer;		// Current Layer (for pen setting in the multi-layer mode)
	WORD			m_wPriority;	// Pen, Brush Priority flag

	LOGPEN			m_sPen;			// Current Pen
	LOGBRUSH		m_sBrush;		// Current Brush

	bool			m_blCurveLine;
	bool			m_blForceDrawPoly;

	CAD_FLOAT		m_fPenScale;
	short			m_nPenStyle;
	CCAD_PenStyle*	m_pPenStyle;
	CAD_FLOAT		m_fPaperScale;	// paper's scale

	BYTE			m_bTxtPaint;	// Text Filling?

	FLOATINGPOINT	m_sOrgLP;		// for edge afterimage at 2005.4.13		// viewport org screen Point(MM_TEXTMOD)

	BOOL			m_CameraDir;	// for Direction in Printing.

	BOOL			m_blCancelMessage;

	BOOL			m_blLayoutView;

	inline BOOL		CheckMessage()	{ return m_blCancelMessage; }

private:
	//	for PolyPolygon
	SCAD_POINT*		m_p2DPoints;
	int				m_n2DPointNum;
	int*			m_p2DPolyCounts;
	int	 			m_n2DPolyCountNum;

	// pen style information
	PEN_STYLE		m_sPSConstPattern;
	PEN_STYLE		m_sPSWorkPattern;

	// moveto and lineto information
	CCAD_Point		m_cOrgPt;
	CCAD_Point		m_cOrgTopPt;


	CCAD_ClipPoly*	m_pClipRgn;
	
	CCAD_Camera		m_bakParalCamera;
	CCAD_Rect		m_bakParalRect;
	POINT			m_bakParalOrgLp;
	CAD_FLOAT		m_bakParalScale;

	CCAD_Camera		m_bakPerspCamera;
	CCAD_Rect		m_bakPerspRect;
	POINT			m_bakPerspOrgLp;
	CAD_FLOAT		m_bakPerspScale;
	BYTE			m_bChangeCamera;

	// for only tesselation
	double			*m_dpTriVtx;
	long			m_nTriVtxBufNum;
public:
	#define		CHANGE_PALL		0x01
	#define		CHANGE_PERS		0x02

	void		ChangeCamera();
	//----------

// Method
public:
	// view Bound Rect (ECS)
	// used in OpenGL render.
	CCAD_Rect		m_cViewBox;
	BOOL			m_bEnableViewBox;
	BOOL			m_bIsCalcOnlyViewBox;

	void			Init(CAD_FLOAT fWidth, CAD_FLOAT fHeight);
	void			Initial();

	void			SetScreenOrg(float x, float y);
	BOOL			SetViewPort(const RECT& viewRect);
	BOOL			SetViewPort(const RECT& viewRect, const RECT& clipRect, CAD_FLOAT fScale);
	BOOL			InitViewPort(const RECT& viewRect);
	void			SetClipRect(const RECT& cRect);

	CCAD_Camera*	GetCamera(){return m_pCamera;}
	CCAD_Coord3D	PushCamera(CCAD_Coord3D *pCoord);
	void			RestorCamera(const CCAD_Coord3D& cCoord3D);

	void			SetDspFile(CCAD_DspFile* pDspFile);
	void			ReSetPriority(WORD wPriorityFlag, BOOL set);
	void			SetPriority(WORD wPriority) { m_wPriority = wPriority; }
	void			AddPriority(WORD wPriority) { m_wPriority |= wPriority; }

	CCAD_PenStyle * SelectPenStyle(CCAD_PenStyle *, short style, CAD_FLOAT fScale);

	CAD_FLOAT		SelectThick(CAD_FLOAT fThick);
	BOOL			SelectClose(BOOL bIsClose );

	// Set Project Mode
	void		SetProjectMode(BOOL blPerspective, const RECT& viewRect);
	void		SetPerspectiveMode(const RECT& viewRect);
	void		SetParallelMode(const RECT& viewRect);

	void		RotateVertical(const CCAD_Angle& cAngle);
	void		RotateHorizontal(const CCAD_Angle& cAngle);

	//draw function
	void		MoveTo(const SCAD_POINT& cPt);
	void		LineTo(const SCAD_POINT& cPt);

	void		SetPixel(const SCAD_POINT& pt, COLORREF rgbColor);
	
	void		PolyPolygon( CCAD_Point* lpPoints, short * lpPolyCounts, int nCount, CCAD_Point *lpTriPoints, int nTriCount );
	CCAD_Point*		GetTrianglesFromPolygon( CCAD_Point* lpPoints, short * lpPolyCounts, int nCount, int & nTriCount );
private:
	void		PolyPolygon2( CCAD_Vector* lpPoints, short *lpPolyCounts, int nCount, CCAD_Point *lpTriPoints, int nTriCount );
	void		DrawPolyPolygonHatch(SCAD_POINT* lpPoints, short *lpPolyCounts, int nCount, CCAD_Rect bound, int lbHatch, float dScan);
public:
	void		DrawLine(const SCAD_POINT& cPt1, const SCAD_POINT& cPt2);
	void		DrawRayLine(const SCAD_POINT& cPt1, const SCAD_POINT& cPt2);
	// --
	void		ExtentView(CCAD_Rect& cExtentRect, const CCAD_Rect& cScrnRect, short nScreenMargin = 0);
	void		CountVcsRange( const CCAD_Vector & cPt, CCAD_Rect & cRange) const;

	// Camera Translation
	void		XMoveCamera(short nOffset);
	void		YMoveCamera(short nOffset);
	void		SetScale(CAD_FLOAT fNewScale);
	BOOL		IsEnableScale(CAD_FLOAT fNewScale);

	// convert functions
	SCAD_POINT&	WCStoVCS(SCAD_POINT& cPt);
	CCAD_Rect&	WCStoVCS(CCAD_Rect& cRect);
	SCAD_POINT&	VCStoWCS(SCAD_POINT& cPt);

	POINT		ECStoLP(const CCAD_Point& pt);
	long		ConvScalarWorldToScrn(CAD_FLOAT fWorldScalar);
	CAD_FLOAT	ConvScalarScrnToWorld(short nScrnScalar);

	POINT		VCStoLP(const SCAD_POINT& cPoint);
	void		VCStoLP(const SCAD_POINT& cPoint, double& dX, double& dY);
	void		VCStoLP(const SCAD_POINT& cPoint, float& dX, float& dY);
	SCAD_POINT	LPtoVCS(const POINT& sPoint);
	SCAD_POINT	LPtoVCS(const CCAD_Point & sPoint);
	CCAD_Rect	LPtoVCS(const RECT& sLPRect);

	SCAD_POINT	LPtoWCS(POINT sPoint);
	SCAD_POINT	LPtoWCS(const CCAD_Point & sPoint);
	POINT		WCStoLP(SCAD_POINT cPt);

	CCAD_Point	GetTextExtent(LPCSTR lpStr, int cbString);
#ifdef TEMP_FONT_ZIG_BUG_FIX
	void		SetFontPixelSize(long nWidth, long nHeight);
#endif
	BOOL		TextOut(CAD_FLOAT fX, CAD_FLOAT fY, LPCSTR lpStr, int cbStr);
	BOOL		TextOut(int x, int y, LPCSTR lpStr, int cbStr);
	BOOL		TextOut(CAD_FLOAT fX, CAD_FLOAT fY, unsigned short lpStr, int cbStr, float &nWidth);

	short		GetCircleDivide(CAD_FLOAT r);
	short		GetArcDivide(const CCAD_Angle & cDelta, CAD_FLOAT r);
	short		GetEllipseDivide(CAD_FLOAT rx, CAD_FLOAT ry);
	short		GetEllipseArcDivide(const CCAD_Angle & cDelta, CAD_FLOAT rx, CAD_FLOAT ry);
	virtual		COLORREF	SetTextColor ( COLORREF color );
	// operator
	void operator = (const CCAD_DC& cOther);


public:
	POINT m_cOrgScreenPt;
	PFONT m_pFont;
	bool  m_blMoveTo;
	void FillSolidRect(RECT* rect, COLORREF color);
#ifdef SUPPORT_VIEWPORT_CLIPPING
	void SelectClipRect(const RECT& rect);
	void ReleaseClipRect();
#endif
	LOGPEN		SelectPen(const LOGPEN &pen);
	LOGBRUSH 		SelectBrush(const LOGBRUSH &brush);
	LOGPEN SelectNullPen();
	LOGBRUSH SelectNullBrush();
	LOGBRUSH 		SelectColorBrush(COLORREF color);
	LOGPEN 		SelectColorPen(COLORREF color);
	void RestoreObject(const LOGPEN &pen);
	void RestoreObject(const LOGBRUSH &brush);
	void		MoveTo(int x, int y);
	void		LineTo(int x, int y);	
	LOGPEN GetLogPen() {return m_sPen;};
	LOGPEN SelectBrushColorPen();
	void 		StretchDIBits( int x0, int y0, int dx, int src_x0, int src_y0, int src_dx, CHAR* lpBits);
	void		SetPixel(int x, int y, COLORREF rgbColor);
	void		Polygon(POINT* lpPoints, int nCount);

	void		SetCameraInfo(CAD_FLOAT fWidth, CAD_FLOAT fHeight);
	virtual PFONT CreateFontIndirect ( LOGFONTA& logfont );
	PFONT			SelectFont(PFONT font);
	CCAD_Vector		GetTextExtent(LPSTR lpStr, int cbString, BOOL blGemCell = FALSE);
	BOOL			GetTextExtentPoint32(LPSTR lpString, int cbString, LPSIZE lpSize, BOOL blGemCell = FALSE);
	BOOL		GetTextMetrics(TEXTMETRIC* tm);
	BOOL		TextOut(CCAD_Vector *rect, LPCSTR lpStr, int cbStr, BOOL bDispOnlyOutLine = FALSE);
	BOOL		TextOut(WORD wChar, CCAD_Vector *rect, float w_hScale, float fSkew, BOOL bDispOnlyOutLine = TRUE);
	void		GetViewPortBndBox(CCAD_Rect &bndbox);

	static int monotone_check(SCAD_POINT *vertices, int nvertices);
	static int LeftOnRight(SCAD_POINT a, SCAD_POINT b, SCAD_POINT c );
	static void shMonoTriangulation(SCAD_POINT *vertices, int nvertices, SCAD_POINT *triangles, int ntriangles);
	static void shTriangulation(SCAD_POINT *vertices, int nvertices, SCAD_POINT *triangles, int ntriangles);

	BOOL IsVerticality() const;
	double *MallocTriVtxBuf(long num);

	float	*m_fpVtxBuf;
	long	m_nVtxBufNum;
	long	m_nVtxBufCurPos;


	DRAW_LINETYPE	m_nLineType;
	void	MallocVtxBuf(long num);
	void	SetViewBox(const SCAD_POINT &ptECS);
	void	DrawLines();
	void	Lines_Begin(DRAW_LINETYPE linetype);
	void	Lines_End();
	void	Lines_MoveTo(int x, int y);
	void	Lines_LineTo(int x, int y);

private:
	void	DrawLineWithPenStyle(const SCAD_POINT& cPt1, const SCAD_POINT& cPt2);

};

 

#endif 

