#ifndef BMP_HPP
#define BMP_HPP

#include <array>
#include <cstdint> //uint8_t, int32_t

struct BMP {
private:
	enum : size_t { FILE_SIZE = 2, WIDTH = 18, HEIGHT = 22, HEADER_SIZE = 54 };

public:
	std::array<char, HEADER_SIZE> header;
	int32_t width, height;
	int32_t rowPadded;
	int32_t size;
	uint8_t *data;

	BMP(const char *filename) : data(nullptr) { read(filename); }
	BMP(const BMP &source, const int32_t width, const int32_t height);
	~BMP() { delete[]data; }
	void read(const char *filename);
	void write(const char *filename)const;
};

#endif // !BMP_HPP
