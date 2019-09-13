
#include "stdafx.h"
#include "CAD_File.h"
#include "CAD_Angle.h"
#include "CAD_Global.h"
#include "ICAD_File.h"

UINT CCAD_File::m_uiReadCodePage = CP_ACP;
UINT CCAD_File::m_uiWriteCodePage = CP_ACP;

#define CREATE_OBJECT(class_name) return new class_name;

#ifdef _MEM_FILE_
CCAD_MemoryFile::CCAD_MemoryFile(BYTE* buffer, UINT32 length)
{
	ASSERT(buffer);
	ASSERT(length);

	m_pBufHead = buffer;
	m_pBufEnd = m_pBufHead + length;
	m_pCurrent = m_pBufHead;
}

CCAD_MemoryFile::~CCAD_MemoryFile()
{
	if (m_pBufHead)
		delete[] m_pBufHead;
}


UINT32 CCAD_MemoryFile::Seek(INT32 lOffset, SeekPosition inPositionMode/* = dxFilePos_Current */)
{

	switch(inPositionMode)
	{
	case begin:
		m_pCurrent = m_pBufHead + lOffset;
		break;
	case end:
		m_pCurrent = m_pBufEnd - lOffset;
		break;
	case current:
		m_pCurrent += lOffset;
		break;
	}	


	return GetPosition();
}

inline UINT32 CCAD_MemoryFile::GetPosition()
{
	return (UINT32)(m_pCurrent - m_pBufHead);
}

inline UINT32 CCAD_MemoryFile::GetLength()
{
	return (UINT32)(m_pBufEnd - m_pBufHead);
}

inline UINT32 CCAD_MemoryFile::Read(void* lpBuf, UINT32 nCount)
{
	{
		if (m_pCurrent < m_pBufHead || m_pCurrent+nCount > m_pBufEnd)
		{
			return 0;
		}

		memcpy(lpBuf, m_pCurrent, nCount);
		m_pCurrent += nCount;
	}

	return nCount;
}
#endif

#ifdef _MEM_FILE_
CCAD_File::CCAD_File(CCAD_MemoryFile* pFile)
#else
CCAD_File::CCAD_File(ICAD_File* pFile)
#endif
{
	m_pFile = pFile;

	Init();
}

CCAD_File::~CCAD_File()
{
	if ( m_pFile )
	{
		delete m_pFile;
		m_pFile = NULL;
	}
}

void CCAD_File::Init()
{
	// FileConvert
	m_uiReadCodePage = CP_ACP;
	m_uiWriteCodePage = CP_ACP;
}

//-------------------------------------------
void CCAD_File::ReadByte(BYTE * n)
{	
	Read(n, sizeof(BYTE));
}

//-------------------------------------------
void CCAD_File::ReadShort(short * n)
{
	Read(n, sizeof(short));
}

//-------------------------------------------
void  CCAD_File::ReadUShort(unsigned short * n)
{
	Read(n, sizeof(unsigned short));
}

//-------------------------------------------
void CCAD_File::ReadWord(WORD * n)
{
	Read(n, sizeof(WORD));
}

void CCAD_File::ReadDouble(double * d)
{
		Read(d, sizeof(double));
}

void CCAD_File::ReadUInt64(uint64 * ui)
{
		Read(ui, sizeof(uint64));
}

void CCAD_File::ReadFloat(float * f)
{
		Read(f, sizeof(float));
}

void CCAD_File::ReadCAD_Float(CAD_FLOAT * f)
{
		Read(f, sizeof(CAD_FLOAT));
}

void CCAD_File::ReadLong(int * l)
{
		Read(l, sizeof(int) );
}

void CCAD_File::ReadLONG(LONG * l)
{
		Read(l, sizeof(LONG) );
}

void CCAD_File::ReadInt(int * l)
{
		Read(l, sizeof(int));
}

void CCAD_File::ReadUInt(unsigned int * l)
{
		Read(l, sizeof(unsigned int));
}

void CCAD_File::ReadDWord(DWORD * dw)
{
		Read(dw, sizeof(DWORD));
}

void CCAD_File::ReadColorRef(COLORREF * l)
{
	if (m_pFile != NULL)
	{
		ReadDWord((DWORD*)l);
		*l |= 0xFF000000;
	}
}
void CCAD_File::ReadRGBColor( RGBColor * clr )
{
 	// old mac version
		ReadUShort( &clr->red );
		ReadUShort( &clr->green );
		ReadUShort( &clr->blue );
}

int CCAD_File::ReadString(LPSTR str, short n)
{
	Read(str, n);

    return 1;
}

void CCAD_File::ReadBlock(void * p, unsigned int size)const
{
	Read(p, size);
}

void CCAD_File::WriteBlock(void * p, unsigned int size)const
{
	Write(p, size);
}

//-------------------------------------------
void CCAD_File::Read(BYTE & n)const
{
	Read(&n, sizeof(BYTE));
}

void CCAD_File::Write(const BYTE & n)const
{
	Write(&n, sizeof(BYTE));
}

//-------------------------------------------
void CCAD_File::Read(short & n)const
{
	Read(&n, sizeof(short));
}

void CCAD_File::Write(const short & n)const
{
}
void  CCAD_File::Read(unsigned short & n)const
{
	Read(&n, sizeof(unsigned short));
}

void  CCAD_File::Write(const unsigned short & n)const
{
}
void CCAD_File::Read(double & d)const
{
	Read(&d, sizeof(double));
}

void CCAD_File::Write(const double & d)const
{
}
void CCAD_File::Read(uint64 & ui)const
{
	Read(&ui, sizeof(uint64));
}

void CCAD_File::Write(const uint64 & ui)const
{
}
void CCAD_File::Read(float & f)const
{
	Read(&f, sizeof(float));
}

void CCAD_File::Write(const float & f)const
{
}
void CCAD_File::Read(int & l)const
{
	Read(&l, 4);
}

void CCAD_File::Read(unsigned int & l)const
{
	Read(&l, sizeof(unsigned int));
}
void CCAD_File::Write(const unsigned int & l)const
{

}

void CCAD_File::Read(bool & l)const
{
	Read(&l, sizeof(bool) );
}

void CCAD_File::Write(const bool & l)const
{
	Write(&l, sizeof(bool) );
}

// ULONGLONG CCAD_File::GetLength()
// {
// 	return m_pFile->GetLength();
// }
// 
// void CCAD_File::SetLength(DWORD dwNewLen)
// {
// 	m_pFile->SetLength(dwNewLen);
// }

DWORD CCAD_File::GetPosition()
{
	return m_pFile->GetPosition();
}

// BOOL CCAD_File::Open(char* pFileName, UINT nOpenFlags)
// {
// 	return m_pFile->Open(pFileName, nOpenFlags);
// }

void CCAD_File::Close()
{
#ifndef _MEM_FILE_
	m_pFile->Close();
#endif
}

DWORD CCAD_File::Seek(DWORD dwPos, SeekPosition nFrom)
{
	return m_pFile->Seek(dwPos, nFrom);
}


UINT CCAD_File::Read(void* lpBuf, UINT nCount)const
{
	return m_pFile->Read(lpBuf, nCount);
}

void CCAD_File::Write(const void* lpBuf, UINT nCount)const
{
}

//-------------------------------------------
void CCAD_File::ReadBool(BOOL * l)
{
	if (m_pFile != NULL){
		m_pFile->Read( l, (UINT)sizeof(int) );
	}
}
