#if !defined(AFX_CAD_BLOCK_H)
#define AFX_CAD_BLOCK_H

#include	"CAD_Style.h"
#include	"CAD_MgrEntity.h"

class	CCAD_Doc;
typedef struct	tagOBJECT{
	char		szName[LENGTH_NAME];
	char		szPart[LENGTH_DSCRIPT];
	char		szDescript[LENGTH_DSCRIPT];
} OBJECT;

class CCAD_Block : public CCAD_Style {
public:
	char		m_szPart[LENGTH_DSCRIPT];
	char		m_szDescript[LENGTH_DSCRIPT];
	CCAD_Rect	m_cRc;
	int		m_lSize;

	CCAD_MgrEntity	m_cMgrEntity;

public:
	CCAD_Block(HANDLE, CCAD_Doc *);
	virtual ~CCAD_Block();

	virtual void	Init();
	virtual void	CopyFrom(const CCAD_Style *pStyle);
	virtual BOOL	Read(CCAD_File &cFile);

	BOOL	IsIncludeEqualBlock(LPCSTR name);
};

#endif
