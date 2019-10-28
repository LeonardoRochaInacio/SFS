#pragma once
#include "LogAuxiliar.h"
#include <stdio.h>


#define DEFAULT_LOG_ARRANGE(Name, Instruction) template<typename... Type> \
static void Name(const char *Mask, Type... Args)\
DEFAULT_LOG_ARRANGE_DECLARE(Name, Instruction)

#define DEFAULT_LOG_ARRANGE_DECLARE(Name, Instruction){\
void (*CategoriePrint)() = [](){printf("%-5s", "["#Name"] ");};\
Instruction;\
}\

class Log
{

public:

	DEFAULT_LOG_ARRANGE(Error,
	{
		LogAuxiliar::SetConsoleColor(ConsoleColor::Red);
		CategoriePrint();
		printf((Mask), Args...);
		printf("\n");
		LogAuxiliar::ResetConsoleColor();
	})

	DEFAULT_LOG_ARRANGE(Warning,
	{
		LogAuxiliar::SetConsoleColor(ConsoleColor::Yellow);
		CategoriePrint();
		printf((Mask), Args...);
		printf("\n");
		LogAuxiliar::ResetConsoleColor();
	})

	DEFAULT_LOG_ARRANGE(Success,
	{
		LogAuxiliar::SetConsoleColor(ConsoleColor::Green);
		CategoriePrint();
		printf((Mask), Args...);
		printf("\n");
		LogAuxiliar::ResetConsoleColor();
	})

	DEFAULT_LOG_ARRANGE(Common,
	{
		CategoriePrint();
		printf((Mask), Args...);
		printf("\n");
	})
	
	DEFAULT_LOG_ARRANGE(Resource,
	{
		LogAuxiliar::SetConsoleColor(ConsoleColor::Blue);
		CategoriePrint();
		printf((Mask), Args...);
		printf("\n");
		LogAuxiliar::ResetConsoleColor();
	})

};
