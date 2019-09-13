#if !defined(AFX_CAD_TEXTSTYLE_H)
#define AFX_CAD_TEXTSTYLE_H


#include	"CAD_Style.h"
#include	"CAD_Angle.h"

 

typedef unsigned char Str255[256];

typedef struct tagCADTEXTEFFECT {
	unsigned	reserved:1;
	unsigned	vert	:1;
	unsigned	back	:1;
	unsigned	upside	:1;
	unsigned	middle	:1;
	unsigned	under	:1;
	unsigned	italic	:1;
	unsigned	bold	:1;
} CADTEXTEFFECT;


class   CCAD_TextStyle : public CCAD_Style {
public:
	char		m_szFaceName[LF_FACESIZE];
	CADTEXTEFFECT	m_sEffect;
	CAD_FLOAT	m_fHeight;
	CAD_FLOAT	m_fWidth;
	CAD_FLOAT	m_fAngle;

	BYTE		m_bCharSet;
	BYTE		m_bOutPrecision;
	BYTE		m_bClipPrecision;
	BYTE		m_bQuality;
	BYTE		m_bPitchAndFamily;

public:
	CCAD_TextStyle();
	CCAD_TextStyle(const CCAD_TextStyle *pStyle);
	CCAD_TextStyle & operator =(const CCAD_TextStyle &);
	
	BOOL	operator ==(const CCAD_TextStyle &);

	virtual void	Init();
	virtual	BOOL	IsEqual(const CCAD_Style *pStyle);
	virtual void	CopyFrom(const CCAD_Style *pStyle);
	virtual BOOL	Read(CCAD_File &cFile);

	void		SetCadFont(const LOGFONTA/*LOGFONT*/ & sLogFont);
	void		SetLogFont(LOGFONTA/*LOGFONT*/ & sLogFont);
}; 

#endif
