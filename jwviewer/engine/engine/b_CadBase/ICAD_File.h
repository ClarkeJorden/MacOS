#pragma once

class ICAD_File
{
public:
	// @name Operations
	// {-------------------------------------------------------------------
	virtual UINT			Read(void* lpBuf, UINT nCount = 0)const = 0;
	virtual void			Write(const void* lpBuf, UINT nCount = 0)const = 0;

	virtual UINT			Read(void* lpBuf, int nCount = 0)const = 0;
	virtual void			Write(const void* lpBuf, int nCount = 0)const = 0;

	virtual DWORD			Seek(DWORD dwPos, UINT nFrom) = 0;

	virtual BOOL			Open(char* pFileName, UINT nOpenFlags) = 0;
	virtual void			Close() = 0;
	// }-------------------------------------------------------------------

	virtual DWORD			GetPosition() = 0;
	// }-------------------------------------------------------------------
};

 
