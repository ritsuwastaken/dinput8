#include <windows.h>
#include <MinHook.h>
#include "detours.h"
#include "utils.h"

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

void detours::Setup()
{
    if (MH_Initialize() != MH_OK)
    {
        return;
    }

    if (MH_CreateHookApi(L"gdi32.dll", "SetDeviceGammaRamp", DetourSetDeviceGammaRamp, (LPVOID *)&OriginalSetDeviceGammaRamp) != MH_OK)
    {
        MH_Uninitialize();
        return;
    }

    if (MH_CreateHookApi(L"user32.dll", "MessageBoxW", DetourMessageBoxW, (LPVOID *)&OriginalMessageBoxW) != MH_OK)
    {
        detours::Remove();
        return;
    }

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
    {
        detours::Remove();
        return;
    }
}

void detours::Remove()
{
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}
