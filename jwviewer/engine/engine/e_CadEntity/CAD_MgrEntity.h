#if !defined(AFX_CAD_MGRENTITY_H)
#define AFX_CAD_MGRENTITY_H

#include "CAD_DC.h"
#include "CAD_Entity.h"

class	CCAD_Doc;
class	CCAD_SpaceDoc;
class	CCAD_Uds;
class	CCAD_MgrGroup;
class	CCAD_TextureMgr;
class	CCAD_ImageMgr;

class	CData;
class	CJWCAD_Doc;

#define		DOC_HEAP_SIZE		131072			// 1024 * 128
#define		BLOCK_HEAP_SIZE		2048			// 1024 * 2

typedef struct HEAPHEAD {
	int			lSize;
	int 			lSaved;
	HEAPHEAD	*	pNext;
} HEAPHEAD;

//-----------------------------------------
class   CCAD_MgrEntity {
public:
	long		m_lHeapSize;
	CCAD_PtrArray	m_cArray;	// pointer array
	HANDLE		m_hHeap;
	HEAPHEAD *	m_pHead;

	CCAD_Doc *	m_pDoc;			// space doc number
	short		m_nSpaceDoc;	// space doc number
	short		m_nUds;			// Uds Number
	BYTE *m_pDrawState;

public:
	CCAD_MgrEntity(HANDLE hHeap, CCAD_Doc*, short, short, BOOL);
	~CCAD_MgrEntity();

protected:
	HEAPHEAD	*	AllocHeap(long);
	CCAD_Entity *	AddToHeap(long);

public:
	CCAD_Entity *	GetFirstEntity(long & pos) const;
	CCAD_Entity *	GetEndEntity(long & pos) const;
	CCAD_Entity *	GetNextEntity(long & pos) const;
	CCAD_Entity	*	GetPrevEntity(long & pos) const;
	long			GetPosition(CCAD_Entity * pEntity) const;
	CCAD_Entity *	GetEntity(long pos) const;
	CCAD_Entity *	NewEntity(BYTE bCode, BOOL isBlock = false) const;
	CCAD_Entity *	NewComEntity(BYTE bCode) const;

	void		RemoveAll();
	void		CopyFromOther(const CCAD_MgrEntity &);
	long		AddEntity(CCAD_Entity * pEntity);
	long		CountEntity(BYTE bFlag = E_DRAWING);
	
	BOOL		Read(CCAD_File &, BOOL isBlock = false);
	
	BOOL		DrawEntity(CCAD_DC * pDC, BOOL blRedraw = FALSE, BOOL blViewport = FALSE);
	void		CountWorldRange(CCAD_DC * pDC, CCAD_Rect &, BYTE bFlag = E_DRAWING) const;
	void		CountVcsRange(CCAD_DC * pDC, CCAD_Rect &) const;
	long		CountBlockSize(BYTE bFlag) const;
	
	BOOL		IsEntities();
	void		QuickSort_BoundSize(CCAD_PtrArray & entityAry, int nStart, int nEnd);
};

 

#endif
