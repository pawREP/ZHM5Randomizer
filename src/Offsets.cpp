#include <Windows.h>
#include "Offsets.h"

GameOffsets::GameVersion GameOffsets::getVersion() const {
	void* image_base = (void*)0x140000000;
	auto dos_header = *reinterpret_cast<IMAGE_DOS_HEADER*>(image_base);
	auto nt_header = *reinterpret_cast<IMAGE_NT_HEADERS*>((uintptr_t)image_base + dos_header.e_lfanew);
	int timestamp = nt_header.FileHeader.TimeDateStamp;
	if (timestamp == 0x5DD542FA)
		return GameVersion::DX12;
	else if (timestamp == 0x5DD54263)
		return GameVersion::DX11;
	else
		return GameVersion::UNK;
}

//Check IDA databases for notes about how to update offsets. 
//Current version: 2.72.0
GameOffsets::GameOffsets() {
	switch (getVersion()) {
	case GameVersion::DX12:
		offsets.pPushItem = reinterpret_cast<void*>(0x140C24650);
		offsets.pPushNPCInventoryDetour = reinterpret_cast<void*>(0x140C24BD0);
		offsets.pPushWorldInventoryDetour = reinterpret_cast<void*>(0x140C24581);
		offsets.pPushHeroInventoryDetour = reinterpret_cast<void*>(0x1405D7217);
		offsets.pPushStashInventoryDetour = reinterpret_cast<void*>(0x14059039A);
		offsets.pZEntitySceneContext_LoadScene = reinterpret_cast<void**>(0x1416AEE68);

		break;
	case GameVersion::DX11:
		offsets.pPushItem = reinterpret_cast<void*>(0x140C24AF0);
		offsets.pPushNPCInventoryDetour = reinterpret_cast<void*>(0x140C25070);
		offsets.pPushWorldInventoryDetour = reinterpret_cast<void*>(0x140C24A21);
		offsets.pPushHeroInventoryDetour = reinterpret_cast<void*>(0x1405D78F7);
		offsets.pPushStashInventoryDetour = reinterpret_cast<void*>(0x140590A7A);
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