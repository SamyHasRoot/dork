#include <iostream>
#include <string>
#include <vector>

enum Verb {
	read,
	open,
};

bool TokenizeVerb(std::string text, Verb &verb) {
	if (text == "read")
		verb = Verb::read;
	else if (text == "open")
		verb = Verb::open;
	else
		return false;

	return true;
}

class WorldObj {
	public:
		std::vector<std::string> names;
};

class Room {
	public:
		std::vector<WorldObj> objs;
};

int FindObj(std::string name, Room room) {
	for (int obj_i = 0; obj_i < room.objs.size(); obj_i++) {
		for (std::string obj_name : room.objs[obj_i].names) {
			if (obj_name == name) {
				return obj_i;
			}
		}
	}
	return -1;
}

int main() {
	Room room;

	WorldObj note;
	note.names.push_back("note");
	note.names.push_back("paper");

	room.objs.push_back(note);

	std::string input;
	do {
		std::getline(std::cin, input);
	} while (FindObj(input, room) == -1);

	return 0;
}
