#include "tokens.h"
#include "objects.h"
#include "room.h"

#include "game_state.h"

int main() {
	auto state = std::make_shared<GameState>();

	std::cout << "Load save [y/N]? ";
	char c;
	std::cin >> c;
	std::cin.ignore();
	if (c == 'y')
		state->Load();
	else
		state->LoadRoom("../room_start.txt");

	state->ProcessReplies();

	while (true) {
		std::string input;
		std::getline(std::cin, input);

		Verb verb;
		std::string obj_name;
		switch (Tokenize(input, verb, obj_name)) {
			case TokenizeResult::Ok:
				break;
			case TokenizeResult::InvalidVerb:
				std::cout << "Verb not recognized.\n";
				continue;
			case TokenizeResult::NotASentence:
				std::cout << "Not a sentence.\n";
				continue;
		}

		int obj_i = state->room.SearchFor(obj_name);
		if (obj_i == -1) {
			std::cout << "Can't find \"" << obj_name << "\".\n";
			continue;
		}

		switch (verb) {
			case Verb::Look:
				state->room.objs[obj_i]->LookAction();
				break;
			case Verb::Open:
				state->room.objs[obj_i]->OpenAction();
				break;
			case Verb::Push:
				state->room.objs[obj_i]->PushAction();
				break;
			case Verb::Enter:
				state->room.objs[obj_i]->EnterAction();
				break;
		}

		// user input done; send time action
		for (auto obj : state->room.objs)
			obj->TimeStepAction();

		state->ProcessReplies();

		state->Save();
	}

	return 0;
}
