#include "DefaultItemPoolRepository.h"
#include <fstream>

DefaultItemPoolRepository::DefaultItemPoolRepository(std::string path) {

    std::ifstream ifs(path);

    json repository_json;
    ifs >> repository_json;
    ifs.close();

    for(const auto& it : repository_json.items()) {
        auto key = std::stoull(it.key(), nullptr, 0x10);
        item_pools[key] = std::make_unique<DefaultItemPool>(it.value());
    }
}

DefaultItemPool* DefaultItemPoolRepository::getDefaultPool(Scenario scen) {
    if(item_pools.count(scen))
        return item_pools.at(scen).get();
    return nullptr;
}