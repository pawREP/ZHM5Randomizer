#pragma once

namespace GameOffsets {
	extern const void* pPushItem;
	extern const void* pPushWorldInventoryDetour;
	extern const void* pPushNPCInventoryDetour;
	extern const void* pPushHeroInventoryDetour;
	extern const void* pPushStashInventoryDetour;

	extern const void* pZEntityManager; 
	extern void** pZEntitySceneContext_LoadScene;
}
