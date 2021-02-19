#include <locale>
#include <memory>
#include <string>
#include "button_export.h"
#include "../../objects.h"
#include "../util.h"
#include <iostream>
#include <vector>


class BUTTON_EXPORT Button : public BaseObject {
	public:
		using BaseObject::BaseObject;
		virtual void PushAction() override;

		std::vector<std::shared_ptr<BaseObject>> connections;

		virtual bool AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<BaseObject>> objs, Room::name_index_type& name_index) override;
		std::unique_ptr<BaseObject> Clone() override;
};

void Button::PushAction() {
	reply_handler.Reply(PushReply { .can_push = true });

	for (auto obj : connections)
		obj->ButtonPushedAction();
}

bool Button::AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<BaseObject>> objs, Room::name_index_type& name_index) {
	ADD_STD;
	if (key == "connections") {
		connections.push_back(objs[name_index[value]]);
		return true;
	}
	return false;
}

extern "C" {
	BUTTON_EXPORT BaseObject* create_object(ReplyHandler& rp) {
		return new Button(rp);
	}
	BUTTON_EXPORT void destroy_object(BaseObject* object) {
		delete object;
	}
}

