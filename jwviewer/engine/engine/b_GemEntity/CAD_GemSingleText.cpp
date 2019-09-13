#include "stdafx.h"
#include "CAD_GemSingleText.h"
#include "CAD_RgnDelauProc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCAD_GemSingleText::CCAD_GemSingleText()
{
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fSkew = 0;

	m_fRateX = 1.0f;
	m_fRateY = 1.0f;

	if(m_cContourArray.GetSize() != 0) {
		m_cContourArray.RemoveAll();
	}
	m_pOffsets = NULL;
	m_wChar = 0;
	m_spFont = NULL;
	m_fW_HScale = 1.0f;
}

CCAD_GemSingleText::CCAD_GemSingleText(	const	CCAD_Coord3D	&cBCS,
										const	CCAD_Vector		*rect,
										const	CAD_FLOAT		fW_HRatio,
										const	CAD_FLOAT		fSkew,
										WORD	wChar,
										PFONT	font)
{
	m_cBcs = cBCS;

	m_fSkew = fSkew;

	CAD_FLOAT	fScale = m_fWidth / m_fHeight;
//	m_fRateY = fHeight / LOGFONT_SIZE;
//	m_fRateX = fWidth / LOGFONT_SIZE / fScale;

//	m_cPos = cPos;

	if(m_cContourArray.GetSize() != 0) {
		m_cContourArray.RemoveAll();
	}
	m_pOffsets = NULL;
	m_wChar = wChar;
	m_spFont = font;
	m_fW_HScale = fW_HRatio;
	memcpy(&m_cBndRect[0], rect, sizeof(CCAD_Vector) * 4);
}

CCAD_GemSingleText::CCAD_GemSingleText(	const CCAD_GemSingleText &cOther)
{
	m_cBcs = cOther.m_cBcs;

	m_fWidth = cOther.m_fWidth;
	m_fHeight = cOther.m_fHeight;
	m_fSkew = cOther.m_fSkew;

	m_fRateX = cOther.m_fRateX;
	m_fRateY = cOther.m_fRateY;

	m_cPos = cOther.m_cPos;

	if(m_cContourArray.GetSize() != 0) {
		m_cContourArray.RemoveAll();
	}

	int nSizeArray = cOther.m_cContourArray.GetSize();
	int	i;
	for(i = 0; i < nSizeArray; i ++) {
		m_cContourArray.Add(cOther.m_cContourArray[i]);
	}
	m_pOffsets = NULL;
	m_wChar = cOther.m_wChar;
	m_spFont = cOther.m_spFont;
}

CCAD_GemSingleText
&CCAD_GemSingleText::operator= (	const CCAD_GemSingleText &cOther)
{
	m_cBcs = cOther.m_cBcs;

	m_fWidth = cOther.m_fWidth;
	m_fHeight = cOther.m_fHeight;

	m_fRateX = cOther.m_fRateX;
	m_fRateY = cOther.m_fRateY;

	m_cPos = cOther.m_cPos;

	if(m_cContourArray.GetSize() != 0) {
		m_cContourArray.RemoveAll();
	}

	int nSizeArray = cOther.m_cContourArray.GetSize();
	int	i;
	for(i = 0; i < nSizeArray; i ++) {
		m_cContourArray.Add(cOther.m_cContourArray[i]);
	}
	m_pOffsets = NULL;
	return *this;
}

CCAD_GemSingleText::~CCAD_GemSingleText()
{
	if(m_cContourArray.GetSize() != 0) {
		m_cContourArray.RemoveAll();
	}
	
	if (m_pOffsets != NULL)
	{
		m_pOffsets->RemoveAll();
		delete m_pOffsets;
		m_pOffsets = NULL;
	}
}

#ifdef  _WIN32
void
CCAD_GemSingleText::ConvGlyphToPoly(		CCAD_DC		*pDC,
											BYTE		*bpFontData,
									const	BYTE		*bpGlyphEnd)
{
	while (bpFontData < bpGlyphEnd) {
		CCAD_GemTContour cContour;

		BYTE	*bpContourStart = NULL;
 		bpContourStart = bpFontData;
		
		TTPOLYGONHEADER	sTTPolygonHeader;
		bpFontData = GetTTPOLYGONHEADER(sTTPolygonHeader, bpFontData);
		BYTE	*bpContourEnd = NULL;
		bpContourEnd = bpContourStart + sTTPolygonHeader.cb;
		
		CCAD_Point	cPtWk[3];

		PntFXtoVector(sTTPolygonHeader.pfxStart, cPtWk[0]);
		
		BOOL		blPreLine = TRUE;
		CCAD_Point	cPreLinePt, cMidPt;

		cPreLinePt = cPtWk[0];
		cContour.AddPoint(cPtWk[0]);
		
		while (bpFontData < bpContourEnd){

			TTPOLYCURVE		sTTPolyCurve;
			POINTFX			sPntFX[MAX_CURVE_POINT];

			bpFontData = GetTTPOLYCURVE(sTTPolyCurve, sPntFX, bpFontData);
			if (sTTPolyCurve.wType == TT_PRIM_LINE){
				WORD	wIndex;
				for (wIndex = 0; wIndex < sTTPolyCurve.cpfx; wIndex++){
					PntFXtoVector(sPntFX[wIndex], cPtWk[0]);
					
					if (blPreLine == TRUE)
					{
						AddPoint(cContour, cPreLinePt, cPtWk[0]);
					}
					
					cContour.AddPoint(cPtWk[0]);
					
					blPreLine = TRUE;
					cPreLinePt = cPtWk[0];
				}
			}
			else { // Case of   TT_PRIM_QSPLINE.
				SCAD_QSpline	sQSpline;
				SetQSpline(sQSpline, QSPLINE_DIV_NUM);

				PntFXtoVector(sPntFX[0], cPtWk[1]);
				
				CCAD_Point	cPtWkEnd;
				WORD	wIndex;
				for (wIndex = 1; wIndex < sTTPolyCurve.cpfx; wIndex++) {
					PntFXtoVector(sPntFX[wIndex], cPtWk[2]);
					
					if (wIndex < sTTPolyCurve.cpfx-1){
						cPtWkEnd = cPtWk[2];
						cPtWk[2].m_fX = (cPtWk[2].m_fX + cPtWk[1].m_fX) / 2.0F;
						cPtWk[2].m_fY = (cPtWk[2].m_fY + cPtWk[1].m_fY) / 2.0F;
						cPtWk[2].m_fZ = cPtWk[2].m_fZ;
					}
					QSpline(cContour, cPtWk, &sQSpline);

					cPtWk[1] = cPtWkEnd;
					cPtWk[0] = cPtWk[2];
				}
				blPreLine = TRUE;
				cPreLinePt = cContour.m_cPointArray.GetAt(cContour.m_cPointArray.GetSize() - 1);
			}
		}
		///////////////////////////////////////////////////////
		
		{
			cPreLinePt = cContour.m_cPointArray.GetAt(cContour.m_cPointArray.GetSize() - 1);
			cPtWk[0] = cContour.m_cPointArray.GetAt(0);
			AddPoint(cContour, cPreLinePt, cPtWk[0]);
		}
		
		///////////////////////////////////////////////////////
		m_cContourArray.Add(cContour);
	}
	SetInOutContour();
	
	int nSizeContour = m_cContourArray.GetSize();
	int	i;
	for(i = 0; i < nSizeContour; i ++) {
//		BOOL blRot = blJudgRotDir(m_cContourArray[i]);
		BOOL blRot = m_cContourArray[i].blJudgRotDir();
		if(	(blRot == TRUE && m_cContourArray[i].m_blParent == TRUE) ||
			(blRot == FALSE && m_cContourArray[i].m_blParent == FALSE) ) {
			m_cContourArray[i].InvertContourDir();
		}
	}

	ConvGlyphToBody(pDC);
}
#else
/*
void GetCadPoint( CCAD_Point &cadPoint, FT_Vector* pointArray, int &curpnt ) {
	curpnt ++;
	cadPoint.m_fX = pointArray[curpnt].x / 64.0f;
	cadPoint.m_fY = pointArray[curpnt].y / 64.0f;
}

void
CCAD_GemSingleText::ConvGlyphToPoly(		CCAD_DC	* pDC,
											FT_Outline * outline)
{
	int cntr;

	FT_Vector* points = outline->points;
	FT_Vector* endPoint;

	int start = 0;
	int end = -1;
	int nPoint = 0;

	int curpnt = -1;
	bool blContEnd = false;

	SCAD_QSpline	sQSpline;
	SetQSpline(  sQSpline, QSPLINE_DIV_NUM );

	for ( cntr = 0; cntr < outline->n_contours; cntr ++ ) {
		start = end + 1;
		end = outline->contours[ cntr ];

		endPoint = &points[end];

		nPoint = end - start + 1;

		CCAD_GemTContour cContour;
		CCAD_Point	cPtWk[3], cPtTmp;
		int curwork = 0;
		blContEnd = false;

		while ( ( ! blContEnd ) && (&points[curpnt] <= endPoint) ) {
			if ( &points[curpnt] == endPoint ) {
				blContEnd = true;
				curpnt = start - 1;
			}
			switch ( FT_CURVE_TAG( outline->tags[ curpnt + 1 ] ) ) {
			case FT_CURVE_TAG_ON:
				switch ( curwork ) {
				case 0:
					GetCadPoint( cPtWk[0], points, curpnt );
					cContour.AddPoint(cPtWk[0]);
					break;
				case 1:
					GetCadPoint( cPtWk[2], points, curpnt );
					QSpline(cContour, cPtWk, &sQSpline);
					cPtWk[0] = cPtWk[2];
					break;
				}
				
				curwork = 0;
				break;
			case FT_CURVE_TAG_CONIC:
				switch ( curwork ) {
				case 0:
					GetCadPoint( cPtWk[1], points, curpnt );
					curwork = 1;
					break;
				case 1:
					GetCadPoint( cPtTmp, points, curpnt );

					cPtWk[2].m_fX = (cPtTmp.m_fX + cPtWk[1].m_fX) / 2.0F;
					cPtWk[2].m_fY = (cPtTmp.m_fY + cPtWk[1].m_fY) / 2.0F;
					cPtWk[2].m_fZ =  cPtTmp.m_fZ;
					
					QSpline(cContour, cPtWk, &sQSpline);
					
					cPtWk[0] = cPtWk[2];
					cPtWk[1] = cPtTmp;
					break;
				}
				break;
			default:
				printf ( "Error in glyph outline.\n" );
				break;
			}
			if ( blContEnd ) {
				curpnt = end;
			}
		}

		m_cContourArray.Add( cContour );
	}

	SetInOutContour();
	
	int nSizeContour = m_cContourArray.GetSize();
	int	i;
	for(i = 0; i < nSizeContour; i ++) {
//		BOOL blRot = blJudgRotDir(m_cContourArray[i]);
		BOOL blRot = m_cContourArray[i].blJudgRotDir();
		if(	(blRot == TRUE && m_cContourArray[i].m_blParent == TRUE) ||
			(blRot == FALSE && m_cContourArray[i].m_blParent == FALSE) ) {
			m_cContourArray[i].InvertContourDir();
		}
	}

	ConvGlyphToBody(pDC);
}*/
#endif

void
CCAD_GemSingleText::ConvGlyphToBody(CCAD_DC	*pDC)
{
	CCAD_Point	cSrcPnt, cDstPnt;
	int nSizeContour = m_cContourArray.GetSize();
	int	i, j;
	for(i = 0; i < nSizeContour; i ++) {
		int nSizePoint = m_cContourArray[i].m_cPointArray.GetSize();
		for(j = 0; j < nSizePoint; j ++) {
			cSrcPnt = m_cContourArray[i].m_cPointArray[j];
			GetPointOfGlyph(pDC, cSrcPnt, cDstPnt, m_cPos, m_fRateX, m_fRateY);
			m_cContourArray[i].m_cPointArray[j] = cDstPnt;
		}
	}
}

#ifdef  _WIN32
void
CCAD_GemSingleText::AddPoint(			CCAD_GemTContour	&cContour,
								const	CCAD_Point			cPt1,
								const	CCAD_Point			cPt2)
{
	float	fUnitLen = m_fHeight / LINE_DIV_NUM;
	float	fLen = cPt1.CountLength(cPt2);
	short	nDivNum = (short)(fLen / fUnitLen);

	CAD_FLOAT	dX = (cPt2.m_fX - cPt1.m_fX) / (nDivNum + 1);
	CAD_FLOAT	dY = (cPt2.m_fY - cPt1.m_fY) / (nDivNum + 1);

	CCAD_Point	cMidPt = cPt1;
	int	i;
	for(i = 0; i < nDivNum; i ++) {
		cMidPt.m_fX += dX;
		cMidPt.m_fY += dY;
		cContour.AddPoint(cMidPt);
	}
}

BYTE*
CCAD_GemSingleText::GetTTPOLYGONHEADER(TTPOLYGONHEADER &sTTPolygonHeader,BYTE *pFontData)
{
	sTTPolygonHeader.cb = *(DWORD *)pFontData;
	pFontData += sizeof(DWORD);
			
 	sTTPolygonHeader.dwType = *(DWORD *)pFontData;
	pFontData += sizeof(DWORD);
			
	sTTPolygonHeader.pfxStart.x.fract = *(WORD *)pFontData;
	pFontData += sizeof(WORD);
			
	sTTPolygonHeader.pfxStart.x.value = *(short *)pFontData;
	pFontData += sizeof(short);
			
	sTTPolygonHeader.pfxStart.y.fract = *(WORD *)pFontData;
	pFontData += sizeof(WORD);
			
	sTTPolygonHeader.pfxStart.y.value = *(short *)pFontData;
	pFontData += sizeof(short);
	return pFontData;
}
#endif

void CCAD_GemSingleText::AdjustPoints(CCAD_Angle cAng, CAD_FLOAT f)
{
	int nSizeContour = m_cContourArray.GetSize();
	int	i, j;
	for(i = 0; i < nSizeContour; i ++) {
		int nSizePoint = m_cContourArray[i].m_cPointArray.GetSize();
		CCAD_Point	cPt;
		for(j = 0; j < nSizePoint; j ++) {
			cPt = m_cContourArray[i].m_cPointArray[j];
			CAD_FLOAT fDeltaX = m_fHeight *f * cAng.sin();
			CAD_FLOAT fDeltaY = m_fHeight *f * cAng.cos();
			cPt.m_fX -= fDeltaX;
			cPt.m_fY += fDeltaY;
			m_cContourArray[i].m_cPointArray[j] = cPt;
		}
	}

}

void
CCAD_GemSingleText::GetPointOfGlyph(			CCAD_DC		*pDC,
									const	CCAD_Point	cSrcPnt,
											CCAD_Point	&cPt,
									const	CCAD_Point	&cPtBase,
									const	CAD_FLOAT	fRateX,
									const	CAD_FLOAT	fRateY)
{ 
	CAD_FLOAT	fScale = m_fHeight / LOGFONT_SIZE;
	
	CCAD_Point	cPnt = cSrcPnt;

	cPt.m_fX = (cPnt.m_fX + cPnt.m_fY * m_fSkew) / pDC->m_fScale * fScale;
	cPt.m_fY = (cPnt.m_fY / pDC->m_fScale) * fScale;
	
	cPt.m_fX += cPtBase.m_fX;
	cPt.m_fY += cPtBase.m_fY;
	cPt.m_fZ = cPtBase.m_fZ;
}

void
CCAD_GemSingleText::QSpline(		CCAD_GemTContour	&cContour,
							const	CCAD_Point			sPt[3],
									SCAD_QSpline		*spQSpl)
{
	WORD wDivNum = spQSpl->wDivNum;

	CCAD_Point	cPtWk;
	WORD	wNum;
  	for (wNum=0; wNum <= wDivNum; ++wNum) {
		cPtWk.m_fX = spQSpl->fA[wNum]*sPt[0].m_fX + spQSpl->fB[wNum]*sPt[1].m_fX + spQSpl->fC[wNum]*sPt[2].m_fX;
		cPtWk.m_fY = spQSpl->fA[wNum]*sPt[0].m_fY + spQSpl->fB[wNum]*sPt[1].m_fY + spQSpl->fC[wNum]*sPt[2].m_fY;
		cPtWk.m_fZ = sPt[0].m_fZ;
		cContour.AddPoint(cPtWk);
	}
}

#ifdef  _WIN32
BYTE*
CCAD_GemSingleText::GetTTPOLYCURVE(	TTPOLYCURVE	&sTTPolyCurve,
									POINTFX		*sPntFX,
									BYTE		*pFontData)
{
	sTTPolyCurve.wType = *(WORD *)pFontData;
 	pFontData += sizeof(WORD);

	sTTPolyCurve.cpfx = *(WORD *)pFontData;
	pFontData += sizeof(WORD);
 			
	WORD	i;
	for(i=0; i < sTTPolyCurve.cpfx ; ++i){
		sPntFX[i].x.fract = *(WORD *)pFontData;
		pFontData += sizeof(WORD);
			
		sPntFX[i].x.value = *(short *)pFontData;
		pFontData += sizeof(short);
 						
		sPntFX[i].y.fract = *(WORD *)pFontData;
		pFontData += sizeof(WORD);
							
		sPntFX[i].y.value = *(short *)pFontData;
		pFontData += sizeof(short);
	}						 
	return pFontData;
}		

void
CCAD_GemSingleText::PntFXtoVector(	POINTFX&	sPntFX,
									CCAD_Point	&sPnt)
{
	CAD_FLOAT fFraction;
 
	fFraction = (CAD_FLOAT)sPntFX.x.fract;
	fFraction /= 65536.0F;
	sPnt.m_fX = sPntFX.x.value + fFraction;
	fFraction = (CAD_FLOAT)sPntFX.y.fract;
	fFraction /= 65536.0F;
	sPnt.m_fY = sPntFX.y.value + fFraction;
}
#endif

void
CCAD_GemSingleText::SetQSpline(SCAD_QSpline &sQSpline, WORD wDivNum)
{
	ASSERT( wDivNum >= 2 && wDivNum < CURVE_DIV_MAX);

	float fValue, fStep = (float)wDivNum;
	WORD	wNum;
	for (wNum=0; wNum <= wDivNum; ++wNum) {
		fValue = (float)wNum / fStep; 
		sQSpline.fA[wNum] = (1.0F - fValue) * (1.0F - fValue);
		sQSpline.fB[wNum] = 2.0F * (1.0F - fValue) * fValue;
		sQSpline.fC[wNum] = fValue * fValue;
	}
	sQSpline.wDivNum = wDivNum;
}

void
CCAD_GemSingleText::SetInOutContour(void)
{
	int nSizeArray = m_cContourArray.GetSize();
	
	int	i, j;
	for(i = 0; i < nSizeArray; i ++) {
		m_cContourArray[i].GetXMaxPoint();
	}
	
	for(i = 0; i < nSizeArray; i ++) {
		for(j = 0; j < nSizeArray; j ++) {
			if(i == j) {
				continue;
			}
			if(JudgInOut(m_cContourArray[i].m_cXMax, m_cContourArray[j]) == TRUE) {
				m_cContourArray[i].m_blInFlag = TRUE;
				m_cContourArray[i].m_blParent = FALSE;
				m_cContourArray[j].AddChild(i);
			}
		}
	}

}

BOOL
CCAD_GemSingleText::JudgInOut(	const	CCAD_Point			sMax,
								const	CCAD_GemTContour	&cContour)
{
	CCAD_Point sTem0, sTem1, sTem2, sTem3;
	CCAD_Point sTem = sMax;
	short	nLeft, nRight;
	nLeft = nRight = 0;
	float fX_Min = ( float )3.0E+38;
	BOOL	blRet = FALSE;
	
	short	nSizePoint = cContour.m_cPointArray.GetSize();

	if( nSizePoint < 3 )	// Number of points formed contour must be greater than 2.
		return FALSE;

	int	i;
	for(i = 0; i < nSizePoint; i ++) {
		sTem0 = cContour.m_cPointArray[i];
		if ( i == nSizePoint - 1 ) {
			sTem1 = cContour.m_cPointArray[0];
			sTem2 = cContour.m_cPointArray[1];
			sTem3 = cContour.m_cPointArray[2];
		}
		else if ( i == nSizePoint - 2 ) {
			sTem1 = cContour.m_cPointArray[i + 1];
			sTem2 = cContour.m_cPointArray[0];
			sTem3 = cContour.m_cPointArray[1];
		}
		else if ( i == nSizePoint - 3 ) {
			sTem1 = cContour.m_cPointArray[i + 1];
			sTem2 = cContour.m_cPointArray[i + 2];
			sTem3 = cContour.m_cPointArray[0];
		}
		else {
			sTem1 = cContour.m_cPointArray[i + 1];
			sTem2 = cContour.m_cPointArray[i + 2];
			sTem3 = cContour.m_cPointArray[i + 3];
		}
	
		float fTem;
		if ( ( sTem0.m_fY < sTem.m_fY && sTem.m_fY < sTem1.m_fY ) ||
			 ( sTem0.m_fY > sTem.m_fY && sTem.m_fY > sTem1.m_fY ) ) {
			fTem = ( sTem1.m_fX - sTem0.m_fX ) * 
				   ( sTem.m_fY - sTem0.m_fY ) /
				   ( sTem1.m_fY - sTem0.m_fY ) + sTem0.m_fX;
			if ( fTem > sTem.m_fX ) {
				nRight ++;
				if ( fX_Min > fTem )
					fX_Min = fTem;
			}
			
			else	if ( fTem < sTem.m_fX ) nLeft ++;
		}
		
		else if ( sTem.m_fY == sTem1.m_fY ) {
			if ( ( sTem0.m_fY < sTem.m_fY && sTem.m_fY < sTem2.m_fY ) ||
			     ( sTem0.m_fY > sTem.m_fY && sTem.m_fY > sTem2.m_fY ) ) {
				fTem = sTem1.m_fX;
				if ( fTem > sTem.m_fX ) {
					nRight ++;
					
					if ( fX_Min > fTem ) fX_Min = fTem;
				}
				
				else if ( fTem < sTem.m_fX ) nLeft ++;
			}
			
			else if ( ( sTem0.m_fY < sTem.m_fY && sTem2.m_fY < sTem.m_fY ) ||
				      ( sTem0.m_fY > sTem.m_fY && sTem2.m_fY > sTem.m_fY ) ) {
				fTem = sTem1.m_fX;
				if ( fTem > sTem.m_fX ) {
					nRight += 2;
					if ( fX_Min > fTem ) fX_Min = fTem;
				}
				
				else if ( fTem < sTem.m_fX ) nLeft += 2;
			}
			
			else if ( sTem.m_fY == sTem2.m_fY ) {
				if ( ( sTem0.m_fY < sTem.m_fY && sTem.m_fY < sTem3.m_fY ) ||
				     ( sTem0.m_fY > sTem.m_fY && sTem.m_fY > sTem3.m_fY ) ) {
					if ( sTem.m_fX < sTem1.m_fX ) {
						fTem = ( sTem1.m_fX < sTem2.m_fX )
							   ? sTem1.m_fX : sTem2.m_fX;
					}
					
					else fTem = sTem1.m_fX;
					
					if ( fTem > sTem.m_fX ) {
						nRight ++;
						if ( fX_Min > fTem ) fX_Min = fTem;
					}
					
					else if ( fTem < sTem.m_fX ) nLeft ++;
				}
				
				else if ( ( sTem0.m_fY < sTem.m_fY && sTem3.m_fY < sTem.m_fY ) ||
						  ( sTem0.m_fY > sTem.m_fY && sTem3.m_fY > sTem.m_fY ) ) {
					if ( sTem.m_fX < sTem1.m_fX ) {
						fTem = ( sTem1.m_fX < sTem2.m_fX )
							   ? sTem1.m_fX : sTem2.m_fX;
					}
					
					else fTem = sTem1.m_fX;
					
					if ( fTem > sTem.m_fX ) {
						nRight += 2;
						if ( fX_Min > fTem ) fX_Min = fTem;
					}
					
					else if ( fTem < sTem.m_fX ) nLeft += 2;
				}
				i ++;
			}
		}
	}

	if ( ( ( nLeft & nRight ) & 0x1 ) == 0 ) {
		blRet = FALSE;
	}
	else {
		blRet = TRUE;
	}
	
	return blRet;
}

BOOL
CCAD_GemSingleText::blJudgRotDir (const CCAD_GemTContour	&cContour)

{
	WORD		wIndex, wNumPoint, wMin, wTem,
				wIdxTem[6];
	float		fValue[6];
		
	BOOL blRet = FALSE;
	wIdxTem[1] = wIdxTem[2] = wIdxTem[3] = wIdxTem[4] = 0;
	wNumPoint = cContour.m_cPointArray.GetSize();
	
	CCAD_Point	cTemPt = cContour.m_cPointArray[0];
	fValue[1] = fValue[3] = cTemPt.m_fX;
	fValue[2] = fValue[4] = cTemPt.m_fY;
	
	for ( wIndex = 1; wIndex < wNumPoint; wIndex ++ ) {
		cTemPt = cContour.m_cPointArray[wIndex];
		if ( fValue[1] > cTemPt.m_fX )
			fValue[1] = cTemPt.m_fX, wIdxTem[1] = wIndex;	// X min
		else	if ( fValue[3] < cTemPt.m_fX )
				fValue[3] = cTemPt.m_fX, wIdxTem[3] = wIndex;	// X max
		
		if ( fValue[2] > cTemPt.m_fY )
			fValue[2] = cTemPt.m_fY, wIdxTem[2] = wIndex;	// Y min
		else	if ( fValue[4] < cTemPt.m_fY )
				fValue[4] = cTemPt.m_fY, wIdxTem[4] = wIndex;	// Y max
	}
	
	wIdxTem[0] = wIdxTem[4], wIdxTem[5] = wIdxTem[1];
	wTem = 1, wMin = wIdxTem[1];
	for ( wIndex = 2; wIndex < 5; wIndex ++ ) {
		if ( wMin > wIdxTem[wIndex] )
			wMin = wIdxTem[wIndex], wTem = wIndex;
	}
	
	if ( wIdxTem[wTem - 1] < wIdxTem[wTem + 1] ) blRet = TRUE;
	else	blRet = FALSE;
	
	return blRet;
}
void
CCAD_GemSingleText::Draw(CCAD_DC *pDC, const CCAD_Vector &cThick, const BYTE bPaint)
{
	PFONT oldFont = pDC->SelectFont(m_spFont);
	pDC->TextOut(m_wChar, m_cBndRect, m_fW_HScale, m_fSkew);
	pDC->SelectFont(oldFont);
/*
	short nSizeContour = (short)m_cContourArray.GetSize();
	if (nSizeContour <= 0)
		return;
	int	i, j, k;
	short nSizePoint;
	if (bPaint == YES && cThick.m_fZ == 0){
		if (!pDC->m_pCamera->m_bPerspect){
			short * pNum = new short[nSizeContour];
			if (pNum != NULL)
			{
				short	nTotNum = 0;
				for(i=0; i<nSizeContour; i++){
					*(pNum+i) = (short)m_cContourArray[i].m_cPointArray.GetSize();
					nTotNum += *(pNum+i);
				}
				if (nTotNum > 0){
					CCAD_Point	*pPt = NULL;
					pPt = new CCAD_Point[nTotNum];
					if (pPt != NULL) {
						for(i=0, k=0; i<nSizeContour; i++){
							nSizePoint = (short)m_cContourArray[i].m_cPointArray.GetSize();
							for(j=0; j<nSizePoint; j++){
								*(pPt+k) = m_cContourArray[i].m_cPointArray[j];
								k++;
							}
						}
						int nTriCount = 0;
						pDC->PolyPolygon(pPt, pNum, nSizeContour, NULL, nTriCount);
						delete []pPt;
					}
				}
				delete []pNum;
			}
		}
	}
	for(i=0; i<nSizeContour; i++){
		nSizePoint = m_cContourArray[i].m_cPointArray.GetSize();
		pDC->MoveTo(m_cContourArray[i].m_cPointArray[0]);
		for(j=1; j<nSizePoint; j++)
			pDC->LineTo(m_cContourArray[i].m_cPointArray[j]);
		pDC->LineTo(m_cContourArray[i].m_cPointArray[0]);
	}
*/
}

void CCAD_GemSingleText::CountBodyRange(CCAD_Rect& cRange)
{
	short nSizeContour = m_cContourArray.GetSize();	
	int	i, j;
	for(i = 0; i < nSizeContour; i ++) {
		int nSizePoint = m_cContourArray[i].m_cPointArray.GetSize();
		for(j = 0; j < nSizePoint; j ++)
			cRange.ResetParameter(m_cContourArray[i].m_cPointArray[j]);
	}
}

void CCAD_GemSingleText::CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange)
{
	short nSizeContour = m_cContourArray.GetSize();	
	int	i, j;
	for(i = 0; i < nSizeContour; i ++) {
		int nSizePoint = m_cContourArray[i].m_cPointArray.GetSize();
		for(j = 0; j < nSizePoint; j ++)
			pCadDC->CountVcsRange(m_cContourArray[i].m_cPointArray[j], cRange);
	}
}
