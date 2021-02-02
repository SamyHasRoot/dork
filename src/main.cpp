#include <fstream>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <memory>

#include "tokens.h"
#include "objects.h"
#include "room.h"

#include "replies.h"

struct ReplyVisitor {
	void operator()(const NameReply r) const {
		std::cout << "NameReply: " << r.name << "\n";
	}
	void operator()(const ContainerOpenedReply r) const {
		std::cout << r.text << "\n";
	}
	void operator()(const DescribeReply r) const {
		std::cout << r.description << "\n";
	}
	void operator()(const OpenReply r) const {
		if (r.can_open)
			std::cout << "You open it and find:\n";
		else
			std::cout << "You can't open it.\n";
	}
	void operator()(const PushReply r) const {
		if (r.can_push)
			std::cout << "You push it.\n";
		else
			std::cout << "You can't push it.\n";
	}
	void operator()(const EventReply r) const {
		std::cout << r.text << "\n";
	}
};

void ProcessReplies(std::shared_ptr<ReplyHandler> reply_handler) {
	while (!reply_handler->replies.empty()) {
		std::visit(ReplyVisitor(), reply_handler->replies.front());
		reply_handler->replies.pop();
	}
}

int main() {
	std::ifstream file;
	file.open("../room.txt", std::ios::in);
	if (!file)
		// TODO
		return 1;

	auto reply_handler = std::make_shared<ReplyHandler>();

	auto room = std::shared_ptr<Room>();
	try {
		room = std::make_shared<Room>(reply_handler, std::move(file));
	} catch (int e) {
		std::cout << "error on line " << e << "\n";
		return 1;
	}

	while (true) {
		std::string input;
		std::getline(std::cin, input);

		Verb verb;
		std::string obj_name;
		switch (Tokenize(input, verb, obj_name)) {
			case TokenizeResult::Ok:
				break;
			case TokenizeResult::NotEnoughWords:
				std::cout << "Not enough words.\n";
				continue;
			case TokenizeResult::TooManyWords:
				std::cout << "Too many words.\n";
				continue;
			case TokenizeResult::InvalidVerb:
				std::cout << "Verb not recognized.\n";
				continue;
		}

		int obj_i = room->SearchFor(obj_name);
		if (obj_i == -1) {
			std::cout << "Can't find \"" << obj_name << "\".\n";
			continue;
		}

		switch (verb) {
			case Verb::Look:
				room->objs[obj_i]->DescribeAction();
				break;
			case Verb::Open:
				room->objs[obj_i]->OpenAction();
				break;
			case Verb::Push:
				room->objs[obj_i]->PushAction();
				break;
		}

		ProcessReplies(reply_handler);
	}

	return 0;
}
