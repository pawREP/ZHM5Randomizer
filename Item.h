#pragma once

#include <string>
#include "json.hpp"

using json = nlohmann::json;

enum class ICON {
	MELEE,
	KEY,
	EXPLOSIVE,
	QUESTITEM,
	TOOL,
	SNIPERRIFLE,
	ASSAULTRIFLE,
	REMOTE,
	SHOTGUN,
	SUITCASE,
	PISTOL,
	INVALID_CATEGORY_ICON,
	DISTRACTION,
	POISON,
	CONTAINER,
	SMG,
};

enum class CHEAT_GROUP {
	NONE,
	DEVICES,
	SNIPERS,
	ASSAULTRIFLES,
	PISTOLS,
	SHOTGUNS,
	EXOTICS,
	SMGS,
};

class Item {
	ICON icon;
	CHEAT_GROUP cheat_group;

	std::string common_name;

public:
	Item();
	Item(const json& config);

	bool isEssential() const;
	bool isNotEssential() const; 
	bool isKey() const;
	bool isQuestItem() const;
	bool isWeapon() const;
	bool isPistol() const;
	bool isSmg() const;
	bool isAssaultRifle() const;
	bool isShotgun() const;
	bool isSniper() const;
	bool isMelee() const;
	bool isExplosive() const;
	bool isTool() const;
	bool isSuitcase() const;
	bool isDistraction() const;
	bool isNotEssentialAndNotWeapon()const;

	const std::string& string() const;
	const ICON& getType() const;

	void print() const;
};

