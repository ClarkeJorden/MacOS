// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#include <math.h>
#include "Define_Type.h"
#include "resource.h"
#ifdef _WIN32
#include "glew/include/GL/glew.h"
#include <gl/GL.h>
#else
#include <OpenGLES/ES1/gl.h>
#endif
#ifndef GL_FIXED
#define GL_FIXED GL_INT
#endif

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE GL_CLAMP
#endif

#define 	FONTDIR			"/usr/local/IP_STB/Fonts/"
#define 	FONTENUMFILE	"fonts.dir"

#define 	DEFFONTFILENAME "KCGGot.ttf"
#define     DEFFONTWIDTHSCALE 1.0526f

