#include "stdafx.h"
#include "CAD_Style.h"


CCAD_Style::CCAD_Style()
{
	Init();
}

CCAD_Style::CCAD_Style(const CCAD_Style *pStyle)
{
	CopyFrom(pStyle);
}

CCAD_Style::~CCAD_Style()
{
}


CCAD_Style & CCAD_Style::operator =(const CCAD_Style &cStyle)
{
	CopyFrom(&cStyle);

	return *this;
}


void CCAD_Style::Init()
{
	m_szName[0] = '\0';
}

BOOL CCAD_Style::IsEqual(const CCAD_Style *pStyle)
{
	return (strcmp(m_szName, pStyle->m_szName) == 0);
}


void CCAD_Style::CopyFrom(const CCAD_Style *pStyle)
{
	sprintf(m_szName, "%s", pStyle->m_szName);
}


BOOL CCAD_Style::Read(CCAD_File &cFile)
{
	cFile.ReadString(m_szName, EXT_LENGTH_NAME);
	return TRUE;
}

BOOL CCAD_Style::IsEqualName(const CCAD_Style *pStyle)
{
	return (strcmp(m_szName, pStyle->m_szName) == 0);
}
 
