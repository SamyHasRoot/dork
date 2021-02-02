#ifndef REPLIES_H
#define REPLIES_H

#include <variant>
#include <string>
#include <queue>

struct OpenReply {
	bool can_open;
};

struct ContainerOpenedReply {
	std::string text;
};

struct NameReply {
	std::string name;
};

struct DescribeReply {
	std::string description;
};

struct PushReply {
	bool can_push;
};

struct EventReply {
	std::string text;
};

class ReplyHandler {
	public:
		std::queue<std::variant<NameReply, DescribeReply, OpenReply, ContainerOpenedReply, PushReply, EventReply>> replies;
		template <typename T>
		void Reply(T reply) {
			replies.push(reply);
		}
};

#endif
