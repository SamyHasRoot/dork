#ifndef FILE_PATHS_H
#define FILE_PATHS_H

namespace file_paths {
#ifdef _WINDOWS
	constexpr const char* kDL_PREFIX = "";
	constexpr const char* kDL_SUFFIX = ".dll";
#else
	constexpr const char* kDL_PREFIX = "lib";
	constexpr const char* kDL_SUFFIX = ".so";
#endif
	constexpr const char* kSAVE_DIR = "saves";
	constexpr const char* kWORLD_DIR = "worlds";
	constexpr const char* kOBJECT_DIR = "objects";
}

#endif
