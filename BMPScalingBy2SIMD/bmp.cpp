#include "bmp.hpp"
#include <fstream>
#include <iostream>
#include <string>

BMP::BMP(const BMP & source, const int32_t width, const int32_t height)
	: header(source.header),
	width(width), height(height), rowPadded(3 * width + width % 4), size(rowPadded * height),
	data(new uint8_t[size])
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
	while (!input) {
		std::cerr << "Cant read file.\n";
		std::cout << "Wprowadz poprawna nazwe pliku: ";
		std::string temp;
		std::cin >> temp;
		input.open(temp.c_str(), std::ios::binary | std::ios::in);
	}

	//clear old data
	delete[]data;

	// read the 54-byte header
	input.read(header.data(), header.size());

	// extract image height and width from header
	width = *(int32_t *)&header[WIDTH];
	height = *(int32_t *)&header[HEIGHT];
	
	//padding
	rowPadded = 3 * width + width % 4;

	// allocate 3 bytes per pixel
	size = rowPadded * height;

	if (size >= INT32_MAX / 4) {
		std::cerr << "Too big file.\n";
		std::cout << "Wprowadz inna nazwe pliku: ";
		std::string temp;
		std::cin >> temp;
		return read(temp.c_str());
	}
	
	data = new uint8_t[size];

	// read the rest of the data at once
	input.read((char *)data, sizeof(uint8_t) * size);
	input.close();
}

void BMP::write(const char * filename)const
{
	std::ofstream output(filename, std::ios::binary | std::ios::out);
	while (!output) {
		std::cerr << "Cant write file.\n";
		std::cout << "Wprowadz poprawna nazwe pliku: ";
		std::string temp;
		std::cin >> temp;
		output.open(temp.c_str(), std::ios::binary | std::ios::in);
	}

	output.write(header.data(), header.size());
	output.write((char *)data, sizeof(uint8_t) * size);
	output.close();
}
