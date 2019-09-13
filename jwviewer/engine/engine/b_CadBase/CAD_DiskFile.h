#ifndef _CAD_DISKFILE_H_
#define _CAD_DISKFILE_H_

#include "ICAD_File.h"
#include <stdio.h>

 

class CCAD_DiskFile : public ICAD_File
{
public:

	FILE	*m_pFile;

public:
	CCAD_DiskFile();
	virtual ~CCAD_DiskFile();
	UINT			m_nFileSize;
	UINT			Read(void* lpBuf, UINT nCount = 0)const;
	void			Write(const void* lpBuf, UINT nCount = 0)const;

	UINT			Read(void* lpBuf, int nCount = 0)const;
	void			Write(const void* lpBuf, int nCount = 0)const;

	DWORD			Seek(DWORD dwPos, UINT nFrom);
	BOOL			Open(char* pFileName, UINT nOpenFlags);
	void			Close();

	virtual DWORD	GetPosition();
};

 

#endif // _CAD_DISKFILE_H_

