/****************************************************************************/
/*                                                                          */
/*  The FreeType project -- a free and portable quality TrueType renderer.  */
/*                                                                          */
/*  Copyright 2005-2009, 2011-2013 by                                       */
/*  D. Turner, R.Wilhelm, and W. Lemberg                                    */
/*                                                                          */
/*                                                                          */
/*  ftcommon.c - common routines for the graphic FreeType demo programs.    */
/*                                                                          */
/****************************************************************************/


#ifndef  _GNU_SOURCE
#define  _GNU_SOURCE /* we use `strcasecmp' */
#endif

#include "ft2build.h"
#include FT_FREETYPE_H

#include FT_CACHE_H
#include FT_CACHE_MANAGER_H

#include FT_BITMAP_H

#include "ftcommon.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#ifdef _WIN32
#define strcasecmp  _stricmp
#endif


FT_Error  error;


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****               FREETYPE-SPECIFIC DEFINITIONS                   *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/


#define FLOOR( x )  (   (x)        & -64 )
#define CEIL( x )   ( ( (x) + 63 ) & -64 )
#define ROUND( x )  ( ( (x) + 32 ) & -64 )
#define TRUNC( x )  (   (x) >> 6 )


  /*************************************************************************/
  /*                                                                       */
  /* The face requester is a function provided by the client application   */
  /* to the cache manager, whose role is to translate an `abstract' face   */
  /* ID into a real FT_Face object.                                        */
  /*                                                                       */
  /* In this program, the face IDs are simply pointers to TFont objects.   */
  /*                                                                       */
  static FT_Error
  my_face_requester( FTC_FaceID  face_id,
                     FT_Library  lib,
                     FT_Pointer  request_data,
                     FT_Face*    aface )
  {
    PFont  font = (PFont)face_id;

    FT_UNUSED( request_data );


    if ( font->file_address != NULL )
      error = FT_New_Memory_Face( lib,
                                  (const FT_Byte*)font->file_address,
                                  (FT_Long)font->file_size,
                                  font->face_index,
                                  aface );
    else
      error = FT_New_Face( lib,
                           font->filepathname,
                           font->face_index,
                           aface );
    if ( !error )
    {
      if ( (*aface)->charmaps )
        (*aface)->charmap = (*aface)->charmaps[font->cmap_index];
    }

    return error;
  }


  FTDemo_Handle*
  FTDemo_New( void )
  {
    FTDemo_Handle*  handle;


    handle = (FTDemo_Handle *)malloc( sizeof ( FTDemo_Handle ) );
    if ( !handle )
      return NULL;

    memset( handle, 0, sizeof ( FTDemo_Handle ) );

    error = FT_Init_FreeType( &handle->library );
    error = FTC_Manager_New( handle->library, 0, 0, 0,
                             my_face_requester, 0, &handle->cache_manager );
    error = FTC_SBitCache_New( handle->cache_manager, &handle->sbits_cache );
    error = FTC_ImageCache_New( handle->cache_manager, &handle->image_cache );
    error = FTC_CMapCache_New( handle->cache_manager, &handle->cmap_cache );

    FT_Bitmap_New( &handle->bitmap );

    FT_Stroker_New( handle->library, &handle->stroker );

    handle->hinted    = 1;
    handle->use_sbits = 1;
    handle->autohint  = 0;
    handle->lcd_mode  = LCD_MODE_AA;
    handle->color     = 1;

    handle->use_sbits_cache = 1;

    /* string_init */
    memset( handle->string, 0, sizeof ( TGlyph ) * MAX_GLYPHS );
    handle->string_length = 0;
    handle->string_reload = 1;

    return handle;
  }


  void
  FTDemo_Done( FTDemo_Handle*  handle )
  {
    int  i;


    if ( !handle )
      return;

    for ( i = 0; i < handle->max_fonts; i++ )
    {
      if ( handle->fonts[i] )
      {
        if ( handle->fonts[i]->filepathname )
          free( (void*)handle->fonts[i]->filepathname );
        free( handle->fonts[i] );
      }
    }
    free( handle->fonts );

    /* string_done */
    for ( i = 0; i < MAX_GLYPHS; i++ )
    {
      PGlyph  glyph = handle->string + i;


      if ( glyph->image )
        FT_Done_Glyph( glyph->image );
    }

    FT_Stroker_Done( handle->stroker );
    FT_Bitmap_Done( handle->library, &handle->bitmap );
    FTC_Manager_Done( handle->cache_manager );
    FT_Done_FreeType( handle->library );

    free( handle );
  }


  FT_Error
  FTDemo_Install_Font( FTDemo_Handle*  handle,
                       const char*     filepath,
                       FT_Bool         outline_only,
                       FT_Bool         no_instances )
  {
    static char   filename[1024 + 5];
    long          i, num_faces;
    unsigned int  len;
    FT_Face       face;


    len = strlen( filepath );
    if ( len > 1024 )
      len = 1024;

    strncpy( filename, filepath, len );
    filename[len] = 0;

    /* We use a conservative approach here, at the cost of calling     */
    /* `FT_New_Face' quite often.  The idea is that our demo programs  */
    /* should be able to try all faces and named instances of a font,  */
    /* expecting that some faces don't work for various reasons, e.g., */
    /* a broken subfont, or an unsupported NFNT bitmap font in a Mac   */
    /* dfont resource that holds more than a single font.              */

    error = FT_New_Face( handle->library, filename, -1, &face );
    if ( error )
      return error;
    num_faces = face->num_faces;
    FT_Done_Face( face );

    /* allocate new font object(s) */
    for ( i = 0; i < num_faces; i++ )
    {
      PFont  font;
      long   j, instance_count;


      error = FT_New_Face( handle->library, filename, -( i + 1 ), &face );
      if ( error )
        continue;
      instance_count = no_instances ? 0 : face->style_flags >> 16;
      FT_Done_Face( face );

      /* load face with and without named instances */
      for ( j = 0; j < instance_count + 1; j++ )
      {
        error = FT_New_Face( handle->library,
                             filename,
                             ( j << 16 ) + i,
                             &face );
        if ( error )
          continue;

        if ( outline_only && !FT_IS_SCALABLE( face ) )
        {
          FT_Done_Face( face );
          continue;
        }

          error = FT_Select_Charmap( face, (FT_Encoding)FT_ENCODING_UNICODE );
          if ( error )
          {
            FT_Done_Face( face );
            continue;
          }

        font = (PFont)malloc( sizeof ( *font ) );

        /* We allocate four more bytes since we want to attach an AFM */
        /* or PFM file for Type 1 fonts (if available).  Such fonts   */
        /* always have the extension `.afm' or `.pfm'.                */
        font->filepathname = (char*)malloc( strlen( filename ) + 4 + 1 );
        strcpy( (char*)font->filepathname, filename );

        font->face_index = ( j << 16 ) + i;
        font->cmap_index = face->charmap ? FT_Get_Charmap_Index( face->charmap )
                                         : 0;

        if ( handle->preload )
        {
          FILE*   file = fopen( filename, "rb" );
          size_t  file_size;


          if ( file == NULL )  /* shouldn't happen */
          {
            free( font );
            return FT_Err_Invalid_Argument;
          }

          fseek( file, 0, SEEK_END );
          file_size = (size_t)ftell( file );
          fseek( file, 0, SEEK_SET );

          if ( file_size <= 0 )
          {
            free( font );
            fclose( file );
            return FT_Err_Invalid_Stream_Operation;
          }

          font->file_address = malloc( file_size );
          if ( !font->file_address )
          {
            free( font );
            fclose( file );
            return FT_Err_Out_Of_Memory;
          }

          if ( fread( font->file_address, 1, file_size, file ) != file_size )
          {
            free( font->file_address );
            free( font );
            fclose( file );
            return FT_Err_Invalid_Stream_Read;
          }

          font->file_size = file_size;

          fclose( file );
        }
        else
        {
          font->file_address = NULL;
          font->file_size    = 0;
        }

        font->num_indices = 0x110000L;

        FT_Done_Face( face );
        face = NULL;

        if ( handle->max_fonts == 0 )
        {
          handle->max_fonts = 16;
          handle->fonts     = (PFont*)calloc( (size_t)handle->max_fonts,
                                              sizeof ( PFont ) );
        }
        else if ( handle->num_fonts >= handle->max_fonts )
        {
          handle->max_fonts *= 2;
          handle->fonts      = (PFont*)realloc( handle->fonts,
                                                (size_t)handle->max_fonts *
                                                  sizeof ( PFont ) );

          memset( &handle->fonts[handle->num_fonts], 0,
                  (size_t)( handle->max_fonts - handle->num_fonts ) *
                    sizeof ( PFont ) );
        }

        handle->fonts[handle->num_fonts++] = font;
      }
    }

    return FT_Err_Ok;
  }


  void
  FTDemo_Set_Current_Font( FTDemo_Handle*  handle,
                           PFont           font )
  {
    handle->current_font   = font;
    handle->scaler.face_id = (FTC_FaceID)font;

    handle->string_reload = 1;
  }


  void
  FTDemo_Set_Current_Size( FTDemo_Handle*  handle,
                           int             pixel_size )
  {
    if ( pixel_size > 0xFFFF )
      pixel_size = 0xFFFF;

    handle->scaler.width  = (FT_UInt)pixel_size;
    handle->scaler.height = (FT_UInt)pixel_size;
    handle->scaler.pixel  = 1;                  /* activate integer format */
    handle->scaler.x_res  = 0;
    handle->scaler.y_res  = 0;

    handle->string_reload = 1;
  }


  void
  FTDemo_Set_Current_Charsize( FTDemo_Handle*  handle,
                               int             char_size,
                               int             resolution )
  {
    /* in 26.6 format, corresponding to (almost) 0x4000ppem */
    if ( char_size > 0xFFFFF )
      char_size = 0xFFFFF;

    handle->scaler.width  = (FT_UInt)char_size;
    handle->scaler.height = (FT_UInt)char_size;
    handle->scaler.pixel  = 0;                     /* activate 26.6 format */
    handle->scaler.x_res  = (FT_UInt)resolution;
    handle->scaler.y_res  = (FT_UInt)resolution;

    handle->string_reload = 1;
  }


  void
  FTDemo_Set_Preload( FTDemo_Handle*  handle,
                      int             preload )
  {
    handle->preload = !!preload;
  }


  void
  FTDemo_Update_Current_Flags( FTDemo_Handle*  handle )
  {
    FT_Int32  flags, target;


    flags = FT_LOAD_DEFAULT;  /* really 0 */

    if ( handle->autohint )
      flags |= FT_LOAD_FORCE_AUTOHINT;

    if ( !handle->use_sbits )
      flags |= FT_LOAD_NO_BITMAP;

    if ( handle->hinted )
    {
      target = 0;

      switch ( handle->lcd_mode )
      {
      case LCD_MODE_MONO:
        target = FT_LOAD_TARGET_MONO;
        break;

      case LCD_MODE_LIGHT:
      case LCD_MODE_LIGHT_SUBPIXEL:
        target = FT_LOAD_TARGET_LIGHT;
        break;

      case LCD_MODE_RGB:
      case LCD_MODE_BGR:
        target = FT_LOAD_TARGET_LCD;
        break;

      case LCD_MODE_VRGB:
      case LCD_MODE_VBGR:
        target = FT_LOAD_TARGET_LCD_V;
        break;

      default:
        target = FT_LOAD_TARGET_NORMAL;
      }

      flags |= target;
    }
    else
    {
      flags |= FT_LOAD_NO_HINTING;

      if ( handle->lcd_mode == LCD_MODE_MONO )
        flags |= FT_LOAD_MONOCHROME;
    }

//    if ( handle->color )
//      flags |= FT_LOAD_COLOR;

    handle->load_flags    = flags;
    handle->string_reload = 1;
  }


  FT_UInt
  FTDemo_Get_Index( FTDemo_Handle*  handle,
                    FT_UInt32       charcode )
  {
    FTC_FaceID  face_id = handle->scaler.face_id;
    PFont       font    = handle->current_font;


    return FTC_CMapCache_Lookup( handle->cmap_cache, face_id,
                                 font->cmap_index, charcode );
  }


  FT_Error
  FTDemo_Get_Size( FTDemo_Handle*  handle,
                   FT_Size*        asize )
  {
    FT_Size  size;


    error = FTC_Manager_LookupSize( handle->cache_manager,
                                    &handle->scaler,
                                    &size );

    if ( !error )
      *asize = size;

    return error;
  }


  FT_Error
  FTDemo_Glyph_To_Bitmap( FTDemo_Handle*  handle,
                          FT_Glyph        glyf,
                          grBitmap*       target,
                          int*            left,
                          int*            top,
                          int*            x_advance,
                          int*            y_advance,
                          FT_Glyph*       aglyf )
  {
    FT_BitmapGlyph  bitmap;
    FT_Bitmap*      source;


    *aglyf = NULL;

    error = FT_Err_Ok;

    if ( glyf->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      FT_Render_Mode  render_mode;


      switch ( handle->lcd_mode )
      {
      case LCD_MODE_MONO:
        render_mode = FT_RENDER_MODE_MONO;
        break;

      case LCD_MODE_LIGHT:
      case LCD_MODE_LIGHT_SUBPIXEL:
        render_mode = FT_RENDER_MODE_LIGHT;
        break;

      case LCD_MODE_RGB:
      case LCD_MODE_BGR:
        render_mode = FT_RENDER_MODE_LCD;
        break;

      case LCD_MODE_VRGB:
      case LCD_MODE_VBGR:
        render_mode = FT_RENDER_MODE_LCD_V;
        break;

      default:
        render_mode = FT_RENDER_MODE_NORMAL;
      }

      /* render the glyph to a bitmap, don't destroy original */
      error = FT_Glyph_To_Bitmap( &glyf, render_mode, NULL, 0 );
      if ( error )
        return error;

      *aglyf = glyf;
    }

    bitmap = (FT_BitmapGlyph)glyf;
    source = &bitmap->bitmap;

    target->rows   = (int)source->rows;
    target->width  = (int)source->width;
    target->pitch  = source->pitch;
    target->buffer = source->buffer;
    target->grays  = source->num_grays;

    switch ( source->pixel_mode )
    {
    case FT_PIXEL_MODE_MONO:
      target->mode = gr_pixel_mode_mono;
      break;

    case FT_PIXEL_MODE_GRAY:
      target->mode  = gr_pixel_mode_gray;
      target->grays = source->num_grays;
      break;

    case FT_PIXEL_MODE_GRAY2:
    case FT_PIXEL_MODE_GRAY4:
      (void)FT_Bitmap_Convert( handle->library, source, &handle->bitmap, 1 );
      target->pitch  = handle->bitmap.pitch;
      target->buffer = handle->bitmap.buffer;
      target->mode   = gr_pixel_mode_gray;
      target->grays  = handle->bitmap.num_grays;
      break;

    case FT_PIXEL_MODE_LCD:
      target->mode  = handle->lcd_mode == LCD_MODE_RGB
                      ? gr_pixel_mode_lcd
                      : gr_pixel_mode_lcd2;
      target->grays = source->num_grays;
      break;

    case FT_PIXEL_MODE_LCD_V:
      target->mode  = handle->lcd_mode == LCD_MODE_VRGB
                      ? gr_pixel_mode_lcdv
                      : gr_pixel_mode_lcdv2;
      target->grays = source->num_grays;
      break;

//    case FT_PIXEL_MODE_BGRA:
//      target->mode  = gr_pixel_mode_bgra;
//      target->grays = source->num_grays;
//      break;

    default:
      return FT_Err_Invalid_Glyph_Format;
    }

    *left = bitmap->left;
    *top  = bitmap->top;

    *x_advance = ( glyf->advance.x + 0x8000 ) >> 16;
    *y_advance = ( glyf->advance.y + 0x8000 ) >> 16;

    return error;
  }


  FT_Error
  FTDemo_Index_To_Bitmap( FTDemo_Handle*  handle,
                          FT_ULong        Index,
                          grBitmap*       target,
                          int*            left,
                          int*            top,
                          int*            x_advance,
                          int*            y_advance,
                          FT_Glyph*       aglyf )
  {
    unsigned int  width, height;


    *aglyf     = NULL;
    *x_advance = 0;

    /* use the SBits cache to store small glyph bitmaps; this is a lot */
    /* more memory-efficient                                           */
    /*                                                                 */

    width  = handle->scaler.width;
    height = handle->scaler.height;
    if ( handle->use_sbits_cache && !handle->scaler.pixel )
    {
      width  = ( ( width * handle->scaler.x_res + 36 ) / 72 )  >> 6;
      height = ( ( height * handle->scaler.y_res + 36 ) / 72 ) >> 6;
    }

    if ( handle->use_sbits_cache && width < 48 && height < 48 )
    {
      FTC_SBit   sbit;
      FT_Bitmap  source;


      error = FTC_SBitCache_LookupScaler( handle->sbits_cache,
                                          &handle->scaler,
                                          (FT_ULong)handle->load_flags,
                                          Index,
                                          &sbit,
                                          NULL );
      if ( error )
        goto Exit;

      if ( sbit->buffer )
      {
        target->rows   = sbit->height;
        target->width  = sbit->width;
        target->pitch  = sbit->pitch;
        target->buffer = sbit->buffer;
        target->grays  = sbit->max_grays + 1;

        switch ( sbit->format )
        {
        case FT_PIXEL_MODE_MONO:
          target->mode = gr_pixel_mode_mono;
          break;

        case FT_PIXEL_MODE_GRAY:
          target->mode  = gr_pixel_mode_gray;
          break;

        case FT_PIXEL_MODE_GRAY2:
        case FT_PIXEL_MODE_GRAY4:
          source.rows       = sbit->height;
          source.width      = sbit->width;
          source.pitch      = sbit->pitch;
          source.buffer     = sbit->buffer;
          source.pixel_mode = sbit->format;

          (void)FT_Bitmap_Convert( handle->library, &source,
                                   &handle->bitmap, 1 );

          target->pitch  = handle->bitmap.pitch;
          target->buffer = handle->bitmap.buffer;
          target->mode   = gr_pixel_mode_gray;
          target->grays  = handle->bitmap.num_grays;
          break;

        case FT_PIXEL_MODE_LCD:
          target->mode  = handle->lcd_mode == LCD_MODE_RGB
                          ? gr_pixel_mode_lcd
                          : gr_pixel_mode_lcd2;
          break;

        case FT_PIXEL_MODE_LCD_V:
          target->mode  = handle->lcd_mode == LCD_MODE_VRGB
                          ? gr_pixel_mode_lcdv
                          : gr_pixel_mode_lcdv2;
          break;

//        case FT_PIXEL_MODE_BGRA:
//          target->mode  = gr_pixel_mode_bgra;
//          break;

        default:
          return FT_Err_Invalid_Glyph_Format;
        }

        *left      = sbit->left;
        *top       = sbit->top;
        *x_advance = sbit->xadvance;
        *y_advance = sbit->yadvance;

        goto Exit;
      }
    }

    /* otherwise, use an image cache to store glyph outlines, and render */
    /* them on demand. we can thus support very large sizes easily..     */
    {
      FT_Glyph  glyf;


      error = FTC_ImageCache_LookupScaler( handle->image_cache,
                                           &handle->scaler,
                                           (FT_ULong)handle->load_flags,
                                           Index,
                                           &glyf,
                                           NULL );

      if ( !error )
        error = FTDemo_Glyph_To_Bitmap( handle, glyf, target, left, top,
                                        x_advance, y_advance, aglyf );
    }

  Exit:
    /* don't accept a `missing' character with zero or negative width */
    if ( Index == 0 && *x_advance <= 0 )
      *x_advance = 1;

    return error;
  }

  void
  FTDemo_String_Set( FTDemo_Handle*  handle,
                     const char*     string )
  {
    const char*    p = string;
    const char*    end = p + strlen( string );
    unsigned long  codepoint;
    int            ch;
    int            expect;
    PGlyph         glyph = handle->string;


    handle->string_length = 0;
    codepoint = expect = 0;

    for (;;)
    {
      ch = *p;
      if ( p > end )
        break;

      codepoint = (unsigned long)ch;

      glyph->glyph_index = FTDemo_Get_Index( handle, codepoint );

      p++;
      glyph++;
      handle->string_length++;

      if ( handle->string_length >= MAX_GLYPHS )
        break;
    }

    handle->string_reload = 1;
  }


  static FT_Error
  string_load( FTDemo_Handle*  handle )
  {
    int      n;
    FT_Size  size;
    FT_Face  face;
    FT_Pos   prev_rsb_delta = 0;


    error = FTDemo_Get_Size( handle, &size );
    if ( error )
      return error;

    face = size->face;

    for ( n = 0; n < handle->string_length; n++ )
    {
      PGlyph  glyph = handle->string + n;


      /* clear existing image if there is one */
      if ( glyph->image )
      {
        FT_Done_Glyph( glyph->image );
        glyph->image = NULL;
      }

      /* load the glyph and get the image */
      if ( !FT_Load_Glyph( face, glyph->glyph_index,
                           handle->load_flags )        &&
           !FT_Get_Glyph( face->glyph, &glyph->image ) )
      {
        FT_Glyph_Metrics*  metrics = &face->glyph->metrics;


        /* note that in vertical layout, y-positive goes downwards */

        glyph->vvector.x  =  metrics->vertBearingX - metrics->horiBearingX;
        glyph->vvector.y  = -metrics->vertBearingY - metrics->horiBearingY;

        glyph->vadvance.x = 0;
        glyph->vadvance.y = -metrics->vertAdvance;

        if ( handle->lcd_mode == LCD_MODE_LIGHT_SUBPIXEL )
          glyph->delta = face->glyph->lsb_delta - face->glyph->rsb_delta;
        else
        {
          if ( prev_rsb_delta - face->glyph->lsb_delta >= 32 )
            glyph->delta = -1 * 64;
          else if ( prev_rsb_delta - face->glyph->lsb_delta < -32 )
            glyph->delta = 1 * 64;
          else
            glyph->delta = 0;

          prev_rsb_delta = face->glyph->rsb_delta;
        }
      }
    }

    return FT_Err_Ok;
  }


  static FT_Error
  string_render_prepare( FTDemo_Handle*          handle,
                         FTDemo_String_Context*  sc,
                         FT_Vector*              advances )
  {
    FT_Face     face;
    FT_Size     size;
    PGlyph      glyph;
    FT_Pos      track_kern   = 0;
    FT_UInt     prev_index   = 0;
    FT_Vector*  prev_advance = NULL;
    FT_Vector   extent       = { 0, 0 };
    FT_Int      i;


    error = FTDemo_Get_Size( handle, &size );
    if ( error )
      return error;

    face = size->face;

    if ( !sc->vertical && sc->kerning_degree )
    {
      /* this function needs and returns points, not pixels */
      if ( FT_Get_Track_Kerning( face,
                                 (FT_Fixed)handle->scaler.width << 10,
                                 -sc->kerning_degree,
                                 &track_kern ) )
        track_kern = 0;
      else
        track_kern = (FT_Pos)(
                       ( track_kern / 1024.0 * handle->scaler.x_res ) /
                       72.0 );
    }

    for ( i = 0; i < handle->string_length; i++ )
    {
      glyph = handle->string + i;

      if ( !glyph->image )
        continue;

      if ( sc->vertical )
        advances[i] = glyph->vadvance;
      else
      {
        advances[i]     = glyph->image->advance;
        advances[i].x >>= 10;
        advances[i].y >>= 10;

        if ( handle->lcd_mode == LCD_MODE_LIGHT_SUBPIXEL )
          advances[i].x += glyph->delta;

        if ( prev_advance )
        {
          prev_advance->x += track_kern;

          if ( sc->kerning_mode )
          {
            FT_Vector  kern;


            FT_Get_Kerning( face, prev_index, glyph->glyph_index,
                FT_KERNING_UNFITTED, &kern );

            prev_advance->x += kern.x;
            prev_advance->y += kern.y;

            if ( handle->lcd_mode != LCD_MODE_LIGHT_SUBPIXEL &&
                 sc->kerning_mode > KERNING_MODE_NORMAL      )
              prev_advance->x += glyph->delta;
          }
        }
      }

      if ( prev_advance )
      {
        if ( handle->lcd_mode != LCD_MODE_LIGHT_SUBPIXEL &&
             handle->hinted                              )
        {
          prev_advance->x = ROUND( prev_advance->x );
          prev_advance->y = ROUND( prev_advance->y );
        }

        extent.x += prev_advance->x;
        extent.y += prev_advance->y;
      }

      prev_index   = glyph->glyph_index;
      prev_advance = advances + i;
    }

    if ( prev_advance )
    {
      if ( handle->lcd_mode != LCD_MODE_LIGHT_SUBPIXEL &&
           handle->hinted                              )
      {
        prev_advance->x = ROUND( prev_advance->x );
        prev_advance->y = ROUND( prev_advance->y );
      }

      extent.x += prev_advance->x;
      extent.y += prev_advance->y;
    }

    /* store the extent in the last slot */
    i = handle->string_length - 1;
    advances[i] = extent;

    return FT_Err_Ok;
  }

  unsigned long
  FTDemo_Make_Encoding_Tag( const char*  s )
  {
    int            i;
    unsigned long  l = 0;


    for ( i = 0; i < 4; i++ )
    {
      if ( !s[i] )
        break;
      l <<= 8;
      l  += (unsigned long)s[i];
    }

    return l;
  }


/* End */