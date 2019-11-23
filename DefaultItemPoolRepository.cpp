#include <fstream>
#include "DefaultItemPoolRepository.h"

DefaultItemPoolRepository::DefaultItemPoolRepository(std::string path) {

	std::ifstream ifs(path);
	
	json repository_json;
	ifs >> repository_json;
	ifs.close();

	for (const auto& it : repository_json.items()) {
		Scenario scen = Scenario::from_string(it.key());
		item_pools[scen] = std::make_unique<DefaultItemPool>(it.value());
	}
}

DefaultItemPool* DefaultItemPoolRepository::getDefaultPool(Scenario scen) {
	if(item_pools.count(scen))
		return item_pools.at(scen).get();
	return nullptr;
}