#include "room.h"
#include "objects.h"
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>

int Room::SearchFor(std::string& name) {
	for (unsigned int obj_i = 0; obj_i < this->objs.size(); obj_i++) {
		if (this->objs[obj_i]->is_known) {
			for (std::string obj_name : this->objs[obj_i]->names) {
				if (obj_name == name) {
					return obj_i;
				}
			}
		}
	}
	return -1;
}

unsigned int ReadWord(std::string& input, int start, std::string& out) {
	unsigned int i = start;
	while (input.at(i) != ' ') {
		out.push_back(input.at(i));
		i++;
		if (i == input.length())
			break;
	}
	return i-1;
}

Room::Room() {};

Room::Room(ReplyHandler& reply_handler, std::ifstream& file) {
	// get room description
	int line_i = 0;
	{
		std::string line;
		while (std::getline(file, line)) {
			line_i++;
			if (line == "EOF")
				break;
			description.append(line);
			description.append("\n");
		}
		description.pop_back();
	}

	// TODO
	//auto o = std::vector<std::shared_ptr<WorldObj>>();
	//o.push_back(std::make_shared<WorldObj>(reply_handler));
	//o.push_back(std::make_shared<Container>(reply_handler));
	//o.push_back(std::make_shared<Button>(reply_handler));
	//o.push_back(std::make_shared<Light>(reply_handler));
	//o.push_back(std::make_shared<StartButton>(reply_handler));
	auto m = std::map<std::string, std::unique_ptr<WorldObj>>();
	m["obj"] = std::make_unique<WorldObj>(reply_handler);
	m["container"] = std::make_unique<Container>(reply_handler);
	m["button"] = std::make_unique<Button>(reply_handler);
	m["light"] = std::make_unique<Light>(reply_handler);
	m["start_button"] = std::make_unique<StartButton>(reply_handler);
	m["door"] = std::make_unique<Door>(reply_handler);

	// load objs
	name_index_type name_index;
	std::string line;
	while (std::getline(file, line)) {
		line_i++;
		if (line.length() == 0)
			continue;
		if (line.at(0) != '\t') {
			// add new object to room

			// read object type
			std::string type;
			unsigned int type_end = ReadWord(line, 0, type);

			// check length
			if (line.length() <= type_end+2)
				throw line_i;

			// read name
			std::string name = line.substr(type_end+2, line.length()-1);

			// add to map
			name_index[name] = objs.size();

			// add to room
			objs.push_back(m[type]->clone());
			objs.back()->names.push_back(name);
		} else {
			// add properties to last added object

			// can't expand properties of noting
			if (objs.size() == 0)
				throw line_i;

			// read property to add
			std::string property;
			unsigned int property_end = ReadWord(line, 1, property);

			// TODO: check length
			// check length
			if (line.length() <= property_end+2)
				throw line_i;

			// read value of property to add
			std::string value = line.substr(property_end+2, line.length()-1);

			// add property
			if (!objs.back()->AddProperty(property, value, objs, name_index))
				throw line_i;
		}
	}
}
