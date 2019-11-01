#pragma once
#include <string>
#include "SSceneInitParameters.h"


class Scenario {
public:
	static const Scenario NONE;
	static const Scenario LOCATION_PEACOCK_CASUAL;
	static const Scenario LOCATION_PEACOCK_PROFESSIONAL;
	static const Scenario LOCATION_PEACOCK_MASTER;
	static const Scenario LOCATION_OCTOPUS_CASUAL;
	static const Scenario LOCATION_OCTOPUS_PROFESSIONAL;
	static const Scenario LOCATION_OCTOPUS_MASTER;
	static const Scenario LOCATION_SPIDER_CASUAL;
	static const Scenario LOCATION_SPIDER_PROFESSIONAL;
	static const Scenario LOCATION_SPIDER_MASTER;
	static const Scenario LOCATION_TIGER_CASUAL;
	static const Scenario LOCATION_TIGER_PROFESSIONAL;
	static const Scenario LOCATION_TIGER_MASTER;
	static const Scenario LOCATION_BULL_CASUAL;
	static const Scenario LOCATION_BULL_PROFESSIONAL;
	static const Scenario LOCATION_BULL_MASTER;
	static const Scenario LOCATION_SNOWCRANE_CASUAL;
	static const Scenario LOCATION_SNOWCRANE_PROFESSIONAL;
	static const Scenario LOCATION_SNOWCRANE_MASTER;
	static const Scenario LOCATION_SHEEP_CASUAL;
	static const Scenario LOCATION_SHEEP_PROFESSIONAL;
	static const Scenario LOCATION_SHEEP_MASTER;
	static const Scenario LOCATION_FLAMINGO_CASUAL;
	static const Scenario LOCATION_FLAMINGO_PROFESSIONAL;
	static const Scenario LOCATION_FLAMINGO_MASTER;
	static const Scenario LOCATION_HIPPO_CASUAL;
	static const Scenario LOCATION_HIPPO_PROFESSIONAL;
	static const Scenario LOCATION_HIPPO_MASTER;
	static const Scenario LOCATION_MONGOOSE_CASUAL;
	static const Scenario LOCATION_MONGOOSE_PROFESSIONAL;
	static const Scenario LOCATION_MONGOOSE_MASTER;
	static const Scenario LOCATION_SKUNK_CASUAL;
	static const Scenario LOCATION_SKUNK_PROFESSIONAL;
	static const Scenario LOCATION_SKUNK_MASTER;
	static const Scenario LOCATION_MAGPIE_CASUAL;
	static const Scenario LOCATION_MAGPIE_PROFESSIONAL;
	static const Scenario LOCATION_MAGPIE_MASTER;
	static const Scenario LOCATION_RACCOON_CASUAL;
	static const Scenario LOCATION_RACCOON_PROFESSIONAL;
	static const Scenario LOCATION_RACCOON_MASTER;
	static const Scenario LOCATION_STINGRAY_CASUAL;
	static const Scenario LOCATION_STINGRAY_PROFESSIONAL;
	static const Scenario LOCATION_STINGRAY_MASTER;

private:
	int index;

public:
	std::string string() const;
	bool operator==(const Scenario& scenario) const;
	operator int() const;

	static Scenario from_string(std::string str);
	static Scenario from_SceneInitParams(const SSceneInitParameters& params);

private:
	Scenario(int index);
};

template<>
struct std::hash<Scenario> {
	size_t operator()(const Scenario& scenario) const {
		return std::hash<int>()(scenario);
	}
};