#ifndef OBJECTS_H
#define OBJECTS_H

#include <ios>
#include <sstream>
#include <streambuf>
#include <string>
#include <vector>
#include <memory>
#include "replies.h"
#include "room.h"

class WorldObj {
	public:
		WorldObj(ReplyHandler&);
		// ^- tmp
		WorldObj(ReplyHandler& reply_handler, bool is_known, const std::string& name, const std::string& description);
		ReplyHandler& reply_handler;
		// names the object can be referred to with
		std::vector<std::string> names;
		// whether or not object has been seen*
		bool is_known = true;
		std::string description;
		// try to open object
		virtual void OpenAction();
		// called when container opens (if object in container)
		virtual void ContainerOpenedAction();

		virtual void LookAction();

		virtual void PushAction();
		virtual void ButtonPushedAction();

		virtual void EnterAction();

		virtual void TimeStepAction();


		virtual void Save(std::ostringstream& buf);
		virtual void Load(char*& buf, uint32_t size);

		virtual bool AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<WorldObj>> objs, Room::name_index_type& name_index);
		virtual std::unique_ptr<WorldObj> clone();
};

class Container: public WorldObj {
	public:
		using WorldObj::WorldObj;
		// open object
		virtual void OpenAction() override;

		std::vector<std::shared_ptr<WorldObj>> contents;

		virtual bool AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<WorldObj>> objs, Room::name_index_type& name_index) override;
		std::unique_ptr<WorldObj> clone() override;
};

class Button: public WorldObj {
	public:
		using WorldObj::WorldObj;
		virtual void PushAction() override;

		std::vector<std::shared_ptr<WorldObj>> connections;

		virtual bool AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<WorldObj>> objs, Room::name_index_type& name_index) override;
		std::unique_ptr<WorldObj> clone() override;
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

		virtual bool AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<WorldObj>> objs, Room::name_index_type& name_index) override;
		std::unique_ptr<WorldObj> clone() override;
};

class StartButton: public Button {
	public:
		using Button::Button;
		std::string next_room;
		virtual void PushAction() override;

		virtual bool AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<WorldObj>> objs, Room::name_index_type& name_index) override;
		std::unique_ptr<WorldObj> clone() override;
};

class Door: public WorldObj {
	public:
		using WorldObj::WorldObj;
		std::string next_room;
		virtual void EnterAction() override;

		virtual bool AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<WorldObj>> objs, Room::name_index_type& name_index) override;
		std::unique_ptr<WorldObj> clone() override;
};

#endif
