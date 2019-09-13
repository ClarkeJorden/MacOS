#include "stdafx.h"
/*#include "ArchView.h"*/
#include "CAD_LayoutView.h"
#include "CAD_Doc.h"
#include "CAD_PaperDoc.h"
#include "CAD_Sheet.h"
#include "CAD_ViewMain.h"
/*#include "SpaceView.h"*/
/*#include "CAD_PerformanceChecker.h"*/

extern RECT g_cRect;
extern RECT g_cMagnifierRect;
extern CCAD_Doc *g_pDoc;

/////////////////////////////////////////////////////////////////////////////
// CCAD_LayoutView

#define		PAPERFRAME_THICK	1

CCAD_LayoutView::CCAD_LayoutView(short nID)
{
	m_nSpaceID = nID;
	m_pPaperDoc = NULL;
}

void CCAD_LayoutView::SetPaperDoc(CCAD_PaperDoc* pDoc)
{
	m_pPaperDoc = pDoc;
	if (pDoc != NULL)
		m_cCadDC.m_fPaperScale = pDoc->m_cSheet.m_fScale;
}

void CCAD_LayoutView::InitUpdateView()
{
	CCAD_Camera *pCamera = m_cCadDC.m_pCamera;
	if(pCamera != NULL)
		pCamera->m_bPerspect = false;
	
	CCAD_Rect cExtentRect;
	g_pDoc->GetDefaultVcsRect(&m_cCadDC, cExtentRect);
	
	RECT rt = g_cRect;
	CCAD_Rect cScrnRect(rt.left, rt.top, rt.right, rt.bottom);
	m_cCadDC.ExtentView(cExtentRect, cScrnRect, SCREEN_MARGIN*2);
}

//----------------------------------------------
void CCAD_LayoutView::DrawFrame()
{
	if ( (m_pPaperDoc->m_cSheet.m_bShowFlag & SHEET_SHOW_FRAME) == 0 )
		return;

	CCAD_Vector	cSp, cEp;
	POINT		sp, ep;

	cSp.m_fX = m_pPaperDoc->m_cSheet.m_cFrame.m_fMinX * m_pPaperDoc->m_cSheet.m_fScale;
	cSp.m_fY = m_pPaperDoc->m_cSheet.m_cFrame.m_fMinY * m_pPaperDoc->m_cSheet.m_fScale;
	cEp.m_fX = m_pPaperDoc->m_cSheet.m_cSize.m_fW - (m_pPaperDoc->m_cSheet.m_cFrame.m_fMaxX * m_pPaperDoc->m_cSheet.m_fScale);
	cEp.m_fY = m_pPaperDoc->m_cSheet.m_cSize.m_fH - (m_pPaperDoc->m_cSheet.m_cFrame.m_fMaxY * m_pPaperDoc->m_cSheet.m_fScale);
	m_cCadDC.WCStoVCS(cSp);
	m_cCadDC.WCStoVCS(cEp);

	BYTE	bFlag = 0;
	if (cSp.m_fX < m_cCadDC.m_cClipRect.m_fMinX)
		cSp.m_fX = m_cCadDC.m_cClipRect.m_fMinX;
	else if (cSp.m_fX < m_cCadDC.m_cClipRect.m_fMaxX)
		bFlag |= 0x01;
	if (cSp.m_fY < m_cCadDC.m_cClipRect.m_fMinY)
		cSp.m_fY = m_cCadDC.m_cClipRect.m_fMinY;
	else if (cSp.m_fY < m_cCadDC.m_cClipRect.m_fMaxY)
		bFlag |= 0x02;
	if (cEp.m_fX > m_cCadDC.m_cClipRect.m_fMaxX)
		cEp.m_fX = m_cCadDC.m_cClipRect.m_fMaxX;
	else if (cEp.m_fX > m_cCadDC.m_cClipRect.m_fMinX)
		bFlag |= 0x04;
	if (cEp.m_fY > m_cCadDC.m_cClipRect.m_fMaxY)
		cEp.m_fY = m_cCadDC.m_cClipRect.m_fMaxY;
	else if (cEp.m_fY > m_cCadDC.m_cClipRect.m_fMinY)
		bFlag |= 0x08;

	CAD_FLOAT	fThick = PAPERFRAME_THICK * m_cCadDC.m_pCamera->m_fScale;
	COLORREF	rgb;
	if (m_pPaperDoc->m_cSheet.m_bBackColor == SHEET_BACK_BLACK)
		rgb = RGB(255, 255, 255);
	else
		rgb = RGB(0, 0, 0);

	LOGPEN sPen;
	sPen.lopnStyle = PS_SOLID;
	sPen.lopnWidth.x = (short)fThick;
	sPen.lopnColor = rgb;

	LOGPEN oldPen = m_cCadDC.SelectPen(sPen);

	sp = m_cCadDC.VCStoLP(cSp);
	ep = m_cCadDC.VCStoLP(cEp);

	m_cCadDC.Lines_Begin(LINETYPE_LINES);
	if( bFlag & 0x01 ){
		m_cCadDC.MoveTo( sp.x, sp.y);
		m_cCadDC.LineTo( sp.x, ep.y);
	}
	if( bFlag & 0x02 ){
		m_cCadDC.MoveTo( sp.x, sp.y);
		m_cCadDC.LineTo( ep.x, sp.y);
	}
	if( bFlag & 0x04 ){
		m_cCadDC.MoveTo( ep.x, sp.y);
		m_cCadDC.LineTo( ep.x, ep.y);
	}
	if( bFlag & 0x08 ){
		m_cCadDC.MoveTo( sp.x, ep.y);
		m_cCadDC.LineTo( ep.x, ep.y);
	}
	m_cCadDC.Lines_End();
	m_cCadDC.RestoreObject(oldPen);
}

void CCAD_LayoutView::DrawBackground(BOOL blExportbmp)
{
	CCAD_Vector	cSp, cEp;
	CCAD_Vector	cSp1, cEp1;
	POINT		sp, ep;
	
	RECT rect = g_cRect;
	RECT rc;
	
	cEp.m_fX = m_pPaperDoc->m_cSheet.m_cSize.m_fW;
	cEp.m_fY = m_pPaperDoc->m_cSheet.m_cSize.m_fH;
	m_cCadDC.WCStoVCS(cSp); cSp1 = cSp; 
	m_cCadDC.WCStoVCS(cEp); cEp1 = cEp;
	
	if (cSp.m_fX < m_cCadDC.m_cClipRect.m_fMinX)
		cSp1.m_fX = m_cCadDC.m_cClipRect.m_fMinX;
	if (cSp.m_fY < m_cCadDC.m_cClipRect.m_fMinY)
		cSp1.m_fY = m_cCadDC.m_cClipRect.m_fMinY;
	if (cEp.m_fX > m_cCadDC.m_cClipRect.m_fMaxX)
		cEp1.m_fX = m_cCadDC.m_cClipRect.m_fMaxX;
	if (cEp.m_fY > m_cCadDC.m_cClipRect.m_fMaxY)
		cEp1.m_fY = m_cCadDC.m_cClipRect.m_fMaxY;
	
	sp = m_cCadDC.VCStoLP(cSp1);
	ep = m_cCadDC.VCStoLP(cEp1);

	if (cSp.m_fX < m_cCadDC.m_cClipRect.m_fMinX)
		sp.x = rect.left;
	if (cSp.m_fY < m_cCadDC.m_cClipRect.m_fMinY)
		sp.y = rect.bottom;
	if (cEp.m_fX > m_cCadDC.m_cClipRect.m_fMaxX)
		ep.x = rect.right;
	if (cEp.m_fY > m_cCadDC.m_cClipRect.m_fMaxY)
		ep.y = rect.top;

	CCAD_DC *pGC = (CCAD_DC *)&m_cCadDC;
	COLORREF color;
	if (m_pPaperDoc->m_cSheet.m_bBackColor == SHEET_BACK_BLACK)
		color = RGB(128,128,128);
	else
		color = RGB(255, 255, 255);
	rc = rect;
	rc.left = sp.x -1; rc.top = ep.y -1; rc.right = ep.x; rc.bottom = sp.y;
	pGC->FillSolidRect(&rc, color);
	
// 	if (blExportbmp == TRUE)
// 		hBrush = ::CreateSolidBrush(RGB(255, 255, 255));
// 	else
/*		color = RGB(187, 187, 187);*/

	if (sp.x > rect.right || sp.y < rect.top ||
		ep.x < rect.left || ep.y > rect.bottom)
	{
		sp.x = rect.left; sp.y = rect.top;
		ep.x = rect.right; ep.y = rect.bottom;
		rc = rect;
		rc.left --; rc.top --;
		pGC->FillSolidRect(&rc, color);
		return;
	}
	if (sp.x > rect.left)
	{
		rc = rect;
		rc.left --; rc.top --;
		rc.right = sp.x;
		pGC->FillSolidRect(&rc, color);
	}
	if (ep.x < rect.right)
	{
		rc = rect;
		rc.left = ep.x-1; rc.top--;
		pGC->FillSolidRect(&rc, color);
	}
	if (sp.y < rect.bottom)
	{
		rc = rect;
		rc.left = sp.x-1; rc.top = sp.y -1;
		rc.right = ep.x;
		pGC->FillSolidRect(&rc, color);
	}
	if (ep.y > rect.top)
	{
		rc = rect;
		rc.left = sp.x-1; rc.top--;
		rc.right = ep.x; rc.bottom = ep.y;
		pGC->FillSolidRect(&rc, color);
	}
	
	DrawFrame();
//----

}

void CCAD_LayoutView::DrawTempBuffer()
{
	BYTE *pBuffer = GetTempBuffer();
	if(!pBuffer)
		return;
	RECT rt = g_cRect;
	glEnable(GL_TEXTURE_2D);

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rt.right - rt.left, rt.bottom - rt.top, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	float texs[] = { 0, 1, 0, 0, 1, 1, 1, 0};

	SCAD_POINT *vtTri = new SCAD_POINT[4];
	vtTri[0].m_fX = rt.left;
	vtTri[0].m_fY = rt.top;
	vtTri[1].m_fX = rt.left;
	vtTri[1].m_fY = rt.bottom;
	vtTri[2].m_fX = rt.right;
	vtTri[2].m_fY = rt.top;
	vtTri[3].m_fX = rt.right;
	vtTri[3].m_fY = rt.bottom;
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texs);
	glVertexPointer(2, GL_FLOAT, sizeof(SCAD_POINT), vtTri);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	delete[] vtTri;
}

void CCAD_LayoutView::DrawEntire(BOOL blRedraw)
{
	if (m_blFirstPaint)
	{
		InitUpdateView();
		m_blFirstPaint = false;
	}

	m_cCadDC.m_fPaperScale = GetPaperDoc()->m_cSheet.m_fScale;

	DrawBackground(true);
 	//m_pPaperDoc->GetUds()->DrawAxisOfUCS(&m_cCadDC);

	short i, nTot = m_pPaperDoc->GetUdsMgr()->TotIndex();
	for (i=0; i<nTot; i++)
	{
		CCAD_Uds * pUds = (CCAD_Uds *)m_pPaperDoc->GetUdsMgr()->GetStyle(i);
/*		pUds->DrawEntityMgr(&m_cCadDC, blRedraw);*/
		CCAD_Point pt1, pt2;
		RECT rt;
		if(GetDrawMagnifierFlag())
		{
			rt = g_cMagnifierRect;
			pt1 = CCAD_Point(rt.left, rt.top, 0);
			pt2 = CCAD_Point(rt.right, rt.bottom, 0);
		}
		else
		{
			rt = g_cRect;
			pt1 = m_cCadDC.LPtoWCS(CCAD_Point(rt.left, rt.top, 0));
			pt2 = m_cCadDC.LPtoWCS(CCAD_Point(rt.right, rt.bottom, 0));
		}
		CCAD_Rect cDrawRect(pt1, pt2);
		pUds->m_pSceneGraph->DrawScene(&m_cCadDC, cDrawRect, blRedraw);
		pUds->DrawEntityMgr(&m_cCadDC, blRedraw);
		memset(pUds->m_pMgrEntity->m_pDrawState, 0, sizeof(BYTE) * pUds->m_pMgrEntity->m_cArray.GetSize());
	}
	//--------------------------------------------------
	m_cCadDC.m_blCancelMessage = FALSE;
}

void CCAD_LayoutView::DrawEntity(CCAD_DC *pCadDC)
{
	ASSERT (m_pPaperDoc);

	for (short i = 0; i < m_pPaperDoc->GetUdsMgr()->TotIndex(); i ++) {
		CCAD_Uds * pUds = (CCAD_Uds *) m_pPaperDoc->GetUdsMgr()->GetStyle(i);
		ASSERT (pUds);

		pUds->DrawEntityMgr(pCadDC);
	}
}

BOOL CCAD_LayoutView::UpdateRenderGL(CCAD_DC *pCadDC)
{
	SetDrawing(true);
	if(IsDrawTempBuffer())
	{
		DrawTempBuffer();
		SetDrawTempBuffer(false);
		SetDrawing(false);
		return TRUE;
	}
	
	DrawEntire();
	if(!IsPanMoving())
	{
		RECT rt = g_cRect;
		BYTE *pPixels = GetTempBuffer();
		glReadPixels(rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, GL_RGBA, GL_UNSIGNED_BYTE, pPixels);
	}

	SetDrawing(false);
	return TRUE;
}

CAD_FLOAT	CCAD_LayoutView::GetLimitScale(CCAD_DC * pDC, CCAD_Camera * pCamera, CAD_FLOAT & xLimit, CAD_FLOAT & yLimit)
{
	CAD_FLOAT limitScale = CAD_ZERO;
	xLimit = yLimit = CAD_ZERO;
	
	CCAD_PaperDoc * pPaperDoc = (CCAD_PaperDoc *)GetSpaceDoc();
	if ( pPaperDoc != NULL ) {
		CCAD_Rect cExtRect;
		pPaperDoc->GetDefaultVcsRect(pDC, cExtRect);
		
		RECT rt = g_cRect;
		xLimit = (rt.right - rt.left - SCREEN_MARGIN * 2) * pCamera->m_fScale / (cExtRect.Width());
		yLimit = (rt.bottom - rt.top - SCREEN_MARGIN * 2) * pCamera->m_fScale / (cExtRect.Height());
		limitScale = ( xLimit > yLimit ) ? yLimit : xLimit;
	}
	return limitScale;
}

CAD_FLOAT CCAD_LayoutView::GetMaxLimit()
{
	if ( m_pPaperDoc == NULL )
		return 5000.0f;
	CAD_FLOAT fMaxScale = 1000.0f / (INCH_PER_MM / DEVICEDPI) / m_pPaperDoc->m_cSheet.m_fScale;	//	MaxScale : 1000
	return fMaxScale;
}

float CCAD_LayoutView::GetScale()
{
	if ( m_pPaperDoc == NULL )
		return 1.0f;
	float fScale = m_cCadDC.m_fScale * (INCH_PER_MM / DEVICEDPI) * m_pPaperDoc->m_cSheet.m_fScale;
	return fScale;
}

CCAD_Size CCAD_LayoutView::GetTotalSize()
{
	CCAD_Size size;
	if (m_pPaperDoc != NULL)
	{
		size.m_fW = (long)(m_pPaperDoc->m_cSheet.m_cSize.m_fW * m_cCadDC.m_fScale);
		size.m_fH = (long)(m_pPaperDoc->m_cSheet.m_cSize.m_fH * m_cCadDC.m_fScale);
	}
	return size;
}

void CCAD_LayoutView::AdjustCameraOrg(CCAD_DC* pDC, CCAD_Camera& cCamera, bool blCalcExt)
{
	CCAD_Rect rt(g_cRect.left, g_cRect.top, g_cRect.right, g_cRect.bottom);
	CCAD_Rect cExtRect;
	GetSpaceDoc()->GetDefaultVcsRect(pDC, cExtRect);
	
	CAD_FLOAT dx1 = 0, dx2 = 0, dy1 = 0, dy2 = 0;
	GetExtRect(dx1,dx2,dy1,dy2, blCalcExt);

	cExtRect.m_fMaxX = dx2 + (cExtRect.m_fMaxX-cExtRect.m_fMinX) - rt.Width()/pDC->m_fScale/2;
	cExtRect.m_fMaxY = dy2 + (cExtRect.m_fMaxY-cExtRect.m_fMinY) - rt.Height()/pDC->m_fScale/2;
	cExtRect.m_fMinX = -dx1 + rt.Width()/pDC->m_fScale/2;
	cExtRect.m_fMinY = -dy1 + rt.Height()/pDC->m_fScale/2;

	if( cCamera.m_cOrg.m_fX > cExtRect.m_fMinX || cCamera.m_cOrg.m_fX < cExtRect.m_fMaxX ) {
		if( cCamera.m_cOrg.m_fX <= cExtRect.m_fMinX )
			cCamera.m_cOrg.m_fX = cExtRect.m_fMinX;
		else if( cCamera.m_cOrg.m_fX >= cExtRect.m_fMaxX )
			cCamera.m_cOrg.m_fX = cExtRect.m_fMaxX;
	}

	if( cCamera.m_cOrg.m_fY > cExtRect.m_fMinY || cCamera.m_cOrg.m_fY < cExtRect.m_fMaxY ) {
		if( cCamera.m_cOrg.m_fY <= cExtRect.m_fMinY )
			cCamera.m_cOrg.m_fY = cExtRect.m_fMinY;
		else if( cCamera.m_cOrg.m_fY >= cExtRect.m_fMaxY )
			cCamera.m_cOrg.m_fY = cExtRect.m_fMaxY;
	}
}

