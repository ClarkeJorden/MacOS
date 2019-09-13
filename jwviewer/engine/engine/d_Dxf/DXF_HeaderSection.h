#if !defined(AFX_IMPORTHEADERSECTION_H_)
#define AFX_IMPORTHEADERSECTION_H_


class CDXF_HeaderSection  
{
public:
	CDXF_HeaderSection();
	virtual ~CDXF_HeaderSection();

public:
	DXF_BOOL DXFReadHeaderSection(FILE* fp);

public:
	DXF_CHAR	m_pszACADVER[MAX_STRING_LEN];	// AutoCAD ver
												// "AC1006" - R10
												// "AC1009" - R11 and R12
												// "AC1012" - R13
												// "AC1014" - R14
												// "AC1500" - AutoCAD 2000
	DXF_DOUBLE	m_dANGBASE;		

	DXF_LONG	m_lANGDIR;		

								
								

	DXF_LONG	m_lATTDIA;		
								
								
	DXF_LONG	m_lATTMODE;		
								
								
								
	DXF_LONG	m_lATTREQ;		
								
								
	DXF_LONG	m_lAUNITS;		
								

								

								

								
								
	DXF_LONG	m_lAUPREC;		
	DXF_LONG	m_lBLIPMODE;	
								
								

	DXF_LONG	m_lCECOLOR;		

	DXF_DOUBLE	m_dCELTSCALE;	
	DXF_CHAR	m_pszCELTYPE[MAX_STRING_LEN];	// Âï¼õ º¼¼³´ô ´ªÂôÌ© ººÂôÀô¡Õ
	DXF_DOUBLE	m_dCHAMFERA;	
	DXF_DOUBLE	m_dCHAMFERB;	
	DXF_DOUBLE	m_dCHAMFERC;	
	DXF_DOUBLE	m_dCHAMFERD;	
	DXF_CHAR	m_pszCLAYER[MAX_STRING_LEN];	// Âï¼õÌ© ·²¾¶ËË¶§
	DXF_LONG	m_lCMLJUST;		
								
								
								
	DXF_DOUBLE	m_dCMLSCALE;	
	DXF_CHAR	m_pszCMLSTYLE[MAX_STRING_LEN];	// Âï¼õÌ© ³Þ¼ÚººÊÈ»¥
	DXF_LONG	m_lCOORDS;		
								
								
								
	DXF_LONG	m_lDELOBJ;		
								

								
	DXF_LONG	m_lDIMALT;		
	DXF_LONG	m_lDIMALTD;
	DXF_DOUBLE	m_dDIMALTF;
	DXF_LONG	m_lDIMALTTD;
	DXF_LONG	m_lDIMALTTZ;
	DXF_LONG	m_lDIMALTU;
	DXF_LONG	m_lDIMALTZ;
	DXF_CHAR	m_pszDIMAPOST[MAX_STRING_LEN];	// 
	DXF_LONG	m_lDIMASO;		
								
								
	DXF_DOUBLE	m_dDIMASZ;		
	DXF_LONG	m_lDIMAUNIT;	
	DXF_CHAR	m_pszDIMBLK[MAX_STRING_LEN];	
	DXF_CHAR	m_pszDIMBLK1[MAX_STRING_LEN];	
	DXF_CHAR	m_pszDIMBLK2[MAX_STRING_LEN];	
	DXF_DOUBLE	m_dDIMCEN;		
	DXF_LONG	m_lDIMCLRD;		
	DXF_LONG	m_lDIMCLRE;		
	DXF_LONG	m_lDIMCLRT;		
	DXF_LONG	m_lDIMDEC;		

	DXF_DOUBLE	m_dDIMDLE;		
	DXF_DOUBLE	m_dDIMDLI;		
	DXF_DOUBLE	m_dDIMEXE;		
	DXF_DOUBLE	m_dDIMEXO;		
	DXF_LONG	m_lDIMFIT;		
								

								
								
								
								
								
	DXF_DOUBLE	m_dDIMGAP;		
	DXF_LONG	m_lDIMJUST;		
								
								

								

								

								

	DXF_DOUBLE	m_dDIMLFAC;		

	DXF_LONG	m_lDIMLIM;		
	DXF_CHAR	m_pszDIMPOST[MAX_STRING_LEN];	
	DXF_DOUBLE	m_dDIMRND;		
	DXF_LONG	m_lDIMSAH;		
								
	DXF_DOUBLE	m_dDIMSCALE;	
	DXF_LONG	m_lDIMSD1;		
	DXF_LONG	m_lDIMSD2;		
	DXF_LONG	m_lDIMSE1;		
	DXF_LONG	m_lDIMSE2;		
	DXF_LONG	m_lDIMSHO;		
								
								
	DXF_LONG	m_lDIMSOXD;		
	DXF_CHAR	m_pszDIMSTYLE[MAX_STRING_LEN];	
	DXF_LONG	m_lDIMTAD;		
	DXF_LONG	m_lDIMTDEC;		

	DXF_DOUBLE	m_dDIMTFAC;		
	DXF_LONG	m_lDIMTIH;		

	DXF_LONG	m_lDIMTIX;		
	DXF_DOUBLE	m_dDIMTM;		
	DXF_LONG	m_lDIMTOFL;		
	DXF_LONG	m_lDIMTOH;		

	DXF_LONG	m_lDIMTOL;		
	DXF_LONG	m_lDIMTOLJ;		
								
								
								
	DXF_DOUBLE	m_dDIMTP;		
	DXF_DOUBLE	m_dDIMTSZ;		
								
	DXF_DOUBLE	m_dDIMTVP;		
	DXF_CHAR	m_pszDIMTXSTY[MAX_STRING_LEN];	
	DXF_DOUBLE	m_dDIMTXT;		
	DXF_LONG	m_lDIMTZIN;		
								
								
								
								
								
								
								
	DXF_LONG	m_lDIMUNIT;		
								
								
								
								
								
	DXF_LONG	m_lDIMUPT;		
								
								

								
	DXF_LONG	m_lDIMZIN;		
								
								
								
								
								
								
								
	DXF_LONG	m_lDISPSILH;	
								
								
	DXF_LONG	m_lDRAGMODE;	
								
								
								
	DXF_CHAR	m_pszDWGCODEPAGE[MAX_STRING_LEN];	
	DXF_DOUBLE	m_dELEVATION;	
	SDXF_Point2D	m_sW2DEXTMAX;

	SDXF_Point2D	m_sW2DEXTMIN;

	DXF_DOUBLE	m_dFILLETRAD;	
	DXF_LONG	m_lFILLMODE;	
	DXF_LONG	m_lFLATLAND;	
	DXF_LONG	m_lHANDLING;	
	DXF_LONG	m_lHANDSEED;	

	SDXF_Point3D	m_sW3DINSBASE;	
	DXF_LONG	m_lLIMCHECK;		
	SDXF_Point2D	m_sW2DLIMMAX;	

	SDXF_Point2D	m_sW2DLIMMIN;	

	DXF_DOUBLE	m_dLTSCALE;			
	DXF_LONG	m_lLUNITS;			
									
									
									
									
									
	DXF_LONG	m_lLUPREC;			
	DXF_LONG	m_lMAXACTVP;		
	DXF_LONG	m_lMEASUREMENT;		
									
									
	DXF_CHAR	m_pszMENU[MAX_STRING_LEN];
	DXF_LONG	m_lMIRRTEXT;		
	DXF_LONG	m_lORTHOMODE;		
	DXF_LONG	m_lOSMODE;			
	DXF_LONG	m_lPDMODE;			
	DXF_DOUBLE	m_dPDSIZE;			
	DXF_DOUBLE	m_dPELEVATION;		
	SDXF_Point3D	m_sW3DPEXTMAX;	
	SDXF_Point3D	m_sW3DPEXTMIN;	
	DXF_LONG	m_lPICKSTYLE;		

									
									

									

									

	SDXF_Point3D	m_sW3DPINSBASE;	
	DXF_LONG	m_lPLIMCHECK;		
	SDXF_Point2D	m_sW2DPLIMMAX;	
	SDXF_Point2D	m_sW2DPLIMMIN;	
	DXF_LONG	m_lPLINEGEN;	
								
								
	DXF_DOUBLE	m_dPLINEWID;	

	DXF_LONG	m_lPROXYGRAPHICS;
								
								
	DXF_LONG	m_lPSLTSCALE;	
								


	DXF_CHAR	m_pszPUCSNAME[MAX_STRING_LEN];	
	SDXF_Point3D	m_sW3DPUCSORG;	 
	SDXF_Point3D	m_sW3DPUCSXDIR;	 

	SDXF_Point3D	m_sW3DPUCSYDIR;	 

	DXF_LONG	m_lQTEXTMODE;	
	DXF_LONG	m_lREGENMODE;	
	DXF_LONG	m_lSAVEIMAGES;	
								
								
								
								
	DXF_LONG	m_lSHADEDGE;	
								
								
								

								

	DXF_LONG	m_lSHADEDIF;	
	DXF_DOUBLE	m_dSKETCHINC;	
	DXF_LONG	m_lSKPOLY;		
								
	DXF_LONG	m_lSPLFRAME;	
								
								
	DXF_LONG	m_lSPLINESEGS;	

	DXF_LONG	m_lSPLINETYPE;	

								
								
	DXF_LONG	m_lSURFTAB1;	
	DXF_LONG	m_lSURFTAB2;	
	DXF_LONG	m_lSURFTYPE;	

								
								
								
	DXF_LONG	m_lSURFU;		
	DXF_LONG	m_lSURFV;		
	DXF_DOUBLE	m_dTDCREATE;	
	DXF_DOUBLE	m_dTDINDWG;		
	DXF_DOUBLE	m_dTDUPDATE;	
	DXF_DOUBLE	m_dTDUSRTIMER;	
	DXF_DOUBLE	m_dTEXTSIZE;	
	DXF_CHAR	m_pszTEXTSTYLE[MAX_STRING_LEN];	
	DXF_DOUBLE	m_dTHICKNESS;	
	DXF_LONG	m_lTILEMODE;	
								
								
	DXF_DOUBLE	m_dTRACEWID;	

	DXF_LONG	m_lTREEDEPTH;	
								
								
								
								
								
	DXF_CHAR	m_pszUCSNAME[MAX_STRING_LEN];	
	SDXF_Point3D	m_sW3DUCSORG;	
	SDXF_Point3D	m_sW3DUCSXDIR;	

	SDXF_Point3D	m_sW3DUCSYDIR;	

	DXF_LONG	m_lUNITMODE;	
								
	DXF_LONG	m_lUSERI1;		
	DXF_LONG	m_lUSERI2;		
	DXF_LONG	m_lUSERI3;		
	DXF_LONG	m_lUSERI4;		
	DXF_LONG	m_lUSERI5;		
	DXF_DOUBLE	m_dUSERR1;		
	DXF_DOUBLE	m_dUSERR2;		
	DXF_DOUBLE	m_dUSERR3;		
	DXF_DOUBLE	m_dUSERR4;		
	DXF_DOUBLE	m_dUSERR5;		
	DXF_LONG	m_lUSRTIMER;	
								
								
	DXF_LONG	m_lVISRETAIN;
	DXF_LONG	m_lWORLDVIEW;	
};

#endif // !defined(AFX_IMPORTHEADERSECTION_H_)
