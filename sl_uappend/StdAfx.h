/**
 * Copyright - xWhitey, 2025.
 * StdAfx.h - This is a precompiled header file.
 * Files listed below are compiled only once, improving build performance for future builds.
 * This also affects IntelliSense performance, including code completion and many code browsing features
 * However, files listed here are ALL re-compiled if any one of them is updated between builds.
 * Do not add files here that you will be updating frequently as this negates the performance advantage.
 *
 * goldsrc-paper-next (a total reboot of goldsrc-paper) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef STDAFX_H_RECURSE_GUARD
#error Recursive header files inclusion detected in StdAfx.h
#else //STDAFX_H_RECURSE_GUARD

#define STDAFX_H_RECURSE_GUARD

#ifndef STDAFX_H_GUARD
#define STDAFX_H_GUARD

#ifdef _MSC_VER
#pragma once
#endif //_MSC_VER

// add headers that you want to pre-compile here
#include "common.hpp"

#endif //STDAFX_H_GUARD

#undef STDAFX_H_RECURSE_GUARD
#endif //STDAFX_H_RECURSE_GUARD

#ifdef _MSC_VER
#pragma hdrstop
#endif //_MSC_VER