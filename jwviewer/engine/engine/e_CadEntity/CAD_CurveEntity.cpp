#include "stdafx.h"
#include "CAD_CurveEntity.h"
#include "CAD_Doc.h"

CCAD_CurveEntity::CCAD_CurveEntity(CCAD_Doc * pDoc, short nSpaceDoc, short nUds)
					: CCAD_Entity(pDoc, nSpaceDoc, nUds)
{
	m_bCode = C_CURVE;
	m_bFlag = E_DRAWING;
}

CCAD_CurveEntity::~CCAD_CurveEntity()
{
}

BOOL CCAD_CurveEntity::IsIgno(BOOL isBlock)
{
	if ( CCAD_Entity::IsIgno(isBlock) )
		return true;

	CCAD_GemCurve *pGem = (CCAD_GemCurve *)GetGemEntity();

	short nBorder;
	if (pGem->m_bKind == CURVE_3SPLINE)
		nBorder = 4;
	else
		nBorder = 3;

	if (pGem->m_nCurveTot < nBorder || pGem->m_pCurvePt == NULL)
		return TRUE;
	return FALSE;
}

long CCAD_CurveEntity::GetSize()
{
	long lSize = sizeof(CCAD_CurveEntity);
	lSize += sizeof(CCAD_Point)*m_cGemCurve.m_nCurveTot;
	lSize += sizeof(CAD_FLOAT)*m_cGemCurve.m_nCurveTot;
	lSize += sizeof(CAD_FLOAT)*m_cGemCurve.m_nKnotTot;
	return lSize;
}

LPSTR CCAD_CurveEntity::GetData1(LPSTR pEntity)
{
	return (pEntity + sizeof(CCAD_CurveEntity));
}

LPSTR CCAD_CurveEntity::GetData2(LPSTR pEntity)
{
	long	lSize = sizeof(CCAD_CurveEntity);
	lSize += sizeof(CCAD_Point)*m_cGemCurve.m_nCurveTot;
	return (pEntity + lSize);
}

LPSTR CCAD_CurveEntity::GetData3(LPSTR pEntity)
{
	long	lSize = sizeof(CCAD_CurveEntity);
	lSize += sizeof(CCAD_Point)*m_cGemCurve.m_nCurveTot;
	lSize += sizeof(CAD_FLOAT)*m_cGemCurve.m_nCurveTot;
	return (pEntity + lSize);
}

BOOL CCAD_CurveEntity::Read(CCAD_File & cFile)
{
	CCAD_Entity::Read(cFile);
	m_cPen.Read(cFile);
	m_cBrush.Read(cFile);
	return m_cGemCurve.Read(cFile);
}



