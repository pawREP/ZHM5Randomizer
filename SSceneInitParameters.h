#pragma once
#include "ZString.h"
#include "TArray.h"
#include <regex>

//This is a Glacier 2 Engine type, don't touch!
struct SSceneInitParameters {
	ZString m_SceneResource;
	TArray<ZString> m_aAdditionalBrickResources;

	void print() const;

private:
	SSceneInitParameters() = delete;
};

//Simulation quality bricks are skipped by this hash function!
//TODO: Consider delegating hashing responsibilities to Scenario class
template<> 
struct std::hash<SSceneInitParameters> {
	std::size_t operator()(SSceneInitParameters const& sip) const noexcept {
		size_t hash = std::hash<ZString>()(sip.m_SceneResource);
		for (int i = 0; i < sip.m_aAdditionalBrickResources.size(); ++i) {
			std::string str = sip.m_aAdditionalBrickResources[i].to_string();
			if (std::regex_search(str, std::regex("6core", std::regex_constants::icase)) ||
				std::regex_search(str, std::regex("8core", std::regex_constants::icase)))
				continue;
			hash = Hash::hash_combine(hash, std::hash<std::string>()(str));
		}
		return hash;
	}

};