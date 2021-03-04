#include "files.h"
#include <iostream>

void WriteFile(std::ofstream& file, const char* data, uint32_t data_size) {
	file.write((char*)&data_size, sizeof(uint32_t));
	file.write(data, data_size);
}
