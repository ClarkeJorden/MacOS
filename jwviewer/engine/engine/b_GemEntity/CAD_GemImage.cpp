#include "stdafx.h"
#include "CAD_ClipPoly.h"
#include "CAD_GemImage.h"
#include "CAD_GemLine.h"
#include "CAD_ViewMain.h"
#include "CAD_Image.h"

typedef struct tagBucketSort {
	long	nX;				// X Coordinate
} BucketSort;

typedef struct  SpanRecord {
	long		scrVOffset;
	long		scrHOffset;
	long		nRecPixels;
} SpanRec;

class CMapping 
{
public:
	CMapping();
	CMapping(CCAD_DC* pDC, POINT *scrnpt, int  ptcount);
	virtual ~CMapping();

	void	Init();
	void	GetSpans(POINT *pt, int count);
	void	Rasterize();
	void	YBuckToSpan();
	void	XBuckToYBuck();
	void	ProPolyBoundary( POINT *pt, int count);
	void	EdgePixelsOnX(long prevX, long prevY, long x, long y);

	BOOL	FreeMem();
	BOOL	ReleaseMem(void * DaMemPtr);
	BOOL	MallocMem();
	void * AllocMem(long DaSize);

	void	GetDestRect(RECT * rect = NULL);

	int				m_nCount;
	POINT			m_point[100];


	char*			gMainWorkBuffer;
	long*			gXBucketTab;	// index buffer on screen X
	long*			gYBucketTab;	// index buffer on screen Y
	unsigned long*	m_pFreeBuffer;
	
	RECT				m_sRect;
};

CMapping::CMapping()
{
	Init();
}

void CMapping::Init()
{
	m_nCount = 0;
	gMainWorkBuffer = NULL;
	gXBucketTab = NULL;
	gYBucketTab = NULL;
}

CMapping::CMapping(CCAD_DC* pDC, POINT *scrnpt, int  ptcount)
{
	Init();

	for(int i = 0; i < ptcount; i++)
		m_point[i] = scrnpt[i];
	m_point[ptcount] = m_point[0];
	m_nCount = ptcount + 1;

	GetDestRect(NULL);
}

CMapping::~CMapping()
{
	FreeMem();
}

void CMapping::GetDestRect(RECT* clientRect)
{
	m_sRect.top = MAX_LONG;
	m_sRect.bottom = -MAX_LONG;
	m_sRect.left = MAX_LONG;
	m_sRect.right = -MAX_LONG;

	for(int i = 0; i < m_nCount; i++)
	{
		if(m_point[i].y < m_sRect.top)
			m_sRect.top = m_point[i].y;
		if(m_point[i].y > m_sRect.bottom)
			m_sRect.bottom = m_point[i].y;
		if(m_point[i].x < m_sRect.left)
			m_sRect.left = m_point[i].x;
		if(m_point[i].x > m_sRect.right)
			m_sRect.right = m_point[i].x;
	}

	if(m_sRect.top < 0)
		m_sRect.top  = 0;

	if(m_sRect.left < 0)
		m_sRect.left  = 0;

	if (NULL != clientRect)
	{
		if(m_sRect.right > (clientRect->right - clientRect->left))
			m_sRect.right = clientRect->right - clientRect->left;
		if(m_sRect.bottom > (clientRect->bottom - clientRect->top))
			m_sRect.bottom = clientRect->bottom - clientRect->top;
	}
}

BOOL CMapping::MallocMem()
{
	long nWidth = m_sRect.right - m_sRect.left + 3;
	long nHeight = m_sRect.bottom - m_sRect.top + 3;

	gMainWorkBuffer = (char *)AllocMem( (nWidth+nHeight)*m_nCount*2*sizeof(long));  // maximum tmp data size!
	if (gMainWorkBuffer == NULL)
		return FALSE;

	gXBucketTab = (long *)AllocMem(nWidth * sizeof(long));
	if (gXBucketTab == NULL)
		return FALSE;

	gYBucketTab = (long *)AllocMem(nHeight * sizeof(long));
	if (gYBucketTab == NULL)
		return FALSE;

	return TRUE;
}

void * CMapping::AllocMem(long DaSize)
{
	if(DaSize == 0)
		return NULL;
	void * pMem = malloc(DaSize);
	return pMem;
}

BOOL CMapping::FreeMem()
{
	if(!ReleaseMem(gMainWorkBuffer))
		return FALSE;

	if(!ReleaseMem(gXBucketTab))
		return FALSE;

	if(!ReleaseMem(gYBucketTab))
		return FALSE;

	return TRUE;
}

BOOL CMapping::ReleaseMem(void * DaMemPtr)
{
	if(DaMemPtr == NULL)
		return FALSE;

	free(DaMemPtr);
	return TRUE;
}

void CMapping::EdgePixelsOnX(long prevX, long prevY, long x, long y)
{
	long		d1, d3, d4, d5, d6, a3;

	d4 = prevY;
	d3 = y;
	a3 = d3 - d4;
	if ( a3 == 0)
		return;
	d5 = y - prevY;
	d6 = x - prevX;

	x = d6;
	d1 = d5 * prevX + (d4 * x - prevY * d6) - 1;
	d3 = d5;
	y = d3 + d1;
	if( y >= 0)
	{
		d5 = y / d3;
		d6 = y - d5 * d3 ;
	}
	else
	{
		prevY = d5 = -y;
		d5 = -d5 / d3;
		d6 = prevY - (-d5 * d3);
		if( d6 != 0) 
		{
			d5--;
			d6 = d3- d6;
		}
	}
	if( x >= 0)	
	{
		y = x / d3;
		prevY = x - y * d3;
	}
	else 
	{
		long	val20;

		val20 = y = -x;
		y = -y / d3;
		prevY = val20 - (-y * d3);
		if( prevY != 0) 
		{
			y--;
			prevY = d3 - prevY;
		}
	}

	// increment y Coordinate
	while( a3-- > 0) 
	{
		if (d5 < m_sRect.left)
			d5 = m_sRect.left;
		else if (d5 > m_sRect.right)
			d5 = m_sRect.right;

		*m_pFreeBuffer = gXBucketTab[d5-m_sRect.left];			// Previous Pointer
		gXBucketTab[d5-m_sRect.left] = (long) m_pFreeBuffer++;	// Save Current Pointer to XBucket
		*m_pFreeBuffer++ = d4;									// Y

		d6 += prevY;
		d5 += y;

		d4++;
		if(d6 >= d3)
			d5++,	d6 -= d3;
	}
}

// find pixels on polygon boundary
void CMapping::ProPolyBoundary( POINT *pt, int count)
{
	long		x, prevx;
	long		y, prevy;

	prevx = pt[0].x;
	prevy = pt[0].y;

	for(int i = 1; i < count; i ++)
	{
		x = (long) pt[i].x;
		y = (long) pt[i].y;
		if( prevy < y)
			EdgePixelsOnX( prevx, prevy, x, y);
		else
			EdgePixelsOnX( x, y, prevx, prevy);
		prevx = x;
		prevy = y;
	}
}

void	CMapping::XBuckToYBuck()
{
	unsigned long	*freeBuff;
	long			*pXBucket, *ptrXBuck, y;

	freeBuff = m_pFreeBuffer + 32;								// y bucket value position
	ptrXBuck = gXBucketTab + (m_sRect.right-m_sRect.left) + 1;
	for(long x = m_sRect.right; x >= m_sRect.left; x--) {
		pXBucket = (long*) *(--ptrXBuck);
		while (pXBucket) {
			y = pXBucket[1];
			if (y < m_sRect.top)
				y = m_sRect.top;
			else if (y > m_sRect.bottom)
				y = m_sRect.bottom;
			
			*freeBuff = gYBucketTab[y-m_sRect.top];			// Previous Pointer
			gYBucketTab[y-m_sRect.top] = (long)freeBuff++;	// Save Current Pointer to YBucket
			*freeBuff++ = x;
			pXBucket = (long*) *pXBucket;					// Previous X List data
		}
	}
//	long len = (char*) freeBuff - (char*) m_pFreeBuffer;		// only to count data size!
	return;
}

void CMapping::YBuckToSpan()
{
	long		*ptrYBuck, *pYBucket = gYBucketTab;
//	SpanRec	*	span = m_pSpanRec;
	SpanRec	*	span = NULL;

	for(long sy = m_sRect.top; sy < m_sRect.bottom; sy++) {

		ptrYBuck = (long*) *pYBucket++;
	
		long nX1, nX = -1;
		while(ptrYBuck) {
			nX1 = ptrYBuck[1];
			if(nX1 > m_sRect.right)
				nX1 = m_sRect.right;
			else if(nX1 < m_sRect.left)
				nX1 = m_sRect.left;

			//--------------
			if ( nX != -1 ) {
				long numPixels = nX1 - nX;		 // x offset
				if (numPixels > 0) {
					span->scrVOffset = sy;
					span->scrHOffset = nX;
					span->nRecPixels = numPixels;
					span++;
				}
				nX = -1;
			}
			else
				nX = nX1;
			//--------------
			ptrYBuck = (long*) *ptrYBuck;
		}
	}
	span->scrVOffset = 0xffff;
}

//////////////////////////////////////
// From Sorted Semi-Span Data to Span
//////////////////////////////////////
void CMapping::Rasterize()
{
	long 			nbucket = 0;
//	long 			nport = 0;
	long 			nX;
	BucketSort	*bucket = (BucketSort*) gMainWorkBuffer;
	SpanRec	*	span = NULL;

	for (long y = m_sRect.top; y < m_sRect.bottom; y++)	{	// loop on y
		if (bucket[nbucket].nX != -1) {	// semi-span list
			nX = bucket[nbucket].nX;
			while (nX != -1) {
				long i = nbucket;
				long j = nbucket+1;

				if (bucket[j].nX == -1)
					nbucket++;
				else {
					long numPixels = bucket[j].nX - bucket[i].nX; // x offset
					if (numPixels > 0) {
						span->scrVOffset = y;
						span->scrHOffset = bucket[i].nX;
						span->nRecPixels = numPixels;
						span++;
					}
					nbucket += 2;
				}
				nX = bucket[nbucket].nX;
			}	// while 
		}
		nbucket++;
	}
	span->scrVOffset = 0xffff;
}

void CMapping::GetSpans(POINT *pt, int count)
{
	long *p2 = gYBucketTab, *p3 = gXBucketTab;

	// initialize working buffers
	for(int x = (m_sRect.right-m_sRect.left+1); x >=0; x--)
		*p3++ = 0;
	for(int y = (m_sRect.bottom-m_sRect.top+1); y >=0; y--)
		*p2++ = 0;

	m_pFreeBuffer = (unsigned long*) gMainWorkBuffer;

	ProPolyBoundary(pt, count);
	
	XBuckToYBuck();

//	YBuckToSpan();
	
//	Rasterize(m_sRect);
}


/////////////////////////////////////////////////////////////////////////////
// CCAD_GemImage
//
CCAD_GemImage::CCAD_GemImage()
{
	m_nPtNum = 0;
	m_pPt = NULL;
	m_pScrnPt = NULL;
	m_bDraw = DRAW_FRAME;
	m_nIndex = -1;
	m_pImage = NULL;
    m_szName[0] = '\0';
	m_pTriPoints = NULL;
	m_nTriCount = 0;
    
    m_dRotAngle = 0;
    m_bIsTrim = FALSE;
    m_dTrimLeft = 0;
    m_dTrimRight = 0;
    m_dTrimTop = 0;
    m_dTrimBottom = 0;
    
    m_dBmpWidth = 0;
    m_dBmpHeight = 0;
    m_dBmpTrim1 = 0;
    m_dBmpTrim2 = 0;
    m_dBmpScale = 1;
}

CCAD_GemImage::~CCAD_GemImage()
{
	Remove();
}

//----------------------------------------------
void CCAD_GemImage::Remove()
{
	if (m_pPt != NULL)
	{
		delete [] m_pPt;
		m_pPt = NULL;
	}

	if(m_pScrnPt != NULL) {
		delete [] m_pScrnPt;
		m_pScrnPt = NULL;
	}
	
	if(m_pTriPoints)
		delete[] m_pTriPoints;
	m_nPtNum = 0;
}

//----------------------------------------------
void CCAD_GemImage::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	try
	{
		CCAD_Gem2DEntity::MemCopyTo(pGemEntity, pData1, pData2, pData3);

		((CCAD_GemImage *)pGemEntity)->m_nPtNum = m_nPtNum;
		((CCAD_GemImage *)pGemEntity)->m_bDraw = m_bDraw;
		((CCAD_GemImage *)pGemEntity)->m_nIndex = m_nIndex;
		((CCAD_GemImage *)pGemEntity)->m_pImage = m_pImage;

		if (pData1 == NULL)
			pData1 = (LPSTR)new CCAD_Vector[m_nPtNum];

		short i;
		((CCAD_GemImage *)pGemEntity)->m_pPt = (CCAD_Point *)pData1;
		for (i=0; i<m_nPtNum; i++)
			*(((CCAD_GemImage *)pGemEntity)->m_pPt + i) = *(m_pPt + i);
	}
	catch(...)
	{
	}
}

//--------------------------------------------
BOOL CCAD_GemImage::Read(CCAD_File & cFile)
{
	CCAD_Gem2DEntity::Read(cFile);
	cFile.ReadByte(&m_bDraw);
	cFile.ReadShort(&m_nPtNum);
	cFile.ReadShort(&m_nIndex);
	try {
		if (m_nPtNum > 0) {
			m_pPt = new CCAD_Vector[m_nPtNum];
			short	i;
			for (i=0; i<m_nPtNum; i++)
				(m_pPt+i)->Read(cFile);
		}
	}
	catch(...) {
		return FALSE;
	}
	return TRUE;
}

//--------------------------------------------
BOOL CCAD_GemImage::Draw(CCAD_DC *pDC, BOOL blRedraw)
{
	try
	{
		if (m_pPt == NULL)
			return FALSE;

		BOOL	blPaint = false;
		if (m_bDraw & DRAW_PAINT /*&& !IsPanMoving()*/){
			if (!pDC->m_pCamera->m_bPerspect) {
					DrawImage(pDC);
					blPaint = true;
			}
		}
		if ((m_bDraw & DRAW_FRAME) || !blPaint /*|| IsPanMoving()*/) {
			short i;

			pDC->Lines_Begin(LINETYPE_STRIP);
			pDC->MoveTo(*m_pPt);
			for (i=1; i<m_nPtNum; i++)
				pDC->LineTo(*(m_pPt+i));
			if ( m_nPtNum > 2 )
				pDC->LineTo(*m_pPt);
			pDC->Lines_End();
		}
		else {
			LOGPEN oldPen = pDC->SelectNullPen();
			short i;
			pDC->Lines_Begin(LINETYPE_STRIP);
			pDC->MoveTo(*m_pPt);
			for (i=1; i<m_nPtNum; i++)
				pDC->LineTo(*(m_pPt+i));
			if ( m_nPtNum > 2 )
				pDC->LineTo(*m_pPt);
			pDC->Lines_End();

			pDC->RestoreObject(oldPen);
		}
	}
	catch(...)
	{
	}
	return true;
}

//-----------------------------------------------------
void CCAD_GemImage::CountBodyRange(CCAD_Rect& cRange)
{
	CCAD_Gem2DEntity::CountBodyRange(cRange);
	if (m_nPtNum > 2 && m_pPt != NULL) {
		short i;
		for (i=0; i<m_nPtNum; i++) {
			cRange.ResetParameter(*(m_pPt+i));
		}
	}
}

//-----------------------------------------------------
void CCAD_GemImage::CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange)
{
//	ASSERT(pCadDC != NULL);
	if (pCadDC == NULL)
		return;

	if (m_nPtNum > 2 && m_pPt != NULL) {
		short i;
		for (i=0; i<m_nPtNum; i++) {
			pCadDC->CountVcsRange(*(m_pPt+i), cRange);
		}
	}
}

/*******************************************************************************
	Function name:	DrawImage
********************************************************************************/
BOOL CCAD_GemImage::DrawImage(CCAD_DC* pDC)
{
	return DrawImageInTextureMode(pDC);
//	return DrawImageInPixelMode(pDC);
}

BOOL CCAD_GemImage::DrawImageInTextureMode(CCAD_DC* pDC)
{
	if (m_nPtNum < 3)
		return FALSE;
    if (m_nIndex < 0)
        return FALSE;

	BOOL bRet = TRUE;

	if(m_nTriCount == 0)
		m_pTriPoints = pDC->GetTrianglesFromPolygon(m_pPt, &m_nPtNum, 1, m_nTriCount);
	
	float minX = 0.f, maxX = 0.f, minY = 0.f, maxY = 0.f;
	GLfloat *vb = new GLfloat[m_nTriCount * 2];
    for(int i = 0; i < m_nTriCount; i ++)
    {
        POINT pt = pDC->WCStoLP(m_pTriPoints[i]);
        
        vb[i * 2] = pt.x;
        vb[i * 2 + 1] = pt.y;
    }
    
    for(int i = 0; i < m_nTriCount; i ++)
    {
        CCAD_Point pt = m_pTriPoints[i];
        
        if(i == 0)
        {
            minX = maxX = pt.m_fX;
            minY = maxY = pt.m_fY;
        }
        else
        {
            if(maxX < pt.m_fX)
                maxX = pt.m_fX;
            else if(pt.m_fX < minX)
                minX = pt.m_fX;
            
            if(maxY < pt.m_fY)
                maxY = pt.m_fY;
            else if(pt.m_fY < minY)
                minY = pt.m_fY;
        }
    }

	float boundW = maxX - minX;
	float boundH = maxY - minY;
	if(boundW < 1 || boundH < 1)
	{
		bRet = FALSE;
	}
	else
	{	
		long realhgt = 1 << (long)ceil(log((float)m_pImage->m_sImage.iHeight)/log(2.0f));
		long realwid = 1 << (long)ceil(log((float)m_pImage->m_sImage.iWidth)/log(2.0f));
		BOOL bIsCreateNewImg = (realwid != m_pImage->m_sImage.iWidth) || (realhgt != m_pImage->m_sImage.iHeight);
		PIXELDATA *spNewImgData = m_pImage->m_sImage.pData;
		if(bIsCreateNewImg){
			spNewImgData = new PIXELDATA[realwid * realhgt];
			PIXELDATA *spCurDstData = spNewImgData;
			PIXELDATA *spCurSrcData = m_pImage->m_sImage.pData;
			for(long i = 0; i < realhgt; ++i){
				for(long j = 0; j < realwid; ++j){
					if(i < m_pImage->m_sImage.iHeight && j < m_pImage->m_sImage.iWidth){
						*spCurDstData = *spCurSrcData;
						spCurSrcData++;
					}else{
						spCurDstData->bRed = spCurDstData->bGreen = spCurDstData->bBlue = spCurDstData->bDummy = 255;
					}
					spCurDstData++;
				}
			}
		}

		glEnable(GL_TEXTURE_2D);
//		glEnable(GL_ALPHA_TEST);
//		glAlphaFunc(GL_GREATER, 0.1f);
		
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, realwid, realhgt, 0, GL_RGBA, GL_UNSIGNED_BYTE, spNewImgData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		
		float tex_xratio = (float)m_pImage->m_sImage.iWidth / realwid;
		float tex_yratio = (float)m_pImage->m_sImage.iHeight / realhgt;
		float* texs = new float[m_nTriCount * 2];
        if (m_bIsTrim) {
            texs[0] = m_dTrimLeft * tex_xratio; texs[1] = m_dTrimBottom * tex_yratio;
            texs[2] = m_dTrimRight * tex_xratio; texs[3] = m_dTrimBottom * tex_yratio;
            texs[4] = m_dTrimRight * tex_xratio; texs[5] = m_dTrimTop * tex_yratio;
            texs[6] = m_dTrimLeft * tex_xratio; texs[7] = m_dTrimBottom * tex_yratio;
            texs[8] = m_dTrimRight * tex_xratio; texs[9] = m_dTrimTop * tex_yratio;
            texs[10] = m_dTrimLeft * tex_xratio; texs[11] = m_dTrimTop * tex_yratio;
        } else {
            for(int i = 0; i < m_nTriCount; i ++)
            {
                texs[2*i] = tex_xratio * (m_pTriPoints[i].m_fX - minX) / boundW;
                texs[2*i+1] = tex_yratio * (m_pTriPoints[i].m_fY - minY) / boundH;
            }
        }
		
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, texs);
		glVertexPointer(2, GL_FLOAT, 0, vb);
		glDrawArrays(GL_TRIANGLES, 0, m_nTriCount);
		
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);

		if(bIsCreateNewImg) delete [] spNewImgData;
//		glDisable(GL_ALPHA_TEST);
		
		delete[] texs;
	}
	delete[] vb;
	return bRet;
}

#define ERROR_USER	1
#define ERROR_MEMORY	2
BOOL CCAD_GemImage::DrawImageInPixelMode(CCAD_DC* pDC)
{
	CMapping* map = NULL;
	try
	{
		if(NULL == m_pImage)
		{
			return true;
		}

		short nNumPt = ConvBCSToScrn(pDC);

		if(0 == nNumPt)
		{
			throw ERROR_USER;
		}

		if(NULL == m_pScrnPt)
		{
			throw ERROR_USER;
		}

		map = (CMapping *)new CMapping(pDC, m_pScrnPt, nNumPt);

		if(NULL != m_pScrnPt)
		{
			delete [] m_pScrnPt;
			m_pScrnPt = NULL;
		}

		if(map == NULL)
			return FALSE;

		if(!map ->MallocMem())
		{
			delete map;
			return FALSE;
		}

		if (!CreateImage(pDC, nNumPt, map->m_sRect))
		{
			throw ERROR_USER;
		}

		map->GetSpans(map->m_point, map->m_nCount);

		double denom =	(m_sLptForBCS[1].m_fX-m_sLptForBCS[0].m_fX)*(m_sLptForBCS[3].m_fY-m_sLptForBCS[0].m_fY) - 
						(m_sLptForBCS[3].m_fX-m_sLptForBCS[0].m_fX)*(m_sLptForBCS[1].m_fY-m_sLptForBCS[0].m_fY);
		if( (int)denom == 0 )
		{
			throw ERROR_USER;
		}

		int	w = m_pImage->m_sImage.iWidth-1;
		int	h = m_pImage->m_sImage.iHeight-1;

		double numer1 = m_sLptForBCS[3].m_fY-m_sLptForBCS[0].m_fY;
		double numer2 = -(m_sLptForBCS[3].m_fX-m_sLptForBCS[0].m_fX);
		double a = numer1/denom;
		a *= w;
		double b = numer2/denom;
		b *= w;
		double c = -(a*m_sLptForBCS[0].m_fX+b*m_sLptForBCS[0].m_fY);

		denom *= -1;
		numer1 = m_sLptForBCS[1].m_fY-m_sLptForBCS[0].m_fY;
		numer2 = -(m_sLptForBCS[1].m_fX-m_sLptForBCS[0].m_fX);
		double d = (float)numer1/denom;
		d *= h;
		double e = (float)numer2/denom;
		e *= h;
		double f = -(d*m_sLptForBCS[0].m_fX+e*m_sLptForBCS[0].m_fY);

//////////////////////////////
		CHAR *lpBits = new CHAR[ 13 * (int)m_sizeDIB.m_fW ];         // pointer to the DIB bits
		
		if ( lpBits != NULL ) {
			long		*ptrYBuck, *pYBucket = map->gYBucketTab;

			for(long sy = map->m_sRect.top; sy < map->m_sRect.bottom; sy++) {
				ptrYBuck = (long*) *pYBucket++;
			
				register long nX1, nX = -1;
				while(ptrYBuck) {
					nX1 = ptrYBuck[1];
					if(nX1 > map->m_sRect.right)
						nX1 = map->m_sRect.right;
					else if(nX1 < map->m_sRect.left)
						nX1 = map->m_sRect.left;

					//--------------
					if ( nX != -1 ) {
						if ( nX1 > nX ) {

							//--------------------------- Modified original code
							register int	sx = nX;
							register int	y = sy;
							register double	uu = a*sx+b*y+c;
							register double	vv = d*sx+e*y+f;
							register int	u, v;

							register int vcs0 = (int)m_sLptForVCS[0].m_fX;
				//			register int vcs1 = (int)m_sLptForVCS[1].m_fX;
							register int vcs2 = (int)m_sLptForVCS[2].m_fX;
							register int dib = m_sizeDIB.m_fW-1;
							register int nWidth = m_pImage->m_sImage.iWidth;
							register LPPIXELDATA	pImageData = m_pImage->m_sImage.pData;

							register	int		x0=-1, x1=-1;
							for(int x = nX; x<nX1; x++, uu+=a, vv+=d) {
								u = (int)(uu);
								if ( u > w )			u = w;
								else if ( u < 0 )	u = 0;

								v = (int)(vv);
								if ( v > h )			v = h;
								else if ( v < 0 )	v = 0;

								u += (nWidth * v);

								v = x;
								if( v < vcs0)
									v = (long)vcs0;
								else if( v > vcs2)
									v = (long)vcs2;

								if ( x0 == -1 )
									x0 = v;
								x1 = v;

								v = (int)fabs((float)v - vcs0);
								if ( v > dib )
									v = dib;

								register LPSTR	bpDibBits = lpBits + v*3;
								register LPPIXELDATA	pData = &pImageData[u];

								*bpDibBits = pData->bBlue;
								bpDibBits ++;
								*bpDibBits = pData->bGreen;
								bpDibBits ++;
								*bpDibBits = pData->bRed;
							}
							
							if (x0 < x1) {
								pDC->StretchDIBits(x0, y, (x1-x0)+1, (x0-(int)m_sLptForVCS[0].m_fX), 0, (x1-x0)+1, lpBits);
							}
							//--------------------------- Modified original code
						}
						nX = -1;
					}
					else
						nX = nX1;
					//--------------

					ptrYBuck = (long*) *ptrYBuck;
				}
			}
			
			delete [] lpBits;
		}
		delete map;
		map = NULL;
	}
	catch(int errCode)
	{
	if (errCode == ERROR_USER)
	  { 
		if(NULL != m_pScrnPt)
		{
			delete [] m_pScrnPt;
			m_pScrnPt = NULL;
		}

		if(NULL != map)
		{
			delete map;
			map = NULL;
		}

	  }
	else if (errCode == ERROR_MEMORY)
 	  {
		if(NULL != m_pScrnPt) {
			delete [] m_pScrnPt;
			m_pScrnPt = NULL;
		}

		if(NULL != map)	{
			delete map;
			map = NULL;
		}
	  }
	else  {
		if(NULL != m_pScrnPt)	{
			delete [] m_pScrnPt;
			m_pScrnPt = NULL;
		}

		if(NULL != map)	{
			delete map;
			map = NULL;
		}
	  }
	return false;
	}  // catch

	return true;
}

short	CCAD_GemImage::ConvBCSToScrn(CCAD_DC	*pDC)
{
	CScrnPtArray cScrnPtArray;
	CCAD_Vector* pEcsPt = NULL;
	CCAD_ClipPoly* pClipPoly = NULL;
	short nNumPt = 0;
	
	try
	{
		pEcsPt = new CCAD_Vector[m_nPtNum];

		int i;
		for(i = 0; i < m_nPtNum; i ++) {
			pEcsPt[i] = m_pPt[i];
			pDC->m_pCamera->ConvFromBase(pEcsPt[i]); // WCS->ECS
			pDC->m_pCamera->ECStoVCS(pEcsPt[i]);		// ECS->VCS
		}

		// clipping of polygon
		pClipPoly = new CCAD_ClipPoly(m_nPtNum);
		pClipPoly->SetPoints(m_nPtNum, pEcsPt);
		if(NULL != pEcsPt)
		{
			delete []pEcsPt;
			pEcsPt = NULL;
		}

		if ( !pClipPoly->Cliping(pDC->m_cClipRect) ) {
			delete pClipPoly;
			return 0;
		}

		m_pScrnPt = new POINT[pClipPoly->m_nNum];
		for (int k=0; k<pClipPoly->m_nNum; k++) {
			//------ VCStoLP;
			double	dx, dy;
			pDC->VCStoLP(pClipPoly->m_pPt[k], dx, dy);
			m_pScrnPt[k].x = (int)(dx+0.5f);
			m_pScrnPt[k].y = (int)(dy+0.5f);

			if(dx < 0)
			{
				dx = 0;
			}
			if(dy < 0)
			{
				dy = 0;
			}
		}

		nNumPt = pClipPoly->m_nNum;

		if(NULL != pClipPoly)
		{
			delete pClipPoly;
			pClipPoly = NULL;
		}
	}
	catch(...)
	{
		if(pEcsPt != NULL)
		{
			delete []pEcsPt;
		}
		
		if(pClipPoly != NULL)
		{
			delete pClipPoly;
		}
			return 0;
	}
	
	return nNumPt;
}

void CCAD_GemImage::ClipPoint(CCAD_DC *pCadDC, SCAD_POINT * spPt, const RECT & sRect)
{
	if(spPt->m_fX < sRect.left) {
		spPt->m_fX = (CAD_FLOAT)sRect.left;
	}
	if(spPt->m_fX > sRect.right) {
		spPt->m_fX = (CAD_FLOAT)sRect.right;
	}
	if(spPt->m_fY < sRect.top) {
		spPt->m_fY = (CAD_FLOAT)sRect.top;
	}
	if(spPt->m_fY > sRect.bottom) {
		spPt->m_fY = (CAD_FLOAT)sRect.bottom;
	}
}

BOOL CCAD_GemImage::CreateImage(CCAD_DC	*pDC, short , const RECT & sRect)
{
	if (m_pImage == NULL)
		return FALSE;

	CCAD_Rect cRange1;
	CountVcsRange(pDC, cRange1);
	CCAD_Point cStart(cRange1.m_fMinX, cRange1.m_fMinY, 0.0F);
	CCAD_Point cEnd(cRange1.m_fMaxX, cRange1.m_fMaxY, 0.0F);
	
	CCAD_Point		cVCSPt[4];
	cVCSPt[0] = cStart;
	cVCSPt[1].m_fX = cEnd.m_fX;
	cVCSPt[1].m_fY = cStart.m_fY;
	cVCSPt[2] = cEnd;
	cVCSPt[3].m_fX = cStart.m_fX;
	cVCSPt[3].m_fY = cEnd.m_fY;

	int i;
	double	dx, dy;
	for(i = 0; i < 4; i ++) {
		pDC->VCStoLP(cVCSPt[i], dx, dy);
		m_sLptForVCS[i].m_fX = (CAD_FLOAT)dx;
		m_sLptForVCS[i].m_fY = (CAD_FLOAT)dy;
		ClipPoint(pDC, &m_sLptForVCS[i], sRect);
	}
	
	int nWidth = fabs((m_sLptForVCS[2].m_fX - m_sLptForVCS[0].m_fX)) + 1;
	int nHeight = fabs((m_sLptForVCS[2].m_fY - m_sLptForVCS[0].m_fY)) + 1;
	if (!CreateDib(nWidth, nHeight, 24))
		return FALSE;

	CCAD_Rect cRange;
	CountBodyRange(cRange);			//	set start & end point

	CCAD_Point	cBCSPt[4];
	
	cBCSPt[0].m_fX = cRange.m_fMinX;
	cBCSPt[0].m_fY = cRange.m_fMinY;
	cBCSPt[2].m_fX = cRange.m_fMaxX;
	cBCSPt[2].m_fY = cRange.m_fMaxY;
	cBCSPt[1].m_fX = cBCSPt[2].m_fX;
	cBCSPt[1].m_fY = cBCSPt[0].m_fY;
	cBCSPt[3].m_fX = cBCSPt[0].m_fX;
	cBCSPt[3].m_fY = cBCSPt[2].m_fY;

	if ( (cBCSPt[0].m_fX == cBCSPt[2].m_fX) && (cBCSPt[0].m_fY == cBCSPt[2].m_fY) )
		return FALSE;

	CCAD_Point cPt;
	for(i = 0; i < 4; i ++) {
		cPt = cBCSPt[i];
		pDC->m_pCamera->ConvFromBase(cPt); // WCS->ECS
		pDC->m_pCamera->ECStoVCS(cPt);
		pDC->VCStoLP(cPt, dx, dy);
		m_sLptForBCS[i].m_fX = (CAD_FLOAT)dx;
		m_sLptForBCS[i].m_fY = (CAD_FLOAT)dy;
	}
	return TRUE;
}

BOOL	CCAD_GemImage::CreateDib(int cx, int cy, int ibitcount)
{
	if ((ibitcount != 8) && (ibitcount != 16) && (ibitcount != 24) && (ibitcount != 32))
	{
		return FALSE;
	}

	if (cx <= 0 || cy <= 0)
	{
		return FALSE;
	}

	// Save size for drawing later.
	m_sizeDIB.m_fW = cx;
	m_sizeDIB.m_fH = cy;

	return TRUE;
}

