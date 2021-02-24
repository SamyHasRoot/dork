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

#ifdef _WINDOWS
#include <windows.h>
#include <stdio.h>
static const char* kDL_PREFIX = "";
static const char* kDL_SUFFIX = ".dll";
#else
#include <dlfcn.h>
static const char* kDL_PREFIX = "LIB";
static const char* kDL_SUFFIX = ".so";
#endif

std::shared_ptr<BaseObject> Load(std::string file_path, ReplyHandler& rp) {
	std::cout << "Loading: " << file_path << "\n";
#ifdef _WINDOWS
	typedef void (__stdcall *destroy_ptr)(BaseObject *);
	HINSTANCE handle = LoadLibrary(file_path.c_str());

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

void LoadToMap(const std::string& base_dir, std::map<std::string, std::shared_ptr<BaseObject>>& type_to_obj, ReplyHandler& rp) {
	auto names = {"button", "container", "door", "light", "start_button"};
	for (auto name : names) {
		std::ostringstream file_path;
		file_path << base_dir << "objects/" << kDL_PREFIX << name << kDL_SUFFIX;
		type_to_obj[name] = Load(file_path.str(), rp);
	}
}

int main(int, char *argv[]) {
	fs::path base_dir = argv[0];
	base_dir.remove_filename();

	auto state = std::make_shared<GameState>();

	std::map<std::string, std::shared_ptr<BaseObject>> type_to_obj;
	type_to_obj["obj"] = std::make_shared<BaseObject>(state->reply_handler);

	LoadToMap(base_dir.string(), type_to_obj, state->reply_handler);
	state->type_to_obj_map = type_to_obj;


	std::cout << "Load save [y/N]? ";
	char c;
	std::cin >> c;
	std::cin.ignore();
	if (c == 'y')
		state->Load((base_dir/"saves/save.bin").string());
	else
		state->LoadRoom((base_dir/"rooms/room_start.txt").string());

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

		state->Save((base_dir/"saves/save.bin").string());
	}

	return 0;
}
