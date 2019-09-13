#include "stdafx.h"

#include "DXF_Utils.h"
#include "DXF_Entity.h"

CDXF_Entity::CDXF_Entity()
{
	m_lSpaceType = DXF_SPACE_PAPER; //DXF_SPACE_MODEL;
	memcpy(m_pszLTypeName, "BYLAYER", MAX_STRING_LEN);
	memcpy(m_pszLayerName, "0", MAX_STRING_LEN);
	m_lDXFColor = DXF_COLOR_BYLAYER;
	m_dLTypeScale = 1.0;
	m_lVisibility = DXF_VISIBILITY_ON;

	m_s3DExtrusion.dZ = 1.0;
}

CDXF_Entity::~CDXF_Entity()
{

}

DXF_BOOL CDXF_Entity::DXFReadEntity(FILE * /*pFile*/)
{
	return TRUE;
}

void CDXF_Entity::DXFGenBCS()
{
	DXF_GenBCSFromOCS(m_sBCS, m_s3DExtrusion);
}

void CDXF_Entity::DXFGenOCS()
{
}
