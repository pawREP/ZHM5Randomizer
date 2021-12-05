#include "Version.h"
#include "Pe.h"
#include <Windows.h>

GameVersion getVersion() {
    auto timestamp = PE::getTimestamp();
    if(timestamp == 0x6157119F)
        return GameVersion::H3DX12;
    return GameVersion::UNK;
}
