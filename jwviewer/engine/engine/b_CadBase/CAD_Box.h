#ifndef CAD_BOX_H
#define CAD_BOX_H

#pragma once
#include "Define_Type.h"
#include "CAD_Vector.h"
#include "CAD_Rect.h"

class CCAD_Vector;
class CCAD_File;
class  CCAD_Box
{
public:
	CAD_FLOAT m_fMinX;
	CAD_FLOAT m_fMaxX;
	CAD_FLOAT m_fMinY;
	CAD_FLOAT m_fMaxY;
	CAD_FLOAT m_fMinZ;
	CAD_FLOAT m_fMaxZ;

public:
	void InitZero();
	CCAD_Box();
	CCAD_Box (const CCAD_Box & rc);
	CCAD_Box (const RECT &rc);
	CCAD_Box(const SCAD_POINT &, const CAD_FLOAT &);
	CCAD_Box(const SCAD_POINT & cPt1, const SCAD_POINT & cPt2);
	CCAD_Box & operator = (const CCAD_Box & rc);
	BOOL operator == (const CCAD_Box & other) const;
	BOOL operator != (const CCAD_Box & other) const;

	CAD_FLOAT	Width() const		{ return (m_fMaxX - m_fMinX); };
	CAD_FLOAT	Height() const		{ return (m_fMaxY - m_fMinY); };
	CAD_FLOAT	Depth() const		{ return (m_fMaxZ - m_fMinZ); };

	void	Init();
	BOOL IsInited() const;
	BOOL IsEmpty_XY() const { return IS_ZERO(Width() * Height()); }

	void	SetParameter(const SCAD_POINT & cPt, CAD_FLOAT dR);
	void	SetParameter(const SCAD_POINT &, const SCAD_POINT &);
	void	ResetParameter(const SCAD_POINT &);
	void	ResetParameter(const CCAD_Box &);

	SCAD_POINT CenterPt() const;

	// Z-Axis check functions
	BOOL	IsPointInNear(const SCAD_POINT &) const;
	BOOL	ClipLineInNear(SCAD_POINT &, SCAD_POINT &) const;

	// 2D check functions
	BOOL	IsCross_LineSeg_Range_XY(const SCAD_POINT &pt1,const SCAD_POINT &pt2);

	BOOL	IsPointInRect2D(const SCAD_POINT &) const;
	BOOL	IsLineInRect2D(const SCAD_POINT &, const SCAD_POINT &) const;
	BOOL	IsPointInRect2D_NE(const SCAD_POINT &) const;
	BOOL	IsLineInRect2D_NE(const SCAD_POINT &, const SCAD_POINT &) const;
	BOOL	IsCorssWithBox(CCAD_Box &cBox);

	BOOL	IsCrossable_LineSeg_Range_XY(const SCAD_POINT & pt1, const SCAD_POINT & pt2) const;
	BOOL	Clip_LineSeg_Range_XY(SCAD_POINT & pt1, SCAD_POINT & pt2) const;

	BOOL	IsPointsInRect2D(SCAD_POINT *, short) const;
	void	ClipRayInRect2D(BYTE, SCAD_POINT &, SCAD_POINT &) const;

	BOOL	IsClipCircle(CCAD_Vector & cCp, CAD_FLOAT fR);
	BOOL	IsClipRect(CCAD_Vector * cRp);

	void	Write(CCAD_File & file);
	void	Read(CCAD_File &);

protected:
	void CountPointInRect2D(const SCAD_POINT &, SCAD_POINT &) const;
};

#endif
