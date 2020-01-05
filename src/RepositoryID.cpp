#include "RepositoryID.h"

RepositoryID::RepositoryID(const RepositoryID& guid) {
	id = guid.id;
}

RepositoryID::RepositoryID(const std::string& id_string) {
	UuidFromStringA((unsigned char*)id_string.c_str(), &id);
};

RepositoryID& RepositoryID::operator=(const RepositoryID& guid) {
	id = guid.id;
	return *this;
}

bool RepositoryID::operator ==(const RepositoryID& guid) const {
	return id == guid.id;
}

std::string RepositoryID::toString() const {
	unsigned char* guid_str;
	UuidToStringA(&id, &guid_str);
	return std::string((char*)guid_str);
}