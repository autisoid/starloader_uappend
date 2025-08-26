/**
 * Copyright - xWhitey, 2025.
 * reversible_hooks.cpp - Reversible hooks (Can be turned on/off in a shiny ImGui menu <- TODO)
 *
 * goldsrc-paper-next (a total reboot of goldsrc-paper) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"

ReversibleHooks_c* g_pReversibleHooks;

/*
================
GSP_InitialiseReversibleHooks

Initialise g_pReversibleHooks singleton
================
*/
void GSP_InitialiseReversibleHooks( void )
{
	g_pReversibleHooks = new CReversibleHooks();

	g_pReversibleHooks->m_pVtbl = (CReversibleHooksFunctionTable*)malloc(sizeof(CReversibleHooksFunctionTable));
	if (!g_pReversibleHooks->m_pVtbl)
	{
		Sys_Error("%s: vtable was nullptr\n", __func__);
		
		return;
	}

	memset(g_pReversibleHooks->m_pVtbl, 0, sizeof(CReversibleHooksFunctionTable));

	//Fill the vftable with functions.
	g_pReversibleHooks->m_pVtbl->Initialise = GSP_RH_Initialise;
	g_pReversibleHooks->m_pVtbl->InstallHook = GSP_RH_InstallHook;
	g_pReversibleHooks->m_pVtbl->Shutdown = GSP_RH_Shutdown;
	g_pReversibleHooks->m_pVtbl->FindHookByName = GSP_RH_FindHookByName;
	g_pReversibleHooks->m_pVtbl->InstallHookByAddress = GSP_RH_InstallHookByAddress;
}

/*
================
GSP_DestroyReversibleHooks
================
*/
void GSP_DestroyReversibleHooks( void )
{
	g_pReversibleHooks = NULL;
}

/*
================
GSP_RH_Initialise
================
*/
void GSP_RH_Initialise( ReversibleHooks_c* _ThisPointer )
{
	// TODO: Implement
}

/*
================
GSP_RH_InstallHook

Install a jmp [eip+OFFSET] hook

_FunctionName MUST BE a string literal
================
*/
ReversibleFunctionHook_c* GSP_RH_InstallHook( ReversibleHooks_c* _ThisPointer, void* _Module, size_t _RVA, const char* _FunctionName, void* _Hooked )
{
	ReversibleFunctionHook_c* pHook = (ReversibleFunctionHook_c*)malloc(sizeof(ReversibleFunctionHook_c));
	if (!pHook)
	{
		Sys_Error("%s: failure allocating memory needed for a hook (%d)\n", __func__, sizeof(ReversibleFunctionHook_c));

		return NULL;
	}

	memset(pHook, 0, sizeof(ReversibleFunctionHook_c));

	pHook->m_cbRVA = _RVA;
	pHook->m_pszFunctionName = _FunctionName;
	pHook->m_pfnHooked = _Hooked;
	pHook->m_puchOriginalBytesAtFunctionPrologue = (unsigned char*)malloc(5); // Since we're making a jmp [eip+OFFSET] hook, which takes five bytes, we must copy only five bytes from the function prologue.
	if (!pHook->m_puchOriginalBytesAtFunctionPrologue)
	{
		Sys_Error("%s: failure allocating %d bytes\n", __func__, 5);

		return NULL;
	}

	pHook->m_pPrecomputedOriginalAddress = (void*)(((size_t)_Module) + _RVA);

	GSP_RH_EnableHook(_ThisPointer, pHook);

	if (!_ThisPointer->m_pFunctionHooksHead)
	{
		_ThisPointer->m_pFunctionHooksHead = _ThisPointer->m_pFunctionHooksTail = pHook;
	}
	else
	{
		_ThisPointer->m_pFunctionHooksTail->m_pNext = pHook;
		pHook->m_pPrev = _ThisPointer->m_pFunctionHooksTail;
		_ThisPointer->m_pFunctionHooksTail = pHook;
	}

	return pHook;
}

/*
================
GSP_RH_DestroyHook

Free the memory taken by _Hook and remove it from the linked list of hooks
================
*/
void GSP_RH_DestroyHook( struct ReversibleHooks_c* _ThisPointer, struct ReversibleFunctionHook_c* _Hook )
{
	ReversibleFunctionHook_c* pNext;
	ReversibleFunctionHook_c* pPrev;

	// Unlink NOW
	pPrev = _Hook->m_pPrev;
	pNext = _Hook->m_pNext;
	if (pPrev)
	{
		pPrev->m_pNext = pNext;
	}
	else
	{
		_ThisPointer->m_pFunctionHooksHead = pNext;
	}

	if (pNext)
	{
		pNext->m_pPrev = pPrev;
	}
	else
	{
		_ThisPointer->m_pFunctionHooksTail = pPrev;
	}

	free(_Hook->m_puchOriginalBytesAtFunctionPrologue);
	free(_Hook);
}

/*
================
GSP_RH_FindHookByName
================
*/
struct ReversibleFunctionHook_c* GSP_RH_FindHookByName( struct ReversibleHooks_c* _ThisPointer, const char* _FunctionName )
{
	struct ReversibleFunctionHook_c* pCurrent;

	for (pCurrent = _ThisPointer->m_pFunctionHooksHead; pCurrent != NULL; pCurrent = pCurrent->m_pNext)
	{
		if (!strcmp(pCurrent->m_pszFunctionName, _FunctionName))
			return pCurrent;
	}

	return NULL;
}

/*
================
GSP_RH_EnableHook
================
*/
void GSP_RH_EnableHook( struct ReversibleHooks_c* _ThisPointer, struct ReversibleFunctionHook_c* _Hook )
{
	CPatch::PlaceJump(_Hook->m_pPrecomputedOriginalAddress, _Hook->m_pfnHooked, _Hook->m_puchOriginalBytesAtFunctionPrologue);
}

/*
================
GSP_RH_DisableHook
================
*/
void GSP_RH_DisableHook( struct ReversibleHooks_c* _ThisPointer, struct ReversibleFunctionHook_c* _Hook )
{
	CPatch::Patch(_Hook->m_pPrecomputedOriginalAddress, _Hook->m_puchOriginalBytesAtFunctionPrologue, 5);
}

/*
================
GSP_RH_InstallHookByAddress

(not a part of gspn_win32_8684)
================
*/
struct ReversibleFunctionHook_c* GSP_RH_InstallHookByAddress( struct ReversibleHooks_c* _ThisPointer, void* _Address, const char* _FunctionName, void* _Hooked )
{
	ReversibleFunctionHook_c* pHook = (ReversibleFunctionHook_c*)malloc(sizeof(ReversibleFunctionHook_c));
	if (!pHook)
	{
		Sys_Error("%s: failure allocating memory needed for a hook (%d)\n", __func__, sizeof(ReversibleFunctionHook_c));

		return NULL;
	}

	memset(pHook, 0, sizeof(ReversibleFunctionHook_c));

	pHook->m_pszFunctionName = _FunctionName;
	pHook->m_pfnHooked = _Hooked;
	pHook->m_puchOriginalBytesAtFunctionPrologue = (unsigned char*)malloc(5); // Since we're making a jmp [eip+OFFSET] hook, which takes five bytes, we must copy only five bytes from the function prologue.
	if (!pHook->m_puchOriginalBytesAtFunctionPrologue)
	{
		Sys_Error("%s: failure allocating %d bytes\n", __func__, 5);

		return NULL;
	}

	pHook->m_pPrecomputedOriginalAddress = _Address;

	GSP_RH_EnableHook(_ThisPointer, pHook);

	if (!_ThisPointer->m_pFunctionHooksHead)
	{
		_ThisPointer->m_pFunctionHooksHead = _ThisPointer->m_pFunctionHooksTail = pHook;
	}
	else
	{
		_ThisPointer->m_pFunctionHooksTail->m_pNext = pHook;
		pHook->m_pPrev = _ThisPointer->m_pFunctionHooksTail;
		_ThisPointer->m_pFunctionHooksTail = pHook;
	}

	return pHook;
}

/*
================
GSP_RH_Shutdown

(not a part of gspn_win32_8684)
================
*/
void GSP_RH_Shutdown( struct ReversibleHooks_c* _ThisPointer )
{
	ReversibleFunctionHook_c* pCurrent = _ThisPointer->m_pFunctionHooksHead;
	ReversibleFunctionHook_c* pNext;

	while (pCurrent)
	{
		pNext = pCurrent->m_pNext;

		GSP_RH_DisableHook(_ThisPointer, pCurrent);
		GSP_RH_DestroyHook(_ThisPointer, pCurrent);

		pCurrent = pNext;
	}

	free(_ThisPointer->m_pVtbl);
	delete _ThisPointer;
}