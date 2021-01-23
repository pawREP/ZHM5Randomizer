#include "SigScanner.h"
#include <Windows.h>
#include <winnt.h>

SigScanner::SigScanner(HMODULE mod, int sectionFilter) {
    void* image_base = GetModuleHandle(NULL);
    auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(image_base);
    auto nt_header = reinterpret_cast<IMAGE_NT_HEADERS*>((uintptr_t)image_base + dos_header->e_lfanew);

    USHORT wSections;
    PIMAGE_SECTION_HEADER pSectionHdr;
    pSectionHdr = IMAGE_FIRST_SECTION(nt_header);
    wSections = nt_header->FileHeader.NumberOfSections;
    for(int i = 0; i < wSections; i++) {
        if(sectionFilter == 0 || pSectionHdr[i].Characteristics & sectionFilter) {
            auto addr = reinterpret_cast<void*>(pSectionHdr[i].VirtualAddress + (uint64_t)image_base);
            auto size = pSectionHdr[i].Misc.VirtualSize;
            domain.push_back({ addr, size });
        }
    }
}

uintptr_t SigScanner::find(const std::vector<short>& pattern) const {
    // TODO: Might be worth to check the full domain in debug mode and warn if a pattern isn't unique.
    for(const auto& segment : domain) {
        unsigned char* begin = reinterpret_cast<unsigned char*>(segment.virtualAddr);
        const unsigned char* end = begin + segment.size - pattern.size() + 1;
        for(; begin < end; begin++) {
            if(matchPattern(begin, pattern)) {
                return reinterpret_cast<uintptr_t>(begin);
            }
        }
    }
}
