#ifndef REPLIES_H
#define REPLIES_H

#include <variant>
#include <string>
#include <queue>

struct LookReply {
	std::string description;
};

struct OpenReply {
	bool can_open;
};

struct ContainerOpenedReply {
	std::string text;
};

struct PushReply {
	bool can_push;
};

struct EventReply {
	std::string text;
};

struct LoadRoomReply {
	std::string file;
};

struct RoomLoadedReply {
	std::string text;
};

class ReplyHandler {
	public:
		std::queue<std::variant<
			LookReply,
			OpenReply,
			ContainerOpenedReply,
			PushReply,
			EventReply,
			LoadRoomReply,
			RoomLoadedReply
				>> replies;
		template <typename T>
		void Reply(T reply) {
			replies.push(reply);
		}
};

#endif
