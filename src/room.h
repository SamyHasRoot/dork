#ifndef ROOM_H
#define ROOM_H

#include <memory>
#include <string>
#include <vector>
#include <map>

class BaseObject;

class Room {
	public:
		typedef std::map<std::string, unsigned int> name_index_type;
		Room();
		Room(std::map<std::string, std::shared_ptr<BaseObject>> type_to_obj_map, std::ifstream& file);
		// search room for objects by name. returns index of this.objs or -1 for "not found"
		int SearchFor(std::string &name);
		// objects in room
		std::vector<std::shared_ptr<BaseObject>> objs;
		// room description
		std::string description;
};

#endif
