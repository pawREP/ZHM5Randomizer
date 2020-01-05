#pragma once
#include <type_traits>
#include <unordered_map>
#include <queue>
#include <random>
#include "Repository.h"
#include "..\thirdparty\json.hpp"
#include "Scenario.h"


class DefaultItemPool;

class RandomisationStrategy {
protected:
	RandomDrawRepository& repo;
	std::mt19937 rng_engine;

	RandomisationStrategy(unsigned long long RNG_seed = 0);

public:
	//Takes Repository ID and returns a new ID according to the internal randomisation strategy
	//Item IDs that don't have a corresponding item configuration in the Repository should be skipped.
	virtual const RepositoryID* randomize(const RepositoryID* in_out_ID) = 0;

	//Called on SceneLoad. This function is intended for stateful randomisation strategies
	//which might require knowledge of the next scene and/or default item pool of that scene
	//to setup their internal state in preparation of item randomisation.
	virtual void initialize(Scenario, const DefaultItemPool* const ) = 0;
};

//This randomisation strategy is intended to be used to randomize world items during the initial load of a level.
//It's desiged to be as undistruptive to the game flow as possible.
class WorldInventoryRandomisation : public RandomisationStrategy {
private:
	std::queue<const RepositoryID*> item_queue;

public:
	WorldInventoryRandomisation();

	const RepositoryID* randomize(const RepositoryID* in_out_ID) override;
	void initialize(Scenario scen, const DefaultItemPool* const default_pool) override;
};

class NPCItemRandomisation : public RandomisationStrategy {
private:

public:
	NPCItemRandomisation();

	const RepositoryID* randomize(const RepositoryID* in_out_ID) override;
	void initialize(Scenario scen, const DefaultItemPool* const default_pool) override;
};

/*
This randomisation stratgy is intended to be used to randomize 47's starting inventory.
To preserve some intentionallity the randomizer will only randomize items within their own
category as defined by the InventoryCategoryIcon key in the item repository. Those categories are: 
melee, key, explosives, questitem, tool, sniperrifle, assaultrifle, remote, QuestItem, shotgun,
suitcase, pistol, distraction, poison, Container and smg.
*/
class HeroInventoryRandomisation : public RandomisationStrategy {
public:
	HeroInventoryRandomisation();

	const RepositoryID* randomize(const RepositoryID* in_out_ID) override;
	void initialize(Scenario scen, const DefaultItemPool* const default_pool) override;
};

class StashInventoryRandomisation : public RandomisationStrategy {
public:
	StashInventoryRandomisation();

	const RepositoryID* randomize(const RepositoryID* in_out_ID) override;
	void initialize(Scenario scen, const DefaultItemPool* const default_pool) override;
};

class Randomizer {
private:
	bool enabled;
	std::unique_ptr<RandomisationStrategy> strategy;

public:
	Randomizer(RandomisationStrategy*);
	const RepositoryID* randomize(const RepositoryID* id);
	void initialize(Scenario, const DefaultItemPool* const);
	void disable();
};
