// dllmain.cpp -- Defines the entry point for the DLL application.

#include "StdAfx.h"
//#include "gsp.hpp"

int GAME_EXPORT Initialize( cl_enginefunc_t* pEnginefuncs, int iVersion );

PVOID g_pThisHandle;
cl_enginefunc_t g_EngineFuncs;
BOOLEAN g_fHasHookedClientDll;

RH_Hook(LoadLibraryA);
RH_Hook(Initialize);

void CMD_BXT_Append()
{
	if (g_EngineFuncs.Cmd_Argc() < 2)
    {
        g_EngineFuncs.Con_Printf("Usage: %s <command> - Append a command into the beginning of command queue\n Similar to how \"special\" appends \"_special\"\n", g_EngineFuncs.Cmd_Argv(0));

		return;
	}

    g_EngineFuncs.pfnClientCmd(g_EngineFuncs.Cmd_Argv(1));
}

int GAME_EXPORT Initialize( cl_enginefunc_t* pEnginefuncs, int iVersion )
{
    int iRetVal;

    memcpy(&g_EngineFuncs, pEnginefuncs, sizeof(cl_enginefunc_t));
    g_EngineFuncs.pfnAddCommand("append", CMD_BXT_Append);

    RH_ScopedCallOriginalRetVal(Initialize, iRetVal, pEnginefuncs, iVersion);

    return iRetVal;
}

HMODULE WINAPI HOOKED_LoadLibraryA( _In_ LPCSTR lpLibFileName )
{
    HMODULE hRetVal;
    RH_ScopedCallOriginalRetVal(LoadLibraryA, hRetVal, lpLibFileName);

    if (strstr(lpLibFileName, "cl_dlls\\client.dll"))
    {
        if (!g_fHasHookedClientDll)
        {
			FARPROC pfnInitialize = GetProcAddress(hRetVal, "Initialize");

			RH_ScopedInstallByAddress(Initialize, pfnInitialize, Initialize);

            g_fHasHookedClientDll = TRUE;
        }
    }

    return hRetVal;
}

EXTERN_C VOID CDECL GAME_EXPORT InitializeASI( void )
{
#ifdef _DEBUG
    FILE* pfInput;
    FILE* pfOutput;
    
    AllocConsole();
	freopen_s(&pfInput, "conin$", "rb", stdin);
	freopen_s(&pfOutput, "conout$", "w+", stdout);
#endif //_DEBUG
    RH_Initialise();
    RH_ScopedInstallWinAPI(LoadLibraryA, HOOKED_LoadLibraryA);
}

EXTERN_C VOID CDECL GAME_EXPORT ASIShutdown( void )
{
    RH_Shutdown();
}

DWORD WINAPI DllMain( _In_ void* _DllHandle,
                       _In_ unsigned long _Reason,
                       _In_opt_ void* _Unused
                     )
{
    switch (_Reason)
    {
    case DLL_PROCESS_ATTACH:
    {
        g_pThisHandle = _DllHandle;
    }
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

