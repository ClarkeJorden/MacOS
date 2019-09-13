#if !defined(AFX_CAD_GEMANGULARDIM_H)
#define AFX_CAD_GEMANGULARDIM_H

#include "CAD_GemDimension.h"

enum _AngularDimOptions
{
	ANGULARDIM_QUADDIVIDE	= DIMOPTION_ORTHER
};

class  CCAD_GemAngularDim : public CCAD_GemDimension  
{
public:
	// original position parameter
	CCAD_Vector m_ptVertex;
	CCAD_Vector m_ptFirst;
	CCAD_Vector m_ptSecond;
	CCAD_Vector m_ptDimArc;

public:
	CCAD_GemAngularDim(CCAD_Doc *);
	virtual ~CCAD_GemAngularDim();

	virtual void MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual BOOL Read(CCAD_File& cFile);

	void ReDefineCtrlPts();

	virtual void GetBaseCouninueDim(BOOL bContinue, CCAD_GemDimension* pGetDim, const CCAD_Vector& ptExt2);

protected:
	virtual void DrawDimension(BOOL blTmp=false);

private:
	void GetBaseDimArc(CCAD_GemArc& baseDim);

	void CheckInOutState(CCAD_GemArc& baseDim, const CCAD_Rect& boundTxt);
	BOOL PtInBaseDimArc(CCAD_Vector pt, CCAD_GemArc& baseDim);

	void GetTextArrayBoundsUnderAlignment(CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt,
		CAD_FLOAT& fTxtWidth, CAD_FLOAT& fTxtHeight);
	void GetTextDirect(CCAD_Coord3D* pTxtCoord);

	void DrawExtensionLines(CCAD_GemArc& baseDim);
	void DrawDimensionLines(CCAD_GemArc& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt);
	void DrawArrowHeaders(CCAD_GemArc& baseDim);

	void DrawTextInsidePlace(CCAD_GemArc& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt);
	void DrawTextOutsidePlaceBeside(CCAD_GemArc& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt);
	void DrawTextOutsidePlaceNoBeside(CCAD_GemArc& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt);
};

#endif
