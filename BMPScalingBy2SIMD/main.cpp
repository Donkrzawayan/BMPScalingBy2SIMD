//
#include <iostream>
//#include <Windows.h>
#include <cstring> //strcmp
#include "bmp.hpp"

//typedef int(_fastcall *MyProc1)(int, int);
//typedef int(_fastcall *MyProc1)();

int main(int argc, char *argv[])
{
	std::string filename;
	for (int i = 1; i < argc; ++i) {
		if (!strcmp("-h", argv[i]))
			filename = argv[++i];
	}

	BMP bmp(filename.c_str());

	/*HINSTANCE dllHandle = NULL;
	dllHandle = LoadLibrary(L"DllMasm.dll");
	MyProc1 procedura = (MyProc1)GetProcAddress(dllHandle, "MyProc1");
	int x = 5, y = 7;
	int retVal = procedura(x, y);*/
	/*HINSTANCE dllHandle = NULL;
	dllHandle = LoadLibrary(L"DllC.dll");
	MyProc1 fun = (MyProc1)GetProcAddress(dllHandle, "fun");
	int i = fun();*/

	std::cout << "\nHello World!\n";
}
