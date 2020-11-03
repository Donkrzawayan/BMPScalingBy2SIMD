//Skalowanie obrazu BMP przez 2
//5 sem. 2020/2021
#include <iostream>
#include <Windows.h>
#include <cstring> //strcmp
#include <sstream>
#include "bmp.hpp"
#include <thread>
#include <queue>
#include <chrono>
#include <intrin.h> // cpuinfo

typedef uint8_t *(_fastcall *MyProc1)(uint8_t *, uint8_t *, int32_t, unsigned);

void multithreatingASM(const unsigned N, uint8_t *dest, uint8_t *src, const int32_t width, const int32_t height,
	const int32_t srcRowPadding, const int32_t destRowPadding)
{
	const int32_t subpxWidth = 3 * width;
	const int32_t rowsPerThread = height / N;
	const int32_t firstLoop = height % N;

	HINSTANCE dllHandle = NULL;
	dllHandle = LoadLibrary(L"DllMasm.dll");
	MyProc1 procedura = (MyProc1)GetProcAddress(dllHandle, "MyProc1");
	auto scaler = [&](uint8_t *destination, uint8_t *source, int32_t width, unsigned size) {
		procedura(destination, source, width, size);
	};

	auto start = std::chrono::high_resolution_clock::now();

	std::queue<std::thread> threads;

	//rowsPerThread+1
	int32_t srcSize = srcRowPadding * rowsPerThread + srcRowPadding;
	int32_t destSize = 2 * (destRowPadding * rowsPerThread + destRowPadding);
	for (int i = 0; i < firstLoop; ++i, dest += destSize, src += srcSize)
		threads.push(std::move(std::thread(scaler, dest, src, subpxWidth, srcSize)));

	//rowsPerThread
	srcSize = srcRowPadding * rowsPerThread;
	destSize = 2 * destRowPadding * rowsPerThread;
	for (unsigned i = 0; i < N - firstLoop; ++i, dest += destSize, src += srcSize)
		threads.push(std::move(std::thread(scaler, dest, src, subpxWidth, srcSize)));

	while (!threads.empty()) {
		threads.front().join();
		threads.pop();
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Czas: " << duration.count() << " ms \n";

	FreeLibrary(dllHandle);
}

void multithreatingC(const unsigned N, uint8_t *dest, uint8_t *src, const int32_t width, const int32_t height,
	const int32_t srcRowPadding, const int32_t destRowPadding)
{
	const int32_t subpxWidth = 3 * width;
	const int32_t rowsPerThread = height / N;
	const int32_t firstLoop = height % N;

	HINSTANCE dllHandle = NULL;
	dllHandle = LoadLibrary(L"DllC.dll");
	MyProc1 fun = (MyProc1)GetProcAddress(dllHandle, "fun");
	auto scaler = [&](uint8_t *destination, uint8_t *source, int32_t width, unsigned size) {
		fun(destination, source, width, size);
	};

	auto start = std::chrono::high_resolution_clock::now();

	std::queue<std::thread> threads;

	//rowsPerThread+1
	int32_t srcSize = srcRowPadding * rowsPerThread + srcRowPadding;
	int32_t destSize = 2 * (destRowPadding * rowsPerThread + destRowPadding);
	for (int i = 0; i < firstLoop; ++i, dest += destSize, src += srcSize)
		threads.push(std::move(std::thread(scaler, dest, src, subpxWidth, srcSize)));

	//rowsPerThread
	srcSize = srcRowPadding * rowsPerThread;
	destSize = 2 * destRowPadding * rowsPerThread;
	for (unsigned i = 0; i < N - firstLoop; ++i, dest += destSize, src += srcSize)
		threads.push(std::move(std::thread(scaler, dest, src, subpxWidth, srcSize)));

	while (!threads.empty()) {
		threads.front().join();
		threads.pop();
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Czas: " << duration.count() << " ms \n";

	FreeLibrary(dllHandle);
}

//TODO: delete later
void initializeArray(uint8_t *data, int32_t size) {
	for (int i = 0; i < size; ++i)
		data[i] = static_cast<uint8_t>(2);
}

int main(const int argc, char *argv[])
{
	std::string sourceName = "i.bmp";
	std::string destName = "o.bmp";
	unsigned thread = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1U;
	for (int i = 1; i < argc; ++i) {
		if (!strcmp("-s", argv[i]))
			sourceName = argv[++i];
		else if (!strcmp("-d", argv[i]))
			destName = argv[++i];
		else if (!strcmp("-t", argv[i++]) && argv[i][0] != '0' && argv[i][0] != '-') {
			//if user choose <1 set yourself
			std::stringstream number;
			number << argv[i];
			number >> thread;
		}
	}

	int cpuinfo[4];
	bool avx2Supportted = false;
	__cpuid(cpuinfo, 7);
	avx2Supportted = cpuinfo[1] & (1 << 5) || false;

	std::cout << "Program do skalowania BPM przez 2\n";
	std::cout << "AVX2 " << (avx2Supportted ? "" : "nie ") << "jest wspierany.\n";
	int x = 0;
	while (x != 5) {
		std::cout << "1. Zmien plik zrodlowy.\n"
			<< "2. Zmien plik docelowy.\n"
			<< "3. C\n"
			<< (avx2Supportted ? "4" : " ") << ". ASM\n"
			<< "5. Zakoncz.\n"
			<< "Wybierz: ";
		std::cin >> x;
		std::cin.get();
		switch (x) {
		case 1: {
			std::cout << "Nowy plik: ";
			std::cin >> sourceName;
			break;
		}
		case 2: {
			std::cout << "Nowy plik: ";
			std::cin >> destName;
			break;
		}
		case 3: {
			BMP source(sourceName.c_str());
			BMP dest(source, 2 * source.width, 2 * source.height);
			multithreatingC(thread, dest.data, source.data,
				source.width, source.height, source.rowPadded, dest.rowPadded);

			dest.write(destName.c_str());
			break;
		}
		case 4: {
			if (avx2Supportted) {
				BMP source(sourceName.c_str());
				BMP dest(source, 2 * source.width, 2 * source.height);
				multithreatingASM(thread, dest.data, source.data,
					source.width, source.height, source.rowPadded, dest.rowPadded);

				dest.write(destName.c_str());
			}
			break;
		}
		default:
			break;
		}
	}


	
}
