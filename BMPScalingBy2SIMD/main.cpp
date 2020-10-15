//
//#include <iostream>
#include <Windows.h>
#include <cstring> //strcmp
#include <sstream>
#include "bmp.hpp"
#include <thread>
#include <queue>

//typedef uint8_t *(_fastcall *MyProc1)(uint8_t *, uint8_t *, int32_t, int32_t);
typedef uint8_t *(_fastcall *MyProc1)(uint8_t *, uint8_t *, int32_t, unsigned);

void scaler(uint8_t *destination, uint8_t *source, int32_t width, unsigned size) {
	/*HINSTANCE dllHandle = NULL;
	dllHandle = LoadLibrary(L"DllMasm.dll");
	MyProc1 procedura = (MyProc1)GetProcAddress(dllHandle, "MyProc1");
	uint8_t * retVal = procedura(destination, source, width, height);*/
	HINSTANCE dllHandle = NULL;
	dllHandle = LoadLibrary(L"DllC.dll");
	MyProc1 fun = (MyProc1)GetProcAddress(dllHandle, "fun");
	fun(destination, source, width, size);
}

void multithreating(const unsigned N, uint8_t *destination, uint8_t *source, const int32_t width, const int32_t height) {
	const int32_t subpixelWidth = 3 * width;
	const int32_t rowsPerThread = height / N;
	const int32_t firstLoop = height % N;
	std::queue<std::thread> threads;

	//rowsPerThread+1
	int32_t size = subpixelWidth * rowsPerThread + subpixelWidth;
	for (int i = 0; i < firstLoop; ++i, destination += 4 * size, source += size)
		threads.push(std::move(std::thread(scaler, destination, source, subpixelWidth, size)));

	//rowsPerThread
	size = subpixelWidth * rowsPerThread;
	for (unsigned i = 0; i < N - firstLoop; ++i, destination += 4 * size, source += size)
		threads.push(std::move(std::thread(scaler, destination, source, subpixelWidth, size)));

	while (!threads.empty()) {
		threads.front().join();
		threads.pop();
	}
}

//TODO: delete later
void initializeArray(uint8_t *data, int32_t size) {
	for (int i = 0; i < size; ++i)
		data[i] = static_cast<uint8_t>(2);
}

int main(int argc, char *argv[])
{
	const char *sourceName = "";
	const char *destName = "";
	unsigned thread = 0U;
	for (int i = 1; i < argc; ++i) {
		if (!strcmp("-s", argv[i]))
			sourceName = argv[++i];
		else if (!strcmp("-d", argv[i]))
			destName = argv[++i];
		else if (!strcmp("-t", argv[i])) {
			std::stringstream number;
			number << argv[++i];
			number >> thread;
		}
	}

	BMP source(sourceName);
	BMP dest(source, 2 * source.width, 2 * source.height);
	//initializeArray(dest.data, dest.size);

	multithreating(thread, dest.data, source.data, source.width, source.height);

	dest.write(destName);
}
