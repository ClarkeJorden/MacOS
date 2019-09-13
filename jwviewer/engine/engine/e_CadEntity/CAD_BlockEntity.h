#if !defined(AFX_CAD_BLOCKENTITY_H)
#define AFX_CAD_BLOCKENTITY_H

#include "resource.h"

#include "CAD_Pen.h"
#include "CAD_Brush.h"
#include "CAD_Entity.h"
#include "CAD_GemBlock.h"
#include "CAD_DC.h"
#include "CAD_Doc.h"
#include "CAD_PtrArray.h"
class  CCAD_BlockEntity : public CCAD_Entity
{
public:
	CCAD_BlockEntity(CCAD_Doc *, short, short);
	virtual ~CCAD_BlockEntity();

public:
	WORD			m_wPriority;
	CCAD_Pen		m_cPen;
	CCAD_Brush		m_cBrush;
	CCAD_GemBlock	m_cGemBlock;
public:
	virtual CCAD_BlockEntity & operator=(const CCAD_BlockEntity & cEntity);
	virtual	CCAD_GemEntity*		GetGemEntity();
	virtual	BOOL IsIgno(BOOL isBlock);
	virtual CCAD_Pen* GetCadPen(){return &m_cPen;}
	virtual CCAD_Brush* GetCadBrush(){return &m_cBrush;}

	virtual long	ClassSize() { return sizeof(CCAD_BlockEntity); }
	virtual	void	MemCopyTo(CCAD_Entity * pEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3);
	virtual long	GetSize();
	virtual BOOL	Read(CCAD_File& cFile);

	WORD	GetPriority(WORD wState) const;
	void	SetPriority(WORD wState, BOOL blSet = TRUE);
	
};

#endif
