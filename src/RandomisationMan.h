#pragma once
#include "DefaultItemPoolRepository.h"
#include "Offsets.h"
#include "Randomizer.h"
#include "Scenario.h"

using pushItem0_t = __int64(
__fastcall*)(__int64*, const RepositoryID*, __int64, void*, __int64, __int64, __int64*, void*, char*, char);
using pushItem1_t = __int64(__fastcall*)(signed __int64*, const RepositoryID*, void*, __int64, __int64, __int64*, __int64*);

enum class RandomizerSlot { WorldInventory, NPCInventory, HeroInventory, StashInventory };

class RandomisationMan {
private:
    std::unique_ptr<DefaultItemPoolRepository> default_item_pool_repo;

    static std::unique_ptr<Randomizer> world_inventory_randomizer;
    static std::unique_ptr<Randomizer> npc_item_randomizer;
    static std::unique_ptr<Randomizer> hero_inventory_randomizer;
    static std::unique_ptr<Randomizer> stash_item_randomizer;

    // This function template is called by external game code
    // Don't touch the signature of this function.
    template <std::unique_ptr<Randomizer>* rnd>
    static __int64 __fastcall pushItem0Detour(__int64* worldInventory,
                                              const RepositoryID* repoId,
                                              __int64 a3,
                                              void* a4,
                                              __int64 a5,
                                              __int64 a6,
                                              __int64* a7,
                                              void* a8,
                                              char* a9,
                                              char a10) {
        const RepositoryID* id = (*rnd)->randomize(repoId);
        const auto push = reinterpret_cast<pushItem0_t>(GameOffsets::instance()->getPushItem0());
        return push(worldInventory, id, a3, a4, a5, a6, a7, a8, a9, a10);
    };

    // This function template is called by external game code
    // Don't touch the signature of this function.
    template <std::unique_ptr<Randomizer>* rnd>
    static __int64 __fastcall pushItem1Detour(signed __int64* a1,
                                              const RepositoryID* repoId,
                                              void* a3,
                                              __int64 a4,
                                              __int64 a5,
                                              __int64* a6,
                                              __int64* a7) {
        const RepositoryID* id = (*rnd)->randomize(repoId);
        const auto push = reinterpret_cast<pushItem1_t>(GameOffsets::instance()->getPushItem1());
        return push(a1, id, a3, a4, a5, a6, a7);
    }

    void configureRandomizerCollection();

public:
    RandomisationMan();

    void registerRandomizer(RandomizerSlot slot, std::unique_ptr<Randomizer> rng);
    void initializeRandomizers(const SSceneInitParameters* scen);
};
