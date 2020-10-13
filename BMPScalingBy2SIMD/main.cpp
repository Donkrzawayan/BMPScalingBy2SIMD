//
#include <iostream>
#include <Windows.h>
#include <cstring> //strcmp
#include <sstream>
#include "bmp.hpp"

//typedef int(_fastcall *MyProc1)(int, int);
typedef uint8_t *(_fastcall *MyProc1)(uint8_t *, uint8_t *, int32_t, int32_t);

int main(int argc, char *argv[])
{
	const char *sourceName = "";
	const char *destName = "";
	unsigned thread = 1U;
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

	HINSTANCE dllHandle = NULL;
	dllHandle = LoadLibrary(L"DllMasm.dll");
	MyProc1 procedura = (MyProc1)GetProcAddress(dllHandle, "MyProc1");
	uint8_t * retVal = procedura(dest.data, source.data, source.width, source.height);
	/*HINSTANCE dllHandle = NULL;
	dllHandle = LoadLibrary(L"DllC.dll");
	MyProc1 fun = (MyProc1)GetProcAddress(dllHandle, "fun");
	int i = fun(source.data, source.width, source.height);*/
	dest.write("a.bmp");

	std::cout << "\nHello World!\n";
}
