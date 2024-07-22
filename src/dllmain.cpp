#include <windows.h>
#include "detours.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
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
