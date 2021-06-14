#include "Version.h"
#include <filesystem>
#include <windows.h>

GameVersion getVersion() {
    void* image_base = GetModuleHandle(NULL);
    auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(image_base);
    auto nt_header = reinterpret_cast<IMAGE_NT_HEADERS*>((uintptr_t)image_base + dos_header->e_lfanew);
    int timestamp = nt_header->FileHeader.TimeDateStamp;
    if(timestamp == 0x608B687F)
        return GameVersion::H3DX12;
    return GameVersion::UNK;
}
