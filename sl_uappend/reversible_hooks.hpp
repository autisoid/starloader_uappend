/**
 * Copyright - xWhitey, 2025.
 * reversible_hooks.hpp - Reversible hooks (Can be turned on/off in a shiny ImGui menu)
 *
 * goldsrc-paper-next (a total reboot of goldsrc-paper) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef REVERSIBLE_HOOKS_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in reversible_hooks.hpp
#else //REVERSIBLE_HOOKS_HPP_RECURSE_GUARD

#define REVERSIBLE_HOOKS_HPP_RECURSE_GUARD

#ifndef REVERSIBLE_HOOKS_HPP_GUARD
#define REVERSIBLE_HOOKS_HPP_GUARD

#ifdef _MSC_VER
#pragma once
#endif //_MSC_VER

struct ReversibleHooks_c;
struct ReversibleFunctionHook_c;

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
extern void GSP_InitialiseReversibleHooks( void );
extern void GSP_DestroyReversibleHooks( void );
extern void GSP_RH_Initialise( struct ReversibleHooks_c* _ThisPointer );
extern struct ReversibleFunctionHook_c* GSP_RH_InstallHook( struct ReversibleHooks_c* _ThisPointer, void* _Module, size_t _RVA, const char* _FunctionName, void* _Hooked );
extern struct ReversibleFunctionHook_c* GSP_RH_InstallHookByAddress( struct ReversibleHooks_c* _ThisPointer, void* _Address, const char* _FunctionName, void* _Hooked );
extern void GSP_RH_DestroyHook( struct ReversibleHooks_c* _ThisPointer, struct ReversibleFunctionHook_c* _Hook );
extern struct ReversibleFunctionHook_c* GSP_RH_FindHookByName( struct ReversibleHooks_c* _ThisPointer, const char* _FunctionName );
extern void GSP_RH_EnableHook( struct ReversibleHooks_c* _ThisPointer, struct ReversibleFunctionHook_c* _Hook );
extern void GSP_RH_DisableHook( struct ReversibleHooks_c* _ThisPointer, struct ReversibleFunctionHook_c* _Hook );
extern void GSP_RH_Shutdown( struct ReversibleHooks_c* _ThisPointer );
#ifdef __cplusplus
};
#endif //__cplusplus

#ifdef __cplusplus
#define RH_CallOriginal(_Hooked, _Hook, ...) do {																				\
													decltype(_Hooked)* pfnOriginal = NULL;										\
													pfnOriginal = (decltype(pfnOriginal))_Hook->m_pPrecomputedOriginalAddress;	\
													GSP_RH_DisableHook(g_pReversibleHooks, _Hook);								\
													pfnOriginal(__VA_ARGS__);													\
													GSP_RH_EnableHook(g_pReversibleHooks, _Hook);								\
											} while (0)

#define RH_CallOriginalRetVal(_Hooked, _Hook, _ReturnValue, ...) do {																							\
																	decltype(_Hooked)* pfnOriginal = NULL;														\
																	pfnOriginal = (decltype(pfnOriginal))_Hook->m_pPrecomputedOriginalAddress;					\
																	GSP_RH_DisableHook(g_pReversibleHooks, _Hook);												\
																	_ReturnValue = pfnOriginal(__VA_ARGS__);													\
																	GSP_RH_EnableHook(g_pReversibleHooks, _Hook);												\
															} while (0)
#endif //__cplusplus

#ifdef __cplusplus
extern "C"
#endif //__cplusplus
extern struct ReversibleHooks_c* g_pReversibleHooks;

#define RH_Initialise() do { GSP_InitialiseReversibleHooks(); g_pReversibleHooks->m_pVtbl->Initialise(g_pReversibleHooks); } while (0)
#define RH_Shutdown() do { g_pReversibleHooks->m_pVtbl->Shutdown(g_pReversibleHooks); GSP_DestroyReversibleHooks(); } while (0)
#define RH_InstallHook(_RVA, _Module, _FunctionName, _Hooked) g_pReversibleHooks->m_pVtbl->InstallHook(g_pReversibleHooks, _Module, _RVA, _FunctionName, _Hooked)
//not gspn_win32_8684
#define RH_InstallHookByAddress(_Address, _FunctionName, _Hooked) g_pReversibleHooks->m_pVtbl->InstallHookByAddress(g_pReversibleHooks, _Address, _FunctionName, _Hooked)

typedef struct ReversibleFunctionHook_c
{
	size_t m_cbRVA;
	const char* m_pszFunctionName;
	void* m_pfnHooked;
	unsigned char* m_puchOriginalBytesAtFunctionPrologue;
	void* m_pPrecomputedOriginalAddress; // So we don't waste time calculating the original address a lot of times when calling original function if we do so.

	struct ReversibleFunctionHook_c* m_pNext;
	struct ReversibleFunctionHook_c* m_pPrev;
} ReversibleFunctionHook_c;

#ifdef __cplusplus
using ReversibleFunctionHook_c = struct ReversibleFunctionHook_c;
using CReversibleFunctionHook = struct ReversibleFunctionHook_c;
#endif //__cplusplus

typedef struct ReversibleHooksFunctionTable_c
{
	void (*Initialise)( struct ReversibleHooks_c* _ThisPointer );
	void (*Shutdown)( struct ReversibleHooks_c* _ThisPointer );
	ReversibleFunctionHook_c* (*InstallHook)( struct ReversibleHooks_c* _ThisPointer, void* _Module, size_t _RVA, const char* _FunctionName, void* _Hooked ); //Since we're _**replacing**_ the original functions, we don't need a trampoline -- hence no trampoline pointer.
	ReversibleFunctionHook_c* (*FindHookByName)( struct ReversibleHooks_c* _ThisPointer, const char* _FunctionName );
	ReversibleFunctionHook_c* (*InstallHookByAddress)( struct ReversibleHooks_c* _ThisPointer, void* _Address, const char* _FunctionName, void* _Hooked );
} ReversibleHooksFunctionTable_c, CReversibleHooksFunctionTable;

#ifdef __cplusplus
using ReversibleHooksFunctionTable_c = struct ReversibleHooksFunctionTable_c;
using CReversibleHooksFunctionTable = struct ReversibleHooksFunctionTable_c;
#endif //__cplusplus

typedef struct ReversibleHooks_c
{
	ReversibleHooksFunctionTable_c* m_pVtbl;
	ReversibleFunctionHook_c* m_pFunctionHooksHead;
	ReversibleFunctionHook_c* m_pFunctionHooksTail;
} ReversibleHooks_c, CReversibleHooks;

#ifdef __cplusplus
using ReversibleHooks_c = struct ReversibleHooks_c;
using CReversibleHooks = struct ReversibleHooks_c;
#endif //__cplusplus

#endif //REVERSIBLE_HOOKS_HPP_GUARD

#undef REVERSIBLE_HOOKS_HPP_RECURSE_GUARD
#endif //REVERSIBLE_HOOKS_HPP_RECURSE_GUARD