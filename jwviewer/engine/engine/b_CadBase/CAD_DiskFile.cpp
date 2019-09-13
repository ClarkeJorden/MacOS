#include "stdafx.h"
#include "CAD_DiskFile.h"
#include "CAD_File.h"

 

CCAD_DiskFile::CCAD_DiskFile()
{
	m_pFile = NULL;
}

CCAD_DiskFile::~CCAD_DiskFile()
{
	if(m_pFile)
	{
		fclose(m_pFile);
	}
}

UINT CCAD_DiskFile::Read(void* lpBuf, UINT nCount/* = 0*/)const
{
	ASSERT(lpBuf);
	ASSERT(m_pFile);
	UINT nCurPos = ftell(m_pFile);
	if (nCurPos < 0 || nCurPos+nCount > m_nFileSize)
		return 0;

	return fread((char*)lpBuf, sizeof(char), nCount, m_pFile);
}

void CCAD_DiskFile::Write(const void* lpBuf, UINT nCount/* = 0*/)const
{
	ASSERT(lpBuf);
    ASSERT(m_pFile);
	fwrite((char*)lpBuf, sizeof(char), nCount, m_pFile);
}

UINT CCAD_DiskFile::Read(void* lpBuf, int nCount/* = 0*/)const
{
	ASSERT(lpBuf);
	ASSERT(m_pFile);

	UINT nCurPos = ftell(m_pFile);
	if (nCurPos < 0 || nCurPos+nCount > m_nFileSize)
		return 0;
	return fread((char*)lpBuf, sizeof(char), nCount, m_pFile);
}

void CCAD_DiskFile::Write(const void* lpBuf, int nCount/* = 0*/)const
{
	ASSERT(lpBuf);
	ASSERT(m_pFile);
	fwrite((char*)lpBuf, sizeof(char), nCount, m_pFile);
}

void CCAD_DiskFile::Close()
{
	if(m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

DWORD CCAD_DiskFile::Seek(DWORD dwPos, UINT nFrom)
{
	ASSERT(m_pFile);
	fseek(m_pFile, dwPos, nFrom);
    return GetPosition();
}

DWORD CCAD_DiskFile::GetPosition()
{
	ASSERT(m_pFile);
	return ftell(m_pFile);
}

BOOL CCAD_DiskFile::Open(char* pFileName, UINT nOpenFlags)
{
	if(m_pFile)
		ASSERT(false);

	char pOpenFlag[255];
	memset(pOpenFlag, 0, 255);

	if ( (nOpenFlags & CCAD_File::modeRead) == CCAD_File::modeRead )
		strcat(pOpenFlag, "r");
	if ( (nOpenFlags & CCAD_File::modeWrite) == CCAD_File::modeWrite )
		strcat(pOpenFlag, "w");
	if ( (nOpenFlags & CCAD_File::modeReadWrite) == CCAD_File::modeReadWrite )
		strcat(pOpenFlag, "r+");
	if ( (nOpenFlags & CCAD_File::typeText ) == CCAD_File::typeText )
		strcat(pOpenFlag, "t");
	if ( (nOpenFlags & CCAD_File::typeBinary ) == CCAD_File::typeBinary )
		strcat(pOpenFlag, "b");

	if( (m_pFile = fopen( pFileName, pOpenFlag)) == NULL )
		return FALSE;
	fseek(m_pFile,0L, SEEK_END);
	m_nFileSize = ftell(m_pFile);
	fseek(m_pFile,0L, SEEK_SET);
	return TRUE;
}

 

