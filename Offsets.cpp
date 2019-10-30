#include "stdafx.h"
#include "Offsets.h"

//Check IDA databases for notes about how to update offsets. Based on 2.71.0
namespace GameOffsets {

	const void* pPushNPCInventoryDetour = reinterpret_cast<void*>(0x140C24AC0);
	const void* pPushItem = reinterpret_cast<void*>(0x140C24540);
	const void* pPushWorldInventoryDetour = reinterpret_cast<void*>(0x140C24471);
	const void* pPushHeroInventoryDetour = reinterpret_cast<void*>(0x1405D7257);
	const void* pPushStashInventoryDetour = reinterpret_cast<void*>(0x1405903DA);

	//const void* pSceneEntityContext = (void*)0x142C1EEE8;
	const void* pZEntityManager = reinterpret_cast<void*>(0x142C10AE0); //v2.71.0

	void** pZEntitySceneContext_LoadScene = reinterpret_cast<void**>(0x1416AEE68); //v2.71.0 //
}