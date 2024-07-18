#include "dinput8_proxy.h"

typedef HRESULT(WINAPI *DirectInput8Create_t)(HINSTANCE, DWORD, REFIID, LPVOID *, LPUNKNOWN);
typedef HRESULT(WINAPI *DllCanUnloadNow_t)();
typedef HRESULT(WINAPI *DllGetClassObject_t)(REFCLSID, REFIID, LPVOID *);
typedef HRESULT(WINAPI *DllRegisterServer_t)();
typedef HRESULT(WINAPI *DllUnregisterServer_t)();

DirectInput8Create_t real_DirectInput8Create = nullptr;
DllCanUnloadNow_t real_DllCanUnloadNow = nullptr;
DllGetClassObject_t real_DllGetClassObject = nullptr;
DllRegisterServer_t real_DllRegisterServer = nullptr;
DllUnregisterServer_t real_DllUnregisterServer = nullptr;

void LoadDInput8()
{
    static HMODULE hDInput8 = nullptr;
    if (!hDInput8)
    {
        char systemPath[MAX_PATH];
        GetSystemDirectory(systemPath, MAX_PATH);
        strcat(systemPath, "\\dinput8.dll");
        hDInput8 = LoadLibrary(systemPath);
        if (hDInput8)
        {
            real_DirectInput8Create = (DirectInput8Create_t)GetProcAddress(hDInput8, "DirectInput8Create");
            real_DllCanUnloadNow = (DllCanUnloadNow_t)GetProcAddress(hDInput8, "DllCanUnloadNow");
            real_DllGetClassObject = (DllGetClassObject_t)GetProcAddress(hDInput8, "DllGetClassObject");
            real_DllRegisterServer = (DllRegisterServer_t)GetProcAddress(hDInput8, "DllRegisterServer");
            real_DllUnregisterServer = (DllUnregisterServer_t)GetProcAddress(hDInput8, "DllUnregisterServer");
        }
    }
}

HRESULT WINAPI ProxyDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter)
{
    LoadDInput8();
    return real_DirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

HRESULT WINAPI ProxyDllCanUnloadNow()
{
    LoadDInput8();
    return real_DllCanUnloadNow();
}

HRESULT WINAPI ProxyDllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv)
{
    LoadDInput8();
    return real_DllGetClassObject(rclsid, riid, ppv);
}

HRESULT WINAPI ProxyDllRegisterServer()
{
    LoadDInput8();
    return real_DllRegisterServer();
}

HRESULT WINAPI ProxyDllUnregisterServer()
{
    LoadDInput8();
    return real_DllUnregisterServer();
}
