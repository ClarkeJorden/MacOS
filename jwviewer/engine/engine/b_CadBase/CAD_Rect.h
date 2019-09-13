// CCAD_Rect.h: interface for the CCAD_Direct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_RECT_H_)
#define AFX_CAD_RECT_H_

#include	"CAD_Vector.h"

class   CCAD_Rect {
public:
	CAD_FLOAT		m_fMinX;
	CAD_FLOAT		m_fMaxX;
	CAD_FLOAT		m_fMinY;
	CAD_FLOAT		m_fMaxY;
	CAD_FLOAT		m_fMinZ;
	CAD_FLOAT		m_fMaxZ;

public:
	CCAD_Rect();
	CCAD_Rect (const CCAD_Rect & rc);
	CCAD_Rect(const CCAD_Point &, const CAD_FLOAT &);
	CCAD_Rect(const CCAD_Point & cPt1, const CCAD_Point & cPt2);
	CCAD_Rect(LONG left, LONG top, LONG right, LONG bottom);
	CCAD_Rect & operator=(const CCAD_Rect & rc);

	CAD_FLOAT	Width() const		{ return (m_fMaxX - m_fMinX); };
	CAD_FLOAT	Height() const		{ return (m_fMaxY - m_fMinY); };
	CAD_FLOAT	Depth() const		{ return (m_fMaxZ - m_fMinZ); };

	void	Init();
	BOOL	IsInited();
	void	InflateRect(CAD_FLOAT fDeltaX, CAD_FLOAT fDeltaY, CAD_FLOAT fDeltaZ = 0.0f);
	void	SetParameter(const CCAD_Point &, const CAD_FLOAT &);
	void	SetParameter(const CCAD_Point &, const CCAD_Point &);
	void	ResetParameter(const CCAD_Point &);
	void	ResetParameter(float x, float y);
	void	ResetParameter(const CCAD_Rect &);

	CCAD_Point	CenterPt();

									// Z-acis check functions
	BOOL	IsPointInNear(const CCAD_Point &) const;
	BOOL	IsLineInNear(const CCAD_Point &, const CCAD_Point &) const;
	BOOL	ClipLineInNear(CCAD_Point &, CCAD_Point &) const;
	BOOL	IsPointsInNear(CCAD_Point *, short) const;
	void	ClipRayInNear(BYTE, CCAD_Point &, CCAD_Point &) const;

									// 2D check functions
	BOOL	IsCrossLine2D(const CCAD_Point &pt1,const CCAD_Point &pt2);

	BOOL	IsPointInRect2D(const CCAD_Point &) const;
	BOOL	IsLineInRect2D(const CCAD_Point &, const CCAD_Point &) const;
	BOOL	IsPointInRect2D_NE(const CCAD_Point &) const;
	BOOL	IsLineInRect2D_NE(const CCAD_Point &, const CCAD_Point &) const;

	BOOL	IsEnableLineInRect2D(const CCAD_Point &, const CCAD_Point &) const;

	BOOL	ClipLineInRect2D(CCAD_Point &, CCAD_Point &) const;
	BOOL	IsPointsInRect2D(CCAD_Point *, short) const;
	void	ClipRayInRect2D(BYTE, CCAD_Point &, CCAD_Point &) const;
	
	BOOL	IsClipCircle(CCAD_Vector & cCp, CAD_FLOAT fR);
	BOOL	IsClipRect(CCAD_Vector * cRp);
	
	void	Read(CCAD_File &);

protected:
	void	CountPointInRect2D(const CCAD_Point &, CCAD_Point &) const;
};

 

#endif // !defined(AFX_CAD_RECT_H_)
