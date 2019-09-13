#if !defined(AFX_CAD_GEMDIMENSION_H)
#define AFX_CAD_GEMDIMENSION_H

#include "CAD_DimStyle.h"
#include "CAD_Entity.h"

#include "CAD_Uds.h"

#include "CAD_PointEntity.h"
#include "CAD_LineEntity.h"
#include "CAD_CircleEntity.h"
#include "CAD_ArcEntity.h"
#include "CAD_CurveEntity.h"
#include "CAD_TextEntity.h"
#include "CAD_Pline2DEntity.h"
#include "CAD_PaintEntity.h"
#include "CAD_BlockEntity.h"

enum _DimensionOptions
{
	DIMTEXTPLACE_NODEFAULT	= 0x0001,

	DIMTEXTPLACE_INSIDE		= 0x0010,
	DIMARROWPLACE_INSIDE	= 0x0020,
	DIMTEXTORARROW_INOUTSTATEFIELD	= DIMTEXTPLACE_INSIDE | DIMARROWPLACE_INSIDE,

	DIMOPTION_ORTHER		= 0x1000
};

enum _DimensionElementCode
{
	DIMELEMENT_EXTENSION,
	DIMELEMENT_DIMENSION,
	DIMELEMENT_TEXT
};

enum _DimensionTypeCode
{
	DIM_LINEAR				= 0,
	DIM_ORDINATE,
	DIM_RADIUS,
	DIM_DIAMETER,
	DIM_ANGULAR
};

#define IsObliqueArrow(nStyle)			((nStyle == SDAS_ARCHTICK)	\
											|| (nStyle == SDAS_OBLIQUE)	\
											|| (nStyle == SDAS_INTEGRAL)	\
											|| (nStyle == SDAS_NONE))

#define TANGENT_LEADERLINE				17	// degree

class  CCAD_GemDimension : public CCAD_GemBlock
{
public:
	short			m_nStyle;
	CCAD_DimStyle*	m_pStyle;

	unsigned short	m_nOption;				// oblique/home/angle/left/center/right

	CCAD_TextStyle* m_pTxtStyle;
	char			m_szText[MAX_DIMSTR];
	CCAD_Angle		m_anglText;				// ignore, if text position code no setting angle
	CCAD_Point		m_ptText;

	char			m_szDefaultText[MAX_DIMSTR];

public:
	CCAD_GemDimension(CCAD_Doc *);
	virtual ~CCAD_GemDimension();

public:
	virtual	void Init();
	virtual void MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual BOOL Read(CCAD_File& cFile);

	virtual void CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect &cRange);
	virtual void CountBodyRange(CCAD_Rect &cRange);

	virtual CCAD_Block*	GetBlock();

	virtual void TmpDraw(CCAD_DC* pCadDC);
	virtual void AttachElementToDimBlock(CCAD_Block* pBlock);

	virtual void GetBaseCouninueDim(BOOL bContinue, CCAD_GemDimension* pGetDim, const CCAD_Vector& ptExt2)
		{ ASSERT(FALSE); }

protected:
	CCAD_DC* m_pDrawDC;
	CCAD_Block* m_pDimBlock;

	virtual void DrawDimension(BOOL blTmp=FALSE) {}

	CCAD_Block* GetArrowHeadBlock(short nID);
	void GetDefaultTextParameter(BYTE bCode, CAD_FLOAT fValue, CCAD_TextEntity& txt, 
			CCAD_TextEntity& upper, CCAD_TextEntity& lower, unsigned short& nShowText);
	void GetTextArrayBounds(short nCount, CCAD_TextEntity** ppText, CCAD_Rect& boundTxt);

	void DrawDimensionText(short nTxtCount, CCAD_TextEntity** ppText);

	void DrawElement(short nCode, CCAD_Entity* pElement);
};

#endif
