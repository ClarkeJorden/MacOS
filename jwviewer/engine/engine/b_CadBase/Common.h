
#ifndef	_COMMON_H
#define	_COMMON_H

#include "Define_Type.h"


#define     _CM_MAX_STR_        1024

#define EMPTY_FLOAT   -1000000.0f
#define EMPTY_INT     0
#ifndef _WIN32
LPVOID 	HeapAlloc ( HANDLE hHeap, DWORD dwFlags, DWORD dwBytes );
BOOL 	HeapFree ( HANDLE hHeap, DWORD dwFlags, LPVOID lpMem );
LPVOID 	HeapCreate ( HANDLE hHeap, DWORD dwFlags, size_t dwBytes );
BOOL 	HeapDestroy ( HANDLE hHeap );
#endif

        
#endif	// _COMMON_H
