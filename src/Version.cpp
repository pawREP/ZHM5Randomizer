#include "Version.h"
#include "Pe.h"
#include <Windows.h>

GameVersion getVersion() {
    auto timestamp = PE::getTimestamp();
    if(timestamp == 0x60D1D7D0)
        return GameVersion::H3DX12;
    return GameVersion::UNK;
}
