#include "stdafx.h"
#include "DXF_Utils.h"
#include "DXF_ClassesSection.h"

CDXF_ClassesSection::CDXF_ClassesSection()
{
}

CDXF_ClassesSection::~CDXF_ClassesSection()
{

}

DXF_BOOL CDXF_ClassesSection::DXFReadClassesSection(FILE* pFile)
{
	DXF_LONG	lCode;
	DXF_CHAR	szValue[MAX_STRING_LEN];

	do {
		if (!DXFReadGroup(pFile, lCode, szValue))
			return FALSE;
	} while (lCode != 0 || strcmp(szValue, "ENDSEC") !=0);

	return TRUE;
}

