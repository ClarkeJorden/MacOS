// CAD_GemRadialDim.h: interface for the CCAD_GemRadialDim class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_GEMRADIALDIM_H)
#define AFX_CAD_GEMRADIALDIM_H

#include "CAD_GemDimension.h"

enum _RadialDimOptions
{
	RADIALDIM_RADIUS		= DIMOPTION_ORTHER
};

class  CCAD_GemRadialDim : public CCAD_GemDimension  
{
public:
	// original position parameter
	CCAD_Vector m_ptEnd1;
	CCAD_Vector m_ptEnd2;

public:
	CCAD_GemRadialDim(CCAD_Doc *);
	virtual ~CCAD_GemRadialDim();

public:
	virtual void	MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual BOOL	Read(CCAD_File& cFile);

	void ReDefineCtrlPts();

protected:
	virtual void DrawDimension(BOOL blTmp=false);

private:
	void GetBaseDimLine(CCAD_GemLine& baseDim);

	void GetTextArrayBoundsUnderAlignment(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt,
		CAD_FLOAT& fTxtWidth, CAD_FLOAT& fTxtHeight);

	void CheckInOutState(CCAD_GemLine& baseDim, CAD_FLOAT fTxtWidth, CAD_FLOAT fTxtHeight);

	void DrawDimensionLines(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt);
	void DrawArrowHeaders(CCAD_GemLine& baseDim);

	void DrawTextInsidePlaceBeside(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt);
	void DrawTextOutsidePlaceBeside(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt);
	void DrawTextPlaceNoBeside(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt);
};

#endif
