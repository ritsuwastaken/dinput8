#include <windows.h>
#include <psapi.h>
#include <algorithm>
#include "detours.h"
#include "utils.h"

extern "C" __declspec(dllexport) const char *DInput84L2Version = DINPUT8_PROJECT_VERSION;

BOOL(WINAPI *OriginalSetDeviceGammaRamp)
(HDC, LPVOID);
BOOL WINAPI DetourSetDeviceGammaRamp(HDC hDC, LPVOID lpRamp)
{
    LPVOID returnAddress = utils::GetReturnAddress();
    return utils::IsAddressInModule(returnAddress, "d3ddrv.dll") || OriginalSetDeviceGammaRamp(hDC, lpRamp);
}

int(WINAPI *OriginalMessageBoxW)(HWND, LPCWSTR, LPCWSTR, UINT);
int WINAPI DetourMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
    return utils::Contains(lpText, L"agp is deactivated") ? IDOK : OriginalMessageBoxW(hWnd, lpText, lpCaption, uType);
}

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

        detours::Create(L"user32.dll", "MessageBoxW", DetourMessageBoxW, (LPVOID *)&OriginalMessageBoxW);
        detours::Create(L"gdi32.dll", "SetDeviceGammaRamp", DetourSetDeviceGammaRamp, (LPVOID *)&OriginalSetDeviceGammaRamp);
        detours::ApplyAll();

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
