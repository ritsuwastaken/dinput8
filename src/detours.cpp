#include <windows.h>
#include <vector>
#include <shared_mutex>
#include <MinHook.h>
#include "detours.h"
#include "utils.h"

struct ExportedSymbolDetour
{
    LPCWSTR pszModule;
    LPCSTR pszProcName;
    LPVOID pDetour;
    LPVOID *ppOriginal;
};

static std::vector<ExportedSymbolDetour> deferredDetours;
static std::shared_mutex detoursMutex;
static std::shared_mutex deferredDetoursMutex;

HMODULE(WINAPI *OriginalLoadLibraryA)
(LPCSTR);
HMODULE(WINAPI *OriginalLoadLibraryW)
(LPCWSTR);
HMODULE(WINAPI *OriginalLoadLibraryExA)
(LPCSTR, HANDLE, DWORD);
HMODULE(WINAPI *OriginalLoadLibraryExW)
(LPCWSTR, HANDLE, DWORD);

MH_STATUS QueueEnableHookApi(LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, LPVOID *ppOriginal)
{
    if (MH_STATUS status = MH_CreateHookApi(pszModule, pszProcName, pDetour, ppOriginal); status != MH_OK)
        return status;

    return MH_QueueEnableHook(ppOriginal);
}

void detours::ApplyQueued()
{
    MH_ApplyQueued();
}

void detours::RemoveAll()
{
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}

inline void LoadLibraryCallback(HMODULE hModule)
{
    wchar_t wLibFileName[MAX_PATH];
    if (hModule == NULL || GetModuleFileNameW(hModule, wLibFileName, MAX_PATH) == NULL)
        return;

    if (std::unique_lock<std::shared_mutex> lock(deferredDetoursMutex, std::try_to_lock); lock.owns_lock())
    {
        const auto it = std::remove_if(
            deferredDetours.begin(),
            deferredDetours.end(),
            [wLibFileName](const ExportedSymbolDetour &detour)
            {
                if (utils::Contains(wLibFileName, detour.pszModule))
                {
                    QueueEnableHookApi(detour.pszModule, detour.pszProcName, detour.pDetour, detour.ppOriginal);
                    return true;
                }

                return false;
            });
        deferredDetours.erase(it, deferredDetours.end());
        if (deferredDetours.empty())
        {
            MH_QueueDisableHook(OriginalLoadLibraryA);
            MH_QueueDisableHook(OriginalLoadLibraryW);
            MH_QueueDisableHook(OriginalLoadLibraryExA);
            MH_QueueDisableHook(OriginalLoadLibraryExW);
        }

        detours::ApplyQueued();
    }
}

HMODULE WINAPI DetourLoadLibraryA(LPCSTR lpLibFileName)
{
    HMODULE hModule = OriginalLoadLibraryA(lpLibFileName);
    LoadLibraryCallback(hModule);
    return hModule;
}

HMODULE WINAPI DetourLoadLibraryW(LPCWSTR lpLibFileName)
{
    HMODULE hModule = OriginalLoadLibraryW(lpLibFileName);
    LoadLibraryCallback(hModule);
    return hModule;
}

HMODULE WINAPI DetourLoadLibraryExA(LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
{
    HMODULE hModule = OriginalLoadLibraryExA(lpLibFileName, hFile, dwFlags);
    LoadLibraryCallback(hModule);
    return hModule;
}

HMODULE WINAPI DetourLoadLibraryExW(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
{
    HMODULE hModule = OriginalLoadLibraryExW(lpLibFileName, hFile, dwFlags);
    LoadLibraryCallback(hModule);
    return hModule;
}

void detours::Queue(LPCWSTR lpLibFileName, LPCSTR lpProcName, LPVOID lpDetour, LPVOID *lpOriginal)
{
    if (MH_STATUS status = MH_Initialize(); status != MH_OK && status != MH_ERROR_ALREADY_INITIALIZED)
        return;

    if (QueueEnableHookApi(lpLibFileName, lpProcName, lpDetour, lpOriginal) != MH_ERROR_MODULE_NOT_FOUND)
        return;

    QueueEnableHookApi(L"kernel32.dll", "LoadLibraryA", DetourLoadLibraryA, (LPVOID *)&OriginalLoadLibraryA);
    QueueEnableHookApi(L"kernel32.dll", "LoadLibraryExA", DetourLoadLibraryExA, (LPVOID *)&OriginalLoadLibraryExA);
    QueueEnableHookApi(L"kernel32.dll", "LoadLibraryExW", DetourLoadLibraryExW, (LPVOID *)&OriginalLoadLibraryExW);
    QueueEnableHookApi(L"kernel32.dll", "LoadLibraryW", DetourLoadLibraryW, (LPVOID *)&OriginalLoadLibraryW);
    detours::ApplyQueued();

    {
        const std::unique_lock<std::shared_mutex> lock(detoursMutex);
        deferredDetours.push_back({lpLibFileName, lpProcName, lpDetour, lpOriginal});
    }
}
