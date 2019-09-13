#if !defined(AFX_CAD_GEMLEADERDIM_H)
#define AFX_CAD_GEMLEADERDIM_H

#include "CAD_GemDimension.h"

class  CCAD_GemLeaderDim : public CCAD_GemDimension
{
public:
	// original position parameter
	SCAD_LEADERSETTING	m_sSetting;
	short				m_nPtCount;
	CCAD_Vector *		m_pVertex;

public:
	CCAD_GemLeaderDim(CCAD_Doc *);
	virtual ~CCAD_GemLeaderDim();

public:
	virtual	void	Init();
	virtual void	MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual void	Remove();
	virtual BOOL	Read(CCAD_File& cFile);


protected:
	virtual void DrawDimension(BOOL blTmp=false);

private:
};

#endif
