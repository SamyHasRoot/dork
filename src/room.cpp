#include "room.h"

int Room::SearchFor(std::string name) {
	for (unsigned int obj_i = 0; obj_i < this->objs.size(); obj_i++) {
		if (this->objs[obj_i]->is_known) {
			for (std::string obj_name : this->objs[obj_i]->names) {
				if (obj_name == name) {
					return obj_i;
				}
			}
		}
	}
	return -1;
}
