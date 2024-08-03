#ifndef UTILS_H
#define UTILS_H
#include <windows.h>
#include <string>

namespace utils
{
    __declspec(noinline) LPVOID __cdecl GetReturnAddress();
    BOOL IsAddressInModule(LPVOID address, const char *moduleName);
    bool Contains(const std::wstring &str, const std::wstring &substr);
}

#endif // UTILS_H
