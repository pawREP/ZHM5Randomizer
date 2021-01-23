#include "Signatures.h"

namespace Signatures {

std::unordered_map<std::string, Locator> locators = {
    //	48 89 5C 24 20 4C 89 44 24 18 55 57 41 55 41 56 41 57 48 8D 6C 24 F9 48 81 EC D0 00 00 00 4C 8B F9
    { "PushItem",
      { { 0x48, 0x89, 0x5C, 0x24, 0x20, 0x4C, 0x89, 0x44, 0x24, 0x18, 0x55,
          0x57, 0x41, 0x55, 0x41, 0x56, 0x41, 0x57, 0x48, 0x8D, 0x6C, 0x24,
          0xF9, 0x48, 0x81, 0xEC, 0xD0, 0x00, 0x00, 0x00, 0x4C, 0x8B, 0xF9 },
        0 } },

    //	E8 ?? ?? ?? ?? 48 8B 4C 24 70 8B F8 48 B8 00 00 00 00 00 00 00 80 48 85 C8
    { "PushNPCInventoryDetour",
      { { 0xE8, -1,   -1,   -1,   -1,   0x48, 0x8B, 0x4C, 0x24, 0x70, 0x8B, 0xF8, 0x48,
          0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x48, 0x85, 0xC8 },
        0 } },

    // E8 ?? ?? ?? ?? 48 8B 4D E7 8B F0
    { "PushWorldInventoryDetour", { { 0xE8, -1, -1, -1, -1, 0x48, 0x8B, 0x4D, 0xE7, 0x8B, 0xF0 }, 0 } },

    // E8 ?? ?? ?? ?? 3B 05 ?? ?? ?? ?? 8B D8 74 77
    { "PushHeroInventoryDetour",
      { { 0xE8, -1, -1, -1, -1, 0x3B, 0x05, -1, -1, -1, -1, 0x8B, 0xD8, 0x74, 0x77 }, 0 } },

    // E8 ?? ?? ?? ?? 48 8D 4C 24 70 89 44 24 50 8B D8 E8
    { "PushStashInventoryDetour",
      { { 0xE8, -1, -1, -1, -1, 0x48, 0x8D, 0x4C, 0x24, 0x70, 0x89, 0x44, 0x24, 0x50, 0x8B, 0xD8, 0xE8 }, 0 } },

    // 40 55 56 57 41 56 48 8D 6C 24 C1 48 81 EC E8 00 00 00 48 8B FA
    { "ZEntitySceneContext_LoadScene",
      { { 0x40, 0x55, 0x56, 0x57, 0x41, 0x56, 0x48, 0x8D, 0x6C, 0x24, 0xC1,
          0x48, 0x81, 0xEC, 0xE8, 0x00, 0x00, 0x00, 0x48, 0x8B, 0xFA },
        0 } },

};

}