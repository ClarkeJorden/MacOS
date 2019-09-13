#include "stdafx.h"
#include "CAD_GemEntity.h"

CCAD_GemEntity::CCAD_GemEntity()
{
	m_cBcs.m_fScale = 1;
}

CCAD_GemEntity::~CCAD_GemEntity()
{

}

CCAD_GemEntity & CCAD_GemEntity::operator=(const CCAD_GemEntity & cGemEntity)
{
	m_cBcs = cGemEntity.m_cBcs;
	return *this;
}

void CCAD_GemEntity::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR, LPSTR, LPSTR)
{
	ASSERT(pGemEntity != NULL);
	pGemEntity->m_cBcs = m_cBcs;
}

BOOL CCAD_GemEntity::Read(CCAD_File& cFile)
{
	m_cBcs.Read(cFile);
	return TRUE;
}

//----------------------------------------------------------
BOOL CCAD_GemEntity::Drawing(CCAD_DC* pCadDC, BYTE bState, BOOL blRedraw)
{
	ASSERT(pCadDC != NULL);
	//	set body coordinate.
	CCAD_Coord3D cECS = pCadDC->PushCamera(&m_cBcs);
	//	set thick flag.
	CAD_FLOAT fOldThick = pCadDC->SelectThick(GetThick());
	//	set closed flag.
	BOOL blOldClosed = pCadDC->SelectClose(IsClosed());
	//-------------------------------------
	BOOL blResult;
	blResult = Draw(pCadDC, blRedraw);

	//	set old flags.
 	pCadDC->SelectThick(fOldThick);
 	pCadDC->SelectClose(blOldClosed);
	//-------------------------------------
	pCadDC->RestorCamera(cECS);
	return blResult;
}

//----------------------------------------------------------------------
void CCAD_GemEntity::CountVCSRange(CCAD_DC *pCadDC, CCAD_Rect& cRange)
{
	ASSERT(pCadDC != NULL);
	//	set body coordinate.
	CCAD_Coord3D cECS = pCadDC->PushCamera(&m_cBcs);
	//	set thick flag.
	CAD_FLOAT fOldThick = pCadDC->SelectThick(GetThick());
	//-------------------------------------
	CountVcsRange(pCadDC, cRange);
	//	set old flags.
	pCadDC->SelectThick(fOldThick);
	//-------------------------------------
	pCadDC->RestorCamera(cECS);
}

void CCAD_GemEntity::ConvToBase(SCAD_POINT &sPt) const
{
	CCAD_Vector	cPt;
	cPt = sPt;
	m_cBcs.ConvToBase(cPt);
	sPt = cPt;
}

void CCAD_GemEntity::ConvFromBase(SCAD_POINT &sPt) const
{
	CCAD_Vector	cPt;
	cPt = sPt;
	m_cBcs.ConvFromBase(cPt);
	sPt = cPt;
}
void CCAD_GemEntity::Mirror(CCAD_Coord3D& cCoord)
{
	cCoord.ThisToChild(&m_cBcs);
	//---------------------------------------
	m_cBcs.m_cOrg.m_fX = -m_cBcs.m_cOrg.m_fX;
	m_cBcs.m_cX.m_fX = -m_cBcs.m_cX.m_fX;
	m_cBcs.m_cY.m_fX = -m_cBcs.m_cY.m_fX;
	m_cBcs.m_cZ.m_fX = -m_cBcs.m_cZ.m_fX;
	//---------------------------------------
	cCoord.ChildToThis(&m_cBcs);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCAD_Gem2DEntity::CCAD_Gem2DEntity()
{
}

CCAD_Gem2DEntity::~CCAD_Gem2DEntity()
{
}

CCAD_Gem2DEntity & CCAD_Gem2DEntity::operator=(const CCAD_Gem2DEntity & cGem2DEntity)
{
	CCAD_GemEntity::operator=(cGem2DEntity);
	return *this;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCAD_Gem3DEntity::CCAD_Gem3DEntity()
{
	m_nTexture = DISABLE_INDEX;
}

CCAD_Gem3DEntity::~CCAD_Gem3DEntity()
{
}

CCAD_Gem3DEntity & CCAD_Gem3DEntity::operator=(const CCAD_Gem3DEntity & cGem3DEntity)
{
	CCAD_GemEntity::operator=(cGem3DEntity);
	
	m_nTexture = cGem3DEntity.m_nTexture;
	return *this;
}

void CCAD_Gem3DEntity::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_GemEntity::MemCopyTo(pGemEntity, pData1, pData2, pData3);
	((CCAD_Gem3DEntity *)pGemEntity)->m_nTexture = m_nTexture;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCAD_GemThickEntity::CCAD_GemThickEntity()
{
}

CCAD_GemThickEntity::~CCAD_GemThickEntity()
{
}

CCAD_GemThickEntity & CCAD_GemThickEntity::operator=(const CCAD_GemThickEntity & cGemThickEntity)
{
	CCAD_Gem3DEntity::operator=(cGemThickEntity);
	
	m_cThick = cGemThickEntity.m_cThick;
	return *this;
}

void CCAD_GemThickEntity::MemCopyTo(CCAD_GemEntity* pGemEntity, LPSTR pData1, LPSTR pData2, LPSTR pData3)
{
	CCAD_Gem3DEntity::MemCopyTo(pGemEntity, pData1, pData2, pData3);
	((CCAD_GemThickEntity *)pGemEntity)->m_cThick = m_cThick;
}

BOOL CCAD_GemThickEntity::Read(CCAD_File & cFile)
{
	CCAD_Gem3DEntity::Read(cFile);
	
	m_cThick.Read(cFile);
	return TRUE;
}


void CCAD_GemThickEntity::CountBodyRange(CCAD_Rect& cRange)
{
	CCAD_Gem3DEntity::CountBodyRange(cRange);
	
	if (m_cThick.m_fZ < CAD_ZERO)
		cRange.m_fMinZ = m_cThick.m_fZ;
	else
		cRange.m_fMaxZ = m_cThick.m_fZ;
}
