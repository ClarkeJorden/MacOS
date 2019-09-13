#ifndef _CAD_GEM_TEXT_H_
#define _CAD_GEM_TEXT_H_

#include "CAD_GemEntity.h"
#include "CAD_GemSingleText.h"
#include "CAD_TextStyle.h"

typedef struct	{
	float		a00;
	float		a01;
	float		a10;
	float		a11;
} SCAD_TxtConvert;

typedef	CKon_Array <CCAD_GemSingleText*, CCAD_GemSingleText*>	CTextArray;

class CCAD_TextStyle;

class  CCAD_GemText : public CCAD_GemThickEntity  
{
public:
	CCAD_GemText();
	virtual ~CCAD_GemText();

public:
	short			m_nLength;
	LPSTR			m_pStr;
	BYTE			m_bPosFlag;
	CAD_FLOAT		m_fWidth;
	CAD_FLOAT		m_fHeight;
	CCAD_Point		m_cPt;
	CCAD_Angle		m_cDirect;
	CCAD_Angle		m_cOblique;
	short			m_nStyle;
	BYTE			m_bPaint;
	CAD_FLOAT		m_fEmpty;
	
	CCAD_TextStyle	*m_pTextStyle;
	CCAD_TextStyle	m_cOldTextStyle;
	BYTE			m_bNormalDraw;
	CCAD_Point		*m_pTriPoints;
	int							m_nTriCount;
	CCAD_Vector	m_cRp[4];

public:
 	virtual void	MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1 = NULL, LPSTR pData2 = NULL, LPSTR pData3 = NULL);
	virtual void	Remove();
	virtual BOOL	Read(CCAD_File& cFile);
 
// 	virtual void	GetDspFileData(CCAD_DC *pCadDC);
 	virtual BOOL	Drawing(CCAD_DC* pCadDC, BYTE bState, BOOL blRedraw);
 	virtual BOOL	Draw(CCAD_DC *pCadDC, BOOL bRedraw = FALSE, bool bSelected = true);
	virtual void	CountBodyRange(CCAD_Rect& cRange);
	virtual void	CountVcsRange(CCAD_DC *pCadDC, CCAD_Rect& cRange);

	void	Init();
	void	RemoveArray();
 	void	GetWidth(CAD_FLOAT & fHeight, CAD_FLOAT & fWidth) const;
 	void	GetRange(CCAD_Vector * cRp, BOOL bDirectChange=FALSE) const;
// 	BOOL	SetParaFromRange(CCAD_Vector & cPt1, CCAD_Vector & cPt2, CCAD_Vector & cPt3);
 	void	MakeSingleText(CCAD_DC* pCadDC);

protected:
	short	TextNum() const;

 	void	GetLogFont(CCAD_DC *pDC, LOGFONTA/*LOGFONT*/ & sLogFont) const;
 	PFONT	CreateFont(CCAD_DC *pDC, LOGFONTA/*LOGFONT*/ & sLogFont) const;
	BOOL	IsNormalDraw(CCAD_DC *pDC);
 	BOOL	NormalDraw(CCAD_DC *pDC);
 	void	GetLogFontNormal(CCAD_DC *pDC, LOGFONTA/*LOGFONT*/ & sLogFont) const;
 	PFONT	CreateFontNormal(CCAD_DC *pDC, LOGFONTA/*LOGFONT*/ & sLogFont, CCAD_Vector *cRp) const;
// 	PFONT	CreateFontNormal(CCAD_DC *pDC, LOGFONTA/*LOGFONT*/ & sLogFont) const;
 	void	GetWidth(CCAD_DC *pDC, PFONT font, CAD_FLOAT & fHeight, CAD_FLOAT & fWidth) const;
 	void	GetRangeFromWidth(CAD_FLOAT fHy, CAD_FLOAT fWx, CCAD_Vector * cRp, BOOL bDirectChange) const;
// 	void	DrawDspData(CCAD_DC *pDC, CCAD_Vector * cRp);

private:
	//CCAD_GemSingleText	*m_cpSingleText;
	CTextArray		m_pTextArray;
	CCAD_Angle		m_cVertDir;
// 	BOOL			MakeEachSingleText(CCAD_DC *pDC, CCAD_Point &cInsert, WORD wChar, CCAD_GemSingleText &cSingleText);

};

#endif
