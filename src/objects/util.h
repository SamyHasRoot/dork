#define ADD_STD if(key=="name"){names.push_back(value);return true;}\
	if(key=="description"){description=value;return true;}\
	if(key=="known"){is_known=(bool)std::stoi(value);return true;}

#define ADD_CLONE(to) std::unique_ptr<BaseObject> to::Clone() {\
	return std::make_unique<to>(*this);\
}
