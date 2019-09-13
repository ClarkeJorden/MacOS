#include "stdafx.h"
#include "CAD_ClipPoly.h"
#include "CAD_GeoLine.h"
//#include <math.h>

 

CCAD_ClipPoly::CCAD_ClipPoly(short nPtNum)
{
	m_nNum = 0;
	m_pFlag = new BYTE[nPtNum*3];
	m_pPt = new CCAD_Vector[nPtNum*3];
}

CCAD_ClipPoly::~CCAD_ClipPoly()
{
	if ( m_pFlag != NULL )
		delete [] m_pFlag;
	if ( m_pPt != NULL )
		delete [] m_pPt;
}

void	CCAD_ClipPoly::SetPoints( short nNum, SCAD_POINT * pPt )
{
	m_nNum = nNum;
	memcpy( m_pPt, pPt, sizeof(CCAD_Vector) * nNum );
}

void	CCAD_ClipPoly::AddPoints( CCAD_Vector * pPt )
{
	m_pPt[m_nNum] = *pPt;
	m_nNum++;
}

BOOL	CCAD_ClipPoly::Cliping( const CCAD_Rect & cDrawRc )
{
	return cDrawRc.IsPointsInRect2D(m_pPt, m_nNum);
// 	if ( m_nNum < 3 )
// 		return FALSE;
// 	ClipPoints( cDrawRc.m_fMinY, CLIP_HORZ, CLIP_INF );
// 	if ( m_nNum < 3 )
// 		return FALSE;
// 	ClipPoints( cDrawRc.m_fMaxY, CLIP_HORZ, CLIP_SUF );
// 	if ( m_nNum < 3 )
// 		return FALSE;
// 	ClipPoints( cDrawRc.m_fMinX, CLIP_VERT, CLIP_INF );
// 	if ( m_nNum < 3 )
// 		return FALSE;
// 	ClipPoints( cDrawRc.m_fMaxX, CLIP_VERT, CLIP_SUF );
// 	return (m_nNum >= 3);
}

void	CCAD_ClipPoly::ClipPoints( CAD_FLOAT clipValue, BYTE clipDirect, BYTE cliping )
{
	short	j, m;
	CCAD_Vector	startPt = m_pPt[0];
	CCAD_Vector	insertPt, nextPt;
	BYTE	bStartFlag, bNextFlag;

	for ( j=0; j<m_nNum; j++ ) {
		if ( cliping == CLIP_INF ) {
			if ( clipDirect == CLIP_HORZ ){
				if ( m_pPt[j].m_fY >= clipValue )
					m_pFlag[j] = CLIP_IN;
				else
					m_pFlag[j] = CLIP_OUT;
			}
			else{
				if ( m_pPt[j].m_fX >= clipValue )
					m_pFlag[j] = CLIP_IN;
				else
					m_pFlag[j] = CLIP_OUT;
			}
		}
		else {
			if ( clipDirect == CLIP_HORZ ){
				if ( m_pPt[j].m_fY <= clipValue )
					m_pFlag[j] = CLIP_IN;
				else
					m_pFlag[j] = CLIP_OUT;
			}
			else{
				if ( m_pPt[j].m_fX <= clipValue )
					m_pFlag[j] = CLIP_IN;
				else
					m_pFlag[j] = CLIP_OUT;
			}
		}
	}

	bStartFlag = m_pFlag[0];
	for ( j=0; j<m_nNum; j++ ) {
		if ( j == m_nNum-1 ) {
			m = 0;
			nextPt = startPt;
			bNextFlag = bStartFlag;
		}
		else {
			m = (short)(j+1);
			nextPt = m_pPt[m];
			bNextFlag = m_pFlag[m];
		}
		
		if ( m_pFlag[j] != bNextFlag ) {
			if ( clipDirect == CLIP_HORZ ) {
				if ( m_pPt[j].IsCrossHorizontal(nextPt, clipValue, insertPt.m_fX) )
					insertPt.m_fY = clipValue;
			}
			else {
				if ( m_pPt[j].IsCrossVertical(nextPt, clipValue, insertPt.m_fY) )
					insertPt.m_fX = clipValue;
			}

			if ( m_pFlag[j] == CLIP_OUT )
				m_pPt[j] = insertPt;
			else {
				if ( m == 0 ) {
					m_pPt[m_nNum] = insertPt;
				}
				else {
					memmove( &m_pPt[m+1], &m_pPt[m], sizeof(CCAD_Vector)*(m_nNum-m) );
					memmove( &m_pFlag[m+1], &m_pFlag[m], sizeof(BYTE)*(m_nNum-m) );
					m_pPt[m] = insertPt;
					m_pFlag[m] = CLIP_IN;
				}
				m_nNum++;
				j++;
			}
		}
		else if ( m_pFlag[j] == CLIP_OUT ) {
			if ( m != 0 ) {
				memmove( &m_pPt[j], &m_pPt[m], sizeof(CCAD_Vector)*(m_nNum-m) );
				memmove( &m_pFlag[j], &m_pFlag[m], sizeof(BYTE)*(m_nNum-m) );
			}
			m_nNum--;
			j--;
		}
		if ( m == 0 )
			break;
	}
}

C3DLineArray *
CCAD_ClipPoly::ClippingLine(const SCAD_POINT& sStartPt, const SCAD_POINT& sEndPt)
{
	if( m_LineAry.GetSize() )
		m_LineAry.RemoveAll();
	

	// make Point list ( contain of Cross Point)
	C3DPointArray crossPtAry;

	SCAD_POINT	crossPt;

	crossPtAry.Add(sStartPt);
	crossPtAry.Add(sEndPt);

	CCAD_GeoLine<CAD_FLOAT> line(sStartPt.m_fX, sStartPt.m_fY, sEndPt.m_fX, sEndPt.m_fY);
	int i;
	for(i = 0; i < m_nNum; i ++ )
	{
		CAD_FLOAT x1,x2,y1,y2;
		
		if( i == (m_nNum - 1) ){
			x1 = m_pPt[i].m_fX;
			y1 = m_pPt[i].m_fY;
			x2 = m_pPt[0].m_fX;
			y2 = m_pPt[0].m_fY;

		}else{
			x1 = m_pPt[i].m_fX;
			y1 = m_pPt[i].m_fY;
			x2 = m_pPt[i+1].m_fX;
			y2 = m_pPt[i+1].m_fY;
		}

		if( !line.GetCrossPt(x1, y1, x2, y2, crossPt.m_fX,crossPt.m_fY) )
			continue;
		
		int j; 
		for(j = 1; j < crossPtAry.GetSize(); j ++ )
		{
			CAD_FLOAT fLen1 = (CAD_FLOAT)( fabs(sStartPt.m_fX - crossPtAry[j].m_fX)
										 + fabs(sStartPt.m_fY - crossPtAry[j].m_fY) );

			CAD_FLOAT fLen2 = (CAD_FLOAT)( fabs(sStartPt.m_fX - crossPt.m_fX)
										 + fabs(sStartPt.m_fY - crossPt.m_fY) );
			if( fLen1 >= fLen2 )
				break;
		}

		crossPtAry.InsertAt(j, crossPt);
	}

	// make line information

	SCAD_LINE sLine;

	for(i = 0; i < (crossPtAry.GetSize() - 1); i ++ )
	{
		
		sLine.pt1 = crossPtAry[i];
		sLine.pt2 = crossPtAry[i+1];

		CCAD_Rect rect(sLine.pt1, sLine.pt2);
		CCAD_Point centerPt = rect.CenterPt();

		if( IsPtInsidePolygon(centerPt) )
		{
			m_LineAry.Add(sLine);
		}
	}
	
	return &m_LineAry;
}

BOOL CCAD_ClipPoly::IsPtInsidePolygon(const SCAD_POINT& sPt)
{
	int crossings = 0;
	int yflag0, yflag1, xflag0;
	CCAD_Point	vtx0, vtx1;

	vtx0 = m_pPt[m_nNum-1];
	yflag0 = (vtx0.m_fY >= (int)sPt.m_fY);

	for( int j = 0; j < m_nNum; j++ ){
		vtx1 = m_pPt[j];
		yflag1 = (vtx1.m_fY >= (int)sPt.m_fY);

		if( yflag0 != yflag1)
		{
			xflag0 = (vtx0.m_fX >= (int)sPt.m_fX);

			if( xflag0 == (vtx1.m_fX >= (int)sPt.m_fX) )
			{
				if( xflag0 )
					crossings += (yflag0 ? -1 : 1);
			}else{

				if( (vtx1.m_fX-(vtx1.m_fY - sPt.m_fY)*(vtx0.m_fX-vtx1.m_fX)/(vtx0.m_fY-vtx1.m_fY)) >= sPt.m_fX )
				{
						crossings += (yflag0 ? -1:1);
				}
			}
		}
		yflag0 = yflag1;
		vtx0 = vtx1;
	}
	return (crossings != 0) ? 1 : 0;
}

 
