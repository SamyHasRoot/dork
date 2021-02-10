#include "objects.h"
#include "replies.h"
#include "room.h"
#include <cstdio>
#include <ios>
#include <iterator>
#include <memory>
#include <sstream>

#include <iostream>
#include <streambuf>
#include <string>

WorldObj::WorldObj(ReplyHandler& reply_handler, bool is_known, const std::string &name, const std::string &description) : reply_handler(reply_handler) {
	this->reply_handler = reply_handler;
	this->is_known = is_known;
	this->description = description;
	this->names.push_back(name);
}

WorldObj::WorldObj(ReplyHandler& reply_handler) : reply_handler(reply_handler) {
	this->reply_handler = reply_handler;
	this->is_known = true;
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

void WorldObj::EnterAction() {
	reply_handler.Reply(EnterReply { .can_enter = false });
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

void Door::EnterAction() {
	reply_handler.Reply(EnterReply { .can_enter = true });
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

//#define NAME(obj, name) const std::string& obj::GetTypeName() {static const std::string n=#name;return n;}
//NAME(WorldObj, obj);
//NAME(Container, container);
//NAME(Button, button);
//NAME(Light, light);
//NAME(StartButton, start_button);
//#undef NAME
#define ADD_STD if(key=="name"){names.push_back(value);return true;}\
	if(key=="description"){description=value;return true;}\
	if(key=="known"){is_known=(bool)std::stoi(value);return true;}

// i love STDs! /s
bool WorldObj::AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<WorldObj>>, Room::name_index_type&) {
	ADD_STD;
	return false;
}
bool Container::AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<WorldObj>> objs, Room::name_index_type& name_index) {
	ADD_STD;
	if (key == "contents") {
		contents.push_back(objs[name_index[value]]);
		return true;
	}
	return false;
}
bool Button::AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<WorldObj>> objs, Room::name_index_type& name_index) {
	ADD_STD;
	if (key == "connections") {
		connections.push_back(objs[name_index[value]]);
		return true;
	}
	return false;
}
bool Light::AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<WorldObj>>, Room::name_index_type&) {
	ADD_STD;
	if (key == "activate_text") {
		activate_text = value;
		return true;
	}
	if (key == "deactivate_text") {
		deactivate_text = value;
		return true;
	}
	return false;
}
bool StartButton::AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<WorldObj>>, Room::name_index_type&) {
	ADD_STD;
	if (key == "next_room") {
		next_room = value;
		return true;
	}
	return false;
}
bool Door::AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<WorldObj>>, Room::name_index_type&) {
	ADD_STD;
	if (key == "next_room") {
		next_room = value;
		return true;
	}
	return false;
}
#undef ADD_STD

#define ADD_CLONE(to) std::unique_ptr<WorldObj> to::clone() {\
	return std::make_unique<to>(*this);\
}
ADD_CLONE(WorldObj);
ADD_CLONE(Container);
ADD_CLONE(Button);
ADD_CLONE(Light);
ADD_CLONE(StartButton);
ADD_CLONE(Door);
#undef ADD_CLONE
