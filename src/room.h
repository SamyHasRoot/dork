#ifndef ROOM_H
#define ROOM_H

#include "objects.h"
#include <memory>
#include <string>

class Room {
	public:
		Room();
		Room(ReplyHandler& reply_handler, std::ifstream& file);
		// search room for objects by name. returns index of this.objs or -1 for "not found"
		int SearchFor(std::string &name);
		// objects in room
		std::vector<std::shared_ptr<WorldObj>> objs;
		// room description
		std::string description;
};

#endif
