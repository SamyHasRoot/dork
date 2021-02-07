#ifndef OBJECTS_H
#define OBJECTS_H

#include <ios>
#include <sstream>
#include <streambuf>
#include <string>
#include <vector>
#include <memory>
#include "replies.h"

class WorldObj {
	public:
		WorldObj(ReplyHandler& reply_handler, bool is_known, std::string name, std::string description);
		ReplyHandler& reply_handler;
		// names the object can be referred to with
		std::vector<std::string> names;
		// whether or not object has been seen*
		bool is_known = true;
		// description of object; printed when Verb::look is used
		std::string description;
		// open object
		virtual void OpenAction();
		// called when container opens (if object in container)
		virtual void ContainerOpenedAction();

		virtual void LookAction();

		virtual void PushAction();
		virtual void ButtonPushedAction();

		virtual void TimeStepAction();

		virtual void Save(std::ostringstream& buf);
		virtual void Load(char*& buf, uint32_t size);
};

class Container: public WorldObj {
	public:
		using WorldObj::WorldObj;
		// open object
		virtual void OpenAction() override;

		std::vector<std::shared_ptr<WorldObj>> contents;
};

class Button: public WorldObj {
	public:
		using WorldObj::WorldObj;
		virtual void PushAction() override;

		std::vector<std::shared_ptr<WorldObj>> connections;
};

class Light: public WorldObj {
	public:
		using WorldObj::WorldObj;
		std::string activate_text;
		std::string deactivate_text;
		bool state = false;
		virtual void ButtonPushedAction() override;
		virtual void Save(std::ostringstream& buf) override;
		virtual void Load(char*& buf, uint32_t size) override;
};

class StartButton: public Button {
	public:
		using Button::Button;
		std::string next_room;
		virtual void PushAction() override;
};

#endif
