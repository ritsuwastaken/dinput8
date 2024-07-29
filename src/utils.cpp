#include <algorithm>
#include <cwctype>
#include "utils.h"

__declspec(noinline) LPVOID __cdecl utils::GetReturnAddress()
{
    __asm
    {
            MOV EAX, DWORD PTR SS:[EBP + 4]
    }
}

bool utils::Contains(const std::wstring &str, const std::wstring &substr)
{
    return std::search(
               str.begin(), str.end(),
               substr.begin(), substr.end(),
               [](wchar_t a, wchar_t b)
               {
                   return std::towlower(a) == std::towlower(b);
               }) != str.end();
}

BOOL utils::IsAddressInModule(LPVOID address, const char *moduleName)
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