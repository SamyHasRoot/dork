#include <fstream>
#include <sstream>

#include "tokens.h"
#include "room.h"
#include "replies.h"
#include "objects.h"
#include "game_state.h"

GameState::GameState() {
	reply_handler = ReplyHandler();
}
void GameState::LoadRoom(std::string filename) {
	std::ifstream file;
	room_filename = filename;
	file.open(filename, std::ios::in);
	if (!file) {
		std::cerr << "Can't load room: " << filename << "\n";
		// TODO
		throw;
	}
	try {
		room = Room(reply_handler, file);
	} catch (int e) {
		std::cerr << "Error on line " << e << "\n";
		// TODO
		throw;
	}
	reply_handler.Reply(RoomLoadedReply { .text = room.description });
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

void GameState::Save() {
	std::ofstream file;
	file.open("../save.bin", std::ios::out | std::ios::trunc | std::ios::binary);
	if (!file.is_open())
		throw;

	uint32_t room_filename_size = room_filename.length();
	file.write((char*)&room_filename_size, sizeof(uint32_t));
	file.write(room_filename.c_str(), sizeof(char)*room_filename.size());
	for (auto obj : room.objs) {
		auto data = std::ostringstream();
		obj->Save(data);
		{
			uint32_t t = data.str().length();
			file.write((char*)&t, sizeof(uint32_t));
		}
		file.write(data.str().c_str(), data.str().length());
	}

	file.close();
}
void GameState::Load() {
	std::ifstream file;
	file.open("../save.bin", std::ios::in);
	if (!file.is_open())
		throw;

	uint32_t room_name_size;
	file.read((char*)&room_name_size, sizeof(uint32_t));

	char room_name[room_name_size+1];
	file.read(room_name, room_name_size);
	room_name[room_name_size] = (char)NULL;

	LoadRoom(room_name);

	int i = 0;
	while (!file.eof()) {
		uint32_t size;
		file.read((char*)&size, sizeof(uint32_t));
		if (file.eof())
			break;

		char data[size];
		file.read((char*)&data, size);

		char* p = &data[0];
		room.objs[i]->Load(p, size);
		i++;
	}
}

