#pragma once
#include <type_traits>
#include "ZString.h"

//This is a Glacier 2 Engine type, don't touch!

//There is something fucked up going on with this structure,
//it seems like TArray<ZString> employs some sort of short array
//optimisation, where a TArray<ZString> with a single entry will turn into 
//an inlined ZString. That's how the TArray in SSceneInitParams behaves at least.
//This is fine if you use the source code but reading such structures from
//memory is a bit cumbersome so please excuse the sketchy code and UB in the code below.
//Data members should only be accessed after checking if the struct is 
//using the short array optimisation. This can be done by calling isShort();
template<typename T = ZString>
class TArray {
private:

	static_assert(std::is_same_v<T, ZString>);

	union {
		struct {
			T* start_;
			T* end_;
			T* last_;
		};
		T t;
	};

	bool isShort() const {
		if (start_ == nullptr)
			return false; //empty array

		ZString str;
		memcpy_s(&str, sizeof(ZString), this, sizeof(ZString));

		if (str.len > 0xFFFF)//assuming strings aren't longer than 0xFFFF;
			return false;
		return true;
	};

public:
	TArray<T>() = delete;

	const T* start() const {
		if(isShort())
			return &t;
		return start_;
	};

	const T* end() const {
		if (isShort())
			return &t;
		return end_;
	};

	T operator[](int i) const {
		if(isShort())
			return t;
		return start_[i];
	};

	const T& operator[](int i) {
		if (isShort())
			return t;
		return start_[i];
	};

	size_t size() const {
		if (isShort())
			return 1;
		return end_ - start_;
	}

	bool contains(const T& o) const {
		if (isShort())
			return o == t;
		if (std::find(start_, end_, o) != end_)
			return true;
		return false;
	}

};

static_assert(sizeof(TArray<ZString>) == 0x18);

template<typename T>
struct std::hash<TArray<T>> {
	constexpr size_t operator()(TArray<T> arr) {
		size_t hash = 0;
		for (int i = 0; i < arr.size(); ++i) {
			hash = Hash::hash_combine(hash, std::hash<T>{}(arr[i]));
		}
		return hash;
	}
};

