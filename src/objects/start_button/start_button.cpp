#include <locale>
#include <memory>
#include <string>
#include "start_button_export.h"
#include "../../objects.h"
#include "../util.h"
#include <iostream>
#include <vector>

class START_BUTTON_EXPORT StartButton : public BaseObject {
	public:
		using BaseObject::BaseObject;
		std::string next_room;
		virtual void PushAction() override;

		virtual bool AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<BaseObject>> objs, Room::name_index_type& name_index) override;
		std::unique_ptr<BaseObject> Clone() override;
};

void StartButton::PushAction() {
	reply_handler.Reply(PushReply { .can_push = true });
	reply_handler.Reply(EventReply { .text = "A flash of white light emanates from the button, quickly covering the whole room." });
	reply_handler.Reply(EventReply { .text = "You start feeling dizzy..." });
	reply_handler.Reply(LoadRoomReply { .file = next_room });
}

bool StartButton::AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<BaseObject>>, Room::name_index_type&) {
	ADD_STD;
	if (key == "next_room") {
		next_room = value;
		return true;
	}
	return false;
}

extern "C" {
	START_BUTTON_EXPORT BaseObject* create_object(ReplyHandler& rp) {
		return new StartButton(rp);
	}
	START_BUTTON_EXPORT void destroy_object(BaseObject* object) {
		delete object;
	}
}

