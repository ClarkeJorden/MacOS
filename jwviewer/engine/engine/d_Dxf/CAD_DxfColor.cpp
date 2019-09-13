#include "stdafx.h"
#include "CAD_DxfGlobal.h"
#include <math.h>


SDXF_HLSColor sCvtDXFtoHLSColor(DXF_LONG& lDXF)
{
	SDXF_HLSColor	sHLS;
	DXF_DOUBLE		dHueTbl[25];
	DXF_DOUBLE		dLightTbl[10] = {0.51, 0.75, 0.33, 0.48, 0.25, 0.37, 0.15, 0.22, 0.07, 0.10};
	DXF_DOUBLE		dSatTbl[10] = {1.0, 1.0, 1.0, 0.34, 1.0, 0.35, 1.0, 0.36, 1.0, 0.38};
	DXF_DOUBLE		dAchTbl[6] = {0.33, 0.46, 0.60, 0.73, 0.79, 1.0};
	DXF_DOUBLE		dDelta;
	DXF_LONG		lHueID, lLSID, lIndex;
	div_t			sID;
	
	/* Make Hue Table */
	dDelta = 15.0;			/* Distance in Hue */
	dHueTbl[0] = 360.0;
	for (lIndex = 1; lIndex < 25; lIndex++)
		dHueTbl[lIndex] = (lIndex - 1) * dDelta;
	if (lDXF < 10) {
		switch (lDXF) {
			case 0:
				lDXF = 255;
				break;
			case 1:
				lDXF = 10;
				break;
			case 2:
				lDXF = 50;
				break;
			case 3:
				lDXF = 90;
				break;
			case 4:
				lDXF = 130;
				break;
			case 5:
				lDXF = 170;
				break;
			case 6:
				lDXF = 210;
				break;
			case 7:
				lDXF = 255;
				break;
			case 8:
				lDXF = 136;
				break;
			case 9:
				sHLS.dH = 0;
				sHLS.dL = 0.76F;
				sHLS.dS = 0;
				return sHLS;
		}
	}
	
	/* Separate AutoCAD Color Index */
	sID = div((int)lDXF, 10);
	lHueID = (DXF_LONG)sID.quot;
	lLSID = (DXF_LONG)sID.rem;
	
	if (lHueID != 25) {
		sHLS.dH = dHueTbl[lHueID];
		sHLS.dL = dLightTbl[lLSID];
		sHLS.dS = dSatTbl[lLSID];
	}
	else {
		sHLS.dH = 0;
		sHLS.dL = dAchTbl[lLSID];
		sHLS.dS = 0;
	}

	return sHLS;
}

DXF_DOUBLE dColorValue(DXF_DOUBLE dN1, DXF_DOUBLE dN2, DXF_DOUBLE dHue)
{
	DXF_DOUBLE	dVal;

	if (dHue > 360.0)
		dHue = dHue - 360;
	else if (dHue < 0)
		dHue = dHue + 360;
	if (dHue < 60)
		dVal = dN1 + (dN2 - dN1) * dHue / 60.0;
	else if (dHue < 180)
		dVal = dN2;
	else if (dHue < 240)
		dVal = dN1 + (dN2 - dN1) * (240.0 - dHue) / 60.0;
	else
		dVal = dN1;

	return dVal;
}

SDXF_RGBColor sCvtHLStoRGBColor(SDXF_HLSColor& sHLS)
{
	DXF_DOUBLE		dM1, dM2, dRed, dGreen, dBlue, dTemp;
	DXF_DOUBLE		dColor;
	SDXF_RGBColor	sRGB;
	DXF_BYTE		bRed, bGreen, bBlue;
	
	if (sHLS.dS == 0.0) {
		dRed = dGreen = dBlue = sHLS.dL;	/* Achromatic: There is no hue */
	}
	else {					/* This is the achromatic case. */
		if (sHLS.dL <= 0.5)
			dM2 = sHLS.dL * (1 + sHLS.dS);
		else
			dM2 = sHLS.dL + sHLS.dS - sHLS.dL * sHLS.dS;
		dM1 = 2 * sHLS.dL - dM2;
		dRed = dColorValue(dM1, dM2, sHLS.dH + 120.0);
		dGreen = dColorValue(dM1, dM2, sHLS.dH);
		dBlue = dColorValue(dM1, dM2, sHLS.dH - 120.0);
	}
	dRed *= 255.0, dGreen *= 255.0, dBlue *= 255.0;

	dTemp = modf(dRed, &dColor);
	bRed = (DXF_BYTE)dColor;
	if (dTemp >= 0.5)
		bRed ++;

	dTemp = modf(dGreen, &dColor);
	bGreen = (DXF_BYTE) dColor;
	if (dTemp >= 0.5F)
		bGreen ++;

	dTemp = modf(dBlue, &dColor);
	bBlue = (DXF_BYTE) dColor;
	if (dTemp >= 0.5F)
		bBlue ++;

	if (bRed < 0)
		bRed += 255;
	else if (bRed > 255)
		bRed -= 255;
	if (bGreen < 0)
		bGreen += 255;
	else if (bGreen > 255)
		bGreen -= 255;
	if (bBlue < 0)
		bBlue += 255;
	else if (bBlue > 255)
		bBlue -= 255;

	if (bRed == 0 && bGreen == 0 && bBlue == 0) {
		bRed = 64, bGreen = 64, bBlue = 64;
	}
	else if (bRed == 255 && bGreen == 255 && bBlue == 255)
	{
		//bRed = 192, bGreen = 192, bBlue = 192;
		bRed = 0, bGreen = 0, bBlue = 0;
	}

	sRGB.bRed = bRed, sRGB.bGreen = bGreen, sRGB.bBlue = bBlue;

	return sRGB;
}

SDXF_RGBColor sDXF_CvtDXFToRGBColor(DXF_LONG& lDXF)
{
	SDXF_HLSColor	sHLS;

	if (lDXF < 0)
		lDXF = -lDXF;
	sHLS = sCvtDXFtoHLSColor(lDXF);

	return sCvtHLStoRGBColor(sHLS);
}

SDXF_HLSColor sCvtRGBtoHLSColor(SDXF_RGBColor& sRGB)
{
	SDXF_HLSColor	sHLS;
	DXF_DOUBLE		dRed, dGreen, dBlue;
	DXF_DOUBLE		dMax, dMin;
	DXF_DOUBLE		dDelta;
	
	dRed = (DXF_DOUBLE)sRGB.bRed;
	dGreen = (DXF_DOUBLE)sRGB.bGreen;
	dBlue = (DXF_DOUBLE)sRGB.bBlue;
	
	dRed /= 255.0, dGreen /= 255.0; dBlue /= 255.0;
	
	dMax = dMin = dRed;
	if (dMax < dGreen)
		dMax = dGreen;
	if (dMax < dBlue)
		dMax = dBlue;
	
	if (dMin > dGreen)
		dMin = dGreen;
	if (dMin > dBlue)
		dMin = dBlue;
	
	sHLS.dL = (dMax + dMin) / 2.0;
	
	if (dMax == dMin) {
		sHLS.dS = 0;
		sHLS.dH = 0;	// Undefined
	} 
	else {
        dDelta = dMax - dMin;
		if (sHLS.dL <= 0.5)
			sHLS.dS = dDelta / (dMax + dMin);
		else
			sHLS.dS = dDelta / (2.0 - dMax - dMin);

        if (dRed == dMax)			/* Resulting color is between yellow and magenta */
        	sHLS.dH = (dGreen - dBlue) / dDelta;
        else if (dGreen == dMax)	/* Resulting color is between cyan and yellow*/
        	sHLS.dH = 2.0 + (dBlue - dRed) / dDelta;
        else if (dBlue == dMax)		/*Resulting color is between magenta and cyan*/
        	sHLS.dH = 4.0 + (dRed - dGreen) / dDelta;

        sHLS.dH *= 60.0;			/* Convert to degrees */
        if (sHLS.dH < 0.0)
			sHLS.dH += 360.0;		/* Make degrees be nonnegative */
	}

	return sHLS;
}

DXF_LONG lCvtHLStoDXFColor(SDXF_HLSColor& sHLS)
{
	DXF_LONG		lDXF;
	DXF_LONG		lIndex;
	DXF_LONG		lMinID, lHueID, lLightID;
	DXF_DOUBLE		dHueTbl[25];
	DXF_DOUBLE		dLightTbl[10] = {0.5, 0.75, 0.33, 0.48, 0.25, 0.37, 0.15, 0.22, 0.07, 0.10};
	DXF_DOUBLE		dAchTbl[5] = {0.33, 0.46, 0.60, 0.73, 0.79};
	DXF_DOUBLE		dDelta, dMinDelta;
    
    if (sHLS.dL == 0.0)
		lDXF = 18;
    else if (sHLS.dL == 1.0)
		lDXF = 7;		/* White */
    else if (sHLS.dS == 0.0) {				/* Achromatic case */
		if (sHLS.dL == 0.76) {
			lDXF = 9;
		}
		else {
			dMinDelta = dAchTbl[0];
			lMinID = 0;
			for (lIndex = 1; lIndex < 5; lIndex++) {
				if (dAchTbl[lIndex] == sHLS.dL) {
					lMinID = lIndex;
					break;
				}
				dDelta = fabs(dAchTbl[lIndex] - sHLS.dL);
				if (dMinDelta > dDelta) {
					dMinDelta = dDelta;
					lMinID = lIndex;
				}
			}
			lLightID = lMinID;
			lHueID = 25;
			/* Get complate color index */
			lDXF = lHueID * 10 + lLightID;
		}
	}
	else {					/* Make Hue Table */
		dDelta = 15.0F;			/* Distance in Hue */
		dHueTbl[0] = 360.0F;
		for (lIndex = 1; lIndex < 25; lIndex++)
			dHueTbl[lIndex] = (lIndex - 1) * dDelta;
		/* Set color index */
		/* First calculate the Hue index */
		dMinDelta = dHueTbl[0];
		lMinID = 0;
		for (lIndex = 0; lIndex < 25; lIndex++) {
			if (dHueTbl[lIndex] == sHLS.dH) {
				lMinID = lIndex;
				break;
			}
			dDelta = fabs(dHueTbl[lIndex] - sHLS.dH);
			if (dMinDelta > dDelta) {
				dMinDelta = dDelta;
				lMinID = lIndex;
			}
		}
		lHueID = lMinID;
		
		/* Next calculate the lightness index */
		dMinDelta = dLightTbl[0];
		lMinID = 0;
		for (lIndex = 0; lIndex < 10; lIndex++) {
			if (dLightTbl[lIndex] == sHLS.dL) {
				lMinID = lIndex;
				break;
			}
			dDelta = fabs(dLightTbl[lIndex] - sHLS.dL);
			if (dMinDelta > dDelta) {
				dMinDelta = dDelta;
				lMinID = lIndex;
			}
		}
		lLightID = lMinID;
		/* Get complate color index */
		lDXF = lHueID * 10 + lLightID;
		if (lDXF == 10)
			lDXF = 1;			/* Red */
		else if (lDXF == 50)
			lDXF = 2;			/* Yellow */
		else if (lDXF == 90)
			lDXF = 3;			/* Green */
		else if (lDXF == 130)
			lDXF = 4;			/* Cyan */
		else if (lDXF == 170)
			lDXF = 5;		/* Blue */
		else if (lDXF == 210)
			lDXF = 6;		/* Magenta */
		else if (lDXF == 136)
			lDXF = 8;
	}

	return lDXF;
}

DXF_LONG lDXF_CvtRGBToDXFColor(SDXF_RGBColor& sRGB)
{
	SDXF_HLSColor	sHLS;
	
	sHLS = sCvtRGBtoHLSColor(sRGB);

	return lCvtHLStoDXFColor(sHLS);
}

