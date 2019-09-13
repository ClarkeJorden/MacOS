// CAD_GemText.cpp: implementation of the CCAD_GemText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "CAD_Global.h"
#include "CAD_GemText.h"
#include "CAD_GemLine.h"
#include "CAD_TextStyle.h"
#include "CAD_ViewMain.h"

CCAD_GemText::CCAD_GemText()
{
	m_pTriPoints = NULL;
	Init();
}

//----------------------------------------------
CCAD_GemText::~CCAD_GemText()
{
	Remove();
	if(m_pTriPoints)
		delete[] m_pTriPoints;
}

//----------------------------------------------
void CCAD_GemText::Init()
{
	m_nLength = 0;
	m_pStr = NULL;
	m_bPosFlag = POS_LB;
	m_fWidth = 0;
	m_fHeight = 0;
	m_cDirect = (short)0;
	m_cOblique = (short)0;
	m_nStyle = -1;
	m_pTextStyle = NULL;
	m_bPaint = YES;
	m_fEmpty = 0;
	m_bNormalDraw = YES;
	if(m_pTriPoints)
		delete[] m_pTriPoints;
	m_pTriPoints = NULL;
	m_nTriCount = 0;
	RemoveArray();
}

//----------------------------------------------
void CCAD_GemText::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_GemThickEntity::MemCopyTo(pGemEntity, pData1, pData2, pData3);

	((CCAD_GemText *)pGemEntity)->m_bPosFlag = m_bPosFlag;
	((CCAD_GemText *)pGemEntity)->m_nLength = m_nLength;
	((CCAD_GemText *)pGemEntity)->m_cPt = m_cPt;
	((CCAD_GemText *)pGemEntity)->m_fHeight = m_fHeight;
	((CCAD_GemText *)pGemEntity)->m_fWidth = m_fWidth;
	((CCAD_GemText *)pGemEntity)->m_cDirect = m_cDirect;
	((CCAD_GemText *)pGemEntity)->m_cOblique = m_cOblique;
	((CCAD_GemText *)pGemEntity)->m_nStyle = m_nStyle;
	((CCAD_GemText *)pGemEntity)->m_pTextStyle = m_pTextStyle;
	((CCAD_GemText *)pGemEntity)->m_bPaint = m_bPaint;
	((CCAD_GemText *)pGemEntity)->m_fEmpty = m_fEmpty;

	if (m_nLength > 0){
		if (pData1 == NULL)
			pData1 = (LPSTR)new BYTE[m_nLength];
		((CCAD_GemText *)pGemEntity)->m_pStr = (LPSTR)pData1;
		memcpy(((CCAD_GemText *)pGemEntity)->m_pStr, m_pStr, m_nLength);
	}
	
	((CCAD_GemText*)pGemEntity)->m_cOldTextStyle = m_cOldTextStyle;
	((CCAD_GemText *)pGemEntity)->m_bNormalDraw = m_bNormalDraw;
}

//----------------------------------------------
void CCAD_GemText::RemoveArray()
{
	int i, iSize = m_pTextArray.GetSize();
	for (i = 0; i < iSize; i++)
		delete m_pTextArray[i];

	if (iSize != 0)
		m_pTextArray.RemoveAll();
}

//----------------------------------------------
void CCAD_GemText::Remove()
{
	if (m_pStr != NULL)
		delete [] m_pStr;
	m_pStr = NULL;
	m_nLength = 0;

	RemoveArray();
}

//---------------------------------------------
BOOL CCAD_GemText::Read(CCAD_File & cFile)
{
	CCAD_GemThickEntity::Read(cFile);

	cFile.ReadByte(&m_bPosFlag);
	cFile.ReadByte(&m_bPaint);
	cFile.ReadShort(&m_nStyle);
	cFile.ReadCAD_Float(&m_fWidth);
	cFile.ReadCAD_Float(&m_fHeight);
	cFile.ReadCAD_Float(&m_fEmpty);
	m_cDirect.Read(cFile);
	m_cOblique.Read(cFile);
	m_cPt.Read(cFile);
	cFile.ReadShort(&m_nLength);
	if (m_nLength > 0) {
		if (NULL == (m_pStr = new char[m_nLength]))
			return FALSE;
		cFile.ReadString(m_pStr, m_nLength);
	}
	if( m_bPosFlag & 0x80 )
	{
		m_bPosFlag &= 0x0F;
		cFile.ReadByte(&m_bNormalDraw);
		cFile.ReadByte((BYTE*)&m_cOldTextStyle.m_sEffect);
		if ( !cFile.m_blMacVersion ) {
			CADTEXTEFFECT tmp = m_cOldTextStyle.m_sEffect;
			m_cOldTextStyle.m_sEffect.bold = tmp.reserved;
			m_cOldTextStyle.m_sEffect.italic = tmp.vert;
			m_cOldTextStyle.m_sEffect.under = tmp.back;
			m_cOldTextStyle.m_sEffect.middle = tmp.upside;
			m_cOldTextStyle.m_sEffect.upside = tmp.middle;
			m_cOldTextStyle.m_sEffect.back = tmp.under;
			m_cOldTextStyle.m_sEffect.vert = tmp.italic;
			m_cOldTextStyle.m_sEffect.reserved = tmp.bold;
		}
		short nReserve;
		cFile.ReadShort(&nReserve);
	}
	
	if (cFile.m_blMacVersion == TRUE)
	{
		CCAD_Angle cDir = m_cDirect;
		cDir.RotateAngle(-NINETY);
		CAD_FLOAT len = m_fHeight / 6.0f;
		CCAD_Point	pt(cDir.cos() * len, cDir.sin() * len, 0);
		
		m_cBcs.ConvToBase(pt);
		m_cBcs.m_cOrg = pt;
	}
	
	return TRUE;
}

//---------------------------------------------
short CCAD_GemText::TextNum() const
{
	short nNum = 0;
	int	i;
	for (i=0; i<m_nLength; i++){
		if ((BYTE)m_pStr[i] >= 0x80)
			i++;
		nNum++;
	}
	return nNum;
}

//--------------------------------------
void CCAD_GemText::GetLogFont(CCAD_DC *pDC, LOGFONTA/*LOGFONT*/ & sLogFont) const
{
	memset(&sLogFont, 0, sizeof(LOGFONTA/*LOGFONT*/));

	CAD_FLOAT	fScale = m_fWidth / m_fHeight;
	sLogFont.lfHeight = LOGFONT_SIZE;
	sLogFont.lfWidth = (long)(LOGFONT_SIZE * fScale * 0.5f);
	if (sLogFont.lfHeight < 1)
		sLogFont.lfHeight = 1;
	if (sLogFont.lfWidth < 1)
		sLogFont.lfWidth = 1;
	CCAD_Angle cDir = m_cDirect;
//	if (cView.m_bPrint != NO && cView.m_bPrint != PRINT_EQUAL_DIR)
//		vAngleRotate(dAngle, -HALF_RAD);
	if (m_pTextStyle->m_sEffect.vert == YES)
		cDir.RotateAngle(NINETY);

	sLogFont.lfEscapement = (long)(cDir.GetDegree() * 10);
	sLogFont.lfWeight = FW_NORMAL;
	if (m_pTextStyle->m_sEffect.bold == YES)
		sLogFont.lfWeight = FW_BOLD;
	sLogFont.lfItalic = m_pTextStyle->m_sEffect.italic;
	sLogFont.lfUnderline = m_pTextStyle->m_sEffect.under;
	sLogFont.lfStrikeOut = m_pTextStyle->m_sEffect.middle;
	sLogFont.lfCharSet = SHIFTJIS_CHARSET;
	strcpy(sLogFont.lfFaceName, m_pTextStyle->m_szFaceName);
// 	if( (strncmp(sLogFont.lfFaceName, "PRK", 3) == 0) || \
// 		(strncmp(sLogFont.lfFaceName, "PIC", 3) == 0) )
// 	{
// 		strcpy(sLogFont.lfFaceName, FONT_DEFAULTNAME);
// 	}
}

//--------------------------------------
void CCAD_GemText::GetLogFontNormal(CCAD_DC *pDC, LOGFONTA/*LOGFONT*/ & sLogFont) const
{
	memset(&sLogFont, 0, sizeof(LOGFONTA/*LOGFONT*/));

	sLogFont.lfHeight = pDC->ConvScalarWorldToScrn(m_fHeight);
	sLogFont.lfWidth = pDC->ConvScalarWorldToScrn(m_fWidth * 0.5f);
	if (sLogFont.lfHeight < 1)
		sLogFont.lfHeight = 1;
	if (sLogFont.lfWidth < 1)
		sLogFont.lfWidth = 1;
	CCAD_Angle cDir = m_cDirect;
//	if (cView.m_bPrint != NO && cView.m_bPrint != PRINT_EQUAL_DIR)
//		vAngleRotate(dAngle, -HALF_RAD);
	if (m_pTextStyle->m_sEffect.vert == YES)
		cDir.RotateAngle(NINETY);

	sLogFont.lfEscapement = (long)(cDir.GetDegree() * 10);
	sLogFont.lfWeight = FW_NORMAL;
	if (m_pTextStyle->m_sEffect.bold == YES)
		sLogFont.lfWeight = FW_BOLD;
	sLogFont.lfItalic = m_pTextStyle->m_sEffect.italic;
	
	sLogFont.lfUnderline = 0;//m_pTextStyle->m_sEffect.under;
	sLogFont.lfStrikeOut = 0;//m_pTextStyle->m_sEffect.middle;
	
	sLogFont.lfCharSet = SHIFTJIS_CHARSET;
	strcpy(sLogFont.lfFaceName, m_pTextStyle->m_szFaceName);
}

//--------------------------------------
PFONT CCAD_GemText::CreateFont(CCAD_DC *pDC, LOGFONTA/*LOGFONT*/ & sLogFont) const
{
	GetLogFont(pDC, sLogFont);
	return pDC->CreateFontIndirect(sLogFont);
}

//--------------------------------------
PFONT CCAD_GemText::CreateFontNormal(CCAD_DC *pDC, LOGFONTA/*LOGFONT*/ & sLogFont, CCAD_Vector *cRp) const
{
	GetLogFontNormal(pDC, sLogFont);
	if (sLogFont.lfHeight > SHRT_MAX)
		return NULL;
	CCAD_Vector	cTmp[2];

	for (int i=0; i<2; i++ ) {
		cTmp[i] = cRp[i + 1];
		pDC->m_pCamera->ConvFromBase(cTmp[i]);
	}

	CCAD_Angle	cBaseAng;
	cTmp[0].CountAngle2D(cTmp[1], cBaseAng);
	sLogFont.lfEscapement = (long)(cBaseAng.GetDegree() * 10);

	return pDC->CreateFontIndirect(sLogFont);

}

//---------------------------------------------
void CCAD_GemText::GetWidth(CCAD_DC *pDC, PFONT font, CAD_FLOAT & fTotH, CAD_FLOAT & fTotW) const
{
	PFONT		oldFont = pDC->SelectFont( font );
	if (m_pTextStyle->m_sEffect.vert == NO){
		SIZE	sSize;
		pDC->GetTextExtentPoint32(m_pStr, m_nLength, &sSize);
		fTotW = pDC->ConvScalarScrnToWorld((short)sSize.cx);
		fTotH = pDC->ConvScalarScrnToWorld((short)sSize.cy);

		CAD_FLOAT	fScale = m_fWidth / m_fHeight;
		fTotH *= pDC->m_fScale * m_fHeight / LOGFONT_SIZE;
		fTotW *= pDC->m_fScale * m_fWidth / LOGFONT_SIZE / fScale;
	}
	else {
		fTotW = m_fHeight * TextNum();
		fTotH = m_fWidth;
	}
	pDC->SelectFont( oldFont );
}

//---------------------------------------------
void CCAD_GemText::GetWidth(CAD_FLOAT & fTotH, CAD_FLOAT & fTotW) const
{
	CCAD_DC* pScreen = CCAD_Global::GetSpecialDC();

	CCAD_Camera	cCamera;
	pScreen->m_pCamera = &cCamera;

	pScreen->SetCameraInfo(1000, 1000);

	LOGFONTA	sLogFont;
	memset(&sLogFont, 0, sizeof(LOGFONTA));

	PFONT font = CreateFont(pScreen,sLogFont);
	if (font == NULL)
		return;
	GetWidth(pScreen, font, fTotH, fTotW);
}

void CCAD_GemText::GetRangeFromWidth(CAD_FLOAT fHy, CAD_FLOAT fWx, CCAD_Vector * cRp, BOOL bDirectChange) const
{
	CAD_FLOAT	fWy = m_cDirect.sin() * fWx;
	CAD_FLOAT	fHx = m_cDirect.sin() * fHy;
	fWx *= m_cDirect.cos();
	fHy *= m_cDirect.cos();

	CCAD_Vector  Rp = cRp[0];
	
	if (m_pTextStyle->m_sEffect.vert == NO){
		switch (m_bPosFlag) {
		case POS_LT:
							break;
		case POS_CT:		
			if(bDirectChange) {
							cRp[0].m_fX = Rp.m_fY + fWy/2;
							cRp[0].m_fY = Rp.m_fX - fWx/2;
			}
			else {
							cRp[0].m_fX -= fWx/2;
							cRp[0].m_fY -= fWy/2;
			}
							break;
		case POS_RT:
			if(bDirectChange) {
							cRp[0].m_fX = Rp.m_fY + fWy;
							cRp[0].m_fY = Rp.m_fX - fWx;
			}
			else {
							cRp[0].m_fX -= fWx;
							cRp[0].m_fY -= fWy;
			}
							break;
		case POS_LC:		
			if(bDirectChange) {
							cRp[0].m_fX = Rp.m_fY - fHy/2;
							cRp[0].m_fY = Rp.m_fX - fHx/2;
			}
			else {
							cRp[0].m_fX -= fHx/2;
							cRp[0].m_fY += fHy/2;
			}
							break;
		case POS_CENTER:	
			if(bDirectChange) {
							cRp[0].m_fX = Rp.m_fY + (fWy - fHy)/2;
							cRp[0].m_fY = Rp.m_fX - (fWx + fHx)/2;
			}
			else {
							cRp[0].m_fX -= (fWx + fHx)/2;
							cRp[0].m_fY -= (fWy - fHy)/2;
			}
							break;
		case POS_RC:		
			if(bDirectChange) {
							cRp[0].m_fX = Rp.m_fY + (fWy - fHy/2);
							cRp[0].m_fY = Rp.m_fX - (fWx + fHx/2);
			}
			else {
							cRp[0].m_fX -= (fWx + fHx/2);
							cRp[0].m_fY -= (fWy - fHy/2);
			}
							break;
		case POS_LB:		
			if(bDirectChange) {
							cRp[0].m_fX = Rp.m_fY - fHy;
							cRp[0].m_fY = Rp.m_fX - fHx;
			}
			else {
							cRp[0].m_fX -= fHx;
							cRp[0].m_fY += fHy;
			}
							break;
		case POS_CB:		
			if(bDirectChange) {
							cRp[0].m_fX = Rp.m_fY + (fWy/2 - fHy);
							cRp[0].m_fY = Rp.m_fX - (fWx/2 + fHx);
			}
			else {
							cRp[0].m_fX -= (fWx/2 + fHx);
							cRp[0].m_fY -= (fWy/2 - fHy);
			}
							break;
		case POS_RB:		
			if(bDirectChange) {
							cRp[0].m_fX = Rp.m_fY + (fWy - fHy);
							cRp[0].m_fY = Rp.m_fX - (fWx + fHx);
			}
			else {
							cRp[0].m_fX -= (fWx + fHx);
							cRp[0].m_fY -= (fWy - fHy);
			}
							break;
		}
	}
	else {
		switch (m_bPosFlag) {
		case POS_LT:		cRp[0].m_fX -= fHx;
							cRp[0].m_fY += fHy;
							break;
		case POS_CT:		cRp[0].m_fX -= fHx/2;
							cRp[0].m_fY += fHy/2;
							break;
		case POS_RT:		break;
		case POS_LC:		cRp[0].m_fX -= fHx+fWx/2;
							cRp[0].m_fY += fHy-fWy/2;
							break;
		case POS_CENTER:	cRp[0].m_fX -= (fHx+fWx)/2;
							cRp[0].m_fY += (fHy-fWy)/2;
							break;
		case POS_RC:		cRp[0].m_fX -= fWx/2;
							cRp[0].m_fY -= fWy/2;
							break;
		case POS_LB:		cRp[0].m_fX -= fHx+fWx;
							cRp[0].m_fY += fHy-fWy;
							break;
		case POS_CB:		cRp[0].m_fX -= fHx/2+fWx;
							cRp[0].m_fY += fHy/2-fWy;
							break;
		case POS_RB:		cRp[0].m_fX -= fWx;
							cRp[0].m_fY -= fWy;
							break;
		}
	}
	cRp[1].m_fX = cRp[0].m_fX + fHx;
	cRp[1].m_fY = cRp[0].m_fY - fHy;
	cRp[2].m_fX = cRp[0].m_fX + fWx + fHx;
	cRp[2].m_fY = cRp[0].m_fY + fWy - fHy;
	cRp[3].m_fX = cRp[0].m_fX + fWx;
	cRp[3].m_fY = cRp[0].m_fY + fWy;
}

//---------------------------------------------
void CCAD_GemText::GetRange(CCAD_Vector * cRp, BOOL bDirectChange/*=FALSE*/) const
{
	CAD_FLOAT	fW, fH;
	fW = fH = 0.0f;
	GetWidth(fH, fW);
	GetRangeFromWidth(fH, fW, cRp, bDirectChange);
}

// //---------------------------------------------
// BOOL CCAD_GemText::SetParaFromRange(CCAD_Vector & cPt1, CCAD_Vector & cPt2, CCAD_Vector & cPt3)
// {
// 	CCAD_GemLine	cLine(cPt1, cPt2);
// 	cLine.m_cBcs = m_cBcs;
// 	CAD_FLOAT		fH = cLine.CountDistanceToPoint(cPt3);
// 	if (fH == 0)
// 		return FALSE;
// 
// 	CCAD_Angle	cDirect;
// 	cPt1.CountAngle2D(cPt2, cDirect);
// 	if (m_pTextStyle->m_sEffect.vert == YES) {
// 		m_cDirect = cDirect;
// 		m_fWidth = fH;
// 		m_fHeight = cPt1.CountLength(cPt2) / TextNum();
// 	}
// 	else {
// 		m_cDirect = 0;
// 		m_fHeight = 200;
// 		m_fWidth = 200;
// 
// 		CAD_FLOAT	fWidth, fHeight;
// 		GetWidth(fHeight, fWidth);
// 		m_cDirect = cDirect;
// 		m_fWidth = cPt1.CountLength(cPt2) * m_fHeight / fWidth;
// 		m_fHeight = fH;
// 	}
// 
// 	CCAD_Vector	cVp1, cVp2;
// 	if (cLine.PtDirect(cPt3) == NOCLOCK_DIRECT) {
// 		cDirect.RotateAngle(NINETY);
// 		cVp1 = cPt1;
// 		cVp2 = cPt2;
// 		cVp1.CountVector2D(cPt1, fH, cDirect.sin(), cDirect.cos());
// 		cVp2.CountVector2D(cPt2, fH, cDirect.sin(), cDirect.cos());
// 	}
// 	else {
// 		cDirect.RotateAngle(-NINETY);
// 		cPt1.CountVector2D(cVp1, fH, cDirect.sin(), cDirect.cos());
// 		cPt2.CountVector2D(cVp2, fH, cDirect.sin(), cDirect.cos());
// 	}
// 	switch (m_bPosFlag) {
// 	case POS_LT:		m_cPt = cPt1;
// 						break;
// 	case POS_CT:		m_cPt.m_fX = (cPt1.m_fX + cPt2.m_fX) / 2;
// 						m_cPt.m_fY = (cPt1.m_fY + cPt2.m_fY) / 2;
// 						m_cPt.m_fZ = (cPt1.m_fZ + cPt2.m_fZ) / 2;
// 						break;
// 	case POS_RT:		m_cPt = cPt2;
// 						break;
// 	case POS_LC:		m_cPt.m_fX = (cPt1.m_fX + cVp1.m_fX) / 2;
// 						m_cPt.m_fY = (cPt1.m_fY + cVp1.m_fY) / 2;
// 						m_cPt.m_fZ = (cPt1.m_fZ + cVp1.m_fZ) / 2;
// 						break;
// 	case POS_CENTER:	m_cPt.m_fX = (cPt1.m_fX + cVp2.m_fX) / 2;
// 						m_cPt.m_fY = (cPt1.m_fY + cVp2.m_fY) / 2;
// 						m_cPt.m_fZ = (cPt1.m_fZ + cVp2.m_fZ) / 2;
// 						break;
// 	case POS_RC:		m_cPt.m_fX = (cPt2.m_fX + cVp2.m_fX) / 2;
// 						m_cPt.m_fY = (cPt2.m_fY + cVp2.m_fY) / 2;
// 						m_cPt.m_fZ = (cPt2.m_fZ + cVp2.m_fZ) / 2;
// 						break;
// 	case POS_LB:		m_cPt = cVp1;
// 						break;
// 	case POS_CB:		m_cPt.m_fX = (cVp1.m_fX + cVp2.m_fX) / 2;
// 						m_cPt.m_fY = (cVp1.m_fY + cVp2.m_fY) / 2;
// 						m_cPt.m_fZ = (cVp1.m_fZ + cVp2.m_fZ) / 2;
// 						break;
// 	case POS_RB:		m_cPt = cVp2;
// 						break;
// 	}
// 	return TRUE;
// }

//----------------------------------------------------------
BOOL CCAD_GemText::Drawing(CCAD_DC* pCadDC, BYTE bState, BOOL blRedraw)
{
	if (m_nLength == 0 || m_pStr == NULL || m_pTextStyle == NULL)
		return FALSE;

	int nSizeArray = m_pTextArray.GetSize();
	if (nSizeArray <= 0)
		MakeSingleText(pCadDC);

	ASSERT(pCadDC != NULL);
	//	set body coordinate.
	CCAD_Coord3D cECS = pCadDC->PushCamera(&m_cBcs);
	//	set thick flag.
	CAD_FLOAT fOldThick = pCadDC->SelectThick(GetThick());
	//	set closed flag.
	BOOL blOldClosed = pCadDC->SelectClose(IsClosed());
	//-------------------------------------
	BOOL blResult;
	blResult = Draw(pCadDC, blRedraw, false);
//	set old flags.
	pCadDC->SelectThick(fOldThick);
	pCadDC->SelectClose(blOldClosed);
	//-------------------------------------
	pCadDC->RestorCamera(cECS);
	return blResult;
}

BOOL CCAD_GemText::NormalDraw(CCAD_DC *pDC)
{
	if (m_pTextStyle->m_sEffect.back == 1 || m_pTextStyle->m_sEffect.upside == 1 || m_fEmpty != 0 || m_cOblique.GetAbsDegree() >= CCAD_Global::GetAngleAccy()
		/* || m_pTextStyle->m_sEffect.vert==1 */)
		return FALSE;
//	CAD_FLOAT fScalar = pDC->m_pCamera->m_cZ & CCAD_Vector(0,0,1); // scalar product
//	if( IS_ZERO(fabs(fScalar) - 1.0 ) && !pDC->m_pCamera->m_bPerspect )
	if (!m_cBcs.IsParallelPlan(*(CCAD_Coord3D *)(pDC->m_pCamera)) || pDC->m_pCamera->m_bPerspect)
		return FALSE;
	if ( m_bPaint == NO ) return FALSE;
	if (m_bNormalDraw == NO) return FALSE;

	CCAD_Vector	cRp[4];
	cRp[0] = m_cPt;
	GetRange(cRp);
	memcpy(m_cRp, cRp, sizeof(CCAD_Vector) * 4);

	LOGFONTA/*LOGFONT*/	sLogFont;
	memset(&sLogFont, 0, sizeof(LOGFONTA/*LOGFONT*/));
	PFONT	font = CreateFontNormal(pDC, sLogFont, m_cRp);
	if (font == NULL)
		return TRUE;
	if(sLogFont.lfHeight >= 4 && !IsPanMoving())
	{
		PFONT	oldFont = pDC->SelectFont( font );
		pDC->m_bTxtPaint = m_bPaint;
		if (m_pTextStyle->m_sEffect.vert == NO) {
			if(pDC->IsVerticality() ){
				pDC->TextOut(m_cRp[0].m_fX, m_cRp[0].m_fY, m_pStr, (int)m_nLength);
			}else{
				pDC->TextOut(m_cRp, m_pStr, (int)m_nLength, TRUE);
			}
		}
		else {
			CCAD_Vector	cRp[4];
			memcpy(cRp, m_cRp, sizeof(CCAD_Vector) * 4);
			CCAD_Angle	cDir;
			cRp[1].CountAngle2D(cRp[2], cDir);
			for (short i=0; i<m_nLength; i++){
				if ( (BYTE)m_pStr[i] >= 0x80){
					if(pDC->IsVerticality()){
						pDC->TextOut(cRp[1].m_fX, cRp[1].m_fY, m_pStr + i, 2);
					}else{
						pDC->TextOut(cRp, m_pStr + i, 2);
					}
					i++;
				}
				else{
					CCAD_Vector	cTp(cRp[1]);
					cTp.m_fX += -cDir.sin() * m_fWidth/4;
					cTp.m_fY += cDir.cos() * m_fWidth/4;
					if(pDC->IsVerticality()){
						pDC->TextOut(cTp.m_fX, cTp.m_fY, m_pStr + i, 1);
					}else{
						pDC->TextOut(m_cRp, m_pStr + i, 1);
					}
				}
				cRp[1].m_fX += cDir.cos() * m_fHeight;
				cRp[1].m_fY += cDir.sin() * m_fHeight;
			}
		}
		pDC->m_bTxtPaint = NO;
		pDC->SelectFont( oldFont );
	}
	else {
        short    nNum = 4;
#if 0 //solid
		LOGPEN oldPen = pDC->SelectNullPen();
		if(m_nTriCount == 0)
			m_pTriPoints = pDC->GetTrianglesFromPolygon(m_cRp, &nNum, 1, m_nTriCount);
		pDC->PolyPolygon(m_cRp, &nNum, 1, m_pTriPoints, m_nTriCount);
		pDC->RestoreObject(oldPen);
#else
        pDC->Lines_Begin(LINETYPE_STRIP);
        pDC->MoveTo(*m_cRp);
        for (int i=1; i<nNum; i++)
            pDC->LineTo(*(m_cRp+i));
        pDC->LineTo(*m_cRp);
        pDC->Lines_End();
#endif
	}
    delete font;
/*
	LOGPEN oldPen = pDC->SelectNullPen();
	//--------------------------------------------
	if(pDC->IsVerticality() && !pDC->GetCamera()->m_bPerspect){
		int i;

		pDC->Lines_Begin(LINETYPE_LINES);
		pDC->MoveTo(m_cRp[0]);
		for (i=1; i<4; i++)
			pDC->LineTo(m_cRp[i]);
		pDC->LineTo(m_cRp[0]);
		CAD_FLOAT	fDis = pDC->ConvScalarScrnToWorld(4);
		short	nNum = (short)(m_cRp[0].CountLength(m_cRp[1]) / fDis);
		CCAD_Angle 	cAng;
		m_cRp[1].CountAngle2D(m_cRp[2], cAng);
		CAD_FLOAT	fDx = -fDis * cAng.sin();
		CAD_FLOAT	fDy = fDis * cAng.cos();
		CCAD_Vector 	cSp = m_cRp[1];
		CCAD_Vector	cEp = m_cRp[2];
		for (i=0; i<nNum; i++){
			cSp.m_fX += fDx;
			cSp.m_fY += fDy;
			cEp.m_fX += fDx;
			cEp.m_fY += fDy;
			pDC->MoveTo(cSp);
			pDC->LineTo(cEp);
		}
		pDC->Lines_End();
	}
	//--------------------------------------------
	pDC->RestoreObject(oldPen);
*/	return TRUE;
}

// void CCAD_GemText::GetDspFileData(CCAD_DC *pCadDC)
// {
// 	if (m_nLength == 0 || m_pStr == NULL || pCadDC == NULL)
// 		return;
// 
// 	CCAD_Coord3D cECS = pCadDC->PushCamera(&m_cBcs);
// 	CAD_FLOAT fOldThick = pCadDC->SelectThick(GetThick());
// 	BOOL blOldClosed = pCadDC->SelectClose(IsClosed());
// 	CAD_FLOAT fScalar = pCadDC->m_pCamera->m_cZ & CCAD_Vector(0,0,1); // scalar product
// 
// 	if( IS_ZERO(fabs(fScalar) - 1.0 ) && !pCadDC->m_pCamera->m_bPerspect && (m_pTextStyle->m_sEffect.upside != 1)
// 					&& (m_pTextStyle->m_sEffect.back != 1 ) && IS_ZERO(m_cOblique.GetAbsDegree())){
// 		CCAD_Vector	cRp[4];
// 		cRp[0] = m_cPt;
// 		GetRange(cRp);
// 		DrawDspData(pCadDC, cRp);
// 	}
// 	else {
// 		if (m_pTextArray.GetSize() <= 0 )
// 			MakeSingleText();
// 		Draw(pCadDC);
// 	}
// 	pCadDC->SelectThick(fOldThick);
// 	pCadDC->SelectClose(blOldClosed);
// 	pCadDC->RestorCamera(cECS);
// }

BOOL CCAD_GemText::Draw(CCAD_DC *pDC, BOOL blRedraw, bool bSelected)
{
	if (m_nLength == 0 || m_pStr == NULL || m_pTextStyle == NULL || m_nStyle < 0)
		return FALSE;

//	if(IsPanMoving())
//		return FALSE;

	CCAD_Angle delta; delta=90;
	CCAD_Angle cBakAngle = m_cDirect;

	if ( !NormalDraw(pDC) )
	{
		long nSizeArray = m_pTextArray.GetCount();
		if (nSizeArray == 0)
			return FALSE;

		long i;
		for(i=0; i<nSizeArray; i++)
			m_pTextArray[i]->Draw(pDC, m_cThick, m_bPaint);
	}

	if ( m_pTextStyle->m_sEffect.under == 1 || m_pTextStyle->m_sEffect.middle == 1)
	if (bSelected == false && IS_ZERO(GetThick()) && !pDC->m_pCamera->m_bPerspect) {
		LOGPEN oldPen = pDC->GetLogPen();
		long nWidth;
		if (m_bPaint == CAD_YES)
			nWidth = pDC->ConvScalarWorldToScrn(m_fHeight/20);
		else
			nWidth = 1;

		LOGPEN newPen = oldPen;
		newPen.lopnWidth.x = newPen.lopnWidth.y = nWidth;
		pDC->SelectPen(newPen);
		CCAD_Vector	cRp[4], cSp, cEp;
		cRp[0] = m_cPt;
		GetRange(cRp);
		pDC->Lines_Begin(LINETYPE_LINES);
		if (m_pTextStyle->m_sEffect.under == 1){
			cSp.m_fX = cRp[1].m_fX;
			cSp.m_fY = cRp[1].m_fY;
			cEp.m_fX = cRp[2].m_fX;
			cEp.m_fY = cRp[2].m_fY;
			pDC->MoveTo(cSp);
			pDC->LineTo(cEp);
		}
		if (m_pTextStyle->m_sEffect.middle == 1){
			cSp.m_fX = (cRp[0].m_fX + cRp[1].m_fX)/2;
			cSp.m_fY = (cRp[0].m_fY + cRp[1].m_fY)/2;
			cEp.m_fX = (cRp[2].m_fX + cRp[3].m_fX)/2;
			cEp.m_fY = (cRp[2].m_fY + cRp[3].m_fY)/2;
			pDC->MoveTo(cSp);
			pDC->LineTo(cEp);
		}
		pDC->Lines_End();
		pDC->SelectPen(oldPen);
	}

	return TRUE;
}

// void CCAD_GemText::DrawDspData(CCAD_DC *pDC, CCAD_Vector * cRp)
// {
// 	short i;
// 	pDC->MoveTo(cRp[0]);
// 	for (i=1; i<4; i++)
// 		pDC->LineTo(cRp[i]);
// 	pDC->LineTo(cRp[0]);
// 
// 	CAD_FLOAT	fDis = pDC->ConvScalarScrnToWorld(4);
// 	short	nNum = (short)(cRp[0].CountLength(cRp[1]) / fDis);
// 	CCAD_Angle 	cAng;
// 	cRp[1].CountAngle2D(cRp[2], cAng);
// 	CAD_FLOAT	fDx = -fDis * cAng.sin();
// 	CAD_FLOAT	fDy = fDis * cAng.cos();
// 	CCAD_Vector 	cSp = cRp[1];
// 	CCAD_Vector	cEp = cRp[2];
// 	
// 	for (i=0; i<nNum; i++)
// 	{
// 		cSp.m_fX += fDx;
// 		cSp.m_fY += fDy;
// 		cEp.m_fX += fDx;
// 		cEp.m_fY += fDy;
// 		pDC->MoveTo(cSp);
// 		pDC->LineTo(cEp);
// 	}
// }
// 

void CCAD_GemText::CountBodyRange(CCAD_Rect& cRange)
{
	if (m_nLength == 0 || m_pStr == NULL || m_pTextStyle == NULL)
		return;

		CCAD_Vector cRp[4];
		cRp[0] = m_cPt;
		GetRange(cRp);
		memcpy(m_cRp, cRp, sizeof(CCAD_Vector) * 4);
		cRange.ResetParameter(cRp[0]);
		cRange.ResetParameter(cRp[1]);
		cRange.ResetParameter(cRp[2]);
		cRange.ResetParameter(cRp[3]);
}

BOOL CCAD_GemText::IsNormalDraw(CCAD_DC *pDC)
{
	if (!m_cBcs.IsParallelPlan(*(CCAD_Coord3D *)(pDC->m_pCamera)) || pDC->m_pCamera->m_bPerspect)
		return FALSE;
	if ( m_bPaint == NO ) return FALSE;
	if (m_bNormalDraw == NO) return FALSE;
	return TRUE;
}

void CCAD_GemText::CountVcsRange(CCAD_DC *pDC, CCAD_Rect& cRange)
{
	if (m_nLength == 0 || m_pStr == NULL || m_pTextStyle == NULL)
		return;
	int nSizeArray = m_pTextArray.GetSize();
	if (nSizeArray == 0 || IsNormalDraw(pDC)) {
		CCAD_Vector cRp[4];
		cRp[0] = m_cPt;
		GetRange(cRp);
		pDC->CountVcsRange(cRp[0], cRange);
		pDC->CountVcsRange(cRp[1], cRange);
		pDC->CountVcsRange(cRp[2], cRange);
		pDC->CountVcsRange(cRp[3], cRange);
	}
	else {
		for(int i=0; i<nSizeArray; i++)
			m_pTextArray[i]->CountVcsRange(pDC, cRange);
	}
}


void CCAD_GemText::MakeSingleText(CCAD_DC* pCadDC)
{
	if (m_nLength == 0 || m_pStr == NULL || m_pTextStyle == NULL)
		return;
	
	CCAD_Vector	cRp[4];
	cRp[0] = m_cPt;
	CAD_FLOAT	fW, fH;
	fW = fH = 0.0f;
	GetWidth(fH, fW);
	GetRangeFromWidth(fH, fW, cRp, FALSE);

	LOGFONTA/*LOGFONT*/	sLogFont;
	memset(&sLogFont, 0, sizeof(LOGFONTA/*LOGFONT*/));
	PFONT	font = CreateFontNormal(pCadDC, sLogFont, m_cRp);
	if (font == NULL) return;

	CCAD_Point cCenter;
	
//	if (m_pTextStyle->m_sEffect.upside == 1){
	if (m_pTextStyle->m_sEffect.upside != m_cOldTextStyle.m_sEffect.upside){
		CCAD_Coord3D cCoord;
		cCenter.m_fX = (cRp[0].m_fX + cRp[2].m_fX) / 2.0F;
		cCenter.m_fY = (cRp[0].m_fY + cRp[2].m_fY) / 2.0F;
		cCenter.m_fZ = (cRp[0].m_fZ + cRp[2].m_fZ) / 2.0F;
		cCoord.m_cOrg = cCenter;
		CCAD_Angle	cRot = m_cDirect;
		if (m_pTextStyle->m_sEffect.vert != CAD_YES)
			cRot.RotateAngle(-NINETY);
		cCoord.ZRotate(cRot);
		m_cBcs.ChildToThis(&cCoord);
		CCAD_GemEntity::Mirror (cCoord);
	}
//	if (m_pTextStyle->m_sEffect.back == 1) {
	if (m_pTextStyle->m_sEffect.back != m_cOldTextStyle.m_sEffect.back) {
		CCAD_Coord3D cCoord;
		cCenter.m_fX = (cRp[0].m_fX + cRp[2].m_fX) / 2.0F;
		cCenter.m_fY = (cRp[0].m_fY + cRp[2].m_fY) / 2.0F;
		cCenter.m_fZ = (cRp[0].m_fZ + cRp[2].m_fZ) / 2.0F;

		cCoord.m_cOrg = cCenter;
		CCAD_Angle	cRot = m_cDirect;
		if (m_pTextStyle->m_sEffect.vert == CAD_YES)
			cRot.RotateAngle(NINETY);
		cCoord.ZRotate(cRot);
		m_cBcs.ChildToThis(&cCoord);
		CCAD_GemEntity::Mirror (cCoord);
	}
	
	m_cOldTextStyle = *m_pTextStyle;

	if (sLogFont.lfHeight >= 1 && sLogFont.lfWidth >= 1){
		WORD	wChar;
		CCAD_Point	cIns = cRp[1];
		if (m_pTextStyle->m_sEffect.vert == CAD_YES){
			cRp[1].CountAngle2D(cRp[2], m_cVertDir);
			cIns.m_fX += m_cVertDir.cos() * m_fHeight;
			cIns.m_fY += m_cVertDir.sin() * m_fHeight;
		}

		int	i;
		CAD_FLOAT	fSkew;
		if (fmod((double)m_cOblique.GetAbsDegree(), (double)90) == 0){
			CCAD_Angle	cAngle = (CAD_FLOAT)0;
			fSkew = cAngle.tan();
		}
		else
			fSkew = m_cOblique.tan();

		RemoveArray();

		char	 lpStr[EXT_LENGTH_NAME];
		memset(lpStr, 0, EXT_LENGTH_NAME);
		strcpy(lpStr, m_pStr);
		ConvertStringLocalToUni(lpStr, m_nLength);

		float xpos= 0.0f;

		for (i=0; i<m_nLength; i++) {
			CCAD_Vector cTp(cIns);
			
			int charByteNum = 1;
			if( (unsigned char)m_pStr[i] < (unsigned char)0x80)
			{
				wChar = (WORD)lpStr[i];
			}
			else
			{
				wChar = (lpStr[i] << 8) | (lpStr[i + 1] & 0xFF);
				i++;
				charByteNum = 2;
			}

			CCAD_Vector charrect[4];
			float ratio = xpos / fW;
			charrect[0].m_fX = cRp[0].m_fX + (cRp[3].m_fX - cRp[0].m_fX) * ratio;
			charrect[0].m_fY = cRp[0].m_fY + (cRp[3].m_fY - cRp[0].m_fY) * ratio;
			charrect[0].m_fZ = 0.0f;
			charrect[1].m_fX = cRp[1].m_fX + (cRp[2].m_fX - cRp[1].m_fX) * ratio;
			charrect[1].m_fY = cRp[1].m_fY + (cRp[2].m_fY - cRp[1].m_fY) * ratio;
			charrect[1].m_fZ = 0.0f;

			LPSTR tStr = m_pStr;
			long  tLen = m_nLength;
			m_pStr = &m_pStr[i];
			m_nLength = charByteNum;
			CAD_FLOAT curCharWid, curCharHgt;
			GetWidth(curCharHgt, curCharWid);
			m_pStr = tStr;
			m_nLength = tLen;
			xpos += curCharWid;

			ratio = xpos / fW;
			charrect[3].m_fX = cRp[0].m_fX + (cRp[3].m_fX - cRp[0].m_fX) * ratio;
			charrect[3].m_fY = cRp[0].m_fY + (cRp[3].m_fY - cRp[0].m_fY) * ratio;
			charrect[3].m_fZ = 0.0f;
			charrect[2].m_fX = cRp[1].m_fX + (cRp[2].m_fX - cRp[1].m_fX) * ratio;
			charrect[2].m_fY = cRp[1].m_fY + (cRp[2].m_fY - cRp[1].m_fY) * ratio;
			charrect[2].m_fZ = 0.0f;

			float w_hscale = m_fWidth / m_fHeight;
			if(w_hscale > 1.0f) w_hscale = 1.0f / w_hscale;

			CCAD_GemSingleText* cpSingleText = new CCAD_GemSingleText(m_cBcs, charrect, w_hscale, fSkew, wChar, font);
			//if (MakeEachSingleText(&cCadDC, cIns, wChar, *cpSingleText))
				m_pTextArray.Add(cpSingleText);
			/*
			else {
				delete cpSingleText;
				cpSingleText = NULL;
			}
			*/

			CAD_FLOAT	w = pCadDC->ConvScalarScrnToWorld((short)sLogFont.lfWidth);
			CAD_FLOAT	h = pCadDC->ConvScalarScrnToWorld((short)sLogFont.lfWidth);
			if (m_pTextStyle->m_sEffect.vert == CAD_NO){
				CAD_FLOAT	fScale = m_fWidth / m_fHeight;
				w *= m_fWidth / LOGFONT_SIZE / fScale;
				cIns.m_fX += (w+m_fEmpty) * m_cDirect.cos();
				cIns.m_fY += (w+m_fEmpty) * m_cDirect.sin();
			}
			else {
				h *= m_fHeight / LOGFONT_SIZE;
				cIns.m_fX += m_cVertDir.cos() * (h+m_fEmpty);
//				cIns.m_fY += m_cVertDir.sin() * w;
				cIns.m_fY += m_cVertDir.sin() * (h+m_fEmpty);
			}
		}
	}
}
