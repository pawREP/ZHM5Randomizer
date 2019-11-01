#pragma once
#include <unordered_map>
#include <memory>
#include "DefaultItemPool.h"
#include "Scenario.h"


class DefaultItemPoolRepository
{
private:
	std::unordered_map<Scenario, std::unique_ptr<DefaultItemPool>> item_pools;

public:
	DefaultItemPoolRepository(std::string path);

	DefaultItemPool* getDefaultPool(Scenario);

};

