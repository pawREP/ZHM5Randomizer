#pragma once
#include <rpc.h>
#include <string>

#pragma comment(lib, "Rpcrt4.lib")

struct RepositoryID {
	GUID id;

	RepositoryID(const RepositoryID& guid);
	RepositoryID(const std::string& id_string);

	std::string toString() const;

	RepositoryID& operator=(const RepositoryID& guid);
	bool operator==(const RepositoryID& guid) const;
};

template<>
struct std::hash<RepositoryID> {
	size_t operator()(const RepositoryID& guid) const {
		size_t hash = 14695981039346656037;
		for (unsigned int i = 0; i < sizeof(guid.id); i++) {
			hash *= 1099511628211;
			hash ^= ((const char*)&(guid.id))[i];
		}
		return hash;
	}
};