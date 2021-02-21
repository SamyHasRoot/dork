#include <memory>
#include <string>
#include "light_export.h"
#include "../../objects.h"
#include "../../objects.cpp"
#include "../util.h"
#include <iostream>
#include <vector>

class LIGHT_EXPORT Light : public BaseObject {
	public:
		using BaseObject::BaseObject;
		std::string activate_text;
		std::string deactivate_text;
		bool state = false;
		virtual void ButtonPushedAction() override;
		virtual void Save(std::ostringstream& buf) override;
		virtual void Load(char*& buf, uint32_t size) override;

		virtual bool AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<BaseObject>> objs, Room::name_index_type& name_index) override;
		std::unique_ptr<BaseObject> Clone() override;
};

ADD_CLONE(Light);

void Light::ButtonPushedAction() {
	state = !state;
	if (state)
		reply_handler.Reply(EventReply { .text = activate_text });
	else
		reply_handler.Reply(EventReply { .text = deactivate_text });
}

void Light::Save(std::ostringstream& buf) {
	buf.write((char*)&state, sizeof(bool));
}

void Light::Load(char*& buf, uint32_t) {
	state = ((bool&)*buf);
}

bool Light::AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<BaseObject>>, Room::name_index_type&) {
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

extern "C" {
	LIGHT_EXPORT BaseObject* create_object(ReplyHandler& rp) {
		return new Light(rp);
	}
	LIGHT_EXPORT void destroy_object(BaseObject* object) {
		delete object;
	}
}
