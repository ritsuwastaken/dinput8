#ifndef DETOURS_H
#define DETOURS_H

namespace detours
{
    void Queue(LPCWSTR lpLibFileName, LPCSTR lpProcName, LPVOID lpDetour, LPVOID *lpOriginal);
    void ApplyQueued();
    void RemoveAll();
}

#endif // DETOURS_H
