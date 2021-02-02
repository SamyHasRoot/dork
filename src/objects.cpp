#include "objects.h"
#include <memory>
#include <sstream>

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

	std::ostringstream ss;
	ss << "A " << names[0] << ".";

	reply_handler->Reply(ContainerOpenedReply { .text = ss.str() });
}

void Container::OpenAction() {
	reply_handler->Reply(OpenReply { .can_open = true });

	for (auto obj : contents)
		obj->ContainerOpenedAction();
}

void WorldObj::PushAction() {
	reply_handler->Reply(PushReply { .can_push = false });
}

void Button::PushAction() {
	reply_handler->Reply(PushReply { .can_push = true });

	for (auto obj : connections)
		obj->ButtonPushedAction();
}

void WorldObj::TimeStepAction() {}
void WorldObj::ButtonPushedAction() {}

void Light::ButtonPushedAction() {
	reply_handler->Reply(EventReply { .text = activate_text });
}
