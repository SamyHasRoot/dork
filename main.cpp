#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <sstream>
#include <iterator>

enum Verb {
	look,
	open,
	read,
};

bool TokenizeVerb(std::string text, Verb &verb) {
	if (text == "look")
		verb = Verb::look;
	else if (text == "open")
		verb = Verb::open;
	else if (text == "read")
		verb = Verb::read;
	else
		return false;

	return true;
}

class Room;
class WorldObj {
	public:
		WorldObj(bool is_known, std::string name, std::string description) {
			this->is_known = is_known;
			this->description = description;
			this->names.push_back(name);
		}
		std::vector<std::string> names;
		bool is_known = true;
		std::string description;
		virtual bool open(Room &room) { return false; }
};

class Room {
	public:
		int SearchFor(std::string name) {
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
		std::vector<std::unique_ptr<WorldObj>> objs;
};

class Container: public WorldObj {
	public:
		// TODO: use constructor inheritance
		Container(bool is_known, std::string name, std::string description) : WorldObj(is_known, name, description) {};
		bool open(Room &room) override {
			for (int obj_i : contents)
				room.objs[obj_i]->is_known = true;

			return true;
		}

		std::vector<int> contents;
};

int main() {
	Room room;

	auto box = std::make_unique<Container>(true, "box", "Just a box.");

	auto note = std::make_unique<WorldObj>(false, "note", "It's made of paper.");
	note->names.push_back("paper");

	box->contents.push_back(1); // put note in box

	room.objs.push_back(std::move(box));
	room.objs.push_back(std::move(note));

	std::string input;
	while (true) {
		std::getline(std::cin, input);

		// split on ' '
		std::istringstream iss(input);
		std::vector<std::string> input_vec{std::istream_iterator<std::string>{iss},
			std::istream_iterator<std::string>{}};

		if (input_vec.size() == 2) {
			Verb verb;
			if (TokenizeVerb(input_vec[0], verb)) {
				int obj_i = room.SearchFor(input_vec[1]);
				if (obj_i == -1) {
					std::cout << "Can't find \"" << input_vec[1] << "\".\n";
					continue;
				}
				switch (verb) {
					case Verb::look:
						std::cout << room.objs[obj_i]->description << "\n";
						break;
					case Verb::open:
						if (room.objs[obj_i]->open(room)) {
							std::cout << "Opened " << input_vec[1] << ".\n";
						} else {
							std::cout << "Can't open " << input_vec[1] << ".\n";
						}
						break;
				}
			} else {
				std::cout << "Verb not recognized.\n";
			}
		} else {
			std::cout << "Not enough words.\n";
		}
	}

	return 0;
}
