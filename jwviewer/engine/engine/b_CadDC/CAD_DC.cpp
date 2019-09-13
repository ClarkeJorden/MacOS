#include "stdafx.h"
#include "CAD_DC.h"
#include "CAD_Doc.h"
#include "CAD_ClipPoly.h"
#include "CAD_GeoLine.h"
#include "CAD_Image.h"
#include "CAD_ViewMain.h"
#include "Tessellator.h"

#define fz_mul255(a,b) (((a) * ((b) + 1)) >> 8)

extern BOOL g_blCalcBndBox;
extern RECT g_cRect;
extern CCAD_Rect g_WorldBndBox;
extern CCAD_Doc *g_pDoc;
extern FT_Face g_Face;
//////////////////////////////////////////////////////////////////////

// Construction/Destruction

//////////////////////////////////////////////////////////////////////

CCAD_DC::CCAD_DC()
{
	m_pCamera = NULL;

	m_cClipRect.m_fMinX = - CAMERA_WIDTH / 2;
	m_cClipRect.m_fMaxX = CAMERA_WIDTH / 2;
	m_cClipRect.m_fMinY = - CAMERA_HEIGHT / 2;
	m_cClipRect.m_fMaxY = CAMERA_HEIGHT / 2;

	m_fScale = (CAD_FLOAT)1.0;
//	m_fBakScale = (CAD_FLOAT)1.0;
	m_fThick = (CAD_FLOAT)0.0;

	m_pPlanInfo = NULL;

	m_bIsThick = false;
	m_bIsClose = false;

	m_pDspFile = NULL;				// Display File Pointer

	m_sOrgLP.x = 0;
	m_sOrgLP.y = 0;

	m_pLayer = NULL;
	m_wPriority = 0;

	m_fPenScale = 0;
	m_nPenStyle = -1;
	m_pPenStyle = NULL;
	m_fPaperScale = 1;

	m_sPen.lopnColor = RGB(0, 0, 0);
	m_sPen.lopnStyle = PS_SOLID;
	m_sPen.lopnWidth.x = 1;
	
	m_sBrush.lbColor = RGB(0, 0, 0);
	m_sBrush.lbStyle = BS_SOLID;
	m_sBrush.lbHatch = 0;

	m_pClipRgn = NULL;

	m_bTxtPaint = NO;

	m_bChangeCamera = CHANGE_PALL | CHANGE_PERS;

	m_CameraDir = FALSE;

	m_blCancelMessage = FALSE;

	m_n2DPointNum = 0;
	m_n2DPolyCountNum = 0;
	m_p2DPoints = NULL;
	m_p2DPolyCounts = NULL;

	m_pFont = NULL;

	m_blLayoutView = TRUE;

	m_dpTriVtx = NULL;
	m_nTriVtxBufNum = 0;

	m_blForceDrawPoly = false;

	m_fpVtxBuf = NULL;
	m_nVtxBufNum = 0;
	m_nVtxBufCurPos = 0;
	m_nLineType = LINETYPE_NULL;
	m_bEnableViewBox = TRUE;
	m_bIsCalcOnlyViewBox = FALSE;
}

void CCAD_DC::Init(CAD_FLOAT fWidth, CAD_FLOAT fHeight)

{
	m_pLayer = NULL;
	m_pCamera->Init(fWidth, fHeight);

	m_cClipRect.m_fMinX = - fWidth / 2;
	m_cClipRect.m_fMaxX = fWidth / 2;
	m_cClipRect.m_fMinY = - fHeight / 2;
	m_cClipRect.m_fMaxY = fHeight / 2;
	m_bEnableViewBox = TRUE;
	m_bIsCalcOnlyViewBox = FALSE;
}

void CCAD_DC::Initial()

{
	m_bChangeCamera = CHANGE_PALL | CHANGE_PERS;
}

CCAD_DC::~CCAD_DC()
{
	if( m_pClipRgn != NULL )
		delete m_pClipRgn;

	if (m_p2DPoints != NULL)
		delete [] m_p2DPoints;

	if (m_p2DPolyCounts != NULL)
		delete [] m_p2DPolyCounts;

	if(m_dpTriVtx)
	{
		delete [] m_dpTriVtx;
		m_dpTriVtx = NULL;
	}
	if(m_fpVtxBuf){
		delete [] m_fpVtxBuf;
		m_fpVtxBuf = NULL;
	}
}

void CCAD_DC::operator=(const CCAD_DC& cOther)
{
	m_pCamera = cOther.m_pCamera;
	m_pPlanInfo = cOther.m_pPlanInfo;

	m_cClipRect = cOther.m_cClipRect;
	m_fScale = cOther.m_fScale;
	m_fBakScale = cOther.m_fBakScale;
	m_fThick = cOther.m_fThick;

	m_bIsClose = cOther.m_bIsClose;
	m_bIsThick = cOther.m_bIsThick;

	m_pDspFile = cOther.m_pDspFile;

	m_cViewBox = cOther.m_cViewBox;
	m_bEnableViewBox = cOther.m_bEnableViewBox;
	m_bIsCalcOnlyViewBox = cOther.m_bIsCalcOnlyViewBox;

	m_sPSConstPattern = cOther.m_sPSConstPattern;

	m_sOrgLP = cOther.m_sOrgLP;
	m_fPaperScale = cOther.m_fPaperScale;

	m_bTxtPaint = cOther.m_bTxtPaint;


	m_bakParalCamera = cOther.m_bakParalCamera;
	m_bakPerspCamera = cOther.m_bakPerspCamera;
	m_bChangeCamera = cOther.m_bChangeCamera;
}

void CCAD_DC::SetScreenOrg(float x, float y)
{
	m_sOrgLP.x = x;
	m_sOrgLP.y = y;
}

BOOL CCAD_DC::SetViewPort(const RECT& viewRect)
{
	if(m_pCamera == NULL)
		return false;

	RECT rc = viewRect;
	if( (rc.bottom - rc.top) == 0 || (rc.right - rc.left) == 0 )
		return false;

	m_sOrgLP.x = (rc.right + rc.left) / 2.0f;
	m_sOrgLP.y = (rc.bottom + rc.top) / 2.0f;

	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;

	CAD_FLOAT fScale;
	if (m_pCamera->m_fWidth > m_pCamera->m_fHeight) {
		if (nWidth > nHeight)
			fScale = nWidth / m_pCamera->m_fWidth;
		else
			fScale = nHeight / m_pCamera->m_fWidth;
	} else {
		if (nWidth > nHeight)
			fScale = nWidth / m_pCamera->m_fHeight;
		else
			fScale = nHeight / m_pCamera->m_fHeight;
	}

	//	init m_cCadDC data.
	m_fBakScale = m_fScale;
	m_fScale = fScale;

	//	init camera data.
	CAD_FLOAT fWidth = nWidth / m_fScale;
	CAD_FLOAT fHeight = nHeight / m_fScale;

	Init(fWidth, fHeight);

	return true;
}



BOOL CCAD_DC::SetViewPort(const RECT& viewRect, const RECT& clipRect, CAD_FLOAT fNewScale)
{
	ASSERT(m_pCamera);

	if( (viewRect.bottom - viewRect.top) == 0 || (viewRect.right - viewRect.left) == 0 )
		return false;
	if( (clipRect.bottom - clipRect.top) == 0 || (clipRect.right - clipRect.left) == 0 )
		return false;

	if( !IsEnableScale(fNewScale) )
		return false;

	m_sOrgLP.x = (viewRect.right + viewRect.left) / 2.0f;
	m_sOrgLP.y = (viewRect.bottom + viewRect.top) / 2.0f;

	int nWidth = clipRect.right - clipRect.left;
	int nHeight = clipRect.bottom - clipRect.top;

	m_fBakScale = m_fScale;
	m_fScale = fNewScale;

	CAD_FLOAT fWidth = nWidth / fNewScale;
	CAD_FLOAT fHeight = nHeight / fNewScale;

	Init(fWidth, fHeight);

	m_cClipRect.m_fMinX = (clipRect.left - m_sOrgLP.x) / fNewScale;
	m_cClipRect.m_fMaxX = (clipRect.right - m_sOrgLP.x) / fNewScale;
	m_cClipRect.m_fMinY = (m_sOrgLP.y - clipRect.bottom) / fNewScale;
	m_cClipRect.m_fMaxY = (m_sOrgLP.y - clipRect.top) / fNewScale;

	return true;
}

BOOL CCAD_DC::InitViewPort(const RECT& viewRect)
{
	CCAD_Rect rc(viewRect.left, viewRect.top, viewRect.right, viewRect.bottom);

	if( (rc.Height() == 0) || (rc.Width() == 0 ) )
		return false;

	int nWidth = rc.Width();
	int nHeight = rc.Height();

	m_sOrgLP.x = (CAD_FLOAT) (viewRect.left + nWidth/2);
	m_sOrgLP.y = (CAD_FLOAT) (viewRect.top + nHeight/2);

	CAD_FLOAT fScale;
	if (nWidth > nHeight)
		fScale = nWidth / CAMERA_WIDTH;
	else
		fScale = nHeight / CAMERA_WIDTH;

	m_fBakScale = m_fScale;
	m_fScale = fScale;

	//	init camera data.
	CAD_FLOAT fWidth = nWidth / fScale;
	CAD_FLOAT fHeight = nHeight / fScale;

	Init(fWidth, fHeight);

	fWidth *= 0.4F; fHeight *= 0.4F;
	m_pCamera->m_cOrg.m_fX = fWidth;
	m_pCamera->m_cOrg.m_fY = fHeight;

	return true;
}

void CCAD_DC::SetClipRect(const RECT& cRect)
{
	m_cClipRect = LPtoVCS(cRect);
}

BOOL CCAD_DC::IsEnableScale(CAD_FLOAT fNewScale)
{
	CAD_FLOAT fScale = fNewScale / m_fScale;

	CAD_FLOAT fNewWidth = m_pCamera->m_fWidth / fScale;
	CAD_FLOAT fNewHeight = m_pCamera->m_fHeight / fScale;

	CAD_FLOAT fMinLen = _min(fNewWidth, fNewHeight);
	//	CAD_FLOAT fMaxLen = max(fNewWidth, fNewHeight);

	CAD_FLOAT fLimit = CAD_ZERO * 1000;

	if( fMinLen < fLimit )	
		return false;

	//	if( fMaxLen > (1 / CAD_ZERO) )
	//		return false;

	return true;
}

void CCAD_DC::SetScale(CAD_FLOAT fNewScale)
{
//	ASSERT(m_pCamera);
	if (m_pCamera == NULL)
		return;

	CAD_FLOAT fScale = fNewScale / m_fScale;

	CAD_FLOAT fNewWidth = m_pCamera->m_fWidth / fScale;
	CAD_FLOAT fNewHeight = m_pCamera->m_fHeight / fScale;

	CAD_FLOAT fMinLen = _min(fNewWidth, fNewHeight);
	//CAD_FLOAT fMaxLen = max(fNewWidth, fNewHeight);

//	CAD_FLOAT fLimit = CAD_ZERO * 100;
	CAD_FLOAT fLimit = CAD_ZERO * 1000;//fromMac

	if( fMinLen < fLimit )	
		return;

//	if( fMaxLen > ( 1 / CAD_ZERO) )
//		return;

	Init(fNewWidth, fNewHeight);

	m_fBakScale = m_fScale;
	m_fScale = fNewScale;
}

CCAD_Coord3D CCAD_DC::PushCamera(CCAD_Coord3D *pCoord)
{
//	CCAD_Coord3D oldCameraCoord = (CCAD_Coord3D)(*m_pCamera);
	CCAD_Coord3D oldCameraCoord = *((CCAD_Coord3D*)m_pCamera);

	if( pCoord )
		pCoord->ThisToChild(m_pCamera);

	return oldCameraCoord;
}

void CCAD_DC::RestorCamera(const CCAD_Coord3D& cCamera)
{
	*(CCAD_Coord3D*)m_pCamera = cCamera;

}

CCAD_PenStyle* CCAD_DC::SelectPenStyle(CCAD_PenStyle *pNew, short nStyle, CAD_FLOAT fScale)
{
	CCAD_PenStyle*	pOldPenStyle = m_pPenStyle;
	m_pPenStyle = pNew;

	if (pNew == NULL || nStyle == DISABLE_INDEX) {
		m_nPenStyle = nStyle;
		m_fPenScale = fScale;
		return pOldPenStyle;
	}

	if ( nStyle == m_nPenStyle && fScale == m_fPenScale) {
		memcpy(&m_sPSWorkPattern, &m_sPSConstPattern, sizeof(PEN_STYLE));
		return pOldPenStyle;
	}

	if ( pNew->m_nTotPart <= 0 || (pNew->m_fLength * fScale) <= 0 ) {
		m_nPenStyle = nStyle;
		m_fPenScale = fScale;
		return pOldPenStyle;
	}
	else {
		CAD_FLOAT fMin = MAX_DOUBLE;

		m_nPenStyle = nStyle;
		m_fPenScale = fScale;

		m_sPSConstPattern.nCrntIdx = 0;
		m_sPSConstPattern.nElementNum = (int)pNew->m_nTotPart;	// element Number

		int i;
		for(i = 0; i < m_sPSConstPattern.nElementNum; i++)
		{
			if( pNew->m_fPart[i] == 0 )
				m_sPSConstPattern.fElements[i] = ConvScalarScrnToWorld(1);
			else
				m_sPSConstPattern.fElements[i] = pNew->m_fPart[i] * fScale;

			if( fMin > fabs(m_sPSConstPattern.fElements[i]) )
			{
				fMin = (CAD_FLOAT)fabs(m_sPSConstPattern.fElements[i]);
			}
		}

		memcpy(&m_sPSWorkPattern, &m_sPSConstPattern, sizeof(PEN_STYLE));

		if( ConvScalarWorldToScrn(fMin) == 0 )
			m_pPenStyle = NULL;
	}

	return pOldPenStyle;
}

CAD_FLOAT CCAD_DC::SelectThick(CAD_FLOAT fThick)
{
	if (m_pCamera == NULL)
		return 0;

	CAD_FLOAT fOldThick = m_fThick;

	if( fabs(fThick) < CAD_ZERO ){
		m_bIsThick = false;
		m_fThick = 0.0;
	}
	else	//tempTest
	{
		m_bIsThick = true;
		m_fThick = fThick;
	}

	return fOldThick;
}

COLORREF CCAD_DC::SetTextColor ( COLORREF color )
{ 
	COLORREF oldColor = m_sPen.lopnColor; 
	m_sPen.lopnColor = color;
	return oldColor; 
}

BOOL CCAD_DC::SelectClose(BOOL bIsClose = false)
{
	BOOL old = m_bIsClose;
	m_bIsClose = bIsClose;

	return old;
}

void CCAD_DC::CountVcsRange( const CCAD_Vector & cPt, CCAD_Rect & cRange) const
{
	CCAD_Vector cTmpPt = cPt;
	m_pCamera->ConvFromBase(cTmpPt);
	cRange.ResetParameter(cTmpPt);

	if( m_bIsThick ){
		cTmpPt = CCAD_Point(cPt.m_fX, cPt.m_fY, cPt.m_fZ + m_fThick);
		m_pCamera->ConvFromBase(cTmpPt);
		cRange.ResetParameter(cTmpPt);
	}
}

void CCAD_DC::MoveTo(const SCAD_POINT& cPt)
{
	SCAD_POINT tmpPt = cPt;

	m_cOrgPt = tmpPt;
	m_pCamera->ConvFromBase(m_cOrgPt);

	if( m_bIsThick ){

		m_cOrgTopPt = CCAD_Point(tmpPt.m_fX, tmpPt.m_fY, tmpPt.m_fZ + m_fThick);
		m_pCamera->ConvFromBase(m_cOrgTopPt);

		if( !m_bIsClose )
			DrawLine(m_cOrgPt, m_cOrgTopPt);

	}
}

void CCAD_DC::LineTo(const SCAD_POINT& cPt)
{
	CCAD_Point pt1,pt2;
	pt1 = m_cOrgPt;
	pt2 = cPt;
	m_pCamera->ConvFromBase(pt2);
	m_cOrgPt = pt2;
	if( !m_bIsThick ){
		if( m_pPenStyle )
			DrawLineWithPenStyle(pt1,pt2);
		else
			DrawLine(pt1,pt2);
	}
	else {
		DrawLine(pt1,pt2);
		CCAD_Point topPt( cPt.m_fX, cPt.m_fY, cPt.m_fZ + m_fThick );
		m_pCamera->ConvFromBase(topPt);

		DrawLine(pt2,topPt);
		DrawLine(topPt,m_cOrgTopPt);

		m_cOrgTopPt = topPt;
	}
}

void CCAD_DC::DrawLineWithPenStyle(const SCAD_POINT& cPt1, const SCAD_POINT& cPt2)
{
	CCAD_Point pt1 = cPt1;
	CCAD_Point pt2 = cPt2;

	CCAD_Point ptECS1;
	CCAD_Point ptECS2;

	SetViewBox(cPt1);
	SetViewBox(cPt2);
	if(m_bIsCalcOnlyViewBox) return;

	if( m_pCamera->m_bPerspect ){

		if( !m_pCamera->ClipNear(pt1,pt2) )
			return;

		m_pCamera->ECStoVCS(pt1);
		m_pCamera->ECStoVCS(pt2);

	}



#if SUPPORT_ORTHO_MODE_WRIREFRAME
 	CCAD_Rect clipRect = m_cClipRect;
	CAD_FLOAT fWidth = m_sPen.lopnWidth.x / m_fScale;

 	clipRect.InflateRect(fWidth, fWidth);
 	//if( !clipRect.ClipLineInRect2D(pt1, pt2) )
 	//	return;
#endif

	POINT scrnPt1 = VCStoLP(pt1);
	POINT scrnPt2 = VCStoLP(pt2);
	POINT scrnPt;

	if( m_pPenStyle->m_nTotPart == 0 || (m_pPenStyle->m_fLength * m_fPenScale) == 0 )
	{
		MoveTo(scrnPt1.x, scrnPt1.y);
		LineTo(scrnPt2.x, scrnPt2.y);
		return;
	}

	// pen style drawing
	CCAD_Point	deltaPt;
	PEN_STYLE*  pPatt = &m_sPSWorkPattern;
	CAD_FLOAT fDrawLen, fLineLen, fCos, fSin, fDeltaX, fDeltaY, fPatLen;


	fDeltaX = pt2.m_fX - pt1.m_fX;
	fDeltaY = pt2.m_fY - pt1.m_fY;

	fLineLen = (CAD_FLOAT)sqrt((double)(fDeltaX * fDeltaX) + (double)(fDeltaY * fDeltaY));
	if (IS_ZERO(fLineLen))
		return;

	fCos = fDeltaX / fLineLen;
	fSin = fDeltaY / fLineLen;

	while (fLineLen > 0)
	{

		fPatLen = pPatt->fElements[pPatt->nCrntIdx];
        fPatLen /= m_fScale; //JW

		if( fLineLen <= fabs(fPatLen) ){

			if( fPatLen > 0 ){
				pPatt->fElements[pPatt->nCrntIdx] -= fLineLen;
				fDrawLen = fLineLen;
			}else{
				pPatt->fElements[pPatt->nCrntIdx] += fLineLen;
				fDrawLen = - fLineLen;
			}

			if( ConvScalarWorldToScrn(pPatt->fElements[pPatt->nCrntIdx]) == 0 )
			{
				pPatt->nCrntIdx ++;

				if( pPatt->nCrntIdx == pPatt->nElementNum )
					memcpy(pPatt, &m_sPSConstPattern, sizeof(PEN_STYLE) );
			}

			fLineLen = 0;


		}else{

			fDrawLen = fPatLen;
			fLineLen -= (CAD_FLOAT)fabs(fDrawLen);

			pPatt->nCrntIdx ++;

			if( pPatt->nCrntIdx == pPatt->nElementNum )
				memcpy(pPatt, &m_sPSConstPattern, sizeof(PEN_STYLE) );
		}

		deltaPt.m_fX = (CAD_FLOAT)(fabs(fDrawLen) * fCos);
		deltaPt.m_fY = (CAD_FLOAT)(fabs(fDrawLen) * fSin);

		CCAD_Point newPt;
		newPt.m_fX = pt1.m_fX + deltaPt.m_fX;
		newPt.m_fY = pt1.m_fY + deltaPt.m_fY;


		if( fDrawLen > 0 ){
			scrnPt = VCStoLP(pt1);
			MoveTo(scrnPt.x, scrnPt.y);
			scrnPt = VCStoLP(newPt);
			LineTo(scrnPt.x, scrnPt.y);
		}
		pt1 = newPt;
	}
}

void CCAD_DC::DrawLine(const SCAD_POINT& cPt1, const SCAD_POINT& cPt2)
{
	CCAD_Point pt1 = cPt1;
	CCAD_Point pt2 = cPt2;

	CCAD_Point ptECS1;
	CCAD_Point ptECS2;

	SetViewBox(cPt1);
	SetViewBox(cPt2);
	if(m_bIsCalcOnlyViewBox) return;

	//----
	if( m_pCamera->m_bPerspect ){

		if( !m_pCamera->ClipNear(pt1,pt2) )
			return;

		m_pCamera->ECStoVCS(pt1);
		m_pCamera->ECStoVCS(pt2);

	}

#if SUPPORT_ORTHO_MODE_WRIREFRAME
 	CCAD_Rect clipRect = m_cClipRect;
  	CAD_FLOAT fWidth = m_sPen.lopnWidth.x / m_fScale;
 
 	clipRect.InflateRect(fWidth, fWidth);
 	if( !clipRect.ClipLineInRect2D(pt1, pt2) )
 		return;
#endif

	if (m_pDspFile == NULL)
	{
		POINT scrnPt1;
		POINT scrnPt2;
		scrnPt1 = VCStoLP(pt1);
		scrnPt2 = VCStoLP(pt2);
		MoveTo(scrnPt1.x, scrnPt1.y);
		LineTo(scrnPt2.x, scrnPt2.y);
	}
}


void CCAD_DC::DrawRayLine(const SCAD_POINT& cPt1, const SCAD_POINT& cPt2)
{
	CCAD_Point pt1 = cPt1;
	CCAD_Point pt2 = cPt2;

	m_pCamera->ConvFromBase(pt1);
	m_pCamera->ConvFromBase(pt2);

	if( m_pCamera->m_bPerspect ){

		if( !m_pCamera->ClipNear(pt1,pt2) )
			return;

		m_pCamera->ECStoVCS(pt1);
		m_pCamera->ECStoVCS(pt2);

	}

#if SUPPORT_ORTHO_MODE_WRIREFRAME
	CCAD_Rect clipRect = m_cClipRect;
	CAD_FLOAT fWidth = m_sPen.lopnWidth.x / m_fScale;

	clipRect.InflateRect(fWidth, fWidth);
	if( !clipRect.ClipLineInRect2D(pt1, pt2) )
		return;
#endif

	if (m_pDspFile == NULL)
	{
		POINT scrnPt1, scrnPt2;
		scrnPt1 = VCStoLP(pt1);
		scrnPt2 = VCStoLP(pt2);
		MoveTo(scrnPt1.x, scrnPt1.y);
		LineTo(scrnPt2.x, scrnPt2.y);

	}
}

void CCAD_DC::SetPixel(const SCAD_POINT& cPt, COLORREF rgbColor)
{
	CCAD_Point pt = cPt;

	m_pCamera->ConvFromBase(pt); // WCS->ECS

	if( !m_pCamera->IsInternalPt(pt) )
		return;

	m_pCamera->ECStoVCS(pt);

	if( !m_cClipRect.IsPointInRect2D(pt) )
		return;

	POINT scrnPt = VCStoLP(pt);
	
	SetPixel(scrnPt.x, scrnPt.y, rgbColor);
}

void CCAD_DC::PolyPolygon( CCAD_Point* lpPoints, short *lpPolyCounts, int nCount, CCAD_Point *lpTriPoints, int nTriCount )
{
	if(/*IsPanMoving()*/false)
	{
		LOGPEN pen;
		pen.lopnColor = m_sBrush.lbColor;
		pen.lopnStyle = PS_SOLID;
		pen.lopnWidth.x = 1;
		LOGPEN oldPen = SelectPen(pen);
		short nStart = 0;
		for(int i = 0; i < nCount; i++)
		{
			short nSize = lpPolyCounts[i];
			POINT *pPt = new POINT[nSize];
			for(int j = 0; j < nSize; j++)
			{
				pPt[j] = WCStoLP(lpPoints[nStart + j]);
			}
			Polygon(pPt, nSize);
			delete[] pPt;
			nStart += nSize;
		}
		RestoreObject(oldPen);
	}
	else
	{
		if (!m_blForceDrawPoly && (!m_pCamera || m_pCamera->m_bPerspect ))
			return;
		if (m_sBrush.lbStyle == BS_NULL)
			return;
		if(!lpTriPoints || m_sBrush.lbStyle == BS_HATCHED)
		{
			short nTotalNum = 0;
			int k;
			for(k = 0; k < nCount; k++){
				nTotalNum += lpPolyCounts[k];
			}

			for(int i = 0; i < nTotalNum; i++)
				m_pCamera->ConvFromBase(lpPoints[i]);
		}
		PolyPolygon2( lpPoints, lpPolyCounts, nCount, lpTriPoints, nTriCount );
	}
}

CCAD_Point * CCAD_DC::GetTrianglesFromPolygon( CCAD_Point* lpPoints, short * lpPolyCounts, int nCount, int & nTriCount )
{
	Tesselator _tesselator;

	long vtxnum = 0;
	for(long i = 0; i < nCount; ++i){
		vtxnum += lpPolyCounts[i];
	}
	vtxnum *= 3;

	MallocTriVtxBuf(vtxnum);

	double *curtrivtx = m_dpTriVtx;
	CCAD_Point *cpCurPnt = lpPoints;
	_tesselator.beginPolygon();
	for (long i = 0 ; i < nCount ; i++ )
	{      
		_tesselator.beginContour();
		for ( long j = 0; j < lpPolyCounts[i] ; j++ )
		{
			curtrivtx[0] = cpCurPnt->m_fX;
			curtrivtx[1] = cpCurPnt->m_fY;
			curtrivtx[2] = cpCurPnt->m_fZ;
			_tesselator.feedvert(curtrivtx);
			curtrivtx += 3;
			cpCurPnt++;
		}
		_tesselator.endContour();
	}

	std::vector< float > triangles;
	_tesselator.tesselate( triangles );

	nTriCount = triangles.size() / 2;
	CCAD_Point *lpTriPoints = new CCAD_Point[nTriCount];
	for(int i = 0; i < triangles.size(); i+=2)
	{
		int index = i / 2;
		lpTriPoints[index].m_fX = triangles[i];
		lpTriPoints[index].m_fY = triangles[i + 1];
		lpTriPoints[index].m_fZ = 0;
	}
	return lpTriPoints;
}

void CCAD_DC::PolyPolygon2( CCAD_Vector* lpPoints, short *lpPolyCounts, int nCount, CCAD_Point *lpTriPoints, int nTriCount )
{
	// Fill Hatch of Solid
	float r = GetRValue(m_sBrush.lbColor) / 255.0f;
	float g = GetGValue(m_sBrush.lbColor) / 255.0f;
	float b = GetBValue(m_sBrush.lbColor) / 255.0f;
	glColor4f(r, g, b, 1);

	if (m_sBrush.lbStyle == BS_HATCHED)
	{
		// Get BoundBox
		CCAD_Rect bound;
		short nSize = 0;
		for(int i = 0, nStart = 0, nSize = 0; i < nCount; i++, nStart += nSize)
		{
			nSize = lpPolyCounts[i];
			for (int j = 0; j < nSize; j++)
				bound.ResetParameter(lpPoints[nStart + j]);
		}
#ifdef _WIN32
		const float dScan = ConvScalarScrnToWorld(10); // Hatch distance
#else
		const float dScan = ConvScalarScrnToWorld(4); // Hatch distance
#endif
		const float dd = ConvScalarScrnToWorld(1); // for correct cross-calulating
		bound.InflateRect(dd, dd);
		
		if (m_sBrush.lbHatch == HS_HORIZONTAL || m_sBrush.lbHatch == HS_CROSS)
		{	// ----- or +++++
			DrawPolyPolygonHatch(lpPoints, lpPolyCounts, nCount, bound, HS_HORIZONTAL, dScan);
		}
		if (m_sBrush.lbHatch == HS_VERTICAL || m_sBrush.lbHatch == HS_CROSS)
		{	// ||||| or +++++
			DrawPolyPolygonHatch(lpPoints, lpPolyCounts, nCount, bound, HS_VERTICAL, dScan);
		}
		if (m_sBrush.lbHatch == HS_FDIAGONAL || m_sBrush.lbHatch == HS_DIAGCROSS)
		{	// \\\\\ or xxxxx
			DrawPolyPolygonHatch(lpPoints, lpPolyCounts, nCount, bound, HS_FDIAGONAL, dScan);
		}
		if (m_sBrush.lbHatch == HS_BDIAGONAL || m_sBrush.lbHatch == HS_DIAGCROSS)
		{	// ///// or xxxxx
			DrawPolyPolygonHatch(lpPoints, lpPolyCounts, nCount, bound, HS_BDIAGONAL, dScan);
		}
	}
	else if (m_sBrush.lbStyle == BS_SOLID)
	{
		bool blNull = false;
		if(!lpTriPoints)
		{
			CCAD_Point *pPoints = GetTrianglesFromPolygon(lpPoints, lpPolyCounts, nCount, nTriCount);
			lpTriPoints = pPoints;
			blNull = true;
		}
 		GLfloat *pTri = new GLfloat[nTriCount * 2];
		for(int i = 0; i < nTriCount; i++)
		{
			POINT vPoint;
			if(blNull)
				vPoint = VCStoLP(lpTriPoints[i]);
			else
				vPoint = WCStoLP(lpTriPoints[i]);
			pTri[i * 2] = vPoint.x;
			pTri[i * 2 + 1] = vPoint.y;
		}
		glVertexPointer(2, GL_FLOAT, 0, pTri);
		glDrawArrays(GL_TRIANGLES, 0, nTriCount);
		delete[] pTri;
		if(blNull)
			delete[] lpTriPoints;
	}
}

void CCAD_DC::DrawPolyPolygonHatch(SCAD_POINT* lpPoints, short *lpPolyCounts, int nCount, CCAD_Rect bound, int lbHatch, float dScan)
{
	float limit = 0;
	float scanX0 = 0, scanY0 = 0;
	float scanX1 = 0, scanY1 = 0;

	if (lbHatch == HS_HORIZONTAL)
	{	// ----- or +++++
		limit = bound.Height();
		scanX0 = bound.m_fMinX, scanY0 = bound.m_fMinY;
		scanX1 = bound.m_fMaxX, scanY1 = scanY0;
	}
	else if (lbHatch == HS_VERTICAL)
	{	// ||||| or +++++
		limit = bound.Width();
		scanX0 = bound.m_fMinX, scanY0 = bound.m_fMinY;
		scanX1 = scanX0, scanY1 = bound.m_fMaxY;
	}
	else if (lbHatch == HS_FDIAGONAL)
	{	// \\\\\ or xxxxx
		limit = _max(bound.Width(), bound.Height()) * 2;
		scanX0 = bound.m_fMinX, scanY0 = bound.m_fMinY;
		scanX1 = scanX0, scanY1 = scanY0;
	}
	else if (lbHatch == HS_BDIAGONAL)
	{	// ///// or xxxxx
		limit = _max(bound.Width(), bound.Height()) * 2;
		scanX0 = bound.m_fMinX, scanY0 = bound.m_fMaxY;
		scanX1 = scanX0, scanY1 = scanY0;
	}
	
	CCAD_PtrArray arrCross; // cross info's ptr-array
	struct SCrossInfo
	{	// information for cross point
		float x, y;
		float comp; // compare value for sort of cross points
	};
	SCAD_POINT seg[2];

	for (float ds = dScan; ds < limit; ds += dScan)
	{
		float segX = 0, segY = 0;

		if (lbHatch == HS_HORIZONTAL)
		{
			scanY0 += dScan;
			scanY1 += dScan;
		}
		else if (lbHatch == HS_VERTICAL)
		{
			scanX0 += dScan;
			scanX1 += dScan;
		}
		else if (lbHatch == HS_FDIAGONAL)
		{
			scanY0 += dScan;
			scanX1 += dScan;
		}
		else if (lbHatch == HS_BDIAGONAL)
		{
			scanY0 -= dScan;
			scanX1 += dScan;
		}

		// Calculate all cross points with polygons.
		for (int i = 0, nStart = 0, nSize = 0; i < nCount; i++, nStart += nSize)
		{
			nSize = lpPolyCounts[i];

			for (int j = 0; j < nSize; j++)
			{
				CCAD_GeoLine<float> lineSeg;
				if (j == 0)
				{
					const SCAD_POINT& sp = lpPoints[nStart + nSize - 1];
					const SCAD_POINT& ep = lpPoints[nStart];
					lineSeg.Set(sp.m_fX, sp.m_fY, ep.m_fX, ep.m_fY);
				}
				else
				{
					const SCAD_POINT& sp = lpPoints[nStart + j - 1];
					const SCAD_POINT& ep = lpPoints[nStart + j];
					lineSeg.Set(sp.m_fX, sp.m_fY, ep.m_fX, ep.m_fY);
				}
				CCAD_GeoLine<float> scan(scanX0, scanY0, scanX1, scanY1);
				if (!lineSeg.GetCrossPt(scanX0, scanY0, scanX1, scanY1, segX, segY))
					continue;

				SCrossInfo* cross = new SCrossInfo;
				arrCross.Add(cross);
				cross->x = segX;
				cross->y = segY;

				if (lbHatch == HS_HORIZONTAL || lbHatch == HS_FDIAGONAL || lbHatch == HS_BDIAGONAL)
				{
					cross->comp = cross->x - bound.m_fMinX;
				}
				else if (lbHatch == HS_VERTICAL)
				{
					cross->comp = cross->y - bound.m_fMinY;
				}
			}
		}

		// Sort all cross points by compare value.
		for (int i = 0; i < arrCross.GetSize(); i++)
		{
			for (int j = i+1; j < arrCross.GetSize(); j++)
			{
				if (((SCrossInfo*)arrCross[i])->comp > ((SCrossInfo*)arrCross[j])->comp)
				{	// swap two cross infos
					void* temp = arrCross[j];
					arrCross[j] = arrCross[i];
					arrCross[i] = temp;
				}
			}
		}

		// Draw all line segments by sorted cross points
		int n = arrCross.GetUpperBound();
		for (int i = 0; i < n; i += 2)
		{
			seg[0].m_fX = ((SCrossInfo*)arrCross[i])->x;
			seg[0].m_fY = ((SCrossInfo*)arrCross[i])->y;
			seg[0].m_fZ = bound.m_fMinZ;
			seg[1].m_fX = ((SCrossInfo*)arrCross[i+1])->x;
			seg[1].m_fY = ((SCrossInfo*)arrCross[i+1])->y;
			seg[1].m_fZ = bound.m_fMinZ;
			for (int j = 0; j < 2; j++)
			{
				POINT pt = ECStoLP(seg[j]);
				seg[j].m_fX = pt.x;
				seg[j].m_fY = pt.y;
			}
			
			glVertexPointer(2, GL_FLOAT, sizeof(SCAD_POINT), seg);
			glDrawArrays(GL_LINES, 0, 2);
		}

		// Free all cross infos
		for (int i = 0; i < arrCross.GetSize(); i++)
			delete ((SCrossInfo*)arrCross[i]);
		arrCross.RemoveAll();
	}
}

POINT CCAD_DC::VCStoLP(const SCAD_POINT& cPoint)
{

	double dX = (double)cPoint.m_fX * (double)m_fScale;
	double dY = (double)cPoint.m_fY * (double)m_fScale;

	POINT pt;
	pt.x = (LONG)(m_sOrgLP.x + dX + 0.5);
	pt.y = (LONG)(m_sOrgLP.y - dY + 0.5);
	return pt;
}

void CCAD_DC::VCStoLP(const SCAD_POINT& cPoint, double& dX, double& dY)
{
	dX = (double)cPoint.m_fX * (double)m_fScale;
	dY = (double)cPoint.m_fY * (double)m_fScale;
    
	dX += m_sOrgLP.x;
	dY = m_sOrgLP.y - dY;
}

void CCAD_DC::VCStoLP(const SCAD_POINT& cPoint, float& dX, float& dY)
{
	dX = cPoint.m_fX * m_fScale;
	dY = cPoint.m_fY * m_fScale;
	
	dX += m_sOrgLP.x;
	dY = m_sOrgLP.y - dY;
}

SCAD_POINT CCAD_DC::LPtoVCS(const POINT& pt)
{
	CCAD_Point cPoint;

	cPoint.m_fX = (pt.x - m_sOrgLP.x) / m_fScale;
	cPoint.m_fY = (m_sOrgLP.y - pt.y) / m_fScale;
	cPoint.m_fZ = - m_pCamera->m_fDistToEye;

	return cPoint;
}

SCAD_POINT CCAD_DC::LPtoVCS(const CCAD_Point & cPt)
{
	CCAD_Point cPoint;

/*	cPoint.m_fX = (cPt.m_fX / m_fScale) - (m_pCamera->m_fWidth / 2);
	cPoint.m_fY = (m_pCamera->m_fHeight / 2) - (cPt.m_fY / m_fScale);
*/

	cPoint.m_fX = (cPt.m_fX - m_sOrgLP.x) / m_fScale;
	cPoint.m_fY = (m_sOrgLP.y - cPt.m_fY) / m_fScale;
	cPoint.m_fZ = - m_pCamera->m_fDistToEye;

	return cPoint;
}

CCAD_Rect CCAD_DC::LPtoVCS(const RECT& sLPRect)
{
	CCAD_Rect cRetRect;

	POINT pt1,pt2;

	pt1.x = sLPRect.left;
	pt1.y = sLPRect.top;
	pt2.x = sLPRect.right;
	pt2.y = sLPRect.bottom;

	CCAD_Point cPt1 = LPtoVCS(pt1);
	CCAD_Point cPt2 = LPtoVCS(pt2);

	cRetRect.SetParameter(cPt1,cPt2);

	return cRetRect;
}

long CCAD_DC::ConvScalarWorldToScrn(CAD_FLOAT fWorldScalar)
{
	if( fWorldScalar < 0 )
		return (long)(m_fScale * fWorldScalar / m_pCamera->m_fScale - (CAD_FLOAT)0.5);
	else
		return (long)(m_fScale * fWorldScalar / m_pCamera->m_fScale + (CAD_FLOAT)0.5);
}

CAD_FLOAT CCAD_DC::ConvScalarScrnToWorld(short nScrnScalar)
{
	return (CAD_FLOAT)nScrnScalar * m_pCamera->m_fScale / m_fScale;
}

void CCAD_DC::XMoveCamera(short nXOffset)
{
	if (m_pCamera == NULL)
		return;

	CAD_FLOAT fXOffset = nXOffset / m_fScale;
	m_pCamera->MoveHoriz(fXOffset);
}

void CCAD_DC::YMoveCamera(short nYOffset)
{
	if (m_pCamera == NULL)
		return;

	CAD_FLOAT fYOffset = nYOffset / m_fScale;
	m_pCamera->MoveVerti(fYOffset);
}

POINT CCAD_DC::ECStoLP(const CCAD_Point& pt)
{
	ASSERT(m_pCamera);

	CCAD_Point tempPt = pt;

	if( m_pCamera->m_bPerspect )
		ASSERT( m_pCamera->IsInternalPt(pt) );

	m_pCamera->ECStoVCS(tempPt);
	return VCStoLP(tempPt);
}

SCAD_POINT& CCAD_DC::WCStoVCS(SCAD_POINT& cPt)
{
	if (m_pCamera == NULL)
		return cPt;

	m_pCamera->ConvFromBase(cPt);	// WCS->ECS

	if( m_pCamera->m_bPerspect )
		if (!m_pCamera->IsInternalPt(cPt))
			return cPt;

	m_pCamera->ECStoVCS(cPt);		// ECS->VCS

	return cPt;
}

CCAD_Rect& CCAD_DC::WCStoVCS(CCAD_Rect& cRect)
{
	if (m_pCamera == NULL)
		return cRect;

	SCAD_POINT	pt[8] = {
		{cRect.m_fMinX, cRect.m_fMinY, cRect.m_fMinZ}, 
		{cRect.m_fMaxX, cRect.m_fMinY, cRect.m_fMinZ},
		{cRect.m_fMaxX, cRect.m_fMaxY, cRect.m_fMinZ},
		{cRect.m_fMinX, cRect.m_fMaxY, cRect.m_fMinZ},
		{cRect.m_fMinX, cRect.m_fMinY, cRect.m_fMaxZ}, 
		{cRect.m_fMaxX, cRect.m_fMinY, cRect.m_fMaxZ},
		{cRect.m_fMaxX, cRect.m_fMaxY, cRect.m_fMaxZ},
		{cRect.m_fMinX, cRect.m_fMaxY, cRect.m_fMaxZ}};

	cRect.Init();

	CCAD_Point	ptTemp;
	int i;

	for (i = 0; i < 8; i++)
	{
		ptTemp = pt[i];
		WCStoVCS(ptTemp);
		cRect.ResetParameter(ptTemp);
	}

	return cRect;
}

SCAD_POINT&	CCAD_DC::VCStoWCS(SCAD_POINT& cPt)
{
	cPt.m_fZ = -m_pCamera->m_fDistToEye;
	m_pCamera->ConvToBase(cPt);		// ECS->WCS

	return cPt;
}

SCAD_POINT CCAD_DC::LPtoWCS(POINT sPoint)
{
	CCAD_Point cPt = LPtoVCS(sPoint);
	return VCStoWCS(cPt);
}



SCAD_POINT CCAD_DC::LPtoWCS(const CCAD_Point & sPoint)
{
	CCAD_Point cPt = LPtoVCS(sPoint);
	return VCStoWCS(cPt);
}

POINT CCAD_DC::WCStoLP(SCAD_POINT cPt) 
{
	WCStoVCS(cPt);
	return VCStoLP(cPt);
}



CCAD_Point CCAD_DC::GetTextExtent(LPCSTR lpStr, int cbString)
{
	CCAD_Point	cPt;
// 	CSize		size;
// 
// 	TEXTMETRIC tm;
// 
// 	GetTextMetrics( &tm );
// 
// 	GetTextExtentPoint32( lpStr, cbString, &size );
// 
// 	cPt.m_fX = ConvScalarScrnToWorld((short)size.cx);
// 	cPt.m_fY = ConvScalarScrnToWorld((short)tm.tmHeight);
// 	cPt.m_fZ = ConvScalarScrnToWorld((short)tm.tmAscent);

	return cPt;
}

#ifdef TEMP_FONT_ZIG_BUG_FIX
void CCAD_DC::SetFontPixelSize(long nWidth, long nHeight)
{
	if(m_pFont == NULL)
		return;

	if(g_Face == NULL)
		return;

	long fntWidth = m_pFont->fntWidth;
	long fntHeight = m_pFont->fntHeight;

	if(m_pFont->fntWidth == 0 && m_pFont->fntHeight != 0)
	{
		float factor = (float)m_pFont->fntHeight /  (float)nHeight;
		fntWidth = (long)((float)nWidth * factor);
	}
	else if(m_pFont->fntWidth != 0 && m_pFont->fntHeight == 0)
	{
		float factor = (float)m_pFont->fntWidth /  (float)nWidth;
		fntHeight = (long)((float)nHeight * factor);
	}
	else if(m_pFont->fntWidth == 0 && m_pFont->fntHeight == 0)
	{
		fntWidth = nWidth; fntHeight = nHeight;
	}

	FT_Set_Pixel_Sizes( g_Face, fntWidth, fntHeight );
}
#endif

BOOL CCAD_DC::TextOut(CAD_FLOAT fX, CAD_FLOAT fY, LPCSTR lpStr, int cbStr)
{
	if( m_pCamera->m_bPerspect )
		return true;

	CCAD_Point cPt(fX, fY, 0);

	POINT pt = WCStoLP(cPt);

	return TextOut((int)pt.x, (int)pt.y, lpStr, cbStr);
}

BOOL CCAD_DC::TextOut(CAD_FLOAT fX, CAD_FLOAT fY, unsigned short lpStr, int cbStr, float &nWidth)
{
	if( m_pCamera->m_bPerspect )
		return true;

	CCAD_Point cPt(fX, fY, 0);

	POINT pt = WCStoLP(cPt);

	return TextOut((int)pt.x, (int)pt.y, lpStr, cbStr, nWidth);
}

BOOL CCAD_DC::TextOut(int x, int y, LPCSTR lpcStr, int cbStr)
{
	BOOL	blRet = FALSE;

	if(!m_pFont)
		return FALSE;

	char	 lpStr[EXT_LENGTH_NAME];
	memset(lpStr, 0, EXT_LENGTH_NAME);
	strcpy(lpStr, lpcStr);
	ConvertStringLocalToUni(lpStr, cbStr);

	FT_Error      error;
	FT_Face       face = g_Face;
	FT_GlyphSlot  slot = face->glyph;
	FT_Vector     pen;
	pen.x = 0;
	pen.y = 0;
	for ( int n = 0; n < cbStr; n++ )
	{
		if(CheckMessage())
			return FALSE;
		FT_Set_Transform( face, &m_pFont->fntMat, &pen );
		/* load glyph image into the slot (erase previous one) */
		if( (unsigned char)lpcStr[n] < (unsigned char)0x80)
			error = FT_Load_Char( face, lpStr[n], FT_LOAD_RENDER );
		else
		{
			unsigned short	nCode;
			nCode = (lpStr[n] << 8) | (lpStr[n + 1] & 0xFF);
			error = FT_Load_Char( face, nCode, FT_LOAD_RENDER );
			n++;
		}
		if ( error )
			continue;                 /* ignore errors */
		int nStartX, nStartY;
// 		nStartX = x /*- m_pFont->fntWidth * (float)m_pFont->fntMat.xy / FT_FIXED_ONE*/ + slot->bitmap_left;
// 		nStartY = y /*+ m_pFont->fntHeight * (float)m_pFont->fntMat.xx / FT_FIXED_ONE*/ - slot->bitmap_top;

		nStartX = x - m_pFont->fntWidth * (float)m_pFont->fntMat.xy / FT_FIXED_ONE + slot->bitmap_left;
		nStartY = y + m_pFont->fntHeight * (float)m_pFont->fntMat.xx / FT_FIXED_ONE - slot->bitmap_top;

		// Clip Rect
		CCAD_Vector vRect[4];
		CCAD_Rect cClipRect(g_cRect.left, g_cRect.top, g_cRect.right, g_cRect.bottom);
		vRect[0].m_fX = nStartX;
		vRect[0].m_fY = nStartY;
		vRect[0].m_fZ = 0;
		vRect[1].m_fX = nStartX + slot->bitmap.width;
		vRect[1].m_fY = nStartY;
		vRect[1].m_fZ = 0;
		vRect[2].m_fX = nStartX;
		vRect[2].m_fY = nStartY + slot->bitmap.rows;
		vRect[2].m_fZ = 0;
		vRect[3].m_fX = nStartX + slot->bitmap.width;
		vRect[3].m_fY = nStartY + slot->bitmap.rows;
		vRect[3].m_fZ = 0;
		if(cClipRect.IsClipRect(vRect))
		{
			//Draw Text
			PIXELDATA textclr;
			textclr.bRed = GetRValue(m_sPen.lopnColor);
			textclr.bGreen = GetGValue(m_sPen.lopnColor);
			textclr.bBlue = GetBValue(m_sPen.lopnColor);
			textclr.bDummy = 0;

			long realrows = 1 << (long)ceil(log((float)slot->bitmap.rows)/log(2.0f));
			long realwid = 1 << (long)ceil(log((float)slot->bitmap.width)/log(2.0f));

			PIXELDATA *pixelData = new PIXELDATA[realrows * realwid];
			int nIndex = 0;
			for(int j = 0; j < realrows; j++)
			{
				for(int i = 0; i < realwid; i++)
				{
					char charValue = 0;
					if(j < slot->bitmap.rows && i < slot->bitmap.width){
						charValue = slot->bitmap.buffer[j * slot->bitmap.pitch + i];
					}
					pixelData[nIndex] = textclr;
					pixelData[nIndex].bDummy = charValue;
					nIndex++;
				}
			}

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_TEXTURE_2D);

			//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, realwid, realrows, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)pixelData);
#ifdef _WIN32
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			float widratio = (float)slot->bitmap.width / realwid;
			float rowsratio = (float)slot->bitmap.rows / realrows;
			float texs[8] = { 0, 0, widratio, 0, widratio, rowsratio, 0, rowsratio};

			float vtTri[8];
			vtTri[0] = nStartX;
			vtTri[1] = nStartY;
			vtTri[2] = nStartX + slot->bitmap.width;
			vtTri[3] = nStartY;
			vtTri[4] = nStartX + slot->bitmap.width;
			vtTri[5] = nStartY + slot->bitmap.rows;
			vtTri[6] = nStartX;
			vtTri[7] = nStartY + slot->bitmap.rows;

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, texs);
			glVertexPointer(2, GL_FLOAT, 0, vtTri);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisable(GL_TEXTURE_2D);

			glDisable(GL_BLEND);

			delete[] pixelData;
		}
		
		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
	}


	return blRet;
}

void CCAD_DC::ExtentView(CCAD_Rect& cExtentRect, const CCAD_Rect& cScrnRect, short nScreenMargin)
{
	CAD_FLOAT fProjWidth  = cExtentRect.Width();
	CAD_FLOAT fProjHeight = cExtentRect.Height();

	if( fProjWidth == 0 && fProjHeight == 0)
		return;

	// Camera Move
	CCAD_Point cCenterPt = cExtentRect.CenterPt();
	m_pCamera->MoveVerti(cCenterPt.m_fY);
	m_pCamera->MoveHoriz(cCenterPt.m_fX);

	int len = _min(cScrnRect.Width(), cScrnRect.Height());
	if( len <= nScreenMargin )
		nScreenMargin = 0;

	// CadDC scale 
	m_fBakScale = m_fScale;
	if( fProjWidth == 0.0f )
		m_fScale = ( cScrnRect.Height() - nScreenMargin) / fProjHeight;
	else if( fProjHeight == 0.0f )
		m_fScale = ( cScrnRect.Width() - nScreenMargin) / fProjWidth;
	else {
		CAD_FLOAT fxScale = ( cScrnRect.Width() - nScreenMargin) / fProjWidth;
		CAD_FLOAT fyScale = ( cScrnRect.Height() - nScreenMargin) / fProjHeight;

		if (fxScale < fyScale)
			m_fScale = fxScale;
		else
			m_fScale = fyScale;
	}

	if( m_fScale == 0.0f )
		return;

	fProjHeight = cScrnRect.Height() / m_fScale;
	fProjWidth = cScrnRect.Width() / m_fScale;

	Init(fProjWidth, fProjHeight);
}

short CCAD_DC::GetCircleDivide(CAD_FLOAT r)
{
	short n = m_pPlanInfo->nCircleDivide;

	//r = r * m_fScale * PAI / 2 / m_pCamera->m_fScale;
	if (m_pCamera->m_bPerspect)
	{
		CCAD_Vector pt1, pt2;
		pt2.m_fX = r;

		WCStoLP(pt1);
		WCStoLP(pt2);

		r = pt1.CountLength(pt2);
	}
	else
	{
		r = r * m_fScale * PAI / m_pCamera->m_fScale;
	}

	if ( r < n )
		n = (short)(r+0.5f);
	if ( n < 4 )
		n = 4;

	return n;
}

short CCAD_DC::GetArcDivide(const CCAD_Angle & cDelta, CAD_FLOAT r)
{
	short n = (short)(m_pPlanInfo->nCircleDivide * cDelta.GetAbsDegree() / LONGDOUBLE_NINETY + 0.5);

	r = r * m_fScale * cDelta.GetAbsRadian() / 2 / m_pCamera->m_fScale;
	if ( r < n )
		n = (short)r;
	if ( n < 3 )
		n = 3;

	return n;
}

short CCAD_DC::GetEllipseDivide(CAD_FLOAT rx, CAD_FLOAT ry)
{
	short n = m_pPlanInfo->nEllpDivide;
	
	rx = (rx+ry)/2 * m_fScale * PAI / m_pCamera->m_fScale;
	if ( rx < n )
		n = (short)rx;
	if ( n < 4 )
		n = 4;

	return n;
}

short CCAD_DC::GetEllipseArcDivide(const CCAD_Angle & cDelta, CAD_FLOAT rx, CAD_FLOAT ry)
{
	short n = (short)(m_pPlanInfo->nEllpDivide * cDelta.GetAbsDegree() / LONGDOUBLE_NINETY + 0.5);
	
	rx = (rx+ry)/2 * m_fScale * cDelta.GetAbsRadian() / 2 / m_pCamera->m_fScale;
	if ( rx < n )
		n = (short)rx;
	if ( n < 3 )
		n = 3;

	return n;
}

void CCAD_DC::ChangeCamera()
{
	if (m_pCamera == NULL)
		return;

	if (m_pCamera->m_bPerspect)
		m_bChangeCamera |= CHANGE_PERS;
	else
		m_bChangeCamera |= CHANGE_PALL;
}



void CCAD_DC::SetProjectMode(BOOL blPerspective, const RECT& viewRect)
{
	ASSERT(m_pCamera);

	if( blPerspective )
		SetPerspectiveMode(viewRect);
	else
		SetParallelMode(viewRect);
}



void CCAD_DC::SetPerspectiveMode( const RECT& viewRect)
{
	if( m_pCamera->m_bPerspect)
		return;

	//-------- for perspective
	m_bakParalCamera = *m_pCamera;
	m_bakParalRect = m_cClipRect;
	m_bakParalOrgLp.x = (LONG) m_sOrgLP.x;
	m_bakParalOrgLp.y = (LONG) m_sOrgLP.y;
	m_bakParalScale = m_fScale;
	m_bChangeCamera &= CHANGE_PALL;

	if ( (m_bChangeCamera & CHANGE_PALL) == 0 ) {
		*m_pCamera = m_bakPerspCamera;
		m_cClipRect = m_bakPerspRect;
		m_sOrgLP.x = (CAD_FLOAT) m_bakPerspOrgLp.x;
		m_sOrgLP.y = (CAD_FLOAT) m_bakPerspOrgLp.y;
		m_fScale = m_bakPerspScale;

		return;
	}

	//--------
	m_pCamera->m_bPerspect = true;

	if( m_cViewBox.m_fMaxZ <= m_cViewBox.m_fMinZ )
		return;

	CCAD_Point focusPt = m_cViewBox.CenterPt();
	//m_pCamera->ConvFromBase(focusPt);
//	focusPt.m_fZ = m_cViewBox.m_fMaxZ;
	m_pCamera->SetProjectPlan(focusPt);
}

void CCAD_DC::SetParallelMode( const RECT& viewRect)
{
	if( m_pCamera->m_bPerspect == false)
		return;

	//-------- for perspective
	m_bakPerspCamera = *m_pCamera;
	m_bakPerspRect = m_cClipRect;
	m_bakPerspOrgLp.x = (LONG)m_sOrgLP.x;
	m_bakPerspOrgLp.y = (LONG)m_sOrgLP.y;
	m_bakPerspScale = m_fScale;
	m_bChangeCamera &= CHANGE_PERS;

	if ( (m_bChangeCamera & CHANGE_PERS) == 0 ) {
		*m_pCamera = m_bakParalCamera;
		m_cClipRect = m_bakParalRect;
		m_sOrgLP.x = (CAD_FLOAT) m_bakParalOrgLp.x;
		m_sOrgLP.y = (CAD_FLOAT) m_bakParalOrgLp.y;
		m_fScale = m_bakParalScale;

		return;
	}
	//--------

	m_pCamera->m_bPerspect = false;

	if( m_cViewBox.m_fMaxZ <= m_cViewBox.m_fMinZ )
		return;

	CCAD_Point focusPt = m_cViewBox.CenterPt();
	//m_pCamera->ConvFromBase(focusPt);

	CAD_FLOAT fNewScale = (CAD_FLOAT)((double)m_fScale * (double)m_pCamera->m_fDistToEye / fabs((double)focusPt.m_fZ) );
	SetViewPort(viewRect, viewRect, fNewScale);

	focusPt.m_fZ = m_cViewBox.m_fMaxZ;
	m_pCamera->SetProjectPlan(focusPt);
}

void CCAD_DC::RotateVertical(const CCAD_Angle& cAngle)
{
	ASSERT(m_pCamera);

	CCAD_Point focusPt;

	if( m_cViewBox.m_fMaxX < m_cViewBox.m_fMinX )
		focusPt = CCAD_Point(0,0,-m_pCamera->m_fDistToEye);
	else
		focusPt = m_cViewBox.CenterPt();

// 	m_pCamera->ConvFromBase(focusPt);
	m_pCamera->RotateVertiFocusPoint(focusPt, cAngle);
}

void CCAD_DC::RotateHorizontal(const CCAD_Angle& cAngle)
{
	ASSERT(m_pCamera);

	CCAD_Point focusPt;

	if( m_cViewBox.m_fMaxX < m_cViewBox.m_fMinX )
		focusPt = CCAD_Point(0,0,-m_pCamera->m_fDistToEye);
	else
		focusPt = m_cViewBox.CenterPt();

// 	m_pCamera->ConvFromBase(focusPt);
	m_pCamera->RotateHorizFocusPoint(focusPt, cAngle);
}



void CCAD_DC::SetDspFile(CCAD_DspFile* pDspFile)
{
	m_pDspFile = pDspFile;

	if( m_pDspFile )
	{
		m_cViewBox.Init();
	}
}

void CCAD_DC::ReSetPriority(WORD wPriorityFlag, BOOL set)
{
	if (set)
		m_wPriority |= wPriorityFlag;
	else {
		if (m_wPriority & wPriorityFlag)
			m_wPriority -= wPriorityFlag;
	}
}

void CCAD_DC::FillSolidRect( RECT* rect, COLORREF color )
{
	float r = GetRValue(color) / 255.0f;
	float g = GetGValue(color) / 255.0f;
	float b = GetBValue(color) / 255.0f;

	glColor4f(r, g, b, 1.0f);
	static GLfloat vb[8];
	vb[0] = rect->left, vb[1] = rect->bottom;
	vb[2] = rect->right, vb[3] = rect->bottom;
	vb[4] = rect->left, vb[5] = rect->top;
	vb[6] = rect->right, vb[7] = rect->top;
	glVertexPointer(2, GL_FLOAT, 0, vb);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

#ifdef SUPPORT_VIEWPORT_CLIPPING
void CCAD_DC::SelectClipRect(const RECT& rect)
{
	glEnable(GL_SCISSOR_TEST);
	glScissor(rect.left, rect.bottom, rect.right - rect.left, rect.top - rect.bottom);
}

void CCAD_DC::ReleaseClipRect()
{
	glDisable(GL_SCISSOR_TEST);
}
#endif

LOGPEN CCAD_DC::SelectPen( const LOGPEN &pen )
{
	LOGPEN oldPen;
	oldPen = m_sPen;
	float fWidth = 1;
	m_sPen.lopnStyle = pen.lopnStyle;
	m_sPen.lopnWidth = pen.lopnWidth;
	m_sPen.lopnColor = pen.lopnColor;

	float r = GetRValue(m_sPen.lopnColor) / 255.0f;
	float g = GetGValue(m_sPen.lopnColor) / 255.0f;
	float b = GetBValue(m_sPen.lopnColor) / 255.0f;
	if(m_sPen.lopnWidth.x != 0)
		fWidth = m_sPen.lopnWidth.x;

	glColor4f(r, g, b, 1);
	glLineWidth(fWidth);
	
	return oldPen;
}

void CCAD_DC::RestoreObject( const LOGPEN &pen )
{
	SelectPen(pen);
}

void CCAD_DC::MoveTo( int x, int y )
{
	if(m_nLineType != LINETYPE_NULL){
		Lines_MoveTo(x, y);
	}
	m_cOrgScreenPt.x = x;
	m_cOrgScreenPt.y = y;
	m_blMoveTo = true;
}

void CCAD_DC::LineTo( int x, int y )
{
#ifdef _PERFORMACE_SUPPORT_
	PERFORMANC_INCREASE_ALL_ENTITY_COUNT(2);
#endif
	if(m_sPen.lopnStyle == PS_NULL) return;

	static GLfloat vb[4];

	float fWidth = 1;
	if(m_sPen.lopnWidth.x != 0)
		fWidth = m_sPen.lopnWidth.x;
	if (m_sPen.lopnStyle != PS_NULL)
	{
		if(m_nLineType != LINETYPE_NULL){
			Lines_LineTo(x, y);
		}else{
			vb[0] = m_cOrgScreenPt.x, vb[1] = m_cOrgScreenPt.y;
			vb[2] = x, vb[3] = y;
// 		if(fWidth > 5)
// 		{
// 			glEnable(GL_LINE_SMOOTH);
// 			glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
// 		}
// 		
		//glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, vb);
			glDrawArrays(GL_LINES, 0, 2);
		}
		//glDisableClientState(GL_VERTEX_ARRAY);
// 		glDisable(GL_LINE_SMOOTH);
// 		if(fWidth > 5)
// 		{
// 			glEnable (GL_POINT_SMOOTH);
// 			glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
// 			glPointSize(fWidth);
// 			if(m_blMoveTo)
// 			{
// 				glVertexPointer(2, GL_FLOAT, 0, vb);
// 				glDrawArrays (GL_POINTS , 0 , 1);
// 				m_blMoveTo = false;
// 			}
// 			glVertexPointer(2, GL_FLOAT, 0, vb + 2);
// 			glDrawArrays (GL_POINTS , 0 , 1);
// 			glDisable(GL_POINT_SMOOTH);
// 			glPointSize(1); 
// 		}
	}
	m_cOrgScreenPt.x = x;
	m_cOrgScreenPt.y = y;
}

LOGBRUSH CCAD_DC::SelectBrush( const LOGBRUSH &brush )
{
	LOGBRUSH oldBrush;
	oldBrush = m_sBrush;
	m_sBrush = brush;
	return oldBrush;
}

void CCAD_DC::RestoreObject( const LOGBRUSH &brush )
{
	SelectBrush(brush);
}

LOGPEN CCAD_DC::SelectNullPen()
{
	LOGPEN logPen;
	logPen.lopnStyle = PS_NULL;
	logPen.lopnWidth.x = logPen.lopnWidth.y = 0;

	return SelectPen(logPen);
}

LOGBRUSH CCAD_DC::SelectNullBrush()
{
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_NULL;

	return SelectBrush(logBrush);
}

LOGBRUSH CCAD_DC::SelectColorBrush( COLORREF color )
{
	if ( m_sBrush.lbColor != color || m_sBrush.lbStyle != BS_SOLID )
	{
		LOGBRUSH newLogBrush = m_sBrush;
		newLogBrush.lbColor = color;
		newLogBrush.lbStyle = BS_SOLID;
		return SelectBrush(newLogBrush);
	}
	else
		return m_sBrush;
}

LOGPEN CCAD_DC::SelectColorPen( COLORREF color )
{
	if ( m_sPen.lopnColor != color || m_sPen.lopnWidth.x != 0 )
	{
		LOGPEN newLogPen = m_sPen;
		newLogPen.lopnColor = color;
		newLogPen.lopnWidth.x = newLogPen.lopnWidth.y = 0;
		return SelectPen(newLogPen);
	}
	else
		return m_sPen;
}

LOGPEN CCAD_DC::SelectBrushColorPen()
{
	LOGPEN newPen;
	newPen = m_sPen;
	newPen.lopnColor = m_sBrush.lbColor;
	return SelectPen(newPen);
}

void CCAD_DC::StretchDIBits( int x0, int y0, int dx, int src_x0, int src_y0, int src_dx, CHAR* lpBits )
{
	int x;
	for ( x = x0; x < x0 + dx; x++ )
	{
		int sx = src_x0 + dx * (x - x0) / src_dx;
		COLORREF color = RGB ( lpBits[ sx*3 + 2], lpBits[ sx*3 + 1 ], lpBits[ sx*3 ] ) ;
		SetPixel(x , y0, color );
	}
}

void CCAD_DC::SetPixel( int x, int y, COLORREF rgbColor )
{
	GLfloat pt[2];
	pt[0] = x;
	pt[1] = y;
	float r = GetRValue(rgbColor) / 255.0f;
	float g = GetGValue(rgbColor) / 255.0f;
	float b = GetBValue(rgbColor) / 255.0f;

	glColor4f(r, g, b, 1);
	if(m_sPen.lopnStyle != PS_NULL){
		glVertexPointer(2, GL_FLOAT, 0, pt);
		glDrawArrays(GL_POINTS, 0, 1);
	}
}

void CCAD_DC::SetCameraInfo( CAD_FLOAT fWidth, CAD_FLOAT fHeight )
{
	ASSERT(m_pCamera);

	if(m_pCamera)
		m_pCamera->SetVcsSize(fWidth, fHeight);

	m_cClipRect.m_fMinX = - fWidth / 2;
	m_cClipRect.m_fMaxX = fWidth / 2;
	m_cClipRect.m_fMinY = - fHeight / 2;
	m_cClipRect.m_fMaxY = fHeight / 2;
}

PFONT CCAD_DC::CreateFontIndirect ( LOGFONTA& logfont )
{
	PFONT newFont = NULL;
    newFont = new FONT;
    memset(newFont, 0, sizeof(FONT));

	FT_Matrix tmp;

	if ( logfont.lfEscapement == 0 ) {
		tmp.xx = tmp.yy = FT_FIXED_ONE;
		tmp.xy = tmp.yx = 0;
	} else {
		CCAD_Angle ang = (float)( ( logfont.lfEscapement )/10 * PAI / 180 );
		tmp.xx = tmp.yy= ang.cos() * FT_FIXED_ONE;
		tmp.xy = -ang.sin() * FT_FIXED_ONE;
		tmp.yx = -tmp.xy;
	}

	if ( logfont.lfItalic == FALSE ) {
		newFont->fntMat = tmp;
	} else {
		newFont->fntMat = italicMat;
		FT_Matrix_Multiply( &tmp, &(newFont->fntMat) );
	}

	FT_Set_Transform( g_Face, &newFont->fntMat, NULL );

	newFont->fntBold = ( logfont.lfWeight >= 700 ) ? 1 : 0;
	newFont->fntUnderline = logfont.lfUnderline;
	newFont->fntStrikeOut = logfont.lfStrikeOut;

	double h = logfont.lfHeight;
	if ( h < 0 ) h = -h;
	newFont->fntHeight = (int)(h * 1.055 + 0.5);
	if ( logfont.lfWidth != 0 )
		newFont->fntWidth  = logfont.lfWidth * 200 / 95;

	FT_Set_Pixel_Sizes( g_Face, newFont->fntWidth, newFont->fntHeight );

	newFont->fntAscent = ( g_Face->size->metrics.ascender ) >> 6;

	return newFont;
}

PFONT CCAD_DC::SelectFont( PFONT font )
{
	PFONT oldFont;
	oldFont = m_pFont;
	m_pFont = font;
	return oldFont;	
}

CCAD_Vector CCAD_DC::GetTextExtent( LPSTR lpStr, int cbString, BOOL blGemCell /*= FALSE*/ )
{
	CCAD_Vector	cPt;
	SIZE size;

	GetTextExtentPoint32(lpStr, cbString, &size, blGemCell);

	cPt.m_fX = ConvScalarScrnToWorld((short)size.cx);
	cPt.m_fY = ConvScalarScrnToWorld((short)size.cy);
	cPt.m_fZ = 0;

	return cPt;
}

void BoldProcess( FT_GlyphSlot  slot );
int width_StringFT (PFONT font,LPSTR string, int numchar);

BOOL CCAD_DC::GetTextMetrics(TEXTMETRIC* tm)
{
	if(!tm) return FALSE;
	if (!m_pFont) return FALSE;

	memset(tm, 0x00, sizeof(TEXTMETRIC));
	tm->tmAscent = m_pFont->fntAscent;
	tm->tmHeight = m_pFont->fntHeight;
	tm->tmDescent = tm->tmHeight - tm->tmAscent;

	return TRUE;
}

BOOL CCAD_DC::GetTextExtentPoint32( LPSTR lpString, int cbString, LPSIZE lpSize, BOOL blGemCell /*= FALSE*/ )
{
	lpSize->cx = 0;
	lpSize->cy = 0;
	if(!blGemCell) {
		if ( !m_pFont ) return FALSE;
		if (lpString == NULL || cbString <= 0)
			return FALSE;
		if (lpString[0] == 0)
			return FALSE;

		PFONT		font = m_pFont;
		lpSize->cx = width_StringFT(font,lpString,cbString);
		lpSize->cy = font->fntHeight;
		return TRUE;
	}
	else {
// 		if ( m_pCurFont == NULL ) return FALSE;
// 		if (lpString == NULL || cbString <= 0)
// 			return FALSE;
// 		if (lpString[0] == 0)
// 			return FALSE;
// 		// 		QFontMetrics fm = m_pDC->fontMetrics();
// 		lpSize->cx = lpSize->cy = m_pCurFont->pointSize();
// 		//		lpSize->cx = fm.width(QString::fromUtf16(lpString),-1);
		return FALSE;
	}
}

int width_StringFT (PFONT font,LPSTR string, int numchar) {
	int horiAdvance;

	FT_Face face;
	FT_GlyphSlot slot;

	face = g_Face; /* shortcut */
	slot = face->glyph;   /* shortcut */

	double sin_a = font->fntMat.yx / FT_FIXED_ONE;
	double cos_a = font->fntMat.xx / FT_FIXED_ONE;

	int len, i;
	char ch;

	LPSTR next = string;

	char	 next1[EXT_LENGTH_NAME];
	memset(next1, 0, EXT_LENGTH_NAME);
	strcpy(next1, next);
	ConvertStringLocalToUni(next1, numchar);

	int render_mode = FT_LOAD_RENDER;
//	if ( sin_a != 0 )
//		render_mode |= FT_LOAD_NO_BITMAP;

	int xx = 0;

	for (i=0; i< numchar; i++)
	{
		ch = *next;
		
		if( (unsigned char)ch < (unsigned char)0x80)
			FT_Load_Char (face, next1[i] , render_mode);
		else
		{
			unsigned short	nCode;
			nCode = (next1[i] << 8) | (next1[i + 1] & 0xFF);
			FT_Load_Char (face, nCode, render_mode);
			i++;
			next++;
		}

		next++;

		if ( font->fntBold == 1 ) {
			BoldProcess( face->glyph );
		}

		horiAdvance = slot->metrics.horiAdvance;
		xx += horiAdvance;
	}

	int cx = ( xx + 32 ) >> 6;

	return cx;
}

void BoldProcess( FT_GlyphSlot  slot ) {
#define FIXED_FLOOR( x ) ( (x) & 0xFFFFFFC0 )
	FT_Library  library = slot->library;
	FT_Face     face    = slot->face;
	FT_Error    error;
	FT_Pos      xstr, ystr;

	if ( slot->format != FT_GLYPH_FORMAT_OUTLINE &&
			slot->format != FT_GLYPH_FORMAT_BITMAP )
		return;

	/* some reasonable strength */
	xstr = FT_MulFix( face->units_per_EM, face->size->metrics.y_scale ) / 24;
	ystr = xstr;

	if ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
	{
/*		error = FT_Outline_Embolden( &slot->outline, xstr );*/
		/* ignore error */

		/* this is more than enough for most glyphs; if you need accurate */
		/* values, you have to call FT_Outline_Get_CBox                   */
		xstr = xstr * 2;
		ystr = xstr;
	}
	else if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
	{
/*		xstr = FIXED_FLOOR( xstr );
		if ( xstr == 0 )
			xstr = 1 << 6;
		ystr = FIXED_FLOOR( ystr );

		error = FT_GlyphSlot_Own_Bitmap( slot );
		if ( error )
			return;

		error = FT_Bitmap_Embolden( library, &slot->bitmap, xstr, ystr );
		if ( error )
			return;*///surround 2008.7.21 for compiling on redstar system
	}

	if ( slot->advance.x )
		slot->advance.x += xstr;

	if ( slot->advance.y )
		slot->advance.y += ystr;

	slot->metrics.width        += xstr;
	slot->metrics.height       += ystr;
	//	slot->metrics.horiBearingY += ystr;
	//	slot->metrics.horiAdvance  += xstr;
	slot->metrics.horiAdvance  += 1<<6;
	slot->metrics.vertBearingX -= xstr / 2;
	slot->metrics.vertBearingY += ystr;
	slot->metrics.vertAdvance  += ystr;

	if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
		slot->bitmap_top += ystr >> 6;
}		

void CCAD_DC::Polygon( POINT* lpPoints, int nCount )
{
	Lines_Begin(LINETYPE_STRIP);
	MoveTo(lpPoints[0].x, lpPoints[0].y);
	for(int i = 1; i < nCount; i++)
	{
		LineTo(lpPoints[i].x, lpPoints[i].y);
	}
	LineTo(lpPoints[0].x, lpPoints[0].y);
	Lines_End();
}

int CCAD_DC::monotone_check( SCAD_POINT *vertices, int nvertices )
{
	int i, j, k;
	int leftmost_vertex = 0;
	int rightmost_vertex = 0;
	SCAD_POINT leftmost, rightmost, tmp;
	leftmost.m_fX = vertices[0].m_fX;
	rightmost.m_fX = vertices[0].m_fX;

	//Find the leftmost & rightmost vertices in the path
	for(i = 1; i < nvertices; i++){
		if(vertices[i].m_fX < leftmost.m_fX) {
			leftmost.m_fX = vertices[i].m_fX;
			leftmost_vertex = i;
		}
		else if(vertices[i].m_fX > rightmost.m_fX) {
			rightmost.m_fX = vertices[i].m_fX;
			rightmost_vertex = i;
		}
	}

	i = leftmost_vertex;
	tmp.m_fX = vertices[i--].m_fX;
	while(i != rightmost_vertex){
		if(vertices[i].m_fX <= tmp.m_fX)
			return 0;
		tmp.m_fX = vertices[i].m_fX;
		i = (i + nvertices -1) % nvertices;
	} 

	i = leftmost_vertex;
	tmp.m_fX = vertices[i++].m_fX;
	while(i != rightmost_vertex){

		if(vertices[i].m_fX <= tmp.m_fX)
			return 0;
		tmp.m_fX = vertices[i].m_fX;
		i = (i + 1) % nvertices;
	}
	return 1;
}

int CCAD_DC::LeftOnRight( SCAD_POINT a, SCAD_POINT b, SCAD_POINT c )
{
	float area = a.m_fX*b.m_fY - a.m_fY*b.m_fX +
		a.m_fY*c.m_fX - a.m_fX*c.m_fY +
		b.m_fX*c.m_fY - c.m_fX*b.m_fY;
	if(area>0)		return 0;//left
	else if (area=0)	return 2;//on the line
	else 			return 1;//right
}

void CCAD_DC::shMonoTriangulation( SCAD_POINT *vertices, int nvertices, SCAD_POINT *triangles, int ntriangles )
{
	//LOGD("Monotone polygon triangulation");
	// * Not finish yet
	int i, j, k;
	int leftmost_vertex = 0;
	int rightmost_vertex = 0;
	SCAD_POINT leftmost, rightmost, privious, next;
	leftmost.m_fX = vertices[0].m_fX;
	rightmost.m_fX = vertices[0].m_fX;

	//Find the leftmost & rightmost vertices in the path
	for(i = 1; i < nvertices; i++){
		if(vertices[i].m_fX < leftmost.m_fX) {
			leftmost.m_fX = vertices[i].m_fX;
			leftmost_vertex = i;
		}
		else if(vertices[i].m_fX > rightmost.m_fX) {
			rightmost.m_fX = vertices[i].m_fX;
			rightmost_vertex = i;
		}
	}

	int *chain = (int*)malloc(nvertices * sizeof(int));
	next.m_fX = vertices[(leftmost_vertex + 1) % nvertices].m_fX;
	next.m_fY = vertices[(leftmost_vertex + 1) % nvertices].m_fY;
	privious.m_fX = vertices[(leftmost_vertex + nvertices -1) % nvertices].m_fX;
	privious.m_fY = vertices[(leftmost_vertex + nvertices -1) % nvertices].m_fY;
	int clockwise = next.m_fY > privious.m_fY? 1: 0;

	// 0:upper chain  1:lower chain  2:start  3:end
	for(i=0; i<nvertices; i++){
		chain[i] = 0;
	}
	i = leftmost_vertex;
	while(i != rightmost_vertex){
		chain[i] = 1;
		if(clockwise)	i = (i + nvertices -1) % nvertices;
		else		i = (i + 1) % nvertices;
	} 
	chain[leftmost_vertex] = 2;
	chain[rightmost_vertex] = 3;

	int *sort = (int*)malloc(nvertices * sizeof(int));
	sort[0] = leftmost_vertex;
	int p1, p2;

	p1 = (leftmost_vertex+1) % nvertices;
	p2 = (leftmost_vertex + nvertices -1) % nvertices;
	i = 1;
	while(i<nvertices){
		if(vertices[p1].m_fX < vertices[p2].m_fX){
			sort[i] = p1;
			p1 = (p1==nvertices -1)? 0: p1+1;
		}
		else{
			sort[i] = p2;
			p2 = (p2==0)? nvertices-1: p2-1;
		}
		i++;
	}

	//starting tiangulation
	//int *tri = triangles;
	j = 0;
	int *stack = (int*)malloc(nvertices * sizeof(int));
	int top, current;
	SCAD_POINT v1, v2, v3;
	stack[0] = sort[0];
	stack[1] = sort[1];
	top = 1;
	for(i=2; i<nvertices; i++){

		current = sort[i];
		if(chain[current] != chain[ stack[top] ]){
			k = stack[top];
			while(top!=0){
				triangles[j*3] = vertices[current];
				triangles[j*3 + 1] = vertices[stack[top]];
				triangles[j*3 + 2] = vertices[stack[top-1]];
				top--;
				j++;
			}
			stack[0] = k;
			stack[1] = current;
			top = 1;
		}
		else{
			v1 = vertices[stack[top-1]];
			v2 = vertices[stack[top]];
			v3 = vertices[current];

			if( (chain[ stack[top] ]==0&& LeftOnRight(v1, v2, v3)==0)  ||  (chain[stack[top]]==1&& LeftOnRight(v1, v2, v3)==1)){
				stack[top+1] = current;
				top++;
			}
			else if(LeftOnRight(v1, v2, v3)==2){
				stack[top+1] = current;
				top++;
			}
			else{
				do{

					triangles[j*3] = vertices[current];
					triangles[j*3 + 1] = vertices[stack[top]];
					triangles[j*3 + 2] = vertices[stack[top-1]];
					top--;
					j++;

					if(top==0)	break;

					v1 = vertices[stack[top-1]];
					v2 = vertices[stack[top]];
				}while( !((chain[ stack[top] ]==0&& LeftOnRight(v1, v2, v3)==0)  ||  (chain[stack[top]]==1&& LeftOnRight(v1, v2, v3)==1)) );
				stack[top+1] = current;
				top++;
			}
		}
	}
	free(chain);
	free(sort);
	free(stack);
}

void CCAD_DC::shTriangulation( SCAD_POINT *vertices, int nvertices, SCAD_POINT *triangles, int ntriangles )
{
	//LOGD("Ear-clipping triangulation");
	SCAD_POINT current, left, right, internal;
	SCAD_POINT s1, s2, s3;
	int *plunk;
	int i, j, k;
	int cross;
	plunk = (int *) malloc(nvertices * sizeof(int));
	for (i = 0; i < nvertices; i++) {
		plunk[i] = 0;
	}
	for (i = 0; i < ntriangles; i++) {
		// Find an ear triangle and plunk it.
		for (j = 0; j < nvertices; j++) {
			if (plunk[j] == 1) {
				continue;
			}
			// Find 1st point of the triangle
			current.m_fX = vertices[j].m_fX;
			current.m_fY = vertices[j].m_fY;
			// Find 2nd point of the triangle
			k = j;
			do {
				if (k == nvertices) {
					k = 0;
				} else {
					k++;
				}
				if (plunk[k] == 0) {
					right.m_fX = vertices[k].m_fX;
					right.m_fY = vertices[k].m_fY;
					break;
				}
			} while (k != j);
			// Find 3rd point of the triangle
			k = j;
			do {
				if (k == -1) {
					k = nvertices - 1;
				} else {
					k--;
				}
				if (plunk[k] == 0) {
					left.m_fX = vertices[k].m_fX;
					left.m_fY = vertices[k].m_fY;
					break;
				}
			} while (k != j);
			// Test triangle(left, current, right) form an
			// ear triangle or not
			internal.m_fX = (left.m_fX + right.m_fX) /2;
			internal.m_fY = (left.m_fY + right.m_fY) /2;
			for (k = 0, cross = 0; k < nvertices; k++) {
				// Pick a segment, (s1, s2)
				s1.m_fX = vertices[k].m_fX;
				s1.m_fY = vertices[k].m_fY;
				if ((k + 1) == nvertices) {
					s2.m_fX = vertices[0].m_fX;
					s2.m_fY = vertices[0].m_fY;
				} else {
					s2.m_fX = vertices[k + 1].m_fX;
					s2.m_fY = vertices[k + 1].m_fY;
				}
				if (((s1.m_fY > internal.m_fY) && (s2.m_fY < internal.m_fY))
					|| ((s1.m_fY < internal.m_fY) && (s2.m_fY > internal.m_fY))){
						s3.m_fY = internal.m_fY;
						s3.m_fX = (s3.m_fY - s1.m_fY) * (s1.m_fX - s2.m_fX) / (s1.m_fY - s2.m_fY) + s1.m_fX;
						if (s3.m_fX >= internal.m_fX) {
							cross++;
						}
				}
			}
			if ((cross % 2) == 1) {
				// Internal(x,y) is the real internal point
				// We find one ear triangle 
				// Plunk it and assign to ouput triangle array
				plunk[j] = 1;	
				triangles[i * 3].m_fX = current.m_fX;
				triangles[i * 3].m_fY = current.m_fY;
				triangles[i * 3 + 1].m_fX = left.m_fX;
				triangles[i * 3 + 1].m_fY = left.m_fY;
				triangles[i * 3 + 2].m_fX = right.m_fX;
				triangles[i * 3 + 2].m_fY = right.m_fY;
				break;
			}
		}
	}
	free(plunk);
}

BOOL CCAD_DC::TextOut(CCAD_Vector *rect, LPCSTR lpcStr, int cbStr, BOOL bDispOnlyOutLine)
{
	BOOL	blRet = FALSE;

	if(!m_pFont)
		return FALSE;

	char	 lpStr[EXT_LENGTH_NAME];
	memset(lpStr, 0, EXT_LENGTH_NAME);
	strcpy(lpStr, lpcStr);
	ConvertStringLocalToUni(lpStr, cbStr);

	FT_Error      error;
    FT_Face       face = g_Face;
	FT_GlyphSlot  slot = face->glyph;
	FT_Vector     pen;
	pen.x = 0;
	pen.y = 0;
	int i, j;
	float ang = atan2(rect[2].m_fY - rect[1].m_fY, rect[2].m_fX - rect[1].m_fX);
	float sinval = (float)sin(ang);
	float cosval = (float)cos(ang);
	CCAD_Vector vtr1(rect[1], rect[2]), vtr2(rect[1], rect[0]);
	if(vtr1.m_fX * vtr2.m_fY - vtr1.m_fY * vtr2.m_fX < 0.0f) ang = -ang;
	for ( int n = 0; n < cbStr; n++ )
	{
		if(CheckMessage())
			return FALSE;
		FT_Set_Transform( face, &m_pFont->fntMat, &pen );
		// load glyph image into the slot (erase previous one)
		int charwid = 1;
		if( (unsigned char)lpcStr[n] < (unsigned char)0x80)
			error = FT_Load_Char( face, lpStr[n], FT_LOAD_NO_BITMAP );
		else
		{
			unsigned short	nCode;
			nCode = (lpStr[n] << 8) | (lpStr[n + 1] & 0xFF);
			error = FT_Load_Char( face, nCode, FT_LOAD_NO_BITMAP );
			n++;
			charwid++;
		}
		if ( error )
			continue;                 // ignore errors
		
		CCAD_Vector charrect[4];
		float ratio = (float)(n - charwid + 1) / cbStr;
		charrect[0].m_fX = rect[0].m_fX + (rect[3].m_fX - rect[0].m_fX) * ratio;
		charrect[0].m_fY = rect[0].m_fY + (rect[3].m_fY - rect[0].m_fY) * ratio;
		charrect[0].m_fZ = 0.0f;
		charrect[1].m_fX = rect[1].m_fX + (rect[2].m_fX - rect[1].m_fX) * ratio;
		charrect[1].m_fY = rect[1].m_fY + (rect[2].m_fY - rect[1].m_fY) * ratio;
		charrect[1].m_fZ = 0.0f;
		ratio = (float)(n + 1) / cbStr;
		charrect[3].m_fX = rect[0].m_fX + (rect[3].m_fX - rect[0].m_fX) * ratio;
		charrect[3].m_fY = rect[0].m_fY + (rect[3].m_fY - rect[0].m_fY) * ratio;
		charrect[3].m_fZ = 0.0f;
		charrect[2].m_fX = rect[1].m_fX + (rect[2].m_fX - rect[1].m_fX) * ratio;
		charrect[2].m_fY = rect[1].m_fY + (rect[2].m_fY - rect[1].m_fY) * ratio;
		charrect[2].m_fZ = 0.0f;
		float xmin, xmax, ymin, ymax;
		xmin = xmax = slot->outline.points[0].x;
		ymin = ymax = slot->outline.points[0].y;
		for(i = 0; i < slot->outline.n_points; ++i){
			if(slot->outline.points[i].x < xmin) xmin = slot->outline.points[i].x;
			else if(slot->outline.points[i].x > xmax) xmax = slot->outline.points[i].x;
			if(slot->outline.points[i].y < ymin) ymin = slot->outline.points[i].y;
			else if(slot->outline.points[i].y > ymax) ymax = slot->outline.points[i].y;
		}
		float xratio = sqrt((charrect[2].m_fX - charrect[1].m_fX) * (charrect[2].m_fX - charrect[1].m_fX) +
						(charrect[2].m_fY - charrect[1].m_fY) * (charrect[2].m_fY - charrect[1].m_fY)) / (xmax - xmin);
		float yratio = sqrt((charrect[0].m_fX - charrect[1].m_fX) * (charrect[0].m_fX - charrect[1].m_fX) +
						(charrect[0].m_fY - charrect[1].m_fY) * (charrect[0].m_fY - charrect[1].m_fY)) / (ymax - ymin);

		CCAD_Vector curpnt, firstpnt;
		if(bDispOnlyOutLine){
			for(i = j = 0; i < slot->outline.n_contours; ++i){
				float xpos = (slot->outline.points[j].x - xmin) * xratio;
				float ypos = (slot->outline.points[j].y - ymin) * yratio;
				curpnt.m_fX = charrect[1].m_fX + xpos * cosval - ypos * sinval;
				curpnt.m_fY = charrect[1].m_fY + xpos * sinval + ypos * cosval;
				curpnt.m_fZ = 0.0f;
				firstpnt = curpnt;

				Lines_Begin(LINETYPE_STRIP);
				MoveTo(curpnt);
				for(; j <= slot->outline.contours[i]; ++j){
					xpos = (slot->outline.points[j].x - xmin) * xratio;
					ypos = (slot->outline.points[j].y - ymin) * yratio;
					curpnt.m_fX = charrect[1].m_fX + xpos * cosval - ypos * sinval;
					curpnt.m_fY = charrect[1].m_fY + xpos * sinval + ypos * cosval;
					curpnt.m_fZ = 0.0f;
					LineTo(curpnt);
				}
				LineTo(firstpnt);
				Lines_End();
			}
		}else{
			Tesselator _tesselator;

			long vtxnum = slot->outline.n_points * 3;
			MallocTriVtxBuf(vtxnum);
			double *curtrivtx = m_dpTriVtx;
			_tesselator.beginPolygon();
			for ( i = j = 0 ; i < slot->outline.n_contours ; i++ )
			{      
				_tesselator.beginContour();
				for ( ; j <= slot->outline.contours[i] ; j++ )
				{
					float xpos = (slot->outline.points[j].x - xmin) * xratio;
					float ypos = (slot->outline.points[j].y - ymin) * yratio;
					curtrivtx[0] = charrect[1].m_fX + xpos * cosval - ypos * sinval;
					curtrivtx[1] = charrect[1].m_fY + xpos * sinval + ypos * cosval;
					curtrivtx[2] = 0.0f;
					_tesselator.feedvert(curtrivtx);
					curtrivtx += 3;
				}
				_tesselator.endContour();
			}

			std::vector< float > triangles;
			_tesselator.tesselate( triangles );
			long trisize = triangles.size();

			for(i = 0; i < trisize; i += 6)
			{
				SCAD_POINT pnt[3];
				pnt[0].m_fX = triangles[i];
				pnt[0].m_fY = triangles[i+1];
				pnt[0].m_fZ = 0.0f;
				pnt[1].m_fX = triangles[i+2];
				pnt[1].m_fY = triangles[i+3];
				pnt[1].m_fZ = 0.0f;
				pnt[2].m_fX = triangles[i+4];
				pnt[2].m_fY = triangles[i+5];
				pnt[2].m_fZ = 0.0f;
				m_pCamera->ConvFromBase(pnt[0]);
				m_pCamera->ConvFromBase(pnt[1]);
				m_pCamera->ConvFromBase(pnt[2]);
				float tvtx[10];
				tvtx[0] = pnt[0].m_fX;
				tvtx[1] = pnt[0].m_fY;
				tvtx[2] = pnt[0].m_fZ;
				tvtx[3] = pnt[1].m_fX;
				tvtx[4] = pnt[1].m_fY;
				tvtx[5] = pnt[1].m_fZ;
				tvtx[6] = pnt[2].m_fX;
				tvtx[7] = pnt[2].m_fY;
				tvtx[8] = pnt[2].m_fZ;
				glVertexPointer(3, GL_FLOAT, 0, tvtx);
				glDrawArrays(GL_TRIANGLES, 0, 3);
			}
		}
	}

	return blRet;
}

BOOL CCAD_DC::TextOut(WORD wChar, CCAD_Vector *rect, float w_hScale, float fSkew, BOOL bDispOnlyOutLine)
{
	BOOL	blRet = FALSE;

	if(!m_pFont)
		return FALSE;

	FT_Error      error;
	FT_Face       face = g_Face;
	FT_GlyphSlot  slot = face->glyph;
	FT_Vector     pen;
	pen.x = 0;
	pen.y = 0;
	int i, j;
	float ang = atan2(rect[2].m_fY - rect[1].m_fY, rect[2].m_fX - rect[1].m_fX);
	float sinval = (float)sin(ang);
	float cosval = (float)cos(ang);
	CCAD_Vector vtr1(rect[1], rect[2]), vtr2(rect[1], rect[0]);
	if(vtr1.m_fX * vtr2.m_fY - vtr1.m_fY * vtr2.m_fX < 0.0f) ang = -ang;
	if(CheckMessage())
		return FALSE;
	FT_Set_Transform( face, &m_pFont->fntMat, &pen );
	// load glyph image into the slot (erase previous one)
	if( wChar < 0x80){
		unsigned char nCode = (unsigned char)(wChar & 0xFF);
		error = FT_Load_Char( face, nCode, FT_LOAD_NO_BITMAP );
	}
	else
	{
		error = FT_Load_Char( face, wChar, FT_LOAD_NO_BITMAP );
	}
	if ( error )
		return FALSE;                 // ignore errors
	
	float xratio = sqrt((rect[2].m_fX - rect[1].m_fX) * (rect[2].m_fX - rect[1].m_fX) +
		(rect[2].m_fY - rect[1].m_fY) * (rect[2].m_fY - rect[1].m_fY)) / slot->metrics.horiAdvance;
	float yratio = sqrt((rect[0].m_fX - rect[1].m_fX) * (rect[0].m_fX - rect[1].m_fX) +
		(rect[0].m_fY - rect[1].m_fY) * (rect[0].m_fY - rect[1].m_fY)) / slot->metrics.vertAdvance;
	xratio *= w_hScale;

	CCAD_Vector curpnt, firstpnt;
	if(bDispOnlyOutLine){
		for(i = j = 0; i < slot->outline.n_contours; ++i){
			float xpos = slot->outline.points[j].x * xratio;
			float ypos = slot->outline.points[j].y * yratio;
			xpos += (ypos * fSkew);
			curpnt.m_fX = rect[1].m_fX + xpos * cosval - ypos * sinval;
			curpnt.m_fY = rect[1].m_fY + xpos * sinval + ypos * cosval;
			curpnt.m_fZ = 0.0f;
			firstpnt = curpnt;

			Lines_Begin(LINETYPE_STRIP);
			MoveTo(curpnt);
			for(; j <= slot->outline.contours[i]; ++j){
				xpos = slot->outline.points[j].x * xratio;
				ypos = slot->outline.points[j].y * yratio;
				xpos += (ypos * fSkew);
				curpnt.m_fX = rect[1].m_fX + xpos * cosval - ypos * sinval;
				curpnt.m_fY = rect[1].m_fY + xpos * sinval + ypos * cosval;
				curpnt.m_fZ = 0.0f;
				LineTo(curpnt);
			}
			LineTo(firstpnt);
			Lines_End();
		}
	}else{
		Tesselator _tesselator;

		long vtxnum = slot->outline.n_points * 3;
		MallocTriVtxBuf(vtxnum);
		double *curtrivtx = m_dpTriVtx;
		_tesselator.beginPolygon();
		for ( i = j = 0 ; i < slot->outline.n_contours ; i++ )
		{      
			_tesselator.beginContour();
			for ( ; j <= slot->outline.contours[i] ; j++ )
			{
				float xpos = slot->outline.points[j].x * xratio;
				float ypos = slot->outline.points[j].y * yratio;
				xpos += (ypos * fSkew);
				curtrivtx[0] = rect[1].m_fX + xpos * cosval - ypos * sinval;
				curtrivtx[1] = rect[1].m_fY + xpos * sinval + ypos * cosval;
				curtrivtx[2] = 0.0f;
				_tesselator.feedvert(curtrivtx);
				curtrivtx += 3;
			}
			_tesselator.endContour();
		}

		std::vector< float > triangles;
		_tesselator.tesselate( triangles );
		long trisize = triangles.size();

		for(i = 0; i < trisize; i += 6)
		{
			SCAD_POINT pnt[3];
			pnt[0].m_fX = triangles[i];
			pnt[0].m_fY = triangles[i+1];
			pnt[0].m_fZ = 0.0f;
			pnt[1].m_fX = triangles[i+2];
			pnt[1].m_fY = triangles[i+3];
			pnt[1].m_fZ = 0.0f;
			pnt[2].m_fX = triangles[i+4];
			pnt[2].m_fY = triangles[i+5];
			pnt[2].m_fZ = 0.0f;
			m_pCamera->ConvFromBase(pnt[0]);
			m_pCamera->ConvFromBase(pnt[1]);
			m_pCamera->ConvFromBase(pnt[2]);
			float tvtx[10];
			tvtx[0] = pnt[0].m_fX;
			tvtx[1] = pnt[0].m_fY;
			tvtx[2] = pnt[0].m_fZ;
			tvtx[3] = pnt[1].m_fX;
			tvtx[4] = pnt[1].m_fY;
			tvtx[5] = pnt[1].m_fZ;
			tvtx[6] = pnt[2].m_fX;
			tvtx[7] = pnt[2].m_fY;
			tvtx[8] = pnt[2].m_fZ;
			glVertexPointer(3, GL_FLOAT, 0, tvtx);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}

	return blRet;
}

void CCAD_DC::GetViewPortBndBox(CCAD_Rect &bndbox)
{
	if(!g_blCalcBndBox){
		g_WorldBndBox.Init();
		g_pDoc->CountWorldRange(NULL, g_WorldBndBox);
		g_blCalcBndBox = TRUE;
	}
	bndbox = g_WorldBndBox;

	CCAD_Point pnt[8];
	pnt[0].m_fX = bndbox.m_fMinX;
	pnt[0].m_fY = bndbox.m_fMinY;
	pnt[0].m_fZ = bndbox.m_fMinZ;
	pnt[1].m_fX = bndbox.m_fMaxX;
	pnt[1].m_fY = bndbox.m_fMinY;
	pnt[1].m_fZ = bndbox.m_fMinZ;
	pnt[2].m_fX = bndbox.m_fMaxX;
	pnt[2].m_fY = bndbox.m_fMaxY;
	pnt[2].m_fZ = bndbox.m_fMinZ;
	pnt[3].m_fX = bndbox.m_fMinX;
	pnt[3].m_fY = bndbox.m_fMaxY;
	pnt[3].m_fZ = bndbox.m_fMinZ;
	pnt[4].m_fX = bndbox.m_fMinX;
	pnt[4].m_fY = bndbox.m_fMinY;
	pnt[4].m_fZ = bndbox.m_fMaxZ;
	pnt[5].m_fX = bndbox.m_fMaxX;
	pnt[5].m_fY = bndbox.m_fMinY;
	pnt[5].m_fZ = bndbox.m_fMaxZ;
	pnt[6].m_fX = bndbox.m_fMaxX;
	pnt[6].m_fY = bndbox.m_fMaxY;
	pnt[6].m_fZ = bndbox.m_fMaxZ;
	pnt[7].m_fX = bndbox.m_fMinX;
	pnt[7].m_fY = bndbox.m_fMaxY;
	pnt[7].m_fZ = bndbox.m_fMaxZ;
	long i, j;
	for(i = 0; i < 8; ++i){
		m_pCamera->ConvFromBase(pnt[i]);
	}
	CCAD_Point lnpnt[100];
	lnpnt[0] = pnt[0];
	lnpnt[1] = pnt[1];
	lnpnt[2] = pnt[1];
	lnpnt[3] = pnt[2];
	lnpnt[4] = pnt[2];
	lnpnt[5] = pnt[3];
	lnpnt[6] = pnt[3];
	lnpnt[7] = pnt[0];

	lnpnt[8] = pnt[4];
	lnpnt[9] = pnt[5];
	lnpnt[10] = pnt[5];
	lnpnt[11] = pnt[6];
	lnpnt[12] = pnt[6];
	lnpnt[13] = pnt[7];
	lnpnt[14] = pnt[7];
	lnpnt[15] = pnt[4];

	lnpnt[16] = pnt[0];
	lnpnt[17] = pnt[4];
	lnpnt[18] = pnt[1];
	lnpnt[19] = pnt[5];
	lnpnt[20] = pnt[2];
	lnpnt[21] = pnt[6];
	lnpnt[22] = pnt[3];
	lnpnt[23] = pnt[7];

	long linenum = 12;

	CCAD_Point intpnt[20], pnt1, pnt2;
	long nearidx;
	for(long k = 0; k < 4; ++k){
		long intpntnum = 0;
		for(i = 0; i < linenum; ++i){
			pnt1 = lnpnt[2*i];
			pnt2 = lnpnt[2*i+1];
			BOOL bClipResult = FALSE;
			if(k == 0){
				bClipResult = m_pCamera->ClipLeft(lnpnt[2*i], lnpnt[2*i+1]);
			}else if(k == 1){
				bClipResult = m_pCamera->ClipRight(lnpnt[2*i], lnpnt[2*i+1]);
			}else if(k == 2){
				bClipResult = m_pCamera->ClipTop(lnpnt[2*i], lnpnt[2*i+1]);
			}else{
				bClipResult = m_pCamera->ClipBottom(lnpnt[2*i], lnpnt[2*i+1]);
			}
			if(bClipResult){
				if(!lnpnt[2*i].Equal(pnt1,0) && !lnpnt[2*i].Equal(pnt2,0)){
					intpnt[intpntnum] = lnpnt[2*i];
					intpntnum++;
				}
				if(!lnpnt[2*i+1].Equal(pnt1,0) && !lnpnt[2*i+1].Equal(pnt2,0)){
					intpnt[intpntnum] = lnpnt[2*i+1];
					intpntnum++;
				}
			}else{
				memmove(&lnpnt[2*i], &lnpnt[2*i+2], sizeof(CCAD_Point) * 2 * (linenum - i - 1));
				i--;
				linenum--;
			}
		}
		//// make contour from inpnts
		long bottomidx = 0;
		for(i = 0; i < intpntnum; ++i){
			if(intpnt[i].m_fY < intpnt[bottomidx].m_fY) bottomidx = i;
		}
		float deltax, deltay;
		i = bottomidx;
		long previdx = -1;
		float prevang = PAI;
		for(long p = 0; p < intpntnum; ++p){
			long nextidx = -1;
			float nextang, curang;
			for(j = 0; j < intpntnum; ++j){
				if(j == previdx || j == i) continue;
				deltax = intpnt[j].m_fX - intpnt[i].m_fX;
				deltay = intpnt[j].m_fY - intpnt[i].m_fY;
				if(deltax != 0.0f || deltay != 0.0f){
					if(nextidx == -1){
						nextang = atan2(deltay, deltax);
						if(nextang < 0.0f) nextang += 2 * PAI;
						nextang -= prevang;
						if(nextang < 0.0f) nextang += 2 * PAI;
						nextidx = j;
					}else{
						curang = atan2(deltay, deltax);
						if(curang < 0.0f) curang += 2 * PAI;
						curang -= prevang;
						if(curang < 0.0f) curang += 2 * PAI;
						if(nextang > curang){
							nextang = curang;
							nextidx = j;
						}
					}
				}
			}
			if(nextidx != -1){
				lnpnt[2*linenum] = intpnt[i];
				lnpnt[2*linenum+1] = intpnt[nextidx];
				linenum++;
				previdx = i;
				i = nextidx;

				deltax = intpnt[previdx].m_fX - intpnt[i].m_fX;
				deltay = intpnt[previdx].m_fY - intpnt[i].m_fY;
				prevang = atan2(deltay, deltax);
				if(prevang < 0.0f) prevang += 2 * PAI;
			}
		}
	}
	if(linenum > 0){
		bndbox.m_fMinX = bndbox.m_fMaxX = lnpnt[0].m_fX;
		bndbox.m_fMinY = bndbox.m_fMaxY = lnpnt[0].m_fY;
		bndbox.m_fMinZ = bndbox.m_fMaxZ = lnpnt[0].m_fZ;
		for(i = 0; i < 2*linenum; ++i){
			if(lnpnt[i].m_fX < bndbox.m_fMinX) bndbox.m_fMinX = lnpnt[i].m_fX;
			else if(lnpnt[i].m_fX > bndbox.m_fMaxX) bndbox.m_fMaxX = lnpnt[i].m_fX;
			if(lnpnt[i].m_fY < bndbox.m_fMinY) bndbox.m_fMinY = lnpnt[i].m_fY;
			else if(lnpnt[i].m_fY > bndbox.m_fMaxY) bndbox.m_fMaxY = lnpnt[i].m_fY;
			if(lnpnt[i].m_fZ < bndbox.m_fMinZ) bndbox.m_fMinZ = lnpnt[i].m_fZ;
			else if(lnpnt[i].m_fZ > bndbox.m_fMaxZ) bndbox.m_fMaxZ = lnpnt[i].m_fZ;
		}
	}else{
		bndbox.m_fMinX = bndbox.m_fMaxX = 0.0f;
		bndbox.m_fMinY = bndbox.m_fMaxY = 0.0f;
		bndbox.m_fMinZ = bndbox.m_fMaxZ = 0.0f;
	}
}


BOOL CCAD_DC::IsVerticality() const
{
	CAD_FLOAT fScalar = m_pCamera ->m_cZ & CCAD_Vector(0, 0, 1);
	return IS_ZERO(fabs(fScalar) - 1);
}

double *CCAD_DC::MallocTriVtxBuf(long num)
{
	if(num > m_nTriVtxBufNum){
		if(m_dpTriVtx) 
		{
			delete [] m_dpTriVtx;
			m_dpTriVtx = NULL;
		}
		m_nTriVtxBufNum = num + 10;
		m_dpTriVtx = new double[m_nTriVtxBufNum];
	}
	return m_dpTriVtx;
}

void CCAD_DC::SetViewBox(const SCAD_POINT &ptECS)
{
	if(!m_bEnableViewBox) return;

	if (m_cViewBox.m_fMinX > ptECS.m_fX)
		m_cViewBox.m_fMinX = ptECS.m_fX;
	else if(m_cViewBox.m_fMaxX < ptECS.m_fX)
		m_cViewBox.m_fMaxX = ptECS.m_fX;

	if (m_cViewBox.m_fMinY > ptECS.m_fY)
		m_cViewBox.m_fMinY = ptECS.m_fY;
	else if(m_cViewBox.m_fMaxY < ptECS.m_fY)
		m_cViewBox.m_fMaxY = ptECS.m_fY;

	if (m_cViewBox.m_fMinZ > ptECS.m_fZ)
		m_cViewBox.m_fMinZ = ptECS.m_fZ;
	else if(m_cViewBox.m_fMaxZ < ptECS.m_fZ)
		m_cViewBox.m_fMaxZ = ptECS.m_fZ;

}

void CCAD_DC::MallocVtxBuf(long num)
{
	if(num > m_nVtxBufNum){
		long newvtxbufnum = num + 100;
		float *newvtxbuf = new float[newvtxbufnum];
		if(m_fpVtxBuf){
			memcpy(newvtxbuf, m_fpVtxBuf, sizeof(float) * m_nVtxBufNum);
			delete [] m_fpVtxBuf;
		}
		m_fpVtxBuf = newvtxbuf;
		m_nVtxBufNum = newvtxbufnum;
	}
}

void CCAD_DC::DrawLines()
{
	if(m_sPen.lopnStyle == PS_NULL) return;

	if(m_nVtxBufCurPos >= 4){
		if(m_nLineType == LINETYPE_STRIP){
			glVertexPointer(2, GL_FLOAT, sizeof(float) * 2, m_fpVtxBuf);
			glDrawArrays(GL_LINE_STRIP, 0, m_nVtxBufCurPos / 2);
		}else if(m_nLineType == LINETYPE_LINES){
			glVertexPointer(2, GL_FLOAT, sizeof(float) * 2, m_fpVtxBuf);
			glDrawArrays(GL_LINES, 0, m_nVtxBufCurPos / 2);
		}
	}
	m_nVtxBufCurPos = 0;
}

void CCAD_DC::Lines_Begin(DRAW_LINETYPE linetype)
{
	if(linetype == LINETYPE_STRIP){
		if( !m_bIsThick && m_pPenStyle) linetype = LINETYPE_LINES;
	}
	m_nLineType = linetype;
	m_nVtxBufCurPos = 0;
}

void CCAD_DC::Lines_End()
{
	DrawLines();
	m_nLineType = LINETYPE_NULL;
}

void CCAD_DC::Lines_MoveTo(int x, int y)
{
	if(m_nLineType == LINETYPE_STRIP){
		if(m_nVtxBufCurPos >= 2){
			if(m_fpVtxBuf[m_nVtxBufCurPos - 2] == x && m_fpVtxBuf[m_nVtxBufCurPos - 1] == y){
				return;
			}else{
				if(m_nVtxBufCurPos >= 4) DrawLines();
			}
		}
		m_nVtxBufCurPos = 0;
	}
	if(m_nVtxBufNum < m_nVtxBufCurPos + 2) MallocVtxBuf(m_nVtxBufCurPos + 2);
	m_fpVtxBuf[m_nVtxBufCurPos] = x;
	m_nVtxBufCurPos++;
	m_fpVtxBuf[m_nVtxBufCurPos] = y;
	m_nVtxBufCurPos++;
}

void CCAD_DC::Lines_LineTo(int x, int y)
{
	if(m_nLineType == LINETYPE_LINES && m_nVtxBufCurPos & 0x01 == 0 && m_nVtxBufCurPos >= 2){
		m_fpVtxBuf[m_nVtxBufCurPos] = m_fpVtxBuf[m_nVtxBufCurPos - 2];
		m_nVtxBufCurPos++;
		m_fpVtxBuf[m_nVtxBufCurPos] = m_fpVtxBuf[m_nVtxBufCurPos - 2];
		m_nVtxBufCurPos++;
	}
	if(m_nVtxBufNum < m_nVtxBufCurPos + 2) MallocVtxBuf(m_nVtxBufCurPos + 2);
	m_fpVtxBuf[m_nVtxBufCurPos] = x;
	m_nVtxBufCurPos++;
	m_fpVtxBuf[m_nVtxBufCurPos] = y;
	m_nVtxBufCurPos++;
}
