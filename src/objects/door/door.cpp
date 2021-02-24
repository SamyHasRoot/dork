#include <memory>
#include <string>
#include "door_export.h"
#include "../../objects.h"
#include "../../objects.cpp"
#include "../util.h"
#include <vector>

class DOOR_EXPORT Door : public BaseObject {
	public:
		using BaseObject::BaseObject;
		std::string next_room;
		virtual void EnterAction() override;

		virtual bool AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<BaseObject>> objs, Room::name_index_type& name_index) override;
		std::unique_ptr<BaseObject> Clone() override;
};

ADD_CLONE(Door);

void Door::EnterAction() {
	reply_handler.Reply(EnterReply { /*can_enter: */ true });
	reply_handler.Reply(LoadRoomReply { /*file: */ next_room });
}
bool Door::AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<BaseObject>>, Room::name_index_type&) {
	ADD_STD;
	if (key == "next_room") {
		next_room = value;
		return true;
	}
	return false;
}

extern "C" {
	DOOR_EXPORT BaseObject* create_object(ReplyHandler& rp) {
		return new Door(rp);
	}
	DOOR_EXPORT void destroy_object(BaseObject* object) {
		delete object;
	}
}
