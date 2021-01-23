#include "RandomisationMan.h"
#include "Config.h"
#include "Console.h"
#include "MemoryUtils.h"
#include "Offsets.h"
#include "RNG.h"
#include "SSceneInitParameters.h"
#include <filesystem>

#ifdef DEFAULTPOOLEXPORT
#include "DefaultPoolExport.h"
#endif

std::unique_ptr<Randomizer> RandomisationMan::world_inventory_randomizer = nullptr;
std::unique_ptr<Randomizer> RandomisationMan::npc_item_randomizer = nullptr;
std::unique_ptr<Randomizer> RandomisationMan::hero_inventory_randomizer = nullptr;
std::unique_ptr<Randomizer> RandomisationMan::stash_item_randomizer = nullptr;

template <typename T>
RandomisationStrategy* createInstance() {
    return new T;
}

std::unordered_map<std::string, RandomisationStrategy* (*)()> worldRandomizers{
    { "NONE", &createInstance<IdentityRandomisation> },
    { "DEFAULT", &createInstance<WorldInventoryRandomisation> },
    { "OOPS_ALL_EXPLOSIVES", &createInstance<OopsAllExplosivesWorldInventoryRandomization> },
};

std::unordered_map<std::string, RandomisationStrategy* (*)()> npcRandomizers{
    { "NONE", &createInstance<IdentityRandomisation> },
    { "DEFAULT", &createInstance<NPCItemRandomisation> },
    { "HARD", &createInstance<UnrestrictedNPCRandomization> },
    { "SLEEPY", &createInstance<SleepyNPCRandomization> },
};

std::unordered_map<std::string, RandomisationStrategy* (*)()> heroRandomizers{
    { "NONE", &createInstance<IdentityRandomisation> },
    { "DEFAULT", &createInstance<HeroInventoryRandomisation> },
};

std::unordered_map<std::string, RandomisationStrategy* (*)()> stashRandomizers{
    { "NONE", &createInstance<IdentityRandomisation> },
    { "DEFAULT", &createInstance<StashInventoryRandomisation> },
};

void RandomisationMan::configureRandomizerCollection() {
    registerRandomizer(RandomizerSlot::WorldInventory,
                       std::make_unique<Randomizer>(worldRandomizers[Config::worldInventoryRandomizer]()));
    registerRandomizer(RandomizerSlot::NPCInventory,
                       std::make_unique<Randomizer>(npcRandomizers[Config::npcInventoryRandomizer]()));
    registerRandomizer(RandomizerSlot::HeroInventory,
                       std::make_unique<Randomizer>(heroRandomizers[Config::heroInventoryRandomizer]()));
    registerRandomizer(RandomizerSlot::StashInventory,
                       std::make_unique<Randomizer>(stashRandomizers[Config::stashInventoryRandomizer]()));
}

RandomisationMan::RandomisationMan() {
    default_item_pool_repo = std::make_unique<DefaultItemPoolRepository>(
    Config::base_directory + "\\Retail\\DefaultItemPools.json");

    world_inventory_randomizer = std::make_unique<Randomizer>(new IdentityRandomisation);
    npc_item_randomizer = std::make_unique<Randomizer>(new IdentityRandomisation);
    hero_inventory_randomizer = std::make_unique<Randomizer>(new IdentityRandomisation);
    stash_item_randomizer = std::make_unique<Randomizer>(new IdentityRandomisation);

    MemoryUtils::DetourCall(GameOffsets::instance()->getPushWorldInventoryDetour(),
                            reinterpret_cast<const void*>(&pushWorldItem<&world_inventory_randomizer>));
    MemoryUtils::DetourCall(GameOffsets::instance()->getPushNPCInventoryDetour(),
                            reinterpret_cast<const void*>(&pushWorldItem<&npc_item_randomizer>));
    MemoryUtils::DetourCall(GameOffsets::instance()->getPushHeroInventoryDetour(),
                            reinterpret_cast<const void*>(&pushWorldItem<&hero_inventory_randomizer>));
    MemoryUtils::DetourCall(GameOffsets::instance()->getPushStashInventoryDetour(),
                            reinterpret_cast<const void*>(&pushWorldItem<&stash_item_randomizer>));
}

void RandomisationMan::registerRandomizer(RandomizerSlot slot, std::unique_ptr<Randomizer> rng) {
    switch(slot) {
    case RandomizerSlot::WorldInventory:
        world_inventory_randomizer = std::move(rng);
        break;
    case RandomizerSlot::NPCInventory:
        npc_item_randomizer = std::move(rng);
        break;
    case RandomizerSlot::HeroInventory:
        hero_inventory_randomizer = std::move(rng);
        break;
    case RandomizerSlot::StashInventory:
        stash_item_randomizer = std::move(rng);
        break;
    }
}

void RandomisationMan::initializeRandomizers(const SSceneInitParameters* sip) {
    sip->print();

    configureRandomizerCollection();

    auto seed = Config::RNGSeed;
    if(seed == 0)
        seed = std::random_device{}();
    RNG::inst().seed(seed);

    // auto scenario = Scenario::from_SceneInitParams(*sip);
    auto scenario = std::hash<SSceneInitParameters>()(*sip);
#ifdef DEFAULTPOOLEXPORT
    DefaultPoolExport::loadScenario(scenario);
#endif
    // Console::log("Loading Scenario: %s\n", scenario.string().c_str());

    auto default_pool = default_item_pool_repo->getDefaultPool(scenario);

#ifdef DEFAULTPOOLEXPORT
    world_inventory_randomizer = std::make_unique<Randomizer>(new IdentityRandomisation);
    world_inventory_randomizer->initialize(scenario, default_pool);
    npc_item_randomizer->disable();
    hero_inventory_randomizer->disable();
    stash_item_randomizer->disable();
#else
    if(default_pool != nullptr) {
        world_inventory_randomizer->initialize(scenario, default_pool);
        npc_item_randomizer->initialize(scenario, default_pool);
        hero_inventory_randomizer->initialize(scenario, default_pool);
        stash_item_randomizer->initialize(scenario, default_pool);
    } else {
        world_inventory_randomizer->disable();
        npc_item_randomizer->disable();
        hero_inventory_randomizer->disable();
        stash_item_randomizer->disable();
    }
#endif
}