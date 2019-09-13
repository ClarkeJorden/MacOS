// CAD_GemLinearDim.h: interface for the CCAD_GemLinearDim class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_GEMLINEARDIM_H)
#define AFX_CAD_GEMLINEARDIM_H

#include "CAD_GemDimension.h"

enum _LinearDimOptions
{
	LINEARDIM_HORZ			= DIMOPTION_ORTHER,
	LINEARDIM_ALIGNED		= LINEARDIM_HORZ << 1,
	LINEARDIM_OBLIQUE		= LINEARDIM_ALIGNED << 1
};

class  CCAD_GemLinearDim : public CCAD_GemDimension  
{
public:
	// original position parameter
	CCAD_Vector m_ptExt1;
	CCAD_Vector m_ptExt2;
	CCAD_Vector m_ptDimLine;

	CCAD_Angle m_anglRotate;
	CCAD_Angle m_anglOblique;

public:
	CCAD_GemLinearDim(CCAD_Doc* pDoc);
	virtual ~CCAD_GemLinearDim();

public:
	virtual	void Init();
	virtual void MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual BOOL Read(CCAD_File& cFile);

	void ReDefineCtrlPts();

	virtual void GetBaseCouninueDim(BOOL bContinue, CCAD_GemDimension* pGetDim, const CCAD_Vector& ptExt2);

protected:
	virtual void DrawDimension(BOOL blTmp=false);

private:
	void GetBaseDimLine(CCAD_GemLine& baseDim);

	void GetTextArrayBoundsUnderAlignment(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt,
		CAD_FLOAT& fTxtWidth, CAD_FLOAT& fTxtHeight);

	void CheckInOutState(CCAD_GemLine& baseDim, CAD_FLOAT fTxtWidth, CAD_FLOAT fTxtHeight);

	void DrawExtensionLines(CCAD_GemLine& baseDim);
	void DrawDimensionLines(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt);
	void DrawArrowHeaders(CCAD_GemLine& baseDim);

	void DrawTextInsidePlace(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt);
	void DrawTextOutsidePlaceBeside(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt);
	void DrawTextOutsidePlaceNoBeside(CCAD_GemLine& baseDim, CCAD_Coord3D* pTxtCoord, const CCAD_Rect& boundTxt);
};

#endif
