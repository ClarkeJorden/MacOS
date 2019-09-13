
#include "Common.h"
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
// Heap functions
LPVOID HeapAlloc ( HANDLE hHeap, DWORD dwFlags, DWORD dwBytes )
{
	hHeap = 0;

	LPVOID lpRet = malloc( dwBytes );
	if (lpRet == NULL) return NULL;

	if ( dwFlags == HEAP_ZERO_MEMORY )
		memset( lpRet, 0x00, dwBytes );

	return lpRet;
}

BOOL HeapFree ( HANDLE hHeap, DWORD dwFlags, LPVOID lpMem )
{
	hHeap = 0;
	dwFlags = 0;

	free( lpMem );
	return TRUE;
}

LPVOID HeapCreate ( HANDLE hHeap, DWORD dwFlags, size_t dwBytes )
{
	hHeap = 0;
	dwFlags = 0;
	dwBytes = 0;

	return (LPVOID)1;//NULL
}

BOOL HeapDestroy ( HANDLE hHeap )
{
	hHeap = 0;

	return TRUE;
}

#endif