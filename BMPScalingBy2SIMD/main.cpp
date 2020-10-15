//
#include <iostream>
#include <Windows.h>
#include <cstring> //strcmp
#include <sstream>
#include "bmp.hpp"
#include <thread>
#include <queue>

//typedef int(_fastcall *MyProc1)(int, int);
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
	int32_t perThread = height / N;
	int32_t firstLoop = height % N;

	std::queue<std::thread> threads;
	//auto threadCall = [&](uint8_t * destBegin, uint8_t *sourceBegin, size_t stop) {
	//	//threads.push(std::move(std::thread(scaler, destBegin, sourceBegin, stop)));
	//};

	//unsigned iter = 0;
	for (int i = 0; i < firstLoop; ++i) {
		//perThread+1
		int32_t size = width * perThread + width;
		threads.push(std::move(std::thread(scaler, destination, source, width, size)));
		scaler(destination, source, width, size);
		destination += 2*size;
		source += size;
	}
	for (unsigned i = 0; i < N - firstLoop; ++i) {
		//perThread
		int32_t size = width * perThread;
		threads.push(std::move(std::thread(scaler, destination, source, width, size)));
		scaler(destination, source, width, size);
		destination += 2 * size;
		source += size;
	}
	//for (unsigned i = 0; i < N - firstLoop; ++i)
		//threadCall();

	//threads.push(std::move(std::thread(scaler, destination, source, width, height)));


	while (!threads.empty()) {
		threads.front().join();
		threads.pop();
	}
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
	/*for (int i = 0; i < 3 * source.width*source.height; i += 3)
	{
		std::cout << "Niebieski: " << (int)source.data[i] << " "
			<< "Zielony: " << (int)source.data[i + 1] << " "
			<< "Czerwony: " << (int)source.data[i + 2]
			<< std::endl;
	}*/
	for (int i = 0; i < dest.size; ++i)
		dest.data[i] = 2;
	//std::cout << (void*)(&dest.data[dest.size-1]) << "\n";

	//multithreating(thread, dest.data, source.data, source.width, source.height);
	//scaler(dest.data, source.data, source.width, source.height*source.width);

	dest.write(destName);

	std::cout << "\nHello World!\n";
}

