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

class BaseObject {
	public:
		BaseObject(ReplyHandler& reply_handler);
		virtual ~BaseObject() {}
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

		virtual bool AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<BaseObject>> objs, Room::name_index_type& name_index);
		virtual std::unique_ptr<BaseObject> Clone();
};

#endif
