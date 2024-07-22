#include <windows.h>
#include <psapi.h>
#include <algorithm>
#include "detours.h"

extern "C" __declspec(dllexport) const char *DInput84L2Version = DINPUT8_PROJECT_VERSION;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (HMODULE modules[1024]; K32EnumProcessModules(GetCurrentProcess(), modules, sizeof(modules), &ul_reason_for_call))
        {
            for (DWORD i = 0; i < std::min<DWORD>(ul_reason_for_call / sizeof(HMODULE), ARRAYSIZE(modules)); ++i)
            {
                if (modules[i] != hModule && GetProcAddress(modules[i], "DInput84L2Version") != nullptr)
                {
                    return FALSE;
                }
            }
        }

        detours::Setup();

        break;
    case DLL_PROCESS_DETACH:
        if (lpReserved == nullptr)
        {
            detours::Remove();
        }

        break;
    }

    return TRUE;
}
