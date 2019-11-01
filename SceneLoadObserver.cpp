#include <string>
#include "stdafx.h"
#include "SceneLoadObserver.h"
#include "MemoryUtils.h"
#include "Offsets.h"


tLoadScene SceneLoadObserver::o_load_scene = nullptr;
std::vector<tLoadSceneCallback> SceneLoadObserver::load_scene_callbacks;

SceneLoadObserver::SceneLoadObserver(){
	o_load_scene = *reinterpret_cast<decltype(&o_load_scene)>(GameOffsets::instance()->getZEntitySceneContext_LoadScene());
	printf("o_load_scene: 0x%I64x\n", (uintptr_t)o_load_scene);
	MemoryUtils::DetourVFTCall(GameOffsets::instance()->getZEntitySceneContext_LoadScene(), detour, (void**)&o_load_scene);
}

uint64_t __fastcall SceneLoadObserver::detour(void* this_, SSceneInitParameters* scene_init_params) {
	for(const auto& callback: load_scene_callbacks)
		callback(scene_init_params);

	return o_load_scene(this_, scene_init_params);
}

void SceneLoadObserver::registerSceneLoadCallback(const tLoadSceneCallback& callback) {
	load_scene_callbacks.push_back(callback);
}