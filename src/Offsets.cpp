#include "Offsets.h"
#include "Pe.h"
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

GameOffsets::GameOffsets() {
    switch(getVersion()) {
    case GameVersion::H3DX12: {

        // TODO: Reintroduce signature scanning here. It was previously removed because of major
        // signature changed introduced with Hitman update 3.30. (Inlining, NPC/WorldItem spawn
        // merge). If the new layout turns out to be stable, sig scanning should be reintroduced.

        // Hitman 3 3.30 offsets
        offsets.pPushItem0 = reinterpret_cast<void*>(0x140D75060);
        offsets.pPushItem1 = reinterpret_cast<void*>(0x140D75650);
        offsets.pPushNPCInventoryDetour = reinterpret_cast<void*>(0x14015FF01);
        offsets.pPushWorldInventoryDetour = reinterpret_cast<void*>(0x140D6F68A);
        offsets.pPushHeroInventoryDetour = reinterpret_cast<void*>(0x14064C923);
        offsets.pPushStashInventoryDetour = reinterpret_cast<void*>(0x1403D8074);
        offsets.pZEntitySceneContext_LoadScene = reinterpret_cast<void**>(0x141D11D88);
    } break;
    case GameVersion::H2DX12:
    case GameVersion::H2DX11:
        // TODO: H2 Specific error message
    default:
        MessageBoxA(NULL,
                    std::format("Incompatible client version detected. Please update to the latest "
                                "client release.\n\nPE timestamp: {:X}",
                                PE::getTimestamp())
                    .c_str(),
                    "Incompatible Client Version", NULL);
        ExitProcess(0);
    }
}

const GameOffsets* GameOffsets::instance() {
    static GameOffsets instance;
    return &instance;
}

void* GameOffsets::getPushItem0() const {
    return offsets.pPushItem0;
}
void* GameOffsets::getPushItem1() const {
    return offsets.pPushItem1;
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