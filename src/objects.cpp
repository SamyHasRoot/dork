#include "objects.h"
#include <ios>
#include <iterator>
#include <memory>
#include <sstream>

#include <iostream>
#include <streambuf>

WorldObj::WorldObj(ReplyHandler& reply_handler, bool is_known, std::string name, std::string description) : reply_handler(reply_handler) {
	this->reply_handler = reply_handler;
	this->is_known = is_known;
	this->description = description;
	this->names.push_back(name);
}

void WorldObj::LookAction() {
	reply_handler.Reply(LookReply { .description = description });
}

void WorldObj::OpenAction() {
	reply_handler.Reply(OpenReply { .can_open = false });
}

void WorldObj::ContainerOpenedAction() {
	is_known = true;

	std::ostringstream ss;
	ss << "A " << names[0] << ".";

	reply_handler.Reply(ContainerOpenedReply { .text = ss.str() });
}

void Container::OpenAction() {
	reply_handler.Reply(OpenReply { .can_open = true });

	for (auto obj : contents)
		obj->ContainerOpenedAction();
}

void WorldObj::PushAction() {
	reply_handler.Reply(PushReply { .can_push = false });
}

void Button::PushAction() {
	reply_handler.Reply(PushReply { .can_push = true });

	for (auto obj : connections)
		obj->ButtonPushedAction();
}

void WorldObj::TimeStepAction() {}
void WorldObj::ButtonPushedAction() {}

void Light::ButtonPushedAction() {
	state = !state;
	if (state)
		reply_handler.Reply(EventReply { .text = activate_text });
	else
		reply_handler.Reply(EventReply { .text = deactivate_text });
}

void StartButton::PushAction() {
	reply_handler.Reply(PushReply { .can_push = true });
	reply_handler.Reply(EventReply { .text = "A flash of white light emanates from the button, quickly covering the whole room." });
	reply_handler.Reply(EventReply { .text = "You start feeling dizzy..." });
	reply_handler.Reply(LoadRoomReply { .file = next_room });
}

//struct TestS {
//	int arr[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
//};
//void WorldObj::Save(std::ostringstream& buf) {
//	TestS st;
//	buf.write((char*)&st, sizeof(st));
//}
//void WorldObj::Load(char*& buf, uint32_t) {
//	std::cout << "buf: " << buf << "\n";
//	std::cout << ((TestS&)*buf).arr[0] << "\n";
//}
void WorldObj::Save(std::ostringstream& buf) {
	buf << "help";
}
void WorldObj::Load(char*&, uint32_t) {}

void Light::Save(std::ostringstream& buf) {
	buf.write((char*)&state, sizeof(bool));
}

void Light::Load(char*& buf, uint32_t) {
	state = ((bool&)*buf);
}
