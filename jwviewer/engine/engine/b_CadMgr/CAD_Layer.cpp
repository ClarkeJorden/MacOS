#include "stdafx.h"
#include "CAD_Layer.h"

 

CCAD_Layer::CCAD_Layer()
{
	Init();
}

CCAD_Layer::CCAD_Layer(const CCAD_Layer *pStyle)
{
	CopyFrom(pStyle);
}


CCAD_Layer & CCAD_Layer::operator =(const CCAD_Layer &cStyle)
{
	CopyFrom(&cStyle);

	return *this;
}


void CCAD_Layer::Init()
{
	CCAD_Style::Init();
	m_wState = LAYER_ENABLE;
}


void CCAD_Layer::CopyFrom(const CCAD_Style *pStyle)
{
	CCAD_Style::CopyFrom(pStyle);

	CCAD_Layer	*pLayer = (CCAD_Layer *) pStyle;

	m_wState = pLayer->m_wState;
	m_cPen = pLayer->m_cPen;
	m_cBrush = pLayer->m_cBrush;
}


BOOL CCAD_Layer::Read(CCAD_File & cFile)
{
	CCAD_Style::Read(cFile);
	cFile.ReadWord(&m_wState);
	m_cPen.Read(cFile);
	m_cBrush.Read(cFile);
	return TRUE;
}


BOOL CCAD_Layer::IsVisible() const
{
	if ( (m_wState & LAYER_ON) == 0 )
		return false;
	return true;
}


WORD CCAD_Layer::GetState(WORD wState) const
{
	return (WORD) (m_wState & wState);
}


void CCAD_Layer::SetState(WORD wState, BOOL blSet)
{
	WORD	wZeroState = (WORD) (0xffff ^ wState);
	m_wState &= wZeroState;
	if (blSet)
		m_wState |= wState;
}

 
