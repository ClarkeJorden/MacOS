//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_IMAGE_H)
#define AFX_CAD_IMAGE_H


#include	"CAD_Style.h"

 

#define	NO_TYPE		0xff
#define	BMP_TYPE	0x00
#define TIF_TYPE	0x01
#define JPG_TYPE	0x02
#define PCX_TYPE	0x03
								//--------- pixel color data
#pragma pack(push, 1)
typedef struct  tagPIXELDATA {
    BYTE		bRed;
    BYTE		bGreen;
    BYTE		bBlue;
    BYTE		bDummy;
} PIXELDATA;
typedef PIXELDATA *LPPIXELDATA;
								//--------- image data
typedef struct tagBMPIMAGE {
	int       	iWidth;
	int			iHeight;
	LPPIXELDATA	pData;
	tagBMPIMAGE()
	{
		iWidth = iHeight = 0;
		pData = NULL;
	}
} IMAGE;
typedef IMAGE *LPIMAGE;

typedef struct tagJPGIMAGE {
	int        iCompress;
	int		iLength;
	LPPIXELDATA	pData;
	tagJPGIMAGE()
	{
		iCompress = iLength = 0;
		pData = NULL;
	}
} JPGIMAGE;

struct BMPFILEHEADER {
    WORD    bfType;
    DWORD   bfSize;
    WORD    bfReserved1;
    WORD    bfReserved2;
    DWORD   bfOffBits;
};

struct BMPINFOHEADER{
    DWORD      biSize;
    int       biWidth;
    int       biHeight;
    WORD       biPlanes;
    WORD       biBitCount;
    DWORD      biCompression;
    DWORD      biSizeImage;
    int       biXPelsPerMeter;
    int       biYPelsPerMeter;
    DWORD      biClrUsed;
    DWORD      biClrImportant;
};

struct RGB_QUAD {
    BYTE    rgbBlue;
    BYTE    rgbGreen;
    BYTE    rgbRed;
    BYTE    rgbReserved;
};
#pragma pack(pop)

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L

class   CCAD_Image : public CCAD_Style {
public:
	HANDLE		m_hHeap;
	IMAGE		m_sImage;

	JPGIMAGE	m_sJpgImage;
	BYTE		m_bImageMgr;

public:
	CCAD_Image(HANDLE);
	virtual ~CCAD_Image();

public:
	virtual void	Init();
	virtual void	CopyFrom(const CCAD_Style *pStyle);
	virtual BOOL	Read(CCAD_File &);

	void	SetSpliteTrans();
	void	SetDefaultTrans();

	BOOL	SetSize(long w, long h);
    BOOL	SetBmpData(BYTE * pData);
    BOOL	SetBmpData1(BYTE * pData);
    BOOL	SetBmpData2(BYTE * pData);
    BOOL	SetBmpData4(BYTE * pData);
    BOOL	SetBmpData8(BYTE * pData);
    BOOL	SetBmpData16(BYTE * pData);
    BOOL	SetBmpData24(BYTE * pData);
    BOOL	SetBmpData32(BYTE * pData);
};

 

#endif
