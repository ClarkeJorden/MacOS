#ifndef _KON_ARRAY_
#define _KON_ARRAY_

#include "Define_Type.h"
#include "string.h"
#include <new>

template<class TYPE>
inline void _CopyElements(TYPE* pDest, const TYPE* pSrc, int nCount)
{
	while (nCount--)
		*pDest++ = *pSrc++;
}

template<class TYPE, class ARG_TYPE = const TYPE&>
class  CKon_Array// : public CCAD_Object
{
public:
	// Construction
		
	int GetSize() const
	{ return m_nSize; }
	
	int GetCount() const
	{ return m_nSize; }
	
	BOOL IsEmpty() const
	{ return m_nSize == 0; }
	
	int GetUpperBound() const
	{ return m_nSize-1; }
	
	void RemoveAll()
	{ SetSize(0, -1); }
	
	TYPE& GetAt(int nIndex)
	{ 
		ASSERT(nIndex >= 0);
                ASSERT(nIndex <= m_nSize);
		if(nIndex >= 0 && nIndex < m_nSize)
			return m_pData[nIndex]; 
		AfxThrowInvalidArgException();		
	}
	
	TYPE& GetAt(int nIndex) const
	{
		ASSERT(nIndex >= 0 && nIndex < m_nSize);
		if(nIndex >= 0 && nIndex < m_nSize)
			return m_pData[nIndex]; 
		AfxThrowInvalidArgException();		
	}
	
	void SetAt(int nIndex, ARG_TYPE newElement)
	{ 
		ASSERT(nIndex >= 0 && nIndex < m_nSize);
		if(nIndex >= 0 && nIndex < m_nSize)
			m_pData[nIndex] = newElement; 
		else
			AfxThrowInvalidArgException();		
	}
	
	const TYPE& ElementAt(int nIndex) const
	{ 
		ASSERT(nIndex >= 0 && nIndex < m_nSize);
		if(nIndex >= 0 && nIndex < m_nSize)
			return m_pData[nIndex];
		AfxThrowInvalidArgException();		
	}
	
	TYPE& ElementAt(int nIndex)
	{ 
		ASSERT(nIndex >= 0 && nIndex < m_nSize);
		if(nIndex >= 0 && nIndex < m_nSize)
			return m_pData[nIndex]; 
		AfxThrowInvalidArgException();		
	}
	
	const TYPE* GetData() const
	{ return (const TYPE*)m_pData; }
	
	TYPE* GetData()
	{ return (TYPE*)m_pData; }
	
	int Add(ARG_TYPE newElement)
	{ int nIndex = m_nSize;
	SetAtGrow(nIndex, newElement);
	return nIndex; }
	
	const TYPE& operator[](int nIndex) const
	{ return GetAt(nIndex); }
	
	TYPE& operator[](int nIndex)
	{ return ElementAt(nIndex); }

	/////////////////////////////////////////////////////////////////////////////
	// CKon_Array<TYPE, ARG_TYPE> out-of-line functions

	
	CKon_Array()
	{
		m_pData = NULL;
		m_nSize = m_nMaxSize = m_nGrowBy = 0;
	}

	
	~CKon_Array()
	{
		ASSERT_VALID(this);

		if (m_pData != NULL)
		{
			for( int i = 0; i < m_nSize; i++ )
				(m_pData + i)->~TYPE();
			delete[] (BYTE*)m_pData;
		}
	}

	void SetSize(int nNewSize, int nGrowBy = -1)
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
			if (m_pData != NULL)
			{
				for( int i = 0; i < m_nSize; i++ )
					(m_pData + i)->~TYPE();
				delete[] (BYTE*)m_pData;
				m_pData = NULL;
			}
			m_nSize = m_nMaxSize = 0;
		}
		else if (m_pData == NULL)
		{
			// create buffer big enough to hold number of requested elements or
			// m_nGrowBy elements, whichever is larger.
#ifdef SIZE_T_MAX
			ASSERT(nNewSize <= SIZE_T_MAX/sizeof(TYPE));    // no overflow
#endif
			size_t nAllocSize = nNewSize > m_nGrowBy?nNewSize:m_nGrowBy;
			m_pData = (TYPE*) new BYTE[(size_t)nAllocSize * sizeof(TYPE)];
			memset((void*)m_pData, 0, (size_t)nAllocSize * sizeof(TYPE));
			for( int i = 0; i < nNewSize; i++ )
				::new( (void*)( m_pData + i ) ) TYPE;
			m_nSize = nNewSize;
			m_nMaxSize = nAllocSize;
		}
		else if (nNewSize <= m_nMaxSize)
		{
			// it fits
			if (nNewSize > m_nSize)
			{
				// initialize the new elements
				memset((void*)(m_pData + m_nSize), 0, (size_t)(nNewSize-m_nSize) * sizeof(TYPE));
				for( int i = 0; i < nNewSize-m_nSize; i++ )
					::new( (void*)( m_pData + m_nSize + i ) ) TYPE;
			}
			else if (m_nSize > nNewSize)
			{
				// destroy the old elements
				for( int i = 0; i < m_nSize-nNewSize; i++ )
					(m_pData + nNewSize + i)->~TYPE();
			}
			m_nSize = nNewSize;
		}
		else
		{
			// otherwise, grow array
			nGrowBy = m_nGrowBy;
			if (nGrowBy == 0)
			{
				// heuristically determine growth when nGrowBy == 0
				//  (this avoids heap fragmentation in many situations)
				nGrowBy = m_nSize / 8;
				nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);
			}
			int nNewMax;
			if (nNewSize < m_nMaxSize + nGrowBy)
				nNewMax = m_nMaxSize + nGrowBy;  // granularity
			else
				nNewMax = nNewSize;  // no slush

			ASSERT(nNewMax >= m_nMaxSize);  // no wrap around

			if(nNewMax  < m_nMaxSize)
				AfxThrowInvalidArgException();

#ifdef SIZE_T_MAX
			ASSERT(nNewMax <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
			TYPE* pNewData = (TYPE*) new BYTE[(size_t)nNewMax * sizeof(TYPE)];

			// copy new data from old
			/*::ATL::Checked::*/memcpy(pNewData,
				m_pData, (size_t)m_nSize * sizeof(TYPE));

			// construct remaining elements
			ASSERT(nNewSize > m_nSize);
			memset((void*)(pNewData + m_nSize), 0, (size_t)(nNewSize-m_nSize) * sizeof(TYPE));
			for( int i = 0; i < nNewSize-m_nSize; i++ )
				::new( (void*)( pNewData + m_nSize + i ) ) TYPE;

			// get rid of old stuff (note: no destructors called)
			delete[] (BYTE*)m_pData;
			m_pData = pNewData;
			m_nSize = nNewSize;
			m_nMaxSize = nNewMax;
		}
	}

	
	int Append(const CKon_Array& src)
	{
		ASSERT_VALID(this);
		ASSERT(this != &src);   // cannot append to itself

		if(this == &src)
			AfxThrowInvalidArgException();

		int nOldSize = m_nSize;
		SetSize(m_nSize + src.m_nSize);
		_CopyElements<TYPE>(m_pData + nOldSize, src.m_pData, src.m_nSize);
		return nOldSize;
	}

	
	void Copy(const CKon_Array& src)
	{
		ASSERT_VALID(this);
		ASSERT(this != &src);   // cannot append to itself

		if(this != &src)
		{
			SetSize(src.m_nSize);
			_CopyElements<TYPE>(m_pData, src.m_pData, src.m_nSize);
		}
	}

	
	void FreeExtra()
	{
		ASSERT_VALID(this);

		if (m_nSize != m_nMaxSize)
		{
			// shrink to desired size
#ifdef SIZE_T_MAX
			ASSERT(m_nSize <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
			TYPE* pNewData = NULL;
			if (m_nSize != 0)
			{
				pNewData = (TYPE*) new BYTE[m_nSize * sizeof(TYPE)];
				// copy new data from old
				/*::ATL::Checked::*/memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));
			}

			// get rid of old stuff (note: no destructors called)
			delete[] (BYTE*)m_pData;
			m_pData = pNewData;
			m_nMaxSize = m_nSize;
		}
	}

	
	void SetAtGrow(int nIndex, const ARG_TYPE newElement)
	{
		ASSERT_VALID(this);
		ASSERT(nIndex >= 0);

		if(nIndex < 0)
			AfxThrowInvalidArgException();

		if (nIndex >= m_nSize)
			SetSize(nIndex+1, -1);
		m_pData[nIndex] = newElement;
	}

	
	void InsertAt(int nIndex, ARG_TYPE newElement, int nCount =1)
	{
		ASSERT_VALID(this);
		ASSERT(nIndex >= 0);    // will expand to meet need
		ASSERT(nCount > 0);     // zero or negative size not allowed

		if(nIndex < 0 || nCount <= 0)
			AfxThrowInvalidArgException();

		if (nIndex >= m_nSize)
		{
			// adding after the end of the array
			SetSize(nIndex + nCount, -1);   // grow so nIndex is valid
		}
		else
		{
			// inserting in the middle of the array
			int nOldSize = m_nSize;
			SetSize(m_nSize + nCount, -1);  // grow it to new size
			// destroy intial data before copying over it
			for( int i = 0; i < nCount; i++ )
				(m_pData + nOldSize + i)->~TYPE();
			// shift old data up to fill gap
			/*::ATL::Checked::*/memmove(m_pData + nIndex + nCount, m_pData + nIndex, (nOldSize-nIndex) * sizeof(TYPE));

			// re-init slots we copied from
			memset((void*)(m_pData + nIndex), 0, (size_t)nCount * sizeof(TYPE));
			for( int i = 0; i < nCount; i++ )
				::new( (void*)( m_pData + nIndex + i ) ) TYPE;
		}

		// insert new value in the gap
		ASSERT(nIndex + nCount <= m_nSize);
		while (nCount--)
			m_pData[nIndex++] = newElement;
	}
	
	void RemoveAt(int nIndex, int nCount = 1)
	{
		ASSERT_VALID(this);
		ASSERT(nIndex >= 0);
		ASSERT(nCount >= 0);
		int nUpperBound = nIndex + nCount;
		ASSERT(nUpperBound <= m_nSize && nUpperBound >= nIndex && nUpperBound >= nCount);

		if(nIndex < 0 || nCount < 0 || (nUpperBound > m_nSize) || (nUpperBound < nIndex) || (nUpperBound < nCount))
			AfxThrowInvalidArgException();

		// just remove a range
		int nMoveCount = m_nSize - (nUpperBound);
		for( int i = 0; i < nCount; i++ )
			(m_pData + nIndex + i)->~TYPE();
		if (nMoveCount)
		{
			/*::ATL::Checked::*/memmove(m_pData + nIndex, m_pData + nUpperBound, (size_t)nMoveCount * sizeof(TYPE));
		}
		m_nSize -= nCount;
	}

	
	void InsertAt(int nStartIndex, CKon_Array* pNewArray)
	{
		ASSERT_VALID(this);
		ASSERT(pNewArray != NULL);
		ASSERT_VALID(pNewArray);
		ASSERT(nStartIndex >= 0);

		if(pNewArray == NULL || nStartIndex < 0)
			AfxThrowInvalidArgException();

		if (pNewArray->GetSize() > 0)
		{
			InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
			for (int i = 0; i < pNewArray->GetSize(); i++)
				SetAt(nStartIndex + i, pNewArray->GetAt(i));
		}
	}

	// Implementation
protected:
	TYPE* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount
};

typedef	CKon_Array <short, short>	CChildArray;
typedef	CKon_Array <DWORD, DWORD>	CDWORDArray;
typedef	CKon_Array<long, long> 	CKon_GemRenderInfor_Array;
typedef CKon_Array<long, long> 	CCAD_LongArray;
typedef CKon_Array<int, int>    CIntArray;
typedef CKon_Array<BYTE, BYTE>  CCAD_ByteArray;

#endif

