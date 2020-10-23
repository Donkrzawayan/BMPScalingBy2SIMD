﻿//
#include <iostream>
#include <Windows.h>
#include <cstring> //strcmp
#include <sstream>
#include "bmp.hpp"
#include <thread>
#include <queue>
#include <chrono>

typedef uint8_t *(_fastcall *MyProc1)(uint8_t *, uint8_t *, int32_t, unsigned);

void scalerASM(uint8_t *destination, uint8_t *source, int32_t width, unsigned size) {
	HINSTANCE dllHandle = NULL;
	dllHandle = LoadLibrary(L"DllMasm.dll");
	MyProc1 procedura = (MyProc1)GetProcAddress(dllHandle, "MyProc1");
	procedura(destination, source, width, size);
}

void scalerC(uint8_t *destination, uint8_t *source, int32_t width, unsigned size) {
	HINSTANCE dllHandle = NULL;
	dllHandle = LoadLibrary(L"DllC.dll");
	MyProc1 fun = (MyProc1)GetProcAddress(dllHandle, "fun");
	fun(destination, source, width, size);
}

void multithreating(const unsigned N, uint8_t *dest, uint8_t *src, const int32_t width, const int32_t height) {
	const int32_t subpxWidth = 3 * width;
	const int32_t rowsPerThread = height / N;
	const int32_t firstLoop = height % N;

	//auto start = std::chrono::high_resolution_clock::now();

	std::queue<std::thread> threads;

	//rowsPerThread+1
	int32_t size = subpxWidth * rowsPerThread + subpxWidth;
	for (int i = 0; i < firstLoop; ++i, dest += 4 * size, src += size)
		threads.push(std::move(std::thread(scalerASM, dest, src, subpxWidth, size)));

	//rowsPerThread
	size = subpxWidth * rowsPerThread;
	for (unsigned i = 0; i < N - firstLoop; ++i, dest += 4 * size, src += size)
		threads.push(std::move(std::thread(scalerASM, dest, src, subpxWidth, size)));
	//scaler(dest, src, subpxWidth, size);

	while (!threads.empty()) {
		threads.front().join();
		threads.pop();
	}

	//auto stop = std::chrono::high_resolution_clock::now();
	//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	//std::cout << duration.count() << "\n";
}

//TODO: delete later
void initializeArray(uint8_t *data, int32_t size) {
	for (int i = 0; i < size; ++i)
		data[i] = static_cast<uint8_t>(2);
}

int main(const int argc, char *argv[])
{
	const char *sourceName = "";
	const char *destName = "";
	unsigned thread = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1U;
	for (int i = 1; i < argc; ++i) {
		if (!strcmp("-s", argv[i]))
			sourceName = argv[++i];
		else if (!strcmp("-d", argv[i]))
			destName = argv[++i];
		else if (!strcmp("-t", argv[i]) && strcmp("0", argv[++i])) {//if user choose 0 (default) set yourself
			std::stringstream number;
			number << argv[i];
			number >> thread;
		}
	}

	BMP source(sourceName);
	BMP dest(source, 2 * source.width, 2 * source.height);
	initializeArray(dest.data, dest.size);


	//for (int i = 0; i < 20; ++i)
	multithreating(thread, dest.data, source.data, source.width, source.height);


	dest.write(destName);
}
