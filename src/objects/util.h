#define ADD_STD if(key=="name"){names.push_back(value);return true;}\
	if(key=="description"){description=value;return true;}\
	if(key=="known"){is_known=(bool)std::stoi(value);return true;}
