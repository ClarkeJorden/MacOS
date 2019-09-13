
 

#define		COMPRESS			0x10000000		// Jpeg flag.

#define		JPEG_MAX_LENGTH		(1024*500)		// 500Kbyte



//**********************

BOOL ConvJpegToRgb(	LPSTR pData1, long lSize,
					LPSTR pData2, long lWidth, long lHeight);

BOOL GetBmpSize(LPSTR pData1, int lSize,
							  int* pWidth, int* pHeight);

//**********	ending.


 
