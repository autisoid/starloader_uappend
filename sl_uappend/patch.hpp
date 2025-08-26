/**
 * Copyright - xWhitey, 2025.
 * patch.hpp - CPatch
 *
 * goldsrc-paper-next (a total reboot of goldsrc-paper) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef PATCH_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in patch.hpp
#else //PATCH_HPP_RECURSE_GUARD

#define PATCH_HPP_RECURSE_GUARD

#ifndef PATCH_HPP_GUARD
#define PATCH_HPP_GUARD

#ifdef _MSC_VER
#pragma once
#endif //_MSC_VER

typedef struct Patch_c
{
#ifdef __cplusplus
	static void Patch( void* _Address, void* _Data, size_t _Size );
	static void PlaceJump( void* _Address, void* _Target, void* _OriginalBytes );
#else
	int m_nCSucks;
#endif //__cplusplus
} Patch_c, CPatch;

#ifdef __cplusplus
using Patch_c = struct Patch_c;
#endif //__cplusplus

#endif //PATCH_HPP_GUARD

#undef PATCH_HPP_RECURSE_GUARD
#endif //PATCH_HPP_RECURSE_GUARD