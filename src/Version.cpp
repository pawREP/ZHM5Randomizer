#include "Version.h"
#include <filesystem>
#include <windows.h>

GameVersion getVersion() {
    // Hitman 3
    TCHAR szExeFileName[MAX_PATH];
    GetModuleFileName(NULL, szExeFileName, MAX_PATH);
    std::filesystem::path path(szExeFileName);
    if(path.filename() == "HITMAN3.exe")
        return GameVersion::H3DX12;

    // Hitman 2
    void* image_base = GetModuleHandle(NULL);
    auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(image_base);
    auto nt_header = reinterpret_cast<IMAGE_NT_HEADERS*>((uintptr_t)image_base + dos_header->e_lfanew);
    int timestamp = nt_header->FileHeader.TimeDateStamp;
    if(timestamp == 0x5EE9D095 || timestamp == 0x5F8D56D3)
        return GameVersion::H2DX12; // Hitman 2 DX12
    else if(timestamp == 0x5EE9D065 || timestamp == 0x5F8D57CA)
        return GameVersion::H2DX11; // Hitman 2 DX11
    else
        return GameVersion::UNK;
}
