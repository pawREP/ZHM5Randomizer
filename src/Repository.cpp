#include <fstream>
#include <unordered_set>
#include <algorithm>
#include <filesystem>
#include "RepositoryID.h"
#include "Repository.h"
#include "Item.h"

using json = nlohmann::json;

ItemRepository::ItemRepository(){
	std::string ignorelist_file = "..\\HITMAN2\\Retail\\IgnoreList.json";
	std::ifstream ignorelist_ifs(ignorelist_file);

	json ig_set;
	ignorelist_ifs >> ig_set;
	ignorelist_ifs.close();

	std::unordered_set<RepositoryID> ignore_list;
	for (const auto& it : ig_set["GLOBAL_IGNORE_LIST"].items()) {
		RepositoryID id(it.value());
		ignore_list.insert(id);
	}

	std::string repository_file = "..\\HITMAN2\\Retail\\Repository.json"; 
	std::ifstream repository_ifs(repository_file);
	
	json repository_json;
	repository_ifs >> repository_json;
	repository_ifs.close();

	for (const auto& it : repository_json.items()) {
		RepositoryID id(it.key());
		if (!ignore_list.count(id)) {
			ids.insert(id);
			items[id] = Item(it.value());
		}
	}
}

const RepositoryID* ItemRepository::getStablePointer(const RepositoryID& in) const {
	for (const auto& id : ids)
		if (id == in)
			return &id;
	return nullptr;
}

const Item* ItemRepository::getItem(const RepositoryID& id) const {
	if (contains(id))
		return &items.at(id);
	return nullptr;
}

const std::unordered_set<RepositoryID>& ItemRepository::getIds() const {
	return ids;
}

bool ItemRepository::contains(const RepositoryID& id) const {
	if (items.count(id))
		return true;
	return false;
}

RandomDrawRepository::RandomDrawRepository()
{
	std::random_device rd;
	rng_engine = std::mt19937(rd());
}

RandomDrawRepository& RandomDrawRepository::inst() {
	static RandomDrawRepository instance;
	return instance;
}

void RandomDrawRepository::getRandom(std::vector<const RepositoryID*>& item_set, unsigned int count, bool(Item::* fn)()const){
	auto hash = (void*&)fn;
	if (!cache.count(hash)) {
		cache[hash] = new std::vector<const RepositoryID *>();
		for (const auto& id : getIds()) {
			if ((getItem(id)->*fn)())
				cache[hash]->push_back(&id);
		}
	}

	auto dist = std::uniform_int_distribution<int>(0, cache[hash]->size() - 1);

	for (int i = 0; i < count; ++i)
		item_set.push_back(cache[hash]->operator[](dist(rng_engine)));
}

const RepositoryID* RandomDrawRepository::getRandom(std::function<bool(const Item&)> fn) {
	std::vector<const RepositoryID*> res;
	getRandom(res, 1, fn);
	return res[0];
}

//TODO: build cacheable version of this function that takes  bool(Item::* fn)(const Item&)const instead of lambda
void RandomDrawRepository::getRandom(std::vector<const RepositoryID*>& item_set, unsigned int count, std::function<bool(const Item&)> fn) {
	auto candidates = std::vector<const RepositoryID*>();
	for (const auto& id : getIds()) {
		if (fn(*getItem(id)))
			candidates.push_back(&id);
	}


	auto dist = std::uniform_int_distribution<int>(0, candidates.size() - 1);

	for (int i = 0; i < count; ++i)
		item_set.push_back(candidates[dist(rng_engine)]);
}
