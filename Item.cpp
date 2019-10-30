#include "stdafx.h"
#include "Item.h"
#include <unordered_map>

std::unordered_map<std::string, ICON> icon_map{
	{"melee", ICON::MELEE},
	{"key", ICON::KEY },
	{"explosives", ICON::EXPLOSIVE },
	{"questitem", ICON::QUESTITEM },
	{"tool", ICON::TOOL },
	{"sniperrifle", ICON::SNIPERRIFLE },
	{"assaultrifle", ICON::ASSAULTRIFLE },
	{"remote", ICON::REMOTE },
	{"QuestItem", ICON::QUESTITEM },
	{"shotgun", ICON::SHOTGUN },
	{"suitcase", ICON::SUITCASE },
	{"pistol", ICON::PISTOL },
	{"INVALID_CATEGORY_ICON", ICON::INVALID_CATEGORY_ICON },
	{"distraction", ICON::DISTRACTION },
	{"poison", ICON::POISON },
	{"Container", ICON::CONTAINER },
	{"smg", ICON::SMG },
};

inline ICON operator|(ICON i, ICON j) {
	using T = std::underlying_type_t<ICON>;
	return static_cast<ICON>(static_cast<T>(i) | static_cast<T>(j));
}

inline ICON operator&(ICON i, ICON j) {
	using T = std::underlying_type_t<ICON>;
	return static_cast<ICON>(static_cast<T>(i) & static_cast<T>(j));
}

std::unordered_map<std::string, CHEAT_GROUP> cheat_group_map{
	{ "eCGNone", CHEAT_GROUP::NONE},
	{ "eCGDevices", CHEAT_GROUP::DEVICES },
	{ "eCGSniper", CHEAT_GROUP::SNIPERS },
	{ "eCGAssaultRifles", CHEAT_GROUP::ASSAULTRIFLES },
	{ "eCGPistols", CHEAT_GROUP::PISTOLS },
	{ "eCGShotguns", CHEAT_GROUP::SHOTGUNS },
	{ "eCGExotics", CHEAT_GROUP::EXOTICS },
	{ "eCGSMGs", CHEAT_GROUP::SMGS }
};

Item::Item() {

}

Item::Item(const json& config) {
	if (!config.is_object())
		throw "Item repository corrupted";

	try {
		icon = icon_map[config["InventoryCategoryIcon"].get<std::string>()];
		cheat_group = cheat_group_map[config["CheatGroup"].get<std::string>()];
		common_name = config["CommonName"].get<std::string>();
	}
	catch (...) {
		//There doesn't seem to be a convinient way to tell if a key exists in the json object.
		//The repository builder should ensure that all keys are present!
		throw "ItemConfig(const json& config): Some key is missing from constructor json";
	}
}

bool Item::isEssential() const {
	return isKey() || isQuestItem();
}

bool Item::isNotEssential() const {
	return !isEssential();
}

bool Item::isKey() const {
	return icon == ICON::KEY;
}

bool Item::isQuestItem() const {
	return icon == ICON::QUESTITEM;
}

bool Item::isWeapon() const {
	return isPistol() || isSmg() || isShotgun() || isAssaultRifle() || isSniper();
}

bool Item::isPistol() const {
	return icon == ICON::PISTOL;
}

bool Item::isSmg() const {
	return icon == ICON::SMG;
}

bool Item::isAssaultRifle() const {
	return icon == ICON::ASSAULTRIFLE;
}

bool Item::isShotgun() const {
	return icon == ICON::SHOTGUN;
}

bool Item::isSniper() const {
	return icon == ICON::SNIPERRIFLE;
}

bool Item::isMelee() const {
	return icon == ICON::MELEE;
}

bool Item::isExplosive() const {
	return icon == ICON::EXPLOSIVE;
}

bool Item::isTool() const {
	return icon == ICON::TOOL;
}

bool Item::isSuitcase() const {
	return icon == ICON::SUITCASE;
}

bool Item::isDistraction() const {
	return icon == ICON::DISTRACTION;
}

bool Item::isNotEssentialAndNotWeapon() const {
	return !isEssential() && !isWeapon();
}

const std::string& Item::string() const {
	return common_name;
}

const ICON& Item::getType() const {
	return icon;
}


void Item::print() const {
	//inefficient but it's only for debug printing so shouldn't matter
	std::string icon_name;
	for(const auto& i : icon_map)
		if (i.second == getType()) {
			icon_name = i.first;
			break;
		}
	printf("%s : %s : isEssential = %d\n", string().c_str(), icon_name.c_str(), isEssential());
}