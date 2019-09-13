// CAD_Uds.h: interface for the CCAD_Uds class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_UDS_H)
#define AFX_CAD_UDS_H


#include	"CAD_Style.h"
#include	"CAD_Grid.h"
#include	"CAD_Coord3D.h"
#include	"CAD_Doc.h"
#include	"CAD_MgrEntity.h"
#include    "SceneGraph.h"
 
#define		UDS_POLAR			0x0100
#define		UDS_RELATIVE		0x0200
#define		UDS_ORTHO			0x0400
#define		UDS_CURSORMARK		0x0800

#define		UCS_PLAN_XY	0x0000
#define		UCS_PLAN_YZ	0x0001
#define		UCS_PLAN_ZX	0x0002

class    CCAD_Uds : public CCAD_Style {
public:
	WORD			m_wState;
	CCAD_Grid		m_cGrid;
	CAD_FLOAT		m_fWorkHeight;

	CCAD_Coord3D	m_cCoord3D;
	WORD			m_wCrntPlan;

	CCAD_MgrEntity *m_pMgrEntity;
	CSceneGraph *m_pSceneGraph;

	HANDLE			m_hHeap;
	CCAD_Doc	*	m_pDoc;
	short			m_nSpaceDoc;
	short			m_nID;

public:
	CCAD_Uds(CCAD_Doc*, HANDLE, short, short);
	CCAD_Uds(const CCAD_Uds *pStyle);
	virtual	~CCAD_Uds();

	CCAD_Uds & operator =(const CCAD_Uds &);

	WORD	GetState(WORD wState) const;
	void	SetState(WORD wState, BOOL blSet = TRUE);

    void			SetPlan(WORD wNewPlan);
	void			GetBaseUdsCoord(CCAD_Coord3D & cCoord);
	void			ConvToBase(SCAD_POINT& sOldPt);
	void			ConvFromBase(SCAD_POINT& sOldPt);
	BOOL		GetCursorPos(CCAD_DC * pDC, const POINT &sLP, CCAD_Point* pPtUCS);
	BOOL		GetCrossPt(const CCAD_Point& inPt1,  const CCAD_Point& inPt2,  CCAD_Point& outPt);

	void			DrawAxisOfUCS(CCAD_DC * pDC);
	BOOL			DrawEntityMgr(CCAD_DC * pDC, BOOL blRedraw = FALSE);

	virtual void	Init();
	virtual void	CopyFrom(const CCAD_Style *pStyle);
	virtual BOOL	Read(CCAD_File &cFile);
};

 

#endif
