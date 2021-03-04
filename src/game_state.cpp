#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "tokens.h"
#include "room.h"
#include "replies.h"
#include "objects.h"
#include "game_state.h"
#include "file_paths.h"
#include "files.h"

GameState::GameState(const fs::path& base_dir) {
	reply_handler = ReplyHandler();
	this->base_dir = base_dir;
}

void GameState::LoadWorld(const std::string& world_dir_name) {
	this->world_dir_name = world_dir_name;
	world_path = base_dir/file_paths::kWORLD_DIR/world_dir_name;
}

void GameState::LoadRoom(const std::string& filename) {
	room_filename = filename;
	fs::path room_path = world_path/filename;
	std::ifstream file;
	file.open(room_path, std::ios::in);
	if (!file) {
		std::cerr << "Can't load room: " << room_path << "\n";
		// TODO
		throw;
	}
	try {
		room = Room(type_to_obj_map, file);
	} catch (int e) {
		std::cerr << "Error on line " << e << "\n";
		// TODO
		throw;
	}
	reply_handler.Reply(RoomLoadedReply { /*text: */ room.description });
}

void GameState::ProcessReplies() {
	while (!reply_handler.replies.empty()) {
		std::visit(overloaded {
			[](LookReply r) {
				std::cout << r.description << "\n";
			},
			[](OpenReply r) {
				if (r.can_open)
					std::cout << "You open it and find:\n";
				else
					std::cout << "You can't open it.\n";
			},
			[](ContainerOpenedReply r) {
				std::cout << r.text << "\n";
			},
			[](PushReply r) {
				if (r.can_push)
					std::cout << "You push it.\n";
				else
					std::cout << "You can't push it.\n";
			},
			[](EventReply r) {
				std::cout << r.text << "\n";
			},
			[&](LoadRoomReply r) {
				LoadRoom(r.file);
			},
			[](RoomLoadedReply r) {
				std::cout << r.text << "\n";
			},
			[](EnterReply r) {
				if (r.can_enter)
					std::cout << "You enter it.\n";
				else
					std::cout << "You can't enter it.\n";
			},
		}, reply_handler.replies.front());
		reply_handler.replies.pop();
	}
}

void GameState::WriteSave() {
	fs::path save_path = base_dir/file_paths::kSAVE_DIR/world_dir_name;
	//if (!fs::is_directory(file_path))
	//	fs::create_directories(world_dir_name.c_str());

	std::ofstream save_info_file;
	save_info_file.open(save_path/"SAVE_INFO", std::ios::out | std::ios::trunc);
	if (!save_info_file.is_open()) {
		std::cerr << "Can't save to: " << save_path/"SAVE_INFO" << "\n";
		// TODO
		throw;
	}
	WriteFile(save_info_file, room_filename.c_str(), room_filename.length());
	save_info_file.close();


	std::ofstream room_file;
	room_file.open(save_path/room_filename, std::ios::out | std::ios::trunc | std::ios::binary);
	if (!room_file.is_open()) {
		std::cerr << "Can't save to: " << save_path/room_filename << "\n";
		// TODO
		throw;
	}

	for (auto obj : room.objs) {
		auto data = std::ostringstream();
		obj->Save(data);
		auto data_str = data.str();
		WriteFile(room_file, data_str.c_str(), data.str().length());
	}

	room_file.close();
}

void GameState::ReadSave() {
	fs::path save_path = base_dir/file_paths::kSAVE_DIR/world_dir_name;

	std::ifstream save_info_file;
	save_info_file.open(save_path/"SAVE_INFO", std::ios::in);
	if (!save_info_file.is_open()) {
		std::cerr << "Can't save to: " << save_path/"SAVE_INFO" << "\n";
		// TODO
		throw;
	}
	std::unique_ptr<char, std::function<void(void*)>> data;
	uint32_t size = ReadFile(save_info_file, data);
	room_filename.assign(data.get(), data.get()+size);

	save_info_file.close();


	LoadRoom(room_filename);

	std::ifstream file;
	file.open(save_path/room_filename, std::ios::in);
	if (!file.is_open()) {
		std::cerr << "Can't load save from: " << save_path/room_filename << "\n";
		// TODO
		throw;
	}

	for (auto obj : room.objs) {
		std::unique_ptr<char, std::function<void(void*)>> data;
		uint32_t size = ReadFile(file, data);
		if (size == 0 && data.get() == nullptr)
			break;

		auto _1 = data.get();
		obj->Load(_1, size);
	}
}
