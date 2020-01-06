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

	MemoryUtils::DetourCall(GameOffsets::instance()->getPushWorldInventoryDetour(), reinterpret_cast<const void*>(&pushWorldItem<&world_inventory_randomizer>));
	MemoryUtils::DetourCall(GameOffsets::instance()->getPushNPCInventoryDetour(), reinterpret_cast<const void*>(&pushWorldItem<&npc_item_randomizer>));
	MemoryUtils::DetourCall(GameOffsets::instance()->getPushHeroInventoryDetour(), reinterpret_cast<const void*>(&pushWorldItem<&hero_inventory_randomizer>));
	MemoryUtils::DetourCall(GameOffsets::instance()->getPushStashInventoryDetour(), reinterpret_cast<const void*>(&pushWorldItem<&stash_item_randomizer>));
}

void RandomisationMan::registerRandomizer(RandomizerSlot slot, std::unique_ptr<Randomizer> rng) {
	switch (slot) {
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