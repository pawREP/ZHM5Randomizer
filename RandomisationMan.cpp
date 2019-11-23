#include <filesystem>
#include "RandomisationMan.h"
#include "MemoryUtils.h"
#include "Offsets.h"
#include "SSceneInitParameters.h"
#include "Console.h"
#include "Config.h"

std::unique_ptr<Randomizer> RandomisationMan::world_inventory_randomizer = nullptr;
std::unique_ptr<Randomizer> RandomisationMan::npc_item_randomizer = nullptr;
std::unique_ptr<Randomizer> RandomisationMan::hero_inventory_randomizer = nullptr;
std::unique_ptr<Randomizer> RandomisationMan::stash_item_randomizer = nullptr;

RandomisationMan::RandomisationMan() {
	default_item_pool_repo = std::make_unique<DefaultItemPoolRepository>("..\\HITMAN2\\Retail\\DefaultItemPools.json");

	world_inventory_randomizer = std::make_unique<Randomizer>(new WorldInventoryRandomisation);
	npc_item_randomizer = std::make_unique<Randomizer>(new NPCItemRandomisation);
	hero_inventory_randomizer = std::make_unique<Randomizer>(new HeroInventoryRandomisation);
	stash_item_randomizer = std::make_unique<Randomizer>(new StashInventoryRandomisation);

	MemoryUtils::DetourCall((void*)GameOffsets::instance()->getPushWorldInventoryDetour(), reinterpret_cast<char*>(&pushWorldItem<&world_inventory_randomizer>));
	MemoryUtils::DetourCall((void*)GameOffsets::instance()->getPushNPCInventoryDetour(), reinterpret_cast<char*>(&pushWorldItem<&npc_item_randomizer>));
	MemoryUtils::DetourCall((void*)GameOffsets::instance()->getPushHeroInventoryDetour(), reinterpret_cast<char*>(&pushWorldItem<&hero_inventory_randomizer>));
	MemoryUtils::DetourCall((void*)GameOffsets::instance()->getPushStashInventoryDetour(), reinterpret_cast<char*>(&pushWorldItem<&stash_item_randomizer>));
}

void RandomisationMan::initializeRandomizers(const SSceneInitParameters* sip) {
	sip->print();

	Config::loadConfig();

	auto scenario = Scenario::from_SceneInitParams(*sip);

	Console::log("Loading Scenario: %s\n", scenario.string().c_str());

	auto default_pool = default_item_pool_repo->getDefaultPool(scenario);
	if (default_pool != nullptr) {
		world_inventory_randomizer->initialize(scenario, default_pool);
		npc_item_randomizer->initialize(scenario, default_pool);
		hero_inventory_randomizer->initialize(scenario, default_pool);
		stash_item_randomizer->initialize(scenario, default_pool);
	}
	else {
		world_inventory_randomizer->disable();
		npc_item_randomizer->disable();
		hero_inventory_randomizer->disable();
		stash_item_randomizer->disable();
	}
}