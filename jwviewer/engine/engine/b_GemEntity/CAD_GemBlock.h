#if !defined(AFX_CAD_GEMBLOCK_H)
#define AFX_CAD_GEMBLOCK_H

#include	"CAD_GemEntity.h"
#include	"CAD_Block.h"

class  CCAD_GemBlock : public CCAD_Gem3DEntity {
public:
	CCAD_GemBlock(CCAD_Doc * pDoc);
	virtual ~CCAD_GemBlock();

public:
	char		m_szName[EXT_LENGTH_NAME];
	CCAD_Doc	*m_pDoc;
    DWORD		m_nNumber;  // for JW read

public:
	virtual	void	Init();
	virtual void	MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1= NULL, LPSTR pData2= NULL, LPSTR pData3= NULL);

	virtual BOOL	Read(CCAD_File& cFile);

	virtual BOOL	Draw(CCAD_DC *pCadDC, BOOL bRedraw = FALSE);
	virtual void	CountBodyRange(CCAD_Rect& cRange);
	virtual void	CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange);
	virtual BOOL	IsRender();


	virtual CCAD_Block *	GetBlock();
};

#endif
