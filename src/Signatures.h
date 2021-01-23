#pragma once
#include <unordered_map>
#include <vector>

namespace Signatures {

// Locates a memory region based on a unique byte signature and an offset from the start of the signature.
struct Locator {
    std::vector<int16_t> signature;
    int32_t offset;
};

// Map of names locators
extern std::unordered_map<std::string, Locator> locators;

}; // namespace Signatures
