/**
 * Copyright - xWhitey, 2025.
 * common.hpp - main GSP file with includes.
 *
 * goldsrc-paper-next (a total reboot of goldsrc-paper) header file
 * Authors: xWhitey.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef COMMON_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in common.hpp
#else //COMMON_HPP_RECURSE_GUARD

#define COMMON_HPP_RECURSE_GUARD

#ifndef COMMON_HPP_GUARD
#define COMMON_HPP_GUARD

#ifdef _MSC_VER
#pragma once
#endif //_MSC_VER

// Include GSP's config before anything else
//#include "config.h"

#if defined(_WIN32) || defined(_MSC_VER)
#if GSP_ENHANCES
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define WIN32_EXTRA_LEAN				// Exclude even more stuff if this is even possible
#define VC_EXTRALEAN					// ^^^^^^^
#endif //GSP_ENHANCES
// Windows Header Files
//#include "winquake.h"
#include <Windows.h>
#endif //defined(_WIN32) or defined(_MSC_VER)

// C Runtime Library
#include <stdio.h>
#include <stdlib.h>

//#include "quakedef.h"

// glquake.h goes last because it also includes Windows.h and here we're defining WIN32_LEAN_AND_MEAN (under GSP_ENHANCES)
//#include "glquake.h"

// Sys_ functions
//#include "sys.h"

// GSP stuff
//#include "globals.hpp"
#include "patch.hpp"
#include "reversible_hooks.hpp"
//#include "stub_routines.hpp"

#if 0
#ifdef __cplusplus
template<class _Ty> struct remove_reference_s { typedef _Ty type; };
template<class _Ty> struct remove_reference_s<_Ty&> { typedef _Ty type; };
// don't forget the rvalue reference too!
template<class _Ty> struct remove_reference_s<_Ty&&> { typedef _Ty type; };
#endif //__cplusplus

extern size_t GSP_GetVariableByRVA( size_t _RVA );

// Install a scoped-to-engine hook.
#define RH_ScopedInstall(_RVA, _Hooked) _ ## _Hooked = RH_InstallHook(_RVA, g_pHardwareEngineHandle, #_Hooked, _Hooked)

//
#define RH_ScopedCallOriginal(_Hooked, ...) RH_CallOriginal(_Hooked, _ ## _Hooked, __VA_ARGS__)
#define RH_ScopedCallOriginalRetVal(_Hooked, _ReturnValue, ...) RH_CallOriginalRetVal(_Hooked, _ ## _Hooked, _ReturnValue, __VA_ARGS__)
#define RH_Hook(_Name) CReversibleFunctionHook* _ ## _Name = NULL

#define GSP_PrintUnimplemented(_Text, ...) do { printf("%s (unimplemented): " _Text, __func__, __VA_ARGS__); } while (0)

#define GSP_Var &
#define GSP_InitVar(_Variable, _RVA, ...) *(remove_reference_s<decltype(_Variable)>::type*)(*(DWORD*)GSP_GetVariableByRVA(_RVA))
#define GSP_InitPtrVar(_Variable, _RVA, ...) (remove_reference_s<decltype(_Variable)>::type)(*(DWORD*)GSP_GetVariableByRVA(_RVA))
//__VA_ARGS__ are kept so we can have the values already initialized after we finish reversing the engine. (i.e. we write the original value there and just refactor that later just by holding ALT and dragging mouse LOL)
#define GSP_Future(...)
#else
// the code after this line isn't an actual part of gspn_win32_8684
#include "hlsdk_mini.hpp"

#define RH_ScopedCallOriginal(_Hooked, ...) RH_CallOriginal(_Hooked, _ ## _Hooked, __VA_ARGS__)
#define RH_ScopedCallOriginalRetVal(_Hooked, _ReturnValue, ...) RH_CallOriginalRetVal(_Hooked, _ ## _Hooked, _ReturnValue, __VA_ARGS__)
#define RH_Hook(_Name) CReversibleFunctionHook* _ ## _Name = NULL
#define RH_ScopedInstallWinAPI(_Function, _Hooked) _ ## _Function = RH_InstallHookByAddress(_Function, #_Function, _Hooked)
#define RH_ScopedInstallByAddress(_Function, _Address, _Hooked) _ ## _Function = RH_InstallHookByAddress(_Address, #_Function, _Hooked)

#define Sys_Error(...) fprintf(stderr, __VA_ARGS__)
#ifdef SLUAPPEND_EXPORTS
#define GAME_EXPORT __declspec(dllexport)
#else //!SLUAPPEND_EXPORTS
#define GAME_EXPORT
#endif //SLUAPPEND_EXPORTS
#endif //0

#endif //COMMON_HPP_GUARD

#undef COMMON_HPP_RECURSE_GUARD
#endif //COMMON_HPP_RECURSE_GUARD