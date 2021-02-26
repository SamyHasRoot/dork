#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <filesystem>
namespace fs = std::filesystem;
#include "room.h"
#include "replies.h"
#include <string>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
// ^- wtf c++

class GameState {
	public:
		GameState(const fs::path& base_dir);
		void LoadRoom(const std::string& filename);
		void LoadWorld(const std::string& world_dir_name);

		void ProcessReplies();

		void WriteSave();
		void ReadSave();

		Room room;
		std::string room_filename;
		ReplyHandler reply_handler;
		std::map<std::string, std::shared_ptr<BaseObject>> type_to_obj_map;

		std::string world_dir_name;
		fs::path world_path;
		fs::path base_dir;
};

#endif
