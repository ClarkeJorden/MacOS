
#if !defined(AFX_CAD_GEOLINE_H_)
#define AFX_CAD_GEOLINE_H_

template <class T>
class   CCAD_GeoLine  
{
public:
	T	m_tX1;
	T	m_tY1;
	T	m_tX2;
	T	m_tY2;

	// 2DLine Normal Vector
	T	m_tA;
	T	m_tB;
	// 2DLine Equality Value
	T	m_tC;

	// Rect Infomation
	T	m_tMinX;
	T	m_tMinY;
	T	m_tMaxX;
	T	m_tMaxY;


public:
	CCAD_GeoLine(void)
	{
		m_tX1 = (T)0;
		m_tY1 = (T)0;
		m_tX2 = (T)0;
		m_tY2 = (T)0;

		m_tMinX = (T)0;
		m_tMinY = (T)0;
		m_tMaxX = (T)0;
		m_tMaxY = (T)0;
	}

	CCAD_GeoLine(T x1, T y1, T x2, T y2)
	{
		Set(x1, y1, x2, y2);
	}

	virtual ~CCAD_GeoLine(void){};

	void	Set(T x1, T y1, T x2, T y2)
	{
		m_tX1 = x1;
		m_tY1 = y1;
		m_tX2 = x2;
		m_tY2 = y2;


		m_tMinX = _min(x1,x2);
		m_tMinY = _min(y1,y2);
		m_tMaxX = _max(x1,x2);
		m_tMaxY = _max(y1,y2);

		ComputeABC();
	};

	BOOL	IsCross(T x1, T y1, T x2, T y2)
	{
		
		// Bound Check
		if (x1 <= m_tMinX && x2 <= m_tMinX)
			return false;

		if (x1 >= m_tMaxX && x2 >= m_tMaxX)
			return false;

		if (y1 <= m_tMinY && y2 <= m_tMinY)
			return false;	

		if (y1 >= m_tMaxY && y2 >= m_tMaxY)
			return false;
		
		// Check Cross
		CCAD_GeoLine<T> other(x1,y1,x2,y2);
		return IsCross(other);

	};

	BOOL	IsCross( const CCAD_GeoLine& other)
	{

		int nDirect = 0;

		if( (m_tA * other.m_tX1 + m_tB * other.m_tY1 + m_tC) > (T)0 )
			nDirect ++;

		if( (m_tA * other.m_tX2 + m_tB * other.m_tY2 + m_tC) > (T)0 )
			nDirect ++;

		if( nDirect == 0 || nDirect == 2 ) 
			return false;

		nDirect = 0;

		if( (other.m_tA * m_tX1 + other.m_tB * m_tY1 + other.m_tC) > (T)0 )
			nDirect ++;

		if( (other.m_tA * m_tX2 + other.m_tB * m_tY2 + other.m_tC) > (T)0 )
			nDirect ++;

		if( nDirect == 0 || nDirect == 2 ) 
			return false;

		return true;
	};
	
	BOOL	GetCrossPt(T x1, T y1, T x2, T y2, T& crossX, T& crossY)
	{
		if( !IsCross(x1, y1, x2, y2) )
			return false;

	// ----- computing Cross Point -------
		T A1, B1, C1, A2, B2, C2;
		A1 = m_tA; B1 = m_tB; C1 = m_tC;
		
		// computing A,B,C of line(x1,y1,x2,y2)
		A2 = - (y2 - y1);
		B2 = x2 - x1;
		C2 = - A2 * x1 - B2 * y1;

		crossX = (-C1*B2 + C2*B1) / (A1*B2 - A2*B1);
		crossY = (-A1*C2 + A2*C1) / (A1*B2 - A2*B1);

	// -------------------------------------
		return true;
	};

private:
	void	ComputeABC()
	{
		
		m_tA = - (m_tY2 - m_tY1);
		m_tB = m_tX2 - m_tX1;
		m_tC = - m_tA * m_tX1 - m_tB * m_tY1;
	
	};

};

 

#endif // !defined(AFX_CAD_GEOLINE_H_)
