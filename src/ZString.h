#pragma once
#include <cinttypes>
#include <string>
#include <string_view>
#include "Hash.h"


//This is a Glacier 2 Engine type, don't touch!

struct ZString {
	uint32_t len;
	uint32_t unk;
	char* chars;

	bool operator==(const ZString& other) const {
		if (len != other.len)
			return false;
		if(memcmp(chars, other.chars, len) == 0)
			return true;
		return false;
	}

	std::string to_string() const {
		return std::string(chars, len);
	}
};

template<> struct std::hash<ZString> {
	std::size_t operator()(ZString const& str) const noexcept {
		std::string_view sv = std::basic_string_view(str.chars, str.len);
		return std::hash<std::string_view>{}(sv);
	}
};


static_assert(sizeof(ZString) == 0x10);