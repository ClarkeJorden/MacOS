/****************************************************************************/
/*                                                                          */
/*  The FreeType project -- a free and portable quality TrueType renderer.  */
/*                                                                          */
/*  Copyright 2005, 2006, 2009, 2011-2013 by                                */
/*  D. Turner, R.Wilhelm, and W. Lemberg                                    */
/*                                                                          */
/*                                                                          */
/*  ftcommon.h - common routines for the graphic FreeType demo programs.    */
/*                                                                          */
/****************************************************************************/

#ifndef FT_COMMON_H_
#define FT_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif
    
#include "ft2build.h"
#include FT_FREETYPE_H

#include FT_CACHE_H
#include FT_CACHE_MANAGER_H

#include FT_GLYPH_H
#include FT_STROKER_H
#include FT_BITMAP_H

#include <stdlib.h>
#include <stdarg.h>

extern FT_Error   error;

  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****               FREETYPE-SPECIFIC DEFINITIONS                   *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

typedef enum grPixelMode
{
    gr_pixel_mode_none = 0,
    gr_pixel_mode_mono,        /* monochrome bitmaps               */
    gr_pixel_mode_pal4,        /* 4-bit paletted - 16 colors       */
    gr_pixel_mode_pal8,        /* 8-bit paletted - 256 colors      */
    gr_pixel_mode_gray,        /* 8-bit gray levels                */
    gr_pixel_mode_rgb555,      /* 15-bits mode - 32768 colors      */
    gr_pixel_mode_rgb565,      /* 16-bits mode - 65536 colors      */
    gr_pixel_mode_rgb24,       /* 24-bits mode - 16 million colors */
    gr_pixel_mode_rgb32,       /* 32-bits mode - 16 million colors */
    gr_pixel_mode_lcd,         /* horizontal RGB-decimated         */
    gr_pixel_mode_lcdv,        /* vertical RGB-decimated           */
    gr_pixel_mode_lcd2,        /* horizontal BGR-decimated         */
    gr_pixel_mode_lcdv2,       /* vertical BGR-decimated           */
    gr_pixel_mode_bgra,        /* premultiplied BGRA colors        */
    
    gr_pixel_mode_max          /* don't remove */
    
} grPixelMode;

typedef struct grBitmap_
{
    int             rows;
    int             width;
    int             pitch;
    grPixelMode     mode;
    int             grays;
    unsigned char*  buffer;
    
} grBitmap;

#define MAX_GLYPHS 512            /* at most 512 glyphs in the string */
#define MAX_GLYPH_BYTES  150000   /* 150kB for the glyph image cache */


  typedef struct  TGlyph_
  {
    FT_UInt    glyph_index;
    FT_Glyph   image;    /* the glyph image */

    FT_Pos     delta;    /* delta caused by hinting */
    FT_Vector  vvector;  /* vert. origin => hori. origin */
    FT_Vector  vadvance; /* vertical advance */

  } TGlyph, *PGlyph;

  /* this simple record is used to model a given `installed' face */
  typedef struct  TFont_
  {
    const char*  filepathname;
    int          face_index;
    int          cmap_index;
    int          num_indices;
    void*        file_address;  /* for preloaded files */
    size_t       file_size;

  } TFont, *PFont;

  enum {
    LCD_MODE_MONO = 0,
    LCD_MODE_AA,
    LCD_MODE_LIGHT,
    LCD_MODE_LIGHT_SUBPIXEL,
    LCD_MODE_RGB,
    LCD_MODE_BGR,
    LCD_MODE_VRGB,
    LCD_MODE_VBGR,
    N_LCD_MODES
  };

  enum {
    KERNING_DEGREE_NONE = 0,
    KERNING_DEGREE_LIGHT,
    KERNING_DEGREE_MEDIUM,
    KERNING_DEGREE_TIGHT,
    N_KERNING_DEGREES
  };

  enum {
    KERNING_MODE_NONE = 0,      /* 0: no kerning;                  */
    KERNING_MODE_NORMAL,        /* 1: `kern' values                */
    KERNING_MODE_SMART,         /* 2: `kern' + side bearing errors */
    N_KERNING_MODES
  };

  typedef struct
  {
    int         kerning_mode;
    int         kerning_degree;
    FT_Fixed    center;            /* 0..1 */
    int         vertical;          /* displayed vertically? */
    FT_Matrix*  matrix;            /* string transformation */

  } FTDemo_String_Context;

  typedef struct
  {
    FT_Library      library;           /* the FreeType library          */
    FTC_Manager     cache_manager;     /* the cache manager             */
    FTC_ImageCache  image_cache;       /* the glyph image cache         */
    FTC_SBitCache   sbits_cache;       /* the glyph small bitmaps cache */
    FTC_CMapCache   cmap_cache;        /* the charmap cache             */

    PFont*          fonts;             /* installed fonts */
    int             num_fonts;
    int             max_fonts;

    int             use_sbits_cache;   /* toggle sbits cache */

    /* use FTDemo_Set_Current_XXX to set the following two fields */
    PFont           current_font;      /* selected font */
    FTC_ScalerRec   scaler;
    FT_Int32        load_flags;

    /* call FTDemo_Update_Current_Flags after setting any of the following fields */
    int             hinted;            /* is glyph hinting active?    */
    int             use_sbits;         /* do we use embedded bitmaps? */
    int             autohint;          /* force auto-hinting          */
    int             lcd_mode;          /* mono, aa, light, vrgb, ...  */
    int             preload;           /* force font file preloading  */
    int             color;             /* load color bitmaps          */

    /* don't touch the following fields! */

    /* used for string rendering */
    TGlyph          string[MAX_GLYPHS];
    int             string_length;
    int             string_reload;

    FT_Stroker      stroker;
    FT_Bitmap       bitmap;            /* used as bitmap conversion buffer */

  } FTDemo_Handle;


  FTDemo_Handle*
  FTDemo_New( void );


  void
  FTDemo_Done( FTDemo_Handle*  handle );


  /* install a font */
  FT_Error
  FTDemo_Install_Font( FTDemo_Handle*  handle,
                       const char*     filepath,
                       FT_Bool         outline_only,
                       FT_Bool         no_instances );


  void
  FTDemo_Set_Preload( FTDemo_Handle*  handle,
                      int             preload );

  void
  FTDemo_Set_Current_Font( FTDemo_Handle*  handle,
                           PFont           font );

  void
  FTDemo_Set_Current_Size( FTDemo_Handle*  handle,
                           int             pixel_size );

  void
  FTDemo_Set_Current_Charsize( FTDemo_Handle*  handle,
                               int             point_size,
                               int             res );

  void
  FTDemo_Update_Current_Flags( FTDemo_Handle*  handle );


  /* charcode => glyph index of current font */
  FT_UInt
  FTDemo_Get_Index( FTDemo_Handle*  handle,
                    FT_UInt32       charcode );


  /* get FT_Size of current font */
  FT_Error
  FTDemo_Get_Size( FTDemo_Handle*  handle,
                   FT_Size*        asize );


  /* convert a FT_Glyph to a grBitmap (don't free target->buffer) */
  /* if aglyf != NULL, you should FT_Glyph_Done the aglyf */
  FT_Error
  FTDemo_Glyph_To_Bitmap( FTDemo_Handle*  handle,
                          FT_Glyph        glyf,
                          grBitmap*       target,
                          int*            left,
                          int*            top,
                          int*            x_advance,
                          int*            y_advance,
                          FT_Glyph*       aglyf );

  /* get a grBitmap from glyph index (don't free target->buffer) */
  /* if aglyf != NULL, you should FT_Glyph_Done the aglyf */
  FT_Error
  FTDemo_Index_To_Bitmap( FTDemo_Handle*  handle,
                          FT_ULong        Index,
                          grBitmap*       target,
                          int*            left,
                          int*            top,
                          int*            x_advance,
                          int*            y_advance,
                          FT_Glyph*       aglyf );


  /* set the string to be drawn */
  void
  FTDemo_String_Set( FTDemo_Handle*  handle,
                     const char*     string );


  /* make a FT_Encoding tag from a string */
  unsigned long
  FTDemo_Make_Encoding_Tag( const char*  s );

#ifdef __cplusplus
}
#endif

#endif /* FTCOMMON_H_ */

/* End */
