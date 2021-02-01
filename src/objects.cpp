#include "objects.h"
#include <memory>

WorldObj::WorldObj(std::shared_ptr<ReplyHandler> reply_handler, bool is_known, std::string name, std::string description) {
	this->reply_handler = reply_handler;
	this->is_known = is_known;
	this->description = description;
	this->names.push_back(name);
}

void WorldObj::NameAction() {
	reply_handler->Reply(NameReply { .name = names[0] });
}

void WorldObj::DescribeAction() {
	reply_handler->Reply(DescribeReply { .description = description });
}

void WorldObj::OpenAction() {
	reply_handler->Reply(OpenReply { .can_open = false });
}

void WorldObj::ContainerOpenedAction() {
	is_known = true;
}

void Container::OpenAction() {
	reply_handler->Reply(OpenReply { .can_open = true });

	for (auto obj : contents)
		obj->NameAction();

	// 2 separate loops to prevent weird "interlacing" issues
	for (auto obj : contents)
		obj->ContainerOpenedAction();
}
