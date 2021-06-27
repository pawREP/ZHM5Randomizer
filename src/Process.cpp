#include "Process.h"
#include <Windows.h>

std::wstring Process::getModuleFilename(int moduleHandle) {
    wchar_t buf[MAX_PATH];
    auto path = GetModuleFileNameW((HMODULE)moduleHandle, buf, MAX_PATH);
    return std::wstring(buf);
}
