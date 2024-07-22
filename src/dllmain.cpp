#include <windows.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <cwctype>
#include <MinHook.h>

__declspec(noinline) LPVOID __cdecl GetReturnAddress()
{
    __asm
    {
       MOV EAX, DWORD PTR SS:[EBP + 4]
    }
}

inline BOOL IsAddressInModule(LPVOID address, const char *moduleName)
{
    MEMORY_BASIC_INFORMATION mbi;
    char module[MAX_PATH];

    if (VirtualQuery(address, &mbi, sizeof(mbi)) &&
        GetModuleFileNameA((HMODULE)mbi.AllocationBase, module, sizeof(module)) != 0)
    {
        std::string modulePath(module);
        size_t pos = modulePath.find_last_of("\\/");
        std::string fileName = (pos != std::string::npos) ? modulePath.substr(pos + 1) : modulePath;
        return _stricmp(fileName.c_str(), moduleName) == 0;
    }

    return FALSE;
}

inline bool Contains(const std::wstring &str, const std::wstring &substr)
{
    return std::search(
               str.begin(), str.end(),
               substr.begin(), substr.end(),
               [](wchar_t a, wchar_t b)
               {
                   return std::towlower(a) == std::towlower(b);
               }) != str.end();
}

BOOL(WINAPI *OriginalSetDeviceGammaRamp)
(HDC, LPVOID);
BOOL WINAPI DetourSetDeviceGammaRamp(HDC hDC, LPVOID lpRamp)
{
    LPVOID returnAddress = GetReturnAddress();
    return IsAddressInModule(returnAddress, "d3ddrv.dll") || OriginalSetDeviceGammaRamp(hDC, lpRamp);
}

int(WINAPI *OriginalMessageBoxW)(HWND, LPCWSTR, LPCWSTR, UINT);
int WINAPI DetourMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
    return Contains(lpText, L"agp is deactivated") ? IDOK : OriginalMessageBoxW(hWnd, lpText, lpCaption, uType);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (MH_Initialize() != MH_OK)
        {
            break;
        }

        if (MH_CreateHookApi(L"gdi32.dll", "SetDeviceGammaRamp", DetourSetDeviceGammaRamp, (LPVOID *)&OriginalSetDeviceGammaRamp) != MH_OK)
        {
            MH_Uninitialize();
            break;
        }

        if (MH_CreateHookApi(L"user32.dll", "MessageBoxW", DetourMessageBoxW, (LPVOID *)&OriginalMessageBoxW) != MH_OK)
        {
            MH_RemoveHook(MH_ALL_HOOKS);
            MH_Uninitialize();
            break;
        }

        if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
        {
            MH_RemoveHook(MH_ALL_HOOKS);
            MH_Uninitialize();
            break;
        }

        break;
    case DLL_PROCESS_DETACH:
        if (lpReserved == nullptr)
        {
            MH_RemoveHook(MH_ALL_HOOKS);
            MH_Uninitialize();
        }
        break;
    }

    return TRUE;
}
