#if !defined(AFX_CAD_DOC_H)
#define AFX_CAD_DOC_H

#include	"CAD_Sheet.h"
#include	"CAD_Environment.h"
#include	"CAD_LayerMgr.h"
#include	"CAD_PenStyleMgr.h"
#include	"CAD_ImageMgr.h"
#include	"CAD_TextStyleMgr.h"
#include	"CAD_DimStyleMgr.h"
#include	"CAD_MgrEntity.h"
#include	"CAD_MgrBlock.h"
#include	"CAD_File.h"

#include 	"jwcad_doc.h"

#pragma pack(push, 8)

class	CCAD_UdsMgr;
class	CCAD_WorldDoc;
class	CCAD_PaperDoc;

enum spaceID {
	FIRST_PAPER,
	SECOND_PAPER,
	THIRD_PAPER
};

class   CCAD_SpaceDoc  {

public:
	short			m_nID;

	CCAD_SpaceDoc(HANDLE, short);
	virtual	~CCAD_SpaceDoc();

	virtual	CCAD_Sheet			*GetSheet() { return NULL; }

	virtual	CCAD_UdsMgr *	GetUdsMgr() { return NULL; }
	virtual	CCAD_Uds *		GetUds() {return NULL;}

	virtual void		CountWorldRange(CCAD_DC *, CCAD_Rect &, BYTE bFlag = E_DRAWING) const {};
	virtual void		GetDefaultWorldRect(CCAD_DC *, CCAD_Rect &) const {};
	virtual void		CountVcsRange(CCAD_DC *, CCAD_Rect &) const {};
	virtual void		GetDefaultVcsRect(CCAD_DC *, CCAD_Rect &) const {};
	virtual long		CountEntity() const { return 0;}
	virtual	BOOL		IsEntities() { return false; }

};

class   CCAD_MgrDoc  {
public:
	HANDLE				m_hHeap;				// Heap Handle

	CCAD_PenStyleMgr	*m_pPenStyleMgr;		// Common Mgr's
	CCAD_ImageMgr		*m_pImageMgr;
	CCAD_TextStyleMgr	*m_pTextStyleMgr;
	CCAD_DimStyleMgr	*m_pDimStyleMgr;

	CCAD_MgrBlock		*m_pMgrBlock;
	CCAD_MgrBlock		*m_pMgrDimBlock;

public:
	CCAD_MgrDoc();
	virtual ~CCAD_MgrDoc();

	virtual void	RemoveAll();
	virtual CCAD_StyleMgr	*GetStyleMgr(LPARAM lParam);
};


typedef	struct tagSCAD_Inform {
	char		szDesigner[LENGTH_NAME];
	char		szProject[LENGTH_DSCRIPT];
	char		szPaper[LENGTH_DSCRIPT];
	SYSTEMTIME	sTime;
	void		Read(CCAD_File &cFile)
	{
		cFile.ReadString(szDesigner, LENGTH_NAME);
		cFile.ReadString(szProject, LENGTH_DSCRIPT);
		cFile.ReadString(szPaper, LENGTH_DSCRIPT);
		cFile.ReadWord(&sTime.wYear);
		cFile.ReadWord(&sTime.wMonth);
		cFile.ReadWord(&sTime.wDayOfWeek);
		cFile.ReadWord(&sTime.wDay);
		cFile.ReadWord(&sTime.wHour);
		cFile.ReadWord(&sTime.wMinute);
		cFile.ReadWord(&sTime.wSecond);
		cFile.ReadWord(&sTime.wMilliseconds);
	};

} SCAD_Inform;

typedef struct tagBlockRenderColor {
	COLORREF	dwPenColor;
	COLORREF	dwBrushColor;
	BYTE		bPenPriority;
	BYTE		bBrushPriority;
	tagBlockRenderColor()
	{
		bPenPriority = NO;
		bBrushPriority = NO;
	}
} BlockRenderColor;

class   CCAD_Doc : public CCAD_MgrDoc {

private:
	
	CCAD_PtrArray		m_cPaperArray;			// paper doc array
	CCAD_SpaceDoc		*m_pSpaceDoc;			// current doc

public:
	SCAD_Inform			m_sInform;
	CCAD_Environment	m_cEnvironment;

	BlockRenderColor	m_cBRC;
	CCAD_LayerMgr		*m_pLayerMgr;
    
	CCAD_PreViewInfo	m_cInitViewInfo;
	CCAD_PreViewInfo *	GetInitViewInfo() { return &m_cInitViewInfo;}
	
public:
	CCAD_Doc();
	virtual ~CCAD_Doc();

	virtual void	RemoveAll();
	virtual CCAD_StyleMgr	*GetStyleMgr(LPARAM lParam);

	void	InitUds(short n = 0);
	short	Read(CCAD_File & cFile);

	CCAD_SpaceDoc *	GetCurrentDoc() { return m_pSpaceDoc; }
	CCAD_SpaceDoc *	GetSpaceDoc(short nID);
	short			GetCurrentDocID();
	CCAD_SpaceDoc *	CreatePaperDoc();
	void	SetSpaceDoc( CCAD_SpaceDoc * pDoc );
	void			CreateSceneGraph(CCAD_PaperDoc *pPaperDoc);//JW

								// Get SpaceDoc Functions
	CCAD_Sheet			*GetSheet();
	CCAD_UdsMgr			*GetUdsMgr();
	CCAD_Uds*			GetUds();
	short				GetActiveUdsID();
	CCAD_Uds			*GetUds(short nSpace, short nUds);
	CCAD_Grid*			GetGrid();

 	short		GetPenStyle() { return m_cEnvironment.m_cPen.m_nStyle; }
	void		SetPenScale(CAD_FLOAT scale) { m_cEnvironment.m_cPen.m_fScale = scale; }
 
	void		CountWorldRange(CCAD_DC * pDC, CCAD_Rect &, BYTE bFlag = E_DRAWING) const;
	void		CountVcsRange(CCAD_DC * pDC, CCAD_Rect &) const;
	long		CountEntity() const;
	void		GetDefaultVcsRect(CCAD_DC * pDC, CCAD_Rect & cOutRect) const { m_pSpaceDoc->GetDefaultVcsRect(pDC, cOutRect);}

	CCAD_StyleMgr		*GetSpaceStyleMgr(LPARAM lParam);

	BOOL		UpdateDoc(short message, long lParam1 = 0, long lParam2 = 0);

	void		EmptyDoc();

public:
	short	GetSpriteIndex();
	void	SetTerrainImageInfo();
	BOOL	IsAttachEntity(CCAD_Entity* pEntity);
	void	AdjustSpriteDir();
};

typedef struct tagSCAD_EntityInfo {
	short	nSpace;
	short	nUds;
	long	lPos;
	long	lParam;
} SCAD_EntityInfo;

#pragma pack(pop)


#endif
