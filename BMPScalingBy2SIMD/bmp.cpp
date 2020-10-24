#include "bmp.hpp"
#include <fstream>
#include <iostream>

BMP::BMP(const BMP & source, const int32_t width, const int32_t height)
	: header(source.header), width(width), height(height), size(3 * width * height), data(new uint8_t[size])
{
	uint32_t file_size = HEADER_SIZE + size;
	*(int32_t *)&header[FILE_SIZE] = file_size;

	// write image height and width to header
	*(int32_t *)&header[WIDTH] = width;
	*(int32_t *)&header[HEIGHT] = height;
}

void BMP::read(const char * filename)
{
	std::ifstream input(filename, std::ios::binary | std::ios::in);
	if (!input) {
		std::cerr << "Cant read file.\n";
		return;
	}

	//clear old data
	if (data)
		delete[]data;

	// read the 54-byte header
	input.read(header.data(), header.size());

	// extract image height and width from header
	width = *(int32_t *)&header[WIDTH];
	height = *(int32_t *)&header[HEIGHT];

	// allocate 3 bytes per pixel
	size = 3 * width * height;
	if (size >= INT32_MAX / 4) {
		std::cerr << "Too big file.\n";
		return;
	}
	data = new uint8_t[size];

	// read the rest of the data at once
	input.read((char *)data, sizeof(uint8_t) * size);
	input.close();
}

void BMP::write(const char * filename)const
{
	std::ofstream output(filename, std::ios::binary | std::ios::out);
	if (!output) {
		std::cerr << "Cant write file.\n";
		return;
	}

	output.write(header.data(), header.size());
	output.write((char *)data, sizeof(uint8_t) * size);
	output.close();
}
