// CAD_Global.h: interface for the CCAD_Global class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_GLOBAL_H_)
#define AFX_CAD_GLOBAL_H_

#include "CAD_Constance.h"

class	CCAD_Accurancy;
class	CCAD_DC;

#define CP_ACP 0
#define CSK_MAX			8
#define	CODE_MAX		4332
enum	CodeSystemKind{
	CSK_PIC_97		=0,		
	CSK_UNICODE,			
	CSK_PRK_97,				
	CSK_KSC_FONT,			
	CSK_PIC_F0,				
	CSK_KW,					
	CSK_KS_88,				
	CSK_KS_5601				
};
class   CCAD_Global {
public:
	CCAD_Global() {};

	static	const short	m_nUnit[];

public:
	static	CAD_FLOAT	GetLengthAccy();
	static	CAD_FLOAT	GetSqrtAccy();
	static	CAD_FLOAT	GetAngleAccy();

	static	BOOL	IsOverFloat(CAD_FLOAT fValue);
	static	void 	StrToTime(LPCSTR lpszDate, SYSTEMTIME &sTime);
	static int pic_atoi(char *str, int dec = 10);	

    static void SetSpecialDC(CCAD_DC * pDC);
	static CCAD_DC *GetSpecialDC();
	static void SetSignValue(CAD_FLOAT & Val, short iSign);
	static int	uni_strlen(const TCHAR* str);
};

#endif // !defined(AFX_CAD_GLOBAL_H_)
