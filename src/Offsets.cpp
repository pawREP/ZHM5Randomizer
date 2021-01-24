#include "Offsets.h"
#include "SigScanner.h"
#include "Signatures.h"
#include <Windows.h>
#include <filesystem>

[[noreturn]] void offsetSearchFailed() {
    MessageBoxA(NULL, "Signature scanning failed. The current game version might not be supported.", "", NULL);
    ExitProcess(0);
}

void* getOffsetByName(const std::string& name) {
    SigScanner scanner;
    auto off = scanner.find(Signatures::locators[name].signature) + Signatures::locators[name].offset;
    if(off < 0)
        offsetSearchFailed();
    return reinterpret_cast<void*>(off);
}

// Check IDA databases for notes about how to update offsets.
// Current version: 2.72.0 Update 3
GameOffsets::GameOffsets() {
    switch(getVersion()) {
    case GameVersion::H2DX12:
        offsets.pPushItem = reinterpret_cast<void*>(0x140C24650);
        offsets.pPushNPCInventoryDetour = reinterpret_cast<void*>(0x140C24BD0);
        offsets.pPushWorldInventoryDetour = reinterpret_cast<void*>(0x140C24581);
        offsets.pPushHeroInventoryDetour = reinterpret_cast<void*>(0x1405D7217);
        offsets.pPushStashInventoryDetour = reinterpret_cast<void*>(0x14059039A);
        offsets.pZEntitySceneContext_LoadScene = reinterpret_cast<void**>(0x1416AEE68);

        break;
    case GameVersion::H2DX11:
        offsets.pPushItem = reinterpret_cast<void*>(0x140C24AF0);
        offsets.pPushNPCInventoryDetour = reinterpret_cast<void*>(0x140C25070);
        offsets.pPushWorldInventoryDetour = reinterpret_cast<void*>(0x140C24A21);
        offsets.pPushHeroInventoryDetour = reinterpret_cast<void*>(0x1405D78F7);
        offsets.pPushStashInventoryDetour = reinterpret_cast<void*>(0x140590A7A);
        offsets.pZEntitySceneContext_LoadScene = reinterpret_cast<void**>(0x141693D70);
        break;
    case GameVersion::H3DX12: {
        offsets.pPushItem = getOffsetByName("PushItem");
        offsets.pPushNPCInventoryDetour = getOffsetByName("PushNPCInventoryDetour");
        offsets.pPushWorldInventoryDetour = getOffsetByName("PushWorldInventoryDetour");
        offsets.pPushHeroInventoryDetour = getOffsetByName("PushHeroInventoryDetour");
        offsets.pPushStashInventoryDetour = getOffsetByName("PushStashInventoryDetour");

        // Scan for ZEntitySceneContext_LoadScene function and then for it's vtable entry.
        auto ZEntitySceneContext_LoadScene = getOffsetByName("ZEntitySceneContext_LoadScene");
        std::vector<int16_t> pattern(sizeof(void*));
        for(int i = 0; i < sizeof(void*); ++i)
            pattern[i] = reinterpret_cast<char*>(&ZEntitySceneContext_LoadScene)[i];
        SigScanner scanner;
        offsets.pZEntitySceneContext_LoadScene = reinterpret_cast<void**>(scanner.find(pattern));
        if(offsets.pZEntitySceneContext_LoadScene < 0)
            offsetSearchFailed();
        break;
    }
    default:
        MessageBoxA(NULL, "Randomizer doesn't support current game version", "", NULL);
        ExitProcess(0);
    }
}

const GameOffsets* GameOffsets::instance() {
    static GameOffsets instance;
    return &instance;
}

void* GameOffsets::getPushItem() const {
    return offsets.pPushItem;
}
void* GameOffsets::getPushWorldInventoryDetour() const {
    return offsets.pPushWorldInventoryDetour;
}
void* GameOffsets::getPushNPCInventoryDetour() const {
    return offsets.pPushNPCInventoryDetour;
}
void* GameOffsets::getPushHeroInventoryDetour() const {
    return offsets.pPushHeroInventoryDetour;
}
void* GameOffsets::getPushStashInventoryDetour() const {
    return offsets.pPushStashInventoryDetour;
}
void** GameOffsets::getZEntitySceneContext_LoadScene() const {
    return offsets.pZEntitySceneContext_LoadScene;
}