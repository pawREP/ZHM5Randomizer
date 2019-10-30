#include "stdafx.h"
#include "Hash.h"

size_t Hash::hash_combine(const size_t h0, const size_t h1) {
	return (h0 << 1) ^ h1;
}