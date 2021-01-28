#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>

#include <sstream>
#include <iterator>

enum Verb {
	Look,
	Open,
	Read,
};

bool TokenizeVerb(std::string text, Verb &verb) {
	if (text == "look")
		verb = Verb::Look;
	else if (text == "open")
		verb = Verb::Open;
	else if (text == "read")
		verb = Verb::Read;
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
		// names the object can be referred to with
		std::vector<std::string> names;
		// whether or not object has been seen*
		bool is_known = true;
		// description of object; printed when Verb::look is used
		std::string description;
		// determine whether object can be opened (you can't)
		virtual bool open() { return false; }
		// called when container opens (if object in container)
		virtual void OnContainerOpen() {
			is_known = true;
		}
};

class Room {
	public:
		// search room for objects by name. returns index of this.objs or -1 for "not found"
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

		// objects in room
		std::vector<std::shared_ptr<WorldObj>> objs;
};

class Container: public WorldObj {
	public:
		// TODO: use constructor inheritance
		Container(bool is_known, std::string name, std::string description) : WorldObj(is_known, name, description) {};
		// open container; calls OnContainerOpen for all objects in contents
		bool open() override {
			for (auto obj : contents)
				obj->OnContainerOpen();

			return true;
		}

		std::vector<std::shared_ptr<WorldObj>> contents;
};

enum TokenizeResult {
	Ok,
	NotEnoughWords,
	TooManyWords,
	InvalidVerb,
};

TokenizeResult Tokenize(std::string text, Verb &verb, std::string &obj_name) {
	// split on ' ' (also wtf, c++)
	std::istringstream iss(text);
	std::vector<std::string> text_vec{std::istream_iterator<std::string>{iss},
		std::istream_iterator<std::string>{}};

	obj_name = text_vec[1];

	if (text_vec.size() != 2)
		return text_vec.size() > 2 ? TooManyWords : NotEnoughWords;

	if (!TokenizeVerb(text_vec[0], verb))
		return TokenizeResult::InvalidVerb;

	return TokenizeResult::Ok;
}

int main() {
	// hardcoded room for now
	Room room;

	auto box = std::make_shared<Container>(true, "box", "Just a box.");

	auto note = std::make_shared<WorldObj>(false, "note", "It's made of paper.");
	note->names.push_back("paper");

	box->contents.push_back(note);

	room.objs.push_back(box);
	room.objs.push_back(note);

	while (true) {
		std::string input;
		std::getline(std::cin, input);

		Verb verb;
		std::string obj_name;
		switch (Tokenize(input, verb, obj_name)) {
			case TokenizeResult::Ok:
				break;
			case TokenizeResult::NotEnoughWords:
				std::cout << "Not enough words.\n";
				continue;
			case TokenizeResult::TooManyWords:
				std::cout << "Too many words.\n";
				continue;
			case TokenizeResult::InvalidVerb:
				std::cout << "Verb not recognized.\n";
				continue;
		}

		int obj_i = room.SearchFor(obj_name);
		if (obj_i == -1) {
			std::cout << "Can't find \"" << obj_name << "\".\n";
			continue;
		}

		switch (verb) {
			case Verb::Look:
				std::cout << room.objs[obj_i]->description << "\n";
				break;
			case Verb::Open:
				if (room.objs[obj_i]->open()) {
					std::cout << "Opened " << obj_name << ".\n";
				} else {
					std::cout << "Can't open " << obj_name << ".\n";
				}
				break;
			case Verb::Read:
				// TODO
				break;
		}
	}

	return 0;
}
