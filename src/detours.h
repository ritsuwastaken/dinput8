#ifndef DETOURS_H
#define DETOURS_H

namespace detours
{
    void Create(LPCWSTR lpLibFileName, LPCSTR lpProcName, LPVOID lpDetour, LPVOID *lpOriginal);
    void RemoveAll();
    void ApplyAll();
}

#endif // DETOURS_H
