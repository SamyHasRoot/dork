#include "objects.h"

WorldObj::WorldObj(bool is_known, std::string name, std::string description) {
	this->is_known = is_known;
	this->description = description;
	this->names.push_back(name);
}

bool WorldObj::open() { return false; }

void WorldObj::OnContainerOpen() {
	is_known = true;
}

bool Container::open() {
	for (auto obj : contents)
		obj->OnContainerOpen();

	return true;
}
