// CAD_Vector.h: interface for the CCAD_Vector class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CAD_VECTOR_H_
#define _CAD_VECTOR_H_

#include "CAD_Constance.h"
#include "CAD_Angle.h"
 
typedef struct{
	CAD_FLOAT		m_fX;				
	CAD_FLOAT		m_fY;				
	CAD_FLOAT		m_fZ;				
	void Init(const CAD_FLOAT & fX = 0, const CAD_FLOAT & fY = 0, const CAD_FLOAT & fZ = 0)
	{
		m_fX = fX;
		m_fY = fY;
		m_fZ = fZ;
	}
	void	Read(CCAD_File &cFile)
	{
		cFile.ReadCAD_Float(&m_fX);
		cFile.ReadCAD_Float(&m_fY);
		cFile.ReadCAD_Float(&m_fZ);
	};

}SCAD_POINT,*pSCAD_POINT;

class   CCAD_Vector : public SCAD_POINT
{
public:
	CCAD_Vector();
	CCAD_Vector(const CAD_FLOAT& x, const CAD_FLOAT& y, const CAD_FLOAT& z);
	CCAD_Vector (const SCAD_POINT& inPt);
	CCAD_Vector (const SCAD_POINT& startPt,const SCAD_POINT& endPt);


	CCAD_Vector & operator=(const CCAD_Vector& inPt);
	CCAD_Vector & operator=(const SCAD_POINT& inPt);

	void	Normalize();

	void	RotateLeft();
	void	RotateRight();

	void	CountAngle2D(const CCAD_Vector & inPt, CCAD_Angle & outAngle) const;
	BYTE	CountSide2D(const CCAD_Vector & inPt, const CCAD_Angle & inAngle) const;
	void	CountVector2D(CCAD_Vector & outPt, const CAD_FLOAT& fR, const CAD_FLOAT & dSinA, const CAD_FLOAT & fCosA) const;

	void	Move(const CCAD_Vector & inPt);
	void	Rotate(const CCAD_Vector & inPt, const CAD_FLOAT & dSinA, const CAD_FLOAT & dCosA);
	void	Mirror(const CCAD_Vector & inPt, const CAD_FLOAT & dSinA, const CAD_FLOAT & dCosA);
	void	Scale(const CCAD_Vector & inPt, const CAD_FLOAT & dScale);

	BOOL		Equal(const CCAD_Vector & inPt, const CAD_FLOAT & dAccrancy = DEFAULT_ACCURANCE) const;
	BOOL		EqualX(const CCAD_Vector & inPt, const CAD_FLOAT & dAccrancy = DEFAULT_ACCURANCE) const;
	BOOL		EqualY(const CCAD_Vector & inPt, const CAD_FLOAT & dAccrancy = DEFAULT_ACCURANCE) const;
	BOOL		EqualZ(const CCAD_Vector & inPt, const CAD_FLOAT & dAccrancy = DEFAULT_ACCURANCE) const;
	BOOL		IsCrossHorizontal(const CCAD_Vector cPt, const CAD_FLOAT & dHrztY, CAD_FLOAT & dGetX) const;
	BOOL		IsCrossVertical(const CCAD_Vector cPt, const CAD_FLOAT & dVrtX, CAD_FLOAT & dGetY) const;
	BOOL		IsParallel(const CCAD_Vector & inPt) const;
	CAD_FLOAT	CountLength(const CCAD_Vector & inPt) const;
	double		Abs() const;
	void	Read(CCAD_File & cFile);
	void	Write(CCAD_File & cFile);
	
//////// operator ////////////
	CCAD_Vector operator * (const CCAD_Vector& cOther) const; // Vector Product
	void		operator *= (const CCAD_Vector& cOther); // Vector Product
	CAD_FLOAT	operator & (const CCAD_Vector& cOther) const; // scalar Product

	CCAD_Vector operator - (const CCAD_Vector& cOther) const;	// Vector Differance
	void		operator -= (const CCAD_Vector& cOther);		// Vector Differance
	CCAD_Vector operator + (const CCAD_Vector& cOther)  const;	// Vector Differance
	void		operator += (const CCAD_Vector& cOther);		// Vector Differance

	// constant multipule
	CCAD_Vector operator * (CAD_FLOAT fConst) const;
	CCAD_Vector operator * (int nConst) const;
	void		operator *= (CAD_FLOAT fConst);
	void		operator *= (int nConst);
	BOOL		operator == (const CCAD_Vector& cOther);
	BOOL		operator != (const CCAD_Vector& cOther);
};

#define CCAD_Point CCAD_Vector 


//--------------------------------------------------------
typedef struct _tagSCAD_MESH_4
{
	SCAD_POINT	m_sVertex[4];
	SCAD_POINT	m_sNVectVertex[4];
	SCAD_POINT	m_sNVectMesh;
	SCAD_POINT	m_sTexCoord[4];
	
	_tagSCAD_MESH_4()
	{
		memset(m_sVertex, 0, 4 * sizeof(SCAD_POINT));
		memset(m_sNVectVertex, 0, 4 * sizeof(SCAD_POINT));
		memset(&m_sNVectMesh, 0, sizeof(SCAD_POINT));
		memset(m_sTexCoord, 0, 4 * sizeof(SCAD_POINT));
	}
} SCAD_MESH_4;

//--------------------------------------------------------
typedef struct _tagSCAD_MESH_3
{
	SCAD_POINT	m_sVertex[3];
	SCAD_POINT	m_sNVectVertex[3];
	SCAD_POINT	m_sNVectMesh;
	SCAD_POINT	m_sTexCoord[3];
	
	_tagSCAD_MESH_3()
	{
		memset(m_sVertex, 0, 3 * sizeof(SCAD_POINT));
		memset(m_sNVectVertex, 0, 3 * sizeof(SCAD_POINT));
		memset(&m_sNVectMesh, 0, sizeof(SCAD_POINT));
		memset(m_sTexCoord, 0, 3 * sizeof(SCAD_POINT));
	}
} SCAD_MESH_3;

typedef struct _tagSCAD_VERTEX
{
	SCAD_POINT	m_sVertex;
	SCAD_POINT	m_sNVectVertex;
	SCAD_POINT	m_sTexCoord;
} SCAD_VERTEX;

//--------------------------------
#define	MESH_DIVNUM	18
#define MESH_TYPE_3			0x00
#define MESH_TYPE_4			0x01
#define MESH_TYPE_SPRITE	0x02

#define MESH_TYPE_BLEND		0x10
#define MESH_TYPE_AXISROT	0x20
#define MESH_TYPE_POINTROT	0x40

typedef struct _tagMaterial
{
	COLORREF	ambient;
	COLORREF	diffuse;
	COLORREF	specular;
	COLORREF	emissive;
	BYTE		alpha;

	BYTE		m_bReflect;
	BYTE		m_bRefract;
	BYTE		m_bEmissiveEnable;
	CAD_FLOAT	m_fKa;
	CAD_FLOAT	m_fKd;
	CAD_FLOAT	m_fKs;
	CAD_FLOAT	m_fKt;
	CAD_FLOAT	m_fKrfr;
	short		m_Ns;
	CAD_FLOAT	m_fBlend;
	CAD_FLOAT	m_fKe;
} Material, *LPMaterial;

typedef struct _tagEXP_MESH
{
	COLORREF		color;
	Material		material;
	short			textureIndex;
	BOOL			culling;
	
	BYTE			typeMesh;
	SCAD_MESH_4		sMesh;
	_tagEXP_MESH()
	{
		color = RGB(0xFF, 0xFF, 0xFF);
		memset(&material, 0, sizeof(Material));
		material.alpha = 255;
		material.m_bReflect = NO;
		material.m_bRefract = NO;
		material.m_bEmissiveEnable = NO;
		material.m_fKa = 0.1f;
		material.m_fKd = 0.3f;
		material.m_fKs = 0.2f;
		material.m_fKt = 0.0f;
		material.m_fKrfr = 0.3f;
		material.m_Ns = 3;
		material.m_fBlend = 0.0f;
		material.m_fKe = 0.0f;

		textureIndex = -1;
		culling = FALSE;
		
		typeMesh = MESH_TYPE_4;
		memset(&sMesh, 0, sizeof(SCAD_MESH_4));
	}
} EXP_MESH, *LPEXP_MESH;

#endif // !defined(AFX_CAD_VECTOR_H_)
