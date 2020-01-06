#pragma once
#include <random>

class RNG {
private:
	std::mt19937 rng;

	RNG();
public:
	static RNG& inst();

	void seed(uint32_t seed);
	std::mt19937* getEngine();
};

