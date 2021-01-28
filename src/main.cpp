#include <vector>
#include <memory>

#include "tokens.h"
#include "objects.h"
#include "room.h"

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
