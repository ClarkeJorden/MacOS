#include "stdafx.h"
#include "resource.h"
#include "CAD_StyleMgr.h"


CCAD_StyleMgr::CCAD_StyleMgr()
{
	m_nAct = -1;
}


CCAD_StyleMgr::~CCAD_StyleMgr()
{
	RemoveAll();
}


BOOL CCAD_StyleMgr::Add(CCAD_Style *pStyle)
{
	if ( pStyle != NULL ) {
		if ( GetNameIndex(pStyle) != -1 ) {
			return FALSE;
		}
		CCAD_Style * pNewStyle = NewStyle();
		if ( pNewStyle == NULL )
			return FALSE;
		pNewStyle->CopyFrom(pStyle);
		m_cArray.Add(pNewStyle);
		if (GetActStyle() == NULL)
			m_nAct = m_cArray.GetSize() - 1;
	}
	return TRUE;
}

BOOL CCAD_StyleMgr::Read(CCAD_File & cFile)
{
	short		nTotal;
	CCAD_Style *pStyle;

	cFile.ReadShort(&nTotal);
	cFile.ReadShort(&m_nAct);

	if (m_nAct == -1 && nTotal > 0)
		m_nAct = 0;
	
	short	i;
	for (i = 0; i < nTotal; i ++) {
		if (NULL == (pStyle = NewStyle())) {
			RemoveAll();
			return FALSE;
		}
		pStyle->Read(cFile);
		m_cArray.Add(pStyle);
	}
	return TRUE;
}


short CCAD_StyleMgr::Append(CCAD_Style *pStyle)
{
	if (pStyle == NULL)
		return -1;

	short	nTotal = TotIndex();
	if (nTotal > 0) {
		CCAD_Style	 *pStyle0;
		for (short i = 0; i < nTotal; i ++) {
			pStyle0 = (CCAD_Style *) m_cArray[i];
			if (!pStyle0->IsDeleted() && pStyle->IsEqual(pStyle0))
				return i;
		}
	}

	CCAD_Style	*pNewStyle = NewStyle();
	if (pNewStyle == NULL)
		return -1;
	pNewStyle->CopyFrom(pStyle);

	if ( strlen(pStyle->m_szName) == 0 )
		sprintf( pNewStyle->m_szName, "%d", nTotal );

	m_cArray.Add(pNewStyle);
	if (GetActStyle() == NULL)
		m_nAct = m_cArray.GetSize() - 1;
	return nTotal;
}


void CCAD_StyleMgr::RemoveAll()
{
	int nTotal = m_cArray.GetSize();
	if (nTotal > 0) {
		CCAD_Style	 *pStyle;
		short	i;
		for (i = 0; i < nTotal; i ++) {
			pStyle = (CCAD_Style *) m_cArray[i];
			delete pStyle;
		}
		m_cArray.RemoveAll();
	}
	m_nAct = -1;
}

short CCAD_StyleMgr::GetIndex(CCAD_Style *pDstStyle) const
{
	int nTotal = m_cArray.GetSize();
	CCAD_Style *pStyle;
	int	i;
	for (i = 0; i < nTotal; i ++) {
		pStyle = (CCAD_Style *) m_cArray[i];
		if (pStyle == pDstStyle)
			return (short)i;
	}
	return -1;
}


short CCAD_StyleMgr::GetNameIndex(CCAD_Style *pDstStyle) const
{
	int nTotal = m_cArray.GetSize();
	CCAD_Style *pStyle;
	int	i;
	for (i = 0; i < nTotal; i ++) {
		pStyle = (CCAD_Style *) m_cArray[i];
		if (pStyle->IsEqualName(pDstStyle))
			return (short)i;
	}
	return -1;
}

short CCAD_StyleMgr::GetNameIndex(LPCSTR lpszName) const
{
	int nTotal = m_cArray.GetSize();
	CCAD_Style	 *pStyle;
	int	i;
	for (i = 0; i < nTotal; i ++) {
		pStyle = (CCAD_Style *) m_cArray[i];
		if (strcmp(pStyle->m_szName, lpszName) == 0)
			return (short)i;
	}
	return -1;
}

CCAD_Style * CCAD_StyleMgr::GetStyle(int nIndex) const
{
	if (nIndex < 0 || nIndex >= TotIndex())
		return NULL;
	return (CCAD_Style *) m_cArray[nIndex];
}

CCAD_Style * CCAD_StyleMgr::GetStyle(LPCSTR pszName) const
{
	return GetStyle(GetNameIndex(pszName));
}

CCAD_Style * CCAD_StyleMgr::GetActStyle() const
{
	if (m_nAct >= 0 && m_nAct < TotIndex() ) {
		CCAD_Style *pStyle = (CCAD_Style *) m_cArray.GetAt(m_nAct);
		return pStyle;
	}
	return NULL;
}

 
