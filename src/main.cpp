#include "tokens.h"
#include "objects.h"
#include "room.h"
#include "game_state.h"
#include <memory>
#include <string>
#include <sstream>
#include "replies.h"
#include <filesystem>
namespace fs = std::filesystem;

#include "file_paths.h"

#ifdef _WINDOWS
#include <windows.h>
#include <stdio.h>
#else
#include <dlfcn.h>
#endif

std::shared_ptr<BaseObject> Load(const fs::path& file_path, ReplyHandler& rp) {
	std::cout << "Loading: " << file_path << "\n";
#ifdef _WINDOWS
	typedef void (__stdcall *destroy_ptr)(BaseObject *);
	HINSTANCE handle = LoadLibrary(file_path.string().c_str());

	BaseObject* (__stdcall *create)(ReplyHandler&);

	create = (BaseObject* (__stdcall *)(ReplyHandler&))GetProcAddress(handle, "create_object");
	destroy_ptr destroy = (void (__stdcall *)(BaseObject*))GetProcAddress(handle, "destroy_object");
#else
	typedef void (*destroy_ptr)(BaseObject *);
	void* handle = dlopen(file_path.c_str(), RTLD_LAZY);

	BaseObject* (*create)(ReplyHandler&);

	create = (BaseObject* (*)(ReplyHandler&))dlsym(handle, "create_object");
	destroy_ptr destroy = (void (*)(BaseObject*))dlsym(handle, "destroy_object");
#endif

	BaseObject* obj = (BaseObject*)create(rp);
	return std::shared_ptr<BaseObject>(obj, destroy);
}

void LoadToMap(const fs::path& base_dir, std::map<std::string, std::shared_ptr<BaseObject>>& type_to_obj, ReplyHandler& rp) {
	auto names = {"button", "container", "door", "light", "start_button"};
	for (auto name : names) {
		std::ostringstream filename;
		filename << file_paths::kDL_PREFIX << name << file_paths::kDL_SUFFIX;
		type_to_obj[name] = Load(base_dir/file_paths::kOBJECT_DIR/filename.str(), rp);
	}
}

int main(int, char *argv[]) {
	// TODO: use fs::current_path()
	fs::path base_dir = argv[0];
	base_dir.remove_filename();

	auto state = std::make_shared<GameState>(base_dir);

	std::map<std::string, std::shared_ptr<BaseObject>> type_to_obj;
	type_to_obj["obj"] = std::make_shared<BaseObject>(state->reply_handler);

	LoadToMap(base_dir.string(), type_to_obj, state->reply_handler);
	state->type_to_obj_map = type_to_obj;


	state->LoadWorld("door_world");


	std::cout << "Load save [y/N]? ";
	char c;
	std::cin >> c;
	std::cin.ignore();
	if (c == 'y')
		state->ReadSave();
	else
		state->LoadRoom("start.txt");

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

		state->WriteSave();
	}

	return 0;
}
