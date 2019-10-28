#include <Log/LogAuxiliar.h>
#include <stdio.h>

void LogAuxiliar::SetConsoleColor(ConsoleColor Color)
{
#ifdef _WIN32
	#include <windows.h> 
	 HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
	 SetConsoleTextAttribute(Console, Color);
#endif

#ifdef __linux__
	printf("\e[%im", Color);
#endif
}

void LogAuxiliar::ResetConsoleColor()
{
	SetConsoleColor(ConsoleColor::White);
}
