//
#include <iostream>
#include <Windows.h>

//typedef int(_fastcall *MyProc1)(int, int);
typedef int(_fastcall *MyProc1)();

int main()
{
	/*HINSTANCE dllHandle = NULL;
	dllHandle = LoadLibrary(L"DllMasm.dll");
	MyProc1 procedura = (MyProc1)GetProcAddress(dllHandle, "MyProc1");
	int x = 5, y = 7;
	int retVal = procedura(x, y);*/
	HINSTANCE dllHandle = NULL;
	dllHandle = LoadLibrary(L"DllC.dll");
	MyProc1 fun = (MyProc1)GetProcAddress(dllHandle, "fun");
	int i = fun();

	std::cout << "Hello World!\n" << i;
}
