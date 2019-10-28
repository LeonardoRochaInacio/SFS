#pragma once


enum ConsoleColor
{
#ifdef _WIN32
	Red = 12,
	Blue = 9,
	Grey = 7,
	Green = 10,
	Yellow = 14,
	White = 15,
	Pink = 13
#endif

#ifdef __linux__
	Red = 31,
	Blue = 34,
	Grey = 36,
	Green = 32,
	Yellow = 33,
	White = 97,
	Pink = 35
#endif
};




class LogAuxiliar
{
public:

	static void SetConsoleColor(ConsoleColor Color);
	
	static void ResetConsoleColor();
};