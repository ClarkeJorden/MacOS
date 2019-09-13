#include "stdafx.h"
#include "resource.h"
#include "DXF_Utils.h"
#include "DXF_Class.h"

CDXF_Class::CDXF_Class()
{
	memset(m_pszClassName, 0x00, MAX_STRING_LEN);
	memset(m_pszPlusClassName, 0x00, MAX_STRING_LEN);
	memset(m_pszAppName, 0x00, MAX_STRING_LEN);
	m_lClassVer = 0;
	m_lProxyFlag = 0;
	m_lEntityFlag = 0;
}

CDXF_Class::CDXF_Class(DXF_CHAR* pszClassName, DXF_CHAR* pszPlusClassName, DXF_CHAR* pszAppName, DXF_LONG lClassVer, DXF_LONG lProxyFlag, DXF_LONG lEntityFlag)
{
	strcpy(m_pszClassName, pszClassName);
	strcpy(m_pszPlusClassName, pszPlusClassName);
	strcpy(m_pszAppName, pszAppName);
	m_lClassVer = lClassVer;
	m_lProxyFlag = lProxyFlag;
	m_lEntityFlag = lEntityFlag;
}

CDXF_Class::~CDXF_Class()
{

}

BOOL CDXF_Class::DXFReadClass(FILE* pFile)
{
	return TRUE;
}

