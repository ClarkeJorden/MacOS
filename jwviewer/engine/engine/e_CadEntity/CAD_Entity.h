#if !defined(AFX_CAD_ENTITY_H)
#define AFX_CAD_ENTITY_H

#include "CAD_GemEntity.h"
#include "CAD_DC.h"

class CCAD_Coord3D;
class CCAD_Brush;
class CCAD_Pen;
class CCAD_Uds;
class CCAD_Doc;
class CCAD_MgrDoc;
class CCAD_SpaceDoc;
class CCAD_MgrEntity;

class   CCAD_Entity
{
public:
	CCAD_Entity(CCAD_Doc *, short, short);
	CCAD_Entity() {};
	virtual ~CCAD_Entity();
	virtual CCAD_Entity & operator=(const CCAD_Entity & cEntity);

public:
	BYTE		m_bCode;
	BYTE		m_bFlag;
	short		m_nLayer;
	CCAD_Doc *	m_pDoc;
	short		m_nSpaceDoc;
	short		m_nUds;

	CCAD_Rect m_cWorldBound;
	int     m_nIndex;

	CAD_FLOAT m_fMaxBoundSize;

public:

	virtual	CCAD_GemEntity *GetGemEntity(){return NULL;}

	virtual	BOOL	IsVisible(BYTE bFlag = E_DRAWING);
	virtual	BOOL	IsIgno(BOOL isBlock);
	
	virtual CCAD_Pen*	GetCadPen(){return NULL;}
	virtual CCAD_Brush* GetCadBrush(){return NULL;}
	CCAD_MgrEntity *	GetMgrEntity();
	
	virtual	BOOL	Draw(CCAD_DC* pCadDC, BOOL blRedraw = FALSE);

	virtual long	ClassSize() { return sizeof(CCAD_Entity); }
	virtual long	GetSize() { return 0; }

	virtual LPSTR	GetData1(LPSTR) {return NULL;}
	virtual LPSTR	GetData2(LPSTR) {return NULL;}
	virtual LPSTR	GetData3(LPSTR) {return NULL;}
	virtual BOOL	Read(CCAD_File & cFile);

	virtual void	MemCopyTo(CCAD_Entity *, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);

	void		SetBlockEntityPara(const CCAD_MgrEntity *);
	void		InitEntity(CCAD_Entity * pEntity = NULL);
	void		Remove();
	BYTE		GetState(){return m_bFlag;}
	void		SetState(BYTE bFlag) {m_bFlag = bFlag;}
	BOOL		IsDeleted(){return (m_bFlag == E_DELETE);}
	BOOL		GetLogPen(CCAD_DC *, LOGPEN &, BYTE bState = E_DRAWING);
	BOOL		GetLogBrush(CCAD_DC *, LOGBRUSH &, BYTE bState = E_DRAWING);
	void		CopyEntityTo(CCAD_Entity *pEntity);

	BOOL		Is3DEntity();
	BOOL		IsClosed();
	BOOL		IsThick();
	void		CountBodyRange(CCAD_Rect& cRange);
	void		CountVcsRange(CCAD_DC* pCadDC, CCAD_Rect& cRange);
	BOOL		GetUdsRange(CCAD_Coord3D* pUCS, CCAD_Rect& cRange);

	void			ConvToBase(const CCAD_Coord3D &cBcs, EXP_MESH *spMesh);
	BOOL			CalcTexCoord(SCAD_POINT* spStartPoint,	SCAD_POINT*	spEndPoint,
										short	nNumVertex,	SCAD_POINT*	spVertex,
										CAD_FLOAT	fSizeTexWidth, CAD_FLOAT fSizeTexHeight,
										SCAD_POINT*	spTexCoord);
	BOOL			CalcNormal(const SCAD_POINT	sVertex[3], SCAD_POINT *n);

	BOOL			IsBlockEntity();
	BOOL			IsDimensionEntity();

	void			SetMaxBoundSize();
	
protected:
	CCAD_PenStyle *	SelectPenStyle(CCAD_DC *pCadDC);	
};

#endif
