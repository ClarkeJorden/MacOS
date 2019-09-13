// SanakJpeg.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "b_CadJpg.h"
#include "jpeglib.h"
#include <setjmp.h>


 


struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

void    put_scanline_someplace(char *destbuffer, char *srcbuffer, int bytesperline, int y)
{
	memcpy(&destbuffer[bytesperline*(y-1)], srcbuffer, bytesperline);
};

GLOBAL(int)
read_JPEG_file (char *pData1, long lSize, LPSTR image_buffer)
{
  /// This struct contains the JPEG decompression parameters and pointers to
   // working space (which is allocated as needed by the JPEG library).
   ///
  struct jpeg_decompress_struct cinfo;
  /// We use our private extension JPEG error handler.
   // Note that this struct must live as long as the main JPEG parameter
   // struct, to avoid dangling-pointer problems.
   ///
  struct my_error_mgr jerr;
  /// More stuff ///
  JSAMPARRAY buffer;		/// Output row buffer ///
  int row_stride;		/// physical row width in output buffer ///

  /// In this example we want to open the input file before doing anything else,
   // so that the setjmp() error recovery below can assume the file is open.
   // VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   // requires it in order to read binary files.
   ///

  /// Step 1: allocate and initialize JPEG decompression object ///

  /// We set up the normal JPEG error routines, then override error_exit. ///
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  /// Establish the setjmp return context for my_error_exit to use. ///
  if (setjmp(jerr.setjmp_buffer)) {
    /// If we get here, the JPEG code has signaled an error.
     // We need to clean up the JPEG object, close the input file, and return.
     ///
    jpeg_destroy_decompress(&cinfo);
    return 0;
  }
  /// Now we can initialize the JPEG decompression object. ///
  jpeg_create_decompress(&cinfo);

  /// Step 2: specify data source (eg, a file) ///

  JpegMemFile inFile;
  inFile.pData = pData1;
  inFile.lSize = lSize;
  jpeg_stdio_src(&cinfo, &inFile);

  /// Step 3: read file parameters with jpeg_read_header() ///

  (void) jpeg_read_header(&cinfo, TRUE);
  /// We can ignore the return value from jpeg_read_header since
   //   (a) suspension is not possible with the stdio data source, and
   //   (b) we passed TRUE to reject a tables-only JPEG file as an error.
   // See libjpeg.doc for more info.
   ///

  /// Step 4: set parameters for decompression ///

  /// In this example, we don't need to change any of the defaults set by
   // jpeg_read_header(), so we do nothing here.
   ///

  /// Step 5: Start decompressor ///

  (void) jpeg_start_decompress(&cinfo);
  /// We can ignore the return value since suspension is not possible
   // with the stdio data source.
   ///

  /// We may need to do some setup of our own at this point before reading
   // the data.  After jpeg_start_decompress() we have the correct scaled
   // output image dimensions available, as well as the output colormap
   // if we asked for color quantization.
   // In this example, we need to make an output work buffer of the right size.
   /// 
  /// JSAMPLEs per row in output buffer ///
  row_stride = cinfo.output_width * cinfo.output_components;
  /// Make a one-row-high sample array that will go away when done with image ///
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  /// Step 6: while (scan lines remain to be read) ///
  ///           jpeg_read_scanlines(...); ///

  /// Here we use the library's state variable cinfo.output_scanline as the
   // loop counter, so that we don't have to keep track ourselves.
   ///
  while (cinfo.output_scanline < cinfo.output_height) {
    /// jpeg_read_scanlines expects an array of pointers to scanlines.
     // Here the array is only one element long, but you could ask for
     // more than one scanline at a time if that's more convenient.
     ///
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    /// Assume put_scanline_someplace wants a pointer and sample count. ///
    put_scanline_someplace((char *)image_buffer, (char *)buffer[0], row_stride, cinfo.output_scanline);
  }

  /// Step 7: Finish decompression ///

  (void) jpeg_finish_decompress(&cinfo);
  /// We can ignore the return value since suspension is not possible
   // with the stdio data source.

  /// Step 8: Release JPEG decompression object ///

  /// This is an important step since it will release a good deal of memory. ///
  jpeg_destroy_decompress(&cinfo);


  return 1;
}

//****************************************************************************

//****************************************************************************

BOOL GetBmpSize(LPSTR pData1, int lSize, int* pWidth, int* pHeight)
{
	struct my_error_mgr jerr;
	struct jpeg_decompress_struct cinfo;
	BOOL blRet = TRUE;
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		return FALSE;
	}
	jpeg_create_decompress(&cinfo);
	  JpegMemFile inFile;
	  inFile.pData = pData1;
	  inFile.lSize = lSize;
	jpeg_stdio_src(&cinfo, &inFile);
	(void) jpeg_read_header(&cinfo, TRUE);
	*pWidth = cinfo.image_width;
	*pHeight = cinfo.image_height;
	jpeg_destroy_decompress(&cinfo);

	return blRet;
}

BOOL ConvJpegToRgb(LPSTR pData1, long lSize, LPSTR pData2, long lWidth, long lHeight)
{
	long image_width = lWidth;
	long image_height = lHeight;
	LPSTR image_buffer = pData2;
	if (image_buffer == NULL)
		return FALSE;
	
	int bytesperline = image_width*3 ;
	if (read_JPEG_file(pData1, lSize, image_buffer) == 0)
		return FALSE;

	char *temp = new char[bytesperline];
	for(int y = 0; y<image_height; y++ )	//	rgb
	{
		memcpy(temp, &image_buffer[bytesperline*y], bytesperline);
		for(int x = 0; x<image_width; x++ )
		{
			image_buffer[bytesperline*y + 3*x] = temp[3*x + 2];
		//	image_buffer[bytesperline*y + 3*x+1] = temp[3*x + 1];
			image_buffer[bytesperline*y + 3*x+2] = temp[3*x];
		}
	}

	for(int i=0; i<image_height/2; i++) {	//	updown
		memcpy( (void*)temp, (void*)&pData2[bytesperline*i], bytesperline);
		memcpy(	(void*)&pData2[bytesperline*i],
				(void*)&image_buffer[bytesperline*(image_height-i-1)], 
				bytesperline );
		memcpy( (void*)&image_buffer[bytesperline*(image_height-i-1)], (void*)temp, bytesperline);
	}

	delete [] temp;
	
	return TRUE;
}

 
