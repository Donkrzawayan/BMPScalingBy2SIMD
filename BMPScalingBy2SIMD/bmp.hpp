#ifndef BMP_HPP
#define BMP_HPP

#include <array>
#include <cstdint> //uint8_t, int32_t

struct BMP {
	std::array<char, 54> header;
	int32_t width, height;
	int32_t size;
	uint8_t *data;

	BMP(const char *filename) : data(nullptr) { read(filename); }
	void read(const char *filename);
	~BMP() { delete[]data; }
};

#endif // !BMP_HPP
