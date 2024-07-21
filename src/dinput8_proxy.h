#ifndef DINPUT8_PROXY_H
#define DINPUT8_PROXY_H
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <dinput.h>

HRESULT WINAPI ProxyDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter);
HRESULT WINAPI ProxyDllCanUnloadNow();
HRESULT WINAPI ProxyDllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv);
HRESULT WINAPI ProxyDllRegisterServer();
HRESULT WINAPI ProxyDllUnregisterServer();
LPCDIDATAFORMAT WINAPI ProxyGetdfDIJoystick();

#endif // DINPUT8_PROXY_H
