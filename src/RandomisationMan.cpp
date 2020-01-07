#include <filesystem>
#include "RandomisationMan.h"
#include "MemoryUtils.h"
#include "Offsets.h"
#include "SSceneInitParameters.h"
#include "Console.h"
#include "Config.h"
#include "RNG.h"

std::unique_ptr<Randomizer> RandomisationMan::world_inventory_randomizer = nullptr;
std::unique_ptr<Randomizer> RandomisationMan::npc_item_randomizer = nullptr;
std::unique_ptr<Randomizer> RandomisationMan::hero_inventory_randomizer = nullptr;
std::unique_ptr<Randomizer> RandomisationMan::stash_item_randomizer = nullptr;

void RandomisationMan::configureRandomizerCollection() {
	Console::log("configureRandomizerCollection called with : %s\n", Config::randomizationScenario.c_str());
	if(Config::randomizationScenario == "Default") {
		registerRandomizer(RandomizerSlot::WorldInventory, std::make_unique<Randomizer>(new WorldInventoryRandomisation));
		registerRandomizer(RandomizerSlot::NPCInventory, std::make_unique<Randomizer>(new NPCItemRandomisation));
		registerRandomizer(RandomizerSlot::HeroInventory, std::make_unique<Randomizer>(new HeroInventoryRandomisation));
		registerRandomizer(RandomizerSlot::StashInventory, std::make_unique<Randomizer>(new StashInventoryRandomisation));
		return;
	}

	if(Config::randomizationScenario == "Hard"){
		registerRandomizer(RandomizerSlot::WorldInventory, std::make_unique<Randomizer>(new WorldInventoryRandomisation));
		registerRandomizer(RandomizerSlot::NPCInventory, std::make_unique<Randomizer>(new UnrestrictedNPCRandomization));
		registerRandomizer(RandomizerSlot::HeroInventory, std::make_unique<Randomizer>(new HeroInventoryRandomisation));
		registerRandomizer(RandomizerSlot::StashInventory, std::make_unique<Randomizer>(new StashInventoryRandomisation));
		return;
	}
}

RandomisationMan::RandomisationMan() {
	default_item_pool_repo = std::make_unique<DefaultItemPoolRepository>("..\\HITMAN2\\Retail\\DefaultItemPools.json");

	world_inventory_randomizer = std::make_unique<Randomizer>(new IdentityRandomisation);
	npc_item_randomizer = std::make_unique<Randomizer>(new IdentityRandomisation);
	hero_inventory_randomizer = std::make_unique<Randomizer>(new IdentityRandomisation);
	stash_item_randomizer = std::make_unique<Randomizer>(new IdentityRandomisation);

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

	configureRandomizerCollection();

	auto seed = Config::RNGSeed;
	if (seed == 0)
		seed = std::random_device{}();
	RNG::inst().seed(Config::RNGSeed);

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