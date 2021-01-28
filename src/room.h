#ifndef ROOM_H
#define ROOM_H

#include "objects.h"

class Room {
	public:
		// search room for objects by name. returns index of this.objs or -1 for "not found"
		int SearchFor(std::string name);
		// objects in room
		std::vector<std::shared_ptr<WorldObj>> objs;
};

#endif
