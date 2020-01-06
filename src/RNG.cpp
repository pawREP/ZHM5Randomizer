#include "RNG.h"

RNG::RNG() : rng(std::random_device{}()) {

}

RNG& RNG::inst() {
	static RNG rng;
	return rng;
}

void RNG::seed(uint32_t seed) {
	rng.seed(seed);
}

std::mt19937* RNG::getEngine() {
	return &rng;
}
