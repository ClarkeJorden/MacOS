#include "stdafx.h"
#include "CAD_DxfGlobal.h"
#include "CAD_Style.h"
#include "DXF_Object.h"
//#include "DXF_GroupObject.h"
#include "CAD_Global.h"

// BOOL ImportDxfGroup(CCAD_Doc *pDoc, CDXF_GroupObject *pDxfGroup)
// {
// //	pDxfGroup->m_pszGroupName
// //	pDxfGroup->m_cEntityIndexArray.GetSize();
// 
// 	return TRUE;
// }

BOOL ImportDxfObjectSection(CDXF_MemFile * pDxfMemFile, CCAD_Doc *pDoc)
{
//	long lCount, i;
// 	lCount = pDxfMemFile->DXFGetMLineStyleCnt();
// 	for (i=0; i<lCount; i++){
// 		CDXF_MLineStyleObject*	pDxfMLineStyle;
// 		pDxfMLineStyle = pDxfMemFile->DXFGetMLineStyle(i);
// 		if (!ImportDxfMLineStyle(pDoc, pDxfMLineStyle))
// 			return FALSE;
// 	}

// 	lCount = pDxfMemFile->DXFGetGroupCnt();
// 	for (i=0; i<lCount; i++){
// 		CDXF_GroupObject*	pDxfGroup;
// 		pDxfGroup = pDxfMemFile->DXFGetGroup(i);
// 		if (!ImportDxfGroup(pDoc, pDxfGroup))
// 			return FALSE;
// 	}

	return TRUE;
}
