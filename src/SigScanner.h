#pragma once
#include <Windows.h>
#include <cinttypes>
#include <vector>

// TODO: SigScanner could be made faster

class SigScanner {
public:
    // Contruct signature scanner for a loaded module. The search domain can be restricted by providing a section characteristic (IMAGE_SCN_CNT_CODE, ...)
    SigScanner(HMODULE mod = GetModuleHandle(NULL), int sectionFilter = 0);

    // Find first occurance of byte pattern. Returns absolute, virtual address.
    uintptr_t find(const std::vector<short>& pattern) const;

private:
    struct MemRange {
        void* virtualAddr;
        uint64_t size;
    };

    std::vector<MemRange> domain;

    inline bool matchPattern(unsigned char* mem, const std::vector<short>& pattern) const {
        for(const auto& c : pattern) {
            if(c < 0) {
                mem++;
                continue;
            }
            if(c != *mem++)
                return false;
        }
        return true;
    }
};
