#ifndef FILES_H
#define FILES_H

#include <fstream>
#include <functional>
#include <iostream>
#include <memory>

void WriteFile(std::ofstream& file, const char* data, uint32_t data_size);

//uint32_t ReadFile(std::ifstream& file, std::unique_ptr<char[]>&);

template <typename T>
uint32_t ReadFile(std::ifstream& file, std::unique_ptr<T, std::function<void(void*)>>& data) {
	uint32_t data_size;
	file.read((char*)&data_size, sizeof(uint32_t));

	if (file.eof() | (data_size == 0))
		return 0;

	char* raw_data = new char[data_size];
	file.read(raw_data, data_size);

	std::unique_ptr<T, std::function<void(void*)>> p((T*)raw_data, /*std::default_delete<char[]>()*/[](void* ptr){
		delete[] (T*)ptr;
	});
	data.swap(p);

	return data_size;
}

#endif
