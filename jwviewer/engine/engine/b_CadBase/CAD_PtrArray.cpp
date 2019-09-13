
#include "CAD_PtrArray.h"
#include <string.h>

CCAD_PtrArray::CCAD_PtrArray()
{
	m_pData = NULL;
	m_nSize = 0;
    m_nMaxSize = 0;
    m_nGrowBy = 0;
}

CCAD_PtrArray::~CCAD_PtrArray()
{
	ASSERT_VALID(this);

	delete[] (BYTE*)m_pData;
}

void CCAD_PtrArray::SetSize(INT_PTR nNewSize, INT_PTR nGrowBy)
{
	ASSERT_VALID(this);
	ASSERT(nNewSize >= 0);

	if(nNewSize < 0 )
		AfxThrowInvalidArgException();

	if (nGrowBy >= 0)
		m_nGrowBy = nGrowBy;  // set new size

	if (nNewSize == 0)
	{
		// shrink to nothing
		delete[] (BYTE*)m_pData;
		m_pData = NULL;
		m_nSize = 0;
        m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		ASSERT(nNewSize <= SIZE_T_MAX/sizeof(void*));    // no overflow
#endif
		m_pData = (void**) new BYTE[nNewSize * sizeof(void*)];

		memset(m_pData, 0, nNewSize * sizeof(void*));  // zero fill

		m_nSize = nNewSize;
        m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements

			memset(&m_pData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(void*));

		}

		m_nSize = nNewSize;
	}
	else
	{
		// otherwise, grow array
		INT_PTR nGrowArrayBy = m_nGrowBy;
		if (nGrowArrayBy == 0)
		{
			// heuristically determine growth when nGrowArrayBy == 0
			//  (this avoids heap fragmentation in many situations)
			nGrowArrayBy = _min(1024, _max(4, m_nSize / 8));
		}
		INT_PTR nNewMax;
		if (nNewSize < m_nMaxSize + nGrowArrayBy)
			nNewMax = m_nMaxSize + nGrowArrayBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

		ASSERT(nNewMax >= m_nMaxSize);  // no wrap around

		if(nNewMax  < m_nMaxSize)
			AfxThrowInvalidArgException();

#ifdef SIZE_T_MAX
		ASSERT(nNewMax <= SIZE_T_MAX/sizeof(void*)); // no overflow
#endif
		void** pNewData = (void**) new BYTE[nNewMax * sizeof(void*)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(void*));

		// construct remaining elements
		ASSERT(nNewSize > m_nSize);

		memset(&pNewData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(void*));

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

INT_PTR CCAD_PtrArray::Append(const CCAD_PtrArray& src)
{
	ASSERT_VALID(this);
	ASSERT(this != &src);   // cannot append to itself

	if(this == &src)
		AfxThrowInvalidArgException();

	INT_PTR nOldSize = m_nSize;
	SetSize(m_nSize + src.m_nSize);

	memcpy(m_pData + nOldSize, src.m_pData, src.m_nSize * sizeof(void*));

	return nOldSize;
}

void CCAD_PtrArray::Copy(const CCAD_PtrArray& src)
{
	ASSERT_VALID(this);
	ASSERT(this != &src);   // cannot append to itself

	if(this != &src)
	{
		SetSize(src.m_nSize);

		memcpy(m_pData, src.m_pData, src.m_nSize * sizeof(void*));
	}

}

void CCAD_PtrArray::FreeExtra()
{
	ASSERT_VALID(this);

	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		ASSERT(m_nSize <= SIZE_T_MAX/sizeof(void*)); // no overflow
#endif
		void** pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (void**) new BYTE[m_nSize * sizeof(void*)];

			// copy new data from old 
			memcpy(pNewData, m_pData, m_nSize * sizeof(void*));
		}

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

/////////////////////////////////////////////////////////////////////////////

void CCAD_PtrArray::SetAtGrow(INT_PTR nIndex, void* newElement)
{
	ASSERT_VALID(this);
	ASSERT(nIndex >= 0);

	if(nIndex < 0)
		AfxThrowInvalidArgException();

	if (nIndex >= m_nSize)
		SetSize(nIndex+1);
	m_pData[nIndex] = newElement;
}





void CCAD_PtrArray::InsertAt(INT_PTR nIndex, void* newElement, INT_PTR nCount)
{

	ASSERT_VALID(this);
	ASSERT(nIndex >= 0);    // will expand to meet need
	ASSERT(nCount > 0);     // zero or negative size not allowed

	if(nIndex < 0 || nCount <= 0)
		AfxThrowInvalidArgException();

	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		SetSize(nIndex + nCount);  // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		INT_PTR nOldSize = m_nSize;
		SetSize(m_nSize + nCount);  // grow it to new size

		// shift old data up to fill gap 
		memmove(&m_pData[nIndex+nCount], &m_pData[nIndex], (nOldSize-nIndex) * sizeof(void*));

		// re-init slots we copied from
		memset(&m_pData[nIndex], 0, nCount * sizeof(void*));
	}

	// insert new value in the gap
	ASSERT(nIndex + nCount <= m_nSize);



	// copy elements into the empty space
	while (nCount--)
		m_pData[nIndex++] = newElement;

}



void CCAD_PtrArray::RemoveAt(INT_PTR nIndex, INT_PTR nCount)
{
	ASSERT_VALID(this);
	ASSERT(nIndex >= 0);
	ASSERT(nCount >= 0);
	INT_PTR nUpperBound = nIndex + nCount;
	ASSERT(nUpperBound <= m_nSize && nUpperBound >= nIndex && nUpperBound >= nCount);

	if(nIndex < 0 || nCount < 0 || (nUpperBound > m_nSize) || (nUpperBound < nIndex) || (nUpperBound < nCount))
		AfxThrowInvalidArgException();

	// just remove a range
	INT_PTR nMoveCount = m_nSize - (nUpperBound);

	if (nMoveCount)
	{
		memmove(&m_pData[nIndex], &m_pData[nUpperBound], nMoveCount * sizeof(void*));
	}

	m_nSize -= nCount;
}

void CCAD_PtrArray::InsertAt(INT_PTR nStartIndex, CCAD_PtrArray* pNewArray)
{
	ASSERT_VALID(this);
	ASSERT(pNewArray != NULL);
	ASSERT_KINDOF(CCAD_PtrArray, pNewArray);
	ASSERT_VALID(pNewArray);
	ASSERT(nStartIndex >= 0);

	if(pNewArray == NULL || nStartIndex < 0)
		AfxThrowInvalidArgException();

	if (pNewArray->GetSize() > 0)
	{
		InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
		for (INT_PTR i = 0; i < pNewArray->GetSize(); i++)
			SetAt(nStartIndex + i, pNewArray->GetAt(i));
	}
}

INT_PTR CCAD_PtrArray::GetSize() const
{ return m_nSize; }
INT_PTR CCAD_PtrArray::GetCount() const
{ return m_nSize; }
BOOL CCAD_PtrArray::IsEmpty() const
{ return m_nSize == 0; }
INT_PTR CCAD_PtrArray::GetUpperBound() const
{ return m_nSize-1; }
void CCAD_PtrArray::RemoveAll()
{ SetSize(0); }
void* CCAD_PtrArray::GetAt(INT_PTR nIndex) const
{ ASSERT(nIndex >= 0 && nIndex < m_nSize);
if( nIndex < 0 || nIndex >= m_nSize )
AfxThrowInvalidArgException();
return m_pData[nIndex]; }
void CCAD_PtrArray::SetAt(INT_PTR nIndex, void* newElement)
{ ASSERT(nIndex >= 0 && nIndex < m_nSize);
if( nIndex < 0 || nIndex >= m_nSize )
AfxThrowInvalidArgException();
m_pData[nIndex] = newElement; }

void*& CCAD_PtrArray::ElementAt(INT_PTR nIndex)
{ ASSERT(nIndex >= 0 && nIndex < m_nSize);
if( nIndex < 0 || nIndex >= m_nSize )
AfxThrowInvalidArgException();
return m_pData[nIndex]; }
const void** CCAD_PtrArray::GetData() const
{ return (const void**)m_pData; }
void** CCAD_PtrArray::GetData()
{ return (void**)m_pData; }
INT_PTR CCAD_PtrArray::Add(void* newElement)
{ INT_PTR nIndex = m_nSize;
SetAtGrow(nIndex, newElement);
return nIndex; }

void* CCAD_PtrArray::operator[](INT_PTR nIndex) const
{ return GetAt(nIndex); }
void*& CCAD_PtrArray::operator[](INT_PTR nIndex)
{ return ElementAt(nIndex); }


