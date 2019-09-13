
#include "stdafx.h"
#include "CAD_TextStyle.h"
#include "resource.h"


 

CCAD_TextStyle::CCAD_TextStyle()
{
	Init();
}

CCAD_TextStyle::CCAD_TextStyle(const CCAD_TextStyle *pStyle)
{
	CopyFrom(pStyle);
}

CCAD_TextStyle & CCAD_TextStyle::operator =(const CCAD_TextStyle & cStyle)
{
	CopyFrom(&cStyle);

	return *this;
}

BOOL CCAD_TextStyle::operator ==(const CCAD_TextStyle & cTextStyle)
{
	if (strcmp(cTextStyle.m_szFaceName, m_szFaceName) != 0)
		return FALSE;
	if (m_sEffect.bold != cTextStyle.m_sEffect.bold || m_sEffect.italic != cTextStyle.m_sEffect.italic ||
		m_sEffect.under != cTextStyle.m_sEffect.under || m_sEffect.middle != cTextStyle.m_sEffect.middle ||
		m_sEffect.upside != cTextStyle.m_sEffect.upside || m_sEffect.back != cTextStyle.m_sEffect.back ||
		m_sEffect.vert != cTextStyle.m_sEffect.vert)
			return FALSE;
	if (m_bCharSet != cTextStyle.m_bCharSet || m_bOutPrecision != cTextStyle.m_bOutPrecision || m_bClipPrecision != cTextStyle.m_bClipPrecision ||
		m_bQuality != cTextStyle.m_bQuality || m_bPitchAndFamily != cTextStyle.m_bPitchAndFamily )
			return FALSE;
	if (IS_ZERO(m_fHeight - cTextStyle.m_fHeight) && IS_ZERO(m_fWidth - cTextStyle.m_fWidth) &&
		IS_ZERO(m_fAngle - cTextStyle.m_fAngle))
		return TRUE;

	return FALSE;
}

void CCAD_TextStyle::Init()
{
	CCAD_Style::Init();

	strcpy(m_szFaceName, FONT_DEFAULTNAME);

	memset(&m_sEffect, 0, sizeof (m_sEffect));
	m_fHeight = 1000;
	m_fWidth = 1000;
	m_fAngle = 0;
	m_bCharSet = 0;
	m_bOutPrecision = 0;
	m_bClipPrecision = 0;
	m_bQuality = 0;
	m_bPitchAndFamily = 0;
}

void CCAD_TextStyle::CopyFrom(const CCAD_Style *pStyle)
{
	CCAD_Style::CopyFrom(pStyle);

	CCAD_TextStyle	*pText = (CCAD_TextStyle *) pStyle;

	sprintf(m_szFaceName, "%s", pText->m_szFaceName);
	m_sEffect = pText->m_sEffect;
	m_fHeight = pText->m_fHeight;
	m_fWidth = pText->m_fWidth;
	m_fAngle = pText->m_fAngle;

	m_bCharSet = pText->m_bCharSet;
	m_bOutPrecision = pText->m_bOutPrecision;
	m_bClipPrecision = pText->m_bClipPrecision;
	m_bQuality = pText->m_bQuality;
	m_bPitchAndFamily = pText->m_bPitchAndFamily;
}

BOOL CCAD_TextStyle::IsEqual(const CCAD_Style *pStyle)
{
	if ( strlen(pStyle->m_szName) > 0 ) {
		if (strcmp(m_szName, pStyle->m_szName) == 0)
			return true;
	}

	if (strcmp(m_szFaceName, ((CCAD_TextStyle*)pStyle)->m_szFaceName) != 0)
		return false;
	if (m_fHeight != ((CCAD_TextStyle*)pStyle)->m_fHeight)
		return false;
	if (m_fWidth != ((CCAD_TextStyle*)pStyle)->m_fWidth)
		return false;
	if (m_fAngle != ((CCAD_TextStyle*)pStyle)->m_fAngle)
		return false;
	if (m_bCharSet != ((CCAD_TextStyle*)pStyle)->m_bCharSet)
		return false;
	if (m_bOutPrecision != ((CCAD_TextStyle*)pStyle)->m_bOutPrecision)
		return false;
	if (m_bClipPrecision != ((CCAD_TextStyle*)pStyle)->m_bClipPrecision)
		return false;
	if (m_bQuality != ((CCAD_TextStyle*)pStyle)->m_bQuality)
		return false;
	if (m_bPitchAndFamily != ((CCAD_TextStyle*)pStyle)->m_bPitchAndFamily)
		return false;

	if (m_sEffect.bold != ((CCAD_TextStyle*)pStyle)->m_sEffect.bold)
		return false;
	if (m_sEffect.italic != ((CCAD_TextStyle*)pStyle)->m_sEffect.italic)
		return false;
	if (m_sEffect.middle != ((CCAD_TextStyle*)pStyle)->m_sEffect.middle)
		return false;
	if (m_sEffect.under != ((CCAD_TextStyle*)pStyle)->m_sEffect.under)
		return false;
	if (m_sEffect.upside != ((CCAD_TextStyle*)pStyle)->m_sEffect.upside)
		return false;
	if (m_sEffect.back != ((CCAD_TextStyle*)pStyle)->m_sEffect.back)
		return false;
	if (m_sEffect.vert != ((CCAD_TextStyle*)pStyle)->m_sEffect.vert)
		return false;
	return true;
}

BOOL CCAD_TextStyle::Read(CCAD_File & cFile)
{
	CCAD_Style::Read(cFile);

	cFile.ReadString(m_szFaceName, LF_FACESIZE);
	cFile.ReadByte((BYTE*)&m_sEffect);
	if ( !cFile.m_blMacVersion ) {
		CADTEXTEFFECT tmp = m_sEffect;
		m_sEffect.bold = tmp.reserved;
		m_sEffect.italic = tmp.vert;
		m_sEffect.under = tmp.back;
		m_sEffect.middle = tmp.upside;
		m_sEffect.upside = tmp.middle;
		m_sEffect.back = tmp.under;
		m_sEffect.vert = tmp.italic;
		m_sEffect.reserved = tmp.bold;
	}

	cFile.ReadCAD_Float(&m_fHeight);
	cFile.ReadCAD_Float(&m_fWidth);
	cFile.ReadCAD_Float(&m_fAngle);

	cFile.ReadByte(&m_bCharSet);
	cFile.ReadByte(&m_bOutPrecision);
	cFile.ReadByte(&m_bClipPrecision);
	cFile.ReadByte(&m_bQuality);
	cFile.ReadByte(&m_bPitchAndFamily);

	return TRUE;
}

void CCAD_TextStyle::SetCadFont(const LOGFONTA/*LOGFONT*/ & sLogFont)
{
	m_sEffect.italic = sLogFont.lfItalic;
	m_sEffect.bold = (sLogFont.lfWeight & FW_BOLD) ? 1 : 0;
	m_sEffect.under = sLogFont.lfUnderline;
	m_sEffect.middle = sLogFont.lfStrikeOut;
	strcpy(m_szFaceName, sLogFont.lfFaceName);
}

void CCAD_TextStyle::SetLogFont(LOGFONTA/*LOGFONT*/ & sLogFont)	// use in Mac.
{
	sLogFont.lfItalic = m_sEffect.italic;
	if (m_sEffect.bold == 1)
		sLogFont.lfWeight = FW_BOLD;
	else
		sLogFont.lfWeight = FW_NORMAL;
	sLogFont.lfUnderline = m_sEffect.under;
	sLogFont.lfStrikeOut = m_sEffect.middle;
	strcpy(sLogFont.lfFaceName, m_szFaceName);
}

 
