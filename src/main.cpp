#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "tokens.h"
#include "objects.h"
#include "room.h"

//#include <sstream>
//#include <iterator>

int main() {
	std::ifstream file;
	file.open("room.txt", std::ios::in);
	if (!file)
		// TODO
		return 1;

	Room room;
	try {
		room = Room(std::move(file));
	} catch (int e) {
		std::cout << "error on line " << e << "\n";
		return 1;
	}

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
