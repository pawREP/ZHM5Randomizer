#include "stdafx.h"
#include "SceneLoadObserver.h"
#include "MemoryUtils.h"
#include "Offsets.h"
#include <string>

tLoadScene SceneLoadObserver::o_load_scene = nullptr;
tLoadSceneCallback SceneLoadObserver::load_scene_callback = nullptr;

SceneLoadObserver::SceneLoadObserver(){
	//o_load_scene = reinterpret_cast<decltype(o_load_scene)>(GameOffsets::pZEntitySceneContext_LoadScene);
	MemoryUtils::DetourVFTCall(GameOffsets::pZEntitySceneContext_LoadScene, detour, (void**)&o_load_scene);
}

uint64_t __fastcall SceneLoadObserver::detour(void* this_, SSceneInitParameters* scene_init_params) {
	//scene_init_params->print(); //DEBUG print

	load_scene_callback(scene_init_params);

	auto f = (tLoadScene)0x1401BF350;//TODO: replace this hardcoded offset with address read from VFT.
	return f(this_, scene_init_params);
}

void SceneLoadObserver::registerSceneLoadCallback(const tLoadSceneCallback& callback) {
	load_scene_callback = callback;
}