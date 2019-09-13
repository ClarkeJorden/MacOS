// CAD_ClipPoly.h: interface for the CCAD_ClipPoly class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_CLIPPOLY_H_)
#define AFX_CAD_CLIPPOLY_H_

#include	"CAD_Vector.h"
#include	"CAD_Rect.h"
#include "Kon_Array.h"


typedef struct{
	SCAD_POINT pt1;
	SCAD_POINT pt2;
}SCAD_LINE,*PSCAD_LINE;

typedef CKon_Array<SCAD_LINE, SCAD_LINE> C3DLineArray;
typedef CKon_Array<SCAD_POINT, SCAD_POINT>  C3DPointArray;

#define		CLIP_HORZ		0x00
#define		CLIP_VERT		0x01
#define		CLIP_INF		0x00
#define		CLIP_SUF		0x01
#define		CLIP_IN			0x00
#define		CLIP_OUT		0x01


class   CCAD_ClipPoly {
protected:
	BYTE	*		m_pFlag;
public:
	short			m_nNum;
	CCAD_Point	*	m_pPt;

private:
	C3DLineArray	m_LineAry;

public:
	CCAD_ClipPoly( short nPtNum );
	~CCAD_ClipPoly();

	void	SetPoints( short nNum, SCAD_POINT * pPt );
	void	AddPoints( CCAD_Vector * pPt );
	BOOL	Cliping( const CCAD_Rect & cDrawRc );
	
	C3DLineArray* ClippingLine(const SCAD_POINT& sPt1, const SCAD_POINT& sPt2);
	BOOL	IsPtInsidePolygon(const SCAD_POINT& sPt);

protected:
	void	ClipPoints( CAD_FLOAT clipValue, BYTE clipDirect, BYTE cliping );
};

 

#endif 
