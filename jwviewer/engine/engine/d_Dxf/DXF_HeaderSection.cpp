#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_HeaderSection.h"

#define		DXF_HEADER_ANGBASE		50
#define		DXF_HEADER_LONG			70
#define		DXF_HEADER_DOUBLE		40
#define		DXF_HEADER_GROUP		9
#define		DXF_HEADER_POINT			10
#define		DXF_HEADER_STRING		2
#define		DXF_HEADER_COLOR		62
#define		DXF_HEADER_LTYPE		6

CDXF_HeaderSection::CDXF_HeaderSection()
{
	memset(m_pszACADVER, 0x00, MAX_STRING_LEN);
	m_dANGBASE = 0;
	m_lANGDIR = 0;
	m_lATTDIA = 0;
	m_lATTMODE = 1;
	m_lATTREQ = 1;
	m_lAUNITS = 0;
	m_lAUPREC = 0;
	m_lBLIPMODE = 1;
	m_lCECOLOR = 256;
	m_dCELTSCALE = 1.0;
	memcpy(m_pszCELTYPE, "BYLAYER", MAX_STRING_LEN);
	m_dCHAMFERA = 0;
	m_dCHAMFERB = 0;
	m_dCHAMFERC = 0;
	m_dCHAMFERD = 0;
	memcpy(m_pszCLAYER, "0", MAX_STRING_LEN);
	m_lCMLJUST = 0;
	m_dCMLSCALE = 1.0;
	memcpy(m_pszCMLSTYLE, "STANDARD", MAX_STRING_LEN);
	m_lCOORDS = 0;
	m_lDELOBJ = 1;
	m_lDIMALT = 0;
	m_lDIMALTD = 2;
	m_dDIMALTF = 25.4f;
	m_lDIMALTTD = 2;
	m_lDIMALTTZ = 0;
	m_lDIMALTU = 2;
	m_lDIMALTZ = 0;
	sprintf(m_pszDIMAPOST, "%s", "");
	m_lDIMASO = 1;
	m_dDIMASZ = 0.18;
	m_lDIMAUNIT = 0;
	sprintf(m_pszDIMBLK, "%s", "");
	sprintf(m_pszDIMBLK1, "%s", "");
	sprintf(m_pszDIMBLK2, "%s", "");
	m_dDIMCEN = 0.09;
	m_lDIMCLRD = 0;
	m_lDIMCLRE = 0;
	m_lDIMCLRT = 0;
	m_lDIMDEC = 4;
	m_dDIMDLE = 0;
	m_dDIMDLI = 0.38;
	m_dDIMEXE = 0.18;
	m_dDIMEXO = 0.0625;
	m_lDIMFIT = 3;
	m_dDIMGAP = 0.09;
	m_lDIMJUST = 0;
	m_dDIMLFAC = 1.0;
	m_lDIMLIM = 0;
	sprintf(m_pszDIMPOST, "%s", "");
	m_dDIMRND = 0;
	m_lDIMSAH = 0;
	m_dDIMSCALE = 1;
	m_lDIMSD1 = 0;
	m_lDIMSD2 = 0;
	m_lDIMSE1 = 0;
	m_lDIMSE2 = 0;
	m_lDIMSHO = 1;
	m_lDIMSOXD = 0;
	sprintf(m_pszDIMSTYLE, "%s", "STANDARD");
	m_lDIMTAD = 1;
	m_lDIMTDEC = 4;
	m_dDIMTFAC = 1;
	m_lDIMTIH = 1;
	m_lDIMTIX = 0;
	m_dDIMTM = 0;
	m_lDIMTOFL = 0;
	m_lDIMTOH = 1;
	m_lDIMTOL = 0;
	m_lDIMTOLJ = 1;
	m_dDIMTP = 0;
	m_dDIMTSZ = 0;
	m_dDIMTVP = 0;
	sprintf(m_pszDIMTXSTY, "%s", "STANDARD");
	m_dDIMTXT = 0.18;
	m_lDIMTZIN = 0;
	m_lDIMUNIT = 2;
	m_lDIMUPT = 0;
	m_lDIMZIN = 8;
	m_lDISPSILH = 0;
	m_lDRAGMODE = 2;
	sprintf(m_pszDWGCODEPAGE, "%s", "ANSI_950");
//	sprintf(m_pszDWGCODEPAGE, "%s", "ANSI_949"); for chinese.
	m_dELEVATION = 0.0;
	m_sW2DEXTMAX.dX = -1.0e20;
	m_sW2DEXTMAX.dY = -1.0e20;
//	m_sW3DEXTMAX.dZ = -1.0e20;
	m_sW2DEXTMIN.dX = 1.0e20;
	m_sW2DEXTMIN.dY = 1.0e20;
//	m_sW3DEXTMIN.dZ = 1.0e20;
	m_dFILLETRAD = 0;
	m_lFILLMODE = 1;
	m_lHANDLING = 4097;
	m_lHANDSEED = g_lDXFHandleNumber;
	//m_3dINSBASE.dX = 0;
	//m_3dINSBASE.dY = 0;
	//m_3dINSBASE.dZ = 0;
	m_lLIMCHECK = 0;
	m_sW2DLIMMAX.dX = 12.0;
	m_sW2DLIMMAX.dY = 9.0;
	m_sW2DLIMMIN.dX = 0.0;
	m_sW2DLIMMIN.dY = 0.0;
	m_dLTSCALE = 1.0;
	m_lLUNITS = 2;
	m_lLUPREC = 4;
	m_lMAXACTVP = 16;
	m_lMEASUREMENT = 0;
	m_lMIRRTEXT = 1;
	m_lORTHOMODE = 0;
	m_lOSMODE = 0;
	m_lPDMODE = 0;
	m_dPDSIZE = 0.0;
	m_dPELEVATION = 0.0;
	m_lPICKSTYLE = 1;
	m_sW3DPEXTMAX.dX = 1e20;
	m_sW3DPEXTMAX.dY = 1e20;
	m_sW3DPEXTMAX.dZ = 1e20;
	m_sW3DPEXTMIN.dX = -1e20;
	m_sW3DPEXTMIN.dY = -1e20;
	m_sW3DPEXTMIN.dZ = -1e20;
	m_sW2DPLIMMAX.dX = 12.0;
	m_sW2DPLIMMAX.dY = 9.0;
	m_sW2DPLIMMIN.dX = 0.0;
	m_sW2DPLIMMIN.dY = 0.0;
	m_lPLIMCHECK = 0;
	m_lPLINEGEN = 0;
	m_dPLINEWID = 0.0;
	m_lPROXYGRAPHICS = 1;
	m_lPSLTSCALE = 1;
	memset(m_pszPUCSNAME, 0x00, MAX_STRING_LEN);
	//m_sW3DPUCSORG.dX = 0.0;
	//m_sW3DPUCSORG.dY = 0.0;
	//m_sW3DPUCSORG.dZ = 0.0;
	m_sW3DPUCSXDIR.dX = 1.0;
	m_sW3DPUCSXDIR.dY = 0.0;
	m_sW3DPUCSXDIR.dZ = 0.0;
	m_sW3DPUCSYDIR.dX = 0.0;
	m_sW3DPUCSYDIR.dY = 1.0;
	m_sW3DPUCSYDIR.dZ = 0.0;
	m_lQTEXTMODE = 0;
	m_lREGENMODE = 1;
	m_lSAVEIMAGES = 0;
	m_lSHADEDGE = 3;
	m_lSHADEDIF = 70;
	m_dSKETCHINC = 0.1;
	m_lSKPOLY = 0;
	m_lSPLFRAME = 0;
	m_lSPLINESEGS = 8;
	m_lSPLINETYPE = 6;
	m_lSURFTAB1 = 6;
	m_lSURFTAB2 = 6;
	m_lSURFTYPE = 6;
	m_lSURFU = 6;
	m_lSURFV = 6;
	m_dTDCREATE = 0.0;
	m_dTDINDWG = 0.0;
	m_dTDUPDATE = 0.0;
	m_dTDUSRTIMER = 0.0;
	m_dTEXTSIZE = 0.2;
	sprintf(m_pszTEXTSTYLE, "%s", "STANDARD");
	m_dTHICKNESS = 0.0;
	m_lTILEMODE = 1;
	m_dTRACEWID = 0.05;
	m_lTREEDEPTH = 3020;
	sprintf(m_pszUCSNAME, "%s", "");
	//m_sW3DUCSORG.dX = 0.0;
	//m_sW3DUCSORG.dY = 0.0;
	//m_sW3DUCSORG.dZ = 0.0;
	m_sW3DUCSXDIR.dX = 1.0;
	m_sW3DUCSXDIR.dY = 0.0;
	m_sW3DUCSXDIR.dZ = 0.0;
	m_sW3DUCSYDIR.dX = 0.0;
	m_sW3DUCSYDIR.dY = 1.0;
	m_sW3DUCSYDIR.dZ = 0.0;
	m_lUNITMODE = 0;
	m_lUSERI1 = 0;
	m_lUSERI2 = 0;
	m_lUSERI3 = 0;
	m_lUSERI4 = 0;
	m_lUSERI5 = 0;
	m_dUSERR1 = 0.0;
	m_dUSERR2 = 0.0;
	m_dUSERR3 = 0.0;
	m_dUSERR4 = 0.0;
	m_dUSERR5 = 0.0;
	m_lUSRTIMER = 0;
	m_lVISRETAIN = 0;
	m_lWORLDVIEW = 1;
}

CDXF_HeaderSection::~CDXF_HeaderSection()
{

}

DXF_BOOL CDXF_HeaderSection::DXFReadHeaderSection(FILE* pFile)
{
	DXF_BOOL	blReturn;
	DXF_LONG	code;
	DXF_CHAR	szString[MAX_STRING_LEN];

	blReturn = DXFReadGroup(pFile, code, szString);
	while (blReturn && strcmp(szString, "ENDSEC") != 0) {
		if (code == DXF_HEADER_GROUP && strcmp(szString, "$ACADVER") == 0) {
			if (FALSE == DXFReadGroupString(pFile, 1, m_pszACADVER))
				return FALSE;
			if (strcmp(m_pszACADVER, "AC1009") != 0)	//1014 // r12, r13
			{
				g_lDXFErrorCode = DXF_INVALID_VERSION;
				return FALSE;
			}
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$ANGBASE") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_ANGBASE, m_dANGBASE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$ANGDIR") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lANGDIR);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$ATTMODE") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lATTMODE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$ATTREQ") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lATTREQ);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$AUNITS") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lAUNITS);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$AUPREC") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lAUPREC);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$BLIPMODE") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lBLIPMODE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$CECOLOR") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_COLOR, m_lCECOLOR);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$CELTSCALE") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dCELTSCALE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$CELTYPE") == 0) {
			blReturn = DXFReadGroupString(pFile, DXF_HEADER_LTYPE, m_pszCELTYPE);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$CELWEIGHT") == 0) {
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$CEPSNID") == 0) {
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$CEPSNTYPE") == 0) {
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$CHAMFERA") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dCHAMFERA);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$CHAMFERB") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dCHAMFERB);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$CHAMFERC") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dCHAMFERC);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$CHAMFERD") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dCHAMFERD);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$CLAYER") == 0) {
			blReturn = DXFReadGroupString(pFile, 8, m_pszCLAYER);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$CMLJUST") == 0) {
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lCMLJUST);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$CMLSCALE") == 0) {
//			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dCMLSCALE);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$CMLSTYLE") == 0) {
//			blReturn = DXFReadGroupString(pFile, DXF_HEADER_STRING, m_pszCMLSTYLE);
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$COORDS") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lCOORDS);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DELOBJ") == 0) {
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDELOBJ);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMADEC") == 0) {
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMALT") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMALT);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMALTD") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMALTD);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMALTF") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMALTF);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMALTRND") == 0) {
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMALTTD") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMALTTD);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMALTTZ") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMALTTZ);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMALTU") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMALTU);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMALTZ") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMALTZ);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMAPOST") == 0) {
			blReturn = DXFReadGroupString(pFile, 1, m_pszDIMAPOST);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMASO") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMASO);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMASZ") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMASZ);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMATFIT") == 0) {
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMAUNIT") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMAUNIT);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMAZIN") == 0) {
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMBLK") == 0) {
//			blReturn = DXFReadGroupString(pFile, 1, m_pszDIMBLK);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMBLK1") == 0) {
//			blReturn = DXFReadGroupString(pFile, 1, m_pszDIMBLK1);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMBLK2") == 0) {
//			blReturn = DXFReadGroupString(pFile, 1, m_pszDIMBLK2);
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMCEN") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMCEN);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMCLRD") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMCLRD);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMCLRE") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMCLRE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMCLRT") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMCLRT);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMDEC") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMDEC);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMDLE") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMDLE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMDLI") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMDLI);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMDSEP") == 0) {
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMEXE") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMEXE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMEXO") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMEXO);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMFAC") == 0) {
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMFIT") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMFIT);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMGAP") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMGAP);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMJUST") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMJUST);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMLDRBLK") == 0) {
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMLFAC") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMLFAC);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMLIM") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMLIM);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMLUNIT") == 0) {
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMLWD") == 0) {
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMPOST") == 0) {
			blReturn = DXFReadGroupString(pFile, 1, m_pszDIMPOST);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMRND") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMRND);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMSAH") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMSAH);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMSCALE") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMSCALE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMSD1") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMSD1);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMSD2") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMSD2);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMSE1") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMSE1);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMSE2") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMSE2);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMSHO") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMSHO);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMSOXD") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMSOXD);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMSTYLE") == 0) {//2
			blReturn = DXFReadGroupString(pFile, DXF_HEADER_STRING, m_pszDIMSTYLE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTAD") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMTAD);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTDEC") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMTDEC);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTFAC") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMTFAC);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTIH") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMTIH);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTIX") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMTIX);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTM") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMTM);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTMOVE") == 0) {
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTOFL") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMTOFL);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTOH") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMTOH);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTOL") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMTOL);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTOLJ") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMTOLJ);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTP") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMTP);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTSZ") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMTSZ);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTVP") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMTVP); 
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTXSTY") == 0) {
			blReturn = DXFReadGroupString(pFile, 7, m_pszDIMTXSTY);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTXT") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dDIMTXT);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMTZIN") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMTZIN);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMUNIT") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMUNIT);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMUPT") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMUPT);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DIMZIN") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDIMZIN);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DISPSILH") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDISPSILH);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DRAGMODE") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lDRAGMODE);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$DWGCODEPAGE") == 0) {
//			blReturn = DXFReadGroupString(pFile, code, m_pszDWGCODEPAGE);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$ELEVATION") == 0) {
//			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dELEVATION);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$ENDCAPS") == 0) {
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$EXTMAX") == 0) {
			//10, 20, 30
			blReturn = DXFReadGroupPoint2D(pFile, DXF_HEADER_POINT, m_sW2DEXTMAX);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$EXTMIN") == 0) {
			// 10, 20, 30
			blReturn = DXFReadGroupPoint2D(pFile, DXF_HEADER_POINT, m_sW2DEXTMIN);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$EXTNAMES") == 0) {
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$FILLETRAD") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dFILLETRAD);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$FILLMODE") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lFILLMODE);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$FINGERPRINTGUID") == 0) {
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$FLATLAND") == 0) {
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lFLATLAND);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$HANDLING") == 0) {
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lHANDLING);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$HANDSEED") == 0) {
		// 5 (handler)
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$HYPERLINKBASE") == 0) {
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$INSBASE") == 0) {
			//10, 20, 30
			blReturn = DXFReadGroupPoint3D(pFile, DXF_HEADER_POINT, m_sW3DINSBASE);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$INSUNITS") == 0) {
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$JOINSTYLE") == 0) {
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$LIMCHECK") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lLIMCHECK);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$LIMMAX") == 0) {
			//10, 20
			blReturn = DXFReadGroupPoint2D(pFile, DXF_HEADER_POINT, m_sW2DLIMMAX);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$LIMMIN") == 0) {
			//10, 20
			blReturn = DXFReadGroupPoint2D(pFile, DXF_HEADER_POINT, m_sW2DLIMMIN);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$LTSCALE") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dLTSCALE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$LUNITS") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lLUNITS);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$LUPREC") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lLUPREC);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$LWDISPLAY") == 0) {
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$MAXACTVP") == 0) {
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lMAXACTVP);
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$MEASUREMENT") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lMEASUREMENT);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$MENU") == 0) {
//			blReturn = DXFReadGroupString(pFile, code, m_pszMENU);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$MIRRTEXT") == 0) {
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lMIRRTEXT);
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$ORTHOMODE") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lORTHOMODE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$OSMODE") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lOSMODE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PDMODE") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lPDMODE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PDSIZE") == 0) {
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dPDSIZE);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PELEVATION") == 0) {
//			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dPELEVATION);
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PEXTMAX") == 0) {
			//10, 20, 30
			blReturn = DXFReadGroupPoint3D(pFile, DXF_HEADER_POINT, m_sW3DPEXTMAX);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PEXTMIN") == 0) {
			//10, 20, 30
			blReturn = DXFReadGroupPoint3D(pFile, DXF_HEADER_POINT, m_sW3DPEXTMIN);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PLIMMIN") == 0) {
			//10, 20, 30
			blReturn = DXFReadGroupPoint2D(pFile, DXF_HEADER_POINT, m_sW2DPLIMMIN);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PLIMMAX") == 0) {
			//10, 20, 30
			blReturn = DXFReadGroupPoint2D(pFile, DXF_HEADER_POINT, m_sW2DPLIMMAX);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PICKSTYLE") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lPICKSTYLE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PINSBASE") == 0) {
			//10, 20, 30
			blReturn = DXFReadGroupPoint3D(pFile, DXF_HEADER_POINT, m_sW3DPINSBASE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PLIMCHECK") == 0) {
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lPLIMCHECK);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PLINEGEN") == 0)
		{
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lPLINEGEN);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PLINEWID") == 0)
		{
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dPLINEWID);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PROXYGRAPHICS") == 0)
//		{
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lPROXYGRAPHICS);
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PSLTSCALE") == 0)
		{
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lPSLTSCALE);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PSTYLEMODE") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PSVPSCALE") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PUCSBASE") == 0)
//		{
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PUCSNAME") == 0)
		{//2
			blReturn = DXFReadGroupString(pFile, code, m_pszPUCSNAME);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PUCSORG") == 0)
		{//10, 20, 30
			blReturn = DXFReadGroupPoint3D(pFile, DXF_HEADER_POINT, m_sW3DPUCSORG);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PUCSORGBACK") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PUCSORGBOTTOM") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PUCSORGFRONT") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PUCSORGLEFT") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PUCSORGRIGHT") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PUCSORGTOP") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PUCSORTHOREF") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PUCSORTHOVIEW") == 0)
//		{
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PUCSXDIR") == 0)
		{//10, 20, 30
			blReturn = DXFReadGroupPoint3D(pFile, DXF_HEADER_POINT, m_sW3DPUCSXDIR);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$PUCSYDIR") == 0)
		{//10, 20, 30
			blReturn = DXFReadGroupPoint3D(pFile, DXF_HEADER_POINT, m_sW3DPUCSYDIR);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$QTEXTMODE") == 0)
		{
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lQTEXTMODE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$REGENMODE") == 0)
		{
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lREGENMODE);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SAVEIMAGES") == 0)
//		{
//			// r13¹¢À¾ ¾¡°¡, r14¹¢À¾ º¢½£
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lSAVEIMAGES);
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SHADEDIF") == 0)
		{
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lSHADEDIF);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SKETCHINC") == 0)
		{
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dSKETCHINC);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SKPOLY") == 0)
		{
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lSKPOLY);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SPLFRAME") == 0)
		{
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lSPLFRAME);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SPLINESEGS") == 0)
		{
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lSPLINESEGS);

		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SPLINETYPE") == 0)
		{
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lSPLINETYPE);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SURFTAB1") == 0)
//		{
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lSURFTAB1);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SURFTAB2") == 0)
//		{
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lSURFTAB2);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SURFTYPE") == 0)
//		{
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lSURFTYPE);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SURFU") == 0)
//		{
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lSURFU);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SURFV") == 0)
//		{
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lSURFV);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$TDCREATE") == 0)
//		{
//			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dTDCREATE);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$TDINDWG") == 0)
//		{
//			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dTDINDWG);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$TDUCREATE") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$TDUPDATE") == 0)
//		{
//			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dTDUPDATE);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$TDUSRTIMER") == 0)
//		{
//			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dTDUSRTIMER);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$TDUUPDATE") == 0)
//		{
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$TEXTSIZE") == 0)
		{
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dTEXTSIZE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$TEXTSTYLE") == 0)
		{// 7 
			blReturn = DXFReadGroupString(pFile, 7, m_pszTEXTSTYLE);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$THICKNESS") == 0)
		{
			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dTHICKNESS);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$TILEMODE") == 0)
//		{
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lTILEMODE);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$TRACEWID") == 0)
//		{
//			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dTRACEWID);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$TREEDEPTH") == 0)
//		{
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lTREEDEPTH);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$UCSBASE") == 0)
//		{
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$UCSNAME") == 0)
		{// 2
			blReturn = DXFReadGroupString(pFile, DXF_HEADER_STRING, m_pszUCSNAME);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$UCSORG") == 0)
		{//10, 20, 30
			blReturn = DXFReadGroupPoint3D(pFile, DXF_HEADER_POINT, m_sW3DUCSORG);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$UCSORGBACK") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$UCSORGBOTTOM") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$UCSORGFRONT") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$UCSORGLEFT") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$UCSORGRIGHT") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$UCSORGTOP") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$UCSORTHOREF") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$UCSORTHOVIEW") == 0)
//		{
//		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$UCSXDIR") == 0)
		{// 10, 20, 30
			blReturn = DXFReadGroupPoint3D(pFile, DXF_HEADER_POINT, m_sW3DUCSXDIR);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$UCSYDIR") == 0)
		{//10, 20, 30
			blReturn = DXFReadGroupPoint3D(pFile, DXF_HEADER_POINT, m_sW3DUCSYDIR);
		}
		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$UNITMODE") == 0)
		{
			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lUNITMODE);
		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$USERI1") == 0)
//		{
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lUSERI1);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$USERI2") == 0)
//		{
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lUSERI2);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$USERI3") == 0)
//		{
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lUSERI3);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$USERI4") == 0)
//		{
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lUSERI4);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$USERI5") == 0)
//		{
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lUSERI5);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$USERR1") == 0)
//		{
//			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dUSERR1);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$USERR2") == 0)
//		{
//			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dUSERR2);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$USERR3") == 0)
//		{
//			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dUSERR3);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$USERR4") == 0)
//		{
//			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dUSERR4);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$USERR5") == 0)
//		{
//			blReturn = DXFReadGroupDouble(pFile, DXF_HEADER_DOUBLE, m_dUSERR5);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$USERTIMER") == 0)
//		{
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lUSERTIMER);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$VERSIONGUID") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$VISRETAIN") == 0)
//		{
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lVISRETAIN);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$WORLDVIEW") == 0)
//		{
//			blReturn = DXFReadGroupLong(pFile, DXF_HEADER_LONG, m_lWORLDVIEW);
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$XEDIT") == 0)
//		{
//
	///  VPORT Header variables
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$FASTZOOM") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$GRIDMODE") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$GRIDUNIT") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SNAPANG") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SNAPBASE") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SNAPISOPAIR") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SNAPMODE") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SNAPSTYLE") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$SNAPUNIT") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$VIEWCTR") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$VIEWDIR") == 0)
//		{
//		}
//		else if (code == DXF_HEADER_GROUP && strcmp(szString, "$VIEWSIZE") == 0)
//		{
//		}
		else
		{
			blReturn = DXFReadGroup(pFile, code, szString);
		}
		blReturn = DXFReadGroup(pFile, code, szString);
	}

	return blReturn;
}

