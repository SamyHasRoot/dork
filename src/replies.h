#ifndef REPLIES_H
#define REPLIES_H

#include <variant>
#include <string>
#include <queue>

struct OpenReply {
	bool can_open;
};

struct NameReply {
	std::string name;
};

struct DescribeReply {
	std::string description;
};

class ReplyHandler {
	public:
		std::queue<std::variant<NameReply, DescribeReply, OpenReply>> replies;
		template <typename T>
		void Reply(T reply) {
			replies.push(reply);
		}
};

#endif
