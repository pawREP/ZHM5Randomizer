#pragma once

#include <string>
#include "..\thirdparty\json.hpp"

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

enum class THROW_TYPE {
    NONE,
    PACIFY_LIGHT,
    PACIFY_HEAVY,
    DEADLY_LIGHT,
    DEADLY_HEAVY,
};

enum class SILENCE_RATING {
    NONE,
    NOT_SILENCED,
    SILENCED,
    SUPER_SILENCED,
};

class Item {
	ICON icon;
	CHEAT_GROUP cheat_group;
	THROW_TYPE throw_type;
	SILENCE_RATING silence_rating;

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
	const THROW_TYPE& getThrowType() const;
	const SILENCE_RATING& getSilenceRating() const;

	void print() const;
};

