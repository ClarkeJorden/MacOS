#include "stdafx.h"
//#include <math.h>
#include "CAD_Global.h"
#include "CAD_Angle.h"

 CAD_FLOAT CCAD_Global::GetLengthAccy()
{
	return 0.1f;
}

CAD_FLOAT CCAD_Global::GetSqrtAccy()
{
	return 0.01f;
}

CAD_FLOAT CCAD_Global::GetAngleAccy()
{
	return 0.1f;
}

void CCAD_Global::StrToTime(LPCSTR lpszDate, SYSTEMTIME &sTime)
{
	char strDate[256], temp[256];
	strcpy(strDate, lpszDate);
	int	iPrev = 0;
	int	i;
	int	iSize = strlen(lpszDate);

	for (i = iPrev; i < iSize; i ++) {
		if (lpszDate[i] < '0' || lpszDate[i] > '9') {
			strncpy(temp, strDate+iPrev, i-iPrev);
			temp[i-iPrev] = '\0';
			sTime.wYear= (WORD) pic_atoi(temp);
			break;
		}
	}
	iPrev = i + 1;
	for (i = iPrev; i < iSize; i ++) {
		if (lpszDate[i] < '0' || lpszDate[i] > '9') {
			strncpy(temp, strDate+iPrev, i-iPrev);
			temp[i-iPrev] = '\0';
			sTime.wMonth = (WORD) pic_atoi(temp);
			iPrev = i;
			break;
		}
	}
	iPrev = i + 1;
	for (i = iPrev; i < iSize; i ++) {
		if (lpszDate[i] < '0' || lpszDate[i] > '9') {
			strncpy(temp, strDate+iPrev, i-iPrev);
			temp[i-iPrev] = '\0';
			sTime.wDay = (WORD) pic_atoi(temp);
			break;
		}
		if (i == iSize - 1) {
			strncpy(temp, strDate+iPrev, i-iPrev+1);
			temp[i-iPrev+1] = '\0';
			sTime.wDay = (WORD) pic_atoi(temp);
			break;
		}
	}
	if (sTime.wYear < 1970 || sTime.wMonth < 1 || sTime.wMonth > 12 || sTime.wDay < 1 || sTime.wDay > 31)
		::memset(&sTime, 0, sizeof(sTime));
	
/*fprintf(stderr, "%d,%d,%d\n", sTime.wYear, sTime.wMonth, sTime.wDay);*/

}

BOOL CCAD_Global::IsOverFloat(CAD_FLOAT fValue)
{
	CAD_FLOAT fMaxLimit = 3.4E+12F;
//	CAD_FLOAT fMinLimit = 3.4E-12F;

	CAD_FLOAT fABSVal = (CAD_FLOAT)fabs((CAD_FLOAT)fValue);

//	if( fABSVal < fMinLimit || fABSVal > fMaxLimit )
	if( fABSVal > fMaxLimit )
		return true;
	else
		return false;
}

int CCAD_Global::pic_atoi( char *str, int dec )
{
	if(str==NULL) return 0;
	int nStrLen = 0;
	for(nStrLen = 0; ;nStrLen++) {
		if (str[nStrLen] == 0)
			break;
	}
	int retValue = 0;
	int value = 0;
	for(int i = 0; i < nStrLen; i++)
	{
		value = str[i] - 0x30;
		if(value < 0 || value > 9)
			return retValue;
		retValue = retValue * dec + value;
	}
	return retValue;
}

CCAD_DC* gpCadDC = NULL;

void CCAD_Global::SetSpecialDC( CCAD_DC * pDC )
{
	gpCadDC = pDC;
}

CCAD_DC * CCAD_Global::GetSpecialDC( )
{
	return gpCadDC;
}

void CCAD_Global::SetSignValue( CAD_FLOAT & Val, short iSign )
{
	Val = (CAD_FLOAT)fabs(Val);

	if(iSign < 0)
		iSign = -1;
	else
		iSign = 1;

	Val *= iSign;
}

int CCAD_Global::uni_strlen( const TCHAR* str )
{
	int i = 0;
	for(i = 0; ;i++) {
		if (str[i] == 0)
			break;
	}
	return i;
}



 
