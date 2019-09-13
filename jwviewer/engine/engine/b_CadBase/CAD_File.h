#ifndef _CAD_FILE_H_
#define _CAD_FILE_H_

#include <string.h>
#include "CAD_Constance.h"
#include "Define_Type.h"

class ICAD_File;

enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };

#define _MEM_FILE_

#ifdef _MEM_FILE_
class CCAD_MemoryFile {

protected:
	BYTE*			m_pBufHead;
	BYTE*			m_pBufEnd;
	BYTE*			m_pCurrent;

public:
	CCAD_MemoryFile(BYTE* buffer, UINT32 length); //[2016.10] Arch51
	virtual ~CCAD_MemoryFile();

public:
	inline UINT32	Read(void* lpBuf, UINT32 nCount);
	inline UINT32	GetPosition();
	inline UINT32	GetLength();
	inline UINT32	Seek(INT32 lOffset, SeekPosition inPositionMode);
	inline BOOL		IsEOF();
};
#endif

class CCAD_File
{
private:
#ifdef _MEM_FILE_
	CCAD_MemoryFile* m_pFile;
#else	
	ICAD_File*		m_pFile;
#endif	
public:
	enum OpenFlags {
		modeRead =         (int) 0x00001,
		modeWrite =        (int) 0x00002,
		modeReadWrite =    (int) 0x00004,
		shareCompat =      (int) 0x00000,
		shareExclusive =   (int) 0x00010, 
		shareDenyWrite =   (int) 0x00020,
		shareDenyRead =    (int) 0x00030,
		shareDenyNone =    (int) 0x00040,
		modeNoInherit =    (int) 0x00080,
		modeCreate =       (int) 0x01000,
		modeNoTruncate =   (int) 0x02000,
		typeText =         (int) 0x04000, // typeText and typeBinary are
		typeBinary =       (int) 0x08000, // used in derived classes only
		osNoBuffer =       (int) 0x10000,
		osWriteThrough =   (int) 0x20000,
		osRandomAccess =   (int) 0x40000,
		osSequentialScan = (int) 0x80000,
	};


public:	
//	CCAD_File(){}
#ifdef _MEM_FILE_
	CCAD_File(CCAD_MemoryFile*);
#else
	CCAD_File(ICAD_File*);
#endif	
	virtual ~CCAD_File();
	
	BOOL m_blMacVersion;
	BYTE m_bVersion;
	DWORD	m_nJWVersion;	// jw_cad version
	BOOL	m_blLoaded;
	BOOL	m_blFileDelete;

	/**
	For Convert
	*/
	static	UINT m_uiReadCodePage;
	static	UINT m_uiWriteCodePage;


public:
	void Init();
// 	virtual ULONGLONG	GetLength();
// 	virtual void		SetLength(DWORD dwNewLen);

	BOOL	IsMustSwap();

	virtual DWORD		GetPosition();
	// }-------------------------------------------------------------------

	// @name Logic Functions
	// {-------------------------------------------------------------------
	// }-------------------------------------------------------------------

	// @name Operations
	// {-------------------------------------------------------------------		
//	virtual BOOL		Open(char* pFileName, UINT nOpenFlags);
	virtual void		Close();

	virtual DWORD		Seek(DWORD dwPos, SeekPosition nFrom);

	void	ReadByte(BYTE * n);
	void	ReadShort(short * n);
	void	ReadUShort(unsigned short * n);
	void	ReadWord(WORD * n);
	void	ReadDouble(double * d);
	void	ReadUInt64(uint64 * ui);
	void	ReadFloat(float * f);
	void	ReadCAD_Float(CAD_FLOAT * f);
	void	ReadLong(int * l);
	void	ReadLONG(LONG * l);
	void	ReadInt(int * l);
	void	ReadUInt(unsigned int * l);
	void	ReadDWord(DWORD * dw);
	void	ReadColorRef(COLORREF * l);
	int		ReadString(LPSTR str, short n);
	void	ReadRGBColor( RGBColor * clr );

	void ReadBlock(void * p, unsigned int size)const;
	void WriteBlock(void * p, unsigned int size)const;

	void	SwapBool(BOOL* bl)const;
	void	ReadBool(BOOL *l);

	virtual UINT	Read(void* lpBuf, UINT nCount = 0)const;
	virtual void	Write(const void* lpBuf, UINT nCount = 0)const;

	void	Read(BYTE & n) const;
	void	Write(const BYTE & n) const;
	void	Read(short & n) const;
	void	Write(const short & n) const;
	void	Read(unsigned short & n)const;
	void	Write(const unsigned short & n)const;
	void	Read(double & d)const;
	void	Write(const double & d)const;
	void	Read(uint64 & ui)const;
	void	Write(const uint64 & ui)const;
	void	Read(float & f)const;
	void	Write(const float & f)const;
	void    ReadCadFloat(CAD_FLOAT & f) const
	{
		Read(f);
	}
	void WriteCadFloat(const CAD_FLOAT & f) const
	{
		Write(f);
	}
	void	Read(int & l)const;
	void	Write(const int & l)const;
	void	Read(bool & l)const;
	void	Write(const bool & l)const;
	void	Read(unsigned int & l)const;
	void	Write(const unsigned int & l)const;
	// }-------------------------------------------------------------------
};


#endif // _CAD_FILE_H_

