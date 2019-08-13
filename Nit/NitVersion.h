/*
 * PROJECT:   Nit - A Windows Image Tweaker based on DismCore
 * FILE:      NitVersion.h
 * PURPOSE:   Nit Version Definition
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

// #include "CIBuild.h"

#ifndef NIT_VER
#define NIT_VER

#define NIT_VER_MAJOR 0
#define NIT_VER_MINOR 1
#define NIT_VER_BUILD 0
#define NIT_VER_REV 0
#endif

#ifndef NIT_VER_FMT_COMMA
#define NIT_VER_FMT_COMMA NIT_VER_MAJOR,NIT_VER_MINOR,NIT_VER_BUILD,NIT_VER_REV
#endif

#ifndef NIT_VER_FMT_DOT
#define NIT_VER_FMT_DOT NIT_VER_MAJOR.NIT_VER_MINOR.NIT_VER_BUILD.NIT_VER_REV
#endif


#ifndef MACRO_TO_STRING
#define _MACRO_TO_STRING(arg) L#arg
#define MACRO_TO_STRING(arg) _MACRO_TO_STRING(arg)
#endif

#ifndef NIT_VERSION
#define NIT_VERSION NIT_VER_FMT_COMMA
#endif

#ifndef _NIT_VERSION_STRING_
#define _NIT_VERSION_STRING_ MACRO_TO_STRING(NIT_VER_FMT_DOT)
#endif

#ifndef NIT_VERSION_STRING
#ifdef NIT_CI_BUILD
#define NIT_VERSION_STRING _NIT_VERSION_STRING_ L" " NIT_CI_BUILD
#else
#define NIT_VERSION_STRING _NIT_VERSION_STRING_
#endif
#endif
