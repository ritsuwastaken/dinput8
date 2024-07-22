#include "version_proxy.h"

typedef BOOL(WINAPI *GetFileVersionInfoA_t)(LPCSTR, DWORD, DWORD, LPVOID);
typedef int(WINAPI *GetFileVersionInfoByHandle_t)(int, int, int, int);
typedef BOOL(WINAPI *GetFileVersionInfoExA_t)(DWORD, LPCSTR, DWORD, DWORD, LPVOID);
typedef BOOL(WINAPI *GetFileVersionInfoExW_t)(DWORD, LPCWSTR, DWORD, DWORD, LPVOID);
typedef DWORD(WINAPI *GetFileVersionInfoSizeA_t)(LPCSTR, LPDWORD);
typedef DWORD(WINAPI *GetFileVersionInfoSizeExA_t)(DWORD, LPCSTR, LPDWORD);
typedef DWORD(WINAPI *GetFileVersionInfoSizeExW_t)(DWORD, LPCWSTR, LPDWORD);
typedef DWORD(WINAPI *GetFileVersionInfoSizeW_t)(LPCWSTR, LPDWORD);
typedef BOOL(WINAPI *GetFileVersionInfoW_t)(LPCWSTR, DWORD, DWORD, LPVOID);
typedef DWORD(WINAPI *VerFindFileA_t)(DWORD, LPCSTR, LPCSTR, LPCSTR, LPSTR, PUINT, LPSTR, PUINT);
typedef DWORD(WINAPI *VerFindFileW_t)(DWORD, LPCWSTR, LPCWSTR, LPCWSTR, LPWSTR, PUINT, LPWSTR, PUINT);
typedef DWORD(WINAPI *VerInstallFileA_t)(DWORD, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPSTR, PUINT);
typedef DWORD(WINAPI *VerInstallFileW_t)(DWORD, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, LPWSTR, PUINT);
typedef DWORD(WINAPI *VerLanguageNameA_t)(DWORD, LPSTR, DWORD);
typedef DWORD(WINAPI *VerLanguageNameW_t)(DWORD, LPWSTR, DWORD);
typedef BOOL(WINAPI *VerQueryValueA_t)(LPCVOID, LPCSTR, LPVOID *, PUINT);
typedef BOOL(WINAPI *VerQueryValueW_t)(LPCVOID, LPCWSTR, LPVOID *, PUINT);

GetFileVersionInfoA_t real_GetFileVersionInfoA = nullptr;
GetFileVersionInfoByHandle_t real_GetFileVersionInfoByHandle = nullptr;
GetFileVersionInfoExA_t real_GetFileVersionInfoExA = nullptr;
GetFileVersionInfoExW_t real_GetFileVersionInfoExW = nullptr;
GetFileVersionInfoSizeA_t real_GetFileVersionInfoSizeA = nullptr;
GetFileVersionInfoSizeExA_t real_GetFileVersionInfoSizeExA = nullptr;
GetFileVersionInfoSizeExW_t real_GetFileVersionInfoSizeExW = nullptr;
GetFileVersionInfoSizeW_t real_GetFileVersionInfoSizeW = nullptr;
GetFileVersionInfoW_t real_GetFileVersionInfoW = nullptr;
VerFindFileA_t real_VerFindFileA = nullptr;
VerFindFileW_t real_VerFindFileW = nullptr;
VerInstallFileA_t real_VerInstallFileA = nullptr;
VerInstallFileW_t real_VerInstallFileW = nullptr;
VerLanguageNameA_t real_VerLanguageNameA = nullptr;
VerLanguageNameW_t real_VerLanguageNameW = nullptr;
VerQueryValueA_t real_VerQueryValueA = nullptr;
VerQueryValueW_t real_VerQueryValueW = nullptr;

void LoadVersion()
{
    static HMODULE hVersion = nullptr;
    if (!hVersion)
    {
        char systemPath[MAX_PATH];
        GetSystemDirectory(systemPath, MAX_PATH);
        strcat(systemPath, "\\version.dll");
        hVersion = LoadLibrary(systemPath);
        if (hVersion)
        {
            real_GetFileVersionInfoA = (GetFileVersionInfoA_t)GetProcAddress(hVersion, "GetFileVersionInfoA");
            real_GetFileVersionInfoByHandle = (GetFileVersionInfoByHandle_t)GetProcAddress(hVersion, "GetFileVersionInfoByHandle");
            real_GetFileVersionInfoExA = (GetFileVersionInfoExA_t)GetProcAddress(hVersion, "GetFileVersionInfoExA");
            real_GetFileVersionInfoExW = (GetFileVersionInfoExW_t)GetProcAddress(hVersion, "GetFileVersionInfoExW");
            real_GetFileVersionInfoSizeA = (GetFileVersionInfoSizeA_t)GetProcAddress(hVersion, "GetFileVersionInfoSizeA");
            real_GetFileVersionInfoSizeExA = (GetFileVersionInfoSizeExA_t)GetProcAddress(hVersion, "GetFileVersionInfoSizeExA");
            real_GetFileVersionInfoSizeExW = (GetFileVersionInfoSizeExW_t)GetProcAddress(hVersion, "GetFileVersionInfoSizeExW");
            real_GetFileVersionInfoSizeW = (GetFileVersionInfoSizeW_t)GetProcAddress(hVersion, "GetFileVersionInfoSizeW");
            real_GetFileVersionInfoW = (GetFileVersionInfoW_t)GetProcAddress(hVersion, "GetFileVersionInfoW");
            real_VerFindFileA = (VerFindFileA_t)GetProcAddress(hVersion, "VerFindFileA");
            real_VerFindFileW = (VerFindFileW_t)GetProcAddress(hVersion, "VerFindFileW");
            real_VerInstallFileA = (VerInstallFileA_t)GetProcAddress(hVersion, "VerInstallFileA");
            real_VerInstallFileW = (VerInstallFileW_t)GetProcAddress(hVersion, "VerInstallFileW");
            real_VerLanguageNameA = (VerLanguageNameA_t)GetProcAddress(hVersion, "VerLanguageNameA");
            real_VerLanguageNameW = (VerLanguageNameW_t)GetProcAddress(hVersion, "VerLanguageNameW");
            real_VerQueryValueA = (VerQueryValueA_t)GetProcAddress(hVersion, "VerQueryValueA");
            real_VerQueryValueW = (VerQueryValueW_t)GetProcAddress(hVersion, "VerQueryValueW");
        }
    }
}

BOOL WINAPI ProxyGetFileVersionInfoA(LPCSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
    LoadVersion();
    return real_GetFileVersionInfoA(lptstrFilename, dwHandle, dwLen, lpData);
}

int WINAPI ProxyGetFileVersionInfoByHandle(int a1, int a2, int a3, int a4)
{
    LoadVersion();
    return real_GetFileVersionInfoByHandle(a1, a2, a3, a4);
}

BOOL WINAPI ProxyGetFileVersionInfoExA(DWORD dwFlags, LPCSTR lpwstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
    LoadVersion();
    return real_GetFileVersionInfoExA(dwFlags, lpwstrFilename, dwHandle, dwLen, lpData);
}

BOOL WINAPI ProxyGetFileVersionInfoExW(DWORD dwFlags, LPCWSTR lpwstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
    LoadVersion();
    return real_GetFileVersionInfoExW(dwFlags, lpwstrFilename, dwHandle, dwLen, lpData);
}

DWORD WINAPI ProxyGetFileVersionInfoSizeA(LPCSTR lptstrFilename, LPDWORD lpdwHandle)
{
    LoadVersion();
    return real_GetFileVersionInfoSizeA(lptstrFilename, lpdwHandle);
}

DWORD WINAPI ProxyGetFileVersionInfoSizeExA(DWORD dwFlags, LPCSTR lpwstrFilename, LPDWORD lpdwHandle)
{
    LoadVersion();
    return real_GetFileVersionInfoSizeExA(dwFlags, lpwstrFilename, lpdwHandle);
}

DWORD WINAPI ProxyGetFileVersionInfoSizeExW(DWORD dwFlags, LPCWSTR lpwstrFilename, LPDWORD lpdwHandle)
{
    LoadVersion();
    return real_GetFileVersionInfoSizeExW(dwFlags, lpwstrFilename, lpdwHandle);
}

DWORD WINAPI ProxyGetFileVersionInfoSizeW(LPCWSTR lptstrFilename, LPDWORD lpdwHandle)
{
    LoadVersion();
    return real_GetFileVersionInfoSizeW(lptstrFilename, lpdwHandle);
}

BOOL WINAPI ProxyGetFileVersionInfoW(LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
    LoadVersion();
    return real_GetFileVersionInfoW(lptstrFilename, dwHandle, dwLen, lpData);
}

DWORD WINAPI ProxyVerFindFileA(DWORD uFlags, LPCSTR szFileName, LPCSTR szWinDir, LPCSTR szAppDir, LPSTR szCurDir, PUINT puCurDirLen, LPSTR szDestDir, PUINT puDestDirLen)
{
    LoadVersion();
    return real_VerFindFileA(uFlags, szFileName, szWinDir, szAppDir, szCurDir, puCurDirLen, szDestDir, puDestDirLen);
}

DWORD WINAPI ProxyVerFindFileW(DWORD uFlags, LPCWSTR szFileName, LPCWSTR szWinDir, LPCWSTR szAppDir, LPWSTR szCurDir, PUINT puCurDirLen, LPWSTR szDestDir, PUINT puDestDirLen)
{
    LoadVersion();
    return real_VerFindFileW(uFlags, szFileName, szWinDir, szAppDir, szCurDir, puCurDirLen, szDestDir, puDestDirLen);
}

DWORD WINAPI ProxyVerInstallFileA(DWORD uFlags, LPCSTR szSrcFileName, LPCSTR szDestFileName, LPCSTR szSrcDir, LPCSTR szDestDir, LPCSTR szCurDir, LPSTR szTmpFile, PUINT puTmpFileLen)
{
    LoadVersion();
    return real_VerInstallFileA(uFlags, szSrcFileName, szDestFileName, szSrcDir, szDestDir, szCurDir, szTmpFile, puTmpFileLen);
}

DWORD WINAPI ProxyVerInstallFileW(DWORD uFlags, LPCWSTR szSrcFileName, LPCWSTR szDestFileName, LPCWSTR szSrcDir, LPCWSTR szDestDir, LPCWSTR szCurDir, LPWSTR szTmpFile, PUINT puTmpFileLen)
{
    LoadVersion();
    return real_VerInstallFileW(uFlags, szSrcFileName, szDestFileName, szSrcDir, szDestDir, szCurDir, szTmpFile, puTmpFileLen);
}

DWORD WINAPI ProxyVerLanguageNameA(DWORD wLang, LPSTR szLang, DWORD nSize)
{
    LoadVersion();
    return real_VerLanguageNameA(wLang, szLang, nSize);
}

DWORD WINAPI ProxyVerLanguageNameW(DWORD wLang, LPWSTR szLang, DWORD nSize)
{
    LoadVersion();
    return real_VerLanguageNameW(wLang, szLang, nSize);
}

BOOL WINAPI ProxyVerQueryValueA(LPCVOID pBlock, LPCSTR lpSubBlock, LPVOID *lplpBuffer, PUINT puLen)
{
    LoadVersion();
    return real_VerQueryValueA(pBlock, lpSubBlock, lplpBuffer, puLen);
}

BOOL WINAPI ProxyVerQueryValueW(LPCVOID pBlock, LPCWSTR lpSubBlock, LPVOID *lplpBuffer, PUINT puLen)
{
    LoadVersion();
    return real_VerQueryValueW(pBlock, lpSubBlock, lplpBuffer, puLen);
}
