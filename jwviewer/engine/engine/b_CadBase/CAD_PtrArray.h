#ifndef _CAD_PTRARRAY_H_
#define _CAD_PTRARRAY_H_

#include "Define_Type.h"

class CCAD_PtrArray
{

//	DECLARE_DYNCREATE_CAD(CCAD_PtrArray)
public:

	// Construction
	CCAD_PtrArray();
	~CCAD_PtrArray();

	// Attributes
	int GetSize() const;
	int GetCount() const;
	BOOL IsEmpty() const;
	int GetUpperBound() const;
	void SetSize(int nNewSize, int nGrowBy = -1);

	// Operations
	// Clean up
	void FreeExtra();
	void RemoveAll();

	// Accessing elements
	void* GetAt(int nIndex) const;
	void SetAt(int nIndex, void* newElement);

	void*& ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const void** GetData() const;
	void** GetData();

	// Potentially growing the array
	void SetAtGrow(int nIndex, void* newElement);

	int Add(void* newElement);

	int Append(const CCAD_PtrArray& src);
	void Copy(const CCAD_PtrArray& src);

	// overloaded operator helpers
	void* operator[](int nIndex) const;
	void*& operator[](int nIndex);

	// Operations that move elements around
	void InsertAt(int nIndex, void* newElement, int nCount = 1);

	void RemoveAt(int nIndex, int nCount = 1);
	void InsertAt(int nStartIndex, CCAD_PtrArray* pNewArray);

	// Implementation
protected:
	void** m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

	// local typedefs for class templates
	typedef void* BASE_TYPE;
	typedef void* BASE_ARG_TYPE;
};

#endif // _CAD_PTRARRAY_H_

