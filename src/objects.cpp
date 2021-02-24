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

BaseObject::BaseObject(ReplyHandler& reply_handler) : reply_handler(reply_handler) {
	this->reply_handler = reply_handler;
}

void BaseObject::LookAction() {
	reply_handler.Reply(LookReply { /*description: */ description });
}

void BaseObject::OpenAction() {
	reply_handler.Reply(OpenReply { /*can_open: */ false });
}

void BaseObject::ContainerOpenedAction() {
	is_known = true;

	std::ostringstream ss;
	ss << "A " << names[0] << ".";

	reply_handler.Reply(ContainerOpenedReply { /*text: */ ss.str() });
}

void BaseObject::PushAction() {
	reply_handler.Reply(PushReply { /*can_push: */ false });
}

void BaseObject::EnterAction() {
	reply_handler.Reply(EnterReply { /*can_enter: */ false });
}

void BaseObject::TimeStepAction() {}
void BaseObject::ButtonPushedAction() {}

void BaseObject::Save(std::ostringstream& buf) {
	// TODO
	buf << "help";
}
void BaseObject::Load(char*&, uint32_t) {}

#define ADD_STD if(key=="name"){names.push_back(value);return true;}\
	if(key=="description"){description=value;return true;}\
	if(key=="known"){is_known=(bool)std::stoi(value);return true;}
// i love STDs! /s
bool BaseObject::AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<BaseObject>>, Room::name_index_type&) {
	ADD_STD;
	return false;
}
#undef ADD_STD

#define ADD_CLONE(to) std::unique_ptr<BaseObject> to::Clone() {\
	return std::make_unique<to>(*this);\
}
ADD_CLONE(BaseObject);

#undef ADD_CLONE
