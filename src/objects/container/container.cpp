#include <locale>
#include <memory>
#include <string>
#include "container_export.h"
#include "../../objects.h"
#include "../../objects.cpp"
#include "../util.h"
#include <iostream>
#include <vector>

class CONTAINER_EXPORT Container : public BaseObject {
	public:
		using BaseObject::BaseObject;
		// open object
		virtual void OpenAction() override;

		std::vector<std::shared_ptr<BaseObject>> contents;

		bool AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<BaseObject>> objs, Room::name_index_type& name_index) override;
		std::unique_ptr<BaseObject> Clone() override;
};

ADD_CLONE(Container);

void Container::OpenAction() {
	reply_handler.Reply(OpenReply { /*can_open: */ true });

	for (auto obj : contents)
		obj->ContainerOpenedAction();
}
bool Container::AddProperty(std::string& key, std::string& value, std::vector<std::shared_ptr<BaseObject>> objs, Room::name_index_type& name_index) {
	ADD_STD;
	if (key == "contents") {
		contents.push_back(objs[name_index[value]]);
		return true;
	}
	return false;
}

extern "C" CONTAINER_EXPORT BaseObject* create_object(ReplyHandler& rp) {
	return new Container(rp);
}

extern "C" CONTAINER_EXPORT void destroy_object(BaseObject* object) {
	delete object;
}
