#ifndef OBJECTS_H
#define OBJECTS_H

#include <iostream>
#include <vector>
#include <memory>

class WorldObj {
	public:
		WorldObj(bool is_known, std::string name, std::string description);
		// names the object can be referred to with
		std::vector<std::string> names;
		// whether or not object has been seen*
		bool is_known = true;
		// description of object; printed when Verb::look is used
		std::string description;
		// determine whether object can be opened (you can't)
		virtual bool open();
		// called when container opens (if object in container)
		virtual void OnContainerOpen();
};

class Container: public WorldObj {
	public:
		using WorldObj::WorldObj;
		// open container; calls OnContainerOpen for all objects in contents
		bool open() override;

		std::vector<std::shared_ptr<WorldObj>> contents;
};

#endif
