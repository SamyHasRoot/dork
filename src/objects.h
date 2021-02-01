#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <vector>
#include <memory>
#include "replies.h"

class WorldObj {
	public:
		WorldObj(std::shared_ptr<ReplyHandler> reply_handler, bool is_known, std::string name, std::string description);
		std::shared_ptr<ReplyHandler> reply_handler;
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

		virtual void NameAction();
		virtual void DescribeAction();
};

class Container: public WorldObj {
	public:
		using WorldObj::WorldObj;
		// open object
		void OpenAction() override;

		std::vector<std::shared_ptr<WorldObj>> contents;
};

#endif
