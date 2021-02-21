#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "room.h"
#include "replies.h"

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
// ^- wtf c++

class GameState {
	public:
		GameState();
		void LoadRoom(std::string filename);

		void ProcessReplies();

		void Save();
		void Load();

		Room room;
		std::string room_filename;
		ReplyHandler reply_handler;
		std::map<std::string, std::shared_ptr<BaseObject>> type_to_obj_map;
};

#endif
