#include "tokens.h"
#include "objects.h"
#include "room.h"
#include "game_state.h"
#include <memory>
#include <string>
#include <sstream>
#include <dlfcn.h>
#include "replies.h"
#include <filesystem>

typedef void (*destroy_ptr)(BaseObject *);

std::shared_ptr<BaseObject> Load(std::string file_path, ReplyHandler& rp) {
	std::cout << "Loading: " << file_path << "\n";

	void* handle = dlopen(file_path.c_str(), RTLD_LAZY);
	if (auto e = dlerror()) std::cout << e << "\n";
	if (auto e = dlerror()) std::cout << e << "\n";
	if (auto e = dlerror()) std::cout << e << "\n";

	BaseObject* (*create)(ReplyHandler&);

	create = (BaseObject* (*)(ReplyHandler&))dlsym(handle, "create_object");
	if (auto e = dlerror()) std::cout << e << "\n";
	if (auto e = dlerror()) std::cout << e << "\n";
	if (auto e = dlerror()) std::cout << e << "\n";
	destroy_ptr destroy = (void (*)(BaseObject*))dlsym(handle, "destroy_object");
	if (auto e = dlerror()) std::cout << e << "\n";
	if (auto e = dlerror()) std::cout << e << "\n";
	if (auto e = dlerror()) std::cout << e << "\n";

	BaseObject* obj = (BaseObject*)create(rp);
	return std::shared_ptr<BaseObject>(obj, destroy);
}

void LoadToMap(const std::string& base_dir, std::map<std::string, std::shared_ptr<BaseObject>>& type_to_obj, ReplyHandler& rp) {
	auto names = {"button", "container", "door", "light", "start_button"};
	for (auto name : names) {
		std::ostringstream file_path;
		file_path << base_dir << "objects/" << "lib" << name << ".so";
		type_to_obj[name] = Load(file_path.str(), rp);
	}
}

int main(int, char *argv[]) {
	std::filesystem::path base_dir = argv[0];
	base_dir.remove_filename();

	auto state = std::make_shared<GameState>();

	std::map<std::string, std::shared_ptr<BaseObject>> type_to_obj;
	type_to_obj["obj"] = std::make_shared<BaseObject>(state->reply_handler);

	LoadToMap(base_dir, type_to_obj, state->reply_handler);
	state->type_to_obj_map = type_to_obj;


	std::cout << "Load save [y/N]? ";
	char c;
	std::cin >> c;
	std::cin.ignore();
	if (c == 'y')
		state->Load(base_dir/"saves/save.bin");
	else
		state->LoadRoom(base_dir/"rooms/room_start.txt");

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

		state->Save(base_dir/"saves/save.bin");
	}

	return 0;
}
