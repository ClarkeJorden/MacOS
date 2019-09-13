#ifndef InstBaseStruct_H
#define InstBaseStruct_H

//#include "math.h"
#include "CAD_Constance.h"
#include <math.h>
 

class CCAD_DC;
#ifndef FLT_MAX
#define FLT_MAX		MAX_DOUBLE
#endif

class FLOATINGPOINT
{
public:
	float			x;
	float			y;

	FLOATINGPOINT()
		{
			x = 0.0f;
			y = 0.0f;
		}
	FLOATINGPOINT(const FLOATINGPOINT& inPoint)
		{
			x = inPoint.x;
			y = inPoint.y;
		}
	FLOATINGPOINT(float inX, float inY)
		{
			x = inX;
			y = inY;
		}
	void SetPoint(const FLOATINGPOINT& inPoint, BOOL isShift = false)
		{
			x = inPoint.x;
			y = inPoint.y;
			if (isShift) {
				float	fx = (float)fabs(x);
				float	fy = (float)fabs(y);
				if( fx < fy ) {
					if( fx*2 < fy )
						x = 0;
					else 
						x = (x>0) ? fy : -fy;
				}
				else {
					if (fy*2 < fx)
						y = 0;
					else
						y = (y>0) ? fx : -fx;
				}
			}
		}
	void SetPoint(const FLOATINGPOINT& inPoint, const FLOATINGPOINT& bp, BOOL isShift = false)
		{
			x = inPoint.x;
			y = inPoint.y;
			if (isShift) {
				float	fx = (float)fabs(bp.x-x);
				float	fy = (float)fabs(bp.y-y);
				if( fx < fy ) {
					if( fx*2 < fy )
						x = bp.x;
					else 
						x = (x>bp.x) ? bp.x+fy : bp.x-fy;
				}
				else {
					if (fy*2 < fx)
						y = bp.y;
					else
						y = (y>bp.y) ? bp.y+fx : bp.y-fx;
				}
			}
		}
	void SetPoint(float inX, float inY)
		{
			x = inX;
			y = inY;
		}
	void operator = (const FLOATINGPOINT& inPoint)
		{
			x = inPoint.x;
			y = inPoint.y;
		}
	FLOATINGPOINT operator - (const FLOATINGPOINT& inPoint) const
		{
			return FLOATINGPOINT(x - inPoint.x, y - inPoint.y);
		}
	void operator -= (const FLOATINGPOINT& inPoint)
		{
			x -= inPoint.x;	y -= inPoint.y;
		}
	FLOATINGPOINT operator + (const FLOATINGPOINT& inPoint) const
		{
			return FLOATINGPOINT(x + inPoint.x, y + inPoint.y);
		}
	void operator += (const FLOATINGPOINT& inPoint)
		{
			x += inPoint.x;	y += inPoint.y;
		}
	FLOATINGPOINT operator * (float scale) const
		{
			return FLOATINGPOINT(x * scale, y * scale);
		}
	void operator *= (float scale)
		{
			x *= scale;
			y *= scale;
		}
	FLOATINGPOINT operator / (float scale) const
		{
			return FLOATINGPOINT(x / scale, y / scale);
		}
	void operator /= (float scale)
		{
			x /= scale;
			y /= scale;
		}
	FLOATINGPOINT operator - () const
		{
			return FLOATINGPOINT(-x, -y);
		}
	float	CountLength(FLOATINGPOINT & cPt)
		{
			float	d1 = x - cPt.x;
			float	d2 = y - cPt.y;
			d1 = (d1 * d1 + d2 * d2);
			d1 = (float)sqrt(d1);
			return d1;
		}
};



#endif	// InstBaseStruct_H
