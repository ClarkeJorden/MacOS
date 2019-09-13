#include "stdafx.h"
#include "CAD_Uds.h"

CCAD_Uds::CCAD_Uds(CCAD_Doc * pDoc, HANDLE hHeap, short nSpaceDoc, short nID)
{
	Init();
	m_pDoc = pDoc;
	m_hHeap = hHeap;
	m_nID = nID;
	m_nSpaceDoc = nSpaceDoc;
	m_pMgrEntity = NULL;
	m_pSceneGraph = NULL;
	m_pMgrEntity = new CCAD_MgrEntity(m_hHeap, pDoc, nSpaceDoc, nID, true);
}

CCAD_Uds::CCAD_Uds(const CCAD_Uds *pStyle)
{
	CopyFrom(pStyle);
	m_pMgrEntity = new CCAD_MgrEntity(m_hHeap, m_pDoc, m_nSpaceDoc, m_nID, true);
}

CCAD_Uds::~CCAD_Uds()
{
	if ( m_pMgrEntity != NULL )
		delete m_pMgrEntity;
	if(m_pSceneGraph)
		delete m_pSceneGraph;
}

CCAD_Uds & CCAD_Uds::operator =(const CCAD_Uds & cStyle)
{
	CopyFrom(&cStyle);

	return *this;
}

void CCAD_Uds::Init()
{
	CCAD_Style::Init();
	m_wState = UDS_RELATIVE | UDS_CURSORMARK;
	m_wCrntPlan = UCS_PLAN_XY;
	m_fWorkHeight = (CAD_FLOAT)0.0;

	m_cGrid.m_cGridSize.m_fX = CAMERA_WIDTH / 2;
	m_cGrid.m_cGridSize.m_fY = m_cGrid.m_cGridSize.m_fX;
	m_cGrid.m_cGridSize.m_fZ = m_cGrid.m_cGridSize.m_fX;
	m_cGrid.m_cGridStep.m_fX = (CAD_FLOAT)CAMERA_GRID_WIDTH;
	m_cGrid.m_cGridStep.m_fY = (CAD_FLOAT)CAMERA_GRID_WIDTH;
	m_cGrid.m_cGridStep.m_fZ = (CAD_FLOAT)CAMERA_GRID_WIDTH;
	m_cGrid.m_cSnapStep.m_fX = (CAD_FLOAT)CAMERA_SNAP_WIDTH;
	m_cGrid.m_cSnapStep.m_fY = (CAD_FLOAT)CAMERA_SNAP_WIDTH;
	m_cGrid.m_cSnapStep.m_fZ = (CAD_FLOAT)CAMERA_SNAP_WIDTH;	
	m_cGrid.m_rgbColor = RGB(0, 0, 255);
	m_cGrid.m_wFlag = GRID_DISPLAY;
	
	m_cCoord3D.m_cOrg = CCAD_Vector(0,0,0);
	m_cCoord3D.m_cX = CCAD_Vector(1,0,0);
	m_cCoord3D.m_cY = CCAD_Vector(0,1,0);
	m_cCoord3D.m_cZ = CCAD_Vector(0,0,1);
	m_cCoord3D.m_fScale = 1;
}

void CCAD_Uds::CopyFrom(const CCAD_Style *pStyle)
{
	CCAD_Style::CopyFrom(pStyle);

	CCAD_Uds	*pUds = (CCAD_Uds *) pStyle;
	m_wState = pUds->m_wState;
	m_wCrntPlan = pUds->m_wCrntPlan;
	m_cGrid = pUds->m_cGrid;
	m_cCoord3D = pUds->m_cCoord3D;

	m_pDoc = pUds->m_pDoc;
	m_hHeap = pUds->m_hHeap;
	m_nSpaceDoc = pUds->m_nSpaceDoc;
	m_nID = pUds->m_nID;
}

BOOL CCAD_Uds::Read(CCAD_File & cFile)
{
	return TRUE;
}

WORD CCAD_Uds::GetState(WORD wState) const
{
	return m_wState & wState;
}

void CCAD_Uds::SetState(WORD wState, BOOL blSet)
{
	WORD	wZeroState = 0xff ^ wState;
	m_wState &= wZeroState;
	if (blSet)
		m_wState |= wState;
}

void CCAD_Uds::SetPlan(WORD wNewPlan)
{
	int nPlan = (int)wNewPlan - (int)m_wCrntPlan;
	
	if (nPlan == 0)
		return;
	else if(nPlan < 0)
		nPlan +=  UCS_PLAN_ZX - UCS_PLAN_XY + 1; 
	
	CCAD_Vector  sVect;
	int	i;
	for (i = 0; i < nPlan; i++)
	{ 
		sVect = m_cCoord3D.m_cX;
		m_cCoord3D.m_cX = m_cCoord3D.m_cY;
		m_cCoord3D.m_cY = m_cCoord3D.m_cZ;
		m_cCoord3D.m_cZ = sVect;

		m_cGrid.m_cGridSize.RotateLeft();
		m_cGrid.m_cGridStep.RotateLeft();
		m_cGrid.m_cSnapStep.RotateLeft();
	}

	m_wCrntPlan = wNewPlan;
}

void CCAD_Uds::GetBaseUdsCoord(CCAD_Coord3D & cCoord)
{
	cCoord = m_cCoord3D;
	if (m_wCrntPlan != UCS_PLAN_XY) {
		CCAD_Vector sVect; 
		int	i;
		for (i = 0; i < m_wCrntPlan; i++) { 
			sVect = cCoord.m_cZ;
			cCoord.m_cZ = cCoord.m_cY;
			cCoord.m_cY = cCoord.m_cX;
			cCoord.m_cX = sVect;
		}
	}
}

void CCAD_Uds::ConvToBase(SCAD_POINT& sOldPt)
{
	CCAD_Coord3D cCoord;
	
	GetBaseUdsCoord(cCoord);
	cCoord.ConvToBase(sOldPt);
}

void CCAD_Uds::ConvFromBase(SCAD_POINT& sOldPt)
{
	CCAD_Coord3D cCoord;
	
	GetBaseUdsCoord(cCoord);
	cCoord.ConvFromBase(sOldPt);
}


void CCAD_Uds::DrawAxisOfUCS(CCAD_DC * pDC)
{
	if (pDC->m_pCamera == NULL)
		return;

	CAD_FLOAT fLen = pDC->ConvScalarScrnToWorld(100);
	CAD_FLOAT fHeadW = fLen * (CAD_FLOAT)0.1;
	CAD_FLOAT fHeadH = fHeadW * (CAD_FLOAT)0.5;

	CCAD_Point cOrg = m_cCoord3D.m_cOrg;
	pDC->m_pCamera->ConvFromBase(cOrg);

	LOGPEN	pen[3];
	if( pDC->m_pCamera->IsInternalPt(cOrg) ){
		pDC->m_pCamera->ECStoVCS(cOrg);
		pen[0].lopnColor = RGB(255,0,0);
		pen[1].lopnColor = RGB(0,255,0);
		pen[2].lopnColor = RGB(0,0,255);
	}else{
		cOrg.m_fX = - pDC->m_pCamera->m_fWidth / (CAD_FLOAT)2.0 + fLen;
		cOrg.m_fY = - pDC->m_pCamera->m_fHeight / (CAD_FLOAT)2.0 + fLen;
		cOrg.m_fZ = - pDC->m_pCamera->m_fDistToEye + fLen;
		pen[0].lopnColor = RGB(255,160,160);
		pen[1].lopnColor = RGB(160,255,160);
		pen[2].lopnColor = RGB(160,160,255);
	}
	
	CCAD_Coord3D	cCoord3D = m_cCoord3D;
	cCoord3D.m_cOrg = cOrg;

    pDC->m_pCamera->ConvToBase(cCoord3D.m_cOrg);
	CCAD_Coord3D oldCamera = pDC->PushCamera(&cCoord3D);

	SCAD_POINT axisList[3] = {
		{fLen, 0, 0}, // x
		{0, fLen, 0}, // y
		{0, 0, fLen}, // z
	};

	SCAD_POINT heads[6] = {
		{fLen-fHeadW, fHeadH, 0}, // x
		{fLen-fHeadW, -fHeadH, 0}, // x

		{fHeadH, fLen-fHeadW, 0}, // y
		{-fHeadH, fLen-fHeadW, 0}, // y

		{fHeadH, 0, fLen-fHeadW}, // z
		{-fHeadH, 0, fLen-fHeadW,}, // z
	};
	
	CCAD_Point cPlus,cMinus,cAxis;

	/////// DspFile Disable //////////
	void* saveDspFile = pDC->m_pDspFile;
	pDC->m_pDspFile = NULL;
	pDC->m_bEnableViewBox = FALSE;

	//////////////////////////////////
	int i, j, k = m_wCrntPlan;
	for (i = 0, j = 0; i < 3; i++, j += 2)
	{
		pen[k].lopnStyle = PS_SOLID;
		pen[k].lopnWidth.x = 2;
		LOGPEN oldPen = pDC->SelectPen( pen[k] );

		cAxis = axisList[i];
		cPlus = heads[j];
		cMinus = heads[j+1];

		pDC->m_pCamera->ConvFromBase(cAxis);
		pDC->m_pCamera->ConvFromBase(cPlus);
		pDC->m_pCamera->ConvFromBase(cMinus);

		pDC->Lines_Begin(LINETYPE_STRIP);
		pDC->DrawLine(cOrg, cAxis);
		pDC->DrawLine(cAxis, cPlus);
		pDC->DrawLine(cAxis, cMinus);
		pDC->Lines_End();

		k ++;
		if (k == 3)
			k = 0;
		pDC->RestoreObject(oldPen);
	}

	pDC->RestorCamera(oldCamera);
	pDC->m_bEnableViewBox = TRUE;
	pDC->m_pDspFile = (CCAD_DspFile *)saveDspFile; // restore Display File 
}

BOOL CCAD_Uds::DrawEntityMgr(CCAD_DC * pCadDC, BOOL blRedraw)
{
	CCAD_Coord3D cECS;
// 	if (m_nSpaceDoc != DISABLE_INDEX)
// 		cECS = pCadDC->PushCamera(&m_cCoord3D);

	pCadDC->m_wPriority = 0;
	BOOL blResult = m_pMgrEntity->DrawEntity(pCadDC, blRedraw);

// 	if (m_nSpaceDoc != DISABLE_INDEX)
// 		pCadDC->RestorCamera(cECS);
	return blResult;
}

BOOL CCAD_Uds::GetCursorPos(CCAD_DC * pDC, const POINT &sLP, CCAD_Point* pPtUCS)
{
	CCAD_Point ptWCS1, ptWCS2;

	ptWCS1 = pDC->LPtoVCS(sLP);

	if (pDC->m_pCamera->m_bPerspect)
	{
		ptWCS2.m_fX = 0;
		ptWCS2.m_fY = 0;
		ptWCS2.m_fZ = 0;
	}
	else
	{
		ptWCS2.m_fX = ptWCS1.m_fX;
		ptWCS2.m_fY = ptWCS1.m_fY;
		ptWCS2.m_fZ = 0;
	}

	// VCS -> WCS.
	pDC->m_pCamera->ConvToBase(ptWCS1);
	pDC->m_pCamera->ConvToBase(ptWCS2);
	// WCS -> Uds.
	m_cCoord3D.ConvFromBase(ptWCS1);
	m_cCoord3D.ConvFromBase(ptWCS2);

	return GetCrossPt(ptWCS1,ptWCS2,*pPtUCS);
}

BOOL CCAD_Uds::GetCrossPt(const CCAD_Point& inPt1,  const CCAD_Point& inPt2,  CCAD_Point& outPt)
{
	// computing Vector of Line (pt1,pt2)
	CCAD_Vector delta = inPt2;
	delta -= inPt1;

	// check crossing 
	CAD_FLOAT fScalar = delta & CCAD_Vector(0,0,1); // scalar product

	if( IS_ZERO(fabs(fScalar)))
		return false;

	// computing Cross Point (line eqution)
	outPt.m_fX = delta.m_fX * (m_fWorkHeight - inPt1.m_fZ) / delta.m_fZ + inPt1.m_fX;
	outPt.m_fY = delta.m_fY * (m_fWorkHeight - inPt1.m_fZ) / delta.m_fZ + inPt1.m_fY;
	outPt.m_fZ = m_fWorkHeight;

	return true;
}

