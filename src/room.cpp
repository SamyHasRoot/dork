#include "room.h"
#include <fstream>
#include <map>
#include <memory>
#include <string>

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
	{
		std::string line;
		while (std::getline(file, line)) {
			if (line == "EOF")
				break;
			description.append(line);
			description.append("\n");
		}
		description.pop_back();
	}

	// load objs
	std::map<std::string, unsigned int> name_index;
	int line_i = 0;
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
			std::string name;
			ReadWord(line, type_end+2, name);

			// add to map
			name_index[name] = objs.size();

			// add to room
			if (type == "obj")
				objs.push_back(std::make_shared<WorldObj>(reply_handler, true, name, ""));
			else if (type == "container")
				objs.push_back(std::make_shared<Container>(reply_handler, true, name, ""));
			else if (type == "button")
				objs.push_back(std::make_shared<Button>(reply_handler, true, name, ""));
			else if (type == "light")
				objs.push_back(std::make_shared<Light>(reply_handler, true, name, ""));
			else if (type == "start_button")
				objs.push_back(std::make_shared<StartButton>(reply_handler, true, name, ""));
			else
				throw line_i;
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
			if (property == "name")
				objs.back()->names.push_back(value);
			else if (property == "description")
				objs.back()->description = value;
			else if (property == "known")
				objs.back()->is_known = (bool)std::stoi(value);
			else if (property == "activate_text")
				// TODO: room.objs.back() *must* be a Light
				std::dynamic_pointer_cast<Light>(objs.back())->activate_text = value;
			else if (property == "deactivate_text")
				// TODO: room.objs.back() *must* be a Light
				std::dynamic_pointer_cast<Light>(objs.back())->deactivate_text = value;
			else if (property == "content")
				// TODO: room.objs.back() *must* be a Container
				std::dynamic_pointer_cast<Container>(objs.back())->contents.push_back(objs[name_index[value]]);
			else if (property == "connections")
				// TODO: room.objs.back() *must* be a Button
				std::dynamic_pointer_cast<Button>(objs.back())->connections.push_back(objs[name_index[value]]);
			else if (property == "next_room")
				// TODO: room.objs.back() *must* be a StartButton
				std::dynamic_pointer_cast<StartButton>(objs.back())->next_room = value;
			else
				throw line_i;
		}
	}
}
