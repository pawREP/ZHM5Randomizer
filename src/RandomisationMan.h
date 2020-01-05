#pragma once
#include "Randomizer.h"
#include "Scenario.h"
#include "DefaultItemPoolRepository.h"
#include "Offsets.h"

using tPushItem = __int64(__fastcall*)(__int64*, const RepositoryID*, __int64, void*, __int64, __int64, __int64*, void*, char*, char);

class RandomisationMan
{
private:
	std::unique_ptr<DefaultItemPoolRepository> default_item_pool_repo;

	static std::unique_ptr<Randomizer> world_inventory_randomizer;
	static std::unique_ptr<Randomizer> npc_item_randomizer;
	static std::unique_ptr<Randomizer> hero_inventory_randomizer;
	static std::unique_ptr<Randomizer> stash_item_randomizer;

	//This function template is called by external game code
	//Don't touch the signature of this function. 
	template<std::unique_ptr<Randomizer>* rnd>
	static int __fastcall pushWorldItem(__int64* worldInventory, const RepositoryID* repoID, __int64 a3, void* a4, __int64 a5, __int64 a6, __int64* a7, void* a8, char* a9, char a10) {
		const RepositoryID* id = (*rnd)->randomize(repoID);
		return reinterpret_cast<tPushItem>(GameOffsets::instance()->getPushItem())(worldInventory, id, a3, a4, a5, a6, a7, a8, a9, a10);
	};

public:

	RandomisationMan();

	void initializeRandomizers(const SSceneInitParameters* scen);
};

