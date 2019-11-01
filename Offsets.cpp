#include "stdafx.h"
#include "Offsets.h"

GameOffsets::GameVersion GameOffsets::getVersion() const {
	int timestamp = *reinterpret_cast<int*>(0x1400001C0);
	if (timestamp == 0x5D9DEA53)
		return GameVersion::DX12;
	else if (timestamp == 0x5D9DEA3E)
		return GameVersion::DX11;
	else
		return GameVersion::UNK;
}

//Check IDA databases for notes about how to update offsets. 
//Current version: 2.71.0
GameOffsets::GameOffsets() {
	switch (getVersion()) {
	case GameVersion::DX12:
		offsets.pPushItem = reinterpret_cast<void*>(0x140C24540);
		offsets.pPushNPCInventoryDetour = reinterpret_cast<void*>(0x140C24AC0);
		offsets.pPushWorldInventoryDetour = reinterpret_cast<void*>(0x140C24471);
		offsets.pPushHeroInventoryDetour = reinterpret_cast<void*>(0x1405D7257);
		offsets.pPushStashInventoryDetour = reinterpret_cast<void*>(0x1405903DA);
		offsets.pZEntitySceneContext_LoadScene = reinterpret_cast<void**>(0x1416AEE68); 
		break;
	case GameVersion::DX11:
		offsets.pPushItem = reinterpret_cast<void*>(0x140C249E0);
		offsets.pPushNPCInventoryDetour = reinterpret_cast<void*>(0x140C24F60);
		offsets.pPushWorldInventoryDetour = reinterpret_cast<void*>(0x140C24911);
		offsets.pPushHeroInventoryDetour = reinterpret_cast<void*>(0x1405D7937);
		offsets.pPushStashInventoryDetour = reinterpret_cast<void*>(0x140590ABA);
		offsets.pZEntitySceneContext_LoadScene = reinterpret_cast<void**>(0x141693D70); 
		break;
	default:
		MessageBoxA(NULL, "Randomizer doesn't support current game version", "", NULL);
		ExitProcess(0);
	}
}

const GameOffsets* GameOffsets::instance() {
	static GameOffsets instance;
	return &instance;
}

void* GameOffsets::getPushItem()const { return offsets.pPushItem; }
void* GameOffsets::getPushWorldInventoryDetour()const { return offsets.pPushWorldInventoryDetour; }
void* GameOffsets::getPushNPCInventoryDetour()const { return offsets.pPushNPCInventoryDetour; }
void* GameOffsets::getPushHeroInventoryDetour()const { return offsets.pPushHeroInventoryDetour; }
void* GameOffsets::getPushStashInventoryDetour()const { return offsets.pPushStashInventoryDetour; }
void** GameOffsets::getZEntitySceneContext_LoadScene()const { return offsets.pZEntitySceneContext_LoadScene; }