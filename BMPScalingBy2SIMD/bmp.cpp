#include "bmp.hpp"
#include <fstream>

void BMP::read(const char * filename)
{
	std::fstream input(filename, std::ios::binary | std::ios::in);
	if (!input) { return; }

	//clear old data
	if (data)
		delete[]data;

	// read the 54-byte header
	input.read(header.data(), header.size());

	// extract image height and width from header
	width = *(int32_t *)&header[18];
	height = *(int32_t *)&header[22];

	// allocate 3 bytes per pixel
	size = 3 * width * height;
	data = new uint8_t[size];

	// read the rest of the data at once
	input.read((char *)data, sizeof(uint8_t) * size);
	input.close();
}
