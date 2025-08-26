/**
 * Copyright - xWhitey, 2025.
 * patch.cpp - CPatch
 *
 * goldsrc-paper-next (a total reboot of goldsrc-paper) source file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#include "StdAfx.h"

/*
================
CPatch::Patch

Replace (_Size) number bytes of data at _Address to the data passed as _Data argument
================
*/
void CPatch::Patch( void* _Address, void* _Data, size_t _Size )
{
	DWORD dwProtect[2] = {};
	VirtualProtect(_Address, _Size, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
	memcpy(_Address, _Data, _Size);
	VirtualProtect(_Address, _Size, dwProtect[0], &dwProtect[1]); //I'm afraid VirtualProtect can choke at some point later if lpflOldProtect is the same as flNewProtect, so better safe than sorry.
}

/*
================
CPatch::PlaceJump

Place a jmp [eip+OFFSET] at _Address
================
*/
void CPatch::PlaceJump( void* _Address, void* _Target, void* _OriginalBytes )
{
	if (_OriginalBytes)
	{
		memcpy(_OriginalBytes, _Address, 5);
	}
	unsigned char rguchShellcode[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
	*(size_t*)&rguchShellcode[1] = (size_t)_Target - (((size_t)_Address) + 5); // calculate a relative to EIP address
	Patch(_Address, rguchShellcode, 5);
}